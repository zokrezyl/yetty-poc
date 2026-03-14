# Silent Error Patterns in Yetty Codebase

This report identifies locations where `Result<>` values are returned but errors are silently ignored.

## Explicit (void) Casts on Result-Returning Functions

These are intentional suppressions but may warrant review:

| File | Line | Code |
|------|------|------|
| `src/yetty/main.cpp` | 410 | `(void)state.yetty->iterate();` |
| `src/yetty/main.cpp` | 420 | `(void)state.yetty->shutdown();` |
| `src/yetty/yetty.cpp` | 2196 | `(void)(*base::EventLoop::instance())->stop();` |
| `src/yetty/base/event-queue.cpp` | 41 | `(void)loop->dispatch(_queue.front());` |
| `src/yetty/base/event-queue.cpp` | 116 | `(void)loop->dispatch(events.front());` |

## Result Values Assigned But Potentially Not Checked

These patterns assign Result values and may not check for errors:

### High Priority (Core Initialization)

| File | Line | Code | Notes |
|------|------|------|-------|
| `src/yetty/yetty.cpp` | 1730 | `auto paneResult = workspace->createPane();` | Pane creation - should check |
| `src/yetty/yetty.cpp` | 2062 | `auto inputTimerResult = loop->createTimer();` | Timer creation |
| `src/yetty/yetty.cpp` | 2078 | `auto frameTimerResult = loop->createTimer();` | Timer creation |
| `src/yetty/terminal.cpp` | 238 | `auto ptyResult = platform->createPTY();` | PTY creation - critical |

### Platform/Window Initialization

| File | Line | Code | Notes |
|------|------|------|-------|
| `src/yetty/platform/glfw-platform.cpp` | 84 | `auto pollResult = loop->createPoll();` | Poll creation |
| `src/yetty/telnet/telnet-client.cpp` | 191 | `auto pollRes = loop->createPoll();` | Poll for telnet |
| `src/yetty/vnc/vnc-server.cpp` | 125 | `auto pollResult = loop->createPoll();` | VNC server poll |
| `src/yetty/vnc/vnc-server.cpp` | 1379 | `auto pollResult = loop->createPoll();` | VNC server poll |
| `src/yetty/vnc/vnc-client.cpp` | 234 | `auto pollRes = loop->createPoll();` | VNC client poll |
| `src/yetty/pty-reader-desktop.cpp` | 89 | `auto pollResult = loop->createPoll();` | PTY reader poll |

### Font/Rendering

| File | Line | Code | Notes |
|------|------|------|-------|
| `src/yetty/font/raw-font-manager.cpp` | 61 | `auto res = static_cast<RawFontManagerImpl*>(impl.get())->init();` | Init but not returned? |
| `src/yetty/gpu-memory-manager.cpp` | 156 | `auto result = pool->allocate();` | Memory allocation |

### Video/Media

| File | Line | Code | Notes |
|------|------|------|-------|
| `src/yetty/yvideo/yvideo-decoder.cpp` | 545 | `auto frameRes = _decoder->getFrame();` | Frame decoding |
| `src/yetty/cards/yvideo/yvideo.cpp` | 340 | `auto frameResult = _videoSource->nextFrame();` | Video frame |
| `src/yetty/cards/yvideo/yvideo.cpp` | 394 | `auto frameResult = _videoSource->nextFrame();` | Video frame |
| `src/yetty/cards/yvideo/yvideo.cpp` | 504 | `auto timerResult = loop->createTimer();` | Video timer |

### RPC/Client

| File | Line | Code | Notes |
|------|------|------|-------|
| `src/yetty/client/ui.cpp` | 36 | `auto connResult = client->connect();` | Client connection |
| `src/yetty/client/ui.cpp` | 41 | `auto treeResult = client->uiTree();` | UI tree fetch |
| `src/yetty/client/event.cpp` | 143 | `auto connResult = client->connect();` | Client connection |

### Cards/Plotting

| File | Line | Code | Notes |
|------|------|------|-------|
| `src/yetty/cards/yplot/yplot-card.cpp` | 535 | `auto compileResult = _state->compile();` | Plot compilation |

## Recommendations

1. **Critical paths**: The PTY creation (`terminal.cpp:238`) and pane creation (`yetty.cpp:1730`) should definitely check errors as these are critical for terminal functionality.

2. **Timer/Poll creation**: All `createTimer()` and `createPoll()` calls should check for errors, as failure means the event loop won't function correctly.

3. **Memory allocation**: `gpu-memory-manager.cpp:156` should handle allocation failures gracefully.

4. **Intentional ignores**: The `(void)` casts in Android main loop and event dispatch are likely intentional (best-effort operations), but consider adding comments explaining why errors are acceptable there.

## Pattern to Fix

```cpp
// Before (silent error):
auto result = something->create();
// code continues without checking result

// After (proper error handling):
auto result = something->create();
if (!result) {
    yerror("Failed to create something: {}", error_msg(result));
    return Err<void>("Creation failed", result);
}
```
