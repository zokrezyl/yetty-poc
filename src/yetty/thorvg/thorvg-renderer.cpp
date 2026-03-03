#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "thorvg-renderer.h"
#include "../ydraw/ydraw-buffer.h"
#include "../ydraw/ydraw-types.gen.h"
#include <thorvg.h>
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cstring>
#include <mutex>
#include <stack>
#include <vector>

namespace yetty::thorvg {

//=============================================================================
// ThorVG engine reference counting
//=============================================================================

static int s_thorvgRefCount = 0;
static std::mutex s_thorvgMutex;

Result<void> thorvgInit() {
    std::lock_guard<std::mutex> lock(s_thorvgMutex);
    if (s_thorvgRefCount == 0) {
        auto result = tvg::Initializer::init(0);
        if (result != tvg::Result::Success) {
            return Err<void>("ThorVG: tvg::Initializer::init failed");
        }
        uint32_t major, minor, micro;
        const char* version = tvg::Initializer::version(&major, &minor, &micro);
        yinfo("ThorVG engine initialized: {}", version ? version : "unknown");
    }
    ++s_thorvgRefCount;
    return Ok();
}

void thorvgTerm() {
    std::lock_guard<std::mutex> lock(s_thorvgMutex);
    if (--s_thorvgRefCount == 0) {
        tvg::Initializer::term();
        yinfo("ThorVG engine terminated");
    }
}

//=============================================================================
// Color conversion helpers
//=============================================================================

static uint32_t rgbaToPackedABGR(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    // YDraw uses ABGR format: 0xAABBGGRR
    return (static_cast<uint32_t>(a) << 24) |
           (static_cast<uint32_t>(b) << 16) |
           (static_cast<uint32_t>(g) << 8) |
           static_cast<uint32_t>(r);
}

//=============================================================================
// ThorVgRendererImpl
//=============================================================================

class ThorVgRendererImpl : public ThorVgRenderer {
public:
    explicit ThorVgRendererImpl(std::shared_ptr<YDrawBuffer> buffer)
        : _buffer(std::move(buffer)) {}

    ~ThorVgRendererImpl() override {
        clear();
        thorvgTerm();
    }

    Result<void> load(const std::string& data, const char* mimeType) override {
        clear();

        // Auto-detect content type if not specified
        std::string detectedType;
        if (!mimeType) {
            size_t start = data.find_first_not_of(" \t\n\r");
            if (start != std::string::npos && data[start] == '{') {
                detectedType = "lottie";
            } else if (data.find("<svg") != std::string::npos) {
                detectedType = "svg";
            }
            mimeType = detectedType.empty() ? nullptr : detectedType.c_str();
        }

        // Create Animation wrapper (manages Picture lifetime)
        _animation.reset(tvg::Animation::gen());
        if (!_animation) {
            return Err<void>("ThorVgRenderer: failed to create Animation");
        }

        tvg::Picture* picture = _animation->picture();
        if (!picture) {
            _animation.reset();
            return Err<void>("ThorVgRenderer: failed to get picture from Animation");
        }

        // Load content
        tvg::Result result;
        if (mimeType && std::string(mimeType) == "lottie") {
            result = picture->load(data.c_str(), static_cast<uint32_t>(data.size()),
                                   "lottie", nullptr, true);
            if (result == tvg::Result::Success) {
                _totalFrames = _animation->totalFrame();
                _duration = _animation->duration();
                _isAnimated = _totalFrames > 1;
            }
        } else {
            result = picture->load(data.c_str(), static_cast<uint32_t>(data.size()),
                                   mimeType, nullptr, true);
            _isAnimated = false;
        }

        if (result != tvg::Result::Success) {
            _animation.reset();
            return Err<void>("ThorVgRenderer: failed to load content (result=" + std::to_string(static_cast<int>(result)) + ")");
        }

        // Get content dimensions
        float w = 0, h = 0;
        picture->size(&w, &h);
        _contentWidth = w > 0 ? w : 256;
        _contentHeight = h > 0 ? h : 256;

        _picture = picture;
        ydebug("ThorVgRenderer: loaded {}x{} animated={}", _contentWidth, _contentHeight, _isAnimated);
        return Ok();
    }

    Result<void> loadFile(const std::string& path) override {
        clear();

        _animation.reset(tvg::Animation::gen());
        if (!_animation) {
            return Err<void>("ThorVgRenderer: failed to create Animation");
        }

        tvg::Picture* picture = _animation->picture();
        if (!picture) {
            _animation.reset();
            return Err<void>("ThorVgRenderer: failed to get picture from Animation");
        }

        auto result = picture->load(path.c_str());
        if (result != tvg::Result::Success) {
            _animation.reset();
            return Err<void>("ThorVgRenderer: failed to load file: " + path);
        }

        // Check if it's animated (Lottie)
        _totalFrames = _animation->totalFrame();
        _duration = _animation->duration();
        _isAnimated = (_totalFrames > 1);

        float w = 0, h = 0;
        picture->size(&w, &h);
        _contentWidth = w > 0 ? w : 256;
        _contentHeight = h > 0 ? h : 256;

        _picture = picture;
        return Ok();
    }

