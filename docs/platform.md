# Platform Abstraction Layer

Yetty runs on desktop (Linux/macOS/Windows), Android, and WebAssembly. Each platform has different APIs for input, windowing, filesystem, clipboard, and shell access. The platform layer abstracts these differences so the core terminal engine (gpu-screen, terminal, cards) remains platform-agnostic.

## Threading Model

**Desktop (GLFW):**
- **Main thread**: Runs `InitManager` - initializes GLFW, spawns render thread, blocks on `glfwWaitEvents()`
- **Render thread**: Creates window via `GlfwWindowSingleton`, runs libuv event loop, renders with WebGPU

**Key insight**: `glfwWaitEvents()` is GLOBAL - it processes events for all windows, doesn't need a window handle. So:
1. Main thread can block on event loop before window exists
2. Render thread creates window
3. Window callbacks (set on render thread) push events to EventQueue
4. Main thread's `glfwWaitEvents()` returns, callbacks fire, events are queued
5. EventQueue wakes render thread via libuv async

**Android:**
- **Main thread**: `android_main` handles both input (ALooper) and rendering

**WebASM:**
- **Single thread**: Browser doesn't support multiple threads. Everything runs on main thread via `requestAnimationFrame`. No separate render thread, no `InitManager` - just direct initialization and RAF loop.

## Managers

### InitManager

**ThreadSingleton** (main thread). Entry point for platform initialization. All platforms except WebASM.

| Platform | Responsibilities |
|----------|------------------|
| GLFW | `glfwInit()`, spawn render thread, block on `glfwWaitEvents()` |
| Android | Setup ALooper, spawn render thread |

```cpp
// main.cpp (desktop/Android)
auto init = InitManager::instance();
init->run([&]() {
    // === RENDER THREAD ===
    auto yetty = Yetty::create(argc, argv);
    yetty->run();
    yetty->shutdown();
});
```

### GlfwWindowSingleton

**ThreadSingleton** (render thread). Creates and owns the GLFW window.

- Created on render thread (first `instance()` call)
- Calls `glfwCreateWindow()`
- Sets up GLFW callbacks that push to `EventQueue`
- Provides window handle for SurfaceManager

```cpp
// On render thread
auto window = GlfwWindowSingleton::instance();
GLFWwindow* w = window->getWindow();
```

### SurfaceManager

**ThreadSingleton** (render thread). Window properties and WebGPU surface creation.

| Method | Purpose |
|--------|---------|
| `getWindowSize()` | Current window dimensions |
| `getFramebufferSize()` | Framebuffer dimensions (may differ on HiDPI) |
| `getContentScale()` | HiDPI scale factor |
| `shouldClose()` | Window close requested |
| `setTitle()` | Set window title |
| `setIcon()` | Set window icon |
| `createWGPUSurface()` | Create platform-specific WebGPU surface |

GLFW implementation gets window from `GlfwWindowSingleton`.

### InputManager

**ThreadSingleton** (render thread). Transforms raw input events into internal events.

`GlfwWindowSingleton` callbacks push RAW events to `EventQueue`. `InputManager` subscribes to these events and generates internal events:

| Raw Event | Transformation | Internal Event |
|-----------|----------------|----------------|
| KeyDown + Ctrl + letter | Ctrl+A=0x01, Ctrl+B=0x02, ..., Ctrl+Z=0x1A | charInput |

**Why here?** GLFW does NOT fire `charCallback` for control characters (Ctrl+A, Ctrl+C, etc). The transformation happens in InputManager so it can be platform-specific.

### All Managers Summary

| Manager | Thread | Purpose |
|---------|--------|---------|
| InitManager | main | Platform init, spawn render thread, event loop |
| GlfwWindowSingleton | render | Create window, setup callbacks (push RAW events) |
| SurfaceManager | render | Window properties, WebGPU surface |
| InputManager | render | Transform raw events (Ctrl+letter -> control char) |
| FsPathManager | render | Asset paths |
| ClipboardManager | render | Clipboard access |
| PtyManager | render | PTY/shell creation |

### FsPathManager

Returns platform-appropriate paths for assets and runtime files.

| Path | Desktop | Android | WebASM |
|------|---------|---------|--------|
| Shaders | `./shaders/` | `{dataDir}/shaders/` | embedded |
| MSDF fonts | `./msdf-fonts/` | `{dataDir}/msdf-fonts/` | embedded |
| TTF fonts | `./fonts/` | `{dataDir}/fonts/` | embedded |
| Runtime | `$XDG_RUNTIME_DIR` | `{dataDir}/` | N/A |

### ClipboardManager

Clipboard read/write operations.

| Platform | Implementation |
|----------|----------------|
| GLFW | `glfwGetClipboardString()` / `glfwSetClipboardString()` |
| Android | JNI to `ClipboardManager` |
| WebASM | `navigator.clipboard` API |

### PtyManager

Creates platform-appropriate PTY/shell provider.

| Platform | Implementation |
|----------|----------------|
| Desktop | `forkpty()` + user's shell |
| Android | Toybox shell or Termux connection |
| WebASM | JSLinux iframe emulator |
| Windows | ConPTY |

## Event Flow (Desktop)

