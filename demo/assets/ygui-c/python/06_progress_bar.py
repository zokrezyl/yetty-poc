#!/usr/bin/env python3
"""
Demo 06: Progress Bar

Animated progress bar with start/stop controls.
"""

import sys
import time
sys.path.insert(0, '../../../src/ygui-bindings/python')

from ygui import Engine, EventType

def main():
    engine = Engine(width=800, height=600)

    # Title
    engine.label("title", x=100, y=50, text="Download Progress")

    # Progress bar (0.0 to 1.0)
    progress = engine.progress("download", x=100, y=100, w=400, h=30, value=0.0)

    # Percentage label
    percent_label = engine.label("percent", x=520, y=105, text="0%")

    # Control buttons
    start_btn = engine.button("start", x=100, y=150, w=100, h=36, label="Start")
    reset_btn = engine.button("reset", x=220, y=150, w=100, h=36, label="Reset")

    running = [False]
    current_progress = [0.0]

    def on_event(event):
        if event.type == EventType.CLICK:
            if event.widget_id == "start":
                if not running[0]:
                    running[0] = True
                    start_btn.set_label(b"Pause")
                    print("Download started")
                else:
                    running[0] = False
                    start_btn.set_label(b"Resume")
                    print("Download paused")
            elif event.widget_id == "reset":
                running[0] = False
                current_progress[0] = 0.0
                progress.set_value(0.0)
                percent_label.set_text(b"0%")
                start_btn.set_label(b"Start")
                print("Download reset")

    engine.on_event(on_event)
    engine.rebuild()

    # Simulate download progress
    print("Simulating download...")
    engine.mouse_down(150, 168, 0)  # Click start
    engine.mouse_up(150, 168, 0)

    for i in range(0, 101, 10):
        current_progress[0] = i / 100.0
        progress.set_value(current_progress[0])
        percent_label.set_text(f"{i}%".encode('utf-8'))
        print(f"Progress: {i}%")
        engine.rebuild()

    print("Download complete!")

if __name__ == "__main__":
    main()
