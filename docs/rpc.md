# Yetty RPC Interface

Yetty exposes a msgpack-RPC interface over Unix domain sockets for external control and automation.

## Socket Location

The RPC server listens on a Unix domain socket:
- **Path:** `$XDG_RUNTIME_DIR/yetty/yetty-<pid>.sock` (Linux)
- **Windows:** `\\.\pipe\yetty-<pid>`

The socket path is exported via the `YETTY_SOCKET` environment variable for child processes.

## Wire Protocol

The protocol follows msgpack-RPC with a channel extension:

```
Request:      [0, msgid, channel, method, params]
Response:     [1, msgid, error, result]
Notification: [2, channel, method, params]
```

- **msgid:** Unique request identifier for correlating responses
- **channel:** Subsystem selector (see Channels below)
- **method:** String method name
- **params:** msgpack map of parameters
- **error:** nil on success, string error message on failure
- **result:** Method-specific return value

## Channels

### Channel 0: EventLoop

Dispatch events to the yetty event loop. Used for input simulation and UI queries.

#### Keyboard Events

| Method | Parameters | Description |
|--------|------------|-------------|
| `key_down` | `{key: int, mods: int, scancode: int}` | Key press |
| `key_up` | `{key: int, mods: int, scancode: int}` | Key release |
| `char` | `{codepoint: uint32, mods: int}` | Character input |

#### Mouse Events (Screen-level)

| Method | Parameters | Description |
|--------|------------|-------------|
| `mouse_down` | `{x: float, y: float, button: int}` | Mouse button press |
| `mouse_up` | `{x: float, y: float, button: int}` | Mouse button release |
| `mouse_move` | `{x: float, y: float}` | Mouse move |
| `mouse_drag` | `{x: float, y: float, button: int}` | Mouse drag |
| `scroll` | `{x: float, y: float, dx: float, dy: float, mods: int}` | Scroll event |

#### Card Mouse Events

| Method | Parameters | Description |
|--------|------------|-------------|
| `card_mouse_down` | `{target_id: uint64, x: float, y: float, button: int}` | Card click |
| `card_mouse_up` | `{target_id: uint64, x: float, y: float, button: int}` | Card release |
| `card_mouse_move` | `{target_id: uint64, x: float, y: float}` | Card hover |
| `card_scroll` | `{target_id: uint64, x: float, y: float, dx: float, dy: float}` | Card scroll |

#### Window/Focus Events

| Method | Parameters | Description |
|--------|------------|-------------|
| `set_focus` | `{object_id: uint64}` | Set focus to object |
| `resize` | `{width: float, height: float}` | Window resize |
| `close` | `{object_id: uint64}` | Close object (request) |
| `split` | `{object_id: uint64, orientation: int}` | Split pane (request) |

#### Queries

| Method | Parameters | Returns | Description |
|--------|------------|---------|-------------|
| `ui_tree` | `{}` | `string` | YAML dump of UI tree |

### Channel 1: CardStream

Card buffer and texture streaming for external renderers.

| Method | Parameters | Returns | Description |
|--------|------------|---------|-------------|
| `stream_connect` | `{card_name: string}` | `{stream_id: int, ...}` | Connect to card stream |
| `stream_get_buffer` | `{stream_id: int}` | Buffer data | Get current frame |
| `stream_disconnect` | `{stream_id: int}` | `bool` | Disconnect stream |
| `stream_mark_dirty` | `{stream_id: int}` | `bool` | Mark for re-render |
| `cards_list` | `{}` | Array of card info | List all cards |
| `buffers_list` | `{}` | Array of buffer info | List all buffers |

## Client Libraries

### C++ Client

```cpp
#include <yetty/rpc/rpc-client.h>

// Synchronous client
auto client = yetty::rpc::RpcClient::create(socketPath);
client->connect();
client->mouseDown(100.0f, 200.0f, 0);  // Left click at (100, 200)
auto tree = client->uiTree();           // Get UI tree

// Async client (with libuv loop)
auto client = yetty::rpc::RpcClient::create(socketPath, uvLoop);
client->connect();
client->keyDown(GLFW_KEY_A, 0, 0);
```

### Python Client (Example)

```python
import socket
import msgpack

def connect(socket_path):
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    sock.connect(socket_path)
    return sock

def request(sock, channel, method, params):
    msgid = 1
    msg = [0, msgid, channel, method, params]
    sock.send(msgpack.packb(msg))
    response = msgpack.unpackb(sock.recv(4096))
    # response = [1, msgid, error, result]
    if response[2] is not None:
        raise Exception(response[2])
    return response[3]

def notify(sock, channel, method, params):
    msg = [2, channel, method, params]
    sock.send(msgpack.packb(msg))

# Usage
sock = connect(os.environ['YETTY_SOCKET'])
notify(sock, 0, 'mouse_down', {'x': 100.0, 'y': 200.0, 'button': 0})
tree = request(sock, 0, 'ui_tree', {})
```

---

## Integration Testing

The RPC interface enables automated integration testing of yetty and ygui-c applications.

### Architecture

```
┌──────────────────┐                    ┌──────────────────┐
│   Test Runner    │◄──── RPC ─────────►│      Yetty       │
│  (Python/C/etc)  │     Socket         │   (terminal)     │
└──────────────────┘                    └────────┬─────────┘
                                                 │ PTY
                                                 ▼
                                        ┌──────────────────┐
                                        │   YGui-C App     │
                                        │  (test subject)  │
                                        └──────────────────┘
```

