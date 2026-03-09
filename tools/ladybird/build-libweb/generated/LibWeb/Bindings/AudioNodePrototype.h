
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AudioNodePrototype : public JS::Object {
    JS_OBJECT(AudioNodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AudioNodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AudioNodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AudioNodePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(connect);
        
    JS_DECLARE_NATIVE_FUNCTION(connect0);

    JS_DECLARE_NATIVE_FUNCTION(connect1);

    JS_DECLARE_NATIVE_FUNCTION(disconnect);
        
    JS_DECLARE_NATIVE_FUNCTION(disconnect0);

    JS_DECLARE_NATIVE_FUNCTION(disconnect1);

    JS_DECLARE_NATIVE_FUNCTION(disconnect2);

    JS_DECLARE_NATIVE_FUNCTION(disconnect3);

    JS_DECLARE_NATIVE_FUNCTION(disconnect4);

    JS_DECLARE_NATIVE_FUNCTION(disconnect5);

    JS_DECLARE_NATIVE_FUNCTION(disconnect6);

    JS_DECLARE_NATIVE_FUNCTION(context_getter);

    JS_DECLARE_NATIVE_FUNCTION(number_of_inputs_getter);

    JS_DECLARE_NATIVE_FUNCTION(number_of_outputs_getter);

    JS_DECLARE_NATIVE_FUNCTION(channel_count_getter);

    JS_DECLARE_NATIVE_FUNCTION(channel_count_setter);

    JS_DECLARE_NATIVE_FUNCTION(channel_count_mode_getter);

    JS_DECLARE_NATIVE_FUNCTION(channel_count_mode_setter);

    JS_DECLARE_NATIVE_FUNCTION(channel_interpretation_getter);

    JS_DECLARE_NATIVE_FUNCTION(channel_interpretation_setter);


};


enum class ChannelInterpretation : u8 {

    Speakers,

    Discrete,

};

inline String idl_enum_to_string(ChannelInterpretation value)
{
    switch (value) {

    case ChannelInterpretation::Speakers:
        return "speakers"_string;

    case ChannelInterpretation::Discrete:
        return "discrete"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class ChannelCountMode : u8 {

    Max,

    ClampedMax,

    Explicit,

};

inline String idl_enum_to_string(ChannelCountMode value)
{
    switch (value) {

    case ChannelCountMode::Max:
        return "max"_string;

    case ChannelCountMode::ClampedMax:
        return "clamped-max"_string;

    case ChannelCountMode::Explicit:
        return "explicit"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    