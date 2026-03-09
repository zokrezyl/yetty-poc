
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class GlobalPrototype : public JS::Object {
    JS_OBJECT(GlobalPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(GlobalPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit GlobalPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~GlobalPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(value_of);
        
    JS_DECLARE_NATIVE_FUNCTION(value_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_setter);


};


enum class ValueType : u8 {

    I32,

    I64,

    F32,

    F64,

    V128,

    Externref,

    Anyfunc,

};

inline String idl_enum_to_string(ValueType value)
{
    switch (value) {

    case ValueType::I32:
        return "i32"_string;

    case ValueType::I64:
        return "i64"_string;

    case ValueType::F32:
        return "f32"_string;

    case ValueType::F64:
        return "f64"_string;

    case ValueType::V128:
        return "v128"_string;

    case ValueType::Externref:
        return "externref"_string;

    case ValueType::Anyfunc:
        return "anyfunc"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    