# WebAssembly Sandbox Integration for Yetty

This document explores integrating a WebAssembly (WASM) runtime into yetty to safely execute untrusted third-party code that can render content in cards.

## Executive Summary

WebAssembly provides a sandboxed execution environment with:
- **Memory isolation** - WASM modules cannot access host memory directly
- **Capability-based security** - only explicitly imported functions are accessible
- **Deterministic execution** - no undefined behavior
- **Near-native performance** - compiled to machine code

This enables a plugin marketplace where users can download and run community-created visualizations, widgets, and tools without compromising system security.

---

## 1. Use Cases

### 1.1 Target Scenarios

| Use Case | Description | Example |
|----------|-------------|---------|
| **Custom visualizations** | Data viz widgets | Stock charts, network graphs |
| **Interactive widgets** | Mini-apps in terminal | Calculators, games, clocks |
| **Format renderers** | File format plugins | Custom diagram formats |
| **Shader-like effects** | Per-pixel compute | Image filters, generative art |
| **Protocol handlers** | Network data display | MQTT visualizer, API monitors |

### 1.2 User Flow

```
1. User discovers WASM widget in marketplace/registry
2. Downloads .wasm file (or fetches URL)
3. Loads into yetty: `yetty --wasm-card widget.wasm --args "config=value"`
4. WASM module renders to assigned card area
5. Module receives input events, timer ticks
6. User can pause/unload module at any time
```

---

## 2. WebAssembly Runtime Options

### 2.1 Runtime Comparison

| Runtime | Language | Performance | Embedding | WASI | Size | License |
|---------|----------|-------------|-----------|------|------|---------|
| **Wasmtime** | Rust | Excellent | C API | Full | ~15MB | Apache 2.0 |
| **Wasmer** | Rust | Excellent | C API | Full | ~12MB | MIT |
| **wasm3** | C | Good | Native C | Partial | ~100KB | MIT |
| **WAMR** | C | Good | Native C | Full | ~200KB | Apache 2.0 |
| **WasmEdge** | C++ | Excellent | C API | Full | ~10MB | Apache 2.0 |
| **V8** | C++ | Excellent | Complex | No | ~30MB | BSD |

### 2.2 Detailed Analysis

#### Option A: Wasmtime (Recommended)

**Pros:**
- Industry-leading security (Bytecode Alliance)
- Excellent JIT performance with Cranelift
- Comprehensive WASI support
- Active development, production-ready
- Clean C API (`wasmtime.h`)

**Cons:**
- Larger binary size (~15MB)
- Rust toolchain needed for building from source

**Integration:**
```cpp
#include <wasmtime.h>

// Initialize engine
wasm_engine_t* engine = wasm_engine_new();
wasmtime_store_t* store = wasmtime_store_new(engine, NULL, NULL);
wasmtime_context_t* context = wasmtime_store_context(store);

// Load module
wasmtime_module_t* module;
wasmtime_module_new(engine, wasm_bytes, wasm_len, &module);

// Create instance with imports
wasmtime_instance_t instance;
wasmtime_instance_new(context, module, imports, num_imports, &instance, &trap);
```

#### Option B: wasm3 (Lightweight Alternative)

**Pros:**
- Tiny footprint (~100KB)
- Pure C, no dependencies
- Easy to embed
- Interpreter-based (no JIT = simpler security model)

**Cons:**
- 4-10x slower than JIT runtimes
- Less complete WASI support
- Fewer features

**Integration:**
```cpp
#include <wasm3.h>
#include <m3_env.h>

IM3Environment env = m3_NewEnvironment();
IM3Runtime runtime = m3_NewRuntime(env, 64*1024, NULL);  // 64KB stack

IM3Module module;
m3_ParseModule(env, &module, wasm_bytes, wasm_len);
m3_LoadModule(runtime, module);

// Link host function
m3_LinkRawFunction(module, "env", "draw_rect", "v(iiii)", &host_draw_rect);
```

#### Option C: WAMR (WebAssembly Micro Runtime)

**Pros:**
- Small footprint (~200KB)
- Multiple execution modes (interpreter, AOT, JIT)
- Designed for embedded systems
- Good WASI support

**Cons:**
- Less community adoption
- Documentation could be better

