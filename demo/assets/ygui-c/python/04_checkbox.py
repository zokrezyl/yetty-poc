#!/usr/bin/env python3
"""
Demo 04: Checkbox

Multiple checkboxes for options selection.
"""

import sys
sys.path.insert(0, '../../../src/ygui-bindings/python')

from ygui import Engine, EventType

def main():
    engine = Engine(width=800, height=600)

    # Title
    engine.label("title", x=100, y=50, text="Notification Settings")

    # Checkboxes
    email_cb = engine.checkbox("email", x=100, y=100, w=200, h=28,
                               label="Email notifications", checked=True)
    sms_cb = engine.checkbox("sms", x=100, y=140, w=200, h=28,
                             label="SMS notifications", checked=False)
    push_cb = engine.checkbox("push", x=100, y=180, w=200, h=28,
                              label="Push notifications", checked=True)

    # Status label
    status = engine.label("status", x=100, y=230, text="")

    def update_status():
        enabled = []
        if email_cb.checked:
            enabled.append("Email")
        if sms_cb.checked:
            enabled.append("SMS")
        if push_cb.checked:
            enabled.append("Push")

        if enabled:
            status.set_text(f"Enabled: {', '.join(enabled)}".encode('utf-8'))
        else:
            status.set_text(b"All notifications disabled")

    def on_event(event):
        if event.type == EventType.CHANGE:
            print(f"Checkbox '{event.widget_id}' changed")
            update_status()

    engine.on_event(on_event)
    engine.rebuild()
    update_status()

    # Simulate toggling checkboxes
    print("Toggling SMS checkbox...")
    engine.mouse_down(200, 154, 0)
    engine.mouse_up(200, 154, 0)

    print(f"Email: {email_cb.checked}, SMS: {sms_cb.checked}, Push: {push_cb.checked}")

if __name__ == "__main__":
    main()
