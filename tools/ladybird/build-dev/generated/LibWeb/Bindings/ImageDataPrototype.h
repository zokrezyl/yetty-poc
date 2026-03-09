
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ImageDataPrototype : public JS::Object {
    JS_OBJECT(ImageDataPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ImageDataPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ImageDataPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ImageDataPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(data_getter);

    JS_DECLARE_NATIVE_FUNCTION(color_space_getter);


};


enum class PredefinedColorSpace : u8 {

    Srgb,

    DisplayP3,

};

inline String idl_enum_to_string(PredefinedColorSpace value)
{
    switch (value) {

    case PredefinedColorSpace::Srgb:
        return "srgb"_string;

    case PredefinedColorSpace::DisplayP3:
        return "display-p3"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    