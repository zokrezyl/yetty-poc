# Android Build Fix Analysis

## Summary

After the `main.cpp` refactoring (issue 2026.01.01), the Android build is broken. The refactoring moved initialization logic from `main.cpp` into a new `Yetty` engine class, but the Android-specific code paths were not properly updated.

## Build Errors

Running `make build-android-wgpu-release` produces these errors:

### 1. Missing Android Log Header in `main.cpp`

```
src/yetty/main.cpp:34:25: error: use of undeclared identifier 'ANDROID_LOG_INFO'
src/yetty/main.cpp:38:29: error: use of undeclared identifier 'ANDROID_LOG_ERROR'
```

**Location**: `src/yetty/main.cpp:29-46`

**Cause**: The Android entry point uses `__android_log_print()` but doesn't include `<android/log.h>`.

**Fix**: Add conditional include:
```cpp
#if defined(__ANDROID__)
#include <android/log.h>
#include <android_native_app_glue.h>
#endif
```

### 2. Missing `Yetty::create(android_app*)` Overload

```
include/yetty/base/factory.h:135:13: error: static assertion failed due to requirement 'sizeof(yetty::Yetty) == 0': ObjectFactory: No createImpl found.
```

**Location**: `src/yetty/main.cpp:36`

**Cause**: `main.cpp` calls `yetty::Yetty::create(app)` where `app` is `struct android_app*`, but `Yetty` only defines `createImpl(ContextType&, int argc, char* argv[])`.

**Fix**: Add Android-specific `createImpl` overload in `yetty.h`:
```cpp
#if defined(__ANDROID__)
  static Result<Ptr> createImpl(ContextType &ctx, struct android_app* app) noexcept;
#endif
```

### 3. GLFW Header Not Available on Android

```
src/yetty/yetty.cpp:16:10: fatal error: 'glfw3webgpu.h' file not found
```

**Location**: `src/yetty/yetty.cpp:16`

**Cause**: `yetty.cpp` unconditionally includes `<glfw3webgpu.h>`, but GLFW/glfw3webgpu is desktop-only (not built for Android).

**Fix**: Conditional include:
```cpp
#if !defined(__ANDROID__) && !YETTY_WEB
#include <glfw3webgpu.h>
#endif
```

## Files Requiring Changes

### 1. `src/yetty/main.cpp`

**Current state**: Has Android entry point but missing includes.

**Changes needed**:
- Add `#include <android/log.h>` inside `#if defined(__ANDROID__)`
- Add `#include <android_native_app_glue.h>` for `struct android_app`

### 2. `include/yetty/yetty.h`

**Current state**: Only has desktop signature `createImpl(ContextType&, int argc, char* argv[])`.

**Changes needed**:
- Add forward declaration of `struct android_app` for Android
- Add Android-specific `createImpl` overload
- Consider adding `shutdown()` to the interface (Android entry point calls it)

```cpp
#if defined(__ANDROID__)
struct android_app;  // Forward declaration
#endif

class Yetty : public virtual Object, public ObjectFactory<Yetty> {
public:
  using Ptr = std::shared_ptr<Yetty>;

  ~Yetty() override = default;

  // Desktop entry point
  static Result<Ptr> createImpl(ContextType &ctx, int argc, char *argv[]) noexcept;

#if defined(__ANDROID__)
  // Android entry point
  static Result<Ptr> createImpl(ContextType &ctx, struct android_app* app) noexcept;
#endif

  virtual Result<void> run() noexcept = 0;
  virtual Result<void> shutdown() noexcept = 0;  // Add to interface

protected:
  Yetty() = default;
};
```

### 3. `src/yetty/yetty.cpp`

**Current state**: Heavily uses GLFW and desktop-only APIs without conditional compilation.

**Changes needed**:

#### Include guards
```cpp
#if !defined(__ANDROID__) && !YETTY_WEB
#include <glfw3webgpu.h>
#endif

#if defined(__ANDROID__)
#include <android/native_window.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#endif
```

#### Class member guards

The `YettyImpl` class has members that are desktop-only:

```cpp
// Line 58: GLFWwindow* _window = nullptr;
#if !defined(__ANDROID__) && !YETTY_WEB
    GLFWwindow* _window = nullptr;
#endif

#if defined(__ANDROID__)
    struct android_app* _androidApp = nullptr;
    ANativeWindow* _nativeWindow = nullptr;
#endif
```

#### Method guards

Multiple methods use GLFW and should be guarded:

- `initWindow()` - needs Android implementation using `ANativeWindow`
- `initWebGPU()` - uses `glfwCreateWindowWGPUSurface()`, needs Android path
- `initCallbacks()` - sets GLFW callbacks, needs Android event handling
- `mainLoopIteration()` - uses `glfwPollEvents()`, needs Android event loop

#### Android-specific implementation needed

