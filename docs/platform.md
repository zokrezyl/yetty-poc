# Platform Abstraction Layer

Yetty runs on desktop (Linux/macOS/Windows), Android, and WebAssembly. Each platform has different APIs for input, windowing, filesystem, clipboard, and shell access. The platform layer abstracts these differences so the core terminal engine (gpu-screen, terminal, cards) remains platform-agnostic.

## Threading Model

**Desktop (GLFW):**
- **Main thread**: Creates window, runs blocking input loop (`glfwWaitEvents`)
- **Render thread**: Runs libuv event loop, processes EventQueue, renders with WebGPU

**Android:**
- **Main thread**: `android_main` handles both input (ALooper) and rendering

**WebASM:**
- **Single thread**: Browser event loop handles everything

This separation on desktop allows:
- Zero input latency (blocking wait, no polling)
- Render thread runs libuv independently
- No busy-polling or timers needed

## Design Principles

1. **Event-driven**: Platform managers inject `base::Event` into EventQueue. No callbacks propagating through layers.
2. **Small interfaces**: Each manager has a single responsibility. Empty interfaces where possible.
3. **ThreadSingleton pattern**: Each manager is a thread singleton. Build system links the correct implementation per platform.
4. **Internal glue via shared singletons**: Platform-specific state (window handles, etc.) shared via internal singletons.

## Managers

### InputManager

Translates platform input into `base::Event` and injects into EventLoop.

| Platform | Input Sources |
|----------|---------------|
| GLFW | Keyboard, mouse, scroll wheel |
| Android | Touch, gestures (pinch/pan), soft keyboard, hardware keys |
| WebASM | Keyboard, mouse (via Emscripten) |
| Windows | Keyboard, mouse, touch |

Events produced: `KeyDown`, `KeyUp`, `Char`, `MouseMove`, `MouseDown`, `MouseUp`, `Scroll`

Gesture translation (Android):
- Pinch → `Scroll` with `mods=2` (Ctrl) → triggers visual zoom
- Two-finger pan → `Scroll` with `mods=0` → pans in zoom mode

### SurfaceManager

Window lifecycle and WebGPU surface creation.

| Method | Purpose |
|--------|---------|
| `createWindow()` | Create native window (GLFW) or bind to system window (Android) |
| `createWGPUSurface()` | Create platform-specific WebGPU surface |
| `getWindowSize()` | Current window dimensions |
| `getContentScale()` | HiDPI scale factor |
| `runMainLoop()` | Platform main loop (blocking on desktop, RAF on web) |
| `pollEvents()` | Process pending window events |
| `getTime()` | High-resolution timestamp |

### FsPathManager

Returns platform-appropriate paths for assets and runtime files.

| Path | Desktop | Android | WebASM |
|------|---------|---------|--------|
| Shaders | `./shaders/` or system path | `{dataDir}/shaders/` | embedded |
| MSDF fonts | `./msdf-fonts/` | `{dataDir}/msdf-fonts/` | embedded |
| TTF fonts | `./fonts/` | `{dataDir}/fonts/` | embedded |
| Runtime | `$XDG_RUNTIME_DIR` | `{dataDir}/` | N/A |

Android extracts embedded assets (via incbin) to dataDir on first run.

### ClipboardManager

Clipboard read/write operations. Listens for clipboard-related events.

| Platform | Implementation |
|----------|----------------|
| GLFW | `glfwGetClipboardString()` / `glfwSetClipboardString()` |
| Android | JNI to `ClipboardManager` |
| WebASM | `navigator.clipboard` API |
| Windows | Win32 clipboard API |

### PtyManager

Creates platform-appropriate PTY/shell provider.

| Platform | Implementation |
|----------|----------------|
| Desktop | `forkpty()` + user's shell |
| Android | Toybox shell or Termux connection |
| WebASM | JSLinux iframe emulator |
| Windows | ConPTY |

## File Structure

```
include/yetty/platform/
    input-manager.h
    surface-manager.h
    fs-path-manager.h
    clipboard-manager.h
    pty-manager.h

src/yetty/platform/
    android/
        input-manager.cpp
        surface-manager.cpp
        fs-path-manager.cpp
        clipboard-manager.cpp
        pty-manager.cpp
    glfw/
        input-manager.cpp
        surface-manager.cpp
        fs-path-manager.cpp
        clipboard-manager.cpp
        pty-manager.cpp
    webasm/
        ...
    windows/
        ...
```

## Build Integration

CMake adds platform-specific sources:

```cmake
if(ANDROID)
    target_sources(yetty PRIVATE
        platform/android/input-manager.cpp
        platform/android/surface-manager.cpp
        ...)
elseif(EMSCRIPTEN)
    target_sources(yetty PRIVATE
        platform/webasm/input-manager.cpp
        ...)
else()
    target_sources(yetty PRIVATE
        platform/glfw/input-manager.cpp
        ...)
endif()
```

## Usage in yetty.cpp

```cpp
auto input = InputManager::create();
auto surface = SurfaceManager::create();
auto paths = FsPathManager::create();
auto clipboard = ClipboardManager::create();
auto pty = PtyManager::create();

// InputManager injects events directly - no callbacks needed
surface->runMainLoop([&]() {
    surface->pollEvents();
    // Events already in EventLoop, processed by gpu-screen
    return !surface->shouldClose();
});
```
