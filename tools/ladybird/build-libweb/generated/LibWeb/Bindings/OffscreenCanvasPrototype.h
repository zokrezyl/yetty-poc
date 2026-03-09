
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class OffscreenCanvasPrototype : public JS::Object {
    JS_OBJECT(OffscreenCanvasPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(OffscreenCanvasPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit OffscreenCanvasPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~OffscreenCanvasPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_context);
        
    JS_DECLARE_NATIVE_FUNCTION(transfer_to_image_bitmap);
        
    JS_DECLARE_NATIVE_FUNCTION(convert_to_blob);
        
    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_setter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_setter);

    JS_DECLARE_NATIVE_FUNCTION(oncontextlost_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncontextlost_setter);

    JS_DECLARE_NATIVE_FUNCTION(oncontextrestored_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncontextrestored_setter);


};


enum class OffscreenRenderingContextId : u8 {

    _2d,

    Bitmaprenderer,

    Webgl,

    Webgl2,

    Webgpu,

};

inline String idl_enum_to_string(OffscreenRenderingContextId value)
{
    switch (value) {

    case OffscreenRenderingContextId::_2d:
        return "2d"_string;

    case OffscreenRenderingContextId::Bitmaprenderer:
        return "bitmaprenderer"_string;

    case OffscreenRenderingContextId::Webgl:
        return "webgl"_string;

    case OffscreenRenderingContextId::Webgl2:
        return "webgl2"_string;

    case OffscreenRenderingContextId::Webgpu:
        return "webgpu"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    