```
┌─────────────────────────────────────────────────────────────────┐
│ MAIN THREAD                                                      │
│                                                                  │
│  InitManager::run()                                              │
│       │                                                          │
│       ├──► spawn render thread                                   │
│       │                                                          │
│       ▼                                                          │
│  glfwWaitEvents() ◄─── blocks until event arrives               │
│       │                                                          │
│       ▼                                                          │
│  GLFW callback fires (key, mouse, etc.)                         │
│       │                                                          │
│       ▼                                                          │
│  EventQueue::push(event) ────────────────────┐                  │
│       │                                       │                  │
│       └──► loop continues                     │                  │
└───────────────────────────────────────────────│──────────────────┘
                                                │
                                                ▼
┌─────────────────────────────────────────────────────────────────┐
│ RENDER THREAD                                                    │
│                                                                  │
│  GlfwWindowSingleton::instance()  ◄── creates window            │
│       │                                                          │
│       ▼                                                          │
│  Yetty::create() / Yetty::run()                                 │
│       │                                                          │
│       ▼                                                          │
│  libuv event loop                                                │
│       │                                                          │
│       ▼                                                          │
│  uv_async wakes loop  ◄─────────── EventQueue signals           │
│       │                                                          │
│       ▼                                                          │
│  EventLoop::dispatch(event)                                      │
│       │                                                          │
│       ▼                                                          │
│  Listeners process event (gpu-screen, etc.)                     │
│       │                                                          │
│       ▼                                                          │
│  Render frame                                                    │
└─────────────────────────────────────────────────────────────────┘
```

## File Structure

```
include/yetty/platform/
    init-manager.h          # Singleton - platform init, thread management
    input-manager.h         # ThreadSingleton - empty interface
    surface-manager.h       # ThreadSingleton - window properties, surface
    fs-path-manager.h
    clipboard-manager.h
    pty-manager.h

src/yetty/platform/
    shared/
        glfw-init-manager.cpp       # InitManager for GLFW
        glfw-window-singleton.cpp   # Window creation, callbacks
        glfw-window-singleton.h     # Internal header
    input-manager/
        glfw.cpp            # Empty - just instantiates
        android.cpp
    surface-manager/
        glfw.cpp            # Gets window from GlfwWindowSingleton
        android.cpp
    ...
```

## Implementation Status

| Manager | Linux | macOS | Windows | Android | WebASM |
|---------|-------|-------|---------|---------|--------|
| InitManager | DONE | shared | shared | N/A | N/A |
| GlfwWindowSingleton | DONE | shared | shared | N/A | N/A |
| SurfaceManager | DONE | shared | shared | DONE | DONE |
| InputManager | DONE | shared | shared | DONE | DONE |
| FsPathManager | DONE | shared | DONE | DONE | DONE |
| ClipboardManager | DONE | shared | shared | stub | DONE |
| PtyManager | DONE | shared | DONE | stub | DONE |

**Legend:**
- **DONE**: Fully implemented with new factory pattern
- **shared**: Uses same code as Linux (GLFW platforms share code)
- **stub**: Factory exists but implementation incomplete (e.g., JNI needed)
- **N/A**: Not applicable (WebASM is single-threaded, no InitManager needed)

### Notes

**Linux/macOS/Windows (GLFW platforms):**
- Share `glfw-init-manager.cpp` and `glfw-window-singleton.cpp`
- macOS uses CoreText for fonts, Windows uses DirectWrite
- PTY: Linux/macOS use `forkpty()`, Windows uses ConPTY

**Android:**
- No InitManager (android_main handles lifecycle)
- AndroidAppSingleton provides access to `android_app*`
- Input via `yetty_android_dispatch_input()` C function
- PTY via toybox telnet or Termux connection

**WebASM:**
- Single-threaded, no InitManager or EventQueue needed
- Input via Emscripten HTML5 callbacks
- PTY via JSLinux iframe with postMessage

## File Structure

```
include/yetty/platform/
    init-manager.h          # ThreadSingleton - platform init, thread management
    input-manager.h         # ThreadSingleton - raw->internal event transform
    surface-manager.h       # ThreadSingleton - window properties, surface
    fs-path-manager.h       # ThreadSingleton - asset paths
    clipboard-manager.h     # ThreadSingleton - clipboard access
    pty-manager.h           # ThreadSingleton - PTY creation

src/yetty/platform/
    shared/
        glfw-init-manager.cpp       # InitManager for GLFW (Linux/macOS/Windows)
        glfw-window-singleton.cpp   # Window creation, callbacks
        glfw-window-singleton.h     # Internal header
        android-app-singleton.cpp   # AndroidAppSingleton
        android-app-singleton.h     # Internal header
    input-manager/
        glfw.cpp            # GLFW: Ctrl+letter -> control char, scroll -> zoom
        android.cpp         # Android: touch -> mouse, pinch -> zoom
        webasm.cpp          # Emscripten HTML5 callbacks
    surface-manager/
        glfw.cpp            # Gets window from GlfwWindowSingleton
        android.cpp         # Gets window from AndroidAppSingleton
        webasm.cpp          # Canvas element sizing
    fs-path-manager/
        unix.cpp            # Linux/macOS: XDG paths
        windows.cpp         # Windows: %LOCALAPPDATA%
        android.cpp         # Android data directory
        webasm.cpp          # /assets preload paths
    clipboard-manager/
        glfw.cpp            # GLFW clipboard (Linux/macOS/Windows)
        android.cpp         # stub (JNI needed)
        webasm.cpp          # navigator.clipboard API
    pty-manager/
        unix.cpp            # Linux/macOS: forkpty()
        windows.cpp         # Windows: ConPTY
        android.cpp         # stub (toybox/termux)
        webasm.cpp          # JSLinux iframe
    obsolete/               # Old monolithic Platform implementations
        glfw/
        android/
        windows/
        webasm/
```

## Build Integration

CMake adds platform-specific sources. See `build-tools/cmake/targets/*.cmake`:

```cmake
# linux.cmake
set(YETTY_PLATFORM_SOURCES
    platform/shared/glfw-init-manager.cpp
    platform/shared/glfw-window-singleton.cpp
    platform/input-manager/glfw.cpp
    platform/surface-manager/glfw.cpp
    platform/pty-manager/unix.cpp
)
```