**Integration:**
```cpp
#include <wasm_export.h>

// Initialize runtime
wasm_runtime_init();

// Load module
wasm_module_t module = wasm_runtime_load(wasm_bytes, wasm_len, error_buf, sizeof(error_buf));

// Instantiate with memory limits
wasm_module_inst_t instance = wasm_runtime_instantiate(
    module,
    8192,    // stack size
    8192,    // heap size
    error_buf, sizeof(error_buf)
);
```

### 2.3 Recommendation

| Scenario | Recommended Runtime |
|----------|---------------------|
| Production, performance-critical | **Wasmtime** |
| Minimal binary size, embedded | **wasm3** |
| Balanced size/performance | **WAMR** |

**Primary recommendation: Wasmtime** for security track record and performance.
**Fallback: wasm3** if binary size is critical.

---

## 3. Security Architecture

### 3.1 Threat Model

| Threat | Mitigation |
|--------|------------|
| **Memory access** | WASM linear memory is isolated; host pointers never exposed |
| **File system access** | WASI filesystem virtualized; whitelist directories |
| **Network access** | No raw sockets; only through host-provided APIs |
| **CPU exhaustion** | Fuel/gas metering; instruction limits |
| **Memory exhaustion** | Configurable memory limits per module |
| **Infinite loops** | Epoch interruption; watchdog timers |
| **Host function abuse** | Rate limiting; capability tokens |

### 3.2 Sandboxing Layers

```
┌─────────────────────────────────────────────────────────┐
│                    Yetty Process                        │
├─────────────────────────────────────────────────────────┤
│  ┌─────────────────────────────────────────────────┐    │
│  │              WASM Runtime (Wasmtime)            │    │
│  ├─────────────────────────────────────────────────┤    │
│  │  ┌─────────────┐  ┌─────────────┐               │    │
│  │  │  Module A   │  │  Module B   │  ...          │    │
│  │  │  (64MB max) │  │  (64MB max) │               │    │
│  │  │  Linear Mem │  │  Linear Mem │               │    │
│  │  └──────┬──────┘  └──────┬──────┘               │    │
│  │         │                │                      │    │
│  │  ┌──────▼────────────────▼──────┐               │    │
│  │  │      Host Function Bridge    │               │    │
│  │  │  - Validates all arguments   │               │    │
│  │  │  - Copies data (no pointers) │               │    │
│  │  │  - Rate limiting             │               │    │
│  │  └──────────────┬───────────────┘               │    │
│  └─────────────────┼───────────────────────────────┘    │
│                    │                                    │
│  ┌─────────────────▼───────────────────────────────┐    │
│  │           Yetty Card/Drawing APIs               │    │
│  │  - Scoped to module's assigned card region      │    │
│  │  - GPU resources pre-allocated per module       │    │
│  └─────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────┘
```

### 3.3 Resource Limits

```cpp
struct WasmModuleConfig {
    // Memory limits
    uint32_t maxMemoryPages = 1024;      // 64MB (64KB per page)
    uint32_t initialMemoryPages = 16;    // 1MB initial
    
    // Execution limits
    uint64_t maxInstructions = 100000000; // ~100ms @ 1GHz
    uint32_t maxCallDepth = 1000;
    
    // Time limits
    uint32_t maxExecutionMs = 16;        // One frame budget
    uint32_t watchdogIntervalMs = 100;
    
    // I/O limits
    uint32_t maxDrawCallsPerFrame = 10000;
    uint32_t maxTextureBytes = 4 * 1024 * 1024;  // 4MB
    
    // Capabilities (explicit opt-in)
    bool allowFileRead = false;
    bool allowNetworkFetch = false;
    bool allowClipboard = false;
    std::vector<std::string> allowedPaths;
};
```

### 3.4 Fuel/Gas Metering

```cpp
// Wasmtime fuel-based limiting
wasmtime_store_set_fuel(store, 1000000);  // 1M instructions

// Check remaining fuel periodically
uint64_t remaining;
wasmtime_store_get_fuel(store, &remaining);
if (remaining == 0) {
    // Module exhausted budget, pause until next frame
}

// Epoch-based interruption (for infinite loops)
wasmtime_store_epoch_deadline_trap(store);
engine_increment_epoch(engine);  // Called from watchdog thread
```

---

## 4. Host API Design

### 4.1 API Layers

