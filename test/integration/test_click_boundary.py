#!/usr/bin/env python3
"""
Integration test for click boundary bug.

Tests the REAL coordinate transformation by:
1. Starting yetty with the debug demo
2. Connecting via RPC socket
3. Getting actual cell size and card info
4. Sending clicks at boundary positions
5. Verifying hit/miss behavior

This catches bugs that the emulated tests miss because it uses
the REAL yetty coordinate computation.
"""

import os
import socket
import msgpack
import subprocess
import time
import sys

CHANNEL_EVENT_LOOP = 0

def connect(socket_path):
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    sock.connect(socket_path)
    sock.settimeout(5.0)
    return sock

def rpc_request(sock, channel, method, params):
    msgid = 1
    msg = [0, msgid, channel, method, params]
    sock.send(msgpack.packb(msg))
    response = msgpack.unpackb(sock.recv(4096), raw=False)
    if response[2] is not None:
        raise Exception(f"RPC error: {response[2]}")
    return response[3]

def rpc_notify(sock, channel, method, params):
    msg = [2, channel, method, params]
    sock.send(msgpack.packb(msg))

def get_cell_size(sock):
    return rpc_request(sock, CHANNEL_EVENT_LOOP, "get_cell_size", {})

def get_card_info(sock, name):
    return rpc_request(sock, CHANNEL_EVENT_LOOP, "get_card_info", {"name": name})

def compute_click_coords(sock, name, display_x, display_y):
    return rpc_request(sock, CHANNEL_EVENT_LOOP, "compute_click_coords", {
        "name": name,
        "display_x": display_x,
        "display_y": display_y
    })

def send_card_click(sock, card_id, x, y, button=0):
    """Send a card mouse down/up event via RPC."""
    rpc_notify(sock, CHANNEL_EVENT_LOOP, "card_mouse_down", {
        "target_id": card_id,
        "x": float(x),
        "y": float(y),
        "button": button
    })
    time.sleep(0.05)
    rpc_notify(sock, CHANNEL_EVENT_LOOP, "card_mouse_up", {
        "target_id": card_id,
        "x": float(x),
        "y": float(y),
        "button": button
    })

def find_socket():
    """Find the yetty socket."""
    socket_path = os.environ.get("YETTY_SOCKET")
    if socket_path and os.path.exists(socket_path):
        return socket_path

    runtime_dir = os.environ.get("XDG_RUNTIME_DIR", "/tmp")
    yetty_dir = os.path.join(runtime_dir, "yetty")
    if os.path.isdir(yetty_dir):
        sockets = [f for f in os.listdir(yetty_dir) if f.startswith("yetty-") and f.endswith(".sock")]
        if sockets:
            return os.path.join(yetty_dir, sockets[0])
    return None

