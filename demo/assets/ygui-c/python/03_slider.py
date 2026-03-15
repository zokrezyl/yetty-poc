#!/usr/bin/env python3
"""
Demo 03: Slider

Interactive slider with live value display.
"""

import ygui

def main():
    ygui.init()

    engine = ygui.Engine("slider-demo", width=400, height=200)

    # Labels
    engine.label("title", 50, 30, "Volume Control")
    value_label = engine.label("value", 50, 70, "Volume: 50%")

    # Slider: 0-100, starting at 50
    slider = engine.slider("volume", 50, 110, 300, 30, 0, 100, 50)

    def on_change(value):
        value_label.set_text(f"Volume: {int(value)}%")

    slider.on_change(on_change)

    def on_key(key, mods):
        if key == ord('q'):
            engine.stop()

    engine.on_key(on_key)

    # Show and run
    engine.show(x=2, y=2, w=50, h=12)
    engine.run()

    ygui.shutdown()

if __name__ == "__main__":
    main()
