#include "ywasm.h"

#include <wasm3.h>
#include <yetty/ydraw-builder.h>
#include "../../ydraw/ydraw-buffer.h"
#include <yetty/yetty-context.h>
#include <ytrace/ytrace.hpp>

#include <cstring>
#include <vector>

namespace yetty::card {

//=============================================================================
// YWasmImpl - Implementation of YWasm card using ydraw rendering
//=============================================================================

class YWasmImpl : public YWasm {
public:
    YWasmImpl(const YettyContext& ctx,
              int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells,
              const std::string& args, const std::string& payload)
        : YWasm(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _fontManager(ctx.fontManager)
        , _gpuAllocator(ctx.gpuAllocator)
        , _argsStr(args)
        , _wasmBytes(payload.begin(), payload.end())
    {
        _shaderGlyph = SHADER_GLYPH;
        _buffer = *YDrawBuffer::create();
    }

    ~YWasmImpl() override {
        dispose();
    }

    const char* typeName() const override { return "ywasm"; }

    //=========================================================================
    // Card interface - delegate to YDrawBuilder
    //=========================================================================

    bool needsTexture() const override {
        return _builder && _builder->hasCustomAtlas();
    }

    bool needsBufferRealloc() override {
        return _builder && _builder->needsBufferRealloc();
    }

    bool needsTextureRealloc() override {
        return _builder && _builder->needsTextureRealloc();
    }

    void declareBufferNeeds() override {
        if (!_builder) return;
        if (auto res = _builder->declareBufferNeeds(); !res) {
            yerror("YWasm::declareBufferNeeds: {}", error_msg(res));
        }
    }

    Result<void> allocateBuffers() override {
        if (!_builder) return Ok();
        return _builder->allocateBuffers();
    }

    Result<void> allocateTextures() override {
        if (!_builder) return Ok();
        return _builder->allocateTextures();
    }

    Result<void> writeTextures() override {
        if (!_builder) return Ok();
        return _builder->writeTextures();
    }

    Result<void> init() {
        // Allocate metadata slot
        auto metaResult = _cardMgr->allocateMetadata(sizeof(YDrawMetadata));
        if (!metaResult) {
            return Err<void>("YWasm::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;
        ydebug("YWasm::init: allocated metadata at offset {}", _metaHandle.offset);

        // Create ydraw builder
        auto builderRes = YDrawBuilder::create(
            _fontManager, _gpuAllocator, _buffer, _cardMgr, metadataSlotIndex());
        if (!builderRes) {
            return Err<void>("YWasm::init: failed to create builder", builderRes);
        }
        _builder = *builderRes;
        _builder->addFlags(YDrawBuilder::FLAG_UNIFORM_SCALE);

        // Parse args
        parseArgs(_argsStr);

        // Initialize wasm3 runtime
        if (auto res = initWasmRuntime(); !res) {
            return Err<void>("YWasm::init: failed to init wasm runtime", res);
        }

        _builder->setViewport(_widthCells, _heightCells);
        _builder->setView(_viewZoom, _viewPanX, _viewPanY);
        _dirty = true;
        return Ok();
    }

    Result<void> dispose() override {
        // Free wasm3 resources
        if (_runtime) {
            m3_FreeRuntime(_runtime);
            _runtime = nullptr;
        }
        if (_env) {
            m3_FreeEnvironment(_env);
            _env = nullptr;
        }
        _module = nullptr;
        _startFunc = nullptr;
        _onFrameFunc = nullptr;

        if (_metaHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateMetadata(_metaHandle);
            _metaHandle = MetadataHandle::invalid();
        }

        _wasmBytes.clear();
        _wasmBytes.shrink_to_fit();

        return Ok();
    }

    void suspend() override {
        ydebug("YWasm::suspend");
    }

    void renderToStaging(float time) override {
        if (!_running || !_onFrameFunc) {
            // Not animated - just calculate if dirty
            if (_dirty && !_buffer->empty()) {
                _builder->calculate();
                _dirty = false;
            }
            return;
        }

        float dt = (_lastTime < 0.0f) ? 0.0f : (time - _lastTime);
        _lastTime = time;

        // Call on_frame(dt) if exported
        M3Result result = m3_CallV(_onFrameFunc, dt);
        if (result) {
            yerror("YWasm::renderToStaging: on_frame failed: {}", result);
            _running = false;
            return;
        }

        // Recalculate if dirty
        if (_dirty && !_buffer->empty()) {
            _builder->calculate();
            _dirty = false;
        }
    }

    Result<void> finalize() override {
        if (!_builder) return Ok();
        return _builder->writeBuffers();
    }

private:
    //=========================================================================
    // Wasm3 runtime initialization
    //=========================================================================

    Result<void> initWasmRuntime() {
        if (_wasmBytes.empty()) {
            return Err<void>("YWasm: empty wasm payload");
        }

        // Create environment
        _env = m3_NewEnvironment();
        if (!_env) {
            return Err<void>("YWasm: failed to create wasm3 environment");
        }

        // Create runtime with 64KB stack
        _runtime = m3_NewRuntime(_env, 64 * 1024, this);
        if (!_runtime) {
            return Err<void>("YWasm: failed to create wasm3 runtime");
        }

        // Parse module
        M3Result result = m3_ParseModule(_env, &_module, _wasmBytes.data(),
                                         static_cast<uint32_t>(_wasmBytes.size()));
        if (result) {
            return Err<void>(std::string("YWasm: parse failed: ") + result);
        }

        // Load module into runtime
        result = m3_LoadModule(_runtime, _module);
        if (result) {
            m3_FreeModule(_module);
            _module = nullptr;
            return Err<void>(std::string("YWasm: load failed: ") + result);
        }

        // Link yetty host functions
        if (auto res = linkHostFunctions(); !res) {
            return Err<void>("YWasm: failed to link host functions", res);
        }

        // Find entry point (_start or main)
        result = m3_FindFunction(&_startFunc, _runtime, "_start");
        if (result) {
            result = m3_FindFunction(&_startFunc, _runtime, "main");
        }

        // Find optional on_frame callback
        m3_FindFunction(&_onFrameFunc, _runtime, "on_frame");

        // Run start function if found
        if (_startFunc) {
            ydebug("YWasm: calling entry point");
            result = m3_CallV(_startFunc);
            if (result) {
                M3ErrorInfo info;
                m3_GetErrorInfo(_runtime, &info);
                yerror("YWasm: start failed: {} - {}", result,
                       info.message ? info.message : "no details");
                return Err<void>(std::string("YWasm: start failed: ") + result);
            }
        }

        _running = (_onFrameFunc != nullptr);
        yinfo("YWasm: module loaded, {} prims, running={}",
              _buffer->primCount(), _running);
        return Ok();
    }

    //=========================================================================
    // Host function bindings
    //=========================================================================

    Result<void> linkHostFunctions() {
        M3Result result;

        // yetty_log(ptr, len)
        result = m3_LinkRawFunctionEx(_module, "yetty", "log", "v(ii)",
                                      &hostLog, this);
        if (result && result != m3Err_functionLookupFailed) {
            return Err<void>(std::string("YWasm: link log failed: ") + result);
        }

        // yetty_draw_rect(x, y, w, h, color)
        result = m3_LinkRawFunctionEx(_module, "yetty", "draw_rect", "v(ffffi)",
                                      &hostDrawRect, this);
        if (result && result != m3Err_functionLookupFailed) {
            return Err<void>(std::string("YWasm: link draw_rect failed: ") + result);
        }

        // yetty_draw_circle(cx, cy, r, color)
        result = m3_LinkRawFunctionEx(_module, "yetty", "draw_circle", "v(fffi)",
                                      &hostDrawCircle, this);
        if (result && result != m3Err_functionLookupFailed) {
            return Err<void>(std::string("YWasm: link draw_circle failed: ") + result);
        }

        // yetty_draw_line(x1, y1, x2, y2, color, width)
        result = m3_LinkRawFunctionEx(_module, "yetty", "draw_line", "v(ffffif)",
                                      &hostDrawLine, this);
        if (result && result != m3Err_functionLookupFailed) {
            return Err<void>(std::string("YWasm: link draw_line failed: ") + result);
        }

        // yetty_draw_text(ptr, len, x, y, size, color)
        result = m3_LinkRawFunctionEx(_module, "yetty", "draw_text", "v(iifffi)",
                                      &hostDrawText, this);
        if (result && result != m3Err_functionLookupFailed) {
            return Err<void>(std::string("YWasm: link draw_text failed: ") + result);
        }

        // yetty_request_frame()
        result = m3_LinkRawFunctionEx(_module, "yetty", "request_frame", "v()",
                                      &hostRequestFrame, this);
        if (result && result != m3Err_functionLookupFailed) {
            return Err<void>(std::string("YWasm: link request_frame failed: ") + result);
        }

        // yetty_exit(code)
        result = m3_LinkRawFunctionEx(_module, "yetty", "exit", "v(i)",
                                      &hostExit, this);
        if (result && result != m3Err_functionLookupFailed) {
            return Err<void>(std::string("YWasm: link exit failed: ") + result);
        }

        // yetty_get_width() -> i32
        result = m3_LinkRawFunctionEx(_module, "yetty", "get_width", "i()",
                                      &hostGetWidth, this);
        if (result && result != m3Err_functionLookupFailed) {
            return Err<void>(std::string("YWasm: link get_width failed: ") + result);
        }

        // yetty_get_height() -> i32
        result = m3_LinkRawFunctionEx(_module, "yetty", "get_height", "i()",
                                      &hostGetHeight, this);
        if (result && result != m3Err_functionLookupFailed) {
            return Err<void>(std::string("YWasm: link get_height failed: ") + result);
        }

        // yetty_clear()
        result = m3_LinkRawFunctionEx(_module, "yetty", "clear", "v()",
                                      &hostClear, this);
        if (result && result != m3Err_functionLookupFailed) {
            return Err<void>(std::string("YWasm: link clear failed: ") + result);
        }

        return Ok();
    }

    //=========================================================================
    // Host function implementations
    //=========================================================================

    static m3ApiRawFunction(hostLog) {
        m3ApiGetArg(int32_t, ptr);
        m3ApiGetArg(int32_t, len);

        auto* self = static_cast<YWasmImpl*>(m3_GetUserData(runtime));
        if (!self) m3ApiTrap("null context");

        uint32_t memSize = 0;
        uint8_t* mem = m3_GetMemory(runtime, &memSize, 0);
        if (!mem || ptr < 0 || static_cast<uint32_t>(ptr + len) > memSize) {
            m3ApiTrap(m3Err_trapOutOfBoundsMemoryAccess);
        }

        std::string msg(reinterpret_cast<char*>(mem + ptr), len);
        yinfo("YWasm[{}]: {}", self->id(), msg);

        m3ApiSuccess();
    }

    static m3ApiRawFunction(hostDrawRect) {
        m3ApiGetArg(float, x);
        m3ApiGetArg(float, y);
        m3ApiGetArg(float, w);
        m3ApiGetArg(float, h);
        m3ApiGetArg(int32_t, color);

        auto* self = static_cast<YWasmImpl*>(m3_GetUserData(runtime));
        if (!self) m3ApiTrap("null context");

        // addBox uses center + half-sizes
        float cx = x + w * 0.5f;
        float cy = y + h * 0.5f;
        float hw = w * 0.5f;
        float hh = h * 0.5f;
        self->_buffer->addBox(0, cx, cy, hw, hh,
                              static_cast<uint32_t>(color), 0, 0.0f, 0.0f);
        self->_dirty = true;

        m3ApiSuccess();
    }

    static m3ApiRawFunction(hostDrawCircle) {
        m3ApiGetArg(float, cx);
        m3ApiGetArg(float, cy);
        m3ApiGetArg(float, r);
        m3ApiGetArg(int32_t, color);

        auto* self = static_cast<YWasmImpl*>(m3_GetUserData(runtime));
        if (!self) m3ApiTrap("null context");

        self->_buffer->addCircle(0, cx, cy, r,
                                 static_cast<uint32_t>(color), 0, 0.0f, 0.0f);
        self->_dirty = true;

        m3ApiSuccess();
    }

    static m3ApiRawFunction(hostDrawLine) {
        m3ApiGetArg(float, x1);
        m3ApiGetArg(float, y1);
        m3ApiGetArg(float, x2);
        m3ApiGetArg(float, y2);
        m3ApiGetArg(int32_t, color);
        m3ApiGetArg(float, width);

        auto* self = static_cast<YWasmImpl*>(m3_GetUserData(runtime));
        if (!self) m3ApiTrap("null context");

        self->_buffer->addSegment(0, x1, y1, x2, y2,
                                  0, static_cast<uint32_t>(color), width, 0.0f);
        self->_dirty = true;

        m3ApiSuccess();
    }

    static m3ApiRawFunction(hostDrawText) {
        m3ApiGetArg(int32_t, ptr);
        m3ApiGetArg(int32_t, len);
        m3ApiGetArg(float, x);
        m3ApiGetArg(float, y);
        m3ApiGetArg(float, size);
        m3ApiGetArg(int32_t, color);

        auto* self = static_cast<YWasmImpl*>(m3_GetUserData(runtime));
        if (!self) m3ApiTrap("null context");

        uint32_t memSize = 0;
        uint8_t* mem = m3_GetMemory(runtime, &memSize, 0);
        if (!mem || ptr < 0 || static_cast<uint32_t>(ptr + len) > memSize) {
            m3ApiTrap(m3Err_trapOutOfBoundsMemoryAccess);
        }

        std::string text(reinterpret_cast<char*>(mem + ptr), len);
        self->_buffer->addText(x, y, text, size, static_cast<uint32_t>(color), 0, -1);
        self->_dirty = true;

        m3ApiSuccess();
    }

    static m3ApiRawFunction(hostRequestFrame) {
        auto* self = static_cast<YWasmImpl*>(m3_GetUserData(runtime));
        if (!self) m3ApiTrap("null context");

        self->_running = true;
        m3ApiSuccess();
    }

    static m3ApiRawFunction(hostExit) {
        m3ApiGetArg(int32_t, code);

        auto* self = static_cast<YWasmImpl*>(m3_GetUserData(runtime));
        if (!self) m3ApiTrap("null context");

        yinfo("YWasm: exit({})", code);
        self->_running = false;
        self->_exitCode = code;

        m3ApiSuccess();
    }

    static m3ApiRawFunction(hostGetWidth) {
        m3ApiReturnType(int32_t);

        auto* self = static_cast<YWasmImpl*>(m3_GetUserData(runtime));
        if (!self) m3ApiTrap("null context");

        m3ApiReturn(static_cast<int32_t>(self->_widthCells));
    }

    static m3ApiRawFunction(hostGetHeight) {
        m3ApiReturnType(int32_t);

        auto* self = static_cast<YWasmImpl*>(m3_GetUserData(runtime));
        if (!self) m3ApiTrap("null context");

        m3ApiReturn(static_cast<int32_t>(self->_heightCells));
    }

    static m3ApiRawFunction(hostClear) {
        auto* self = static_cast<YWasmImpl*>(m3_GetUserData(runtime));
        if (!self) m3ApiTrap("null context");

        self->_buffer->clear();
        self->_dirty = true;

        m3ApiSuccess();
    }

    //=========================================================================
    // Args parsing
    //=========================================================================

    void parseArgs(const std::string& args) {
        ydebug("YWasm::parseArgs: args='{}'", args);
        // TODO: parse wasm-specific args
    }

    //=========================================================================
    // Member variables
    //=========================================================================

    FontManager::Ptr _fontManager;
    GpuAllocator::Ptr _gpuAllocator;
    std::string _argsStr;
    std::vector<uint8_t> _wasmBytes;

    // ydraw rendering
    YDrawBuilder::Ptr _builder;
    YDrawBuffer::Ptr _buffer;
    bool _dirty = true;
    float _viewZoom = 1.0f;
    float _viewPanX = 0.0f;
    float _viewPanY = 0.0f;

    // wasm3 state
    IM3Environment _env = nullptr;
    IM3Runtime _runtime = nullptr;
    IM3Module _module = nullptr;
    IM3Function _startFunc = nullptr;
    IM3Function _onFrameFunc = nullptr;

    // Runtime state
    bool _running = false;
    int32_t _exitCode = 0;
    float _lastTime = -1.0f;
};

//=============================================================================
// Factory methods
//=============================================================================

Result<YWasm::Ptr> YWasm::createImpl(
    ContextType& ctx,
    const YettyContext& yettyCtx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)ctx;

    if (!yettyCtx.cardManager) {
        return Err<Ptr>("YWasm::createImpl: null GpuMemoryManager");
    }

    auto card = std::make_shared<YWasmImpl>(
        yettyCtx, x, y, widthCells, heightCells, args, payload);

    if (auto res = card->init(); !res) {
        return Err<Ptr>("YWasm::createImpl: init failed", res);
    }

    return Ok<Ptr>(card);
}

} // namespace yetty::card
