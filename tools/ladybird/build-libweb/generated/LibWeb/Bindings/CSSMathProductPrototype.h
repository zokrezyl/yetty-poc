
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSMathProductPrototype : public JS::Object {
    JS_OBJECT(CSSMathProductPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSMathProductPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSMathProductPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSMathProductPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(values_getter);


};


} // namespace Web::Bindings
    