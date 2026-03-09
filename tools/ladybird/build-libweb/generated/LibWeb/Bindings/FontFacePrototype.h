
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class FontFacePrototype : public JS::Object {
    JS_OBJECT(FontFacePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(FontFacePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit FontFacePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~FontFacePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(load);
        
    JS_DECLARE_NATIVE_FUNCTION(family_getter);

    JS_DECLARE_NATIVE_FUNCTION(family_setter);

    JS_DECLARE_NATIVE_FUNCTION(style_getter);

    JS_DECLARE_NATIVE_FUNCTION(style_setter);

    JS_DECLARE_NATIVE_FUNCTION(weight_getter);

    JS_DECLARE_NATIVE_FUNCTION(weight_setter);

    JS_DECLARE_NATIVE_FUNCTION(stretch_getter);

    JS_DECLARE_NATIVE_FUNCTION(stretch_setter);

    JS_DECLARE_NATIVE_FUNCTION(unicode_range_getter);

    JS_DECLARE_NATIVE_FUNCTION(unicode_range_setter);

    JS_DECLARE_NATIVE_FUNCTION(feature_settings_getter);

    JS_DECLARE_NATIVE_FUNCTION(feature_settings_setter);

    JS_DECLARE_NATIVE_FUNCTION(variation_settings_getter);

    JS_DECLARE_NATIVE_FUNCTION(variation_settings_setter);

    JS_DECLARE_NATIVE_FUNCTION(display_getter);

    JS_DECLARE_NATIVE_FUNCTION(display_setter);

    JS_DECLARE_NATIVE_FUNCTION(ascent_override_getter);

    JS_DECLARE_NATIVE_FUNCTION(ascent_override_setter);

    JS_DECLARE_NATIVE_FUNCTION(descent_override_getter);

    JS_DECLARE_NATIVE_FUNCTION(descent_override_setter);

    JS_DECLARE_NATIVE_FUNCTION(line_gap_override_getter);

    JS_DECLARE_NATIVE_FUNCTION(line_gap_override_setter);

    JS_DECLARE_NATIVE_FUNCTION(status_getter);

    JS_DECLARE_NATIVE_FUNCTION(loaded_getter);


};


enum class FontFaceLoadStatus : u8 {

    Unloaded,

    Loading,

    Loaded,

    Error,

};

inline String idl_enum_to_string(FontFaceLoadStatus value)
{
    switch (value) {

    case FontFaceLoadStatus::Unloaded:
        return "unloaded"_string;

    case FontFaceLoadStatus::Loading:
        return "loading"_string;

    case FontFaceLoadStatus::Loaded:
        return "loaded"_string;

    case FontFaceLoadStatus::Error:
        return "error"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    