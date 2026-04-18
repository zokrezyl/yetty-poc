# WebAssembly Build Fix Analysis

## Summary

After the `main.cpp` refactoring (issue 2026.01.01), the WebAssembly (Emscripten) build is broken. The refactoring moved initialization logic into a new `Yetty` engine class, but:

1. Missing source files referenced in CMakeLists.txt
2. Web-specific code paths were not updated
3. Dawn vs wgpu-native API incompatibilities exist

## Build Errors

Running `make build-webasm-release` produces:

```
CMake Error at CMakeLists.txt:600 (add_executable):
  Cannot find source file:
    src/yetty/input-handler.cpp
```

### 1. Missing Source File

**Location**: `CMakeLists.txt:589`

**Cause**: The Emscripten source list references `src/yetty/input-handler.cpp` which doesn't exist.

```cmake
else()
    # Web uses main.cpp with Emscripten GLFW
    list(APPEND YETTY_SOURCES
        src/yetty/main.cpp
        src/yetty/yetty.cpp
        src/yetty/config.cpp
        src/yetty/input-handler.cpp    # <-- DOES NOT EXIST
        src/yetty/web-terminal.cpp
        src/yetty/web-display.cpp
        src/yetty/gpu-screen.cpp
    )
endif()
```

**Fix Options**:
1. Remove `input-handler.cpp` from the list (input handling is in web-terminal.cpp)
2. Create `input-handler.cpp` with web-specific input handling
3. Use conditional compilation in a shared input-handler

## Files Requiring Changes

### 1. `CMakeLists.txt`

**Current state**: Web source list references non-existent file.

**Changes needed**:
- Remove `src/yetty/input-handler.cpp` from Emscripten sources
- Or create the file with web-specific implementation

### 2. `src/yetty/yetty.cpp`

**Current state**: Has `!YETTY_WEB` guards but the web path is incomplete.

**Concerns**:
- `yetty.cpp` still uses GLFW which is available in Emscripten (`-sUSE_GLFW=3`)
- BUT the initialization assumes desktop patterns
- Web build needs different WebGPU surface creation (canvas-based)

**Reference**: The deprecated `webgpu-context.cpp` has working web initialization:
```cpp
#if YETTY_WEB
    device_ = emscripten_webgpu_get_device();
    // ...
    WGPUSurfaceDescriptorFromCanvasHTMLSelector canvasDesc = {};
    canvasDesc.chain.sType = WGPUSType_SurfaceDescriptorFromCanvasHTMLSelector;
    canvasDesc.selector = "#canvas";
#endif
```

### 3. `include/yetty/yetty.h`

**Current state**: Only desktop `createImpl` signature.

**Changes needed**: Consider adding web-specific factory or parameters.

### 4. `src/yetty/main.cpp`

**Current state**: Has `YETTY_WEB` guard for global Yetty pointer but assumes desktop `create(argc, argv)`.

```cpp
#if YETTY_WEB
static yetty::Yetty::Ptr g_yetty;
#endif
```

**Changes needed**: Web may need different initialization (no command line args, canvas-based).

---

## Dawn vs wgpu-native API Incompatibilities

### Critical Issue: Desktop Uses Dawn, Web Uses Emscripten WebGPU

The project now defaults to **Dawn** for desktop builds but the web build uses **Emscripten's built-in WebGPU** (browser-native). These have different APIs:

| Feature | wgpu-native v27 / Dawn | Emscripten WebGPU |
|---------|------------------------|-------------------|
| Strings | `WGPUStringView` | `const char*` |
| Callbacks | `WGPURequestAdapterCallbackInfo` struct | Function pointer + userdata |
| Shader source | `WGPUShaderSourceWGSL` | `WGPUShaderModuleWGSLDescriptor` |
| Surface creation | Platform-specific | `WGPUSurfaceDescriptorFromCanvasHTMLSelector` |

### The `wgpu-compat.h` Header

The project has a compatibility header at `include/yetty/wgpu-compat.h` that attempts to bridge these differences:

