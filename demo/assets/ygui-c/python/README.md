# YGui-C Python Demos

Python demos for YGui-C widget library, from simple to complex.

## Prerequisites

1. Build the YGui-C shared library:
   ```bash
   cd build
   cmake ..
   make ygui-c-shared
   ```

2. The library should be available as `libygui.so`

## Demos

### Basic (01-04)

- **01_hello_button.py** - Single button, basic event handling
- **02_label_and_button.py** - Labels + buttons, click counter
- **03_slider.py** - Slider with live value display
- **04_checkbox.py** - Multiple checkboxes for options

### Intermediate (05-07)

- **05_panel_layout.py** - Panel with nested widgets
- **06_progress_bar.py** - Animated progress bar with controls
- **07_calculator.py** - Calculator UI with button grid

### Advanced (08-10)

- **08_color_mixer.py** - RGB sliders with color preview
- **09_todo_list.py** - Dynamic widget creation/removal
- **10_dashboard.py** - Complete dashboard with multiple panels

## Running

```bash
cd demo/ygui-c/python
python3 01_hello_button.py
```

## Notes

- Demos are self-contained and simulate user interactions
- No actual window is displayed (headless) - output goes to console
- For visual output, integrate with a renderer (VNC, framebuffer, etc.)