```
┌─────────────────────────────────────────────────────────┐
│                  WASM Module Code                       │
├─────────────────────────────────────────────────────────┤
│              yetty_* import functions                   │
│  (declared in module, implemented by host)              │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────┐        │
│  │   Drawing   │ │    Input    │ │   System    │        │
│  │   Context   │ │   Events    │ │   Utils     │        │
│  └─────────────┘ └─────────────┘ └─────────────┘        │
│                                                         │
├─────────────────────────────────────────────────────────┤
│              Host Function Implementations              │
│         (validate, translate, dispatch to yetty)        │
└─────────────────────────────────────────────────────────┘
```

### 4.2 Core API Functions

#### Drawing API

```c
// Coordinate system: (0,0) = top-left, (width,height) = bottom-right
// Colors: 0xRRGGBBAA format

// Primitive drawing
void yetty_draw_rect(f32 x, f32 y, f32 w, f32 h, u32 fill_color, u32 stroke_color, f32 stroke_width);
void yetty_draw_rounded_rect(f32 x, f32 y, f32 w, f32 h, f32 radius, u32 fill, u32 stroke, f32 stroke_w);
void yetty_draw_circle(f32 cx, f32 cy, f32 radius, u32 fill_color, u32 stroke_color, f32 stroke_width);
void yetty_draw_ellipse(f32 cx, f32 cy, f32 rx, f32 ry, u32 fill, u32 stroke, f32 stroke_width);
void yetty_draw_line(f32 x1, f32 y1, f32 x2, f32 y2, u32 color, f32 width);
void yetty_draw_triangle(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, u32 fill, u32 stroke, f32 sw);

// Path drawing (bezier curves)
void yetty_path_begin();
void yetty_path_move_to(f32 x, f32 y);
void yetty_path_line_to(f32 x, f32 y);
void yetty_path_quad_to(f32 cx, f32 cy, f32 x, f32 y);
void yetty_path_cubic_to(f32 c1x, f32 c1y, f32 c2x, f32 c2y, f32 x, f32 y);
void yetty_path_close();
void yetty_path_fill(u32 color);
void yetty_path_stroke(u32 color, f32 width);

// Text drawing
void yetty_draw_text(f32 x, f32 y, i32 text_ptr, i32 text_len, f32 font_size, u32 color);
f32 yetty_measure_text(i32 text_ptr, i32 text_len, f32 font_size);  // Returns width

// Images/textures
i32 yetty_create_texture(i32 width, i32 height);  // Returns texture_id
void yetty_update_texture(i32 texture_id, i32 pixels_ptr, i32 pixels_len);
void yetty_draw_texture(i32 texture_id, f32 x, f32 y, f32 w, f32 h);
void yetty_destroy_texture(i32 texture_id);

// Frame management
void yetty_clear(u32 color);
void yetty_set_clip_rect(f32 x, f32 y, f32 w, f32 h);
void yetty_reset_clip();
```

#### Input API

```c
// Input state queries (polled each frame)
i32 yetty_mouse_x();           // Current mouse X (0 to width)
i32 yetty_mouse_y();           // Current mouse Y (0 to height)
i32 yetty_mouse_buttons();     // Bitmask: 1=left, 2=right, 4=middle
i32 yetty_mouse_scroll_x();    // Horizontal scroll delta
i32 yetty_mouse_scroll_y();    // Vertical scroll delta

// Keyboard (polled)
i32 yetty_key_pressed(i32 keycode);   // 1 if pressed this frame
i32 yetty_key_held(i32 keycode);      // 1 if currently held
i32 yetty_key_released(i32 keycode);  // 1 if released this frame
i32 yetty_get_char();                 // UTF-32 char input, 0 if none

// Modifier keys
i32 yetty_mod_shift();
i32 yetty_mod_ctrl();
i32 yetty_mod_alt();
```

#### System API

```c
// Timing
f64 yetty_time();              // Seconds since module load
f64 yetty_delta_time();        // Seconds since last frame
i32 yetty_frame_count();       // Frame number

// Canvas info
i32 yetty_width();             // Canvas width in pixels
i32 yetty_height();            // Canvas height in pixels
f32 yetty_dpi_scale();         // HiDPI scale factor

// Logging (for debugging)
void yetty_log(i32 level, i32 msg_ptr, i32 msg_len);  // level: 0=debug, 1=info, 2=warn, 3=error

// Random (seeded per-module for reproducibility)
f64 yetty_random();            // 0.0 to 1.0
i32 yetty_random_int(i32 max); // 0 to max-1
```