    float contentWidth() const override { return _contentWidth; }
    float contentHeight() const override { return _contentHeight; }

    bool isAnimated() const override { return _isAnimated; }
    float totalFrames() const override { return _totalFrames; }
    float duration() const override { return _duration; }

    void setFrame(float frame) override {
        if (_animation && _isAnimated) {
            _currentFrame = frame;
            _animation->frame(frame);
        }
    }

    Result<void> render() override {
        if (!_picture) {
            return Err<void>("ThorVgRenderer: no content loaded");
        }

        uint32_t beforeCount = _buffer->primCount();
        _buffer->clear();
        _nextPrimId = 0;

        // Use Accessor to traverse, computing world transforms via parent chain
        renderWithAccessor(_picture);

        yinfo("ThorVgRenderer::render: frame={:.1f} prims before={} after={}", 
              _currentFrame, beforeCount, _buffer->primCount());
        return Ok();
    }

    void clear() override {
        _animation.reset();
        _picture = nullptr;
        _contentWidth = 0;
        _contentHeight = 0;
        _isAnimated = false;
        _totalFrames = 0;
        _duration = 0;
        _currentFrame = 0;
    }

private:
    //=========================================================================
    // Matrix operations
    //=========================================================================

    // Multiply two 2D affine matrices
    tvg::Matrix multiplyMatrix(const tvg::Matrix& a, const tvg::Matrix& b) {
        return {
            a.e11 * b.e11 + a.e12 * b.e21,  // e11
            a.e11 * b.e12 + a.e12 * b.e22,  // e12
            a.e11 * b.e13 + a.e12 * b.e23 + a.e13,  // e13
            a.e21 * b.e11 + a.e22 * b.e21,  // e21
            a.e21 * b.e12 + a.e22 * b.e22,  // e22
            a.e21 * b.e13 + a.e22 * b.e23 + a.e23,  // e23
            0, 0, 1  // e31, e32, e33 (always identity for 2D)
        };
    }

    // Apply 2D affine transform to a point
    tvg::Point transformPoint(const tvg::Point& p, const tvg::Matrix& m) {
        return {
            m.e11 * p.x + m.e12 * p.y + m.e13,
            m.e21 * p.x + m.e22 * p.y + m.e23
        };
    }

    //=========================================================================
    // Scene graph traversal
    //=========================================================================

    void renderPaintRecursive(const tvg::Paint* paint, const tvg::Matrix& parentTransform) {
        if (!paint) return;

        // Get this node's local transform
        auto* mutablePaint = const_cast<tvg::Paint*>(paint);
        tvg::Matrix& local = mutablePaint->transform();
        
        // Accumulate with parent transform
        tvg::Matrix world = multiplyMatrix(parentTransform, local);

        auto type = paint->type();

        if (type == tvg::Type::Shape) {
            renderShape(static_cast<const tvg::Shape*>(paint), world);
        }
        // For Scene/Picture, the Accessor will handle traversal - we just need to
        // apply transforms. But Accessor doesn't give us hierarchy info, so we use
        // a simple approach: traverse once with Accessor, accumulating transforms.
    }

    // Main entry point for rendering - uses Accessor with transform tracking
    void renderWithAccessor(tvg::Picture* picture) {
        auto accessor = std::unique_ptr<tvg::Accessor>(tvg::Accessor::gen());
        if (!accessor) return;

        // Build parent chain for each paint to compute world transforms
        // Accessor traverses depth-first, so we track the transform as we go
        
        accessor->set(picture, [](const tvg::Paint* paint, void* data) -> bool {
            auto* self = static_cast<ThorVgRendererImpl*>(data);
            
            if (paint->type() == tvg::Type::Shape) {
                // Compute world transform by walking up parent chain
                tvg::Matrix world = {1, 0, 0, 0, 1, 0, 0, 0, 1};
                self->computeWorldTransform(paint, world);
                self->renderShape(static_cast<const tvg::Shape*>(paint), world);
            }
            return true;
        }, this);
    }

    void computeWorldTransform(const tvg::Paint* paint, tvg::Matrix& result) {
        // Walk up parent chain accumulating transforms
        std::vector<const tvg::Paint*> chain;
        const tvg::Paint* p = paint;
        while (p) {
            chain.push_back(p);
            p = p->parent();
        }
        
        // Start with identity and multiply from root to leaf
        result = {1, 0, 0, 0, 1, 0, 0, 0, 1};
        for (auto it = chain.rbegin(); it != chain.rend(); ++it) {
            auto* mutablePaint = const_cast<tvg::Paint*>(*it);
            tvg::Matrix& local = mutablePaint->transform();
            result = multiplyMatrix(result, local);
        }
    }

