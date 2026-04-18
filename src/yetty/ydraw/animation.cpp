#include "animation.h"
#include "ydraw-types.gen.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <unordered_map>

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
// AnimationImpl — private implementation
//=============================================================================

class AnimationImpl : public Animation {
public:
    explicit AnimationImpl(std::shared_ptr<YDrawBuffer> buffer)
        : _buffer(std::move(buffer)) {}

    void play() override { _playing = true; }
    void pause() override { _playing = false; }

    void stop() override {
        _playing = false;
        _time = 0.0f;
    }

    void seek(float timeSeconds) override {
        _time = std::clamp(timeSeconds, 0.0f, _duration);
    }

    void setLoop(bool v) override { _loop = v; }
    void setSpeed(float s) override { _speed = s; }
    void setDuration(float d) override { _duration = std::max(d, 0.001f); }

    bool isPlaying() const override { return _playing; }
    float currentTime() const override { return _time; }
    float duration() const override { return _duration; }
    bool hasProperties() const override { return !_properties.empty(); }

    void addProperty(AnimatedProperty&& p) override {
        std::sort(p.keyframes.begin(), p.keyframes.end(),
                  [](const Keyframe& a, const Keyframe& b) { return a.time < b.time; });
        _properties.push_back(std::move(p));
    }