#### Optional APIs (Capability-Gated)

```c
// File system (requires allowFileRead capability)
i32 yetty_file_read(i32 path_ptr, i32 path_len, i32 buf_ptr, i32 buf_size);  // Returns bytes read
i32 yetty_file_size(i32 path_ptr, i32 path_len);

// Network (requires allowNetworkFetch capability)
i32 yetty_fetch_start(i32 url_ptr, i32 url_len);  // Returns request_id
i32 yetty_fetch_status(i32 request_id);           // 0=pending, 1=done, -1=error
i32 yetty_fetch_read(i32 request_id, i32 buf_ptr, i32 buf_size);

// Clipboard (requires allowClipboard capability)
i32 yetty_clipboard_read(i32 buf_ptr, i32 buf_size);
void yetty_clipboard_write(i32 text_ptr, i32 text_len);

// Storage (per-module persistent key-value, sandboxed)
void yetty_storage_set(i32 key_ptr, i32 key_len, i32 val_ptr, i32 val_len);
i32 yetty_storage_get(i32 key_ptr, i32 key_len, i32 buf_ptr, i32 buf_size);
```

### 4.3 Module Entry Points

WASM modules must export these functions:

```c
// Required exports
void yetty_init(i32 width, i32 height);  // Called once on load
void yetty_frame(f64 time);              // Called each frame
void yetty_resize(i32 width, i32 height); // Called on resize

// Optional exports
void yetty_destroy();                    // Called before unload
void yetty_focus(i32 focused);           // Focus gained/lost
void yetty_suspend();                    // App going to background
void yetty_resume();                     // App returning to foreground
```

### 4.4 Example WASM Module (Rust)

```rust
// lib.rs - Bouncing ball example
#![no_std]

mod yetty {
    extern "C" {
        pub fn yetty_draw_circle(cx: f32, cy: f32, r: f32, fill: u32, stroke: u32, sw: f32);
        pub fn yetty_clear(color: u32);
        pub fn yetty_width() -> i32;
        pub fn yetty_height() -> i32;
    }
}

static mut BALL_X: f32 = 100.0;
static mut BALL_Y: f32 = 100.0;
static mut VEL_X: f32 = 200.0;
static mut VEL_Y: f32 = 150.0;

#[no_mangle]
pub extern "C" fn yetty_init(_width: i32, _height: i32) {
    // Initialize state
}

#[no_mangle]
pub extern "C" fn yetty_frame(time: f64) {
    unsafe {
        let dt = 0.016;  // ~60fps
        let w = yetty::yetty_width() as f32;
        let h = yetty::yetty_height() as f32;
        
        // Update position
        BALL_X += VEL_X * dt;
        BALL_Y += VEL_Y * dt;
        
        // Bounce off walls
        if BALL_X < 20.0 || BALL_X > w - 20.0 { VEL_X = -VEL_X; }
        if BALL_Y < 20.0 || BALL_Y > h - 20.0 { VEL_Y = -VEL_Y; }
        
        // Draw
        yetty::yetty_clear(0x1a1a2eff);  // Dark background
        yetty::yetty_draw_circle(BALL_X, BALL_Y, 20.0, 0xff6b6bff, 0xffffffff, 2.0);
    }
}

#[no_mangle]
pub extern "C" fn yetty_resize(width: i32, height: i32) {
    // Handle resize
}
```

Compile with:
```bash
cargo build --target wasm32-unknown-unknown --release
```

### 4.5 Example WASM Module (C)

