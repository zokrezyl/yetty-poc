#!/usr/bin/env python3
"""
Demo 01: Hello Button

Simple demo showing how easy it is to create an interactive GUI.
No manual OSC, no hit testing, no event parsing - just callbacks!
"""

import ygui

def main():
    # Initialize library (sets up raw terminal mode)
    ygui.init()

    # Create engine with card name, position, and pixel size hints
    engine = ygui.Engine("dashboard", x=2, y=2, width=500, height=350)

    # App state
    click_count = 0

    # Get references for callbacks
    status_label = None
    click_button = None

    def on_click():
        nonlocal click_count
        click_count += 1
        click_button.set_label(f"Clicks: {click_count}")
        status_label.set_text(f"Clicked! Total: {click_count}")

    def on_reset():
        nonlocal click_count
        click_count = 0
        click_button.set_label("Clicks: 0")
        slider.set_value(50)
        checkbox.set_checked(False)
        status_label.set_text("Reset!")

    def on_quit():
        engine.stop()

    def on_slider_change(value):
        status_label.set_text(f"Volume: {int(value)}%")
        progress.set_value(value / 100.0)

    def on_checkbox_change(checked):
        state = "enabled" if checked else "disabled"
        status_label.set_text(f"Feature {state}")

    def on_key(key, mods):
        if key == ord('q') or key == ord('Q'):
            engine.stop()

    # Create widgets
    engine.label("title", 20, 15, "YGui Dashboard")

    click_button = engine.button("btn_click", 20, 50, 150, 45, "Clicks: 0")
    click_button.on_click(on_click)

    btn_reset = engine.button("btn_reset", 190, 50, 100, 45, "Reset")
    btn_reset.on_click(on_reset)

    btn_quit = engine.button("btn_quit", 310, 50, 100, 45, "Quit")
    btn_quit.on_click(on_quit)

    engine.label("slider_lbl", 20, 115, "Volume: 50%")
    slider = engine.slider("slider", 20, 145, 300, 30, 0, 100, 50)
    slider.on_change(on_slider_change)

    engine.label("prog_lbl", 20, 195, "Progress:")
    progress = engine.progress("progress", 20, 220, 300, 25, 0.5)

    checkbox = engine.checkbox("checkbox", 20, 265, 200, 30, "Enable feature", False)
    checkbox.on_change(on_checkbox_change)

    status_label = engine.label("status", 20, 315, "Ready - Click widgets or 'q' to quit")

    # Set keyboard handler
    engine.on_key(on_key)

    # Show card and run event loop
    engine.show()
    engine.run()

    # Cleanup
    ygui.shutdown()

if __name__ == "__main__":
    main()
