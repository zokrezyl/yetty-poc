#!/usr/bin/env python3
"""
Demo 06: Progress Bar

Animated progress bar with start/stop controls.
Uses libuv timer for animation.
"""

import ygui

def main():
    ygui.init()

    engine = ygui.Engine("progress-demo", x=2, y=2, width=500, height=200)

    # Title
    engine.label("title", 40, 20, "Download Progress")

    # Progress bar
    progress = engine.progress("download", 40, 60, 350, 30, 0.0)

    # Percentage label
    percent_label = engine.label("percent", 410, 65, "0%")

    # State
    running = False
    current = 0.0

    def on_start():
        nonlocal running
        running = not running
        if running:
            start_btn.set_label("Pause")
        else:
            start_btn.set_label("Resume")

    def on_reset():
        nonlocal running, current
        running = False
        current = 0.0
        progress.set_value(0.0)
        percent_label.set_text("0%")
        start_btn.set_label("Start")

    start_btn = engine.button("start", 40, 120, 100, 40, "Start")
    start_btn.on_click(on_start)

    reset_btn = engine.button("reset", 160, 120, 100, 40, "Reset")
    reset_btn.on_click(on_reset)

    def on_key(key, mods):
        if key == ord('q'):
            engine.stop()

    engine.on_key(on_key)

    engine.show()
    engine.run()

    ygui.shutdown()

if __name__ == "__main__":
    main()