def test_boundary_clicks():
    """Test click boundaries against real yetty."""

    socket_path = find_socket()
    if not socket_path:
        print("ERROR: No yetty socket found. Start yetty first:")
        print("  ./build-desktop-ytrace-debug/yetty -e ./demo/scripts/ygui-c/python/00-debug-events.sh")
        return False

    print(f"Connecting to {socket_path}")
    sock = connect(socket_path)

    try:
        # Get cell size
        cell = get_cell_size(sock)
        print(f"\nCell size: {cell['width']:.1f} x {cell['height']:.1f} pixels")

        # Get card info
        card = get_card_info(sock, "debug-card")
        print(f"\nCard 'debug-card':")
        print(f"  ID: {card['id']}")
        print(f"  Position: ({card['x']}, {card['y']}) cells")
        print(f"  Size: {card['width_cells']}x{card['height_cells']} cells")
        print(f"  Display: {card['display_w']:.1f}x{card['display_h']:.1f} pixels")

        # The demo has a button at canvas (20, 50) with size (200, 50)
        # With CANVAS_FIXED mode, we need to understand the scaling

        # Canvas is 500x300, display is card cells * cell size
        canvas_w = 500.0
        canvas_h = 300.0
        display_w = card['display_w']
        display_h = card['display_h']

        print(f"\nCanvas: {canvas_w}x{canvas_h}")
        print(f"Display: {display_w}x{display_h}")
        print(f"Scale: {display_w/canvas_w:.4f} x {display_h/canvas_h:.4f}")

        # Button in canvas coordinates
        btn_x = 20.0
        btn_y = 50.0
        btn_w = 200.0
        btn_h = 50.0
        btn_right = btn_x + btn_w  # 220
        btn_bottom = btn_y + btn_h  # 100

        print(f"\nButton (canvas): x={btn_x}, y={btn_y}, w={btn_w}, h={btn_h}")
        print(f"  Right edge (canvas): {btn_right}")
        print(f"  Bottom edge (canvas): {btn_bottom}")

        # Map button to display coordinates
        # display_x = canvas_x * (display_w / canvas_w)
        scale_x = display_w / canvas_w
        scale_y = display_h / canvas_h

        btn_display_left = btn_x * scale_x
        btn_display_right = btn_right * scale_x
        btn_display_top = btn_y * scale_y
        btn_display_bottom = btn_bottom * scale_y

        print(f"\nButton (display): left={btn_display_left:.2f}, right={btn_display_right:.2f}")
        print(f"  Visual boundaries: x=[{btn_display_left:.2f}, {btn_display_right:.2f})")

        # Now test what coordinates yetty would send for clicks at various positions
        print("\n=== Testing coordinate transformation ===")

        test_points = [
            ("Left edge (visual)", btn_display_left, btn_display_top + btn_h/2 * scale_y),
            ("Left edge -1", btn_display_left - 1, btn_display_top + btn_h/2 * scale_y),
            ("Right edge (visual)", btn_display_right - 1, btn_display_top + btn_h/2 * scale_y),
            ("Right edge +1", btn_display_right + 1, btn_display_top + btn_h/2 * scale_y),
            ("Center", (btn_display_left + btn_display_right) / 2, (btn_display_top + btn_display_bottom) / 2),
        ]

        print("\nClick coordinate analysis:")
        print("-" * 80)

        for label, disp_x, disp_y in test_points:
            coords = compute_click_coords(sock, "debug-card", disp_x, disp_y)

            # What canvas coordinate does this map to?
            # The scale_coords function does: canvas_x = (display_x / display_w) * canvas_w
            canvas_x = (coords['osc_x'] / display_w) * canvas_w
            canvas_y = (coords['osc_y'] / display_h) * canvas_h

            # Would this hit the button?
            hit_x = btn_x <= canvas_x < btn_right
            hit_y = btn_y <= canvas_y < btn_bottom
            hit = hit_x and hit_y

            print(f"{label}:")
            print(f"  Display: ({disp_x:.2f}, {disp_y:.2f})")
            print(f"  OSC (int): ({coords['osc_x']}, {coords['osc_y']})")
            print(f"  Canvas (computed): ({canvas_x:.2f}, {canvas_y:.2f})")
            print(f"  Hit test: x={hit_x} ({btn_x} <= {canvas_x:.2f} < {btn_right}), y={hit_y}")
            print(f"  RESULT: {'HIT' if hit else 'MISS'}")
            print()

        # Now actually send clicks and check behavior
        print("=== Sending actual clicks via RPC ===")
        print("Watch the debug-card window to see if clicks register!")
        print()

        # Send click at center (should definitely hit)
        center_x = (btn_display_left + btn_display_right) / 2
        center_y = (btn_display_top + btn_display_bottom) / 2
        print(f"Clicking at center ({center_x:.1f}, {center_y:.1f})...")
        send_card_click(sock, card['id'], center_x, center_y)
        time.sleep(0.5)

        # Send click at right edge
        right_x = btn_display_right - 1
        print(f"Clicking at right edge ({right_x:.1f}, {center_y:.1f})...")
        send_card_click(sock, card['id'], right_x, center_y)
        time.sleep(0.5)

        # Send click just outside right edge
        outside_x = btn_display_right + 1
        print(f"Clicking outside right edge ({outside_x:.1f}, {center_y:.1f})...")
        send_card_click(sock, card['id'], outside_x, center_y)
        time.sleep(0.5)

        print("\nDone! Check the click counter in the debug window.")
        return True

    finally:
        sock.close()

if __name__ == "__main__":
    success = test_boundary_clicks()
    sys.exit(0 if success else 1)
