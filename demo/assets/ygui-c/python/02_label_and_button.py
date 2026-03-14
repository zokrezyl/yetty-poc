#!/usr/bin/env python3
"""
Demo 02: Label and Button

Shows label and button together, updating label on click.
"""

import sys
sys.path.insert(0, '../../../src/ygui-bindings/python')

from ygui import Engine, EventType

def main():
    engine = Engine(width=800, height=600)

    # Create widgets
    title = engine.label("title", x=100, y=50, text="Click Counter")
    counter_label = engine.label("counter", x=100, y=100, text="Clicks: 0")
    btn = engine.button("increment", x=100, y=150, w=100, h=36, label="Add +1")
    reset_btn = engine.button("reset", x=220, y=150, w=100, h=36, label="Reset")

    click_count = [0]  # Use list to allow modification in closure

    def on_event(event):
        if event.type == EventType.CLICK:
            if event.widget_id == "increment":
                click_count[0] += 1
                counter_label.set_text(f"Clicks: {click_count[0]}".encode('utf-8'))
                print(f"Count: {click_count[0]}")
            elif event.widget_id == "reset":
                click_count[0] = 0
                counter_label.set_text(b"Clicks: 0")
                print("Counter reset")

    engine.on_event(on_event)
    engine.rebuild()

    # Simulate clicks
    for i in range(5):
        engine.mouse_down(150, 168, 0)
        engine.mouse_up(150, 168, 0)

    print(f"Final count: {click_count[0]}")

if __name__ == "__main__":
    main()
