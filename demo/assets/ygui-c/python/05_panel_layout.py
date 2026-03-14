#!/usr/bin/env python3
"""
Demo 05: Panel Layout

Panel container with nested widgets.
"""

import sys
sys.path.insert(0, '../../../src/ygui-bindings/python')

from ygui import Engine, EventType

def main():
    engine = Engine(width=800, height=600)

    # Create a panel
    panel = engine.panel("settings_panel", x=50, y=50, w=400, h=300)
    panel.set_bg_color(0xFF2D2D2D)  # Dark background

    # Title inside panel (relative positioning would need parent support)
    engine.label("panel_title", x=70, y=70, text="Settings Panel")

    # Controls inside the panel area
    engine.label("brightness_label", x=70, y=110, text="Brightness")
    brightness = engine.slider("brightness", x=70, y=140, w=200, h=20,
                               min_val=0, max_val=100, value=75)

    engine.label("contrast_label", x=70, y=180, text="Contrast")
    contrast = engine.slider("contrast", x=70, y=210, w=200, h=20,
                             min_val=0, max_val=100, value=50)

    auto_save = engine.checkbox("auto_save", x=70, y=250, w=180, h=24,
                                label="Auto-save", checked=True)

    apply_btn = engine.button("apply", x=70, y=290, w=90, h=32, label="Apply")
    cancel_btn = engine.button("cancel", x=170, y=290, w=90, h=32, label="Cancel")

    def on_event(event):
        if event.type == EventType.CLICK:
            if event.widget_id == "apply":
                print(f"Applied settings:")
                print(f"  Brightness: {brightness.value}")
                print(f"  Contrast: {contrast.value}")
                print(f"  Auto-save: {auto_save.checked}")
            elif event.widget_id == "cancel":
                print("Settings cancelled")
        elif event.type == EventType.CHANGE:
            print(f"{event.widget_id} changed to {event.data}")

    engine.on_event(on_event)
    engine.rebuild()

    # Simulate interactions
    engine.mouse_down(115, 300, 0)  # Click Apply
    engine.mouse_up(115, 300, 0)

if __name__ == "__main__":
    main()
