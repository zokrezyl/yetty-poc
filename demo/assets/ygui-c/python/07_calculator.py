#!/usr/bin/env python3
"""
Demo 07: Simple Calculator

A basic calculator UI demonstrating grid layout of buttons.
"""

import ygui

def main():
    ygui.init()

    engine = ygui.Engine("calculator", x=2, y=2, width=380, height=480)

    # Display
    display = engine.label("display", 20, 20, "0")

    # Calculator state
    state = {
        'current': '0',
        'previous': None,
        'operator': None,
        'new_number': True
    }

    def update_display():
        display.set_text(state['current'])

    def calculate():
        if state['previous'] is None or state['operator'] is None:
            return

        prev = float(state['previous'])
        curr = float(state['current'])

        if state['operator'] == '+':
            result = prev + curr
        elif state['operator'] == '-':
            result = prev - curr
        elif state['operator'] == 'x':
            result = prev * curr
        elif state['operator'] == '/':
            result = prev / curr if curr != 0 else 0
        else:
            result = curr

        if result == int(result):
            state['current'] = str(int(result))
        else:
            state['current'] = f"{result:.6g}"

        state['previous'] = None
        state['operator'] = None

    def make_handler(key):
        def handler():
            if key.isdigit():
                if state['new_number']:
                    state['current'] = key
                    state['new_number'] = False
                else:
                    if state['current'] == '0':
                        state['current'] = key
                    else:
                        state['current'] += key
            elif key == '.':
                if '.' not in state['current']:
                    state['current'] += '.'
                    state['new_number'] = False
            elif key == 'C':
                state['current'] = '0'
                state['previous'] = None
                state['operator'] = None
                state['new_number'] = True
            elif key == '+/-':
                if state['current'].startswith('-'):
                    state['current'] = state['current'][1:]
                elif state['current'] != '0':
                    state['current'] = '-' + state['current']
            elif key == '%':
                state['current'] = str(float(state['current']) / 100)
            elif key in ['+', '-', 'x', '/']:
                if state['previous'] is not None:
                    calculate()
                state['previous'] = state['current']
                state['operator'] = key
                state['new_number'] = True
            elif key == '=':
                calculate()
                state['new_number'] = True

            update_display()
        return handler

    # Button layout (ASCII-friendly symbols)
    buttons = [
        ['C', '+/-', '%', '/'],
        ['7', '8', '9', 'x'],
        ['4', '5', '6', '-'],
        ['1', '2', '3', '+'],
        ['0', '0', '.', '='],
    ]

    btn_w = 80
    btn_h = 50
    margin = 10
    start_y = 70

    # Create buttons
    for row_idx, row in enumerate(buttons):
        for col_idx, label in enumerate(row):
            # Skip duplicate '0'
            if row_idx == 4 and col_idx == 1:
                continue

            x = 20 + col_idx * (btn_w + margin)
            y = start_y + row_idx * (btn_h + margin)

            # Wide zero button
            w = btn_w
            if row_idx == 4 and col_idx == 0:
                w = btn_w * 2 + margin

            btn = engine.button(f"btn_{label}", x, y, w, btn_h, label)
            btn.on_click(make_handler(label))

    def on_key(key, mods):
        if key == ord('q'):
            engine.stop()

    engine.on_key(on_key)

    # Show and run
    engine.show()
    engine.run()

    ygui.shutdown()

if __name__ == "__main__":
    main()
