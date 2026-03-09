
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGFECompositeElementPrototype : public JS::Object {
    JS_OBJECT(SVGFECompositeElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGFECompositeElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGFECompositeElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGFECompositeElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(in1_getter);

    JS_DECLARE_NATIVE_FUNCTION(in2_getter);

    JS_DECLARE_NATIVE_FUNCTION(operator_getter);

    JS_DECLARE_NATIVE_FUNCTION(k2_getter);

    JS_DECLARE_NATIVE_FUNCTION(k3_getter);

    JS_DECLARE_NATIVE_FUNCTION(k1_getter);

    JS_DECLARE_NATIVE_FUNCTION(k4_getter);

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(result_getter);


};


} // namespace Web::Bindings
    