    void renderShape(const tvg::Shape* shape, const tvg::Matrix& m) {
        if (!shape) return;

        // Get paint opacity (0-255) and apply to colors
        uint8_t paintOpacity = shape->opacity();
        
        // Check for gradient fill
        const tvg::Fill* gradientFill = shape->fill();
        bool hasLinearGradient = false;
        bool hasRadialGradient = false;
        float gx1 = 0, gy1 = 0, gx2 = 0, gy2 = 0;  // Linear gradient endpoints
        float gcx = 0, gcy = 0, gr = 0;             // Radial gradient params
        uint32_t gradColor1 = 0, gradColor2 = 0;
        
        if (gradientFill) {
            auto fillType = gradientFill->type();
            if (fillType == tvg::Type::LinearGradient) {
                hasLinearGradient = true;
                auto* lg = static_cast<const tvg::LinearGradient*>(gradientFill);
                lg->linear(&gx1, &gy1, &gx2, &gy2);
                // Transform gradient endpoints by matrix
                tvg::Point gStart = transformPoint({gx1, gy1}, m);
                tvg::Point gEnd = transformPoint({gx2, gy2}, m);
                gx1 = gStart.x; gy1 = gStart.y;
                gx2 = gEnd.x; gy2 = gEnd.y;
                
                // Get color stops
                const tvg::Fill::ColorStop* stops = nullptr;
                uint32_t stopCount = gradientFill->colorStops(&stops);
                if (stopCount >= 2 && stops) {
                    gradColor1 = rgbaToPackedABGR(stops[0].r, stops[0].g, stops[0].b, 
                        static_cast<uint8_t>((stops[0].a * paintOpacity) / 255));
                    gradColor2 = rgbaToPackedABGR(stops[stopCount-1].r, stops[stopCount-1].g, stops[stopCount-1].b,
                        static_cast<uint8_t>((stops[stopCount-1].a * paintOpacity) / 255));
                } else if (stopCount == 1 && stops) {
                    gradColor1 = gradColor2 = rgbaToPackedABGR(stops[0].r, stops[0].g, stops[0].b,
                        static_cast<uint8_t>((stops[0].a * paintOpacity) / 255));
                }
            } else if (fillType == tvg::Type::RadialGradient) {
                hasRadialGradient = true;
                auto* rg = static_cast<const tvg::RadialGradient*>(gradientFill);
                float fx = 0, fy = 0, fr = 0;  // focal point (ignored for now)
                rg->radial(&gcx, &gcy, &gr, &fx, &fy, &fr);
                // Transform gradient center by matrix (radius needs scale)
                tvg::Point gCenter = transformPoint({gcx, gcy}, m);
                gcx = gCenter.x; gcy = gCenter.y;
                // Approximate radius scale (use x-scale)
                gr = gr * std::sqrt(m.e11 * m.e11 + m.e21 * m.e21);
                
                // Get color stops
                const tvg::Fill::ColorStop* stops = nullptr;
                uint32_t stopCount = gradientFill->colorStops(&stops);
                if (stopCount >= 2 && stops) {
                    gradColor1 = rgbaToPackedABGR(stops[0].r, stops[0].g, stops[0].b,
                        static_cast<uint8_t>((stops[0].a * paintOpacity) / 255));
                    gradColor2 = rgbaToPackedABGR(stops[stopCount-1].r, stops[stopCount-1].g, stops[stopCount-1].b,
                        static_cast<uint8_t>((stops[stopCount-1].a * paintOpacity) / 255));
                } else if (stopCount == 1 && stops) {
                    gradColor1 = gradColor2 = rgbaToPackedABGR(stops[0].r, stops[0].g, stops[0].b,
                        static_cast<uint8_t>((stops[0].a * paintOpacity) / 255));
                }
            }
        }
        
        // Get fill color (using getter overload) - for solid fills
        uint8_t fr = 0, fg = 0, fb = 0, fa = 0;
        shape->fill(&fr, &fg, &fb, &fa);
        // Apply paint opacity to fill alpha
        fa = static_cast<uint8_t>((static_cast<uint32_t>(fa) * paintOpacity) / 255);
        uint32_t fillColor = (fa > 0) ? rgbaToPackedABGR(fr, fg, fb, fa) : 0;
        
        // If we have a gradient, use it instead of solid fill
        if (hasLinearGradient || hasRadialGradient) {
            fillColor = gradColor1;  // For now, use first color as fallback
        }

        // Get stroke
        float strokeWidth = shape->strokeWidth();
        uint8_t sr = 0, sg = 0, sb = 0, sa = 0;
        shape->strokeFill(&sr, &sg, &sb, &sa);
        // Apply paint opacity to stroke alpha
        sa = static_cast<uint8_t>((static_cast<uint32_t>(sa) * paintOpacity) / 255);
        uint32_t strokeColor = (sa > 0 && strokeWidth > 0)
                               ? rgbaToPackedABGR(sr, sg, sb, sa) : 0;

        // Get stroke dash pattern
        const float* dashPattern = nullptr;
        float dashOffset = 0.0f;
        uint32_t dashCount = shape->strokeDash(&dashPattern, &dashOffset);

        // Skip invisible shapes
        if (fillColor == 0 && strokeColor == 0 && !hasLinearGradient && !hasRadialGradient) return;

        // Get path data using the combined path() method
        const tvg::PathCommand* cmds = nullptr;
        uint32_t cmdCount = 0;
        const tvg::Point* pts = nullptr;
        uint32_t ptCount = 0;

        if (shape->path(&cmds, &cmdCount, &pts, &ptCount) != tvg::Result::Success) {
            return;
        }

        if (cmdCount == 0 || ptCount == 0) return;

        // For gradient fills on boxes, try to detect and emit gradient box primitive
        if ((hasLinearGradient || hasRadialGradient) && (gradColor1 != 0 || gradColor2 != 0)) {
            if (tryRenderAsGradientBox(cmds, cmdCount, pts, ptCount, m,
                                       hasLinearGradient, gx1, gy1, gx2, gy2,
                                       gcx, gcy, gr, gradColor1, gradColor2,
                                       strokeColor, strokeWidth)) {
                return;
            }
        }

        // Convert path to YDraw primitives with full matrix transform
        renderPath(cmds, cmdCount, pts, ptCount, m, fillColor, strokeColor, strokeWidth,
                   dashPattern, dashCount, dashOffset);
    }

