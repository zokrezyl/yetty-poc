
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGImageElementPrototype : public JS::Object {
    JS_OBJECT(SVGImageElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGImageElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGImageElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGImageElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(cross_origin_getter);

    JS_DECLARE_NATIVE_FUNCTION(cross_origin_setter);

    JS_DECLARE_NATIVE_FUNCTION(href_getter);


};


} // namespace Web::Bindings
    