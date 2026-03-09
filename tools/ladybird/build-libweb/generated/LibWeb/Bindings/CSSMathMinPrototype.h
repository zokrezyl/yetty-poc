
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSMathMinPrototype : public JS::Object {
    JS_OBJECT(CSSMathMinPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSMathMinPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSMathMinPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSMathMinPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(values_getter);


};


} // namespace Web::Bindings
    