    // Try to render a gradient-filled box
    bool tryRenderAsGradientBox(const tvg::PathCommand* cmds, uint32_t cmdCount,
                                const tvg::Point* pts, uint32_t ptCount,
                                const tvg::Matrix& m,
                                bool hasLinearGradient,
                                float gx1, float gy1, float gx2, float gy2,
                                float gcx, float gcy, float gr,
                                uint32_t gradColor1, uint32_t gradColor2,
                                uint32_t strokeColor, float strokeWidth) {
        // Check for simple rectangle pattern: MoveTo + 3*LineTo + Close
        if (cmdCount != 5) {
            yinfo("tryRenderAsGradientBox: cmdCount={} (expected 5)", cmdCount);
            return false;
        }
        if (cmds[0] != tvg::PathCommand::MoveTo) return false;
        if (cmds[1] != tvg::PathCommand::LineTo) return false;
        if (cmds[2] != tvg::PathCommand::LineTo) return false;
        if (cmds[3] != tvg::PathCommand::LineTo) return false;
        if (cmds[4] != tvg::PathCommand::Close) return false;
        if (ptCount < 4) return false;
        
        // Transform all points
        tvg::Point p0 = transformPoint(pts[0], m);
        tvg::Point p1 = transformPoint(pts[1], m);
        tvg::Point p2 = transformPoint(pts[2], m);
        tvg::Point p3 = transformPoint(pts[3], m);
        
        // Find bounding box
        float minX = std::min({p0.x, p1.x, p2.x, p3.x});
        float maxX = std::max({p0.x, p1.x, p2.x, p3.x});
        float minY = std::min({p0.y, p1.y, p2.y, p3.y});
        float maxY = std::max({p0.y, p1.y, p2.y, p3.y});
        
        float cx = (minX + maxX) * 0.5f;
        float cy = (minY + maxY) * 0.5f;
        float hw = (maxX - minX) * 0.5f;
        float hh = (maxY - minY) * 0.5f;
        
        // Convert gradient coordinates from normalized (0-1) to world coordinates
        // if they look like percentage values (small range)
        float worldGx1 = gx1, worldGy1 = gy1, worldGx2 = gx2, worldGy2 = gy2;
        if (hasLinearGradient && std::abs(gx2 - gx1) <= 1.0f && std::abs(gy2 - gy1) <= 1.0f) {
            // Looks like normalized coordinates - scale to box
            worldGx1 = minX + gx1 * (maxX - minX);
            worldGy1 = minY + gy1 * (maxY - minY);
            worldGx2 = minX + gx2 * (maxX - minX);
            worldGy2 = minY + gy2 * (maxY - minY);
        }
        
        float worldGcx = gcx, worldGcy = gcy, worldGr = gr;
        if (!hasLinearGradient && std::abs(gcx) <= 1.0f && std::abs(gcy) <= 1.0f) {
            // Radial - scale center and radius to box
            worldGcx = minX + gcx * (maxX - minX);
            worldGcy = minY + gcy * (maxY - minY);
            worldGr = gr * std::max(maxX - minX, maxY - minY);
        }
        
        yinfo("tryRenderAsGradientBox: linear={} box=({},{}) {}x{} grad=({},{})→({},{}) colors=0x{:08x},0x{:08x}",
              hasLinearGradient, cx, cy, hw*2, hh*2, worldGx1, worldGy1, worldGx2, worldGy2, gradColor1, gradColor2);
        
        if (hasLinearGradient) {
            auto result = _buffer->addLinearGradientBox(
                0,              // layer
                cx, cy, hw, hh, // box params
                worldGx1, worldGy1, worldGx2, worldGy2, // gradient endpoints (world coords)
                gradColor1, gradColor2,
                strokeColor, strokeWidth,
                0.0f            // round
            );
            if (result) {
                _nextPrimId++;
                return true;
            }
        } else {
            // Radial gradient
            auto result = _buffer->addRadialGradientCircle(
                0,              // layer
                cx, cy,         // circle center
                std::max(hw, hh), // radius (use larger extent)
                worldGcx, worldGcy, worldGr,   // gradient center and radius (world coords)
                gradColor1, gradColor2,
                strokeColor, strokeWidth,
                0.0f            // round
            );
            if (result) {
                _nextPrimId++;
                return true;
            }
        }
        
        return false;
    }

