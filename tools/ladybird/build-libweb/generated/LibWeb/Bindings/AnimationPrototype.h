
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AnimationPrototype : public JS::Object {
    JS_OBJECT(AnimationPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AnimationPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AnimationPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AnimationPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(cancel);
        
    JS_DECLARE_NATIVE_FUNCTION(finish);
        
    JS_DECLARE_NATIVE_FUNCTION(play);
        
    JS_DECLARE_NATIVE_FUNCTION(pause);
        
    JS_DECLARE_NATIVE_FUNCTION(update_playback_rate);
        
    JS_DECLARE_NATIVE_FUNCTION(reverse);
        
    JS_DECLARE_NATIVE_FUNCTION(persist);
        
    JS_DECLARE_NATIVE_FUNCTION(id_getter);

    JS_DECLARE_NATIVE_FUNCTION(id_setter);

    JS_DECLARE_NATIVE_FUNCTION(effect_getter);

    JS_DECLARE_NATIVE_FUNCTION(effect_setter);

    JS_DECLARE_NATIVE_FUNCTION(timeline_getter);

    JS_DECLARE_NATIVE_FUNCTION(timeline_setter);

    JS_DECLARE_NATIVE_FUNCTION(start_time_getter);

    JS_DECLARE_NATIVE_FUNCTION(start_time_setter);

    JS_DECLARE_NATIVE_FUNCTION(current_time_getter);

    JS_DECLARE_NATIVE_FUNCTION(current_time_setter);

    JS_DECLARE_NATIVE_FUNCTION(playback_rate_getter);

    JS_DECLARE_NATIVE_FUNCTION(playback_rate_setter);

    JS_DECLARE_NATIVE_FUNCTION(play_state_getter);

    JS_DECLARE_NATIVE_FUNCTION(replace_state_getter);

    JS_DECLARE_NATIVE_FUNCTION(pending_getter);

    JS_DECLARE_NATIVE_FUNCTION(ready_getter);

    JS_DECLARE_NATIVE_FUNCTION(finished_getter);

    JS_DECLARE_NATIVE_FUNCTION(onfinish_getter);

    JS_DECLARE_NATIVE_FUNCTION(onfinish_setter);

    JS_DECLARE_NATIVE_FUNCTION(oncancel_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncancel_setter);

    JS_DECLARE_NATIVE_FUNCTION(onremove_getter);

    JS_DECLARE_NATIVE_FUNCTION(onremove_setter);


};


enum class AnimationReplaceState : u8 {

    Active,

    Removed,

    Persisted,

};

inline String idl_enum_to_string(AnimationReplaceState value)
{
    switch (value) {

    case AnimationReplaceState::Active:
        return "active"_string;

    case AnimationReplaceState::Removed:
        return "removed"_string;

    case AnimationReplaceState::Persisted:
        return "persisted"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class AnimationPlayState : u8 {

    Idle,

    Running,

    Paused,

    Finished,

};

inline String idl_enum_to_string(AnimationPlayState value)
{
    switch (value) {

    case AnimationPlayState::Idle:
        return "idle"_string;

    case AnimationPlayState::Running:
        return "running"_string;

    case AnimationPlayState::Paused:
        return "paused"_string;

    case AnimationPlayState::Finished:
        return "finished"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    