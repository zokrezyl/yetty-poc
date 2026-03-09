
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AudioParamPrototype : public JS::Object {
    JS_OBJECT(AudioParamPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AudioParamPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AudioParamPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AudioParamPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(set_value_at_time);
        
    JS_DECLARE_NATIVE_FUNCTION(linear_ramp_to_value_at_time);
        
    JS_DECLARE_NATIVE_FUNCTION(exponential_ramp_to_value_at_time);
        
    JS_DECLARE_NATIVE_FUNCTION(set_target_at_time);
        
    JS_DECLARE_NATIVE_FUNCTION(set_value_curve_at_time);
        
    JS_DECLARE_NATIVE_FUNCTION(cancel_scheduled_values);
        
    JS_DECLARE_NATIVE_FUNCTION(cancel_and_hold_at_time);
        
    JS_DECLARE_NATIVE_FUNCTION(value_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_setter);

    JS_DECLARE_NATIVE_FUNCTION(automation_rate_getter);

    JS_DECLARE_NATIVE_FUNCTION(automation_rate_setter);

    JS_DECLARE_NATIVE_FUNCTION(default_value_getter);

    JS_DECLARE_NATIVE_FUNCTION(min_value_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_value_getter);


};


enum class AutomationRate : u8 {

    ARate,

    KRate,

};

inline String idl_enum_to_string(AutomationRate value)
{
    switch (value) {

    case AutomationRate::ARate:
        return "a-rate"_string;

    case AutomationRate::KRate:
        return "k-rate"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    