    // Check if path is an ellipse (MoveTo + 4 CubicTo + Close pattern)
    bool tryRenderAsEllipse(const tvg::PathCommand* cmds, uint32_t cmdCount,
                            const tvg::Point* pts, uint32_t ptCount,
                            const tvg::Matrix& m,
                            uint32_t fillColor, uint32_t strokeColor, float strokeWidth) {
        // Ellipse pattern: MoveTo, CubicTo, CubicTo, CubicTo, CubicTo, Close
        if (cmdCount != 6) return false;
        if (cmds[0] != tvg::PathCommand::MoveTo) return false;
        if (cmds[1] != tvg::PathCommand::CubicTo) return false;
        if (cmds[2] != tvg::PathCommand::CubicTo) return false;
        if (cmds[3] != tvg::PathCommand::CubicTo) return false;
        if (cmds[4] != tvg::PathCommand::CubicTo) return false;
        if (cmds[5] != tvg::PathCommand::Close) return false;
        
        // Need 1 + 3*4 = 13 points (1 for MoveTo, 3 for each CubicTo)
        if (ptCount < 13) return false;
        
        // Transform all points and find bounding box
        float minX = 1e10f, minY = 1e10f, maxX = -1e10f, maxY = -1e10f;
        for (uint32_t i = 0; i < ptCount; ++i) {
            tvg::Point tp = transformPoint(pts[i], m);
            minX = std::min(minX, tp.x);
            minY = std::min(minY, tp.y);
            maxX = std::max(maxX, tp.x);
            maxY = std::max(maxY, tp.y);
        }
        
        float cx = (minX + maxX) / 2.0f;
        float cy = (minY + maxY) / 2.0f;
        float rx = (maxX - minX) / 2.0f;
        float ry = (maxY - minY) / 2.0f;
        
        yinfo("Ellipse detected: center=({},{}) radii=({},{}) fill=0x{:08X}", cx, cy, rx, ry, fillColor);
        
        auto result = _buffer->addEllipse(
            0,              // layer
            cx, cy,         // center
            rx, ry,         // radii
            fillColor,
            strokeColor,
            strokeWidth,
            0.0f            // round
        );
        if (result) _nextPrimId++;
        return true;
    }

