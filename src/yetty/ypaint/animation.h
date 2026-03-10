#pragma once

#include "ydraw-buffer.h"
#include <yetty/base/object.h>
#include <yetty/base/factory.h>
#include <cstdint>
#include <vector>

namespace yetty::animation {

//=============================================================================
// Animatable property types
//=============================================================================
enum class PropertyType : uint8_t {
    Position = 0,       // geometry center (x, y) — absolute
    Scale = 1,          // uniform scale multiplier on size params
    FillOpacity = 2,    // alpha of fillColor (0.0–1.0)
    FillColor = 3,      // RGBA as 4 floats (0.0–1.0 each)
    StrokeOpacity = 4,  // alpha of strokeColor (0.0–1.0)
    StrokeWidth = 5,    // strokeWidth field — absolute
    Radius = 6,         // radius param — absolute
};

//=============================================================================
// Keyframe — single value at a point in time
//=============================================================================
struct Keyframe {
    float time;              // seconds
    float value[4];          // up to 4 components
    uint8_t componentCount;  // 1–4
};

//=============================================================================
// AnimatedProperty — a sequence of keyframes for one property of one primitive
//=============================================================================
struct AnimatedProperty {
    PropertyType type;
    uint32_t primitiveId;     // YDrawBuffer prim ID
    std::vector<Keyframe> keyframes;  // sorted by time ascending

    // Evaluate at given time via linear interpolation.
    uint8_t evaluate(float time, float* out) const;
};

//=============================================================================
// Animation — playback controller + property collection
//
// Created with a shared_ptr to YDrawBuffer. On snapshotBase(), stores raw
// word data for each animated prim. On apply(), computes animated values
// from the base snapshot and calls updatePrim() on the buffer.
// Then the builder's calculate() rebuilds the grid.
//=============================================================================
class Animation : public base::Object,
                  public base::ObjectFactory<Animation> {
public:
    using Ptr = base::ObjectFactory<Animation>::Ptr;

    static Result<Ptr> createImpl(std::shared_ptr<YDrawBuffer> buffer);

    ~Animation() override = default;
    const char* typeName() const override { return "Animation"; }

    // Playback control
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void seek(float timeSeconds) = 0;

    // Configuration
    virtual void setLoop(bool v) = 0;
    virtual void setSpeed(float s) = 0;
    virtual void setDuration(float d) = 0;

    // State queries
    virtual bool isPlaying() const = 0;
    virtual float currentTime() const = 0;
    virtual float duration() const = 0;
    virtual bool hasProperties() const = 0;

    // Property management
    virtual void addProperty(AnimatedProperty&& p) = 0;

    // Advance time by deltaTime * speed.  Returns true if time changed.
    virtual bool advance(float deltaTime) = 0;

    // Snapshot current buffer state for all animated prims as base data.
    // Call once before starting playback.
    virtual void snapshotBase() = 0;

    // Apply animation at current time — modifies buffer prims in place
    // via updatePrim(). Call after advance() returns true.
    virtual void apply() = 0;

    // Expand bounding box to encompass all keyframe positions/scales.
    virtual void expandBounds(float& minX, float& minY,
                              float& maxX, float& maxY) const = 0;

protected:
    Animation() = default;
};

} // namespace yetty::animation