```c
// widget.c - Simple clock widget
#include <stdint.h>
#include <math.h>

// Host imports
extern void yetty_draw_circle(float cx, float cy, float r, uint32_t fill, uint32_t stroke, float sw);
extern void yetty_draw_line(float x1, float y1, float x2, float y2, uint32_t color, float w);
extern void yetty_clear(uint32_t color);
extern int32_t yetty_width(void);
extern int32_t yetty_height(void);
extern double yetty_time(void);

static float center_x, center_y, radius;

void yetty_init(int32_t width, int32_t height) {
    center_x = width / 2.0f;
    center_y = height / 2.0f;
    radius = (width < height ? width : height) / 2.0f - 10.0f;
}

void yetty_frame(double time) {
    yetty_clear(0x2d2d2dff);
    
    // Clock face
    yetty_draw_circle(center_x, center_y, radius, 0x404040ff, 0xffffffff, 2.0f);
    
    // Hour marks
    for (int i = 0; i < 12; i++) {
        float angle = i * 3.14159f / 6.0f - 3.14159f / 2.0f;
        float x1 = center_x + cosf(angle) * (radius - 10);
        float y1 = center_y + sinf(angle) * (radius - 10);
        float x2 = center_x + cosf(angle) * (radius - 20);
        float y2 = center_y + sinf(angle) * (radius - 20);
        yetty_draw_line(x1, y1, x2, y2, 0xffffffff, 2.0f);
    }
    
    // Hands
    float seconds = fmod(time, 60.0);
    float minutes = fmod(time / 60.0, 60.0);
    float hours = fmod(time / 3600.0, 12.0);
    
    // Second hand
    float sec_angle = seconds * 3.14159f / 30.0f - 3.14159f / 2.0f;
    yetty_draw_line(center_x, center_y,
                    center_x + cosf(sec_angle) * (radius - 30),
                    center_y + sinf(sec_angle) * (radius - 30),
                    0xff0000ff, 1.0f);
    
    // Minute hand
    float min_angle = minutes * 3.14159f / 30.0f - 3.14159f / 2.0f;
    yetty_draw_line(center_x, center_y,
                    center_x + cosf(min_angle) * (radius - 40),
                    center_y + sinf(min_angle) * (radius - 40),
                    0xffffffff, 3.0f);
    
    // Hour hand
    float hour_angle = hours * 3.14159f / 6.0f - 3.14159f / 2.0f;
    yetty_draw_line(center_x, center_y,
                    center_x + cosf(hour_angle) * (radius - 60),
                    center_y + sinf(hour_angle) * (radius - 60),
                    0xffffffff, 5.0f);
}

void yetty_resize(int32_t width, int32_t height) {
    yetty_init(width, height);
}
```

Compile with:
```bash
clang --target=wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -o clock.wasm widget.c
```

---

## 5. Implementation Architecture

### 5.1 Component Overview

```
┌─────────────────────────────────────────────────────────┐
│                    WasmPlugin                           │
│  - Manages WASM runtime lifecycle                       │
│  - Loads/unloads modules                                │
│  - Handles module marketplace/registry                  │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  ┌─────────────────┐    ┌─────────────────┐             │
│  │  WasmRuntime    │    │  WasmModule     │  (1 per     │
│  │  (singleton)    │◄───│  Instance       │   widget)   │
│  │                 │    │                 │             │
│  │  - Engine       │    │  - Store/Ctx    │             │
│  │  - Epoch thread │    │  - Memory       │             │
│  │  - Module cache │    │  - Imports      │             │
│  └─────────────────┘    │  - Exports      │             │
│                         │  - Config       │             │
│                         └────────┬────────┘             │
│                                  │                      │
│  ┌───────────────────────────────▼──────────────────┐   │
│  │              WasmHostBridge                      │   │
│  │  - Implements all yetty_* functions              │   │
│  │  - Translates calls to YDraw/Card APIs           │   │
│  │  - Validates inputs, enforces limits             │   │
│  └───────────────────────────────┬──────────────────┘   │
│                                  │                      │
└──────────────────────────────────┼──────────────────────┘
                                   │
                                   ▼
┌─────────────────────────────────────────────────────────┐
│                YDraw Card (existing)                    │
│  - addCircle(), addBox(), addBezier(), etc.             │
│  - GPU-accelerated SDF rendering                        │
└─────────────────────────────────────────────────────────┘
```

### 5.2 Class Design