    bool advance(float deltaTime) override {
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

    void snapshotBase() override {
        _baseData.clear();
        _buffer->forEachPrim([this](uint32_t id, const float* data, uint32_t wc) {
            _baseData[id] = std::vector<float>(data, data + wc);
        });
    }

    void apply() override {
        float values[4];

        // Collect modified word arrays per prim (copy-on-first-write)
        std::unordered_map<uint32_t, std::vector<float>> modified;

        for (const auto& prop : _properties) {
            auto baseIt = _baseData.find(prop.primitiveId);
            if (baseIt == _baseData.end()) continue;

            uint8_t n = prop.evaluate(_time, values);
            if (n == 0) continue;

            const auto& baseWords = baseIt->second;
            uint32_t wc = static_cast<uint32_t>(baseWords.size());

            // Get or create the working copy
            auto& words = modified[prop.primitiveId];
            if (words.empty()) {
                words = baseWords;
            }

            // Get type from word[0]
            uint32_t typeVal;
            std::memcpy(&typeVal, &baseWords[0], sizeof(uint32_t));
            auto sdfType = static_cast<card::SDFType>(typeVal);

            switch (prop.type) {
            case PropertyType::Position:
                if (n >= 2) {
                    switch (sdfType) {
                    case card::SDFType::Segment:
                    case card::SDFType::Bezier2:
                    case card::SDFType::Triangle: {
                        float dx = values[0] - baseWords[2];
                        float dy = values[1] - baseWords[3];
                        int pointCount = (sdfType == card::SDFType::Segment) ? 4 :
                                         (sdfType == card::SDFType::Triangle) ? 6 : 6;
                        for (int i = 0; i < pointCount; i += 2) {
                            words[2 + i]     = baseWords[2 + i]     + dx;
                            words[2 + i + 1] = baseWords[2 + i + 1] + dy;
                        }
                        break;
                    }
                    case card::SDFType::Bezier3: {
                        float dx = values[0] - baseWords[2];
                        float dy = values[1] - baseWords[3];
                        for (int i = 0; i < 8; i += 2) {
                            words[2 + i]     = baseWords[2 + i]     + dx;
                            words[2 + i + 1] = baseWords[2 + i + 1] + dy;
                        }
                        break;
                    }
                    default:
                        words[2] = values[0];
                        words[3] = values[1];
                        break;
                    }
                }
                break;

            case PropertyType::Scale:
                if (n >= 1) {
                    float s = values[0];
                    switch (sdfType) {
                    case card::SDFType::Circle:
                        words[4] = baseWords[4] * s;
                        break;
                    case card::SDFType::Box:
                    case card::SDFType::Ellipse:
                        words[4] = baseWords[4] * s;
                        words[5] = baseWords[5] * s;
                        break;
                    case card::SDFType::RoundedBox:
                        for (uint32_t i = 4; i <= 9; i++) words[i] = baseWords[i] * s;
                        break;
                    case card::SDFType::Star:
                    case card::SDFType::Pentagon:
                    case card::SDFType::Hexagon:
                        words[4] = baseWords[4] * s;
                        break;
                    default:
                        break;
                    }
                    // Also scale stroke width
                    words[wc - 2] = baseWords[wc - 2] * s;
                }
                break;

            case PropertyType::FillOpacity:
                if (n >= 1) {
                    float alpha = std::clamp(values[0], 0.0f, 1.0f);
                    uint8_t a = static_cast<uint8_t>(alpha * 255.0f);
                    uint32_t baseColor;
                    std::memcpy(&baseColor, &baseWords[wc - 4], sizeof(uint32_t));
                    uint32_t newColor = (baseColor & 0x00FFFFFF) | (static_cast<uint32_t>(a) << 24);
                    std::memcpy(&words[wc - 4], &newColor, sizeof(uint32_t));
                }
                break;

            case PropertyType::FillColor:
                if (n >= 3) {
                    uint8_t r = static_cast<uint8_t>(std::clamp(values[0], 0.0f, 1.0f) * 255.0f);
                    uint8_t g = static_cast<uint8_t>(std::clamp(values[1], 0.0f, 1.0f) * 255.0f);
                    uint8_t b = static_cast<uint8_t>(std::clamp(values[2], 0.0f, 1.0f) * 255.0f);
                    uint32_t baseColor;
                    std::memcpy(&baseColor, &baseWords[wc - 4], sizeof(uint32_t));
                    uint8_t a = (n >= 4)
                        ? static_cast<uint8_t>(std::clamp(values[3], 0.0f, 1.0f) * 255.0f)
                        : static_cast<uint8_t>((baseColor >> 24) & 0xFF);
                    uint32_t newColor = static_cast<uint32_t>(a) << 24 |
                                        static_cast<uint32_t>(b) << 16 |
                                        static_cast<uint32_t>(g) << 8 |
                                        static_cast<uint32_t>(r);
                    std::memcpy(&words[wc - 4], &newColor, sizeof(uint32_t));
                }
                break;

            case PropertyType::StrokeOpacity:
                if (n >= 1) {
                    float alpha = std::clamp(values[0], 0.0f, 1.0f);
                    uint8_t a = static_cast<uint8_t>(alpha * 255.0f);
                    uint32_t baseColor;
                    std::memcpy(&baseColor, &baseWords[wc - 3], sizeof(uint32_t));
                    uint32_t newColor = (baseColor & 0x00FFFFFF) | (static_cast<uint32_t>(a) << 24);
                    std::memcpy(&words[wc - 3], &newColor, sizeof(uint32_t));
                }
                break;

            case PropertyType::StrokeWidth:
                if (n >= 1) {
                    words[wc - 2] = values[0];
                }
                break;

            case PropertyType::Radius:
                if (n >= 1) {
                    words[4] = values[0];
                }
                break;
            }
        }

        // Write all modified prims back to buffer
        for (const auto& [id, words] : modified) {
            _buffer->updatePrim(id, words.data(), static_cast<uint32_t>(words.size()));
        }
    }

    void expandBounds(float& minX, float& minY,
                      float& maxX, float& maxY) const override {
        for (const auto& prop : _properties) {
            auto it = _baseData.find(prop.primitiveId);
            if (it == _baseData.end()) continue;
            const auto& words = it->second;
            uint32_t wc = static_cast<uint32_t>(words.size());

            if (prop.type == PropertyType::Position) {
                // Estimate half-size from base data geometry params
                float hw = 0, hh = 0;
                if (wc > 4) {
                    hw = std::abs(words[4]);
                    hh = (wc > 5) ? std::abs(words[5]) : hw;
                }
                for (const auto& kf : prop.keyframes) {
                    if (kf.componentCount >= 2) {
                        minX = std::min(minX, kf.value[0] - hw);
                        minY = std::min(minY, kf.value[1] - hh);
                        maxX = std::max(maxX, kf.value[0] + hw);
                        maxY = std::max(maxY, kf.value[1] + hh);
                    }
                }
            } else if (prop.type == PropertyType::Scale) {
                float maxScale = 1.0f;
                for (const auto& kf : prop.keyframes) {
                    if (kf.componentCount >= 1)
                        maxScale = std::max(maxScale, kf.value[0]);
                }
                if (wc > 4) {
                    float cx = words[2], cy = words[3];
                    float hw = std::abs(words[4]) * maxScale;
                    float hh = (wc > 5 ? std::abs(words[5]) : hw) * maxScale;
                    minX = std::min(minX, cx - hw);
                    minY = std::min(minY, cy - hh);
                    maxX = std::max(maxX, cx + hw);
                    maxY = std::max(maxY, cy + hh);
                }
            }
        }
    }

private:
    std::shared_ptr<YDrawBuffer> _buffer;
    std::vector<AnimatedProperty> _properties;
    std::unordered_map<uint32_t, std::vector<float>> _baseData;

    float _time = 0.0f;
    float _duration = 1.0f;
    float _speed = 1.0f;
    bool _playing = false;
    bool _loop = true;
};

//=============================================================================
// Factory
//=============================================================================

Result<Animation::Ptr> Animation::createImpl(std::shared_ptr<YDrawBuffer> buffer) {
    auto impl = Ptr(new AnimationImpl(std::move(buffer)));
    return Ok(std::move(impl));
}

} // namespace yetty::animation
