#!/usr/bin/env python3
"""Encode a file to base94 and output the OSC sequence for yetty plugins.

OSC Format: ESC ] 99999;<plugin>;<mode>;<x>;<y>;<w>;<h>;<base94_payload> BEL

Where:
- 99999 is YETTY_OSC_VENDOR_ID
- plugin: shader, image, etc.
- mode: A (absolute), R (relative to cursor)
- x, y: position in cells
- w, h: size in cells
- base94_payload: file content encoded in base94
"""

import sys
import os

def base94_encode(data: bytes) -> str:
    """Encode bytes to base94 (2 chars per byte)."""
    result = []
    for byte in data:
        result.append(chr(33 + (byte // 94)))
        result.append(chr(33 + (byte % 94)))
    return ''.join(result)

def base94_decode(encoded: str) -> bytes:
    """Decode base94 string back to bytes."""
    result = []
    i = 0
    while i + 1 < len(encoded):
        c1 = ord(encoded[i]) - 33
        c2 = ord(encoded[i + 1]) - 33
        result.append(c1 * 94 + c2)
        i += 2
    return bytes(result)

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <file|--string=PAYLOAD> [options]", file=sys.stderr)
        print("Options:", file=sys.stderr)
        print("  --plugin=X   Plugin name: shader, image, ymery. Default: auto-detect", file=sys.stderr)
        print("  --string=X   Use string X as payload instead of reading a file", file=sys.stderr)
        print("  --raw        Output only the base94 encoded string (no OSC wrapper)", file=sys.stderr)
        print("  --decode     Decode base94 from stdin instead of encoding", file=sys.stderr)
        print("  --mode=X     Position mode: A (absolute) or R (relative). Default: R", file=sys.stderr)
        print("  --x=N        X position in cells. Default: 0", file=sys.stderr)
        print("  --y=N        Y position in cells. Default: 0", file=sys.stderr)
        print("  --w=N        Width in cells. Default: 40", file=sys.stderr)
        print("  --h=N        Height in cells. Default: 20", file=sys.stderr)
        print("  --save=FILE  Save output to FILE.encoded.yetty (strips ESC codes)", file=sys.stderr)
        sys.exit(1)

    # Check for decode mode
    if '--decode' in sys.argv:
        encoded = sys.stdin.read().strip()
        try:
            decoded = base94_decode(encoded)
            sys.stdout.buffer.write(decoded)
        except Exception as e:
            print(f"Decode error: {e}", file=sys.stderr)
            sys.exit(1)
        return

    # Parse options
    input_file = None
    string_payload = None
    raw_mode = '--raw' in sys.argv
    save_file = None
    plugin = None
    mode = 'R'
    x, y = 0, 0
    w, h = 40, 20

    for arg in sys.argv[1:]:
        if arg.startswith('--plugin='):
            plugin = arg.split('=', 1)[1]
        elif arg.startswith('--string='):
            string_payload = arg.split('=', 1)[1]
        elif arg.startswith('--mode='):
            mode = arg.split('=', 1)[1].upper()
        elif arg.startswith('--x='):
            x = int(arg.split('=', 1)[1])
        elif arg.startswith('--y='):
            y = int(arg.split('=', 1)[1])
        elif arg.startswith('--w=') or arg.startswith('--width='):
            w = int(arg.split('=', 1)[1])
        elif arg.startswith('--h=') or arg.startswith('--height='):
            h = int(arg.split('=', 1)[1])
        elif arg.startswith('--save='):
            save_file = arg.split('=', 1)[1]
        elif not arg.startswith('--'):
            input_file = arg

    # Get payload data
    if string_payload is not None:
        data = string_payload.encode('utf-8')
        if plugin is None:
            plugin = 'ymery'  # string payloads are typically for ymery
    elif input_file:
        # Auto-detect plugin from file extension
        if plugin is None:
            ext = os.path.splitext(input_file)[1].lower()
            if ext in ('.wgsl',):
                plugin = 'shader'
            elif ext in ('.png', '.jpg', '.jpeg', '.gif', '.bmp'):
                plugin = 'image'
            else:
                plugin = 'shader'  # default
        with open(input_file, 'rb') as f:
            data = f.read()
    else:
        print("Error: No input file or --string payload specified", file=sys.stderr)
        sys.exit(1)

    encoded = base94_encode(data)

    if raw_mode:
        print(encoded)
    elif save_file:
        # Save to file with ESC prefix and BEL terminator (for demo files that can be cat'd to terminal)
        # Format: ESC ] 99999;<plugin>;<mode>;<x>;<y>;<w>;<h>;<payload> BEL
        content = f"\x1b]99999;{plugin};{mode};{x};{y};{w};{h};{encoded}\x07\n"
        out_path = f"{save_file}.encoded.yetty" if not save_file.endswith('.encoded.yetty') else save_file
        with open(out_path, 'w') as f:
            f.write(content)
        print(f"Saved to: {out_path}", file=sys.stderr)
    else:
        # Output OSC sequence: ESC ] 99999;<plugin>;<mode>;<x>;<y>;<w>;<h>;<payload> ST
        # Using ST (ESC \) instead of BEL (0x07) as terminator - more reliable through PTY
        osc = f"\033]99999;{plugin};{mode};{x};{y};{w};{h};{encoded}\033\\"
        print(osc, end='')

if __name__ == '__main__':
    main()