```cpp
#if defined(__ANDROID__)
Result<Yetty::Ptr> Yetty::createImpl(ContextType&, struct android_app* app) noexcept {
    auto impl = Ptr(new YettyImpl());
    if (auto res = static_cast<YettyImpl*>(impl.get())->initAndroid(app); !res) {
        return Err<Ptr>("Failed to init Yetty for Android", res);
    }
    return Ok(std::move(impl));
}

Result<void> YettyImpl::initAndroid(struct android_app* app) noexcept {
    _androidApp = app;
    // Wait for window...
    // Initialize WebGPU with ANativeWindow...
    // Set up event callbacks...
}
#endif
```

### 4. Files with existing `__ANDROID__` guards (reference)

These files already have proper Android conditional compilation:

| File | Guard Type | Notes |
|------|------------|-------|
| `src/yetty/webgpu-context.cpp` | `YETTY_ANDROID` | Has full Android WebGPU surface creation |
| `src/yetty/terminal.cpp` | `!defined(__ANDROID__)` | PTY code excluded for Android |
| `src/yetty/workspace.cpp` | `!defined(__ANDROID__)` | Terminal spawning excluded |
| `src/yetty/base/event-loop.cpp` | `!defined(__ANDROID__)` | libuv event loop excluded |
| `src/yetty/widget-factory.cpp` | `!defined(__ANDROID__)` | Python plugin excluded |
| `src/yetty/bm-font.cpp` | `defined(__ANDROID__)` | Android asset loading |
| `src/yetty/shader-font.cpp` | `defined(__ANDROID__)` | Pre-built atlas path |

### 5. `CMakeLists.txt`

**Current state**: Has Android source files but missing some dependencies.

**Changes needed**:
- Verify all Android sources are listed
- Consider adding Android-specific initialization sources

Current Android sources (lines 521-531):
```cmake
if(YETTY_ANDROID)
    list(APPEND YETTY_SOURCES
        src/yetty/main.cpp
        src/yetty/yetty.cpp
        src/yetty/config.cpp
        src/yetty/terminal.cpp
        src/yetty/gpu-screen.cpp
        src/yetty/osc-command.cpp
    )
endif()
```

May need to add:
- Base sources that Android needs (e.g., `base/event-loop.cpp` stubs)

## Architecture Considerations

### Option A: Unified YettyImpl with Platform Branches

Single `YettyImpl` class with `#ifdef` blocks for each platform. This is the current approach.

**Pros**: Single class, shared code
**Cons**: Complex `#ifdef` nesting, harder to read

### Option B: Separate Platform Implementations

```
src/yetty/yetty.cpp          # Shared interface
src/yetty/yetty-desktop.cpp  # Desktop (GLFW) implementation
src/yetty/yetty-android.cpp  # Android (native_app_glue) implementation
src/yetty/yetty-web.cpp      # Web (Emscripten) implementation
```

**Pros**: Clean separation, easier to maintain each platform
**Cons**: Code duplication, need to keep in sync

### Option C: Composition Pattern

Create platform-specific "backend" classes:

```cpp
class WindowBackend {
    virtual void pollEvents() = 0;
    virtual void* getNativeHandle() = 0;
    // ...
};

class GlfwWindowBackend : public WindowBackend { ... };
class AndroidWindowBackend : public WindowBackend { ... };
```

**Pros**: Clean architecture, testable
**Cons**: More code, may be overkill

### Recommendation

Continue with **Option A** (unified impl with ifdefs) since it matches the existing pattern in other files (`webgpu-context.cpp`, `terminal.cpp`, etc.), but be careful to:

1. Use consistent guard style: `#if defined(__ANDROID__)` or `#if YETTY_ANDROID`
2. Keep platform-specific code in clearly marked sections
3. Extract common code into shared methods

## Missing `build-dawn.sh` Script

The Makefile references `build-tools/android/build-dawn.sh` but only `build-wgpu.sh` exists.

**Fix**: Create `build-dawn.sh` similar to `build-wgpu.sh` but downloading Dawn Android binaries instead of wgpu-native.

## Implementation Checklist

- [ ] Add missing includes to `main.cpp` (`<android/log.h>`, `<android_native_app_glue.h>`)
- [ ] Add Android `createImpl` overload to `yetty.h`
- [ ] Add `shutdown()` to `Yetty` interface
- [ ] Add conditional compilation to `yetty.cpp` for includes
- [ ] Add Android member variables to `YettyImpl` (`android_app*`, `ANativeWindow*`)
- [ ] Implement `initAndroid()` method
- [ ] Guard GLFW-specific code in `initWindow()`, `initWebGPU()`, `initCallbacks()`
- [ ] Implement Android event loop in `run()` method
- [ ] Create `build-tools/android/build-dawn.sh` script
- [ ] Test Android build compiles
- [ ] Test on actual Android device/emulator

## Reference: Existing Android Code

The old `webgpu-context.cpp` (now deprecated but still in codebase) has working Android WebGPU initialization:

```cpp
#if YETTY_ANDROID
Result<WebGPUContext::Ptr> WebGPUContext::create(ANativeWindow* window, uint32_t width, uint32_t height) noexcept;
// Uses wgpuInstanceCreateSurface with WGPUSurfaceSourceAndroidNativeWindow
#endif
```

This code should be ported to the new `YettyImpl::initWebGPU()` method.
