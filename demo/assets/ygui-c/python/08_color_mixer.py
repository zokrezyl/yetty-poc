#!/usr/bin/env python3
"""
Demo 08: Color Mixer

RGB color mixer with sliders and preview.
"""

import ygui

def rgb_to_abgr(r, g, b, a=255):
    """Convert RGBA to ABGR uint32."""
    return (a << 24) | (b << 16) | (g << 8) | r

def main():
    ygui.init()

    engine = ygui.Engine("color-mixer", width=480, height=380)

    # Title
    engine.label("title", 30, 15, "RGB Color Mixer")

    # Color preview panel
    preview = engine.panel("preview", 280, 55, 160, 140)
    preview.set_bg_color(rgb_to_abgr(128, 128, 128))

    # Hex value label
    hex_label = engine.label("hex_label", 280, 210, "#808080")

    # Red slider
    engine.label("r_label", 30, 55, "Red")
    r_value = engine.label("r_value", 200, 55, "128")
    r_slider = engine.slider("red", 30, 80, 180, 25, 0, 255, 128)

    # Green slider
    engine.label("g_label", 30, 115, "Green")
    g_value = engine.label("g_value", 200, 115, "128")
    g_slider = engine.slider("green", 30, 140, 180, 25, 0, 255, 128)

    # Blue slider
    engine.label("b_label", 30, 175, "Blue")
    b_value = engine.label("b_value", 200, 175, "128")
    b_slider = engine.slider("blue", 30, 200, 180, 25, 0, 255, 128)

    # Alpha slider
    engine.label("a_label", 30, 235, "Alpha")
    a_value = engine.label("a_value", 200, 235, "255")
    a_slider = engine.slider("alpha", 30, 260, 180, 25, 0, 255, 255)

    def update_color():
        r = int(r_slider.value)
        g = int(g_slider.value)
        b = int(b_slider.value)
        a = int(a_slider.value)

        preview.set_bg_color(rgb_to_abgr(r, g, b, a))
        r_value.set_text(str(r))
        g_value.set_text(str(g))
        b_value.set_text(str(b))
        a_value.set_text(str(a))
        hex_label.set_text(f"#{r:02X}{g:02X}{b:02X}")

    def set_preset(r, g, b):
        r_slider.set_value(r)
        g_slider.set_value(g)
        b_slider.set_value(b)
        update_color()

    r_slider.on_change(lambda v: update_color())
    g_slider.on_change(lambda v: update_color())
    b_slider.on_change(lambda v: update_color())
    a_slider.on_change(lambda v: update_color())

    # Preset buttons
    engine.label("presets", 30, 305, "Presets:")

    btn_red = engine.button("preset_red", 100, 300, 60, 30, "Red")
    btn_red.on_click(lambda: set_preset(255, 0, 0))

    btn_green = engine.button("preset_green", 170, 300, 60, 30, "Green")
    btn_green.on_click(lambda: set_preset(0, 255, 0))

    btn_blue = engine.button("preset_blue", 240, 300, 60, 30, "Blue")
    btn_blue.on_click(lambda: set_preset(0, 0, 255))

    btn_white = engine.button("preset_white", 310, 300, 60, 30, "White")
    btn_white.on_click(lambda: set_preset(255, 255, 255))

    def on_key(key, mods):
        if key == ord('q'):
            engine.stop()

    engine.on_key(on_key)

    engine.show(x=2, y=2, w=60, h=22)
    engine.run()

    ygui.shutdown()

if __name__ == "__main__":
    main()
