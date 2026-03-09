
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGFEBlendElementPrototype : public JS::Object {
    JS_OBJECT(SVGFEBlendElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGFEBlendElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGFEBlendElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGFEBlendElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(in1_getter);

    JS_DECLARE_NATIVE_FUNCTION(in2_getter);

    JS_DECLARE_NATIVE_FUNCTION(mode_getter);

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(result_getter);


};


} // namespace Web::Bindings
    