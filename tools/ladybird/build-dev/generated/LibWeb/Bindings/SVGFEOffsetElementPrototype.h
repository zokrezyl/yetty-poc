
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGFEOffsetElementPrototype : public JS::Object {
    JS_OBJECT(SVGFEOffsetElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGFEOffsetElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGFEOffsetElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGFEOffsetElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(in1_getter);

    JS_DECLARE_NATIVE_FUNCTION(dx_getter);

    JS_DECLARE_NATIVE_FUNCTION(dy_getter);

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(result_getter);


};


} // namespace Web::Bindings
    