#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "ydraw-render-method.h"
#include "../ydraw/ydraw-buffer.h"
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cmath>
#include <cstring>

namespace yetty::ythorvg {

//=============================================================================
// Constructor/Destructor
//=============================================================================

YDrawRenderMethod::YDrawRenderMethod(std::shared_ptr<YDrawBuffer> buffer)
    : _buffer(std::move(buffer)) {}

YDrawRenderMethod::~YDrawRenderMethod() {
    for (auto* rd : _renderDataList) {
        delete rd;
    }
    _renderDataList.clear();
}

void YDrawRenderMethod::setTarget(uint32_t width, uint32_t height) {
    _width = width;
    _height = height;
}

//=============================================================================
// Lifecycle methods
//=============================================================================

bool YDrawRenderMethod::preUpdate() {
    return true;
}

bool YDrawRenderMethod::postUpdate() {
    return true;
}

bool YDrawRenderMethod::preRender() {
    _buffer->clear();
    _nextPrimId = 0;
    return true;
}

bool YDrawRenderMethod::postRender() {
    // YDrawBuilder::calculate() is called by the card, not here
    return true;
}

bool YDrawRenderMethod::clear() {
    _buffer->clear();
    _nextPrimId = 0;
    return true;
}

bool YDrawRenderMethod::sync() {
    return true;
}

//=============================================================================
// Shape preparation
//=============================================================================

tvg::RenderData YDrawRenderMethod::prepare(
    const tvg::RenderShape& rshape, tvg::RenderData data,
    const tvg::Matrix& transform, tvg::Array<tvg::RenderData>& clips,
    uint8_t opacity, tvg::RenderUpdateFlag flags, bool clipper)
{
    (void)clips;
    (void)flags;
    (void)clipper;

    // Reuse or create RenderData
    auto* rd = static_cast<YDrawRenderData*>(data);
    if (!rd) {
        rd = new YDrawRenderData();
        _renderDataList.push_back(rd);
    }

    rd->type = YDrawRenderData::Type::Shape;
    rd->valid = true;

    // Copy path data
    rd->cmds.clear();
    rd->pts.clear();
    for (uint32_t i = 0; i < rshape.path.cmds.count; ++i) {
        rd->cmds.push_back(rshape.path.cmds[i]);
    }
    for (uint32_t i = 0; i < rshape.path.pts.count; ++i) {
        rd->pts.push_back(rshape.path.pts[i]);
    }

    // Store transform (already world space!)
    rd->transform = transform;

    // Store opacity (already accumulated!)
    rd->opacity = opacity;

    // Fill color
    rshape.fillColor(&rd->fillR, &rd->fillG, &rd->fillB, &rd->fillA);
    rd->fill = rshape.fill;
    rd->rule = rshape.rule;

    // Stroke
    if (rshape.stroke) {
        rd->strokeWidth = rshape.stroke->width;
        rd->strokeR = rshape.stroke->color.r;
        rd->strokeG = rshape.stroke->color.g;
        rd->strokeB = rshape.stroke->color.b;
        rd->strokeA = rshape.stroke->color.a;
        rd->strokeFill = rshape.stroke->fill;
        rd->strokeCap = rshape.stroke->cap;
        rd->strokeJoin = rshape.stroke->join;

        // Dash pattern
        rd->dashPattern.clear();
        if (rshape.stroke->dash.pattern && rshape.stroke->dash.count > 0) {
            for (uint32_t i = 0; i < rshape.stroke->dash.count; ++i) {
                rd->dashPattern.push_back(rshape.stroke->dash.pattern[i]);
            }
            rd->dashOffset = rshape.stroke->dash.offset;
        }
    } else {
        rd->strokeWidth = 0.0f;
        rd->strokeR = rd->strokeG = rd->strokeB = rd->strokeA = 0;
        rd->strokeFill = nullptr;
        rd->dashPattern.clear();
        rd->dashOffset = 0.0f;
    }

    // Compute bounds
    if (!rd->pts.empty()) {
        float minX = 1e10f, minY = 1e10f, maxX = -1e10f, maxY = -1e10f;
        for (const auto& pt : rd->pts) {
            auto tp = transformPoint(pt, rd->transform);
            minX = std::min(minX, tp.x);
            minY = std::min(minY, tp.y);
            maxX = std::max(maxX, tp.x);
            maxY = std::max(maxY, tp.y);
        }
        rd->bounds = {{static_cast<int32_t>(minX), static_cast<int32_t>(minY)},
                      {static_cast<int32_t>(maxX + 1), static_cast<int32_t>(maxY + 1)}};
    }

    return rd;
}

tvg::RenderData YDrawRenderMethod::prepare(
    tvg::RenderSurface* surface, tvg::RenderData data,
    const tvg::Matrix& transform, tvg::Array<tvg::RenderData>& clips,
    uint8_t opacity, tvg::RenderUpdateFlag flags)
{
    (void)clips;
    (void)flags;

    auto* rd = static_cast<YDrawRenderData*>(data);
    if (!rd) {
        rd = new YDrawRenderData();
        _renderDataList.push_back(rd);
    }

    rd->type = YDrawRenderData::Type::Image;
    rd->valid = true;
    rd->surface = surface;
    rd->transform = transform;
    rd->opacity = opacity;

    if (surface) {
        rd->bounds = {{0, 0},
                      {static_cast<int32_t>(surface->w), static_cast<int32_t>(surface->h)}};
    }

    return rd;
}

//=============================================================================
// Rendering
//=============================================================================

bool YDrawRenderMethod::renderShape(tvg::RenderData data) {
    auto* rd = static_cast<YDrawRenderData*>(data);
    if (!rd || !rd->valid || rd->type != YDrawRenderData::Type::Shape) {
        return false;
    }

    // Check for gradient fill first
    bool hasGradient = (rd->fill != nullptr);

    // Apply opacity to colors
    uint8_t fillA = static_cast<uint8_t>((rd->fillA * rd->opacity) / 255);
    uint8_t strokeA = static_cast<uint8_t>((rd->strokeA * rd->opacity) / 255);

    uint32_t fillColor = (fillA > 0) ? rgbaToPackedABGR(rd->fillR, rd->fillG, rd->fillB, fillA) : 0;
    uint32_t strokeColor = (strokeA > 0 && rd->strokeWidth > 0)
                               ? rgbaToPackedABGR(rd->strokeR, rd->strokeG, rd->strokeB, strokeA) : 0;

    // Skip invisible shapes (but gradient shapes may have fillA=0)
    if (fillColor == 0 && strokeColor == 0 && !hasGradient) {
        return true;
    }

    // Try gradient box first if we have a gradient
    if (hasGradient && tryRenderAsGradientBox(rd)) return true;

    // Try shape detection patterns (from existing thorvg-renderer)
    if (tryRenderAsEllipse(rd)) return true;
    if (tryRenderAsBox(rd)) return true;
    if (tryRenderAsPolygon(rd)) return true;

    // Fall back to path decomposition
    renderPath(rd);
    return true;
}

bool YDrawRenderMethod::renderImage(tvg::RenderData data) {
    auto* rd = static_cast<YDrawRenderData*>(data);
    if (!rd || !rd->valid || rd->type != YDrawRenderData::Type::Image) {
        return false;
    }

    // TODO: Implement image rendering via YDraw Image primitive
    ywarn("YDrawRenderMethod::renderImage: not yet implemented");
    return true;
}

//=============================================================================
// Disposal
//=============================================================================

void YDrawRenderMethod::dispose(tvg::RenderData data) {
    auto* rd = static_cast<YDrawRenderData*>(data);
    if (!rd) return;

    // Remove from tracking list
    auto it = std::find(_renderDataList.begin(), _renderDataList.end(), rd);
    if (it != _renderDataList.end()) {
        _renderDataList.erase(it);
    }
    delete rd;
}

//=============================================================================
// Region/bounds
//=============================================================================

tvg::RenderRegion YDrawRenderMethod::region(tvg::RenderData data) {
    auto* rd = static_cast<YDrawRenderData*>(data);
    if (!rd) return {{0, 0}, {0, 0}};
    return rd->bounds;
}

bool YDrawRenderMethod::bounds(tvg::RenderData data, tvg::Point* pt4, const tvg::Matrix& m) {
    auto* rd = static_cast<YDrawRenderData*>(data);
    if (!rd || !pt4) return false;

    auto& b = rd->bounds;
    pt4[0] = {static_cast<float>(b.min.x), static_cast<float>(b.min.y)};
    pt4[1] = {static_cast<float>(b.max.x), static_cast<float>(b.min.y)};
    pt4[2] = {static_cast<float>(b.max.x), static_cast<float>(b.max.y)};
    pt4[3] = {static_cast<float>(b.min.x), static_cast<float>(b.max.y)};

    // Transform by m if needed
    for (int i = 0; i < 4; ++i) {
        pt4[i] = transformPoint(pt4[i], m);
    }
    return true;
}

bool YDrawRenderMethod::intersectsShape(tvg::RenderData data, const tvg::RenderRegion& region) {
    auto* rd = static_cast<YDrawRenderData*>(data);
    if (!rd) return false;
    return rd->bounds.intersected(region);
}

bool YDrawRenderMethod::intersectsImage(tvg::RenderData data, const tvg::RenderRegion& region) {
    return intersectsShape(data, region);
}

//=============================================================================
// Compositing (stubs)
//=============================================================================

tvg::RenderCompositor* YDrawRenderMethod::target(
    const tvg::RenderRegion& region, tvg::ColorSpace cs, tvg::CompositionFlag flags)
{
    (void)region;
    (void)cs;
    (void)flags;
    return nullptr;  // No compositing support yet
}

bool YDrawRenderMethod::beginComposite(tvg::RenderCompositor* cmp, tvg::MaskMethod method, uint8_t opacity) {
    (void)cmp;
    (void)method;
    (void)opacity;
    return true;
}

bool YDrawRenderMethod::endComposite(tvg::RenderCompositor* cmp) {
    (void)cmp;
    return true;
}

//=============================================================================
// Effects (stubs)
//=============================================================================

void YDrawRenderMethod::prepare(tvg::RenderEffect* effect, const tvg::Matrix& transform) {
    (void)effect;
    (void)transform;
}

bool YDrawRenderMethod::region(tvg::RenderEffect* effect) {
    (void)effect;
    return false;
}

bool YDrawRenderMethod::render(tvg::RenderCompositor* cmp, const tvg::RenderEffect* effect, bool direct) {
    (void)cmp;
    (void)effect;
    (void)direct;
    return true;
}

void YDrawRenderMethod::dispose(tvg::RenderEffect* effect) {
    (void)effect;
}

//=============================================================================
// Misc
//=============================================================================

bool YDrawRenderMethod::blend(tvg::BlendMethod method) {
    _blendMethod = method;
    return true;
}

tvg::ColorSpace YDrawRenderMethod::colorSpace() {
    return tvg::ColorSpace::ABGR8888;
}

const tvg::RenderSurface* YDrawRenderMethod::mainSurface() {
    return nullptr;
}

void YDrawRenderMethod::damage(tvg::RenderData rd, const tvg::RenderRegion& region) {
    (void)rd;
    (void)region;
}

bool YDrawRenderMethod::partial(bool disable) {
    (void)disable;
    return false;
}

//=============================================================================
// Helper methods
//=============================================================================

tvg::Point YDrawRenderMethod::transformPoint(const tvg::Point& p, const tvg::Matrix& m) {
    return {
        m.e11 * p.x + m.e12 * p.y + m.e13,
        m.e21 * p.x + m.e22 * p.y + m.e23
    };
}

uint32_t YDrawRenderMethod::rgbaToPackedABGR(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (static_cast<uint32_t>(a) << 24) |
           (static_cast<uint32_t>(b) << 16) |
           (static_cast<uint32_t>(g) << 8) |
           static_cast<uint32_t>(r);
}

//=============================================================================
// Shape detection (ported from thorvg-renderer.cpp)
//=============================================================================

bool YDrawRenderMethod::tryRenderAsEllipse(YDrawRenderData* rd) {
    const auto& cmds = rd->cmds;
    const auto& pts = rd->pts;
    const auto& m = rd->transform;

    // Ellipse pattern: MoveTo, CubicTo, CubicTo, CubicTo, CubicTo, Close
    if (cmds.size() != 6) return false;
    if (cmds[0] != tvg::PathCommand::MoveTo) return false;
    if (cmds[1] != tvg::PathCommand::CubicTo) return false;
    if (cmds[2] != tvg::PathCommand::CubicTo) return false;
    if (cmds[3] != tvg::PathCommand::CubicTo) return false;
    if (cmds[4] != tvg::PathCommand::CubicTo) return false;
    if (cmds[5] != tvg::PathCommand::Close) return false;

    if (pts.size() < 13) return false;

    // Transform and find bounding box
    float minX = 1e10f, minY = 1e10f, maxX = -1e10f, maxY = -1e10f;
    for (const auto& pt : pts) {
        auto tp = transformPoint(pt, m);
        minX = std::min(minX, tp.x);
        minY = std::min(minY, tp.y);
        maxX = std::max(maxX, tp.x);
        maxY = std::max(maxY, tp.y);
    }

    float cx = (minX + maxX) / 2.0f;
    float cy = (minY + maxY) / 2.0f;
    float rx = (maxX - minX) / 2.0f;
    float ry = (maxY - minY) / 2.0f;

    // Compute colors with opacity
    uint8_t fillA = static_cast<uint8_t>((rd->fillA * rd->opacity) / 255);
    uint8_t strokeA = static_cast<uint8_t>((rd->strokeA * rd->opacity) / 255);
    uint32_t fillColor = (fillA > 0) ? rgbaToPackedABGR(rd->fillR, rd->fillG, rd->fillB, fillA) : 0;
    uint32_t strokeColor = (strokeA > 0) ? rgbaToPackedABGR(rd->strokeR, rd->strokeG, rd->strokeB, strokeA) : 0;

    auto result = _buffer->addEllipse(0, cx, cy, rx, ry, fillColor, strokeColor, rd->strokeWidth, 0.0f);
    if (result) _nextPrimId++;
    return true;
}

bool YDrawRenderMethod::tryRenderAsBox(YDrawRenderData* rd) {
    const auto& cmds = rd->cmds;
    const auto& pts = rd->pts;
    const auto& m = rd->transform;

    // Simple rectangle: MoveTo + 3 LineTo + Close
    bool isSimpleRect4 = (cmds.size() == 5 && pts.size() == 4 &&
                          cmds[0] == tvg::PathCommand::MoveTo &&
                          cmds[1] == tvg::PathCommand::LineTo &&
                          cmds[2] == tvg::PathCommand::LineTo &&
                          cmds[3] == tvg::PathCommand::LineTo &&
                          cmds[4] == tvg::PathCommand::Close);

    bool isSimpleRect5 = (cmds.size() == 6 && pts.size() == 5 &&
                          cmds[0] == tvg::PathCommand::MoveTo &&
                          cmds[1] == tvg::PathCommand::LineTo &&
                          cmds[2] == tvg::PathCommand::LineTo &&
                          cmds[3] == tvg::PathCommand::LineTo &&
                          cmds[4] == tvg::PathCommand::LineTo &&
                          cmds[5] == tvg::PathCommand::Close);

    bool isSimpleRect = isSimpleRect4 || isSimpleRect5;

    // Rounded rectangle: MoveTo + (LineTo + CubicTo) * 4 + Close = 10 cmds
    bool isRoundedRect = (cmds.size() == 10 &&
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

    // Calculate corner radius
    float cornerRadius = 0.0f;
    if (isRoundedRect && pts.size() >= 5) {
        auto lineEnd = transformPoint(pts[1], m);
        auto bezierCP1 = transformPoint(pts[2], m);
        float dx = bezierCP1.x - lineEnd.x;
        float dy = bezierCP1.y - lineEnd.y;
        float dist = std::sqrt(dx * dx + dy * dy);
        cornerRadius = dist / 0.5522847f;
    }

    // Verify axis-aligned for simple rect
    if (isSimpleRect) {
        uint32_t checkCount = std::min(static_cast<uint32_t>(pts.size()), 4u);
        std::vector<float> xVals(checkCount), yVals(checkCount);
        for (uint32_t i = 0; i < checkCount; ++i) {
            auto tp = transformPoint(pts[i], m);
            xVals[i] = tp.x;
            yVals[i] = tp.y;
        }
        std::sort(xVals.begin(), xVals.end());
        std::sort(yVals.begin(), yVals.end());

        const float eps = 0.5f;
        bool twoUniqueX = (checkCount == 4 &&
                           std::abs(xVals[0] - xVals[1]) < eps &&
                           std::abs(xVals[2] - xVals[3]) < eps &&
                           std::abs(xVals[1] - xVals[2]) > eps);
        bool twoUniqueY = (checkCount == 4 &&
                           std::abs(yVals[0] - yVals[1]) < eps &&
                           std::abs(yVals[2] - yVals[3]) < eps &&
                           std::abs(yVals[1] - yVals[2]) > eps);
        if (!twoUniqueX || !twoUniqueY) return false;
    }

    // Find bounding box
    float minX = 1e10f, minY = 1e10f, maxX = -1e10f, maxY = -1e10f;
    for (const auto& pt : pts) {
        auto tp = transformPoint(pt, m);
        minX = std::min(minX, tp.x);
        minY = std::min(minY, tp.y);
        maxX = std::max(maxX, tp.x);
        maxY = std::max(maxY, tp.y);
    }

    float cx = (minX + maxX) / 2.0f;
    float cy = (minY + maxY) / 2.0f;
    float hw = (maxX - minX) / 2.0f;
    float hh = (maxY - minY) / 2.0f;

    uint8_t fillA = static_cast<uint8_t>((rd->fillA * rd->opacity) / 255);
    uint8_t strokeA = static_cast<uint8_t>((rd->strokeA * rd->opacity) / 255);
    uint32_t fillColor = (fillA > 0) ? rgbaToPackedABGR(rd->fillR, rd->fillG, rd->fillB, fillA) : 0;
    uint32_t strokeColor = (strokeA > 0) ? rgbaToPackedABGR(rd->strokeR, rd->strokeG, rd->strokeB, strokeA) : 0;

    auto result = _buffer->addBox(0, cx, cy, hw, hh, fillColor, strokeColor, rd->strokeWidth, cornerRadius);
    if (result) _nextPrimId++;
    return true;
}

bool YDrawRenderMethod::tryRenderAsPolygon(YDrawRenderData* rd) {
    const auto& cmds = rd->cmds;
    const auto& pts = rd->pts;
    const auto& m = rd->transform;

    uint8_t fillA = static_cast<uint8_t>((rd->fillA * rd->opacity) / 255);
    if (fillA == 0) return false;

    // Check for simple polygon (no curves, single contour, closed)
    bool hasCurves = false;
    bool isClosed = false;
    int moveCount = 0;

    for (const auto& cmd : cmds) {
        if (cmd == tvg::PathCommand::MoveTo) moveCount++;
        else if (cmd == tvg::PathCommand::CubicTo) hasCurves = true;
        else if (cmd == tvg::PathCommand::Close) isClosed = true;
    }

    if (hasCurves || moveCount > 1 || !isClosed) return false;

    // Extract vertices
    std::vector<float> vertices;
    uint32_t ptIdx = 0;

    for (const auto& cmd : cmds) {
        if (ptIdx >= pts.size()) break;
        if (cmd == tvg::PathCommand::MoveTo || cmd == tvg::PathCommand::LineTo) {
            auto p = transformPoint(pts[ptIdx++], m);
            vertices.push_back(p.x);
            vertices.push_back(p.y);
        }
    }

    uint32_t vertexCount = static_cast<uint32_t>(vertices.size() / 2);
    if (vertexCount < 3) return false;

    uint8_t strokeA = static_cast<uint8_t>((rd->strokeA * rd->opacity) / 255);
    uint32_t fillColor = rgbaToPackedABGR(rd->fillR, rd->fillG, rd->fillB, fillA);
    uint32_t strokeColor = (strokeA > 0) ? rgbaToPackedABGR(rd->strokeR, rd->strokeG, rd->strokeB, strokeA) : 0;

    auto result = _buffer->addPolygonWithVertices(0, vertexCount, vertices.data(),
                                                   fillColor, strokeColor, rd->strokeWidth, 0.0f);
    if (result) _nextPrimId++;
    return true;
}

void YDrawRenderMethod::renderPath(YDrawRenderData* rd) {
    const auto& cmds = rd->cmds;
    const auto& pts = rd->pts;
    const auto& m = rd->transform;

    uint8_t strokeA = static_cast<uint8_t>((rd->strokeA * rd->opacity) / 255);
    uint32_t strokeColor = (strokeA > 0 && rd->strokeWidth > 0)
                               ? rgbaToPackedABGR(rd->strokeR, rd->strokeG, rd->strokeB, strokeA) : 0;

    if (strokeColor == 0) return;

    bool hasDash = !rd->dashPattern.empty() && rd->dashPattern.size() >= 2;
    float dashPos = rd->dashOffset;
    uint32_t dashIdx = 0;
    bool dashVisible = true;

    tvg::Point current{0, 0};
    uint32_t ptIdx = 0;

    for (const auto& cmd : cmds) {
        if (ptIdx >= pts.size()) break;

        switch (cmd) {
            case tvg::PathCommand::MoveTo:
                current = transformPoint(pts[ptIdx++], m);
                // Reset dash state on MoveTo
                dashPos = rd->dashOffset;
                dashIdx = 0;
                dashVisible = true;
                break;

            case tvg::PathCommand::LineTo: {
                auto next = transformPoint(pts[ptIdx++], m);
                if (hasDash) {
                    renderDashedSegment(current.x, current.y, next.x, next.y,
                                        strokeColor, rd->strokeWidth, rd->dashPattern,
                                        dashPos, dashIdx, dashVisible);
                } else {
                    _buffer->addSegment(0, current.x, current.y, next.x, next.y,
                                        0, strokeColor, rd->strokeWidth, 0.0f);
                    _nextPrimId++;
                }
                current = next;
                break;
            }

            case tvg::PathCommand::CubicTo: {
                if (ptIdx + 2 >= pts.size()) break;
                auto cp1 = transformPoint(pts[ptIdx++], m);
                auto cp2 = transformPoint(pts[ptIdx++], m);
                auto end = transformPoint(pts[ptIdx++], m);
                // TODO: Dashed beziers need subdivision - for now just draw solid
                _buffer->addBezier3(0, current.x, current.y, cp1.x, cp1.y,
                                    cp2.x, cp2.y, end.x, end.y,
                                    0, strokeColor, rd->strokeWidth, 0.0f);
                _nextPrimId++;
                current = end;
                break;
            }

            case tvg::PathCommand::Close:
                break;
        }
    }
}

void YDrawRenderMethod::renderDashedSegment(float x0, float y0, float x1, float y1,
                                             uint32_t strokeColor, float strokeWidth,
                                             const std::vector<float>& dashPattern,
                                             float& dashPos, uint32_t& dashIdx, bool& dashVisible)
{
    float dx = x1 - x0;
    float dy = y1 - y0;
    float segLen = std::sqrt(dx * dx + dy * dy);
    if (segLen < 0.001f) return;

    float ux = dx / segLen;
    float uy = dy / segLen;
    float pos = 0.0f;

    while (pos < segLen) {
        float dashLen = dashPattern[dashIdx];
        float remaining = dashLen - dashPos;
        float advance = std::min(remaining, segLen - pos);

        if (advance <= 0.001f) {
            dashPos = 0.0f;
            dashIdx = (dashIdx + 1) % dashPattern.size();
            dashVisible = !dashVisible;
            continue;
        }

        if (dashVisible && advance > 0.001f) {
            float sx = x0 + ux * pos;
            float sy = y0 + uy * pos;
            float ex = x0 + ux * (pos + advance);
            float ey = y0 + uy * (pos + advance);
            _buffer->addSegment(0, sx, sy, ex, ey, 0, strokeColor, strokeWidth, 0.0f);
            _nextPrimId++;
        }

        pos += advance;
        dashPos += advance;

        if (dashPos >= dashLen - 0.001f) {
            dashPos = 0.0f;
            dashIdx = (dashIdx + 1) % dashPattern.size();
            dashVisible = !dashVisible;
        }
    }
}

//=============================================================================
// Gradient helpers
//=============================================================================

bool YDrawRenderMethod::extractGradientInfo(YDrawRenderData* rd, bool& isLinear,
                                            float& gx1, float& gy1, float& gx2, float& gy2,
                                            float& gcx, float& gcy, float& gr,
                                            uint32_t& color1, uint32_t& color2)
{
    if (!rd->fill) return false;

    auto fillType = rd->fill->type();
    const tvg::Fill::ColorStop* stops = nullptr;
    uint32_t stopCount = rd->fill->colorStops(&stops);

    if (stopCount < 2 || !stops) return false;

    // Get first and last stop colors
    const auto& c1 = stops[0];
    const auto& c2 = stops[stopCount - 1];

    uint8_t a1 = static_cast<uint8_t>((c1.a * rd->opacity) / 255);
    uint8_t a2 = static_cast<uint8_t>((c2.a * rd->opacity) / 255);

    color1 = rgbaToPackedABGR(c1.r, c1.g, c1.b, a1);
    color2 = rgbaToPackedABGR(c2.r, c2.g, c2.b, a2);

    if (fillType == tvg::Type::LinearGradient) {
        isLinear = true;
        auto* lg = static_cast<tvg::LinearGradient*>(rd->fill);
        lg->linear(&gx1, &gy1, &gx2, &gy2);
        return true;
    } else if (fillType == tvg::Type::RadialGradient) {
        isLinear = false;
        auto* rg = static_cast<tvg::RadialGradient*>(rd->fill);
        rg->radial(&gcx, &gcy, &gr);
        return true;
    }

    return false;
}

bool YDrawRenderMethod::tryRenderAsGradientBox(YDrawRenderData* rd) {
    const auto& cmds = rd->cmds;
    const auto& pts = rd->pts;
    const auto& m = rd->transform;

    // Must have gradient fill
    if (!rd->fill) return false;

    // Check for simple rect pattern (5 commands: MoveTo + 3/4 LineTo + Close)
    if (cmds.size() < 4 || cmds.size() > 6) return false;
    if (cmds[0] != tvg::PathCommand::MoveTo) return false;
    if (cmds.back() != tvg::PathCommand::Close) return false;

    // All middle commands must be LineTo
    for (size_t i = 1; i < cmds.size() - 1; ++i) {
        if (cmds[i] != tvg::PathCommand::LineTo) return false;
    }

    // Need at least 4 points for a rectangle
    if (pts.size() < 4) return false;

    // Extract gradient info
    bool isLinear = false;
    float gx1 = 0, gy1 = 0, gx2 = 0, gy2 = 0;
    float gcx = 0, gcy = 0, gr = 0;
    uint32_t gradColor1 = 0, gradColor2 = 0;

    if (!extractGradientInfo(rd, isLinear, gx1, gy1, gx2, gy2, gcx, gcy, gr, gradColor1, gradColor2)) {
        return false;
    }

    // Calculate bounding box
    float minX = 1e10f, minY = 1e10f, maxX = -1e10f, maxY = -1e10f;
    for (size_t i = 0; i < std::min(pts.size(), size_t(4)); ++i) {
        auto tp = transformPoint(pts[i], m);
        minX = std::min(minX, tp.x);
        minY = std::min(minY, tp.y);
        maxX = std::max(maxX, tp.x);
        maxY = std::max(maxY, tp.y);
    }

    float cx = (minX + maxX) / 2.0f;
    float cy = (minY + maxY) / 2.0f;
    float hw = (maxX - minX) / 2.0f;
    float hh = (maxY - minY) / 2.0f;

    // Transform gradient coordinates
    // If gradient coords are normalized (0-1), convert to world coords
    float worldGx1 = gx1, worldGy1 = gy1, worldGx2 = gx2, worldGy2 = gy2;
    float worldGcx = gcx, worldGcy = gcy, worldGr = gr;

    if (isLinear && std::abs(gx2 - gx1) <= 1.0f && std::abs(gy2 - gy1) <= 1.0f) {
        // Normalized coords - convert to world
        worldGx1 = minX + gx1 * (maxX - minX);
        worldGy1 = minY + gy1 * (maxY - minY);
        worldGx2 = minX + gx2 * (maxX - minX);
        worldGy2 = minY + gy2 * (maxY - minY);
    } else {
        // Transform gradient endpoints
        tvg::Point p1{gx1, gy1}, p2{gx2, gy2};
        p1 = transformPoint(p1, m);
        p2 = transformPoint(p2, m);
        worldGx1 = p1.x; worldGy1 = p1.y;
        worldGx2 = p2.x; worldGy2 = p2.y;
    }

    if (!isLinear && std::abs(gcx) <= 1.0f && std::abs(gcy) <= 1.0f) {
        // Normalized coords
        worldGcx = minX + gcx * (maxX - minX);
        worldGcy = minY + gcy * (maxY - minY);
        worldGr = gr * std::max(maxX - minX, maxY - minY);
    } else {
        // Transform center
        tvg::Point pc{gcx, gcy};
        pc = transformPoint(pc, m);
        worldGcx = pc.x; worldGcy = pc.y;
        float scaleX = std::sqrt(m.e11 * m.e11 + m.e21 * m.e21);
        worldGr = gr * scaleX;
    }

    uint8_t strokeA = static_cast<uint8_t>((rd->strokeA * rd->opacity) / 255);
    uint32_t strokeColor = (strokeA > 0) ? rgbaToPackedABGR(rd->strokeR, rd->strokeG, rd->strokeB, strokeA) : 0;

    if (isLinear) {
        auto result = _buffer->addLinearGradientBox(
            0, cx, cy, hw, hh,
            worldGx1, worldGy1, worldGx2, worldGy2,
            gradColor1, gradColor2,
            strokeColor, rd->strokeWidth, 0.0f);
        if (result) _nextPrimId++;
    } else {
        auto result = _buffer->addRadialGradientCircle(
            0, cx, cy, std::max(hw, hh),
            worldGcx, worldGcy, worldGr,
            gradColor1, gradColor2,
            strokeColor, rd->strokeWidth, 0.0f);
        if (result) _nextPrimId++;
    }

    return true;
}

} // namespace yetty::ythorvg
