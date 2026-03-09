
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSMathMaxPrototype : public JS::Object {
    JS_OBJECT(CSSMathMaxPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSMathMaxPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSMathMaxPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSMathMaxPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(values_getter);


};


} // namespace Web::Bindings
    