<p align="center">
  <img src="docs/logo.jpeg" alt="Yetty Logo" width="200">
</p>

# Yetty - Next-Generation Terminal Emulator

A WebGPU-powered terminal emulator that breaks the boundaries of what terminals can display.

> **🚀 Like what you see?** We're looking for contributors! Whether it's code, documentation, or ideas - all contributions are welcome. Share your suggestions and opinions on [GitHub Discussions](https://github.com/zokrezyl/yetty/discussions).

> **🛠️ Development Status:** We're developing intensively and moving fast. We admit that test coverage is still limited, but we're committed to improving it. Contributions to testing are especially welcome!

## The Problem We're Solving

Traditional terminals are stuck in the 1970s. They can display text, maybe some colors, and that's about it. While the rest of computing has evolved to support rich graphics, animations, and interactive content, terminals remain fundamentally limited to a character grid.

**Yetty changes this.**

## What Makes Yetty Different

### Card-Based Rendering System

Yetty introduces **Cards** - rich content widgets that live within the terminal grid and scroll with your content, just like text. Cards can display anything:

- **Images** - Inline images that render at native resolution
- **PDFs** - View PDF documents directly in your terminal
- **Plots** - Real-time GPU-accelerated data visualizations
- **Vector Graphics** - SVG and Lottie animations via ThorVG
- **Interactive Widgets** - ImGui-based UI elements (Ymery)
- **2D Drawing** - SDF primitives with spatial-hash-accelerated rendering (YDraw)
- **Python Graphics** - matplotlib-style plots and custom visualizations

Cards occupy cells in the terminal grid. When the terminal scrolls, card content scrolls with it - just like cells in a Jupyter notebook. This enables workflows never possible before:

```
$ python train_model.py
Training epoch 1/100...
[Live loss plot rendered here, scrolling with output]
Training epoch 2/100...
[Another visualization]
...
```

### Multi-Layer Glyph Architecture

Yetty renders text using multiple glyph types, each optimized for different use cases:

| Glyph Type | Description | Use Case |
|------------|-------------|----------|
| **MSDF** | Multi-channel Signed Distance Field fonts | Primary text - crisp at any zoom |
| **Monospace MSDF** | Fixed-width MSDF glyphs | Terminal grid alignment |
| **Bitmap** | RGBA texture atlas | Color emojis 🎉 and image-based glyphs |
| **Shader Glyphs** | Procedurally rendered via GPU | Animated spinners, progress indicators |
| **Card Glyphs** | Multi-cell shader-rendered regions | Cards (images, plots, widgets) |

The shader glyph system uses Unicode Private Use Area codepoints (Plane 16) to encode card metadata directly in terminal cells, enabling seamless integration with libvterm's scrollback buffer.

### GPU-Accelerated Everything

- **WebGPU Backend** - Modern, cross-platform GPU acceleration via wgpu-native/Dawn
- **MSDF Font Rendering** - Crisp, scalable text at any zoom level
- **60fps Animations** - Smooth shader animations and transitions
- **Compute Shaders** - Text layout and image scaling on GPU
- **Runs Everywhere** - Native (Linux, macOS, Windows) and Web (WASM)

## Communication Protocols

Yetty supports multiple protocols for creating and managing cards:

### OSC Sequences
Cards can be created via terminal escape sequences (OSC 666666), allowing any program to spawn cards:

```bash
# Create an image card
printf '\033]666666;create -w 40 -h 20 -p image;;base94_encoded_png\033\\'
```

### RPC Server
A Unix domain socket RPC server (msgpack-rpc) enables out-of-band communication for:
- Creating/updating cards without escape sequences
- Streaming data to live cards
- Integration with language servers and external tools

## Built-in Cards

| Card | Description |
|------|-------------|
| `image` | Display PNG, JPEG, WebP images |
| `pdf` | Render PDF documents (via PDFium/MuPDF) |
| `plot` | Line, bar, scatter, area plots |
| `ydraw` | 2D vector graphics with SDF primitives and text fragments with different MSDF font sizes |
| `ymery` | ImGui-based interactive widgets |
| `thorvg` | SVG and Lottie animation rendering |
| `python` | Python-driven visualizations |

## Roadmap: Plugin API

We're working on a clear, stable API for third-party card plugins. The goal is to let developers create custom cards that:

- Integrate seamlessly with the terminal grid and scrollback
- Have access to GPU resources (buffers, textures, compute shaders)
- Receive mouse/keyboard input
- Can be hot-reloaded during development

Stay tuned for the Plugin SDK documentation.

## Documentation

- **[Font System](docs/fonts.md)** - MSDF fonts, emoji rendering, shader glyphs, and GPU text rendering
- **[Card System](docs/cards.md)** - Card architecture, OSC sequences, texture atlas, storage buffers, and YDraw spatial hashing
- **[Shader Effects](docs/effects.md)** - Pre/post-processing effects, CRT simulation, Matrix rain, and custom effect creation
- **[Credits](docs/credits.md)** - Acknowledgments to the projects and people that made Yetty possible

## Technical Highlights

> ⚠️ **Note:** We currently build with the **Dawn** WebGPU backend. The native wgpu-native backend may not build successfully at this time.

### Core Dependencies
- **libvterm** - VT100/xterm terminal emulation
- **wgpu-native/Dawn** - WebGPU abstraction layer
- **FreeType + msdfgen** - Font rasterization and MSDF atlas generation
- **GLFW** - Cross-platform windowing
- **libuv** - Async I/O and event loop

### Optional Dependencies
- **ThorVG** - SVG and Lottie rendering
- **PDFium/MuPDF** - PDF rendering
- **ImGui** - Immediate-mode GUI for Ymery cards
- **Python** - Embedded interpreter for Python cards

## Building

Build targets are defined in the wrapper `Makefile`. The suggested build command is:

```bash
# Recommended: Debug build with Dawn backend
make build-desktop-dawn-debug

# Release build
make build-desktop-dawn-release

# List all available targets
make help
```


## Usage

```bash
# Run with default shell
./build-desktop-dawn-relase/yetty

# Run with specific command
SHELL=/bin/zsh ./yetty

## License

Yetty is licensed under the **MIT License** - see [LICENSE](LICENSE) for details.

### Dependencies

All dependencies use MIT-compatible licenses (MIT, BSD, Zlib, Apache-2.0). See [DEPENDENCIES.md](DEPENDENCIES.md) for the complete list.

---

*Yetty: Your terminal, unchained.*
