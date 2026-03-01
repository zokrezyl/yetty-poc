# WebAssembly/Emscripten Build Issues

## Summary

The WebASM build compiles and runs, but **text does not render**. WebGPU initializes correctly, fonts load, shaders compile, but 0 non-empty cells are rendered.

## Root Cause

**vterm_obtain_state() returns NULL** - the terminal state layer fails to initialize, so `onPutglyph` callbacks never fire when text is written.

## Issues Fixed During Debugging

### 1. FreeType File Loading
- **Problem**: `FT_New_Face()` fails silently on Emscripten virtual filesystem
- **Solution**: Use `FT_New_Memory_Face()` with file data read into persistent buffer
- **Files**: `src/yetty/vector-sdf-font.cpp`, `src/yetty/raster-font.cpp`

### 2. CDB Library Compatibility  
- **Problem**: howerj/cdb library fails to open djb-format CDB files
- **Solution**: Set `opts.size = 32` before `cdb_open()`
- **File**: `src/yetty/cdb-wrapper.cpp`

### 3. std::filesystem::exists()
- **Problem**: Returns false for Emscripten preloaded files
- **Solution**: Skip existence checks on web, just try to open
- **File**: `src/yetty/font-manager.cpp`

### 4. vterm_new() Compound Literal
- **Problem**: Compound literal struct initialization may have issues in Emscripten
- **Solution**: Rewrote to explicit struct initialization
- **File**: `src/libvterm-0.3.3/src/vterm.c`

## Current Blocker: vterm State NULL

### Symptoms
```
createVTerm: vterm_new returned 0x5c8e8  (SUCCESS)
[ATTACH] state_=0x0                       (NULL!)
vterm_input_write returned 28             (writes accepted but ignored)
render: uploading 1920 cells, 0 non-empty (no text rendered)
```

### Debug Added
- `src/libvterm-0.3.3/src/state.c` line 2059-2073: fprintf debug in `vterm_obtain_state()`
- Debug strings ARE in wasm binary but DO NOT print to console
- This suggests either:
  1. fprintf to stderr broken in Emscripten
  2. Function not actually being called (linker issue?)
  3. `vt->state` already non-NULL but invalid (returns early)

## Fix: Replace fprintf with EM_ASM

### Step 1: Replace fprintf with EM_ASM
```c
#include <emscripten.h>
EM_ASM({ console.log("vterm_obtain_state called"); });
```
This bypasses stdio and logs directly to browser console.

### Option 2: Check vterm_obtain_state Call Site
In `gpu-screen.cpp` `attach()` method, verify `vterm_obtain_state()` is actually called:
```cpp
fprintf(stderr, "BEFORE vterm_obtain_state\n");
state_ = vterm_obtain_state(vt_);
fprintf(stderr, "AFTER vterm_obtain_state state_=%p\n", state_);
```

### Option 3: Initialize State Immediately After vterm_new
In `createVTerm()`, call `vterm_obtain_state()` right after `vterm_new()`:
```cpp
vt_ = vterm_new(rows, cols);
if (vt_) {
    VTermState* state = vterm_obtain_state(vt_);
    // force state initialization early
}
```

### Option 4: Check for Multiple vterm Definitions
```bash
nm build-webasm-dawn-release/webtest.wasm | grep vterm_obtain_state
```
Ensure only one definition exists.

### Option 5: Static Initialization Order
Emscripten may have issues with static initialization. Check if any static variables in vterm are being accessed before initialization.

### Option 6: Memory Corruption Check
Add canary values around vterm struct to detect corruption:
```c
vt->magic_start = 0xDEADBEEF;
// ... 
vt->magic_end = 0xCAFEBABE;
```

## Test Infrastructure

Created standalone test: `src/yetty/webtest/main.cpp`
- Isolates GPUScreen rendering from full yetty app
- Build: `ninja webtest` in build-webasm-dawn-release
- Run: `python -m http.server 8000` then open `webtest.html`

## Files Modified

| File | Changes |
|------|---------|
| `src/yetty/gpu-screen.cpp` | Debug logging in createVTerm, attach, write, render |
| `src/yetty/vector-sdf-font.cpp` | FT_New_Memory_Face for web |
| `src/yetty/raster-font.cpp` | FT_New_Memory_Face for web |
| `src/yetty/cdb-wrapper.cpp` | opts.size = 32 |
| `src/yetty/font-manager.cpp` | Skip filesystem exists checks |
| `src/libvterm-0.3.3/src/vterm.c` | Rewrite vterm_new, add debug |
| `src/libvterm-0.3.3/src/state.c` | Add debug to vterm_obtain_state |
| `CMakeLists.txt` | Add yetty-core library, webtest target |

## Next Steps

1. Use EM_ASM for debug output (bypasses broken fprintf)
2. Trace exact call path from attach() to vterm_obtain_state()
3. If vterm is fundamentally broken on Emscripten, consider:
   - Using a different terminal emulator library
   - Porting vterm to pure C++ without problematic C patterns
   - Pre-initializing vterm state before any Emscripten-specific code runs
