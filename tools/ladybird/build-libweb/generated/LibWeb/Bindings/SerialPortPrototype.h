
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SerialPortPrototype : public JS::Object {
    JS_OBJECT(SerialPortPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SerialPortPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SerialPortPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SerialPortPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_info);
        
    JS_DECLARE_NATIVE_FUNCTION(open);
        
    JS_DECLARE_NATIVE_FUNCTION(set_signals);
        
    JS_DECLARE_NATIVE_FUNCTION(get_signals);
        
    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(forget);
        
    JS_DECLARE_NATIVE_FUNCTION(onconnect_getter);

    JS_DECLARE_NATIVE_FUNCTION(onconnect_setter);

    JS_DECLARE_NATIVE_FUNCTION(ondisconnect_getter);

    JS_DECLARE_NATIVE_FUNCTION(ondisconnect_setter);

    JS_DECLARE_NATIVE_FUNCTION(connected_getter);

    JS_DECLARE_NATIVE_FUNCTION(readable_getter);

    JS_DECLARE_NATIVE_FUNCTION(writable_getter);


};


enum class FlowControlType : u8 {

    None,

    Hardware,

};

inline String idl_enum_to_string(FlowControlType value)
{
    switch (value) {

    case FlowControlType::None:
        return "none"_string;

    case FlowControlType::Hardware:
        return "hardware"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class ParityType : u8 {

    None,

    Even,

    Odd,

};

inline String idl_enum_to_string(ParityType value)
{
    switch (value) {

    case ParityType::None:
        return "none"_string;

    case ParityType::Even:
        return "even"_string;

    case ParityType::Odd:
        return "odd"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    