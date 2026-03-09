
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGFETurbulenceElementPrototype : public JS::Object {
    JS_OBJECT(SVGFETurbulenceElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGFETurbulenceElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGFETurbulenceElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGFETurbulenceElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(base_frequency_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(base_frequency_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(num_octaves_getter);

    JS_DECLARE_NATIVE_FUNCTION(seed_getter);

    JS_DECLARE_NATIVE_FUNCTION(stitch_tiles_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(result_getter);


};


} // namespace Web::Bindings
    