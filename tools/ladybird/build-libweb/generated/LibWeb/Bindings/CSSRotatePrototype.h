
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSRotatePrototype : public JS::Object {
    JS_OBJECT(CSSRotatePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSRotatePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSRotatePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSRotatePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(x_setter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_setter);

    JS_DECLARE_NATIVE_FUNCTION(z_getter);

    JS_DECLARE_NATIVE_FUNCTION(z_setter);

    JS_DECLARE_NATIVE_FUNCTION(angle_getter);

    JS_DECLARE_NATIVE_FUNCTION(angle_setter);


};


} // namespace Web::Bindings
    