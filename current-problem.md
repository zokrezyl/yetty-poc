# Click Boundary Bug in ygui-c

## The Problem
- Click on right edge of button → MISS (should hit)
- Click left of button → FALSE HIT (should miss)
- Hit detection area is SHIFTED LEFT relative to visual rendering

## Status
NOT FIXED. Tests pass but real yetty demo still has the issue.

## What needs to be done
1. Trace real yetty to see actual coordinate values
2. Find where mismatch occurs between visual position and hit detection
3. Fix the actual bug

## Files involved
- `src/yetty/gpu-screen.cpp` - sends OSC 777777 with click coordinates
- `src/yetty/ygui-c/ygui_engine.c` - receives coordinates, does hit testing
