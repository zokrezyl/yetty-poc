
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSNestedDeclarationsPrototype : public JS::Object {
    JS_OBJECT(CSSNestedDeclarationsPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSNestedDeclarationsPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSNestedDeclarationsPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSNestedDeclarationsPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(style_getter);

    JS_DECLARE_NATIVE_FUNCTION(style_setter);


};


} // namespace Web::Bindings
    