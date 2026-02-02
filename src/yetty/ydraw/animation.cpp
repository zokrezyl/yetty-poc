#include "animation.h"
#include "../cards/hdraw/hdraw.h"  // SDFPrimitive, SDFType
#include <algorithm>
#include <cmath>
#include <cstring>

namespace yetty::animation {

//=============================================================================
// AnimatedProperty::evaluate — linear interpolation between keyframes
//=============================================================================

uint8_t AnimatedProperty::evaluate(float time, float* out) const {
    if (keyframes.empty()) return 0;

    // Single keyframe or before first → clamp to first
    if (keyframes.size() == 1 || time <= keyframes.front().time) {
        uint8_t n = keyframes.front().componentCount;
        std::memcpy(out, keyframes.front().value, n * sizeof(float));
        return n;
    }

    // After last → clamp to last
    if (time >= keyframes.back().time) {
        uint8_t n = keyframes.back().componentCount;
        std::memcpy(out, keyframes.back().value, n * sizeof(float));
        return n;
    }

    // Binary search for the interval [k0, k1] containing time
    // We want the largest i such that keyframes[i].time <= time
    size_t lo = 0, hi = keyframes.size() - 1;
    while (lo + 1 < hi) {
        size_t mid = (lo + hi) / 2;
        if (keyframes[mid].time <= time) {
            lo = mid;
        } else {
            hi = mid;
        }
    }

    const auto& k0 = keyframes[lo];
    const auto& k1 = keyframes[hi];

    float span = k1.time - k0.time;
    float t = (span > 0.0f) ? (time - k0.time) / span : 0.0f;

    // Linear interpolation
    uint8_t n = std::min(k0.componentCount, k1.componentCount);
    for (uint8_t i = 0; i < n; i++) {
        out[i] = k0.value[i] + t * (k1.value[i] - k0.value[i]);
    }
    return n;
}

//=============================================================================
// Animation — playback control
//=============================================================================

void Animation::play() { _playing = true; }
void Animation::pause() { _playing = false; }

void Animation::stop() {
    _playing = false;
    _time = 0.0f;
}

void Animation::seek(float timeSeconds) {
    _time = std::clamp(timeSeconds, 0.0f, _duration);
}

void Animation::setLoop(bool v) { _loop = v; }
void Animation::setSpeed(float s) { _speed = s; }
void Animation::setDuration(float d) { _duration = std::max(d, 0.001f); }

void Animation::addProperty(AnimatedProperty&& p) {
    // Sort keyframes by time
    std::sort(p.keyframes.begin(), p.keyframes.end(),
              [](const Keyframe& a, const Keyframe& b) { return a.time < b.time; });
    _properties.push_back(std::move(p));
}

//=============================================================================
// Animation::advance — step time forward
//=============================================================================

bool Animation::advance(float deltaTime) {
    if (!_playing) return false;

    _time += deltaTime * _speed;

    if (_time >= _duration) {
        if (_loop) {
            _time = std::fmod(_time, _duration);
        } else {
            _time = _duration;
            _playing = false;
        }
    } else if (_time < 0.0f) {
        if (_loop) {
            _time = _duration + std::fmod(_time, _duration);
        } else {
            _time = 0.0f;
            _playing = false;
        }
    }

    return true;
}

//=============================================================================
// Animation::apply — evaluate all properties, write to output primitives
//=============================================================================

void Animation::apply(const card::SDFPrimitive* base,
                      card::SDFPrimitive* out,
                      uint32_t count) const {
    // Start with a copy of base primitives
    if (base != out) {
        std::memcpy(out, base, count * sizeof(card::SDFPrimitive));
    }

    float values[4];

    for (const auto& prop : _properties) {
        if (prop.primitiveIndex >= count) continue;

        uint8_t n = prop.evaluate(_time, values);
        if (n == 0) continue;

        auto& prim = out[prop.primitiveIndex];
        const auto& basePrim = base[prop.primitiveIndex];

        switch (prop.type) {
        case PropertyType::Position:
            if (n >= 2) {
                // For center-based shapes: set params[0], params[1] directly
                // For multi-point shapes (segment, bezier): translate all points
                auto sdfType = static_cast<card::SDFType>(basePrim.type);
                switch (sdfType) {
                case card::SDFType::Segment:
                case card::SDFType::Bezier2:
                case card::SDFType::Triangle: {
                    // Translate all control points by delta from base center
                    float dx = values[0] - basePrim.params[0];
                    float dy = values[1] - basePrim.params[1];
                    int pointCount = (sdfType == card::SDFType::Segment) ? 4 :
                                     (sdfType == card::SDFType::Triangle) ? 6 : 6;
                    for (int i = 0; i < pointCount; i += 2) {
                        prim.params[i]     = basePrim.params[i]     + dx;
                        prim.params[i + 1] = basePrim.params[i + 1] + dy;
                    }
                    break;
                }
                case card::SDFType::Bezier3: {
                    float dx = values[0] - basePrim.params[0];
                    float dy = values[1] - basePrim.params[1];
                    for (int i = 0; i < 8; i += 2) {
                        prim.params[i]     = basePrim.params[i]     + dx;
                        prim.params[i + 1] = basePrim.params[i + 1] + dy;
                    }
                    break;
                }
                default:
                    // Center-based shapes (circle, box, ellipse, star, etc.)
                    prim.params[0] = values[0];
                    prim.params[1] = values[1];
                    break;
                }
            }
            break;

        case PropertyType::Scale:
            if (n >= 1) {
                float s = values[0];
                auto sdfType = static_cast<card::SDFType>(basePrim.type);
                switch (sdfType) {
                case card::SDFType::Circle:
                    prim.params[2] = basePrim.params[2] * s;
                    break;
                case card::SDFType::Box:
                case card::SDFType::Ellipse:
                    prim.params[2] = basePrim.params[2] * s;
                    prim.params[3] = basePrim.params[3] * s;
                    break;
                case card::SDFType::RoundedBox:
                    prim.params[2] = basePrim.params[2] * s;
                    prim.params[3] = basePrim.params[3] * s;
                    prim.params[4] = basePrim.params[4] * s;
                    prim.params[5] = basePrim.params[5] * s;
                    prim.params[6] = basePrim.params[6] * s;
                    prim.params[7] = basePrim.params[7] * s;
                    break;
                case card::SDFType::Star:
                case card::SDFType::Pentagon:
                case card::SDFType::Hexagon:
                    prim.params[2] = basePrim.params[2] * s;  // radius
                    break;
                default:
                    break;
                }
                // Also scale stroke width
                prim.strokeWidth = basePrim.strokeWidth * s;
            }
            break;

        case PropertyType::FillOpacity:
            if (n >= 1) {
                float alpha = std::clamp(values[0], 0.0f, 1.0f);
                uint8_t a = static_cast<uint8_t>(alpha * 255.0f);
                prim.fillColor = (basePrim.fillColor & 0x00FFFFFF) | (static_cast<uint32_t>(a) << 24);
            }
            break;

        case PropertyType::FillColor:
            if (n >= 3) {
                uint8_t r = static_cast<uint8_t>(std::clamp(values[0], 0.0f, 1.0f) * 255.0f);
                uint8_t g = static_cast<uint8_t>(std::clamp(values[1], 0.0f, 1.0f) * 255.0f);
                uint8_t b = static_cast<uint8_t>(std::clamp(values[2], 0.0f, 1.0f) * 255.0f);
                uint8_t a = (n >= 4)
                    ? static_cast<uint8_t>(std::clamp(values[3], 0.0f, 1.0f) * 255.0f)
                    : static_cast<uint8_t>((basePrim.fillColor >> 24) & 0xFF);
                prim.fillColor = static_cast<uint32_t>(a) << 24 |
                                 static_cast<uint32_t>(b) << 16 |
                                 static_cast<uint32_t>(g) << 8 |
                                 static_cast<uint32_t>(r);
            }
            break;

        case PropertyType::StrokeOpacity:
            if (n >= 1) {
                float alpha = std::clamp(values[0], 0.0f, 1.0f);
                uint8_t a = static_cast<uint8_t>(alpha * 255.0f);
                prim.strokeColor = (basePrim.strokeColor & 0x00FFFFFF) | (static_cast<uint32_t>(a) << 24);
            }
            break;

        case PropertyType::StrokeWidth:
            if (n >= 1) {
                prim.strokeWidth = values[0];
            }
            break;

        case PropertyType::Radius:
            if (n >= 1) {
                prim.params[2] = values[0];
            }
            break;
        }
    }
}

//=============================================================================
// Animation::expandBounds — include all keyframe positions in bounding box
//=============================================================================

void Animation::expandBounds(const card::SDFPrimitive* base, uint32_t count,
                             float& minX, float& minY,
                             float& maxX, float& maxY) const {
    for (const auto& prop : _properties) {
        if (prop.primitiveIndex >= count) continue;
        const auto& basePrim = base[prop.primitiveIndex];

        if (prop.type == PropertyType::Position) {
            // Compute base primitive half-size from AABB
            float hw = (basePrim.aabbMaxX - basePrim.aabbMinX) * 0.5f;
            float hh = (basePrim.aabbMaxY - basePrim.aabbMinY) * 0.5f;
            for (const auto& kf : prop.keyframes) {
                if (kf.componentCount >= 2) {
                    minX = std::min(minX, kf.value[0] - hw);
                    minY = std::min(minY, kf.value[1] - hh);
                    maxX = std::max(maxX, kf.value[0] + hw);
                    maxY = std::max(maxY, kf.value[1] + hh);
                }
            }
        } else if (prop.type == PropertyType::Scale) {
            // Find max scale and expand this primitive's AABB
            float maxScale = 1.0f;
            for (const auto& kf : prop.keyframes) {
                if (kf.componentCount >= 1)
                    maxScale = std::max(maxScale, kf.value[0]);
            }
            float cx = (basePrim.aabbMinX + basePrim.aabbMaxX) * 0.5f;
            float cy = (basePrim.aabbMinY + basePrim.aabbMaxY) * 0.5f;
            float hw = (basePrim.aabbMaxX - basePrim.aabbMinX) * 0.5f * maxScale;
            float hh = (basePrim.aabbMaxY - basePrim.aabbMinY) * 0.5f * maxScale;
            minX = std::min(minX, cx - hw);
            minY = std::min(minY, cy - hh);
            maxX = std::max(maxX, cx + hw);
            maxY = std::max(maxY, cy + hh);
        }
    }
}

} // namespace yetty::animation
