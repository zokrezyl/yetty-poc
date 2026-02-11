#include "python.h"
#include <yetty/yetty-context.h>
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>

#include <yetty/python/engine.h>

#include <pybind11/embed.h>
#include <pybind11/eval.h>

#include <fstream>
#include <sstream>
#include <cstring>
#include <unordered_set>

// GLFW modifier constants
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;

namespace py = pybind11;

// Defined in yetty_card_module.cpp
namespace yetty::python {
void setCardManager(CardManager* mgr);
void setCardTextureManager(CardTextureManager* mgr);
void setGPUContext(const GPUContext* ctx);
void setMetadataSlotIndex(uint32_t idx);
}

namespace yetty::card {

// Scripts whose setup() has already been called (keyed on payload — file path or inline content)
static std::unordered_set<std::string> s_setupDone;

// Helper to set/clear all thread-local pointers around Python callbacks
struct PythonCallContext {
    PythonCallContext(CardManager* mgr, CardTextureManager* texMgr, const GPUContext* gpu, uint32_t slotIndex) {
        python::setCardManager(mgr);
        python::setCardTextureManager(texMgr);
        python::setGPUContext(gpu);
        python::setMetadataSlotIndex(slotIndex);
    }
    ~PythonCallContext() {
        python::setCardManager(nullptr);
        python::setCardTextureManager(nullptr);
        python::setGPUContext(nullptr);
        python::setMetadataSlotIndex(0);
    }
};

//=============================================================================
// Metadata structure (64 bytes, matches texture card shader layout)
//=============================================================================

struct PythonMetadata {
    uint32_t textureDataOffset;
    uint32_t textureWidth;
    uint32_t textureHeight;
    uint32_t atlasX;
    uint32_t atlasY;
    uint32_t widthCells;
    uint32_t heightCells;
    float zoom;
    float centerX;
    float centerY;
    uint32_t flags;
    uint32_t bgColor;
    uint32_t scaledWidth;
    uint32_t scaledHeight;
    uint32_t _reserved[2];
};
static_assert(sizeof(PythonMetadata) == 64, "PythonMetadata must be 64 bytes");

//=============================================================================
// PythonCardImpl
//=============================================================================

class PythonCardImpl : public PythonCard {
public:
    PythonCardImpl(const YettyContext& ctx,
                   int32_t x, int32_t y,
                   uint32_t widthCells, uint32_t heightCells,
                   const std::string& args, const std::string& payload)
        : PythonCard(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _ctx(ctx)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;
        ydebug("PythonCard::ctor: x={} y={} w={} h={} args='{}' payload_size={}",
               x, y, widthCells, heightCells, args, payload.size());
    }

    ~PythonCardImpl() override {
        dispose();
    }

    const char* typeName() const override { return "python"; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }
    bool needsTexture() const override { return true; }

    Result<void> allocateTextures() override {
        _textureHandle = TextureHandle::invalid();
        ydebug("PythonCard::allocateTextures: engineInit={} globals={}",
               _engineInitialized, (bool)_globals);
        if (_engineInitialized && _globals) {
            python::GILGuard gil;
            PythonCallContext pcc(_cardMgr.get(), _cardMgr->textureManager().get(), &_ctx.gpu, metadataSlotIndex());

            if (auto res = callPython("allocate_textures"); !res) {
                yerror("PythonCard::allocateTextures: callPython failed: {}", error_msg(res));
                return Err<void>("PythonCard::allocateTextures: failed", res);
            }
            updateTextureHandleFromScript();
            ydebug("PythonCard::allocateTextures: after call, texHandle={} id={}",
                   _textureHandle.isValid(), _textureHandle.isValid() ? _textureHandle.id : 0);
        }
        _metadataDirty = true;
        return Ok();
    }

    Result<void> writeTextures() override {
        if (_engineInitialized && _globals && _textureHandle.isValid()) {
            python::GILGuard gil;
            PythonCallContext pcc(_cardMgr.get(), _cardMgr->textureManager().get(), &_ctx.gpu, metadataSlotIndex());

            if (auto res = callPython("write_textures"); !res) {
                // Not an error if the callback doesn't exist — Python card may not need textures
                ydebug("PythonCard::writeTextures: no write_textures callback or failed: {}", error_msg(res));
            }
        }
        return Ok();
    }

