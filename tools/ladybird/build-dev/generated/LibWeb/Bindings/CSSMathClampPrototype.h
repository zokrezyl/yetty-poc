
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSMathClampPrototype : public JS::Object {
    JS_OBJECT(CSSMathClampPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSMathClampPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSMathClampPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSMathClampPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(lower_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_getter);

    JS_DECLARE_NATIVE_FUNCTION(upper_getter);


};


} // namespace Web::Bindings
    