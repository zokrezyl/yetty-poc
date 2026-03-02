# VNC Architecture

Yetty includes a custom VNC-like protocol for remote terminal access.

## Principle

**Server**: Renders terminal to framebuffer at desired FPS or when input events arrive from clients. Uses GPU tile-based diff detection to send only changed tiles (64x64 pixels). Tiles are JPEG-compressed when beneficial (compression is synchronous via turbojpeg).

**Client**: Receives tiles, decompresses JPEG (synchronous via turbojpeg), uploads to GPU texture, renders fullscreen quad. Forwards keyboard/mouse events to server.

**All I/O is async via libuv EventLoop.** Sockets are registered with `EventLoop::createPoll()` and callbacks fire via `onEvent()` when data is available.

## Protocol (`src/yetty/vnc/protocol.h`)

```
Server -> Client:
  FrameHeader { magic, width, height, tile_size, num_tiles }
  TileHeader { tile_x, tile_y, encoding, data_size }
  [tile data: RAW BGRA or JPEG]
  ... repeat for each tile ...

Client -> Server:
  InputHeader { type, data_size }
  [event data: MouseMove, KeyDown, Resize, CellSize, etc.]
```

## Working Example: vnc-test-server + vnc-test-client

These work because they properly drive the libuv event loop:

**vnc-test-server** (`src/yetty/vnc/test/vnc-test-server.cpp`):
```cpp
while (g_running) {
    server.processInput();           // Poll clients for input (non-blocking)
    // ... generate pattern, render to framebuffer ...
    server.sendFrame(texture, cpuPixels, width, height);
    std::this_thread::sleep_for(frameInterval);
}
```

**vnc-test-client** (`src/yetty/vnc/test/vnc-test-client.cpp`):
```cpp
while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    uv_run(uvLoop, UV_RUN_NOWAIT);   // <-- Drives libuv, triggers socket callbacks
    vncClient->updateTexture();
    // ... render ...
}
```

## Yetty Integration

The main yetty app (`src/yetty/yetty.cpp`) properly integrates with libuv:

1. **EventLoop is created** via `EventLoop::instance()` singleton
2. **VncServer registers polls** with EventLoop for async accept/read
3. **EventLoop::start()** is called (line ~1742), which runs `uv_run(_loop, UV_RUN_DEFAULT)`
4. **Timer-based rendering**: Frame and input timers drive `mainLoopIteration()` from within the libuv loop

This is different from the standalone test client which uses `UV_RUN_NOWAIT` in a GLFW loop.
Yetty uses libuv as the primary event loop with timers for frame rendering.

## VncServer Async State Machine

`sendFrame()` uses a state machine to avoid blocking on GPU operations:

```
IDLE -> WAITING_CLEAR -> WAITING_COMPUTE -> WAITING_MAP -> READY_TO_SEND
                                                              |
                                    WAITING_TILE_READBACK <---+
```

Each state submits GPU work and returns immediately. On next `sendFrame()` call, it checks `_gpuWorkDone` flag (set by async callback) and advances state.

GPU callbacks use `uv_async_send()` to wake the libuv loop when async GPU work completes.

## VncClient Async I/O

**Receive** uses `RecvState` machine:
```
FRAME_HEADER -> TILE_HEADER -> TILE_DATA -> (repeat or back to FRAME_HEADER)
```

Tiles are uploaded to GPU texture immediately on receipt via `wgpuQueueWriteTexture()`.

**Connect** is fully async:
- Socket set non-blocking BEFORE `connect()`
- `EINPROGRESS` handled - polls for `PollWritable` to detect completion
- `getsockopt(SO_ERROR)` verifies connection success

**Send** is fully async:
- `sendInput()` tries immediate send with `MSG_DONTWAIT`
- On `EAGAIN`, data is queued in `_sendQueue`
- `PollWritable` events trigger `drainSendQueue()` to flush
- Poll events dynamically enabled/disabled via `setPollEvents()`

## Key Files

- `src/yetty/vnc/protocol.h` - Wire protocol definitions
- `src/yetty/vnc/vnc-server.cpp` - Server implementation
- `src/yetty/vnc/vnc-client.cpp` - Client implementation
- `src/yetty/vnc/test/vnc-test-server.cpp` - Working standalone server
- `src/yetty/vnc/test/vnc-test-client.cpp` - Working standalone client
- `src/yetty/base/event-loop.cpp` - libuv wrapper for async I/O
