# Architecture

Yetty is a GPU-accelerated terminal emulator using WebGPU with the Dawn backend.

## Core Components

**Entry Points** (`/src/yetty/`):
- `main.cpp` - Platform dispatch (desktop/android/web)
- `yetty.cpp` - Main `YettyImpl` class orchestrating everything

**Terminal Engine**:
- `terminal.cpp` - VT100/xterm emulation via libvterm
- `gpu-screen.cpp` - GPU-accelerated rendering interface
- `osc-scanner.cpp`, `osc-command.cpp` - OSC escape sequence handling
- `pty-reader-*.cpp` - Platform-specific PTY reading (desktop/web/windows)

**Font System** (`/src/yetty/font/`):
Multi-layer architecture with MSDF as primary renderer:
- MSDF (Multi-channel Signed Distance Field) - Main text rendering
- Vector SDF/Coverage - Real-time curve rendering (U+F0000-U+F1FFF)
- Bitmap/Emoji - Color texture glyphs
- Shader Glyphs - Procedural rendering (spinners, effects)
- Card Glyphs - Multi-cell widgets (U+100000+)

**Card System** (`/src/yetty/cards/`):
Rich widgets that scroll with terminal content:
- `image/` - PNG, JPEG, WebP
- `plot/` - GPU-accelerated data visualization
- `ydraw/` - 2D vector graphics with SDF primitives
- `pdf/`, `ypdf/` - PDF rendering
- `ymery/` - ImGui-based interactive widgets
- `yhtml/` - HTML/CSS rendering via litehtml
- `markdown/`, `qrcode/`, `diagram/`

**Platform Abstraction** (`/src/yetty/platform/`):
- `glfw-platform.cpp` - Desktop (Linux/macOS/Windows)
- `web-platform.cpp` - Emscripten/WebAssembly
- `android-platform.cpp` - Android native
- `windows-platform.cpp` - Windows-specific

**RPC Server** (`/src/yetty/rpc/`):
msgpack-RPC over Unix socket (`~/.yetty/socket`) for out-of-band card streaming.

**VNC Protocol** (`/src/yetty/vnc/`):
VNC server/client for remote terminal access.

**Shaders** (`/src/yetty/shaders/`):
WGSL shaders - main rendering in `gpu-screen.wgsl`, card-specific shaders in subdirectories.

## Key Subsystems

- `workspace.cpp`, `tile.cpp` - Multi-tile workspace management
- `frame-renderer.cpp` - Frame rendering pipeline
- `base/event-loop.cpp` - Core event loop using libuv
- `imgui-manager.cpp` - ImGui integration

## Key Dependencies

- libvterm 0.3.3 - Terminal emulation
- Dawn (WebGPU) - GPU graphics
- GLFW 3.4 - Window management
- libuv - Async I/O and event loop
- FreeType + msdfgen - Font rendering
- msgpack-c - RPC serialization

## Related Documentation

- [fonts.md](fonts.md) - Font system architecture
- [cards.md](cards.md) - Card system and OSC sequences
- [effects.md](effects.md) - Pre/post-processing effects
- [ydraw.md](ydraw.md) - 2D vector graphics
