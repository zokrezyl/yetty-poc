
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSFontFaceDescriptorsPrototype : public JS::Object {
    JS_OBJECT(CSSFontFaceDescriptorsPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSFontFaceDescriptorsPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSFontFaceDescriptorsPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSFontFaceDescriptorsPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(src_getter);

    JS_DECLARE_NATIVE_FUNCTION(src_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_family_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_family_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_family_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_family_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_weight_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_weight_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_weight_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_weight_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_stretch_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_stretch_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_stretch_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_stretch_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(unicode_range_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(unicode_range_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(unicode_range_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(unicode_range_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_feature_settings_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_feature_settings_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_feature_settings_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_feature_settings_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variation_settings_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variation_settings_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variation_settings_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variation_settings_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_named_instance_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_named_instance_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_named_instance_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_named_instance_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_display_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_display_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_display_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_display_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_language_override_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_language_override_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_language_override_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_language_override_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(ascent_override_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(ascent_override_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(ascent_override_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(ascent_override_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(descent_override_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(descent_override_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(descent_override_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(descent_override_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(line_gap_override_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(line_gap_override_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(line_gap_override_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(line_gap_override_dashed_setter);


};


} // namespace Web::Bindings
    