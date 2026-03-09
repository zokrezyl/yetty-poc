
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CanvasRenderingContext2DPrototype : public JS::Object {
    JS_OBJECT(CanvasRenderingContext2DPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CanvasRenderingContext2DPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CanvasRenderingContext2DPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CanvasRenderingContext2DPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(create_linear_gradient);
        
    JS_DECLARE_NATIVE_FUNCTION(create_radial_gradient);
        
    JS_DECLARE_NATIVE_FUNCTION(create_conic_gradient);
        
    JS_DECLARE_NATIVE_FUNCTION(create_pattern);
        
    JS_DECLARE_NATIVE_FUNCTION(get_context_attributes);
        
    JS_DECLARE_NATIVE_FUNCTION(set_line_dash);
        
    JS_DECLARE_NATIVE_FUNCTION(get_line_dash);
        
    JS_DECLARE_NATIVE_FUNCTION(create_image_data);
        
    JS_DECLARE_NATIVE_FUNCTION(create_image_data0);

    JS_DECLARE_NATIVE_FUNCTION(create_image_data1);

    JS_DECLARE_NATIVE_FUNCTION(get_image_data);
        
    JS_DECLARE_NATIVE_FUNCTION(put_image_data);
        
    JS_DECLARE_NATIVE_FUNCTION(put_image_data0);

    JS_DECLARE_NATIVE_FUNCTION(put_image_data1);

    JS_DECLARE_NATIVE_FUNCTION(scale);
        
    JS_DECLARE_NATIVE_FUNCTION(rotate);
        
    JS_DECLARE_NATIVE_FUNCTION(translate);
        
    JS_DECLARE_NATIVE_FUNCTION(transform);
        
    JS_DECLARE_NATIVE_FUNCTION(get_transform);
        
    JS_DECLARE_NATIVE_FUNCTION(set_transform);
        
    JS_DECLARE_NATIVE_FUNCTION(set_transform0);

    JS_DECLARE_NATIVE_FUNCTION(set_transform1);

    JS_DECLARE_NATIVE_FUNCTION(reset_transform);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_rect);
        
    JS_DECLARE_NATIVE_FUNCTION(fill_rect);
        
    JS_DECLARE_NATIVE_FUNCTION(stroke_rect);
        
    JS_DECLARE_NATIVE_FUNCTION(draw_image);
        
    JS_DECLARE_NATIVE_FUNCTION(draw_image0);

    JS_DECLARE_NATIVE_FUNCTION(draw_image1);

    JS_DECLARE_NATIVE_FUNCTION(draw_image2);

    JS_DECLARE_NATIVE_FUNCTION(save);
        
    JS_DECLARE_NATIVE_FUNCTION(restore);
        
    JS_DECLARE_NATIVE_FUNCTION(reset);
        
    JS_DECLARE_NATIVE_FUNCTION(is_context_lost);
        
    JS_DECLARE_NATIVE_FUNCTION(close_path);
        
    JS_DECLARE_NATIVE_FUNCTION(move_to);
        
    JS_DECLARE_NATIVE_FUNCTION(line_to);
        
    JS_DECLARE_NATIVE_FUNCTION(quadratic_curve_to);
        
    JS_DECLARE_NATIVE_FUNCTION(bezier_curve_to);
        
    JS_DECLARE_NATIVE_FUNCTION(arc_to);
        
    JS_DECLARE_NATIVE_FUNCTION(rect);
        
    JS_DECLARE_NATIVE_FUNCTION(round_rect);
        
    JS_DECLARE_NATIVE_FUNCTION(arc);
        
    JS_DECLARE_NATIVE_FUNCTION(ellipse);
        
    JS_DECLARE_NATIVE_FUNCTION(fill_text);
        
    JS_DECLARE_NATIVE_FUNCTION(stroke_text);
        
    JS_DECLARE_NATIVE_FUNCTION(measure_text);
        
    JS_DECLARE_NATIVE_FUNCTION(begin_path);
        
    JS_DECLARE_NATIVE_FUNCTION(fill);
        
    JS_DECLARE_NATIVE_FUNCTION(fill0);

    JS_DECLARE_NATIVE_FUNCTION(fill1);

    JS_DECLARE_NATIVE_FUNCTION(stroke);
        
    JS_DECLARE_NATIVE_FUNCTION(stroke0);

    JS_DECLARE_NATIVE_FUNCTION(stroke1);

    JS_DECLARE_NATIVE_FUNCTION(clip);
        
    JS_DECLARE_NATIVE_FUNCTION(clip0);

    JS_DECLARE_NATIVE_FUNCTION(clip1);

    JS_DECLARE_NATIVE_FUNCTION(is_point_in_path);
        
    JS_DECLARE_NATIVE_FUNCTION(is_point_in_path0);

    JS_DECLARE_NATIVE_FUNCTION(is_point_in_path1);

    JS_DECLARE_NATIVE_FUNCTION(canvas_getter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_style_getter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_style_setter);

    JS_DECLARE_NATIVE_FUNCTION(fill_style_getter);

    JS_DECLARE_NATIVE_FUNCTION(fill_style_setter);

    JS_DECLARE_NATIVE_FUNCTION(line_width_getter);

    JS_DECLARE_NATIVE_FUNCTION(line_width_setter);

    JS_DECLARE_NATIVE_FUNCTION(line_cap_getter);

    JS_DECLARE_NATIVE_FUNCTION(line_cap_setter);

    JS_DECLARE_NATIVE_FUNCTION(line_join_getter);

    JS_DECLARE_NATIVE_FUNCTION(line_join_setter);

    JS_DECLARE_NATIVE_FUNCTION(miter_limit_getter);

    JS_DECLARE_NATIVE_FUNCTION(miter_limit_setter);

    JS_DECLARE_NATIVE_FUNCTION(line_dash_offset_getter);

    JS_DECLARE_NATIVE_FUNCTION(line_dash_offset_setter);

    JS_DECLARE_NATIVE_FUNCTION(global_alpha_getter);

    JS_DECLARE_NATIVE_FUNCTION(global_alpha_setter);

    JS_DECLARE_NATIVE_FUNCTION(global_composite_operation_getter);

    JS_DECLARE_NATIVE_FUNCTION(global_composite_operation_setter);

    JS_DECLARE_NATIVE_FUNCTION(filter_getter);

    JS_DECLARE_NATIVE_FUNCTION(filter_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_align_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_align_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_baseline_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_baseline_setter);

    JS_DECLARE_NATIVE_FUNCTION(direction_getter);

    JS_DECLARE_NATIVE_FUNCTION(direction_setter);

    JS_DECLARE_NATIVE_FUNCTION(shadow_offset_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(shadow_offset_x_setter);

    JS_DECLARE_NATIVE_FUNCTION(shadow_offset_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(shadow_offset_y_setter);

    JS_DECLARE_NATIVE_FUNCTION(shadow_blur_getter);

    JS_DECLARE_NATIVE_FUNCTION(shadow_blur_setter);

    JS_DECLARE_NATIVE_FUNCTION(shadow_color_getter);

    JS_DECLARE_NATIVE_FUNCTION(shadow_color_setter);

    JS_DECLARE_NATIVE_FUNCTION(image_smoothing_enabled_getter);

    JS_DECLARE_NATIVE_FUNCTION(image_smoothing_enabled_setter);

    JS_DECLARE_NATIVE_FUNCTION(image_smoothing_quality_getter);

    JS_DECLARE_NATIVE_FUNCTION(image_smoothing_quality_setter);


};


enum class ImageSmoothingQuality : u8 {

    Low,

    Medium,

    High,

};

inline String idl_enum_to_string(ImageSmoothingQuality value)
{
    switch (value) {

    case ImageSmoothingQuality::Low:
        return "low"_string;

    case ImageSmoothingQuality::Medium:
        return "medium"_string;

    case ImageSmoothingQuality::High:
        return "high"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class CanvasTextBaseline : u8 {

    Top,

    Hanging,

    Middle,

    Alphabetic,

    Ideographic,

    Bottom,

};

inline String idl_enum_to_string(CanvasTextBaseline value)
{
    switch (value) {

    case CanvasTextBaseline::Top:
        return "top"_string;

    case CanvasTextBaseline::Hanging:
        return "hanging"_string;

    case CanvasTextBaseline::Middle:
        return "middle"_string;

    case CanvasTextBaseline::Alphabetic:
        return "alphabetic"_string;

    case CanvasTextBaseline::Ideographic:
        return "ideographic"_string;

    case CanvasTextBaseline::Bottom:
        return "bottom"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class CanvasColorType : u8 {

    Unorm8,

    Float16,

};

inline String idl_enum_to_string(CanvasColorType value)
{
    switch (value) {

    case CanvasColorType::Unorm8:
        return "unorm8"_string;

    case CanvasColorType::Float16:
        return "float16"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class CanvasDirection : u8 {

    Ltr,

    Rtl,

    Inherit,

};

inline String idl_enum_to_string(CanvasDirection value)
{
    switch (value) {

    case CanvasDirection::Ltr:
        return "ltr"_string;

    case CanvasDirection::Rtl:
        return "rtl"_string;

    case CanvasDirection::Inherit:
        return "inherit"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class CanvasLineCap : u8 {

    Butt,

    Round,

    Square,

};

inline String idl_enum_to_string(CanvasLineCap value)
{
    switch (value) {

    case CanvasLineCap::Butt:
        return "butt"_string;

    case CanvasLineCap::Round:
        return "round"_string;

    case CanvasLineCap::Square:
        return "square"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class CanvasLineJoin : u8 {

    Round,

    Bevel,

    Miter,

};

inline String idl_enum_to_string(CanvasLineJoin value)
{
    switch (value) {

    case CanvasLineJoin::Round:
        return "round"_string;

    case CanvasLineJoin::Bevel:
        return "bevel"_string;

    case CanvasLineJoin::Miter:
        return "miter"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class CanvasTextAlign : u8 {

    Start,

    End,

    Left,

    Right,

    Center,

};

inline String idl_enum_to_string(CanvasTextAlign value)
{
    switch (value) {

    case CanvasTextAlign::Start:
        return "start"_string;

    case CanvasTextAlign::End:
        return "end"_string;

    case CanvasTextAlign::Left:
        return "left"_string;

    case CanvasTextAlign::Right:
        return "right"_string;

    case CanvasTextAlign::Center:
        return "center"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    