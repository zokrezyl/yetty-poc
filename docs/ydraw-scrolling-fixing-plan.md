# YDraw Scrolling Fix Plan

## Problem Statement

Shapes disappear when cursor position is non-zero at time of `addYdrawBuffer()`. The grid stores primitives at cursor-adjusted rows, but shader looks up using world pixel coordinates.

## Root Cause

| Component | Current Behavior | Expected |
|-----------|-----------------|----------|
| Grid storage | Row = cursorRow + localLine | Correct (for scroll removal) |
| Primitive word[2] | Stores cursorRow offset | Correct (for shader translation) |
| Shader SDF eval | Uses word[2] offset | Correct |
| **Shader grid lookup** | Uses world pixel / cellSize | **BUG** - should match grid rows |

## Fix Strategy

The grid rows ARE cursor-relative. The shader lookup must ALSO be cursor-relative.

**Solution**: Set `sceneMinY = 0` always for scrolling mode, and ensure grid row 0 = world pixel row 0.

Wait - that won't work because primitives are stored at cursorRow + localLine.

**Correct Solution**: The shader needs to look up at the SAME row the primitive was stored at.

If primitive at world Y=120 (cellY=6) was stored at grid row cursorRow+6, then shader must look up at grid row cursorRow+6, not row 6.

But cursorRow changes over time with scrolling...

**ACTUAL Solution**: When building the packed grid, the grid rows should correspond to WORLD pixel rows, not scroll-buffer line indices.

OR: sceneMinY should be negative (-cursorRow * cellSizeY) so that world coords map to cursor-adjusted grid rows.

## Implementation Plan

### Phase 1: Verify Current State

- [ ] 1.1 Read and document all test files checking offset in word[2]
- [ ] 1.2 Read shader code for grid lookup and SDF evaluation
- [ ] 1.3 Read rebuildPackedGridScrolling() to understand grid building
- [ ] 1.4 Run debug_grid to see exact mismatch

### Phase 2: Identify Fix Location

- [ ] 2.1 Determine if fix is in grid building or shader lookup
- [ ] 2.2 Document exact change needed

### Phase 3: Implement Fix

- [ ] 3.1 Implement the fix
- [ ] 3.2 Update tests to verify fix
- [ ] 3.3 Build and run tests

### Phase 4: Verify

- [ ] 4.1 Run ydraw_tests
- [ ] 4.2 Run shapes.sh demo with various cursor positions
- [ ] 4.3 Verify visual output

## Files to Examine

1. `src/yetty/ydraw/ydraw-builder.cpp` - Grid building, addYdrawBuffer, scrollLines
2. `src/yetty/shaders/gpu-screen.wgsl` - Shader grid lookup (lines 743-763)
3. `src/yetty/shaders/lib/sdf-overlay.gen.wgsl` - SDF evaluation with offset
4. `test/ut/ydraw/ydraw_scrolling_test.cpp` - Scrolling tests
5. `test/ut/ydraw/ydraw_shapes_demo_test.cpp` - Shapes demo test
6. `test/ut/ydraw/debug_grid.cpp` - Debug tool

## Current Understanding

The offset in word[2]:
- Stored: cursorRow at time of addition
- Updated: decremented by N on scrollLines(N)
- Used by shader: `pAdj = p - (offset * cellSize)`

This translates world pixel position to LOCAL primitive coordinates for SDF evaluation.

But the GRID LOOKUP is separate from SDF evaluation. Grid lookup determines WHICH primitives to evaluate. If grid lookup fails, the primitive is never evaluated.
