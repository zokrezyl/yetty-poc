
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSMathNegatePrototype : public JS::Object {
    JS_OBJECT(CSSMathNegatePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSMathNegatePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSMathNegatePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSMathNegatePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(value_getter);


};


} // namespace Web::Bindings
    