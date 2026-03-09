
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ImageBitmapPrototype : public JS::Object {
    JS_OBJECT(ImageBitmapPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ImageBitmapPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ImageBitmapPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ImageBitmapPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);


};


enum class PremultiplyAlpha : u8 {

    None,

    Premultiply,

    Default,

};

inline String idl_enum_to_string(PremultiplyAlpha value)
{
    switch (value) {

    case PremultiplyAlpha::None:
        return "none"_string;

    case PremultiplyAlpha::Premultiply:
        return "premultiply"_string;

    case PremultiplyAlpha::Default:
        return "default"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class ImageOrientation : u8 {

    FromImage,

    Flipy,

};

inline String idl_enum_to_string(ImageOrientation value)
{
    switch (value) {

    case ImageOrientation::FromImage:
        return "from-image"_string;

    case ImageOrientation::Flipy:
        return "flipY"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class ResizeQuality : u8 {

    Pixelated,

    Low,

    Medium,

    High,

};

inline String idl_enum_to_string(ResizeQuality value)
{
    switch (value) {

    case ResizeQuality::Pixelated:
        return "pixelated"_string;

    case ResizeQuality::Low:
        return "low"_string;

    case ResizeQuality::Medium:
        return "medium"_string;

    case ResizeQuality::High:
        return "high"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class ColorSpaceConversion : u8 {

    None,

    Default,

};

inline String idl_enum_to_string(ColorSpaceConversion value)
{
    switch (value) {

    case ColorSpaceConversion::None:
        return "none"_string;

    case ColorSpaceConversion::Default:
        return "default"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    