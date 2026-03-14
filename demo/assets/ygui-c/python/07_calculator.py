#!/usr/bin/env python3
"""
Demo 07: Simple Calculator

A basic calculator UI demonstrating grid layout of buttons.
"""

import sys
sys.path.insert(0, '../../../src/ygui-bindings/python')

from ygui import Engine, EventType

def main():
    engine = Engine(width=400, height=500)

    # Display
    display = engine.label("display", x=20, y=20, text="0")

    # Calculator state
    state = {
        'current': '0',
        'previous': None,
        'operator': None,
        'new_number': True
    }

    # Button layout
    buttons = [
        ['C', '±', '%', '÷'],
        ['7', '8', '9', '×'],
        ['4', '5', '6', '-'],
        ['1', '2', '3', '+'],
        ['0', '0', '.', '='],
    ]

    btn_width = 80
    btn_height = 50
    margin = 10
    start_y = 80

    # Create buttons
    for row_idx, row in enumerate(buttons):
        for col_idx, label in enumerate(row):
            # Skip duplicate '0' button
            if row_idx == 4 and col_idx == 1:
                continue

            x = 20 + col_idx * (btn_width + margin)
            y = start_y + row_idx * (btn_height + margin)

            # Wide zero button
            w = btn_width
            if row_idx == 4 and col_idx == 0:
                w = btn_width * 2 + margin

            btn_id = f"btn_{label}"
            engine.button(btn_id, x=x, y=y, w=w, h=btn_height, label=label)

    def update_display():
        display.set_text(state['current'].encode('utf-8'))

    def calculate():
        if state['previous'] is None or state['operator'] is None:
            return

        prev = float(state['previous'])
        curr = float(state['current'])

        if state['operator'] == '+':
            result = prev + curr
        elif state['operator'] == '-':
            result = prev - curr
        elif state['operator'] == '×':
            result = prev * curr
        elif state['operator'] == '÷':
            result = prev / curr if curr != 0 else 0
        else:
            result = curr

        # Format result
        if result == int(result):
            state['current'] = str(int(result))
        else:
            state['current'] = f"{result:.6g}"

        state['previous'] = None
        state['operator'] = None

    def on_event(event):
        if event.type != EventType.CLICK:
            return

        btn_id = event.widget_id
        if not btn_id.startswith("btn_"):
            return

        key = btn_id[4:]  # Remove "btn_" prefix
        print(f"Pressed: {key}")

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
        elif key == '±':
            if state['current'].startswith('-'):
                state['current'] = state['current'][1:]
            elif state['current'] != '0':
                state['current'] = '-' + state['current']
        elif key == '%':
            state['current'] = str(float(state['current']) / 100)
        elif key in ['+', '-', '×', '÷']:
            if state['previous'] is not None:
                calculate()
            state['previous'] = state['current']
            state['operator'] = key
            state['new_number'] = True
        elif key == '=':
            calculate()
            state['new_number'] = True

        update_display()

    engine.on_event(on_event)
    engine.rebuild()

    # Simulate: 2 + 3 = 5
    print("\nCalculating: 2 + 3 = ?")
    for btn_id in ["btn_2", "btn_+", "btn_3", "btn_="]:
        # Find button position (simplified - would need proper lookup)
        engine.mouse_down(100, 200, 0)
        engine.mouse_up(100, 200, 0)

    print(f"Result: {state['current']}")

if __name__ == "__main__":
    main()
