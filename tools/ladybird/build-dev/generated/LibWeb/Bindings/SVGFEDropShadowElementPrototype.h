
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGFEDropShadowElementPrototype : public JS::Object {
    JS_OBJECT(SVGFEDropShadowElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGFEDropShadowElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGFEDropShadowElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGFEDropShadowElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(set_std_deviation);
        
    JS_DECLARE_NATIVE_FUNCTION(in1_getter);

    JS_DECLARE_NATIVE_FUNCTION(dx_getter);

    JS_DECLARE_NATIVE_FUNCTION(dy_getter);

    JS_DECLARE_NATIVE_FUNCTION(std_deviation_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(std_deviation_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(result_getter);


};


} // namespace Web::Bindings
    