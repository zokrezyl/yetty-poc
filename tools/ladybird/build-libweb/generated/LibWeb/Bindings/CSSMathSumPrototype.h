
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSMathSumPrototype : public JS::Object {
    JS_OBJECT(CSSMathSumPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSMathSumPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSMathSumPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSMathSumPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(values_getter);


};


} // namespace Web::Bindings
    