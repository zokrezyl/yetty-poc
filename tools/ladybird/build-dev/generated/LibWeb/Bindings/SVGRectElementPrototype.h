
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGRectElementPrototype : public JS::Object {
    JS_OBJECT(SVGRectElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGRectElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGRectElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGRectElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(rx_getter);

    JS_DECLARE_NATIVE_FUNCTION(ry_getter);


};


} // namespace Web::Bindings
    