
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGFilterElementPrototype : public JS::Object {
    JS_OBJECT(SVGFilterElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGFilterElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGFilterElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGFilterElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(filter_units_getter);

    JS_DECLARE_NATIVE_FUNCTION(primitive_units_getter);

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(href_getter);


};


} // namespace Web::Bindings
    