
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGRadialGradientElementPrototype : public JS::Object {
    JS_OBJECT(SVGRadialGradientElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGRadialGradientElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGRadialGradientElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGRadialGradientElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(cx_getter);

    JS_DECLARE_NATIVE_FUNCTION(cy_getter);

    JS_DECLARE_NATIVE_FUNCTION(r_getter);

    JS_DECLARE_NATIVE_FUNCTION(fx_getter);

    JS_DECLARE_NATIVE_FUNCTION(fy_getter);

    JS_DECLARE_NATIVE_FUNCTION(fr_getter);


};


} // namespace Web::Bindings
    