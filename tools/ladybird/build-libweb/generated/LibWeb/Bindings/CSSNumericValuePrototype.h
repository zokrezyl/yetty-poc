
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSNumericValuePrototype : public JS::Object {
    JS_OBJECT(CSSNumericValuePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSNumericValuePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSNumericValuePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSNumericValuePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(add);
        
    JS_DECLARE_NATIVE_FUNCTION(sub);
        
    JS_DECLARE_NATIVE_FUNCTION(mul);
        
    JS_DECLARE_NATIVE_FUNCTION(div);
        
    JS_DECLARE_NATIVE_FUNCTION(min);
        
    JS_DECLARE_NATIVE_FUNCTION(max);
        
    JS_DECLARE_NATIVE_FUNCTION(equals);
        
    JS_DECLARE_NATIVE_FUNCTION(to);
        
    JS_DECLARE_NATIVE_FUNCTION(type);
        

};


enum class CSSNumericBaseType : u8 {

    Length,

    Angle,

    Time,

    Frequency,

    Resolution,

    Flex,

    Percent,

};

inline String idl_enum_to_string(CSSNumericBaseType value)
{
    switch (value) {

    case CSSNumericBaseType::Length:
        return "length"_string;

    case CSSNumericBaseType::Angle:
        return "angle"_string;

    case CSSNumericBaseType::Time:
        return "time"_string;

    case CSSNumericBaseType::Frequency:
        return "frequency"_string;

    case CSSNumericBaseType::Resolution:
        return "resolution"_string;

    case CSSNumericBaseType::Flex:
        return "flex"_string;

    case CSSNumericBaseType::Percent:
        return "percent"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    