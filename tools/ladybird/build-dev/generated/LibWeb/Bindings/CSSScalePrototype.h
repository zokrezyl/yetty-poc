
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSScalePrototype : public JS::Object {
    JS_OBJECT(CSSScalePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSScalePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSScalePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSScalePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(x_setter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_setter);

    JS_DECLARE_NATIVE_FUNCTION(z_getter);

    JS_DECLARE_NATIVE_FUNCTION(z_setter);


};


} // namespace Web::Bindings
    