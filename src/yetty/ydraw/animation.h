#pragma once

#include <cstdint>
#include <vector>

// Forward declare SDFPrimitive
namespace yetty::card { struct SDFPrimitive; }

namespace yetty::animation {

//=============================================================================
// Animatable property types
//=============================================================================
enum class PropertyType : uint8_t {
    Position = 0,       // params[0..1] (center x, y) — absolute
    Scale = 1,          // uniform scale multiplier on size params
    FillOpacity = 2,    // alpha of fillColor (0.0–1.0)
    FillColor = 3,      // RGBA as 4 floats (0.0–1.0 each)
    StrokeOpacity = 4,  // alpha of strokeColor (0.0–1.0)
    StrokeWidth = 5,    // strokeWidth field — absolute
    Radius = 6,         // params[2] (circle radius, etc.) — absolute
};

//=============================================================================
// Keyframe — single value at a point in time
//=============================================================================
struct Keyframe {
    float time;              // seconds
    float value[4];          // up to 4 components
    uint8_t componentCount;  // 1–4
    // Future: bezier easing tangents
    // float inTangent[2], outTangent[2];
    // uint8_t easingType;  // 0=linear, 1=bezier
};

//=============================================================================
// AnimatedProperty — a sequence of keyframes for one property of one primitive
//=============================================================================
struct AnimatedProperty {
    PropertyType type;
    uint32_t primitiveIndex;
    std::vector<Keyframe> keyframes;  // sorted by time ascending

    // Evaluate at given time via linear interpolation.
    // Writes to out[], returns number of components written.
    uint8_t evaluate(float time, float* out) const;
};

//=============================================================================
// Animation — playback controller + property collection
//=============================================================================
class Animation {
public:
    Animation() = default;

    // Playback control
    void play();
    void pause();
    void stop();
    void seek(float timeSeconds);

    // Configuration
    void setLoop(bool v);
    void setSpeed(float s);
    void setDuration(float d);

    // State queries
    bool isPlaying() const { return _playing; }
    float currentTime() const { return _time; }
    float duration() const { return _duration; }
    bool hasProperties() const { return !_properties.empty(); }

    // Property management
    void addProperty(AnimatedProperty&& p);

    // Advance time by deltaTime * speed.  Returns true if time changed.
    bool advance(float deltaTime);

    // Apply current animation state to primitives.
    // Reads base values from `base`, writes animated values to `out`.
    // `base` and `out` may alias (but then scale/opacity are applied to already-animated values).
    void apply(const card::SDFPrimitive* base,
               card::SDFPrimitive* out,
               uint32_t count) const;

    // Expand bounding box to encompass all keyframe positions/scales.
    void expandBounds(const card::SDFPrimitive* base, uint32_t count,
                      float& minX, float& minY, float& maxX, float& maxY) const;

private:
    std::vector<AnimatedProperty> _properties;
    float _time = 0.0f;
    float _duration = 1.0f;
    float _speed = 1.0f;
    bool _playing = false;
    bool _loop = true;
};

} // namespace yetty::animation
