#pragma once

#include <yetty/result.hpp>
#include <yetty/webgpu-context.h>
#include <yetty/gpu-allocator.h>
#include <webgpu/webgpu.h>
#include <vector>
#include <string>
#include <unordered_map>

namespace yetty {

//-----------------------------------------------------------------------------
// HDraw Primitive Types - 2D and 3D SDFs (Inigo Quilez)
//-----------------------------------------------------------------------------

enum class HDrawPrimitiveType : uint32_t {
    // 2D Primitives
    Circle2D = 0,
    Box2D = 1,
    Segment2D = 2,
    Triangle2D = 3,
    Bezier2D = 4,         // Quadratic bezier
    Arc2D = 5,            // Circular arc
    Ellipse2D = 6,
    CubicBezier2D = 7,
    EllipseArc2D = 8,     // Elliptical arc (for SVG arc command)
    Polygon2D = 9,

    // 3D Primitives (iquilezles.org)
    Sphere3D = 100,
    Box3D = 101,
    BoxFrame3D = 102,
    Torus3D = 103,
    CappedTorus3D = 104,
    Cylinder3D = 105,
    CappedCylinder3D = 106,
    RoundedCylinder3D = 107,
    Capsule3D = 108,
    Cone3D = 109,
    CappedCone3D = 110,
    RoundCone3D = 111,
    Plane3D = 112,
    HexPrism3D = 113,
    TriPrism3D = 114,
    Octahedron3D = 115,
    Pyramid3D = 116,
    Ellipsoid3D = 117,
    Rhombus3D = 118,
    Link3D = 119,
};

struct HDrawStyle {
    float fill[4] = {1.0f, 1.0f, 1.0f, 1.0f};      // RGBA
    float stroke[4] = {0.0f, 0.0f, 0.0f, 0.0f};    // RGBA (alpha 0 = no stroke)
    float stroke_width = 0.0f;
    float round = 0.0f;
    float rotate = 0.0f;  // degrees
    float _pad = 0.0f;
};

// GPU-side primitive data (padded to 16-byte alignment)
struct HDrawPrimitiveGPU {
    uint32_t type;
    float params[15];  // Flexible params based on type
    HDrawStyle style;  // 48 bytes
};

//-----------------------------------------------------------------------------
// YAML AST Types for HDraw
//-----------------------------------------------------------------------------

struct HDrawStruct {
    std::string name;
    std::vector<std::string> args;
    std::unordered_map<std::string, std::string> defaults;
    std::string body_yaml;  // Raw YAML for lazy evaluation
};

struct HDrawWidget {
    std::string name;
    std::vector<std::string> args;
    std::unordered_map<std::string, std::string> defaults;
    std::string body_yaml;  // Raw YAML for lazy evaluation
};

//-----------------------------------------------------------------------------
// HDrawRenderer - Core 2D/3D SDF rendering
//-----------------------------------------------------------------------------

class HDrawRenderer {
public:
    static constexpr size_t MAX_PRIMITIVES = 256;

    HDrawRenderer();
    ~HDrawRenderer();

    // Initialize renderer with optional YAML/SVG content
    Result<void> init(const std::string& content = "");
    Result<void> dispose();

    // Clear all primitives
    void clear();

    // Add primitives programmatically
    void addPrimitive(const HDrawPrimitiveGPU& prim);

    // Parse content (YAML or SVG)
    Result<void> parse(const std::string& content);

    // Render to the given pass
    Result<void> render(WebGPUContext& ctx,
                        GpuAllocator::Ptr allocator,
                        WGPURenderPassEncoder pass,
                        float x, float y, float width, float height,
                        float screenWidth, float screenHeight,
                        WGPUTextureFormat targetFormat);

    // Get primitive count
    size_t primitiveCount() const { return _primitives.size(); }

    // Color/coordinate parsing helpers
    static bool parseColor(const std::string& color, float out[4]);
    static bool parseVec2(const std::string& str, float out[2]);
    static bool parseVec3(const std::string& str, float out[3]);

private:
    Result<void> createPipeline(WebGPUContext& ctx, GpuAllocator::Ptr allocator, WGPUTextureFormat targetFormat);
    Result<void> parseYAML(const std::string& yaml);
    Result<void> parseSVG(const std::string& svg);

    // YAML processing helpers
    void registerStruct(const HDrawStruct& s);
    void registerWidget(const HDrawWidget& w);
    HDrawStyle expandStruct(const std::string& name,
                            const std::vector<std::string>& positional_args);

    // Definitions
    std::unordered_map<std::string, HDrawStruct> _structs;
    std::unordered_map<std::string, HDrawWidget> _widgets;

    // Primitives to render
    std::vector<HDrawPrimitiveGPU> _primitives;
    bool _primitives_dirty = true;

    float _time = 0.0f;

    // GPU resources
    GpuAllocator::Ptr _allocator;
    WGPURenderPipeline _pipeline = nullptr;
    WGPUBindGroup _bind_group = nullptr;
    WGPUBuffer _uniform_buffer = nullptr;
    WGPUBuffer _primitive_buffer = nullptr;

    WGPUTextureFormat _current_format = WGPUTextureFormat_Undefined;
    bool _gpu_initialized = false;
    bool _failed = false;
};

//-----------------------------------------------------------------------------
// Helper: 2D Primitive Builders
//-----------------------------------------------------------------------------

namespace hdraw {

// 2D primitives
HDrawPrimitiveGPU circle(float x, float y, float radius, const HDrawStyle& style = {});
HDrawPrimitiveGPU box(float x, float y, float halfW, float halfH, const HDrawStyle& style = {});
HDrawPrimitiveGPU segment(float x1, float y1, float x2, float y2, const HDrawStyle& style = {});
HDrawPrimitiveGPU triangle(float x0, float y0, float x1, float y1, float x2, float y2, const HDrawStyle& style = {});
HDrawPrimitiveGPU ellipse(float x, float y, float rx, float ry, const HDrawStyle& style = {});

// 3D primitives (raymarched)
HDrawPrimitiveGPU sphere(float x, float y, float z, float radius, const HDrawStyle& style = {});
HDrawPrimitiveGPU box3d(float x, float y, float z, float hx, float hy, float hz, const HDrawStyle& style = {});
HDrawPrimitiveGPU torus(float x, float y, float z, float majorR, float minorR, const HDrawStyle& style = {});
HDrawPrimitiveGPU cylinder(float x, float y, float z, float radius, float height, const HDrawStyle& style = {});
HDrawPrimitiveGPU capsule(float x, float y, float z, float radius, float height, const HDrawStyle& style = {});
HDrawPrimitiveGPU cone(float x, float y, float z, float radius, float height, const HDrawStyle& style = {});
HDrawPrimitiveGPU octahedron(float x, float y, float z, float size, const HDrawStyle& style = {});
HDrawPrimitiveGPU pyramid(float x, float y, float z, float size, float height, const HDrawStyle& style = {});

// Style helpers
HDrawStyle solid(float r, float g, float b, float a = 1.0f);
HDrawStyle stroke(float r, float g, float b, float width, float a = 1.0f);

} // namespace hdraw

} // namespace yetty