```cpp
#if YETTY_WEB
#define WGPU_STR(s) (s)
#define WGPU_STR_NULL nullptr
#define WGPU_USE_OLD_API 1
using WGPUTexelCopyBufferLayout = WGPUTextureDataLayout;
using WGPUShaderSourceWGSL = WGPUShaderModuleWGSLDescriptor;
// ...
#else
#define WGPU_STR(s) (WGPUStringView{ .data = (s), .length = WGPU_STRLEN })
// ...
#endif
```

### API Differences Requiring Attention

#### 1. Adapter/Device Request Callbacks

**wgpu-native v27 / Dawn** (new style):
```cpp
WGPURequestAdapterCallbackInfo adapterCb = {};
adapterCb.mode = WGPUCallbackMode_AllowSpontaneous;
adapterCb.callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
                        WGPUStringView message, void* userdata1, void* userdata2) {
    // ...
};
adapterCb.userdata1 = &_adapter;
wgpuInstanceRequestAdapter(_instance, &adapterOpts, adapterCb);
```

**Emscripten** (old style):
```cpp
wgpuInstanceRequestAdapter(instance, &adapterOpts,
    [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
       const char* message, void* userdata) {
        // ...
    }, &_adapter);
```

**Files affected**:
- `src/yetty/yetty.cpp` (lines 300-310, 389-402)
- `src/yetty/webgpu-context.cpp` (lines 152-162, 217-227, 257-267, 323-333)
- `src/yetty/msdf-cdb-provider.cpp` (lines 88-99, 142-152)

#### 2. String Parameters

**wgpu-native v27 / Dawn**:
```cpp
WGPUShaderSourceWGSL wgslDesc = {};
wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
wgslDesc.code = { .data = code.c_str(), .length = code.size() };
```

**Emscripten**:
```cpp
WGPUShaderModuleWGSLDescriptor wgslDesc = {};
wgslDesc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
wgslDesc.code = code.c_str();
```

The `WGPU_SHADER_CODE` macro handles this, but code must use the macro consistently.

#### 3. Surface Creation

**Desktop (GLFW/Dawn)**:
```cpp
_surface = glfwCreateWindowWGPUSurface(_instance, _window);
```

**Web (Emscripten)**:
```cpp
WGPUSurfaceDescriptorFromCanvasHTMLSelector canvasDesc = {};
canvasDesc.chain.sType = WGPUSType_SurfaceDescriptorFromCanvasHTMLSelector;
canvasDesc.selector = "#canvas";
WGPUSurfaceDescriptor surfaceDesc = {};
surfaceDesc.nextInChain = &canvasDesc.chain;
surface_ = wgpuInstanceCreateSurface(instance_, &surfaceDesc);
```

#### 4. Swap Chain vs Configure Surface

**Emscripten** (old API): Uses `wgpuDeviceCreateSwapChain()`

**wgpu-native v27 / Dawn** (new API): Uses `wgpuSurfaceConfigure()`

### Current compat header coverage

| API Feature | Covered by `wgpu-compat.h`? |
|-------------|----------------------------|
| String types | ✅ Yes (`WGPU_STR`, `WGPU_STR_NULL`) |
| Shader source struct | ✅ Yes (type alias) |
| Shader code assignment | ✅ Yes (`WGPU_SHADER_CODE`) |
| Color attachment clear | ✅ Yes (`WGPU_COLOR_ATTACHMENT_CLEAR`) |
| Mipmap filter mode | ✅ Yes |
| Callback structs | ❌ No - needs manual `#if YETTY_WEB` |
| Surface creation | ❌ No - platform-specific code |
| Swap chain vs configure | ❌ No - platform-specific code |

### Recommendations for API Compatibility

#### Option A: Extend `wgpu-compat.h`

Add macros/wrappers for callback-style differences:

```cpp
#if YETTY_WEB
// Emscripten-style adapter request
#define WGPU_REQUEST_ADAPTER(instance, opts, callback, userdata) \
    wgpuInstanceRequestAdapter(instance, opts, callback, userdata)
#else
// wgpu-native v27 / Dawn style
#define WGPU_REQUEST_ADAPTER(instance, opts, callbackInfo) \
    wgpuInstanceRequestAdapter(instance, opts, callbackInfo)
#endif
```

