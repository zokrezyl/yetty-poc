
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSMathInvertPrototype : public JS::Object {
    JS_OBJECT(CSSMathInvertPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSMathInvertPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSMathInvertPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSMathInvertPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(value_getter);


};


} // namespace Web::Bindings
    