
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class GamepadPrototype : public JS::Object {
    JS_OBJECT(GamepadPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(GamepadPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit GamepadPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~GamepadPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(id_getter);

    JS_DECLARE_NATIVE_FUNCTION(index_getter);

    JS_DECLARE_NATIVE_FUNCTION(connected_getter);

    JS_DECLARE_NATIVE_FUNCTION(timestamp_getter);

    JS_DECLARE_NATIVE_FUNCTION(mapping_getter);

    JS_DECLARE_NATIVE_FUNCTION(axes_getter);

    JS_DECLARE_NATIVE_FUNCTION(buttons_getter);

    JS_DECLARE_NATIVE_FUNCTION(vibration_actuator_getter);


};


enum class GamepadMappingType : u8 {

    Empty,

    Standard,

    XrStandard,

};

inline String idl_enum_to_string(GamepadMappingType value)
{
    switch (value) {

    case GamepadMappingType::Empty:
        return ""_string;

    case GamepadMappingType::Standard:
        return "standard"_string;

    case GamepadMappingType::XrStandard:
        return "xr-standard"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    