### Test Flow

1. **Start yetty** with a test application
2. **Connect to RPC socket** via `YETTY_SOCKET`
3. **Query state** (cell size, card info, UI tree)
4. **Simulate input** (clicks, keys)
5. **Verify results** (query state again, check OSC output)

### Proposed Testing Extensions

To support coordinate transformation testing, the following handlers should be added:

#### `get_cell_size`

Returns the current cell dimensions in pixels.

```
Request:  {channel: 0, method: "get_cell_size", params: {}}
Response: {width: 8, height: 16}
```

#### `get_card_info`

Returns detailed information about a card by name.

```
Request:  {channel: 0, method: "get_card_info", params: {name: "my-card"}}
Response: {
    id: 12345,
    name: "my-card",
    display_x: 16.0,      // Card position in display pixels
    display_y: 32.0,
    display_w: 496.0,     // Card size in display pixels
    display_h: 288.0,
    cell_cols: 62,        // Card size in cells
    cell_rows: 18,
    canvas_w: 500.0,      // Internal canvas dimensions
    canvas_h: 300.0
}
```

#### `compute_click_coords`

Computes what coordinates yetty would send via OSC 777777 for a click at given display position.

```
Request:  {channel: 0, method: "compute_click_coords", params: {
    name: "my-card",
    display_x: 218.0,
    display_y: 70.0
}}
Response: {
    osc_x: 218,           // Integer coords sent via OSC
    osc_y: 70,
    card_local_x: 218.0,  // Float coords relative to card origin
    card_local_y: 70.0
}
```

### Example: Testing Click Boundaries

```python
#!/usr/bin/env python3
"""Test that button click boundaries are correct."""

import os
import socket
import msgpack
import subprocess
import time

def rpc_request(sock, method, params):
    msg = [0, 1, 0, method, params]  # channel=0 (EventLoop)
    sock.send(msgpack.packb(msg))
    resp = msgpack.unpackb(sock.recv(4096))
    assert resp[2] is None, f"RPC error: {resp[2]}"
    return resp[3]

def test_button_right_edge():
    # Start yetty with test app
    proc = subprocess.Popen(
        ['./build/yetty', '-e', './demo/scripts/ygui-c/python/00-debug-events.sh'],
        env={**os.environ, 'YETTY_TEST_MODE': '1'}
    )
    time.sleep(2)  # Wait for startup

    # Connect to RPC
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    sock.connect(os.environ['YETTY_SOCKET'])

    try:
        # Get card info
        info = rpc_request(sock, 'get_card_info', {'name': 'debug-card'})
        display_w = info['display_w']
        canvas_w = info['canvas_w']  # 500.0

        # Button at canvas (20, 50) size (200, 50)
        # Right edge in canvas space: x = 220
        # Right edge in display space:
        button_right_display = 220.0 * display_w / canvas_w

        # Test click just inside right edge
        coords = rpc_request(sock, 'compute_click_coords', {
            'name': 'debug-card',
            'display_x': button_right_display - 1,
            'display_y': 70.0
        })

        # Verify: these should map back to inside the button
        # ygui-c does: canvas_x = (osc_x / display_w) * canvas_w
        canvas_x = coords['osc_x'] * canvas_w / display_w
        assert 20 <= canvas_x < 220, f"Click should hit button, got canvas_x={canvas_x}"

        print("PASS: Right edge click hits button")

        # Test click just outside right edge
        coords = rpc_request(sock, 'compute_click_coords', {
            'name': 'debug-card',
            'display_x': button_right_display + 1,
            'display_y': 70.0
        })

        canvas_x = coords['osc_x'] * canvas_w / display_w
        assert canvas_x >= 220, f"Click should miss button, got canvas_x={canvas_x}"

        print("PASS: Outside right edge misses button")

    finally:
        sock.close()
        proc.terminate()
        proc.wait()

if __name__ == '__main__':
    test_button_right_edge()
```

### Test Harness (C)

For C-based integration tests, a minimal harness:

```c
// test/integration/yetty_harness.h

typedef struct {
    pid_t pid;
    int rpc_fd;
    char socket_path[256];
} yetty_harness_t;

// Start yetty with given command, wait for RPC socket
int yetty_harness_start(yetty_harness_t* h, const char* cmd);

// Stop yetty process
void yetty_harness_stop(yetty_harness_t* h);

// RPC queries
int yetty_get_cell_size(yetty_harness_t* h, int* width, int* height);
int yetty_get_card_info(yetty_harness_t* h, const char* name,
                        float* display_w, float* display_h,
                        float* canvas_w, float* canvas_h);

// Simulate click and get resulting OSC coordinates
int yetty_simulate_click(yetty_harness_t* h, const char* card_name,
                         float display_x, float display_y,
                         int* osc_x, int* osc_y);
```

### Implementation Checklist

1. **Yetty side:**
   - [x] Add `get_cell_size` handler in `event-loop-handler.cpp`
   - [x] Add `get_card_info` handler (needs GPUScreen access)
   - [x] Add `compute_click_coords` handler
   - [x] Expose handlers via existing RPC server

2. **Test harness:**
   - [x] Create `test/integration/test_rpc_handlers.py` (Python)
   - [ ] Create `test/integration/yetty_harness.{h,c}` (C - optional)
   - [ ] Add msgpack encoding/decoding helpers (C - optional)

3. **Tests:**
   - [ ] Click boundary tests (the current bug)
   - [ ] Coordinate transformation tests
   - [ ] Cell size change handling
