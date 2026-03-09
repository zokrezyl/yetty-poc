
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGEllipseElementPrototype : public JS::Object {
    JS_OBJECT(SVGEllipseElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGEllipseElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGEllipseElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGEllipseElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(cx_getter);

    JS_DECLARE_NATIVE_FUNCTION(cy_getter);

    JS_DECLARE_NATIVE_FUNCTION(rx_getter);

    JS_DECLARE_NATIVE_FUNCTION(ry_getter);


};


} // namespace Web::Bindings
    