    // Check if path is a rounded rectangle (MoveTo + alternating Line/CubicTo + Close)
    // Rounded rect: MoveTo, Line, Cubic, Line, Cubic, Line, Cubic, Line, Cubic, Close (10 cmds)
    // Or simple rect: MoveTo, Line, Line, Line, Line, Close (6 cmds with all LineTo)
    bool tryRenderAsBox(const tvg::PathCommand* cmds, uint32_t cmdCount,
                        const tvg::Point* pts, uint32_t ptCount,
                        const tvg::Matrix& m,
                        uint32_t fillColor, uint32_t strokeColor, float strokeWidth) {
        
        // Simple rectangle: MoveTo + 3 LineTo + Close (4 unique vertices)
        // Note: ThorVG may emit MoveTo + 4 LineTo + Close if it includes returning to start
        bool isSimpleRect4 = (cmdCount == 5 && ptCount == 4 &&
                              cmds[0] == tvg::PathCommand::MoveTo &&
                              cmds[1] == tvg::PathCommand::LineTo &&
                              cmds[2] == tvg::PathCommand::LineTo &&
                              cmds[3] == tvg::PathCommand::LineTo &&
                              cmds[4] == tvg::PathCommand::Close);
        
        bool isSimpleRect5 = (cmdCount == 6 && ptCount == 5 &&
                              cmds[0] == tvg::PathCommand::MoveTo &&
                              cmds[1] == tvg::PathCommand::LineTo &&
                              cmds[2] == tvg::PathCommand::LineTo &&
                              cmds[3] == tvg::PathCommand::LineTo &&
                              cmds[4] == tvg::PathCommand::LineTo &&
                              cmds[5] == tvg::PathCommand::Close);
        
        bool isSimpleRect = isSimpleRect4 || isSimpleRect5;
        
        // Rounded rectangle: MoveTo + (LineTo + CubicTo) * 4 + Close = 10 cmds
        bool isRoundedRect = (cmdCount == 10 &&
                              cmds[0] == tvg::PathCommand::MoveTo &&
                              cmds[1] == tvg::PathCommand::LineTo &&
                              cmds[2] == tvg::PathCommand::CubicTo &&
                              cmds[3] == tvg::PathCommand::LineTo &&
                              cmds[4] == tvg::PathCommand::CubicTo &&
                              cmds[5] == tvg::PathCommand::LineTo &&
                              cmds[6] == tvg::PathCommand::CubicTo &&
                              cmds[7] == tvg::PathCommand::LineTo &&
                              cmds[8] == tvg::PathCommand::CubicTo &&
                              cmds[9] == tvg::PathCommand::Close);
        
        if (!isSimpleRect && !isRoundedRect) return false;
        
        // Calculate corner radius for rounded rectangles
        // Rounded rect path: MoveTo(1pt), LineTo(1pt), CubicTo(3pts), repeated 4 times + Close
        // For cubic bezier approximating a quarter circle, control point is at ~0.552*r from curve start
        float cornerRadius = 0.0f;
        if (isRoundedRect && ptCount >= 5) {
            // First LineTo ends at pts[1], first CubicTo uses pts[2..4]
            // The corner radius is: distance from lineEnd to first control point / 0.552
            tvg::Point lineEnd = transformPoint(pts[1], m);
            tvg::Point bezierCP1 = transformPoint(pts[2], m);
            float dx = bezierCP1.x - lineEnd.x;
            float dy = bezierCP1.y - lineEnd.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            // Magic number for cubic bezier circle approximation: 4/3 * tan(pi/8) ≈ 0.5522847
            cornerRadius = dist / 0.5522847f;
        }
        
        // For simple rect, verify it's actually an axis-aligned rectangle
        // (has exactly 2 unique X values and 2 unique Y values)
        if (isSimpleRect) {
            // Transform points
            uint32_t checkCount = std::min(ptCount, 4u);
            float xVals[4], yVals[4];
            for (uint32_t i = 0; i < checkCount; ++i) {
                tvg::Point tp = transformPoint(pts[i], m);
                xVals[i] = tp.x;
                yVals[i] = tp.y;
            }
            
            // Sort and count unique values
            std::sort(xVals, xVals + checkCount);
            std::sort(yVals, yVals + checkCount);
            
            const float eps = 0.5f;
            bool twoUniqueX = (checkCount == 4 &&
                               std::abs(xVals[0] - xVals[1]) < eps && 
                               std::abs(xVals[2] - xVals[3]) < eps &&
                               std::abs(xVals[1] - xVals[2]) > eps);
            bool twoUniqueY = (checkCount == 4 &&
                               std::abs(yVals[0] - yVals[1]) < eps && 
                               std::abs(yVals[2] - yVals[3]) < eps &&
                               std::abs(yVals[1] - yVals[2]) > eps);
            
            if (!twoUniqueX || !twoUniqueY) {
                return false;  // Not an axis-aligned rectangle
            }
        }
        
        // Transform all points and find bounding box
        float minX = 1e10f, minY = 1e10f, maxX = -1e10f, maxY = -1e10f;
        for (uint32_t i = 0; i < ptCount; ++i) {
            tvg::Point tp = transformPoint(pts[i], m);
            minX = std::min(minX, tp.x);
            minY = std::min(minY, tp.y);
            maxX = std::max(maxX, tp.x);
            maxY = std::max(maxY, tp.y);
        }
        
        float cx = (minX + maxX) / 2.0f;
        float cy = (minY + maxY) / 2.0f;
        float hw = (maxX - minX) / 2.0f;  // half width
        float hh = (maxY - minY) / 2.0f;  // half height
        
        yinfo("Box detected: center=({},{}) halfSize=({},{}) round={} fill=0x{:08X}", cx, cy, hw, hh, cornerRadius, fillColor);
        
        auto result = _buffer->addBox(
            0,              // layer
            cx, cy,         // center
            hw, hh,         // half width, half height
            fillColor,
            strokeColor,
            strokeWidth,
            cornerRadius    // round
        );
        if (result) _nextPrimId++;
        return true;
    }

