#!/usr/bin/env python3
"""
Demo 03: Slider

Interactive slider with live value display.
"""

import sys
sys.path.insert(0, '../../../src/ygui-bindings/python')

from ygui import Engine, EventType

def main():
    engine = Engine(width=800, height=600)

    # Labels
    engine.label("title", x=100, y=50, text="Volume Control")
    value_label = engine.label("value", x=100, y=90, text="Volume: 50%")

    # Slider: 0-100, starting at 50
    slider = engine.slider("volume", x=100, y=130, w=300, h=24,
                           min_val=0, max_val=100, value=50)

    def on_event(event):
        if event.type == EventType.CHANGE and event.widget_id == "volume":
            value = int(event.data) if event.data else int(slider.value)
            value_label.set_text(f"Volume: {value}%".encode('utf-8'))
            print(f"Volume changed to: {value}%")

    engine.on_event(on_event)
    engine.rebuild()

    # Simulate dragging the slider
    print("Simulating slider drag...")
    engine.mouse_down(150, 142, 0)  # Click on slider
    for x in range(150, 350, 20):   # Drag to the right
        engine.mouse_move(x, 142)
    engine.mouse_up(350, 142, 0)

    print(f"Final value: {slider.value}")

if __name__ == "__main__":
    main()
