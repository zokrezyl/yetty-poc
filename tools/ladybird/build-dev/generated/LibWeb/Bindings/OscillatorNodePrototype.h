
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class OscillatorNodePrototype : public JS::Object {
    JS_OBJECT(OscillatorNodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(OscillatorNodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit OscillatorNodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~OscillatorNodePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(set_periodic_wave);
        
    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_setter);

    JS_DECLARE_NATIVE_FUNCTION(frequency_getter);

    JS_DECLARE_NATIVE_FUNCTION(detune_getter);


};


enum class OscillatorType : u8 {

    Sine,

    Square,

    Sawtooth,

    Triangle,

    Custom,

};

inline String idl_enum_to_string(OscillatorType value)
{
    switch (value) {

    case OscillatorType::Sine:
        return "sine"_string;

    case OscillatorType::Square:
        return "square"_string;

    case OscillatorType::Sawtooth:
        return "sawtooth"_string;

    case OscillatorType::Triangle:
        return "triangle"_string;

    case OscillatorType::Custom:
        return "custom"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    