    // Try to render a closed path with fill as a polygon
    // Returns true if rendered as polygon, false otherwise
    bool tryRenderAsPolygon(const tvg::PathCommand* cmds, uint32_t cmdCount,
                            const tvg::Point* pts, uint32_t ptCount,
                            const tvg::Matrix& m,
                            uint32_t fillColor, uint32_t strokeColor, float strokeWidth) {
        // Only use polygon for filled shapes
        if ((fillColor & 0xFF000000) == 0) {
            return false;
        }

        // Check if path is closed and contains only MoveTo, LineTo, Close
        // (no curves - those need different handling)
        bool hasCurves = false;
        bool isClosed = false;
        int moveCount = 0;
        
        for (uint32_t i = 0; i < cmdCount; ++i) {
            switch (cmds[i]) {
                case tvg::PathCommand::MoveTo:
                    moveCount++;
                    break;
                case tvg::PathCommand::LineTo:
                    break;
                case tvg::PathCommand::CubicTo:
                    hasCurves = true;
                    break;
                case tvg::PathCommand::Close:
                    isClosed = true;
                    break;
            }
        }

        // Skip if has curves or multiple subpaths
        if (hasCurves || moveCount > 1) {
            return false;
        }

        if (!isClosed) {
            return false;
        }

        // Extract vertices
        std::vector<float> vertices;
        uint32_t ptIdx = 0;

        for (uint32_t i = 0; i < cmdCount && ptIdx < ptCount; ++i) {
            switch (cmds[i]) {
                case tvg::PathCommand::MoveTo:
                    if (ptIdx < ptCount) {
                        tvg::Point p = transformPoint(pts[ptIdx++], m);
                        vertices.push_back(p.x);
                        vertices.push_back(p.y);
                    }
                    break;
                case tvg::PathCommand::LineTo:
                    if (ptIdx < ptCount) {
                        tvg::Point p = transformPoint(pts[ptIdx++], m);
                        vertices.push_back(p.x);
                        vertices.push_back(p.y);
                    }
                    break;
                case tvg::PathCommand::Close:
                    // Don't add closing vertex - polygon is implicitly closed
                    break;
                default:
                    break;
            }
        }

        uint32_t vertexCount = static_cast<uint32_t>(vertices.size() / 2);
        if (vertexCount < 3) {
            return false;
        }

        auto result = _buffer->addPolygonWithVertices(
            0,              // layer
            vertexCount,
            vertices.data(),
            fillColor,
            strokeColor,
            strokeWidth,
            0.0f            // round
        );
        if (result) _nextPrimId++;
        return true;
    }

    // Render a dashed line segment by breaking it into visible dash segments
    void renderDashedSegment(float x0, float y0, float x1, float y1,
                             uint32_t strokeColor, float strokeWidth,
                             const float* dashPattern, uint32_t dashCount,
                             float& dashPos, uint32_t& dashIdx, bool& dashVisible) {
        float dx = x1 - x0;
        float dy = y1 - y0;
        float segLen = std::sqrt(dx * dx + dy * dy);
        if (segLen < 0.001f) return;
        
        float ux = dx / segLen;  // Unit vector
        float uy = dy / segLen;
        
        float pos = 0.0f;  // Position along this segment
        float curX = x0, curY = y0;
        
        while (pos < segLen) {
            float dashLen = dashPattern[dashIdx];
            float remaining = dashLen - dashPos;
            float advance = std::min(remaining, segLen - pos);
            
            float nextX = curX + ux * advance;
            float nextY = curY + uy * advance;
            
            if (dashVisible && advance > 0.001f) {
                // Draw this dash segment
                auto result = _buffer->addSegment(
                    0, curX, curY, nextX, nextY,
                    0,  // fillColor
                    strokeColor,
                    strokeWidth,
                    0.0f
                );
                if (result) _nextPrimId++;
            }
            
            pos += advance;
            dashPos += advance;
            curX = nextX;
            curY = nextY;
            
            // Move to next dash/gap if current one is complete
            if (dashPos >= dashLen - 0.001f) {
                dashPos = 0.0f;
                dashIdx = (dashIdx + 1) % dashCount;
                dashVisible = !dashVisible;
            }
        }
    }