```cpp
// wasm-runtime.h
class WasmRuntime {
public:
    static WasmRuntime& instance();
    
    // Module loading
    Result<WasmModuleId> loadModule(const std::vector<uint8_t>& wasmBytes);
    Result<WasmModuleId> loadModuleFromFile(const std::string& path);
    void unloadModule(WasmModuleId id);
    
    // Execution control
    void setEpoch(uint64_t epoch);  // For timeout detection
    void incrementEpoch();
    
private:
    wasm_engine_t* _engine;
    std::unordered_map<WasmModuleId, wasmtime_module_t*> _modules;
    std::thread _epochThread;
};

// wasm-module-instance.h
class WasmModuleInstance {
public:
    WasmModuleInstance(WasmModuleId moduleId, const WasmModuleConfig& config);
    ~WasmModuleInstance();
    
    // Lifecycle
    Result<void> init(int32_t width, int32_t height);
    void frame(double time);
    void resize(int32_t width, int32_t height);
    void destroy();
    
    // Input forwarding
    void onMouseMove(int32_t x, int32_t y);
    void onMouseButton(int32_t button, bool pressed);
    void onMouseScroll(int32_t dx, int32_t dy);
    void onKey(int32_t keycode, bool pressed);
    void onChar(uint32_t codepoint);
    
    // Resource access (for host functions)
    WasmHostBridge* bridge() { return _bridge.get(); }
    
private:
    wasmtime_store_t* _store;
    wasmtime_instance_t _instance;
    wasmtime_func_t _initFn, _frameFn, _resizeFn, _destroyFn;
    std::unique_ptr<WasmHostBridge> _bridge;
    WasmModuleConfig _config;
};

// wasm-host-bridge.h
class WasmHostBridge {
public:
    WasmHostBridge(YDraw::Ptr ydraw, const WasmModuleConfig& config);
    
    // Called by WASM module through imports
    void drawRect(float x, float y, float w, float h, uint32_t fill, uint32_t stroke, float sw);
    void drawCircle(float cx, float cy, float r, uint32_t fill, uint32_t stroke, float sw);
    void drawLine(float x1, float y1, float x2, float y2, uint32_t color, float width);
    // ... etc
    
    // Per-frame bookkeeping
    void beginFrame();
    void endFrame();
    
    // Input state
    int32_t mouseX() const { return _mouseX; }
    int32_t mouseY() const { return _mouseY; }
    int32_t mouseButtons() const { return _mouseButtons; }
    
private:
    YDraw::Ptr _ydraw;
    WasmModuleConfig _config;
    uint32_t _drawCallCount;
    
    // Input state
    int32_t _mouseX, _mouseY;
    int32_t _mouseButtons;
    int32_t _scrollDx, _scrollDy;
    std::bitset<512> _keysPressed;
    std::bitset<512> _keysHeld;
    std::queue<uint32_t> _charQueue;
};
```

### 5.3 Host Function Registration

```cpp
// wasm-imports.cpp

static wasm_trap_t* host_draw_circle(
    void* env,
    wasmtime_caller_t* caller,
    const wasmtime_val_t* args,
    size_t nargs,
    wasmtime_val_t* results,
    size_t nresults
) {
    auto* bridge = static_cast<WasmHostBridge*>(env);
    
    float cx = args[0].of.f32;
    float cy = args[1].of.f32;
    float r = args[2].of.f32;
    uint32_t fill = args[3].of.i32;
    uint32_t stroke = args[4].of.i32;
    float sw = args[5].of.f32;
    
    bridge->drawCircle(cx, cy, r, fill, stroke, sw);
    return nullptr;  // No trap
}

void registerHostFunctions(wasmtime_linker_t* linker, WasmHostBridge* bridge) {
    // Drawing functions
    wasmtime_functype_t* draw_circle_type = wasmtime_functype_new(
        {WASM_F32, WASM_F32, WASM_F32, WASM_I32, WASM_I32, WASM_F32},  // params
        {}  // results
    );
    wasmtime_linker_define_func(linker, "env", "yetty_draw_circle",
        draw_circle_type, host_draw_circle, bridge, nullptr);
    
    // ... register all other functions similarly
}
```

---

## 6. Widget Plugin Integration

### 6.1 WasmWidget Class

```cpp
// wasm-widget.h
class WasmWidget : public Widget {
public:
    static Result<WidgetPtr> create(
        const std::string& wasmPath,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const WasmModuleConfig& config
    );
    
    // Widget interface
    Result<void> init() override;
    void prepareFrame() override;
    void render(WGPURenderPassEncoder encoder) override;
    void update(double deltaTime) override;
    void dispose() override;
    
    // Input
    bool onMouseMove(int32_t x, int32_t y) override;
    bool onMouseButton(int32_t button, bool pressed) override;
    bool onMouseScroll(int32_t dx, int32_t dy) override;
    bool onKey(int32_t keycode, bool pressed) override;
    bool onChar(uint32_t codepoint) override;
    
private:
    std::unique_ptr<WasmModuleInstance> _wasmInstance;
    YDraw::Ptr _ydraw;  // SDF drawing card
    double _elapsedTime = 0;
};
```