    void setCellSize(uint32_t cellWidth, uint32_t cellHeight) override {
        if (_cellWidth != cellWidth || _cellHeight != cellHeight) {
            _cellWidth = cellWidth;
            _cellHeight = cellHeight;
            _metadataDirty = true;
        }
    }

    //=========================================================================
    // Event handling
    //=========================================================================

    Result<bool> onEvent(const base::Event& event) override {
        // Handle SetFocus
        if (event.type == base::Event::Type::SetFocus) {
            if (event.setFocus.objectId == id()) {
                if (!_focused) {
                    _focused = true;
                    ydebug("PythonCard::onEvent: focused (id={})", id());
                }
                return Ok(true);
            } else if (_focused) {
                _focused = false;
                ydebug("PythonCard::onEvent: unfocused (id={})", id());
            }
            return Ok(false);
        }

        // CardScroll — dispatched by gpu-screen with card-local coords
        if (event.type == base::Event::Type::CardScroll &&
            event.cardScroll.targetId == id()) {
            firePythonEvent("on_scroll",
                event.cardScroll.x, event.cardScroll.y,
                event.cardScroll.dx, event.cardScroll.dy,
                event.cardScroll.mods);
            return Ok(true);
        }

        // CardMouseDown
        if (event.type == base::Event::Type::CardMouseDown &&
            event.cardMouse.targetId == id()) {
            firePythonEvent("on_mouse_down",
                event.cardMouse.x, event.cardMouse.y, event.cardMouse.button);
            return Ok(true);
        }

        // CardMouseMove
        if (event.type == base::Event::Type::CardMouseMove &&
            event.cardMouse.targetId == id()) {
            firePythonEvent("on_mouse_move",
                event.cardMouse.x, event.cardMouse.y);
            return Ok(true);
        }

        // CardMouseUp
        if (event.type == base::Event::Type::CardMouseUp &&
            event.cardMouse.targetId == id()) {
            firePythonEvent("on_mouse_up",
                event.cardMouse.x, event.cardMouse.y, event.cardMouse.button);
            return Ok(true);
        }

        return Ok(false);
    }

    Result<void> registerForEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("PythonCard::registerForEvents: no EventLoop instance", loopResult);
        }
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();

