
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGTextPositioningElementPrototype : public JS::Object {
    JS_OBJECT(SVGTextPositioningElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGTextPositioningElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGTextPositioningElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGTextPositioningElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(dx_getter);

    JS_DECLARE_NATIVE_FUNCTION(dy_getter);

    JS_DECLARE_NATIVE_FUNCTION(rotate_getter);


};


} // namespace Web::Bindings
    