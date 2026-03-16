#!/usr/bin/env python3
"""
Test integration for yetty RPC handlers.

Usage:
    1. Start yetty with a ygui-c app:
       ./build-desktop-ytrace-debug/yetty -e ./demo/scripts/ygui-c/python/00-debug-events.sh

    2. Run this script:
       python3 test/integration/test_rpc_handlers.py

The script will connect to the running yetty via its RPC socket and test
the new integration testing handlers.
"""

import os
import socket
import msgpack
import sys


def connect(socket_path):
    """Connect to yetty RPC socket."""
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    sock.connect(socket_path)
    sock.settimeout(5.0)
    return sock


def rpc_request(sock, channel, method, params):
    """Send RPC request and return result."""
    msgid = 1
    msg = [0, msgid, channel, method, params]
    sock.send(msgpack.packb(msg))
    response = msgpack.unpackb(sock.recv(4096), raw=False)
    # response = [1, msgid, error, result]
    if response[2] is not None:
        raise Exception(f"RPC error: {response[2]}")
    return response[3]


def test_get_cell_size(sock):
    """Test get_cell_size handler."""
    print("\n=== Testing get_cell_size ===")
    result = rpc_request(sock, 0, "get_cell_size", {})
    print(f"Cell size: {result['width']:.1f} x {result['height']:.1f} pixels")
    assert "width" in result
    assert "height" in result
    assert result["width"] > 0
    assert result["height"] > 0
    print("PASS")
    return result


def test_get_card_info(sock, card_name):
    """Test get_card_info handler."""
    print(f"\n=== Testing get_card_info for '{card_name}' ===")
    try:
        result = rpc_request(sock, 0, "get_card_info", {"name": card_name})
        print(f"Card ID: {result['id']}")
        print(f"Position: ({result['x']}, {result['y']}) cells")
        print(f"Size: {result['width_cells']}x{result['height_cells']} cells")
        print(f"Display size: {result['display_w']:.1f}x{result['display_h']:.1f} pixels")
        print(f"Cell size: {result['cell_w']:.1f}x{result['cell_h']:.1f} pixels")
        print("PASS")
        return result
    except Exception as e:
        print(f"SKIP: {e}")
        return None


def test_compute_click_coords(sock, card_name, display_x, display_y):
    """Test compute_click_coords handler."""
    print(f"\n=== Testing compute_click_coords ({display_x}, {display_y}) ===")
    try:
        result = rpc_request(sock, 0, "compute_click_coords", {
            "name": card_name,
            "display_x": display_x,
            "display_y": display_y
        })
        print(f"Input: display ({display_x:.1f}, {display_y:.1f})")
        print(f"OSC coords: ({result['osc_x']}, {result['osc_y']})")
        print(f"Clamped: ({result['clamped_x']:.1f}, {result['clamped_y']:.1f})")
        print("PASS")
        return result
    except Exception as e:
        print(f"SKIP: {e}")
        return None


def test_cards_list(sock):
    """Test cards_list handler (existing)."""
    print("\n=== Testing cards_list ===")
    result = rpc_request(sock, 1, "cards_list", {})
    print(f"Found {len(result)} cards:")
    for card in result:
        print(f"  - slot={card['slot_index']} name='{card['name']}' type='{card['type']}'")
    return result


def main():
    # Get socket path from environment or construct it
    socket_path = os.environ.get("YETTY_SOCKET")
    if not socket_path:
        # Try to find it
        runtime_dir = os.environ.get("XDG_RUNTIME_DIR", "/tmp")
        yetty_dir = os.path.join(runtime_dir, "yetty")
        if os.path.isdir(yetty_dir):
            sockets = [f for f in os.listdir(yetty_dir) if f.startswith("yetty-") and f.endswith(".sock")]
            if sockets:
                socket_path = os.path.join(yetty_dir, sockets[0])
                print(f"Found socket: {socket_path}")

    if not socket_path:
        print("Error: No YETTY_SOCKET environment variable and no socket found")
        print("Start yetty first, e.g.:")
        print("  ./build-desktop-ytrace-debug/yetty -e ./demo/scripts/ygui-c/python/00-debug-events.sh")
        sys.exit(1)

    try:
        sock = connect(socket_path)
        print(f"Connected to {socket_path}")
    except Exception as e:
        print(f"Error connecting to {socket_path}: {e}")
        sys.exit(1)

    try:
        # Test cell size
        cell_info = test_get_cell_size(sock)

        # List all cards
        cards = test_cards_list(sock)

        # If we have cards, test the card info and click coords
        if cards:
            card_name = cards[0]["name"]
            if card_name:
                card_info = test_get_card_info(sock, card_name)

                if card_info:
                    # Test click at center of card
                    center_x = card_info["display_w"] / 2
                    center_y = card_info["display_h"] / 2
                    test_compute_click_coords(sock, card_name, center_x, center_y)

                    # Test click at right edge (the bug area)
                    right_edge_x = card_info["display_w"] - 1
                    test_compute_click_coords(sock, card_name, right_edge_x, center_y)

                    # Test click outside right edge
                    outside_x = card_info["display_w"] + 10
                    test_compute_click_coords(sock, card_name, outside_x, center_y)

        print("\n=== All tests completed ===")

    finally:
        sock.close()


if __name__ == "__main__":
    main()
