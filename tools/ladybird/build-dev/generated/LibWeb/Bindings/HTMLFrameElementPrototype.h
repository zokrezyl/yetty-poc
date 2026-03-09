
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLFrameElementPrototype : public JS::Object {
    JS_OBJECT(HTMLFrameElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLFrameElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLFrameElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLFrameElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(scrolling_getter);

    JS_DECLARE_NATIVE_FUNCTION(scrolling_setter);

    JS_DECLARE_NATIVE_FUNCTION(src_getter);

    JS_DECLARE_NATIVE_FUNCTION(src_setter);

    JS_DECLARE_NATIVE_FUNCTION(frame_border_getter);

    JS_DECLARE_NATIVE_FUNCTION(frame_border_setter);

    JS_DECLARE_NATIVE_FUNCTION(long_desc_getter);

    JS_DECLARE_NATIVE_FUNCTION(long_desc_setter);

    JS_DECLARE_NATIVE_FUNCTION(no_resize_getter);

    JS_DECLARE_NATIVE_FUNCTION(no_resize_setter);

    JS_DECLARE_NATIVE_FUNCTION(content_document_getter);

    JS_DECLARE_NATIVE_FUNCTION(content_window_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_height_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_height_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_width_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_width_setter);


};


} // namespace Web::Bindings
    