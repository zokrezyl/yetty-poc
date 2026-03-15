#!/usr/bin/env python3
"""
Demo 05: Panel Layout

Panel container with nested widgets.
"""

import ygui

def main():
    ygui.init()

    engine = ygui.Engine("settings", width=420, height=360)

    # Panel background
    panel = engine.panel("settings_panel", 20, 20, 380, 320)
    panel.set_bg_color(0xFF2D2D2D)

    # Title
    engine.label("panel_title", 40, 40, "Settings Panel")

    # Brightness
    engine.label("brightness_label", 40, 80, "Brightness")
    brightness = engine.slider("brightness", 40, 110, 200, 25, 0, 100, 75)

    # Contrast
    engine.label("contrast_label", 40, 150, "Contrast")
    contrast = engine.slider("contrast", 40, 180, 200, 25, 0, 100, 50)

    # Auto-save checkbox
    auto_save = engine.checkbox("auto_save", 40, 220, 180, 30, "Auto-save", True)

    # Status
    status = engine.label("status", 40, 290, "Ready")

    def on_brightness(value):
        status.set_text(f"Brightness: {int(value)}")

    def on_contrast(value):
        status.set_text(f"Contrast: {int(value)}")

    def on_apply():
        status.set_text(f"Applied: B={int(brightness.value)}, C={int(contrast.value)}")

    def on_cancel():
        status.set_text("Cancelled")

    brightness.on_change(on_brightness)
    contrast.on_change(on_contrast)

    apply_btn = engine.button("apply", 40, 260, 90, 35, "Apply")
    apply_btn.on_click(on_apply)

    cancel_btn = engine.button("cancel", 150, 260, 90, 35, "Cancel")
    cancel_btn.on_click(on_cancel)

    def on_key(key, mods):
        if key == ord('q'):
            engine.stop()

    engine.on_key(on_key)

    engine.show(x=2, y=2, w=52, h=20)
    engine.run()

    ygui.shutdown()

if __name__ == "__main__":
    main()
