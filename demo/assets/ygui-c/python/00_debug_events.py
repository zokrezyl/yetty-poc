#!/usr/bin/env python3
"""
Debug: Visual event monitor

Shows events as text inside the ygui card itself.
Uses the new clean callback API.
"""

import ygui

def main():
    ygui.init()

    engine = ygui.Engine("debug-card", x=2, y=2, width=500, height=300)
    # CANVAS_FIT + SCALE_ON is now the default for pixel hint mode

    # Create widgets
    engine.label("title", 20, 15, "Event Monitor")

    status1 = engine.label("status1", 20, 120, "Last event: None")
    status2 = engine.label("status2", 20, 150, "Widget: --")
    counter = engine.label("counter", 20, 200, "Click count: 0")
    engine.label("hint", 20, 260, "Press 'q' to quit")

    click_count = 0

    def on_test_click():
        nonlocal click_count
        click_count += 1
        status1.set_text("Last event: Button CLICK")
        status2.set_text("Widget: test_btn")
        counter.set_text(f"Click count: {click_count}")

    def on_quit():
        engine.stop()

    btn = engine.button("test_btn", 20, 50, 200, 50, "Click Me!")
    btn.on_click(on_test_click)

    quit_btn = engine.button("quit_btn", 240, 50, 100, 50, "Quit")
    quit_btn.on_click(on_quit)

    def on_key(key, mods):
        if key == ord('q') or key == ord('Q'):
            engine.stop()

    engine.on_key(on_key)

    engine.show()
    engine.run()

    ygui.shutdown()

if __name__ == "__main__":
    main()
