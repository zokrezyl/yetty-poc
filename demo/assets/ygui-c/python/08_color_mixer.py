#!/usr/bin/env python3
"""
Demo 08: Color Mixer

RGB color mixer with sliders and preview.
"""

import sys
sys.path.insert(0, '../../../src/ygui-bindings/python')

from ygui import Engine, EventType

def rgb_to_abgr(r, g, b, a=255):
    """Convert RGBA to ABGR uint32."""
    return (a << 24) | (b << 16) | (g << 8) | r

def main():
    engine = Engine(width=500, height=400)

    # Title
    engine.label("title", x=30, y=20, text="RGB Color Mixer")

    # Color preview panel
    preview = engine.panel("preview", x=300, y=60, w=150, h=150)
    preview.set_bg_color(rgb_to_abgr(128, 128, 128))

    # RGB labels
    engine.label("hex_label", x=300, y=230, text="#808080")

    # Red slider
    engine.label("r_label", x=30, y=60, text="Red")
    r_value = engine.label("r_value", x=220, y=60, text="128")
    r_slider = engine.slider("red", x=30, y=85, w=200, h=20,
                              min_val=0, max_val=255, value=128)

    # Green slider
    engine.label("g_label", x=30, y=120, text="Green")
    g_value = engine.label("g_value", x=220, y=120, text="128")
    g_slider = engine.slider("green", x=30, y=145, w=200, h=20,
                              min_val=0, max_val=255, value=128)

    # Blue slider
    engine.label("b_label", x=30, y=180, text="Blue")
    b_value = engine.label("b_value", x=220, y=180, text="128")
    b_slider = engine.slider("blue", x=30, y=205, w=200, h=20,
                              min_val=0, max_val=255, value=128)

    # Alpha slider
    engine.label("a_label", x=30, y=240, text="Alpha")
    a_value = engine.label("a_value", x=220, y=240, text="255")
    a_slider = engine.slider("alpha", x=30, y=265, w=200, h=20,
                              min_val=0, max_val=255, value=255)

    # Preset buttons
    engine.label("presets", x=30, y=310, text="Presets:")
    engine.button("preset_red", x=100, y=300, w=60, h=30, label="Red")
    engine.button("preset_green", x=170, y=300, w=60, h=30, label="Green")
    engine.button("preset_blue", x=240, y=300, w=60, h=30, label="Blue")
    engine.button("preset_white", x=310, y=300, w=60, h=30, label="White")

    hex_label = engine.find("hex_label")

    def update_color():
        r = int(r_slider.value)
        g = int(g_slider.value)
        b = int(b_slider.value)
        a = int(a_slider.value)

        color = rgb_to_abgr(r, g, b, a)
        preview.set_bg_color(color)

        r_value.set_text(str(r).encode('utf-8'))
        g_value.set_text(str(g).encode('utf-8'))
        b_value.set_text(str(b).encode('utf-8'))
        a_value.set_text(str(a).encode('utf-8'))

        hex_str = f"#{r:02X}{g:02X}{b:02X}"
        if hex_label:
            hex_label.set_text(hex_str.encode('utf-8'))

        print(f"Color: RGB({r}, {g}, {b}) A={a} = {hex_str}")

    def set_preset(r, g, b):
        r_slider.set_value(r)
        g_slider.set_value(g)
        b_slider.set_value(b)
        update_color()

    def on_event(event):
        if event.type == EventType.CHANGE:
            update_color()
        elif event.type == EventType.CLICK:
            if event.widget_id == "preset_red":
                set_preset(255, 0, 0)
            elif event.widget_id == "preset_green":
                set_preset(0, 255, 0)
            elif event.widget_id == "preset_blue":
                set_preset(0, 0, 255)
            elif event.widget_id == "preset_white":
                set_preset(255, 255, 255)

    engine.on_event(on_event)
    engine.rebuild()

    # Simulate changing red slider
    print("Setting red to maximum...")
    r_slider.set_value(255)
    update_color()

if __name__ == "__main__":
    main()
