# Credits & Acknowledgments

Yetty stands on the shoulders of giants. This project would not be possible without the incredible work of many individuals and communities who have shared their knowledge, code, and inspiration with the world.

## Inspirations & Key Contributors

### Inigo Quilez
For his pioneering work on **Signed Distance Functions (SDF)**. His articles, ShaderToy creations, and educational content have been invaluable for understanding and implementing GPU-based 2D/3D rendering techniques.
- [iquilezles.org](https://iquilezles.org/)

### Viktor Chlumský
For **msdfgen** - the multi-channel signed distance field generator that enables crisp, scalable font rendering at any zoom level. This technology is at the heart of Yetty's text rendering system.
- [github.com/Chlumsky/msdfgen](https://github.com/Chlumsky/msdfgen)

### Henrik Nyman
For inspiring our approach to **compute shader-based MSDF generation**. His work demonstrated the potential of moving font atlas generation to the GPU.

### WebGPU Authors & Contributors
The teams behind **WebGPU**, **wgpu-native**, and **Dawn** for creating a modern, cross-platform graphics API that makes projects like Yetty possible. Their work enables us to target native platforms and the web with a single rendering backend.
- [WebGPU Specification](https://www.w3.org/TR/webgpu/)
- [wgpu-native](https://github.com/gfx-rs/wgpu-native)
- [Dawn](https://dawn.googlesource.com/dawn)

### PDFium
The **PDFium** project, originally developed by Foxit and open-sourced by Google, powers Yetty's PDF rendering capabilities.
- [pdfium.googlesource.com](https://pdfium.googlesource.com/pdfium/)

## Open Source Community

We're grateful to the maintainers and contributors of all the libraries listed in [DEPENDENCIES.md](../DEPENDENCIES.md). Each one represents countless hours of work that we benefit from daily.

---

*If you believe someone should be acknowledged here and isn't, please open an issue or PR. We want to give credit where it's due.*
