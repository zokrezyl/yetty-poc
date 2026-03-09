
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class GamepadHapticActuatorPrototype : public JS::Object {
    JS_OBJECT(GamepadHapticActuatorPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(GamepadHapticActuatorPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit GamepadHapticActuatorPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~GamepadHapticActuatorPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(play_effect);
        
    JS_DECLARE_NATIVE_FUNCTION(reset);
        
    JS_DECLARE_NATIVE_FUNCTION(effects_getter);


};


enum class GamepadHapticEffectType : u8 {

    DualRumble,

    TriggerRumble,

};

inline String idl_enum_to_string(GamepadHapticEffectType value)
{
    switch (value) {

    case GamepadHapticEffectType::DualRumble:
        return "dual-rumble"_string;

    case GamepadHapticEffectType::TriggerRumble:
        return "trigger-rumble"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class GamepadHapticsResult : u8 {

    Complete,

    Preempted,

};

inline String idl_enum_to_string(GamepadHapticsResult value)
{
    switch (value) {

    case GamepadHapticsResult::Complete:
        return "complete"_string;

    case GamepadHapticsResult::Preempted:
        return "preempted"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    