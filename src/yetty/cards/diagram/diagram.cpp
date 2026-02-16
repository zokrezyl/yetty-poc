#include "diagram.h"
#include <yetty/ydraw-builder.h>
#include "../../ydraw/ydraw-buffer.h"
#include "../../diagram/graph-ir.h"
#include "../../diagram/mermaid-parser.h"
#include "../../diagram/sugiyama-layout.h"
#include "../../diagram/diagram-renderer.h"
#include "../../ydraw/ydraw-types.gen.h"
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cmath>

namespace {
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;
}

namespace yetty::card {

//=============================================================================
// DiagramImpl - Full implementation
//=============================================================================

class DiagramImpl : public Diagram {
public:
    DiagramImpl(const YettyContext& ctx,
                int32_t x, int32_t y,
                uint32_t widthCells, uint32_t heightCells,
                const std::string& args, const std::string& payload)
        : Diagram(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _screenId(ctx.screenId)
        , _argsStr(args)
        , _payloadStr(payload)
        , _fontManager(ctx.fontManager)
        , _gpuAllocator(ctx.gpuAllocator)
    {
        _shaderGlyph = SHADER_GLYPH;
        _buffer = *yetty::YDrawBuffer::create();
    }

    ~DiagramImpl() override { dispose(); }

    //=========================================================================
    // Card lifecycle
    //=========================================================================

    bool needsTexture() const override {
        return _builder && _builder->hasCustomAtlas();
    }

    uint32_t metadataSlotIndex() const override {
        return _metaHandle.offset / 64;
    }

    Result<void> dispose() override {
        deregisterFromEvents();
        if (_builder) _builder->releaseBuffers();
        if (_metaHandle.isValid() && _cardMgr) {
            if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
                yerror("DiagramImpl::dispose: deallocateMetadata failed: {}", error_msg(res));
            }
            _metaHandle = MetadataHandle::invalid();
        }
        return Ok();
    }

    void suspend() override {
        if (_builder) _builder->releaseBuffers();
    }

    void declareBufferNeeds() override {
        if (!_builder) return;
        if (auto res = _builder->declareBufferNeeds(); !res) {
            yerror("DiagramImpl::declareBufferNeeds: {}", error_msg(res));
        }
    }

    Result<void> allocateBuffers() override {
        if (!_builder) return Ok();
        return _builder->allocateBuffers();
    }

    bool needsBufferRealloc() override {
        if (!_builder) return false;
        return _builder->needsBufferRealloc();
    }

    Result<void> allocateTextures() override {
        if (!_builder) return Ok();
        return _builder->allocateTextures();
    }

    Result<void> writeTextures() override {
        if (!_builder) return Ok();
        return _builder->writeTextures();
    }

    Result<void> finalize() override {
        if (!_builder) return Ok();
        if (auto res = _builder->writeBuffers(); !res) return res;
        if (_metadataDirty) {
            if (auto res = _builder->writeMetadata(_widthCells, _heightCells,
                                                    _viewZoom, _viewPanX, _viewPanY); !res) return res;
            _metadataDirty = false;
        }
        return Ok();
    }

    //=========================================================================
    // Events - zoom/pan
    //=========================================================================

    Result<bool> onEvent(const base::Event& event) override {
        if (event.type == base::Event::Type::SetFocus) {
            if (event.setFocus.objectId == id()) {
                _focused = true;
                return Ok(true);
            } else if (_focused) {
                _focused = false;
            }
            return Ok(false);
        }

        if (event.type == base::Event::Type::CardScroll &&
            event.cardScroll.targetId == id()) {
            float sceneW = _builder->sceneMaxX() - _builder->sceneMinX();
            float sceneH = _builder->sceneMaxY() - _builder->sceneMinY();

            if (event.cardScroll.mods & GLFW_MOD_CONTROL) {
                float zoomDelta = event.cardScroll.dy * 0.1f;
                float newZoom = std::clamp(_viewZoom + zoomDelta, 0.1f, 20.0f);
                if (newZoom != _viewZoom) {
                    _viewZoom = newZoom;
                    _metadataDirty = true;
                }
                return Ok(true);
            } else if (event.cardScroll.mods & GLFW_MOD_SHIFT) {
                _viewPanX += event.cardScroll.dy * 0.05f * sceneW / _viewZoom;
                _metadataDirty = true;
                return Ok(true);
            } else {
                _viewPanY += event.cardScroll.dy * 0.05f * sceneH / _viewZoom;
                _metadataDirty = true;
                return Ok(true);
            }
        }

        return Ok(false);
    }

    //=========================================================================
    // Init
    //=========================================================================