### 6.2 WasmPlugin Factory

```cpp
// wasm-plugin.h
class WasmPlugin : public Plugin {
public:
    const char* pluginName() const override { return "wasm"; }
    
    Result<WidgetPtr> createWidget(
        const std::string& widgetName,
        WidgetFactory* factory,
        FontManager* fontManager,
        uv_loop_t* loop,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& pluginArgs,
        const std::string& payload
    ) override;
    
    // Module management
    Result<WasmModuleId> preloadModule(const std::string& path);
    void setDefaultConfig(const WasmModuleConfig& config);
    
private:
    WasmModuleConfig _defaultConfig;
};
```

### 6.3 Usage from Terminal

```bash
# Load WASM widget from file
yetty --wasm /path/to/widget.wasm --size 40x20

# Load with configuration
yetty --wasm widget.wasm --wasm-memory 32MB --wasm-timeout 32ms

# Load from URL (if network capability enabled)
yetty --wasm https://registry.example.com/widgets/clock.wasm

# Multiple widgets
yetty --wasm clock.wasm --at 0,0 --size 20x10 \
      --wasm graph.wasm --at 20,0 --size 20x10
```

---

## 7. Module Distribution & Registry

### 7.1 Module Package Format

```
widget.ypm  (Yetty Package Module - zip archive)
├── manifest.yaml
├── widget.wasm
├── icon.png (optional)
├── README.md (optional)
└── assets/ (optional)
    ├── texture1.png
    └── data.json
```

**manifest.yaml:**
```yaml
name: analog-clock
version: 1.0.0
author: "Developer Name"
license: MIT
description: "A beautiful analog clock widget"

# Entry point
main: widget.wasm

# Required capabilities (user must approve)
capabilities:
  - storage  # Persist settings

# Resource requirements
resources:
  memory: 4MB
  texture: 256KB

# Minimum yetty version
requires: ">=0.5.0"

# Tags for discovery
tags: [clock, time, utility]
```

### 7.2 Registry API

```
GET  /api/v1/widgets                     # List all widgets
GET  /api/v1/widgets?tag=clock           # Filter by tag
GET  /api/v1/widgets/{name}              # Get widget metadata
GET  /api/v1/widgets/{name}/versions     # List versions
GET  /api/v1/widgets/{name}/{version}    # Download specific version
POST /api/v1/widgets                     # Publish new widget (authenticated)
```

### 7.3 Local Cache

```
~/.yetty/wasm/
├── cache/
│   └── {sha256-hash}.wasm    # Cached compiled modules
├── installed/
│   ├── analog-clock/
│   │   ├── 1.0.0/
│   │   │   ├── manifest.yaml
│   │   │   └── widget.wasm
│   │   └── current -> 1.0.0
│   └── stock-chart/
│       └── ...
└── registry-cache.json       # Cached registry metadata
```

---

## 8. Implementation Phases

### Phase 1: Core Runtime (2 weeks)

- [ ] Integrate Wasmtime via CMake (CPMAddPackage)
- [ ] `WasmRuntime` singleton with engine management
- [ ] `WasmModuleInstance` with basic lifecycle
- [ ] Minimal host functions: `yetty_clear`, `yetty_width`, `yetty_height`, `yetty_time`
- [ ] Epoch-based timeout protection
- [ ] Basic fuel metering

**Deliverable:** Load and run minimal WASM module that clears screen.

### Phase 2: Drawing API (2 weeks)

- [ ] `WasmHostBridge` with full drawing API
- [ ] Integration with `YDraw` card
- [ ] Path API (begin/move/line/curve/close/fill/stroke)
- [ ] Text rendering via `yetty_draw_text`
- [ ] Texture creation and drawing

**Deliverable:** WASM modules can draw arbitrary 2D graphics.

### Phase 3: Input & Interaction (1 week)

- [ ] Mouse position/button state
- [ ] Keyboard state
- [ ] Character input queue
- [ ] Focus events
- [ ] Scroll events

**Deliverable:** Interactive WASM widgets responding to input.