This is tricky because the function signatures differ.

#### Option B: Abstract WebGPU Context Factory

Create a platform-specific factory that hides the differences:

```cpp
// gpu-context-factory.h
class GPUContextFactory {
public:
    static Result<GPUContext> createForDesktop(GLFWwindow* window);
    static Result<GPUContext> createForWeb(const char* canvasSelector);
    static Result<GPUContext> createForAndroid(ANativeWindow* window);
};
```

#### Option C: Keep Platform-Specific Init in yetty.cpp

Use `#if YETTY_WEB` blocks within `YettyImpl` for the different initialization paths.

---

## Implementation Checklist

### Immediate Fixes (to make build work)

- [ ] Remove or create `src/yetty/input-handler.cpp`
- [ ] Add web-specific WebGPU initialization to `yetty.cpp`
- [ ] Add `#if YETTY_WEB` guards around GLFW-specific code
- [ ] Ensure all WebGPU calls use `wgpu-compat.h` macros

### API Compatibility Fixes

- [ ] Add callback-style wrappers to `wgpu-compat.h`
- [ ] Update `yetty.cpp` adapter/device request for web
- [ ] Handle swap chain (web) vs surface configure (desktop)
- [ ] Test shader creation on web

### Testing

- [ ] Verify WebAssembly build compiles
- [ ] Test in browser with WebGPU support
- [ ] Verify terminal rendering works
- [ ] Check keyboard input handling

---

## Web-Specific Considerations

### Emscripten Main Loop

Web builds use Emscripten's async main loop:

```cpp
// Current main.cpp
#if YETTY_WEB
static yetty::Yetty::Ptr g_yetty;
#endif
```

The `Yetty::run()` method must handle this:

```cpp
#if YETTY_WEB
emscripten_set_main_loop_arg([](void* arg) {
    static_cast<YettyImpl*>(arg)->mainLoopIteration();
}, this, 0, true);
#else
while (!shouldQuit) {
    mainLoopIteration();
}
#endif
```

### Web Terminal vs Native Terminal

The web build uses `web-terminal.cpp` and `web-display.cpp` which provide:
- JavaScript-callable functions (`EMSCRIPTEN_KEEPALIVE`)
- vterm-based terminal emulation
- No PTY (shell runs in JavaScript/WebAssembly)

Desktop builds use `terminal.cpp` which provides:
- PTY-based terminal with real shell
- libuv event loop

---

## File Structure After Fixes

```
src/yetty/
├── main.cpp              # Entry point (all platforms)
├── yetty.cpp             # Engine class with platform #ifdefs
├── yetty-web.cpp         # (Optional) Web-specific helpers
├── config.cpp            # Configuration (all platforms)
├── web-terminal.cpp      # Web terminal (YETTY_WEB only)
├── web-display.cpp       # Web display (YETTY_WEB only)
├── terminal.cpp          # Native terminal (desktop only)
├── gpu-screen.cpp        # GPU rendering (all platforms)
└── ...

include/yetty/
├── yetty.h               # Engine interface
├── wgpu-compat.h         # WebGPU API compatibility
├── gpu-context.h         # GPU context abstraction
└── ...
```

---

## Priority Order

1. **HIGH**: Fix CMakeLists.txt (remove missing file)
2. **HIGH**: Add web initialization path to `yetty.cpp`
3. **MEDIUM**: Ensure all WebGPU calls use compat macros
4. **MEDIUM**: Add callback-style wrappers to `wgpu-compat.h`
5. **LOW**: Refactor to platform-specific files (cleaner but more work)

---

## Reference: Working Web Code

The following files have working web implementations that can be referenced:

1. **`webgpu-context.cpp`** (deprecated but working):
   - Canvas surface creation
   - Device/adapter setup for Emscripten
   - Swap chain configuration

2. **`web-display.cpp`**:
   - WebDisplay widget
   - vterm integration
   - GPU rendering

3. **`web-terminal.cpp`**:
   - JavaScript-callable terminal API
   - Keyboard handling
   - Terminal state management