    Result<void> init() {
        auto metaResult = _cardMgr->allocateMetadata(sizeof(YDrawMetadata));
        if (!metaResult) {
            return Err<void>("DiagramImpl::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        auto builderRes = YDrawBuilder::create(
            _fontManager, _gpuAllocator, _buffer, _cardMgr, metadataSlotIndex());
        if (!builderRes) {
            return Err<void>("DiagramImpl::init: failed to create builder", builderRes);
        }
        _builder = *builderRes;

        if (auto res = registerForEvents(); !res) {
            ywarn("DiagramImpl::init: event registration failed: {}", error_msg(res));
        }

        parseArgs(_argsStr);

        auto inputResult = loadInput();
        if (!inputResult) {
            yerror("DiagramImpl::init: failed to load input: {}", error_msg(inputResult));
            return Err<void>("Failed to load input", inputResult);
        }

        const std::string& input = *inputResult;
        if (!input.empty()) {
            if (auto res = parseDiagram(input); !res) {
                yerror("DiagramImpl::init: diagram parse failed: {}", error_msg(res));
                return res;
            }
        }

        yinfo("DiagramImpl::init: {} prims, {} glyphs",
              _builder->primitiveCount(), _builder->glyphCount());

        _metadataDirty = true;
        return Ok();
    }

private:
    //=========================================================================
    // Diagram parsing and rendering
    //=========================================================================

    Result<void> parseDiagram(const std::string& payload) {
        // Try Mermaid parser first
        auto mermaidParser = diagram::MermaidParser::create();
        if (!mermaidParser) {
            return Err<void>("Failed to create Mermaid parser");
        }

        if ((*mermaidParser)->canParse(payload)) {
            auto graphResult = (*mermaidParser)->parse(payload);
            if (!graphResult) {
                return Err<void>("Mermaid parse failed", graphResult);
            }
            return renderGraph(*graphResult);
        }

        // TODO: Try DOT parser

        return Err<void>("Unknown diagram format");
    }

    Result<void> renderGraph(diagram::Graph& graph) {
        // Create layout engine
        auto layoutResult = diagram::SugiyamaLayout::create();
        if (!layoutResult) {
            return Err<void>("Failed to create layout engine");
        }

        auto layout = *layoutResult;
        layout->setParams({
            .nodeSpacingX = 60.0f,
            .nodeSpacingY = 80.0f,
            .nodePaddingX = 15.0f,
            .nodePaddingY = 10.0f
        });

        // Measurement function using builder's atlas font
        diagram::MeasureTextFn measureText = [this](const std::string& text, float fontSize) -> float {
            return fontSize * 0.6f * static_cast<float>(text.size());
        };

        // Layout the graph
        if (auto res = layout->layout(graph, measureText); !res) {
            return Err<void>("Layout failed", res);
        }

        // Create renderer
        auto rendererResult = diagram::DiagramRenderer::create(_builder, _buffer, measureText);
        if (!rendererResult) {
            return Err<void>("Failed to create renderer");
        }

        // Render to buffer
        if (auto res = (*rendererResult)->render(graph); !res) {
            return Err<void>("Render failed", res);
        }

        // Set scene bounds on buffer
        _buffer->setSceneBounds(graph.minX, graph.minY, graph.maxX, graph.maxY);
        _buffer->setBgColor(0xFF2E1A1A);

        return Ok();
    }

    //=========================================================================
    // Events
    //=========================================================================

    Result<void> registerForEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return Err<void>("no EventLoop", loopResult);
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();
        if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res) return res;
        if (auto res = loop->registerListener(base::Event::Type::CardScroll, self, 1000); !res) return res;
        return Ok();
    }

    Result<void> deregisterFromEvents() {
        if (weak_from_this().expired()) return Ok();
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return Ok();
        (*loopResult)->deregisterListener(sharedAs<base::EventListener>());
        return Ok();
    }

    //=========================================================================
    // Parsing
    //=========================================================================

    void parseArgs(const std::string& args) {
        std::istringstream iss(args);
        std::string token;
        while (iss >> token) {
            if (token == "-i" || token == "--input") {
                if (iss >> _inputFile) {
                    // _inputFile is set
                }
            } else if (token == "--bg-color") {
                std::string colorStr;
                if (iss >> colorStr) {
                    if (colorStr.substr(0, 2) == "0x" || colorStr.substr(0, 2) == "0X")
                        colorStr = colorStr.substr(2);
                    _builder->setBgColor(static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16)));
                }
            } else if (token == "--show-bounds") {
                _builder->addFlags(YDrawBuilder::FLAG_SHOW_BOUNDS);
            } else if (token == "--show-grid") {
                _builder->addFlags(YDrawBuilder::FLAG_SHOW_GRID);
            }
        }
    }

    Result<std::string> loadInput() {
        if (_inputFile.empty() || _inputFile == "-") {
            // Use payload
            return Ok(_payloadStr);
        }
        // Read from file
        std::ifstream file(_inputFile);
        if (!file) {
            return Err<std::string>("Failed to open file: " + _inputFile);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return Ok(buffer.str());
    }

    //=========================================================================
    // State
    //=========================================================================

    yetty::YDrawBuffer::Ptr _buffer;
    YDrawBuilder::Ptr _builder;
    FontManager::Ptr _fontManager;
    GpuAllocator::Ptr _gpuAllocator;
    base::ObjectId _screenId = 0;
    std::string _argsStr;
    std::string _payloadStr;
    std::string _inputFile;
    bool _metadataDirty = true;

    // View
    float _viewZoom = 1.0f;
    float _viewPanX = 0.0f;
    float _viewPanY = 0.0f;
    bool _focused = false;
};

//=============================================================================
// Factory
//=============================================================================

Result<CardPtr> Diagram::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    if (!ctx.cardManager) {
        return Err<CardPtr>("Diagram::create: null CardManager");
    }
    auto card = std::make_shared<DiagramImpl>(ctx, x, y, widthCells, heightCells, args, payload);
    if (auto res = card->init(); !res) {
        return Err<CardPtr>("Diagram::create: init failed", res);
    }
    return Ok<CardPtr>(card);
}

Result<Diagram::Ptr> Diagram::createImpl(
    ContextType& ctx,
    const YettyContext& yettyCtx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)ctx;
    auto result = create(yettyCtx, x, y, widthCells, heightCells, args, payload);
    if (!result) return Err<Ptr>("Failed to create Diagram", result);
    auto diagram = std::dynamic_pointer_cast<Diagram>(*result);
    if (!diagram) return Err<Ptr>("Created card is not a Diagram");
    return Ok(diagram);
}

} // namespace yetty::card
