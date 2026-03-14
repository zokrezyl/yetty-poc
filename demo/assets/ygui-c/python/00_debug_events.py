#!/usr/bin/env python3
"""
Debug: Visual OSC event monitor

Shows events as text inside the ygui card itself.
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
        lib.ygui_engine_set_size(engine, 500.0, 300.0)

        # Create widgets (define UI structure)
        title = lib.ygui_label(engine, b"title", 20.0, 15.0, b"OSC Event Monitor")
        btn = lib.ygui_button(engine, b"test_btn", 20.0, 50.0, 200.0, 50.0, b"Click Me!")
        btn2 = lib.ygui_button(engine, b"quit_btn", 240.0, 50.0, 100.0, 50.0, b"Quit")

        status1 = lib.ygui_label(engine, b"status1", 20.0, 120.0, b"Last event: None")
        status2 = lib.ygui_label(engine, b"status2", 20.0, 150.0, b"Position: --")
        status3 = lib.ygui_label(engine, b"status3", 20.0, 180.0, b"Button: --")
        counter = lib.ygui_label(engine, b"counter", 20.0, 220.0, b"Click count: 0")
        hint = lib.ygui_label(engine, b"hint", 20.0, 260.0, b"Press 'q' to quit")

        click_count = 0
        first_frame = True

        def render_frame():
            """Render a new frame: clear buffer, rebuild UI, serialize."""
            nonlocal first_frame

            # 1. Clear buffer (wipe all previous primitives)
            lib.ydraw_buffer_clear(buffer)

            # 2. Rebuild UI (render all widgets with current state)
            lib.ygui_engine_rebuild(engine)

            # 3. Serialize to binary
            data_ptr = POINTER(c_uint8)()
            size = lib.ydraw_buffer_serialize(buffer, byref(data_ptr))
            if size == 0:
                return

            b64 = base64.b64encode(bytes(data_ptr[:size])).decode('ascii')

            # 4. Send to terminal
            if first_frame:
                sys.stdout.buffer.write(f"\033]666666;run -c ydraw -x 2 -y 2 -w 62 -h 18 -r --name debug-card;;{b64}\033\\".encode())
                first_frame = False
            else:
                sys.stdout.buffer.write(f"\033]666666;update --name debug-card;;{b64}\033\\".encode())
            sys.stdout.buffer.flush()

        # Initial frame
        render_frame()

        # Subscribe to events
        sys.stdout.buffer.write(b"\033[?1500h\033[?1501h")
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
                                # Click event: card;buttons;press;x;y
                                buttons = int(parts[2])
                                press = int(parts[3])
                                px, py = int(parts[4]), int(parts[5])

                                event_type = "PRESS" if press == 1 else "RELEASE"
                                btn_name = "left" if buttons & 1 else ("right" if buttons & 2 else ("middle" if buttons & 4 else "scroll"))

                                if press == 1:
                                    click_count += 1

                                # Update state (modify widget text)
                                lib.ygui_label_set_text(status1, f"Last event: CLICK {event_type}".encode())
                                lib.ygui_label_set_text(status2, f"Position: ({px}, {py})".encode())
                                lib.ygui_label_set_text(status3, f"Button: {btn_name} (bits={buttons})".encode())
                                lib.ygui_label_set_text(counter, f"Click count: {click_count}".encode())

                                # New frame!
                                render_frame()

                            elif code == 777778 and len(parts) >= 5:
                                # Move event: card;buttons;x;y
                                buttons = int(parts[2])
                                px, py = int(parts[3]), int(parts[4])

                                lib.ygui_label_set_text(status1, b"Last event: MOVE")
                                lib.ygui_label_set_text(status2, f"Position: ({px}, {py})".encode())
                                lib.ygui_label_set_text(status3, f"Buttons held: {buttons}".encode())

                                # New frame!
                                render_frame()

                        except Exception:
                            pass

                        buf = b""

        # Cleanup
        sys.stdout.buffer.write(b"\033[?1500l\033[?1501l")
        sys.stdout.buffer.write(b"\033]666666;kill --name debug-card\033\\")
        sys.stdout.buffer.flush()

        lib.ygui_engine_destroy(engine)
        lib.ydraw_buffer_destroy(buffer)

    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)

if __name__ == "__main__":
    main()
