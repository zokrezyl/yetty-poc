
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class BiquadFilterNodePrototype : public JS::Object {
    JS_OBJECT(BiquadFilterNodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(BiquadFilterNodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit BiquadFilterNodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~BiquadFilterNodePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_frequency_response);
        
    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_setter);

    JS_DECLARE_NATIVE_FUNCTION(frequency_getter);

    JS_DECLARE_NATIVE_FUNCTION(detune_getter);

    JS_DECLARE_NATIVE_FUNCTION(q_getter);

    JS_DECLARE_NATIVE_FUNCTION(gain_getter);


};


enum class BiquadFilterType : u8 {

    Lowpass,

    Highpass,

    Bandpass,

    Lowshelf,

    Highshelf,

    Peaking,

    Notch,

    Allpass,

};

inline String idl_enum_to_string(BiquadFilterType value)
{
    switch (value) {

    case BiquadFilterType::Lowpass:
        return "lowpass"_string;

    case BiquadFilterType::Highpass:
        return "highpass"_string;

    case BiquadFilterType::Bandpass:
        return "bandpass"_string;

    case BiquadFilterType::Lowshelf:
        return "lowshelf"_string;

    case BiquadFilterType::Highshelf:
        return "highshelf"_string;

    case BiquadFilterType::Peaking:
        return "peaking"_string;

    case BiquadFilterType::Notch:
        return "notch"_string;

    case BiquadFilterType::Allpass:
        return "allpass"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    