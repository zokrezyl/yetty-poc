#include "ydraw-base.h"
#include "animation.h"
#include "../cards/hdraw/hdraw.h"  // For SDFPrimitive, SDFType
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <iostream>

namespace {
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;
}

namespace yetty {

using card::SDFPrimitive;
using card::SDFType;

//=============================================================================
// AABB utilities
//=============================================================================

static void computeAABB(SDFPrimitive& prim) {
    float expand = prim.strokeWidth * 0.5f;

    switch (static_cast<SDFType>(prim.type)) {
        case SDFType::Circle: {
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Box: {
            float hw = prim.params[2] + prim.round + expand;
            float hh = prim.params[3] + prim.round + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::Segment: {
            prim.aabbMinX = std::min(prim.params[0], prim.params[2]) - expand;
            prim.aabbMinY = std::min(prim.params[1], prim.params[3]) - expand;
            prim.aabbMaxX = std::max(prim.params[0], prim.params[2]) + expand;
            prim.aabbMaxY = std::max(prim.params[1], prim.params[3]) + expand;
            break;
        }
        case SDFType::Triangle: {
            prim.aabbMinX = std::min({prim.params[0], prim.params[2], prim.params[4]}) - expand;
            prim.aabbMinY = std::min({prim.params[1], prim.params[3], prim.params[5]}) - expand;
            prim.aabbMaxX = std::max({prim.params[0], prim.params[2], prim.params[4]}) + expand;
            prim.aabbMaxY = std::max({prim.params[1], prim.params[3], prim.params[5]}) + expand;
            break;
        }
        case SDFType::Bezier2: {
            prim.aabbMinX = std::min({prim.params[0], prim.params[2], prim.params[4]}) - expand;
            prim.aabbMinY = std::min({prim.params[1], prim.params[3], prim.params[5]}) - expand;
            prim.aabbMaxX = std::max({prim.params[0], prim.params[2], prim.params[4]}) + expand;
            prim.aabbMaxY = std::max({prim.params[1], prim.params[3], prim.params[5]}) + expand;
            break;
        }
        case SDFType::Bezier3: {
            prim.aabbMinX = std::min({prim.params[0], prim.params[2], prim.params[4], prim.params[6]}) - expand;
            prim.aabbMinY = std::min({prim.params[1], prim.params[3], prim.params[5], prim.params[7]}) - expand;
            prim.aabbMaxX = std::max({prim.params[0], prim.params[2], prim.params[4], prim.params[6]}) + expand;
            prim.aabbMaxY = std::max({prim.params[1], prim.params[3], prim.params[5], prim.params[7]}) + expand;
            break;
        }
        case SDFType::Ellipse: {
            prim.aabbMinX = prim.params[0] - prim.params[2] - expand;
            prim.aabbMinY = prim.params[1] - prim.params[3] - expand;
            prim.aabbMaxX = prim.params[0] + prim.params[2] + expand;
            prim.aabbMaxY = prim.params[1] + prim.params[3] + expand;
            break;
        }
        case SDFType::Arc: {
            float r = std::max(prim.params[4], prim.params[5]) + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::RoundedBox: {
            float maxR = std::max({prim.params[4], prim.params[5],
                                   prim.params[6], prim.params[7]});
            float hw = prim.params[2] + maxR + expand;
            float hh = prim.params[3] + maxR + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::Rhombus: {
            float hw = prim.params[2] + expand;
            float hh = prim.params[3] + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::Pentagon:
        case SDFType::Hexagon: {
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Star: {
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Pie: {
            float r = prim.params[4] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Ring: {
            float r = prim.params[4] + prim.params[5] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Heart: {
            float s = prim.params[2] * 1.5f + expand;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::Cross: {
            float hw = std::max(prim.params[2], prim.params[3]) + expand;
            float hh = hw;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::RoundedX: {
            float s = prim.params[2] + prim.params[3] + expand;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::Capsule: {
            float r = prim.params[4] + expand;
            prim.aabbMinX = std::min(prim.params[0], prim.params[2]) - r;
            prim.aabbMinY = std::min(prim.params[1], prim.params[3]) - r;
            prim.aabbMaxX = std::max(prim.params[0], prim.params[2]) + r;
            prim.aabbMaxY = std::max(prim.params[1], prim.params[3]) + r;
            break;
        }
        case SDFType::Moon: {
            float r = std::max(prim.params[3], prim.params[4]) + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r + prim.params[2];
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Egg: {
            float r = std::max(prim.params[2], prim.params[3]) + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r + prim.params[2];
            break;
        }
        case SDFType::ChamferBox: {
            // center(0,1), halfSize(2,3), chamfer(4)
            float hw = prim.params[2] + prim.params[4] + expand;
            float hh = prim.params[3] + prim.params[4] + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::OrientedBox: {
            // a(0,1), b(2,3), thickness(4)
            float maxX = std::max(prim.params[0], prim.params[2]);
            float minX = std::min(prim.params[0], prim.params[2]);
            float maxY = std::max(prim.params[1], prim.params[3]);
            float minY = std::min(prim.params[1], prim.params[3]);
            float th = prim.params[4] * 0.5f + expand;
            prim.aabbMinX = minX - th;
            prim.aabbMinY = minY - th;
            prim.aabbMaxX = maxX + th;
            prim.aabbMaxY = maxY + th;
            break;
        }
        case SDFType::Trapezoid: {
            // center(0,1), r1(2), r2(3), he(4)
            float hw = std::max(prim.params[2], prim.params[3]) + expand;
            float hh = prim.params[4] + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::Parallelogram: {
            // center(0,1), wi(2), he(3), sk(4)
            float hw = prim.params[2] + std::fabs(prim.params[4]) + expand;
            float hh = prim.params[3] + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::EquilateralTriangle:
        case SDFType::Octogon:
        case SDFType::Hexagram:
        case SDFType::Pentagram: {
            // center(0,1), radius(2)
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::IsoscelesTriangle: {
            // center(0,1), q(2,3) where q.x=half-width, q.y=height
            float hw = prim.params[2] + expand;
            float hh = prim.params[3] + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - expand;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::UnevenCapsule: {
            // center(0,1), r1(2), r2(3), h(4)
            float rMax = std::max(prim.params[2], prim.params[3]) + expand;
            prim.aabbMinX = prim.params[0] - rMax;
            prim.aabbMinY = prim.params[1] - prim.params[2] - expand;
            prim.aabbMaxX = prim.params[0] + rMax;
            prim.aabbMaxY = prim.params[1] + prim.params[4] + prim.params[3] + expand;
            break;
        }
        case SDFType::CutDisk: {
            // center(0,1), radius(2), h(3)
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Horseshoe: {
            // center(0,1), c(2,3), r(4), w(5,6)
            float r = prim.params[4] + prim.params[5] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Vesica: {
            // center(0,1), w(2), h(3)
            float hw = prim.params[2] + expand;
            float hh = prim.params[3] + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::OrientedVesica: {
            // a(0,1), b(2,3), w(4)
            float w = prim.params[4] + expand;
            prim.aabbMinX = std::min(prim.params[0], prim.params[2]) - w;
            prim.aabbMinY = std::min(prim.params[1], prim.params[3]) - w;
            prim.aabbMaxX = std::max(prim.params[0], prim.params[2]) + w;
            prim.aabbMaxY = std::max(prim.params[1], prim.params[3]) + w;
            break;
        }
        case SDFType::RoundedCross: {
            // center(0,1), h(2) — size is roughly h+1
            float s = (prim.params[2] + 1.0f) + expand;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::Parabola: {
            // center(0,1), k(2) — parabola extends, use generous bound
            float s = 2.0f / prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::BlobbyCross: {
            // center(0,1), he(2)
            float s = prim.params[2] * 1.5f + expand;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::Tunnel: {
            // center(0,1), wh(2,3)
            float hw = prim.params[2] + expand;
            float hh = prim.params[3] + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::Stairs: {
            // center(0,1), wh(2,3), n(4)
            float tw = prim.params[2] * prim.params[4] + expand;
            float th = prim.params[3] * prim.params[4] + expand;
            prim.aabbMinX = prim.params[0] - expand;
            prim.aabbMinY = prim.params[1] - expand;
            prim.aabbMaxX = prim.params[0] + tw;
            prim.aabbMaxY = prim.params[1] + th;
            break;
        }
        case SDFType::QuadraticCircle: {
            // center(0,1), scale(2)
            float s = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::Hyperbola: {
            // center(0,1), k(2), he(3)
            float s = prim.params[3] + expand + 1.0f;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::CoolS: {
            // center(0,1), scale(2)
            float s = prim.params[2] * 1.2f + expand;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::CircleWave: {
            // center(0,1), tb(2), ra(3)
            float r = prim.params[3] * 2.0f + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        // 3D primitives - not used for grid, set dummy AABB
        case SDFType::Sphere3D:
        case SDFType::Box3D:
        case SDFType::BoxFrame3D:
        case SDFType::Torus3D:
        case SDFType::CappedTorus3D:
        case SDFType::Cylinder3D:
        case SDFType::CappedCylinder3D:
        case SDFType::RoundedCylinder3D:
        case SDFType::VerticalCapsule3D:
        case SDFType::Cone3D:
        case SDFType::CappedCone3D:
        case SDFType::RoundCone3D:
        case SDFType::Plane3D:
        case SDFType::HexPrism3D:
        case SDFType::TriPrism3D:
        case SDFType::Octahedron3D:
        case SDFType::Pyramid3D:
        case SDFType::Ellipsoid3D:
        case SDFType::Rhombus3D:
        case SDFType::Link3D: {
            // 3D primitives are raymarched, not grid-culled.
            // Set a zero-area AABB so they don't affect 2D scene bounds.
            prim.aabbMinX = 0; prim.aabbMinY = 0;
            prim.aabbMaxX = 0; prim.aabbMaxY = 0;
            break;
        }
        default:
            prim.aabbMinX = -1e10f;
            prim.aabbMinY = -1e10f;
            prim.aabbMaxX = 1e10f;
            prim.aabbMaxY = 1e10f;
            break;
    }
}

//=============================================================================
// Constructor / Destructor
//=============================================================================

YDrawBase::YDrawBase(const YettyContext& ctx,
                     int32_t x, int32_t y,
                     uint32_t widthCells, uint32_t heightCells)
    : Card(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
    , _fontManager(ctx.fontManager)
{
    _shaderGlyph = SHADER_GLYPH;
    if (_fontManager) {
        _font = _fontManager->getDefaultMsMsdfFont();
        if (_font) {
            _atlas = _font->atlas();
        }
        ydebug("YDrawBase: fontManager={} font={} atlas={}", (void*)_fontManager.get(), (void*)_font.get(), (void*)_atlas.get());
    } else {
        ywarn("YDrawBase: fontManager is NULL!");
    }
}

YDrawBase::~YDrawBase() {
    dispose();
}

//=============================================================================
// initBase - allocate metadata slot
//=============================================================================

Result<void> YDrawBase::initBase() {
    auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
    if (!metaResult) {
        return Err<void>("YDrawBase::initBase: failed to allocate metadata");
    }
    _metaHandle = *metaResult;
    yinfo("YDrawBase::initBase: allocated metadata at offset {}", _metaHandle.offset);
    _dirty = true;
    _metadataDirty = true;

    if (auto res = registerForEvents(); !res) {
        ywarn("YDrawBase::initBase: event registration failed: {}", error_msg(res));
    }

    return Ok();
}

//=============================================================================
// Card lifecycle
//=============================================================================

Result<void> YDrawBase::dispose() {
    deregisterFromEvents();

    _primStaging.clear();
    _primStaging.shrink_to_fit();

    if (_derivedStorage.isValid() && _cardMgr) {
        if (auto res = _cardMgr->bufferManager()->deallocateBuffer(_derivedStorage); !res) {
            yerror("YDrawBase::dispose: deallocateBuffer (derived) failed: {}", error_msg(res));
        }
        _derivedStorage = StorageHandle::invalid();
        _grid = nullptr;
        _gridSize = 0;
    }

    if (_primStorage.isValid() && _cardMgr) {
        if (auto res = _cardMgr->bufferManager()->deallocateBuffer(_primStorage); !res) {
            yerror("YDrawBase::dispose: deallocateBuffer (prims) failed: {}", error_msg(res));
        }
        _primStorage = StorageHandle::invalid();
        _primitives = nullptr;
        _primCount = 0;
        _primCapacity = 0;
    }

    if (_metaHandle.isValid() && _cardMgr) {
        if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
            yerror("YDrawBase::dispose: deallocateMetadata failed: {}", error_msg(res));
        }
        _metaHandle = MetadataHandle::invalid();
    }

    return Ok();
}

void YDrawBase::suspend() {
    if (_primStorage.isValid() && _primCount > 0) {
        _primStaging.resize(_primCount);
        std::memcpy(_primStaging.data(), _primitives, _primCount * sizeof(SDFPrimitive));
    }

    if (_derivedStorage.isValid()) {
        _cardMgr->bufferManager()->deallocateBuffer(_derivedStorage);
        _derivedStorage = StorageHandle::invalid();
        _grid = nullptr;
        _gridSize = 0;
    }

    if (_primStorage.isValid()) {
        _cardMgr->bufferManager()->deallocateBuffer(_primStorage);
        _primStorage = StorageHandle::invalid();
        _primitives = nullptr;
        _primCount = 0;
        _primCapacity = 0;
    }

    yinfo("YDrawBase::suspend: deallocated storage, saved {} primitives to staging", _primStaging.size());
}

void YDrawBase::declareBufferNeeds() {
    // Save existing GPU data to staging and deallocate
    if (_primStorage.isValid() && _primCount > 0) {
        _primStaging.resize(_primCount);
        std::memcpy(_primStaging.data(), _primitives, _primCount * sizeof(SDFPrimitive));
    }
    uint32_t lastDerivedSize = _derivedStorage.size;
    if (_derivedStorage.isValid()) {
        _cardMgr->bufferManager()->deallocateBuffer(_derivedStorage);
        _derivedStorage = StorageHandle::invalid();
        _grid = nullptr;
        _gridSize = 0;
    }
    if (_primStorage.isValid()) {
        _cardMgr->bufferManager()->deallocateBuffer(_primStorage);
        _primStorage = StorageHandle::invalid();
        _primitives = nullptr;
        _primCount = 0;
        _primCapacity = 0;
    }

    // Reserve prim storage
    if (!_primStaging.empty()) {
        uint32_t primSize = static_cast<uint32_t>(_primStaging.size()) * sizeof(SDFPrimitive);
        _cardMgr->bufferManager()->reserve(primSize);
    }

    // Reserve derived storage (grid + glyphs)
    if (!_primStaging.empty() || !_glyphs.empty()) {
        if (lastDerivedSize > 0) {
            _cardMgr->bufferManager()->reserve(lastDerivedSize);
        } else {
            // First time: estimate derived size from staging data (skip 3D prims)
            uint32_t n = static_cast<uint32_t>(_primStaging.size());
            uint32_t n2D = 0;
            float sMinX = 1e30f, sMinY = 1e30f, sMaxX = -1e30f, sMaxY = -1e30f;
            for (uint32_t i = 0; i < n; i++) {
                if (_primStaging[i].type >= 100) continue;
                sMinX = std::min(sMinX, _primStaging[i].aabbMinX);
                sMinY = std::min(sMinY, _primStaging[i].aabbMinY);
                sMaxX = std::max(sMaxX, _primStaging[i].aabbMaxX);
                sMaxY = std::max(sMaxY, _primStaging[i].aabbMaxY);
                n2D++;
            }
            for (const auto& g : _glyphs) {
                sMinX = std::min(sMinX, g.x);
                sMinY = std::min(sMinY, g.y);
                sMaxX = std::max(sMaxX, g.x + g.width);
                sMaxY = std::max(sMaxY, g.y + g.height);
            }
            float padX = (sMaxX - sMinX) * 0.05f;
            float padY = (sMaxY - sMinY) * 0.05f;
            sMinX -= padX; sMinY -= padY;
            sMaxX += padX; sMaxY += padY;
            float sceneWidth = sMaxX - sMinX;
            float sceneHeight = sMaxY - sMinY;
            float sceneArea = sceneWidth * sceneHeight;
            uint32_t gridW = 0, gridH = 0;
            if ((n2D > 0 || !_glyphs.empty()) && sceneArea > 0) {
                float cs = _cellSize;
                if (cs <= 0.0f) {
                    if (n2D > 0) {
                        float avgPrimArea = 0.0f;
                        for (uint32_t i = 0; i < n; i++) {
                            if (_primStaging[i].type >= 100) continue;
                            float w = _primStaging[i].aabbMaxX - _primStaging[i].aabbMinX;
                            float h = _primStaging[i].aabbMaxY - _primStaging[i].aabbMinY;
                            avgPrimArea += w * h;
                        }
                        avgPrimArea /= n2D;
                        cs = std::sqrt(avgPrimArea) * 1.5f;
                    } else {
                        float avgGlyphH = 0.0f;
                        for (const auto& g : _glyphs) avgGlyphH += g.height;
                        avgGlyphH /= _glyphs.size();
                        cs = avgGlyphH * 2.0f;
                    }
                    float minCS = std::sqrt(sceneArea / 65536.0f);
                    float maxCS = std::sqrt(sceneArea / 16.0f);
                    cs = std::clamp(cs, minCS, maxCS);
                }
                gridW = std::max(1u, static_cast<uint32_t>(std::ceil(sceneWidth / cs)));
                gridH = std::max(1u, static_cast<uint32_t>(std::ceil(sceneHeight / cs)));
                const uint32_t MAX_GRID_DIM = 512;
                gridW = std::min(gridW, MAX_GRID_DIM);
                gridH = std::min(gridH, MAX_GRID_DIM);
            }
            uint32_t cellStride = 1 + _maxPrimsPerCell;
            uint32_t estDerived = gridW * gridH * cellStride * sizeof(uint32_t)
                + static_cast<uint32_t>(_glyphs.size() * sizeof(YDrawGlyph));
            _cardMgr->bufferManager()->reserve(estDerived);
        }
    }
}

uint32_t YDrawBase::computeDerivedSize() const {
    float sMinX = 1e30f, sMinY = 1e30f, sMaxX = -1e30f, sMaxY = -1e30f;
    uint32_t num2D = 0;
    for (uint32_t i = 0; i < _primCount; i++) {
        if (_primitives[i].type >= 100) continue;  // skip 3D prims
        sMinX = std::min(sMinX, _primitives[i].aabbMinX);
        sMinY = std::min(sMinY, _primitives[i].aabbMinY);
        sMaxX = std::max(sMaxX, _primitives[i].aabbMaxX);
        sMaxY = std::max(sMaxY, _primitives[i].aabbMaxY);
        num2D++;
    }
    for (const auto& g : _glyphs) {
        sMinX = std::min(sMinX, g.x);
        sMinY = std::min(sMinY, g.y);
        sMaxX = std::max(sMaxX, g.x + g.width);
        sMaxY = std::max(sMaxY, g.y + g.height);
    }
    float padX = (sMaxX - sMinX) * 0.05f;
    float padY = (sMaxY - sMinY) * 0.05f;
    sMinX -= padX; sMinY -= padY;
    sMaxX += padX; sMaxY += padY;
    float sceneWidth = sMaxX - sMinX;
    float sceneHeight = sMaxY - sMinY;
    float sceneArea = sceneWidth * sceneHeight;
    uint32_t gridW = 0, gridH = 0;
    if ((num2D > 0 || !_glyphs.empty()) && sceneArea > 0) {
        float cs = _cellSize;
        if (cs <= 0.0f) {
            if (num2D > 0) {
                float avgPrimArea = 0.0f;
                for (uint32_t i = 0; i < _primCount; i++) {
                    if (_primitives[i].type >= 100) continue;
                    float w = _primitives[i].aabbMaxX - _primitives[i].aabbMinX;
                    float h = _primitives[i].aabbMaxY - _primitives[i].aabbMinY;
                    avgPrimArea += w * h;
                }
                avgPrimArea /= num2D;
                cs = std::sqrt(avgPrimArea) * 1.5f;
            } else {
                float avgGlyphH = 0.0f;
                for (const auto& g : _glyphs) avgGlyphH += g.height;
                avgGlyphH /= _glyphs.size();
                cs = avgGlyphH * 2.0f;
            }
            float minCS = std::sqrt(sceneArea / 65536.0f);
            float maxCS = std::sqrt(sceneArea / 16.0f);
            cs = std::clamp(cs, minCS, maxCS);
        }
        gridW = std::max(1u, static_cast<uint32_t>(std::ceil(sceneWidth / cs)));
        gridH = std::max(1u, static_cast<uint32_t>(std::ceil(sceneHeight / cs)));
        const uint32_t MAX_GRID_DIM = 512;
        gridW = std::min(gridW, MAX_GRID_DIM);
        gridH = std::min(gridH, MAX_GRID_DIM);
    }
    uint32_t cellStride = 1 + _maxPrimsPerCell;
    uint32_t gridBytes = gridW * gridH * cellStride * sizeof(uint32_t);
    uint32_t glyphBytes = static_cast<uint32_t>(_glyphs.size() * sizeof(YDrawGlyph));
    return gridBytes + glyphBytes;
}

Result<void> YDrawBase::allocateBuffers() {
    if (!_primStaging.empty()) {
        uint32_t count = static_cast<uint32_t>(_primStaging.size());
        if (auto res = ensurePrimCapacity(count); !res) {
            return Err<void>("YDrawBase::allocateBuffers: failed to allocate prim storage");
        }
        _primCount = count;
        std::memcpy(_primitives, _primStaging.data(), count * sizeof(SDFPrimitive));
        _primStaging.clear();
        _primStaging.shrink_to_fit();
        _cardMgr->bufferManager()->markBufferDirty(_primStorage);
    }

    if (_primCount > 0 || !_glyphs.empty()) {
        uint32_t derivedSize = computeDerivedSize();
        if (derivedSize > 0 && !_derivedStorage.isValid()) {
            auto storageResult = _cardMgr->bufferManager()->allocateBuffer(derivedSize);
            if (!storageResult) {
                return Err<void>("YDrawBase::allocateBuffers: failed to allocate derived storage");
            }
            _derivedStorage = *storageResult;
        }
        _dirty = true;
        yinfo("YDrawBase::allocateBuffers: {} primitives, {} glyphs, derived {} bytes",
              _primCount, _glyphs.size(), derivedSize);
    }
    return Ok();
}

Result<void> YDrawBase::render(float time) {
    // Auto-start animation if properties were added but not yet started
    if (_animation && _animation->hasProperties() && !_animation->isPlaying()
        && _basePrimitives.empty() && _primitives && _primCount > 0) {
        startAnimation();
    }

    // Animation update (before dirty check)
    if (_animation && _animation->isPlaying() && _primitives && _primCount > 0) {
        float dt = (_lastRenderTime < 0.0f) ? 0.0f : (time - _lastRenderTime);
        _lastRenderTime = time;

        if (_animation->advance(dt)) {
            _animation->apply(_basePrimitives.data(), _primitives, _primCount);
            for (uint32_t i = 0; i < _primCount; i++) {
                computeAABB(_primitives[i]);
            }
            _cardMgr->bufferManager()->markBufferDirty(_primStorage);
            _dirty = true;
        }
    }

    using Clock = std::chrono::steady_clock;
    auto tRenderStart = Clock::now();
    bool didRebuild = false, didMeta = false;

    if (_dirty) {
        if (auto res = rebuildAndUpload(); !res) {
            return Err<void>("YDrawBase::render: rebuildAndUpload failed", res);
        }
        _dirty = false;
        didRebuild = true;
    }
    auto tAfterRebuild = Clock::now();

    if (_metadataDirty) {
        if (auto res = uploadMetadata(); !res) {
            return Err<void>("YDrawBase::render: metadata upload failed", res);
        }
        _metadataDirty = false;
        didMeta = true;
    }
    auto tAfterMeta = Clock::now();

    if (didRebuild || didMeta) {
        auto us = [](auto a, auto b) { return std::chrono::duration_cast<std::chrono::microseconds>(b - a).count(); };
        std::cout << "YDrawBase::render: rebuild=" << us(tRenderStart, tAfterRebuild)
                  << " us, metadata=" << us(tAfterRebuild, tAfterMeta)
                  << " us, total=" << us(tRenderStart, tAfterMeta) << " us" << std::endl;
    }

    return Ok();
}

//=============================================================================
// Event handling - zoom/pan
//=============================================================================

Result<void> YDrawBase::registerForEvents() {
    auto loopResult = base::EventLoop::instance();
    if (!loopResult) {
        return Err<void>("YDrawBase::registerForEvents: no EventLoop instance", loopResult);
    }
    auto loop = *loopResult;
    auto self = sharedAs<base::EventListener>();

    if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res) {
        return Err<void>("YDrawBase::registerForEvents: failed to register SetFocus", res);
    }
    if (auto res = loop->registerListener(base::Event::Type::CardScroll, self, 1000); !res) {
        return Err<void>("YDrawBase::registerForEvents: failed to register CardScroll", res);
    }
    yinfo("YDrawBase card {} registered for events (priority 1000)", id());
    return Ok();
}

Result<void> YDrawBase::deregisterFromEvents() {
    if (weak_from_this().expired()) {
        return Ok();
    }
    auto loopResult = base::EventLoop::instance();
    if (!loopResult) {
        return Ok();
    }
    auto loop = *loopResult;
    if (auto res = loop->deregisterListener(sharedAs<base::EventListener>()); !res) {
        ywarn("YDrawBase::deregisterFromEvents: {}", error_msg(res));
    }
    return Ok();
}

Result<bool> YDrawBase::onEvent(const base::Event& event) {
    // Handle SetFocus events
    if (event.type == base::Event::Type::SetFocus) {
        if (event.setFocus.objectId == id()) {
            if (!_focused) {
                _focused = true;
                ydebug("YDrawBase::onEvent: focused (id={})", id());
            }
            return Ok(true);
        } else if (_focused) {
            _focused = false;
            ydebug("YDrawBase::onEvent: unfocused (id={})", id());
        }
        return Ok(false);
    }

    // Handle CardScroll events (only for this card)
    if (event.type == base::Event::Type::CardScroll &&
        event.cardScroll.targetId == id()) {

        float sceneW = _sceneMaxX - _sceneMinX;
        float sceneH = _sceneMaxY - _sceneMinY;

        if (event.cardScroll.mods & GLFW_MOD_CONTROL) {
            // Ctrl+Scroll: zoom (no grid rebuild needed — shader handles it)
            float zoomDelta = event.cardScroll.dy * 0.1f;
            float newZoom = std::clamp(_viewZoom + zoomDelta, 0.1f, 20.0f);
            if (newZoom != _viewZoom) {
                _viewZoom = newZoom;
                _metadataDirty = true;
                yinfo("YDrawBase::onEvent: zoom={:.2f}", _viewZoom);
            }
            return Ok(true);
        } else if (event.cardScroll.mods & GLFW_MOD_SHIFT) {
            // Shift+Scroll: pan horizontally
            float dx = event.cardScroll.dy * 0.05f * sceneW / _viewZoom;
            _viewPanX += dx;
            _metadataDirty = true;
            return Ok(true);
        } else {
            // Plain scroll: pan vertically
            float dy = event.cardScroll.dy * 0.05f * sceneH / _viewZoom;
            _viewPanY += dy;
            _metadataDirty = true;
            return Ok(true);
        }
    }

    return Ok(false);
}

//=============================================================================
// Public API - Primitives
//=============================================================================

uint32_t YDrawBase::addPrimitive(const SDFPrimitive& prim) {
    // Auto-set FLAG_HAS_3D for 3D primitives
    if (prim.type >= 100) {
        _flags |= FLAG_HAS_3D;
    }

    if (_initParsing) {
        SDFPrimitive p = prim;
        if (p.aabbMinX == 0 && p.aabbMaxX == 0) {
            computeAABB(p);
        }
        uint32_t idx = static_cast<uint32_t>(_primStaging.size());
        _primStaging.push_back(p);
        return idx;
    }
    if (auto res = ensurePrimCapacity(_primCount + 1); !res) {
        yerror("YDrawBase::addPrimitive: failed to grow storage");
        return 0;
    }
    uint32_t idx = _primCount++;
    _primitives[idx] = prim;
    if (_primitives[idx].aabbMinX == 0 && _primitives[idx].aabbMaxX == 0) {
        computeAABB(_primitives[idx]);
    }
    _cardMgr->bufferManager()->markBufferDirty(_primStorage);
    _dirty = true;
    return idx;
}

uint32_t YDrawBase::addCircle(float cx, float cy, float radius,
                              uint32_t fillColor, uint32_t strokeColor,
                              float strokeWidth, uint32_t layer) {
    SDFPrimitive prim = {};
    prim.type = static_cast<uint32_t>(SDFType::Circle);
    prim.layer = layer;
    prim.params[0] = cx;
    prim.params[1] = cy;
    prim.params[2] = radius;
    prim.fillColor = fillColor;
    prim.strokeColor = strokeColor;
    prim.strokeWidth = strokeWidth;
    return addPrimitive(prim);
}

uint32_t YDrawBase::addBox(float cx, float cy, float halfW, float halfH,
                           uint32_t fillColor, uint32_t strokeColor,
                           float strokeWidth, float round, uint32_t layer) {
    SDFPrimitive prim = {};
    prim.type = static_cast<uint32_t>(SDFType::Box);
    prim.layer = layer;
    prim.params[0] = cx;
    prim.params[1] = cy;
    prim.params[2] = halfW;
    prim.params[3] = halfH;
    prim.fillColor = fillColor;
    prim.strokeColor = strokeColor;
    prim.strokeWidth = strokeWidth;
    prim.round = round;
    return addPrimitive(prim);
}

uint32_t YDrawBase::addSegment(float x0, float y0, float x1, float y1,
                               uint32_t strokeColor, float strokeWidth,
                               uint32_t layer) {
    SDFPrimitive prim = {};
    prim.type = static_cast<uint32_t>(SDFType::Segment);
    prim.layer = layer;
    prim.params[0] = x0;
    prim.params[1] = y0;
    prim.params[2] = x1;
    prim.params[3] = y1;
    prim.strokeColor = strokeColor;
    prim.strokeWidth = strokeWidth;
    return addPrimitive(prim);
}

uint32_t YDrawBase::addBezier2(float x0, float y0, float x1, float y1,
                               float x2, float y2,
                               uint32_t strokeColor, float strokeWidth,
                               uint32_t layer) {
    SDFPrimitive prim = {};
    prim.type = static_cast<uint32_t>(SDFType::Bezier2);
    prim.layer = layer;
    prim.params[0] = x0;
    prim.params[1] = y0;
    prim.params[2] = x1;
    prim.params[3] = y1;
    prim.params[4] = x2;
    prim.params[5] = y2;
    prim.strokeColor = strokeColor;
    prim.strokeWidth = strokeWidth;
    return addPrimitive(prim);
}

//=============================================================================
// Public API - Text
//=============================================================================

uint32_t YDrawBase::addText(float x, float y, const std::string& text,
                            float fontSize, uint32_t color,
                            uint32_t layer, int fontId) {
    if (!_atlas || text.empty()) {
        return 0;
    }

    float fontBaseSize = _atlas->getFontSize();
    float scale = fontSize / fontBaseSize;

    float cursorX = x;
    uint32_t glyphsAdded = 0;

    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(text.data());
    const uint8_t* end = ptr + text.size();

    while (ptr < end) {
        // Decode UTF-8 codepoint
        uint32_t codepoint = 0;
        if ((*ptr & 0x80) == 0) {
            codepoint = *ptr++;
        } else if ((*ptr & 0xE0) == 0xC0) {
            codepoint = (*ptr++ & 0x1F) << 6;
            if (ptr < end) codepoint |= (*ptr++ & 0x3F);
        } else if ((*ptr & 0xF0) == 0xE0) {
            codepoint = (*ptr++ & 0x0F) << 12;
            if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 6;
            if (ptr < end) codepoint |= (*ptr++ & 0x3F);
        } else if ((*ptr & 0xF8) == 0xF0) {
            codepoint = (*ptr++ & 0x07) << 18;
            if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 12;
            if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 6;
            if (ptr < end) codepoint |= (*ptr++ & 0x3F);
        } else {
            ptr++;
            continue;
        }

        uint32_t glyphIndex;
        if (fontId == 0 && _font) {
            // Default font — use MsMsdfFont style dispatch
            glyphIndex = _font->getGlyphIndex(codepoint);
        } else if (fontId > 0) {
            // Registered font — direct atlas CDB lookup
            glyphIndex = _atlas->loadGlyph(fontId, codepoint);
        } else {
            continue;
        }

        const auto& metadata = _atlas->getGlyphMetadata();
        if (glyphIndex >= metadata.size()) {
            cursorX += fontSize * 0.5f;
            continue;
        }

        const auto& glyph = metadata[glyphIndex];

        if (codepoint == 'p' || codepoint == 'A' || codepoint == 'y') {
            yinfo("addText glyph '{}': bearingY={:.2f} sizeY={:.2f} scale={:.3f} "
                  "y={:.2f} → top={:.2f} bot={:.2f} (baseline=y)",
                  (char)codepoint, glyph._bearingY, glyph._sizeY, scale,
                  y, y - glyph._bearingY * scale,
                  y - glyph._bearingY * scale + glyph._sizeY * scale);
        }

        YDrawGlyph posGlyph = {};
        posGlyph.x = cursorX + glyph._bearingX * scale;
        posGlyph.y = y - glyph._bearingY * scale;
        posGlyph.width = glyph._sizeX * scale;
        posGlyph.height = glyph._sizeY * scale;
        posGlyph.glyphIndex = glyphIndex;
        posGlyph.color = color;
        posGlyph.layer = layer;

        _glyphs.push_back(posGlyph);
        glyphsAdded++;

        cursorX += glyph._advance * scale;
    }

    if (glyphsAdded > 0) {
        _dirty = true;
    }

    return glyphsAdded;
}

//=============================================================================
// Public API - Font registration
//=============================================================================

int YDrawBase::registerFont(const std::string& cdbPath,
                            const std::string& ttfPath,
                            MsdfCdbProvider::Ptr provider) {
    if (!_atlas) return -1;
    return _atlas->registerCdb(cdbPath, ttfPath, std::move(provider));
}

int YDrawBase::addFont(const std::string& ttfPath) {
    if (!_atlas || !_fontManager) return -1;

    // Derive CDB path from TTF filename
    auto stem = std::filesystem::path(ttfPath).stem().string();
    auto cacheDir = _fontManager->getCacheDir();
    auto cdbPath = cacheDir + "/" + stem + ".cdb";

    // Check local cache (avoid re-opening same CDB)
    auto it = _fontIdCache.find(cdbPath);
    if (it != _fontIdCache.end()) return it->second;

    // Generate CDB if missing
    if (!std::filesystem::exists(cdbPath)) {
        auto provider = _fontManager->getCdbProvider();
        if (!provider) {
            yerror("addFont: no CDB provider available");
            return -1;
        }

        MsdfCdbConfig cfg;
        cfg.ttfPath = ttfPath;
        cfg.cdbPath = cdbPath;

        if (auto res = provider->generate(cfg); !res) {
            yerror("addFont: CDB generation failed for {}: {}",
                   ttfPath, res.error().message());
            return -1;
        }

    }

    // Open CDB on shared atlas
    int fontId = _atlas->openCdb(cdbPath);
    if (fontId >= 0) {
        _fontIdCache[cdbPath] = fontId;
    }
    return fontId;
}

//=============================================================================
// Public API - State management
//=============================================================================

void YDrawBase::clear() {
    _primCount = 0;
    _gridSize = 0;
    _glyphs.clear();
    _dirty = true;
}

uint32_t YDrawBase::primitiveCount() const {
    return _primCount;
}

uint32_t YDrawBase::totalPendingPrimitives() const {
    return _primCount + static_cast<uint32_t>(_primStaging.size());
}

uint32_t YDrawBase::glyphCount() const {
    return static_cast<uint32_t>(_glyphs.size());
}

void YDrawBase::markDirty() {
    _dirty = true;
}

/* static */ void YDrawBase::recomputeAABB(card::SDFPrimitive& prim) {
    computeAABB(prim);
}

void YDrawBase::markPrimBufferDirty() {
    if (_primStorage.isValid() && _cardMgr) {
        _cardMgr->bufferManager()->markBufferDirty(_primStorage);
    }
}


void YDrawBase::setBgColor(uint32_t color) {
    _bgColor = color;
    _metadataDirty = true;
}

uint32_t YDrawBase::bgColor() const { return _bgColor; }

void YDrawBase::setSceneBounds(float minX, float minY, float maxX, float maxY) {
    _sceneMinX = minX;
    _sceneMinY = minY;
    _sceneMaxX = maxX;
    _sceneMaxY = maxY;
    _hasExplicitBounds = true;
    _dirty = true;
}

bool YDrawBase::hasExplicitBounds() const { return _hasExplicitBounds; }

void YDrawBase::setGridCellSize(float size) {
    _cellSize = size;
    _dirty = true;
}

float YDrawBase::gridCellSize() const { return _cellSize; }

void YDrawBase::setFlags(uint32_t flags) {
    _flags = flags;
    _metadataDirty = true;
}

void YDrawBase::addFlags(uint32_t flags) {
    _flags |= flags;
    _metadataDirty = true;
}

uint32_t YDrawBase::flags() const { return _flags; }

void YDrawBase::setMaxPrimsPerCell(uint32_t max) {
    _maxPrimsPerCell = max;
}

void YDrawBase::setInitParsing(bool v) { _initParsing = v; }
bool YDrawBase::isInitParsing() const { return _initParsing; }

//=============================================================================
// Private - Buffer management
//=============================================================================

Result<void> YDrawBase::ensurePrimCapacity(uint32_t required) {
    if (_primStorage.isValid() && required <= _primCapacity) {
        return Ok();
    }
    uint32_t newCap = std::max(required, _primCapacity == 0 ? 64u : _primCapacity * 2);
    uint32_t newSize = newCap * sizeof(SDFPrimitive);

    auto newStorage = _cardMgr->bufferManager()->allocateBuffer(newSize);
    if (!newStorage) {
        return Err<void>("YDrawBase: failed to allocate prim storage");
    }

    if (_primCount > 0 && _primStorage.isValid()) {
        std::memcpy(newStorage->data, _primStorage.data, _primCount * sizeof(SDFPrimitive));
    }
    if (_primStorage.isValid()) {
        _cardMgr->bufferManager()->deallocateBuffer(_primStorage);
    }

    _primStorage = *newStorage;
    _primitives = reinterpret_cast<SDFPrimitive*>(_primStorage.data);
    _primCapacity = newCap;
    return Ok();
}

//=============================================================================
// Private - Scene bounds
//=============================================================================

void YDrawBase::computeSceneBounds() {
    if (_hasExplicitBounds) return;

    _sceneMinX = 1e10f;
    _sceneMinY = 1e10f;
    _sceneMaxX = -1e10f;
    _sceneMaxY = -1e10f;

    for (uint32_t i = 0; i < _primCount; i++) {
        // Skip 3D primitives - they render in their own 3D coordinate space
        if (_primitives[i].type >= 100) continue;
        _sceneMinX = std::min(_sceneMinX, _primitives[i].aabbMinX);
        _sceneMinY = std::min(_sceneMinY, _primitives[i].aabbMinY);
        _sceneMaxX = std::max(_sceneMaxX, _primitives[i].aabbMaxX);
        _sceneMaxY = std::max(_sceneMaxY, _primitives[i].aabbMaxY);
    }

    for (const auto& glyph : _glyphs) {
        _sceneMinX = std::min(_sceneMinX, glyph.x);
        _sceneMinY = std::min(_sceneMinY, glyph.y);
        _sceneMaxX = std::max(_sceneMaxX, glyph.x + glyph.width);
        _sceneMaxY = std::max(_sceneMaxY, glyph.y + glyph.height);
    }

    float padX = (_sceneMaxX - _sceneMinX) * 0.05f;
    float padY = (_sceneMaxY - _sceneMinY) * 0.05f;
    _sceneMinX -= padX;
    _sceneMinY -= padY;
    _sceneMaxX += padX;
    _sceneMaxY += padY;

    if (_sceneMinX >= _sceneMaxX) {
        _sceneMinX = 0;
        _sceneMaxX = 100;
    }
    if (_sceneMinY >= _sceneMaxY) {
        _sceneMinY = 0;
        _sceneMaxY = 100;
    }
}

//=============================================================================
// Private - Grid building
//=============================================================================

void YDrawBase::buildGrid() {
    if (_gridSize == 0) {
        return;
    }

    uint32_t cellStride = 1 + _maxPrimsPerCell;

    std::memset(_grid, 0, _gridSize * sizeof(uint32_t));

    // Assign primitives to cells (skip 3D - they use raymarching)
    for (uint32_t primIdx = 0; primIdx < _primCount; primIdx++) {
        const auto& prim = _primitives[primIdx];
        if (prim.type >= 100) continue;

        uint32_t cellMinX = static_cast<uint32_t>(
            std::clamp((prim.aabbMinX - _sceneMinX) / _cellSize, 0.0f, float(_gridWidth - 1)));
        uint32_t cellMaxX = static_cast<uint32_t>(
            std::clamp((prim.aabbMaxX - _sceneMinX) / _cellSize, 0.0f, float(_gridWidth - 1)));
        uint32_t cellMinY = static_cast<uint32_t>(
            std::clamp((prim.aabbMinY - _sceneMinY) / _cellSize, 0.0f, float(_gridHeight - 1)));
        uint32_t cellMaxY = static_cast<uint32_t>(
            std::clamp((prim.aabbMaxY - _sceneMinY) / _cellSize, 0.0f, float(_gridHeight - 1)));

        for (uint32_t cy = cellMinY; cy <= cellMaxY; cy++) {
            for (uint32_t cx = cellMinX; cx <= cellMaxX; cx++) {
                uint32_t cellIndex = cy * _gridWidth + cx;
                uint32_t cellOffset = cellIndex * cellStride;
                uint32_t count = _grid[cellOffset];
                if (count < _maxPrimsPerCell) {
                    _grid[cellOffset + 1 + count] = primIdx;
                    _grid[cellOffset] = count + 1;
                }
            }
        }
    }

    // Assign glyphs to cells (bit 31 set to distinguish from prims)
    for (uint32_t gi = 0; gi < static_cast<uint32_t>(_glyphs.size()); gi++) {
        const auto& g = _glyphs[gi];

        uint32_t cellMinX = static_cast<uint32_t>(
            std::clamp((g.x - _sceneMinX) / _cellSize, 0.0f, float(_gridWidth - 1)));
        uint32_t cellMaxX = static_cast<uint32_t>(
            std::clamp((g.x + g.width - _sceneMinX) / _cellSize, 0.0f, float(_gridWidth - 1)));
        uint32_t cellMinY = static_cast<uint32_t>(
            std::clamp((g.y - _sceneMinY) / _cellSize, 0.0f, float(_gridHeight - 1)));
        uint32_t cellMaxY = static_cast<uint32_t>(
            std::clamp((g.y + g.height - _sceneMinY) / _cellSize, 0.0f, float(_gridHeight - 1)));

        for (uint32_t cy = cellMinY; cy <= cellMaxY; cy++) {
            for (uint32_t cx = cellMinX; cx <= cellMaxX; cx++) {
                uint32_t cellIndex = cy * _gridWidth + cx;
                uint32_t cellOffset = cellIndex * cellStride;
                uint32_t count = _grid[cellOffset];
                if (count < _maxPrimsPerCell) {
                    _grid[cellOffset + 1 + count] = gi | GLYPH_BIT;
                    _grid[cellOffset] = count + 1;
                }
            }
        }
    }
}

//=============================================================================
// Private - Rebuild and upload
//=============================================================================

Result<void> YDrawBase::rebuildAndUpload() {
    computeSceneBounds();

    // Grid is always built on full content bounds. Zoom/pan is applied
    // only to the metadata scene bounds sent to the shader.
    float sceneWidth = _sceneMaxX - _sceneMinX;
    float sceneHeight = _sceneMaxY - _sceneMinY;

    uint32_t gridW = 0, gridH = 0;
    float cellSize = _cellSize;

    // Count 2D primitives (skip 3D raymarched prims for grid computation)
    uint32_t num2DPrims = 0;
    for (uint32_t i = 0; i < _primCount; i++) {
        if (_primitives[i].type < 100) num2DPrims++;
    }

    if (num2DPrims > 0 || !_glyphs.empty()) {
        float sceneArea = sceneWidth * sceneHeight;
        if (cellSize <= 0.0f) {
            if (num2DPrims > 0) {
                float avgPrimArea = 0.0f;
                for (uint32_t i = 0; i < _primCount; i++) {
                    if (_primitives[i].type >= 100) continue;
                    float w = _primitives[i].aabbMaxX - _primitives[i].aabbMinX;
                    float h = _primitives[i].aabbMaxY - _primitives[i].aabbMinY;
                    avgPrimArea += w * h;
                }
                avgPrimArea /= num2DPrims;
                cellSize = std::sqrt(avgPrimArea) * 1.5f;
            } else {
                float avgGlyphH = 0.0f;
                for (const auto& g : _glyphs) {
                    avgGlyphH += g.height;
                }
                avgGlyphH /= _glyphs.size();
                cellSize = avgGlyphH * 2.0f;
            }
            float minCellSize = std::sqrt(sceneArea / 65536.0f);
            float maxCellSize = std::sqrt(sceneArea / 16.0f);
            cellSize = std::clamp(cellSize, minCellSize, maxCellSize);
        }
        gridW = std::max(1u, static_cast<uint32_t>(std::ceil(sceneWidth / cellSize)));
        gridH = std::max(1u, static_cast<uint32_t>(std::ceil(sceneHeight / cellSize)));
        const uint32_t MAX_GRID_DIM = 512;
        if (gridW > MAX_GRID_DIM) { gridW = MAX_GRID_DIM; cellSize = sceneWidth / gridW; }
        if (gridH > MAX_GRID_DIM) { gridH = MAX_GRID_DIM; cellSize = std::max(cellSize, sceneHeight / gridH); }
    }

    uint32_t cellStride = 1 + _maxPrimsPerCell;
    uint32_t gridTotalU32 = gridW * gridH * cellStride;
    uint32_t gridBytes = gridTotalU32 * sizeof(uint32_t);
    uint32_t glyphBytes = static_cast<uint32_t>(_glyphs.size() * sizeof(YDrawGlyph));
    uint32_t derivedTotalSize = gridBytes + glyphBytes;

    std::cout << "YDrawBase::rebuild: grid=" << gridW << "x" << gridH
              << " cellSize=" << cellSize
              << " derivedTotal=" << derivedTotalSize
              << " allocated=" << (_derivedStorage.isValid() ? _derivedStorage.size : 0)
              << " prims=" << _primCount << " glyphs=" << _glyphs.size()
              << " zoom=" << _viewZoom << std::endl;

    // Allocate or reallocate derived storage if needed
    if (derivedTotalSize > 0) {
        if (!_derivedStorage.isValid()) {
            auto storageResult = _cardMgr->bufferManager()->allocateBuffer(derivedTotalSize);
            if (!storageResult) {
                return Err<void>("YDrawBase::rebuild: failed to allocate derived storage");
            }
            _derivedStorage = *storageResult;
        } else if (derivedTotalSize > _derivedStorage.size) {
            _cardMgr->bufferManager()->deallocateBuffer(_derivedStorage);
            auto storageResult = _cardMgr->bufferManager()->allocateBuffer(derivedTotalSize);
            if (!storageResult) {
                _derivedStorage = StorageHandle::invalid();
                return Err<void>("YDrawBase::rebuild: failed to reallocate derived storage");
            }
            _derivedStorage = *storageResult;
        }
    }

    if (_derivedStorage.isValid() && derivedTotalSize > 0) {
        uint8_t* base = _derivedStorage.data;
        uint32_t offset = 0;

        // Zero entire buffer to prevent stale data artifacts when
        // grid dimensions change (e.g. on zoom/pan)
        std::memset(base, 0, _derivedStorage.size);

        _grid = reinterpret_cast<uint32_t*>(base + offset);
        _gridSize = gridTotalU32;
        _gridWidth = gridW;
        _gridHeight = gridH;
        _cellSize = cellSize;
        _gridOffset = (_derivedStorage.offset + offset) / sizeof(float);
        offset += gridBytes;

        buildGrid();

        _glyphOffset = (_derivedStorage.offset + offset) / sizeof(float);
        if (!_glyphs.empty()) {
            std::memcpy(base + offset, _glyphs.data(), glyphBytes);
        }

        _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
    }

    _primitiveOffset = _primStorage.isValid() ? _primStorage.offset / sizeof(float) : 0;

    if (_primStorage.isValid()) {
        _cardMgr->bufferManager()->markBufferDirty(_primStorage);
    }

    _metadataDirty = true;

    return Ok();
}

//=============================================================================
// Private - Metadata upload
//=============================================================================

Result<void> YDrawBase::uploadMetadata() {
    if (!_metaHandle.isValid()) {
        return Err<void>("YDrawBase::uploadMetadata: invalid metadata handle");
    }

    // Scene bounds in metadata = content bounds (grid origin).
    // Zoom/pan packed into upper bits of flags, widthCells, heightCells.
    //   flags:       [31:16] = zoom as f16,  [15:0] = flags
    //   widthCells:  [31:16] = panX i16 fixedpoint, [15:0] = widthCells
    //   heightCells: [31:16] = panY i16 fixedpoint, [15:0] = heightCells
    // Pan fixedpoint: value = pan / sceneExtent * 16384, clamped to i16

    // Pack zoom as IEEE 754 half-float in upper 16 bits of flags
    // Simple f32→f16 conversion (no denormals/inf handling needed for zoom range)
    uint32_t zoomBits;
    {
        uint32_t f32bits;
        std::memcpy(&f32bits, &_viewZoom, sizeof(float));
        uint32_t sign = (f32bits >> 16) & 0x8000;
        int32_t  exp  = ((f32bits >> 23) & 0xFF) - 127 + 15;
        uint32_t mant = (f32bits >> 13) & 0x3FF;
        if (exp <= 0) { exp = 0; mant = 0; }
        else if (exp >= 31) { exp = 31; mant = 0; }
        zoomBits = sign | (static_cast<uint32_t>(exp) << 10) | mant;
    }

    float contentW = _sceneMaxX - _sceneMinX;
    float contentH = _sceneMaxY - _sceneMinY;
    int16_t panXi16 = static_cast<int16_t>(std::clamp(
        _viewPanX / std::max(contentW, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));
    int16_t panYi16 = static_cast<int16_t>(std::clamp(
        _viewPanY / std::max(contentH, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));

    Metadata meta = {};
    meta.primitiveOffset = _primitiveOffset;
    meta.primitiveCount = _primCount;
    meta.gridOffset = _gridOffset;
    meta.gridWidth = _gridWidth;
    meta.gridHeight = _gridHeight;
    std::memcpy(&meta.cellSize, &_cellSize, sizeof(float));
    meta.glyphOffset = _glyphOffset;
    meta.glyphCount = static_cast<uint32_t>(_glyphs.size());
    std::memcpy(&meta.sceneMinX, &_sceneMinX, sizeof(float));
    std::memcpy(&meta.sceneMinY, &_sceneMinY, sizeof(float));
    std::memcpy(&meta.sceneMaxX, &_sceneMaxX, sizeof(float));
    std::memcpy(&meta.sceneMaxY, &_sceneMaxY, sizeof(float));
    meta.widthCells  = (_widthCells & 0xFFFF) | (static_cast<uint32_t>(static_cast<uint16_t>(panXi16)) << 16);
    meta.heightCells = (_heightCells & 0xFFFF) | (static_cast<uint32_t>(static_cast<uint16_t>(panYi16)) << 16);
    meta.flags = (_flags & 0xFFFF) | (zoomBits << 16);
    meta.bgColor = _bgColor;

    ydebug("YDrawBase::uploadMetadata: prims={} grid={}x{} cellSize={} "
           "scene=[{},{},{},{}] zoom={:.2f} pan=({:.1f},{:.1f}) size={}x{} bgColor={:#010x}",
           meta.primitiveCount, meta.gridWidth, meta.gridHeight,
           _cellSize, _sceneMinX, _sceneMinY, _sceneMaxX, _sceneMaxY,
           _viewZoom, _viewPanX, _viewPanY,
           _widthCells, _heightCells, meta.bgColor);

    if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
        return Err<void>("YDrawBase::uploadMetadata: write failed");
    }

    return Ok();
}

//=============================================================================
// Animation API
//=============================================================================

animation::Animation* YDrawBase::animation() {
    if (!_animation) {
        _animation = std::make_unique<animation::Animation>();
    }
    return _animation.get();
}

void YDrawBase::startAnimation() {
    if (!_animation || !_animation->hasProperties()) return;
    if (!_primitives || _primCount == 0) return;

    // Snapshot current primitives as base values
    _basePrimitives.assign(_primitives, _primitives + _primCount);
    _lastRenderTime = -1.0f;

    // Compute stable scene bounds that encompass all animation keyframe positions.
    // Without this, scene bounds shift every frame as primitives move, causing
    // the viewport to jitter.
    if (!_hasExplicitBounds) {
        // Start with base primitive bounds
        float minX = 1e10f, minY = 1e10f, maxX = -1e10f, maxY = -1e10f;
        for (uint32_t i = 0; i < _primCount; i++) {
            minX = std::min(minX, _primitives[i].aabbMinX);
            minY = std::min(minY, _primitives[i].aabbMinY);
            maxX = std::max(maxX, _primitives[i].aabbMaxX);
            maxY = std::max(maxY, _primitives[i].aabbMaxY);
        }

        // Expand bounds to include all animation keyframe positions/scales
        _animation->expandBounds(_basePrimitives.data(), _primCount,
                                 minX, minY, maxX, maxY);

        float padX = (maxX - minX) * 0.05f;
        float padY = (maxY - minY) * 0.05f;
        setSceneBounds(minX - padX, minY - padY, maxX + padX, maxY + padY);
    }

    _animation->play();
}

void YDrawBase::stopAnimation() {
    if (!_animation) return;
    _animation->stop();

    // Restore base primitives
    if (!_basePrimitives.empty() && _primitives && _primCount > 0) {
        uint32_t n = std::min(static_cast<uint32_t>(_basePrimitives.size()), _primCount);
        std::memcpy(_primitives, _basePrimitives.data(), n * sizeof(card::SDFPrimitive));
        _basePrimitives.clear();
        _dirty = true;
    }
}

} // namespace yetty
