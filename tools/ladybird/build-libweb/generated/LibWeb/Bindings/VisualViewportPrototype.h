
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class VisualViewportPrototype : public JS::Object {
    JS_OBJECT(VisualViewportPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(VisualViewportPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit VisualViewportPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~VisualViewportPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(offset_left_getter);

    JS_DECLARE_NATIVE_FUNCTION(offset_top_getter);

    JS_DECLARE_NATIVE_FUNCTION(page_left_getter);

    JS_DECLARE_NATIVE_FUNCTION(page_top_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(scale_getter);

    JS_DECLARE_NATIVE_FUNCTION(onresize_getter);

    JS_DECLARE_NATIVE_FUNCTION(onresize_setter);

    JS_DECLARE_NATIVE_FUNCTION(onscroll_getter);

    JS_DECLARE_NATIVE_FUNCTION(onscroll_setter);

    JS_DECLARE_NATIVE_FUNCTION(onscrollend_getter);

    JS_DECLARE_NATIVE_FUNCTION(onscrollend_setter);


};


} // namespace Web::Bindings
    