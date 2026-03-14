#!/usr/bin/env python3
"""
Demo 01: Interactive Dashboard

YGui dashboard with real mouse interaction.
Each event triggers a new frame (clear → rebuild → send).
"""

import sys
import select
import termios
import tty
import ygui
import base64
from ctypes import POINTER, c_uint8, byref

def main():
    old_settings = termios.tcgetattr(sys.stdin)

    try:
        tty.setraw(sys.stdin.fileno())

        lib = ygui._get_lib()
        buffer = lib.ydraw_buffer_create()
        engine = lib.ygui_engine_create(buffer)
        lib.ygui_engine_set_size(engine, 500.0, 350.0)

        # Create widgets (UI structure)
        title = lib.ygui_label(engine, b"title", 20.0, 15.0, b"YGui Dashboard")

        btn_click = lib.ygui_button(engine, b"btn_click", 20.0, 50.0, 150.0, 45.0, b"Clicks: 0")
        btn_reset = lib.ygui_button(engine, b"btn_reset", 190.0, 50.0, 100.0, 45.0, b"Reset")
        btn_quit = lib.ygui_button(engine, b"btn_quit", 310.0, 50.0, 100.0, 45.0, b"Quit")

        slider_label = lib.ygui_label(engine, b"slider_lbl", 20.0, 115.0, b"Volume: 50%")
        slider = lib.ygui_slider(engine, b"slider", 20.0, 145.0, 300.0, 30.0, 0.0, 100.0, 50.0)

        progress_label = lib.ygui_label(engine, b"prog_lbl", 20.0, 195.0, b"Progress:")
        progress = lib.ygui_progress(engine, b"progress", 20.0, 220.0, 300.0, 25.0, 0.5)

        checkbox = lib.ygui_checkbox(engine, b"checkbox", 20.0, 265.0, 200.0, 30.0, b"Enable feature", 0)

        status = lib.ygui_label(engine, b"status", 20.0, 315.0, b"Ready - Click widgets or 'q' to quit")

        # App state
        click_count = 0
        slider_value = 50.0
        checkbox_checked = False
        first_frame = True

        def render_frame():
            """Render a new frame: clear → rebuild → serialize → send."""
            nonlocal first_frame

            # 1. CLEAR buffer (remove all previous primitives)
            lib.ydraw_buffer_clear(buffer)

            # 2. REBUILD UI (render all widgets to fresh buffer)
            lib.ygui_engine_rebuild(engine)

            # 3. SERIALIZE to binary
            data_ptr = POINTER(c_uint8)()
            size = lib.ydraw_buffer_serialize(buffer, byref(data_ptr))
            if size == 0:
                return

            b64 = base64.b64encode(bytes(data_ptr[:size])).decode('ascii')

            # 4. SEND to terminal
            if first_frame:
                sys.stdout.buffer.write(f"\033]666666;run -c ydraw -x 2 -y 2 -w 62 -h 20 -r --name dashboard;;{b64}\033\\".encode())
                first_frame = False
            else:
                sys.stdout.buffer.write(f"\033]666666;update --name dashboard;;{b64}\033\\".encode())
            sys.stdout.buffer.flush()

        def update_widgets():
            """Update widget state from app state."""
            lib.ygui_button_set_label(btn_click, f"Clicks: {click_count}".encode())
            lib.ygui_label_set_text(slider_label, f"Volume: {int(slider_value)}%".encode())
            lib.ygui_slider_set_value(slider, slider_value)
            lib.ygui_progress_set_value(progress, slider_value / 100.0)
            lib.ygui_checkbox_set_checked(checkbox, 1 if checkbox_checked else 0)

        def hit_test(x, y, wx, wy, ww, wh):
            return wx <= x <= wx + ww and wy <= y <= wy + wh

        # Initial frame
        render_frame()

        # Subscribe to card click events
        sys.stdout.buffer.write(b"\033[?1500h")
        sys.stdout.buffer.flush()

        buf = b""
        running = True

        while running:
            if select.select([sys.stdin], [], [], 0.05)[0]:
                ch = sys.stdin.buffer.read(1)
                if not ch:
                    break

                if ch == b'q' or ch == b'Q':
                    running = False
                    break

                buf += ch

                # Check for complete OSC sequence
                if len(buf) > 2 and buf[-2:] == b"\033\\":
                    start = buf.find(b"\033]")
                    if start >= 0:
                        inner = buf[start+2:-2]
                        try:
                            parts = inner.decode('ascii').split(';')
                            code = int(parts[0]) if parts else 0

                            if code == 777777 and len(parts) >= 6:
                                buttons = int(parts[2])
                                press = int(parts[3])
                                px, py = float(parts[4]), float(parts[5])

                                # Only handle left button press
                                if press == 1 and (buttons & 1):
                                    needs_frame = False

                                    if hit_test(px, py, 20, 50, 150, 45):
                                        # Click counter button
                                        click_count += 1
                                        lib.ygui_label_set_text(status, f"Clicked! Total: {click_count}".encode())
                                        update_widgets()
                                        needs_frame = True

                                    elif hit_test(px, py, 190, 50, 100, 45):
                                        # Reset button
                                        click_count = 0
                                        slider_value = 50.0
                                        checkbox_checked = False
                                        lib.ygui_label_set_text(status, b"Reset!")
                                        update_widgets()
                                        needs_frame = True

                                    elif hit_test(px, py, 310, 50, 100, 45):
                                        # Quit button
                                        running = False

                                    elif hit_test(px, py, 20, 145, 300, 30):
                                        # Slider
                                        slider_value = ((px - 20) / 300.0) * 100.0
                                        slider_value = max(0, min(100, slider_value))
                                        lib.ygui_label_set_text(status, f"Volume: {int(slider_value)}%".encode())
                                        update_widgets()
                                        needs_frame = True

                                    elif hit_test(px, py, 20, 265, 200, 30):
                                        # Checkbox
                                        checkbox_checked = not checkbox_checked
                                        state = "enabled" if checkbox_checked else "disabled"
                                        lib.ygui_label_set_text(status, f"Feature {state}".encode())
                                        update_widgets()
                                        needs_frame = True

                                    # Render new frame if state changed
                                    if needs_frame:
                                        render_frame()

                        except Exception:
                            pass
                        buf = b""

        # Cleanup
        sys.stdout.buffer.write(b"\033[?1500l")
        sys.stdout.buffer.write(b"\033]666666;kill --name dashboard\033\\")
        sys.stdout.buffer.flush()

        lib.ygui_engine_destroy(engine)
        lib.ydraw_buffer_destroy(buffer)

    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)

if __name__ == "__main__":
    main()