### Phase 4: Plugin & Widget Integration (1 week)

- [ ] `WasmWidget` class implementing Widget interface
- [ ] `WasmPlugin` factory
- [ ] Command-line arguments parsing
- [ ] Configuration file support

**Deliverable:** `yetty --wasm widget.wasm` works.

### Phase 5: Security Hardening (1 week)

- [ ] Memory limit enforcement
- [ ] Draw call rate limiting
- [ ] Capability system for optional APIs
- [ ] Watchdog thread for runaway modules
- [ ] Audit all host function inputs

**Deliverable:** Safe to run untrusted code.

### Phase 6: Distribution (2 weeks)

- [ ] Package format (`.ypm`)
- [ ] Local installation/cache
- [ ] Registry client
- [ ] `yetty wasm install <name>` command
- [ ] Signature verification (optional)

**Deliverable:** Users can install widgets from registry.

---

## 9. Testing Strategy

### 9.1 Test Modules

| Module | Purpose |
|--------|---------|
| `minimal.wasm` | Just clears screen - tests basic lifecycle |
| `shapes.wasm` | Draws all primitive types |
| `input-echo.wasm` | Displays mouse/keyboard state |
| `stress-draw.wasm` | Max draw calls - tests rate limiting |
| `infinite-loop.wasm` | Tests timeout protection |
| `memory-hog.wasm` | Allocates max memory - tests limits |
| `malformed.wasm` | Invalid WASM - tests error handling |

### 9.2 Security Tests

- [ ] Cannot access host memory outside linear memory
- [ ] Timeout triggers for infinite loops
- [ ] Memory limit prevents OOM
- [ ] Draw call limit prevents GPU stall
- [ ] Capability-gated functions fail without permission
- [ ] Invalid inputs don't crash host

### 9.3 Compatibility Tests

- [ ] Rust-compiled WASM modules
- [ ] C/C++-compiled WASM modules
- [ ] AssemblyScript modules
- [ ] TinyGo modules

---

## 10. Performance Considerations

### 10.1 Overhead Sources

| Source | Mitigation |
|--------|------------|
| WASM → Host call overhead | Batch drawing commands |
| Memory copy for strings | Pass pointers + length, copy once |
| Per-frame fuel check | Check every N instructions, not every one |
| JIT compilation | Cache compiled modules |

### 10.2 Optimization Techniques

1. **Command batching**: Buffer draw calls, flush at frame end
2. **Compiled module cache**: Store AOT-compiled modules on disk
3. **Lazy initialization**: Don't JIT until first call
4. **Memory pooling**: Reuse WASM linear memory between loads

### 10.3 Expected Performance

| Operation | Overhead |
|-----------|----------|
| Host function call | ~50-100ns |
| Draw primitive | ~1-5μs (including SDF primitive creation) |
| Full frame (1000 draws) | ~1-5ms |
| Module instantiation | ~10-50ms (cached: ~1ms) |

---

## 11. Alternative Approaches Considered

### 11.1 Lua Scripting

**Pros:** Simpler, smaller, faster embedding
**Cons:** Less sandboxing, no memory isolation, easier to escape

### 11.2 JavaScript (QuickJS/V8)

**Pros:** Familiar language, huge ecosystem
**Cons:** Larger runtime, GC pauses, harder to sandbox properly

### 11.3 Native Plugins (dlopen)

**Pros:** Maximum performance
**Cons:** No sandboxing, platform-specific, security nightmare

### 11.4 Decision

WASM chosen for:
- Strong sandboxing guarantees
- Multi-language support (Rust, C, C++, AssemblyScript, Go, etc.)
- Growing ecosystem and tooling
- Near-native performance
- Industry adoption for similar use cases (Figma, Shopify, Cloudflare)

---

## 12. References

- [WebAssembly Specification](https://webassembly.github.io/spec/core/)
- [WASI Specification](https://wasi.dev/)
- [Wasmtime Documentation](https://docs.wasmtime.dev/)
- [Wasmtime C API](https://docs.wasmtime.dev/c-api/)
- [wasm3 Repository](https://github.com/xetechnologies/wasm3)
- [Bytecode Alliance Security](https://bytecodealliance.org/articles/security-and-correctness-in-wasmtime)
- [Figma's WASM Plugin System](https://www.figma.com/blog/an-update-on-plugin-security/)
