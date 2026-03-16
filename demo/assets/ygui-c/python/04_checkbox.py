#!/usr/bin/env python3
"""
Demo 04: Checkbox

Multiple checkboxes for options selection.
"""

import ygui

def main():
    ygui.init()

    engine = ygui.Engine("settings", x=2, y=2, width=350, height=300)

    # Title
    engine.label("title", 30, 20, "Notification Settings")

    # Status label
    status = engine.label("status", 30, 240, "")

    # Checkboxes
    email_cb = engine.checkbox("email", 30, 70, 280, 35, "Email notifications", True)
    sms_cb = engine.checkbox("sms", 30, 115, 280, 35, "SMS notifications", False)
    push_cb = engine.checkbox("push", 30, 160, 280, 35, "Push notifications", True)

    def update_status():
        enabled = []
        if email_cb.checked:
            enabled.append("Email")
        if sms_cb.checked:
            enabled.append("SMS")
        if push_cb.checked:
            enabled.append("Push")

        if enabled:
            status.set_text(f"Enabled: {', '.join(enabled)}")
        else:
            status.set_text("All notifications disabled")

    # Callbacks
    email_cb.on_change(lambda checked: update_status())
    sms_cb.on_change(lambda checked: update_status())
    push_cb.on_change(lambda checked: update_status())

    # Initial status
    update_status()

    def on_key(key, mods):
        if key == ord('q'):
            engine.stop()

    engine.on_key(on_key)

    engine.show()
    engine.run()

    ygui.shutdown()

if __name__ == "__main__":
    main()
