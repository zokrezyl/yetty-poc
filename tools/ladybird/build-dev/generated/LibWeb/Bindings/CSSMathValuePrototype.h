
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSMathValuePrototype : public JS::Object {
    JS_OBJECT(CSSMathValuePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSMathValuePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSMathValuePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSMathValuePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(operator_getter);


};


enum class CSSMathOperator : u8 {

    Sum,

    Product,

    Negate,

    Invert,

    Min,

    Max,

    Clamp,

};

inline String idl_enum_to_string(CSSMathOperator value)
{
    switch (value) {

    case CSSMathOperator::Sum:
        return "sum"_string;

    case CSSMathOperator::Product:
        return "product"_string;

    case CSSMathOperator::Negate:
        return "negate"_string;

    case CSSMathOperator::Invert:
        return "invert"_string;

    case CSSMathOperator::Min:
        return "min"_string;

    case CSSMathOperator::Max:
        return "max"_string;

    case CSSMathOperator::Clamp:
        return "clamp"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    