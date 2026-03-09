
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AnimationEffectPrototype : public JS::Object {
    JS_OBJECT(AnimationEffectPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AnimationEffectPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AnimationEffectPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AnimationEffectPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_timing);
        
    JS_DECLARE_NATIVE_FUNCTION(get_computed_timing);
        
    JS_DECLARE_NATIVE_FUNCTION(update_timing);
        

};


enum class FillMode : u8 {

    None,

    Forwards,

    Backwards,

    Both,

    Auto,

};

inline String idl_enum_to_string(FillMode value)
{
    switch (value) {

    case FillMode::None:
        return "none"_string;

    case FillMode::Forwards:
        return "forwards"_string;

    case FillMode::Backwards:
        return "backwards"_string;

    case FillMode::Both:
        return "both"_string;

    case FillMode::Auto:
        return "auto"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class PlaybackDirection : u8 {

    Normal,

    Reverse,

    Alternate,

    AlternateReverse,

};

inline String idl_enum_to_string(PlaybackDirection value)
{
    switch (value) {

    case PlaybackDirection::Normal:
        return "normal"_string;

    case PlaybackDirection::Reverse:
        return "reverse"_string;

    case PlaybackDirection::Alternate:
        return "alternate"_string;

    case PlaybackDirection::AlternateReverse:
        return "alternate-reverse"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    