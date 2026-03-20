# Platform Abstraction Layer

Yetty runs on desktop (Linux/macOS/Windows), Android, and WebAssembly. Each platform has different APIs for input, windowing, filesystem, clipboard, and shell access. The platform layer abstracts these differences so the core terminal engine (gpu-screen, terminal, cards) remains platform-agnostic.

## Threading Model

**Desktop (GLFW) and Android share the same pattern:**
- **Main thread**: Runs `InitManager` - platform init, spawns render thread, blocks on platform event loop
- **Render thread**: Creates surface, initializes rendering, runs libuv EventLoop

**Desktop (GLFW):**
- Main thread: `glfwInit()`, spawn render thread, block on `glfwWaitEvents()`
- Render thread: create window via `GlfwWindowSingleton`, create WebGPU surface, run libuv EventLoop
- GLFW callbacks push events to `EventQueue`, libuv wakes via `uv_async`

**Android:**
- Main thread: wait for window (system creates it), spawn render thread, block on `ALooper_pollAll()`
- Render thread: create WebGPU surface (window already exists), run libuv EventLoop
- ALooper callbacks push events to `EventQueue`, libuv wakes via `uv_async`

**Key difference:** On desktop, window is created on render thread. On Android, window is created by the system - we wait for `APP_CMD_INIT_WINDOW` before spawning render thread.

**WebASM:**
- **Single thread**: Browser event loop drives everything via `requestAnimationFrame`
- No render thread, no libuv - uses custom webasm EventLoop
- Uses InitManager for input callbacks (same pattern as other platforms)

## EventLoop

EventLoop runs on the **render thread** and uses **libuv** on all platforms except WebASM.

| Platform | EventLoop Implementation |
|----------|--------------------------|
| Desktop (Linux/macOS/Windows) | libuv (`uv_run`) |
| Android | libuv (`uv_run`) |
| WebASM | Custom (`requestAnimationFrame`) |

The main thread's platform event loop (GLFW/ALooper) pumps raw input events into `EventQueue`. EventQueue uses `uv_async_send()` to wake the render thread's libuv loop.

## Managers

### InitManager

**ObjectFactory** (created once in main). Entry point for platform initialization.

| Platform | Responsibilities |
|----------|------------------|
| Desktop (GLFW) | `glfwInit()`, spawn render thread, block on `glfwWaitEvents()` |
| Android | Wait for window, spawn render thread, block on `ALooper_pollAll()` |
| WebASM | Setup RAF loop, returns immediately (browser takes over) |

```cpp
// main.cpp (all platforms)
auto init = InitManager::create();
init->run([&]() {
    // === RENDER THREAD (or inline on WebASM) ===
    auto yetty = Yetty::create(argc, argv);
    EventLoop::instance()->start();
    yetty->shutdown();
});
```

### GlfwWindowSingleton

**ThreadSingleton** (render thread). Creates and owns the GLFW window. Desktop only.

- Created on render thread (first `instance()` call)
- Calls `glfwCreateWindow()`
- Sets up GLFW callbacks that push to `EventQueue`
- Provides window handle for SurfaceManager

```cpp
// On render thread (desktop only)
auto window = GlfwWindowSingleton::instance();
GLFWwindow* w = window->getWindow();
```

### AndroidAppSingleton

**ThreadSingleton**. Holds `android_app*` pointer. Android only.

- Initialized in `InitManager::init()` with the `android_app*` from `android_main`
- Provides access to `ANativeWindow*` for surface creation
- Input callbacks registered in InitManager

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
Android implementation gets window from `AndroidAppSingleton`.

### InputManager

**ThreadSingleton** (render thread). Transforms raw input events into internal events.

Main thread callbacks push RAW events to `EventQueue`. `InputManager` subscribes to these events and generates internal events:

| Raw Event | Transformation | Internal Event |
|-----------|----------------|----------------|
| KeyDown + Ctrl + letter | Ctrl+A=0x01, Ctrl+B=0x02, ..., Ctrl+Z=0x1A | charInput |

**Why here?** GLFW does NOT fire `charCallback` for control characters (Ctrl+A, Ctrl+C, etc). The transformation happens in InputManager so it can be platform-specific.

### All Managers Summary

| Manager | Thread | Purpose |
|---------|--------|---------|
| InitManager | main | Platform init, spawn render thread, platform event loop |
| GlfwWindowSingleton | render | Create window, setup callbacks (desktop only) |
| AndroidAppSingleton | any | Hold android_app* pointer (Android only) |
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
│  libuv EventLoop                                                 │
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

## Event Flow (Android)

