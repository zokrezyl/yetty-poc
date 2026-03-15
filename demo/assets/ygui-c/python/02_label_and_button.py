#!/usr/bin/env python3
"""
Demo 02: Label and Button

Shows label and button together, updating label on click.
"""

import ygui

def main():
    ygui.init()

    engine = ygui.Engine("counter", width=400, height=250)

    # Title and counter
    engine.label("title", 50, 30, "Click Counter")
    counter_label = engine.label("counter", 50, 80, "Clicks: 0")

    click_count = 0

    def on_increment():
        nonlocal click_count
        click_count += 1
        counter_label.set_text(f"Clicks: {click_count}")

    def on_reset():
        nonlocal click_count
        click_count = 0
        counter_label.set_text("Clicks: 0")

    btn = engine.button("increment", 50, 130, 120, 40, "Add +1")
    btn.on_click(on_increment)

    reset_btn = engine.button("reset", 190, 130, 120, 40, "Reset")
    reset_btn.on_click(on_reset)

    def on_key(key, mods):
        if key == ord('q'):
            engine.stop()

    engine.on_key(on_key)

    engine.show(x=2, y=2, w=50, h=14)
    engine.run()

    ygui.shutdown()

if __name__ == "__main__":
    main()
