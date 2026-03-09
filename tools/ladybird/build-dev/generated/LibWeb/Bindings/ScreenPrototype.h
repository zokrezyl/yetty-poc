
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ScreenPrototype : public JS::Object {
    JS_OBJECT(ScreenPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ScreenPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ScreenPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ScreenPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(avail_width_getter);

    JS_DECLARE_NATIVE_FUNCTION(avail_height_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(color_depth_getter);

    JS_DECLARE_NATIVE_FUNCTION(pixel_depth_getter);

    JS_DECLARE_NATIVE_FUNCTION(orientation_getter);

    JS_DECLARE_NATIVE_FUNCTION(is_extended_getter);

    JS_DECLARE_NATIVE_FUNCTION(onchange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onchange_setter);


};


} // namespace Web::Bindings
    