#include "diagram.h"
#include <yetty/ydraw-builder.h>
#include "../../diagram/graph-ir.h"
#include "../../diagram/mermaid-parser.h"
#include "../../diagram/sugiyama-layout.h"
#include "../../diagram/diagram-renderer.h"
#include "../hdraw/hdraw.h"
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
    {
        _shaderGlyph = SHADER_GLYPH;
        auto builderRes = YDrawBuilder::create(ctx.fontManager, ctx.globalAllocator);
        if (builderRes) {
            _builder = *builderRes;
        } else {
            yerror("DiagramImpl: failed to create builder");
        }
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
        _derivedStorage = StorageHandle::invalid();
        _grid = nullptr;
        _gridSize = 0;
        _primStorage = StorageHandle::invalid();
        _primitives = nullptr;
        _primCount = 0;
        _primCapacity = 0;
        if (_metaHandle.isValid() && _cardMgr) {
            if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
                yerror("DiagramImpl::dispose: deallocateMetadata failed: {}", error_msg(res));
            }
            _metaHandle = MetadataHandle::invalid();
        }
        return Ok();
    }

    void suspend() override {
        if (_primStorage.isValid() && _primCount > 0 && _builder) {
            auto& staging = _builder->primStagingMut();
            staging.resize(_primCount);
            std::memcpy(staging.data(), _primitives, _primCount * sizeof(SDFPrimitive));
        }
        _derivedStorage = StorageHandle::invalid();
        _grid = nullptr;
        _gridSize = 0;
        _primStorage = StorageHandle::invalid();
        _primitives = nullptr;
        _primCount = 0;
        _primCapacity = 0;
    }

    void declareBufferNeeds() override {
        if (!_builder) return;

        // Save GPU prims back to builder staging before dealloc
        if (_primStorage.isValid() && _primCount > 0) {
            auto& staging = _builder->primStagingMut();
            if (staging.empty()) {
                staging.resize(_primCount);
                std::memcpy(staging.data(), _primitives, _primCount * sizeof(SDFPrimitive));
            }
        }
        _derivedStorage = StorageHandle::invalid();
        _grid = nullptr;
        _gridSize = 0;
        _primStorage = StorageHandle::invalid();
        _primitives = nullptr;
        _primCount = 0;
        _primCapacity = 0;

        const auto& primStaging = _builder->primStaging();
        const auto& glyphs = _builder->glyphs();

        if (primStaging.empty() && glyphs.empty()) {
            _builder->clearGridStaging();
            return;
        }

        // Reserve prim storage
        if (!primStaging.empty()) {
            uint32_t primSize = static_cast<uint32_t>(primStaging.size()) * sizeof(SDFPrimitive);
            _cardMgr->bufferManager()->reserve(primSize);
        }

        // Calculate grid
        if (_builder->gridStaging().empty()) {
            _builder->calculate();
        }

        // Reserve derived size (grid + glyphs)
        uint32_t gridBytes = static_cast<uint32_t>(_builder->gridStaging().size()) * sizeof(uint32_t);
        uint32_t glyphBytes = static_cast<uint32_t>(glyphs.size() * sizeof(YDrawGlyph));
        uint32_t derivedSize = gridBytes + glyphBytes;

        if (derivedSize > 0) {
            _cardMgr->bufferManager()->reserve(derivedSize);
        }
    }

    Result<void> allocateBuffers() override {
        if (!_builder) return Ok();

        const auto& primStaging = _builder->primStaging();
        const auto& gridStaging = _builder->gridStaging();
        const auto& glyphs = _builder->glyphs();

        // Allocate primitives
        if (!primStaging.empty()) {
            uint32_t count = static_cast<uint32_t>(primStaging.size());
            uint32_t allocBytes = count * sizeof(SDFPrimitive);
            auto primResult = _cardMgr->bufferManager()->allocateBuffer(
                metadataSlotIndex(), "prims", allocBytes);
            if (!primResult) {
                return Err<void>("DiagramImpl::allocateBuffers: prim alloc failed");
            }
            _primStorage = *primResult;
            _primitives = reinterpret_cast<SDFPrimitive*>(_primStorage.data);
            _primCapacity = count;
            _primCount = count;
            std::memcpy(_primitives, primStaging.data(), count * sizeof(SDFPrimitive));
            _builder->primStagingMut().clear();
            _builder->primStagingMut().shrink_to_fit();
            _cardMgr->bufferManager()->markBufferDirty(_primStorage);
        }

        // Allocate derived storage (grid + glyphs)
        uint32_t gridBytes = static_cast<uint32_t>(gridStaging.size()) * sizeof(uint32_t);
        uint32_t glyphBytes = static_cast<uint32_t>(glyphs.size() * sizeof(YDrawGlyph));
        uint32_t derivedSize = gridBytes + glyphBytes;

        if (derivedSize > 0) {
            auto storageResult = _cardMgr->bufferManager()->allocateBuffer(
                metadataSlotIndex(), "derived", derivedSize);
            if (!storageResult) {
                return Err<void>("DiagramImpl::allocateBuffers: derived alloc failed");
            }
            _derivedStorage = *storageResult;

            uint8_t* base = _derivedStorage.data;
            uint32_t offset = 0;

            _grid = reinterpret_cast<uint32_t*>(base + offset);
            _gridSize = static_cast<uint32_t>(gridStaging.size());
            _gridOffset = (_derivedStorage.offset + offset) / sizeof(float);
            if (!gridStaging.empty()) {
                std::memcpy(base + offset, gridStaging.data(), gridBytes);
            }
            offset += gridBytes;

            _glyphOffset = (_derivedStorage.offset + offset) / sizeof(float);
            if (!glyphs.empty()) {
                std::memcpy(base + offset, glyphs.data(), glyphBytes);
            }

            _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
        }

        _primitiveOffset = _primStorage.isValid() ? _primStorage.offset / sizeof(float) : 0;
        _gridWidth = _builder->gridWidth();
        _gridHeight = _builder->gridHeight();
        _metadataDirty = true;

        return Ok();
    }

    bool needsBufferRealloc() override {
        if (!_builder) return false;
        if (!_primStorage.isValid() && !_builder->primStaging().empty()) return true;
        return false;
    }

    Result<void> allocateTextures() override {
        return Ok();
    }

    Result<void> writeTextures() override {
        return Ok();
    }

    Result<void> render() override {
        if (!_builder) return Ok();

        if (_metadataDirty) {
            if (auto res = uploadMetadata(); !res) return res;
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

        // Layout the graph
        if (auto res = layout->layout(graph, _builder.get()); !res) {
            return Err<void>("Layout failed", res);
        }

        // Create renderer
        auto rendererResult = diagram::DiagramRenderer::create(_builder);
        if (!rendererResult) {
            return Err<void>("Failed to create renderer");
        }

        // Render to builder
        if (auto res = (*rendererResult)->render(graph); !res) {
            return Err<void>("Render failed", res);
        }

        // Set scene bounds
        _builder->setSceneBounds(graph.minX, graph.minY, graph.maxX, graph.maxY);
        _builder->setBgColor(0xFF2E1A1A);

        return Ok();
    }

    Result<void> uploadMetadata() {
        if (!_metaHandle.isValid()) {
            return Err<void>("DiagramImpl::uploadMetadata: invalid handle");
        }

        // Pack zoom as f16
        uint32_t zoomBits;
        {
            uint32_t f32bits;
            std::memcpy(&f32bits, &_viewZoom, sizeof(float));
            uint32_t sign = (f32bits >> 16) & 0x8000;
            int32_t  exp  = ((f32bits >> 23) & 0xFF) - 127 + 15;
            uint32_t mant = (f32bits >> 13) & 0x3FF;
            if (exp <= 0) { exp = 0; mant = 0; }
            else if (exp >= 31) { exp = 31; mant = 0; }
            zoomBits = sign | (static_cast<uint32_t>(exp) << 10) | mant;
        }

        float sceneMinX = _builder->sceneMinX();
        float sceneMinY = _builder->sceneMinY();
        float sceneMaxX = _builder->sceneMaxX();
        float sceneMaxY = _builder->sceneMaxY();
        float contentW = sceneMaxX - sceneMinX;
        float contentH = sceneMaxY - sceneMinY;

        int16_t panXi16 = static_cast<int16_t>(std::clamp(
            _viewPanX / std::max(contentW, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));
        int16_t panYi16 = static_cast<int16_t>(std::clamp(
            _viewPanY / std::max(contentH, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));

        float cellSize = _builder->cellSize();

        YDrawMetadata meta = {};
        meta.primitiveOffset = _primitiveOffset;
        meta.primitiveCount = _primCount;
        meta.gridOffset = _gridOffset;
        meta.gridWidth = _gridWidth;
        meta.gridHeight = _gridHeight;
        std::memcpy(&meta.cellSize, &cellSize, sizeof(float));
        meta.glyphOffset = _glyphOffset;
        meta.glyphCount = static_cast<uint32_t>(_builder->glyphs().size());
        std::memcpy(&meta.sceneMinX, &sceneMinX, sizeof(float));
        std::memcpy(&meta.sceneMinY, &sceneMinY, sizeof(float));
        std::memcpy(&meta.sceneMaxX, &sceneMaxX, sizeof(float));
        std::memcpy(&meta.sceneMaxY, &sceneMaxY, sizeof(float));
        meta.widthCells  = (_widthCells & 0xFFFF) |
                           (static_cast<uint32_t>(static_cast<uint16_t>(panXi16)) << 16);
        meta.heightCells = (_heightCells & 0xFFFF) |
                           (static_cast<uint32_t>(static_cast<uint16_t>(panYi16)) << 16);
        meta.flags = (_builder->flags() & 0xFFFF) | (zoomBits << 16);
        meta.bgColor = _builder->bgColor();

        if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
            return Err<void>("DiagramImpl::uploadMetadata: write failed");
        }
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

    YDrawBuilder::Ptr _builder;
    base::ObjectId _screenId = 0;
    std::string _argsStr;
    std::string _payloadStr;
    std::string _inputFile;

    // GPU
    StorageHandle _primStorage = StorageHandle::invalid();
    SDFPrimitive* _primitives = nullptr;
    uint32_t _primCount = 0;
    uint32_t _primCapacity = 0;
    StorageHandle _derivedStorage = StorageHandle::invalid();
    uint32_t* _grid = nullptr;
    uint32_t _gridSize = 0;
    uint32_t _primitiveOffset = 0;
    uint32_t _gridOffset = 0;
    uint32_t _glyphOffset = 0;
    uint32_t _gridWidth = 0;
    uint32_t _gridHeight = 0;
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
