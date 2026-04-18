Title: Yetty – A WebGPU terminal emulator with inline graphics, plots, and PDFs

  Text:

  I've been working on Yetty, a terminal emulator that uses WebGPU to render not just text, but also images, PDFs, plots, and interactive widgets directly in the terminal grid.

  The core idea is "cards" – rich content blocks that occupy terminal cells and scroll with your output, similar to Jupyter notebooks but in a proper terminal. You can display a
  matplotlib plot, and it scrolls up with your shell history.

  What's implemented:

   - MSDF font rendering (crisp at any zoom)
   - Bitmap glyphs for color emojis
   - Shader glyphs for animated spinners
   - Cards: images, PDFs, plots, vector graphics (SVG/Lottie), ImGui widgets
   - OSC escape sequences and RPC for card creation
   - Builds on Linux (macOS/Windows untested but should work)

  What's honest:

   - Test coverage is limited (we're moving fast)
   - Currently builds with Dawn backend only (wgpu-native may not work)
   - It's not a daily-driver replacement for your terminal yet
   - Documentation is sparse

  Built with libvterm, Dawn/WebGPU, FreeType, msdfgen, and GLFW. MIT licensed.

  Looking for contributors and feedback. If you've ever wished your terminal could show inline graphics without hacks, I'd love to hear your thoughts.

  GitHub: https://github.com/zokrezyl/yetty (https://github.com/zokrezyl/yetty)