```
┌─────────────────────────────────────────────────────────────────┐
│ MAIN THREAD                                                      │
│                                                                  │
│  InitManager::run()                                              │
│       │                                                          │
│       ├──► wait for window (ALooper_pollAll until window ready) │
│       │                                                          │
│       ├──► spawn render thread                                   │
│       │                                                          │
│       ▼                                                          │
│  ALooper_pollAll(-1) ◄─── blocks until event arrives            │
│       │                                                          │
│       ▼                                                          │
│  source->process() ──► handleAppCmd / handleInputEvent          │
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
│  SurfaceManager::createWGPUSurface()  ◄── window from system    │
│       │                                                          │
│       ▼                                                          │
│  Yetty::create() / Yetty::run()                                 │
│       │                                                          │
│       ▼                                                          │
│  libuv EventLoop                                                 │
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
    init-manager.h          # ObjectFactory - platform init, thread management
    input-manager.h         # ThreadSingleton - raw->internal event transform
    surface-manager.h       # ThreadSingleton - window properties, surface
    fs-path-manager.h       # ThreadSingleton - asset paths
    clipboard-manager.h     # ThreadSingleton - clipboard access
    pty-manager.h           # ThreadSingleton - PTY creation
    webgpu-manager.h        # ThreadSingleton - WebGPU init, device, surface
    event-loop.h            # ThreadSingleton - libuv event loop (render thread)

src/yetty/platform/
    shared/
        glfw-window-singleton.cpp   # Window creation, GLFW callbacks
        glfw-window-singleton.h     # Internal header
        android-app-singleton.cpp   # AndroidAppSingleton
        android-app-singleton.h     # Internal header
    init-manager/
        linux.cpp           # Linux: GLFW + X11/Wayland
        macos.cpp           # macOS: GLFW + Cocoa
        windows.cpp         # Windows: GLFW + Win32
        android.cpp         # Android: ALooper event loop
        webasm.cpp          # WebASM: Emscripten RAF loop
    event-loop/
        libuv.cpp           # Desktop + Android: libuv on render thread
        webasm.cpp          # WebASM: requestAnimationFrame
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
    webgpu-manager/
        linux.cpp           # Dawn/Vulkan
        macos.cpp           # Dawn/Metal
        windows.cpp         # Dawn/D3D12 with D3D11 fallback
        android.cpp         # Vulkan required, reject Null backend
        webasm.cpp          # Async with emscripten_sleep
```

## Implementation Status

| Manager | Linux | macOS | Windows | Android | WebASM |
|---------|-------|-------|---------|---------|--------|
| InitManager | DONE | DONE | DONE | DONE | DONE |
| GlfwWindowSingleton | DONE | shared | shared | N/A | N/A |
| AndroidAppSingleton | N/A | N/A | N/A | DONE | N/A |
| SurfaceManager | DONE | shared | shared | DONE | DONE |
| InputManager | DONE | shared | shared | DONE | DONE |
| FsPathManager | DONE | shared | DONE | DONE | DONE |
| ClipboardManager | DONE | shared | shared | stub | DONE |
| PtyManager | DONE | shared | DONE | stub | DONE |
| WebGpuManager | DONE | DONE | DONE | DONE | DONE |
| EventLoop | DONE (libuv) | shared | shared | shared (libuv) | DONE (RAF) |

**Legend:**
- **DONE**: Fully implemented
- **shared**: Uses same code as Linux (libuv for EventLoop, GLFW for desktop)
- **stub**: Factory exists but implementation incomplete (e.g., JNI needed)
- **N/A**: Not applicable

## Build Integration

CMake adds platform-specific sources. EventLoop uses libuv.cpp for all platforms except WebASM.

```cmake
# src/yetty/base/CMakeLists.txt
if(EMSCRIPTEN)
    set(EVENT_LOOP_SRC platform/event-loop/webasm.cpp)
else()
    set(EVENT_LOOP_SRC platform/event-loop/libuv.cpp)
endif()
```

## Current State (2026-03-19)

### Architecture

**main.cpp is clean** - platform dispatch only. All Yetty lifecycle (create, run, shutdown) is handled inside `InitManager::run()`.

```cpp
// main.cpp pattern for ALL platforms:
auto initResult = InitManager::create();  // or createForAndroid(app)
if (!initResult) { /* error */ }
auto initManager = *initResult;
initManager->run(argc, argv);  // Everything happens inside
```

### InitManager Implementations

| Platform | File | Threading | Yetty Lifecycle |
|----------|------|-----------|-----------------|
| Desktop (Linux/macOS/Windows) | `init-manager/glfw.cpp` | Render thread spawned | Create window, create Yetty, run, shutdown |
| Android | `init-manager/android.cpp` | Render thread spawned | Wait for window, create Yetty, EventLoop::start, shutdown |
| WebASM | `init-manager/webasm.cpp` | Single-threaded | Create Yetty, register input callbacks, run (never returns) |

### WebASM Input Handling

WebASM is single-threaded - no EventQueue needed. Input callbacks registered in `InitManager::run()` BEFORE `yettyInstance->run()` (which never returns due to RAF loop).

```
InitManager::run():
1. Create Yetty (creates EventLoop singleton)
2. Register emscripten input callbacks -> dispatch to EventLoop::instance()
3. Call yettyInstance->run() (sets up RAF, never returns)
```

### File Changes from Main Branch

- **Deleted**: `init-manager/linux.cpp`, `init-manager/macos.cpp`, `init-manager/windows.cpp`
- **Added**: `init-manager/glfw.cpp` (shared by all GLFW desktop platforms)
- **Modified**: All InitManager implementations now create/run/shutdown Yetty internally
