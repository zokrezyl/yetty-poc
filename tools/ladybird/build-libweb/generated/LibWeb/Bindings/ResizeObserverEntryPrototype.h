
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ResizeObserverEntryPrototype : public JS::Object {
    JS_OBJECT(ResizeObserverEntryPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ResizeObserverEntryPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ResizeObserverEntryPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ResizeObserverEntryPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(target_getter);

    JS_DECLARE_NATIVE_FUNCTION(content_rect_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_box_size_getter);

    JS_DECLARE_NATIVE_FUNCTION(content_box_size_getter);

    JS_DECLARE_NATIVE_FUNCTION(device_pixel_content_box_size_getter);


};


} // namespace Web::Bindings
    