    void renderPath(const tvg::PathCommand* cmds, uint32_t cmdCount,
                    const tvg::Point* pts, uint32_t ptCount,
                    const tvg::Matrix& m,
                    uint32_t fillColor, uint32_t strokeColor, float strokeWidth,
                    const float* dashPattern = nullptr, uint32_t dashCount = 0, float dashOffset = 0.0f) {
        
        // Check if we have a dash pattern - if so, skip shape shortcuts that don't support dashing
        bool hasDash = dashPattern != nullptr && dashCount >= 2;
        
        // Try to render as ellipse first (for Lottie circles) - skip if dashed stroke
        if (!hasDash && tryRenderAsEllipse(cmds, cmdCount, pts, ptCount, m, fillColor, strokeColor, strokeWidth)) {
            return;
        }
        
        // Try to render as box (for Lottie rectangles) - skip if dashed stroke
        if (!hasDash && tryRenderAsBox(cmds, cmdCount, pts, ptCount, m, fillColor, strokeColor, strokeWidth)) {
            return;
        }
        
        // Try to render as filled polygon (only for paths with fill and no bezier curves)
        if (tryRenderAsPolygon(cmds, cmdCount, pts, ptCount, m, fillColor, strokeColor, strokeWidth)) {
            return;
        }
        
        tvg::Point current = {0, 0};
        tvg::Point start = {0, 0};
        uint32_t ptIdx = 0;
        
        // For dash patterns, track position along path
        float dashPos = dashOffset;  // Current position in dash pattern
        uint32_t dashIdx = 0;        // Current dash/gap index
        bool dashVisible = true;     // Whether we're in a dash (vs gap)

        for (uint32_t i = 0; i < cmdCount && ptIdx < ptCount; ++i) {
            switch (cmds[i]) {
                case tvg::PathCommand::MoveTo:
                    if (ptIdx < ptCount) {
                        current = transformPoint(pts[ptIdx++], m);
                        start = current;
                    }
                    break;

                case tvg::PathCommand::LineTo:
                    if (ptIdx < ptCount) {
                        tvg::Point next = transformPoint(pts[ptIdx++], m);
                        
                        if (hasDash && strokeColor != 0) {
                            // Render dashed line
                            renderDashedSegment(current.x, current.y, next.x, next.y,
                                               strokeColor, strokeWidth,
                                               dashPattern, dashCount,
                                               dashPos, dashIdx, dashVisible);
                        } else {
                            // Render solid line
                            auto result = _buffer->addSegment(
                                0,                    // layer
                                current.x, current.y, // start
                                next.x, next.y,       // end
                                fillColor,
                                strokeColor,
                                strokeWidth,
                                0.0f                  // round
                            );
                            if (result) _nextPrimId++;
                        }
                        current = next;
                    }
                    break;

                case tvg::PathCommand::CubicTo:
                    if (ptIdx + 2 < ptCount) {
                        tvg::Point cp1 = transformPoint(pts[ptIdx++], m);
                        tvg::Point cp2 = transformPoint(pts[ptIdx++], m);
                        tvg::Point end = transformPoint(pts[ptIdx++], m);
                        
                        ydebug("Bezier3: ({},{}) -> ({},{}) fill=0x{:08X}", current.x, current.y, end.x, end.y, fillColor);
                        
                        auto result = _buffer->addBezier3(
                            0,                    // layer
                            current.x, current.y, // start
                            cp1.x, cp1.y,         // control 1
                            cp2.x, cp2.y,         // control 2
                            end.x, end.y,         // end
                            fillColor,
                            strokeColor,
                            strokeWidth,
                            0.0f                  // round
                        );
                        if (result) _nextPrimId++;
                        current = end;
                    }
                    break;

                case tvg::PathCommand::Close:
                    if (current.x != start.x || current.y != start.y) {
                        auto result = _buffer->addSegment(
                            0,
                            current.x, current.y,
                            start.x, start.y,
                            fillColor,
                            strokeColor,
                            strokeWidth,
                            0.0f
                        );
                        if (result) _nextPrimId++;
                    }
                    current = start;
                    break;
            }
        }
    }

    //=========================================================================
    // Members
    //=========================================================================

    std::shared_ptr<YDrawBuffer> _buffer;
    std::unique_ptr<tvg::Animation> _animation;
    tvg::Picture* _picture = nullptr;
    std::stack<tvg::Matrix> _transformStack;

    float _contentWidth = 0;
    float _contentHeight = 0;
    bool _isAnimated = false;
    float _totalFrames = 0;
    float _duration = 0;
    float _currentFrame = 0;

    uint32_t _nextPrimId = 0;
};

//=============================================================================
// Factory
//=============================================================================

Result<ThorVgRenderer::Ptr> ThorVgRenderer::createImpl(
    std::shared_ptr<YDrawBuffer> buffer)
{
    if (!buffer) {
        return Err<Ptr>("ThorVgRenderer: null buffer");
    }

    // Initialize ThorVG engine (ref-counted)
    if (auto res = thorvgInit(); !res) {
        return Err<Ptr>("ThorVgRenderer: engine init failed", res);
    }

    return Ok<Ptr>(std::make_shared<ThorVgRendererImpl>(std::move(buffer)));
}

} // namespace yetty::thorvg