        if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res) {
            return Err<void>("PythonCard::registerForEvents: failed to register SetFocus", res);
        }
        if (auto res = loop->registerListener(base::Event::Type::CardScroll, self, 1000); !res) {
            return Err<void>("PythonCard::registerForEvents: failed to register CardScroll", res);
        }
        if (auto res = loop->registerListener(base::Event::Type::CardMouseDown, self, 1000); !res) {
            return Err<void>("PythonCard::registerForEvents: failed to register CardMouseDown", res);
        }
        if (auto res = loop->registerListener(base::Event::Type::CardMouseMove, self, 1000); !res) {
            return Err<void>("PythonCard::registerForEvents: failed to register CardMouseMove", res);
        }
        if (auto res = loop->registerListener(base::Event::Type::CardMouseUp, self, 1000); !res) {
            return Err<void>("PythonCard::registerForEvents: failed to register CardMouseUp", res);
        }

        yinfo("PythonCard {} registered for events (priority 1000)", id());
        return Ok();
    }

    //=========================================================================
    // Lifecycle
    //=========================================================================

    Result<void> init() {
        ydebug("PythonCard::init: BEGIN");

        // Initialize Python engine (ref-counted)
        if (auto res = python::engineRef(); !res) {
            return Err<void>("PythonCard::init: engine init failed", res);
        }
        _engineInitialized = true;

        // Allocate metadata slot
        auto metaResult = _cardMgr->allocateMetadata(sizeof(PythonMetadata));
        if (!metaResult) {
            return Err<void>("PythonCard::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;
        ydebug("PythonCard::init: metadata at offset={}", _metaHandle.offset);

        // Load source code
        if (auto res = loadSource(); !res) {
            return Err<void>("PythonCard::init: failed to load source", res);
        }

        // Create isolated Python namespace and execute the script
        {
            python::GILGuard gil;
            PythonCallContext pcc(_cardMgr.get(), _cardMgr->textureManager().get(), &_ctx.gpu, metadataSlotIndex());

            // Create per-card isolated globals dict
            _globals = py::dict();

            // Copy builtins so import works
            auto mainModule = py::module_::import("__main__");
            _globals["__builtins__"] = mainModule.attr("__builtins__");
            _globals["__name__"] = py::str("__yetty_card__");

            // Execute the script in isolated namespace
            try {
                py::exec(_source, _globals);
            } catch (const py::error_already_set& e) {
                yerror("PythonCard::init: script execution failed: {}", e.what());
                return Err<void>("PythonCard::init: script execution failed");
            }

            // Call setup() once per unique script (install packages, one-time init)
            if (_globals.contains("setup") && !s_setupDone.contains(_scriptKey)) {
                yinfo("PythonCard::init: calling setup() for '{}'", _scriptKey);
                if (auto res = callPython("setup"); !res) {
                    return Err<void>("PythonCard::init: setup() failed", res);
                }
                s_setupDone.insert(_scriptKey);
            }

            // Call init(width_cells, height_cells)
            if (auto res = callPython("init", py::make_tuple(_widthCells, _heightCells)); !res) {
                return Err<void>("PythonCard::init: init() failed", res);
            }
        }

        // Metadata will be uploaded in update() after createAtlas() assigns atlas positions
        _metadataDirty = true;

        // Register for mouse/scroll events
        if (auto res = registerForEvents(); !res) {
            ywarn("PythonCard::init: event registration failed: {}", error_msg(res));
            // Non-fatal — card works without events
        }

        ydebug("PythonCard::init: DONE glyph=0x{:X} metaSlot={}", _shaderGlyph, metadataSlotIndex());
        return Ok();
    }

    Result<void> dispose() override {
        if (_disposed) return Ok();
        _disposed = true;

        // Call Python shutdown()
        if (_engineInitialized && _globals) {
            python::GILGuard gil;
            PythonCallContext pcc(_cardMgr.get(), _cardMgr->textureManager().get(), &_ctx.gpu, metadataSlotIndex());

            if (auto res = callPython("shutdown"); !res) {
                ywarn("PythonCard::dispose: shutdown() failed: {}", error_msg(res));
            }

            _globals = py::object();  // Release reference
        }

        if (_metaHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateMetadata(_metaHandle);
            _metaHandle = MetadataHandle::invalid();
        }

        // Unref engine
        if (_engineInitialized) {
            python::engineUnref();
            _engineInitialized = false;
        }

        return Ok();
    }

    void suspend() override {
        // Call Python suspend()
        if (_engineInitialized && _globals) {
            python::GILGuard gil;
            PythonCallContext pcc(_cardMgr.get(), _cardMgr->textureManager().get(), &_ctx.gpu, metadataSlotIndex());

            if (auto res = callPython("suspend"); !res) {
                ywarn("PythonCard::suspend: suspend() failed: {}", error_msg(res));
            }
        }

        _needsRerender = true;
        yinfo("PythonCard::suspend: deallocated texture handle");
    }

    void renderToStaging(float time) override {
        ydebug("PythonCard::renderToStaging: time={:.3f} engineInit={} globals={} needsRerender={}",
               time, _engineInitialized, (bool)_globals, _needsRerender);

        // Reconstruct after suspend — call render to re-populate buffers
        if (_needsRerender) {
            _needsRerender = false;
        }

        // Call Python render(time) — script populates staging buffers/textures
        if (_engineInitialized && _globals) {
            python::GILGuard gil;
            PythonCallContext pcc(_cardMgr.get(), _cardMgr->textureManager().get(), &_ctx.gpu, metadataSlotIndex());

            ydebug("PythonCard::renderToStaging: calling render()...");
            if (auto res = callPython("render", py::make_tuple(time)); !res) {
                yerror("PythonCard::renderToStaging: render() failed: {}", error_msg(res));
            }
            ydebug("PythonCard::renderToStaging: render() returned ok, texHandle valid={}", _textureHandle.isValid());

            // Check if script allocated texture data we need to track
            updateTextureHandleFromScript();
        }
    }

    Result<void> render() override {
        if (_metadataDirty) {
            ydebug("PythonCard::render: uploading metadata");
            if (auto res = uploadMetadata(); !res) {
                return Err<void>("PythonCard::render: metadata upload failed", res);
            }
            _metadataDirty = false;
        }

        return Ok();
    }

private:
    //=========================================================================
    // Python event forwarding
    //=========================================================================

    template<typename... Args>
    void firePythonEvent(const char* name, Args&&... args) {
        if (!_engineInitialized || !_globals) {
            return;
        }
        try {
            python::GILGuard gil;
            PythonCallContext pcc(_cardMgr.get(), _cardMgr->textureManager().get(), &_ctx.gpu, metadataSlotIndex());
            if (!_globals.contains(name)) return;
            _globals[name](std::forward<Args>(args)...);
        } catch (const py::error_already_set& e) {
            ywarn("PythonCard::firePythonEvent: {}() threw: {}", name, e.what());
        }
    }

    //=========================================================================
    // Python Result decoding
    //=========================================================================

    /// Call a Python function and check if it returns a Result with is_err.
    /// Returns C++ Err if the Python function returned Err or threw an exception.
    /// If the function returns None or Ok, returns C++ Ok.
    Result<void> callPython(const char* name, py::args args = py::args(py::tuple())) {
        if (!_globals.contains(name)) {
            ydebug("PythonCard::callPython: '{}' not found in globals, skipping", name);
            return Ok();
        }
        try {
            ydebug("PythonCard::callPython: calling '{}'", name);
            py::object ret = _globals[name](*args);
            ydebug("PythonCard::callPython: '{}' returned, is_none={}", name, ret.is_none());

            // Check if return value is a Result (has is_err attribute)
            if (!ret.is_none() && py::hasattr(ret, "is_err")) {
                bool isErr = ret.attr("is_err").cast<bool>();
                ydebug("PythonCard::callPython: '{}' returned Result, is_err={}", name, isErr);
                if (isErr) {
                    // Extract error message
                    std::string msg;
                    try {
                        auto err = ret.attr("error");
                        msg = err.attr("message").cast<std::string>();
                    } catch (...) {
                        msg = py::str(ret).cast<std::string>();
                    }
                    yerror("PythonCard: {}() returned Err: {}", name, msg);
                    return Err<void>(std::string("PythonCard::") + name + "(): " + msg);
                }
            }
            return Ok();
        } catch (const py::error_already_set& e) {
            yerror("PythonCard: {}() threw: {}", name, e.what());
            return Err<void>(std::string("PythonCard::") + name + "() threw: " + e.what());
        }
    }

    //=========================================================================
    // Source loading
    //=========================================================================

    Result<void> loadSource() {
        if (_payloadStr.empty()) {
            return Err<void>("PythonCard::loadSource: empty payload");
        }

        // Store key for setup() dedup (file path or inline content)
        _scriptKey = _payloadStr;

        // Check for "inline:" prefix
        if (_payloadStr.compare(0, 7, "inline:") == 0) {
            _source = _payloadStr.substr(7);
            yinfo("PythonCard: inline source ({} bytes)", _source.size());
        } else {
            // Treat as file path
            std::ifstream file(_payloadStr);
            if (!file.is_open()) {
                return Err<void>("PythonCard::loadSource: failed to open: " + _payloadStr);
            }
            std::ostringstream ss;
            ss << file.rdbuf();
            _source = ss.str();
            yinfo("PythonCard: loaded script from {}", _payloadStr);
        }

        // Free payload now that source is loaded
        _payloadStr.clear();
        _payloadStr.shrink_to_fit();

        return Ok();
    }

    //=========================================================================
    // Track texture data handle from script globals
    //=========================================================================

    void updateTextureHandleFromScript() {
        // Find the script's texture handle by scanning globals for a PyTextureHandle.
        // The handle has 'valid', 'id', 'width', 'height' attributes.
        try {
            // Look for _tex (convention) or _yetty_texture (legacy)
            py::object texObj;
            if (_globals.contains("_tex")) {
                texObj = _globals["_tex"];
            } else if (_globals.contains("_yetty_texture")) {
                texObj = _globals["_yetty_texture"];
            }

            if (!texObj.is_none() && py::hasattr(texObj, "valid") && py::hasattr(texObj, "id")) {
                bool valid = texObj.attr("valid").cast<bool>();
                if (valid) {
                    uint32_t id = texObj.attr("id").cast<uint32_t>();
                    if (!_textureHandle.isValid() || _textureHandle.id != id) {
                        _textureHandle.id = id;
                        _metadataDirty = true;
                    }
                    // Read actual texture dimensions from the Python handle
                    if (py::hasattr(texObj, "width") && py::hasattr(texObj, "height")) {
                        _texWidth = texObj.attr("width").cast<uint32_t>();
                        _texHeight = texObj.attr("height").cast<uint32_t>();
                    }
                    ydebug("PythonCard::updateTextureHandleFromScript: id={} {}x{}", id, _texWidth, _texHeight);
                }
            }
        } catch (...) {
            // Not all scripts expose _yetty_texture, that's fine
        }
    }

    //=========================================================================
    // Metadata upload
    //=========================================================================

    Result<void> uploadMetadata() {
        if (!_metaHandle.isValid()) {
            return Err<void>("PythonCard::uploadMetadata: invalid handle");
        }

        // Use actual texture dimensions if available, else compute from cells
        uint32_t renderWidth = _texWidth;
        uint32_t renderHeight = _texHeight;
        if (renderWidth == 0 || renderHeight == 0) {
            renderWidth = _widthCells * _cellWidth;
            renderHeight = _heightCells * _cellHeight;
        }
        if (renderWidth == 0) renderWidth = 256;
        if (renderHeight == 0) renderHeight = 256;

        ydebug("PythonCard::uploadMetadata: renderW={} renderH={} cellW={} cellH={} wCells={} hCells={} texHandle valid={}",
               renderWidth, renderHeight, _cellWidth, _cellHeight, _widthCells, _heightCells, _textureHandle.isValid());

        PythonMetadata meta = {};
        meta.textureDataOffset = 0;  // No longer used
        meta.textureWidth = renderWidth;
        meta.textureHeight = renderHeight;

        if (_textureHandle.isValid()) {
            auto pos = _cardMgr->textureManager()->getAtlasPosition(_textureHandle);
            meta.atlasX = pos.x;
            meta.atlasY = pos.y;
        } else {
            meta.atlasX = 0;
            meta.atlasY = 0;
        }
        meta.widthCells = _widthCells;
        meta.heightCells = _heightCells;
        meta.zoom = 1.0f;
        meta.centerX = 0.5f;
        meta.centerY = 0.5f;
        meta.flags = 0;
        meta.bgColor = 0xFF000000;
        meta.scaledWidth = renderWidth;
        meta.scaledHeight = renderHeight;

        if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
            return Err<void>("PythonCard::uploadMetadata: write failed", res);
        }

        return Ok();
    }

    //=========================================================================
    // Members
    //=========================================================================

    const YettyContext& _ctx;
    std::string _argsStr;
    std::string _payloadStr;
    std::string _source;
    std::string _scriptKey;  // Original payload, used to dedup setup() calls

    // Per-card isolated Python namespace
    py::object _globals;

    // Cell dimensions
    uint32_t _cellWidth = 10;
    uint32_t _cellHeight = 20;

    // Texture handle for atlas packing
    TextureHandle _textureHandle = TextureHandle::invalid();
    uint32_t _texWidth = 0;
    uint32_t _texHeight = 0;

    // State
    bool _needsRerender = false;
    bool _metadataDirty = true;
    bool _engineInitialized = false;
    bool _disposed = false;
    bool _focused = false;
};

//=============================================================================
// Factory
//=============================================================================

Result<PythonCard::Ptr> PythonCard::createImpl(
    ContextType& ctx,
    const YettyContext& yettyCtx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)ctx;

    ydebug("PythonCard::createImpl: x={} y={} w={} h={} args='{}' payload_size={}",
           x, y, widthCells, heightCells, args, payload.size());

    if (!yettyCtx.cardManager) {
        return Err<Ptr>("PythonCard::createImpl: null CardBufferManager");
    }
    if (false) { // cardManager always valid
        return Err<Ptr>("PythonCard::createImpl: null CardTextureManager");
    }

    auto card = std::make_shared<PythonCardImpl>(
        yettyCtx, x, y, widthCells, heightCells, args, payload);

    if (auto res = card->init(); !res) {
        yerror("PythonCard::createImpl: init failed: {}", error_msg(res));
        return Err<Ptr>("PythonCard::createImpl: init failed", res);
    }

    ydebug("PythonCard::createImpl: SUCCESS glyph=0x{:X} metaOffset={}",
           card->shaderGlyph(), card->metadataOffset());
    return Ok<Ptr>(card);
}

}  // namespace yetty::card
