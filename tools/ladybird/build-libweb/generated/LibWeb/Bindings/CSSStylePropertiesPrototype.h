
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSStylePropertiesPrototype : public JS::Object {
    JS_OBJECT(CSSStylePropertiesPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSStylePropertiesPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSStylePropertiesPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSStylePropertiesPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(css_float_getter);

    JS_DECLARE_NATIVE_FUNCTION(css_float_setter);

    JS_DECLARE_NATIVE_FUNCTION(moz_appearance_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(moz_appearance_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(moz_appearance_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(moz_appearance_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_content_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_content_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_content_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_content_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_content_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_content_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_items_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_items_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_items_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_items_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_items_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_items_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_self_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_self_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_self_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_self_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_self_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_align_self_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_delay_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_delay_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_delay_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_delay_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_delay_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_delay_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_direction_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_direction_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_direction_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_direction_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_direction_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_direction_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_duration_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_duration_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_duration_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_duration_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_duration_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_duration_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_fill_mode_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_fill_mode_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_fill_mode_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_fill_mode_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_fill_mode_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_fill_mode_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_iteration_count_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_iteration_count_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_iteration_count_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_iteration_count_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_iteration_count_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_iteration_count_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_name_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_name_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_name_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_name_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_name_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_name_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_play_state_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_play_state_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_play_state_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_play_state_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_play_state_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_play_state_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_timing_function_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_timing_function_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_timing_function_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_timing_function_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_timing_function_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_animation_timing_function_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_appearance_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_appearance_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_appearance_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_appearance_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_appearance_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_appearance_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_clip_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_clip_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_clip_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_clip_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_clip_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_clip_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_origin_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_origin_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_origin_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_origin_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_origin_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_origin_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_size_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_size_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_size_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_size_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_size_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_background_size_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_bottom_left_radius_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_bottom_left_radius_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_bottom_left_radius_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_bottom_left_radius_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_bottom_left_radius_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_bottom_left_radius_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_bottom_right_radius_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_bottom_right_radius_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_bottom_right_radius_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_bottom_right_radius_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_bottom_right_radius_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_bottom_right_radius_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_radius_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_radius_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_radius_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_radius_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_radius_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_radius_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_top_left_radius_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_top_left_radius_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_top_left_radius_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_top_left_radius_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_top_left_radius_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_top_left_radius_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_top_right_radius_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_top_right_radius_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_top_right_radius_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_top_right_radius_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_top_right_radius_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_border_top_right_radius_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_align_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_align_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_align_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_align_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_align_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_align_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_flex_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_flex_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_flex_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_flex_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_flex_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_flex_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_ordinal_group_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_ordinal_group_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_ordinal_group_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_ordinal_group_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_ordinal_group_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_ordinal_group_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_orient_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_orient_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_orient_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_orient_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_orient_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_orient_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_pack_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_pack_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_pack_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_pack_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_pack_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_pack_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_shadow_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_shadow_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_shadow_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_shadow_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_shadow_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_shadow_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_sizing_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_sizing_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_sizing_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_sizing_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_sizing_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_box_sizing_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_filter_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_filter_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_filter_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_filter_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_filter_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_filter_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_basis_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_basis_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_basis_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_basis_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_basis_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_basis_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_direction_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_direction_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_direction_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_direction_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_direction_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_direction_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_flow_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_flow_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_flow_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_flow_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_flow_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_flow_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_grow_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_grow_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_grow_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_grow_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_grow_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_grow_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_shrink_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_shrink_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_shrink_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_shrink_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_shrink_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_shrink_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_wrap_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_wrap_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_wrap_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_wrap_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_wrap_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_flex_wrap_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_justify_content_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_justify_content_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_justify_content_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_justify_content_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_justify_content_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_justify_content_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_clip_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_clip_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_clip_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_clip_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_clip_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_clip_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_composite_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_composite_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_composite_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_composite_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_composite_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_composite_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_image_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_image_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_image_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_image_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_image_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_image_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_origin_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_origin_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_origin_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_origin_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_origin_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_origin_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_position_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_position_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_position_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_position_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_position_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_position_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_repeat_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_repeat_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_repeat_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_repeat_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_repeat_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_repeat_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_size_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_size_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_size_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_size_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_size_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_mask_size_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_order_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_order_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_order_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_order_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_order_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_order_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_perspective_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_perspective_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_perspective_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_perspective_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_perspective_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_perspective_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_perspective_origin_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_perspective_origin_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_perspective_origin_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_perspective_origin_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_perspective_origin_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_perspective_origin_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_text_fill_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_text_fill_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_text_fill_color_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_text_fill_color_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_text_fill_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_text_fill_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_origin_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_origin_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_origin_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_origin_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_origin_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_origin_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_style_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_style_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transform_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_delay_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_delay_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_delay_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_delay_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_delay_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_delay_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_duration_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_duration_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_duration_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_duration_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_duration_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_duration_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_property_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_property_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_property_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_property_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_property_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_property_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_timing_function_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_timing_function_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_timing_function_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_timing_function_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_timing_function_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_transition_timing_function_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_user_select_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_user_select_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_user_select_webkit_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_user_select_webkit_setter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_user_select_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(webkit_user_select_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(accent_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(accent_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(accent_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(accent_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(align_content_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_content_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(align_content_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_content_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(align_items_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_items_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(align_items_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_items_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(align_self_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_self_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(align_self_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_self_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(all_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(all_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(anchor_name_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(anchor_name_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(anchor_name_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(anchor_name_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(anchor_scope_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(anchor_scope_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(anchor_scope_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(anchor_scope_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_composition_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_composition_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_composition_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_composition_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_delay_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_delay_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_delay_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_delay_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_direction_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_direction_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_direction_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_direction_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_duration_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_duration_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_duration_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_duration_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_fill_mode_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_fill_mode_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_fill_mode_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_fill_mode_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_iteration_count_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_iteration_count_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_iteration_count_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_iteration_count_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_name_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_name_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_name_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_name_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_play_state_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_play_state_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_play_state_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_play_state_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_timeline_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_timeline_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_timeline_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_timeline_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_timing_function_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_timing_function_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(animation_timing_function_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(animation_timing_function_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(appearance_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(appearance_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(aspect_ratio_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(aspect_ratio_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(aspect_ratio_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(aspect_ratio_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(backdrop_filter_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(backdrop_filter_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(backdrop_filter_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(backdrop_filter_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_attachment_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_attachment_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_attachment_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_attachment_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_blend_mode_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_blend_mode_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_blend_mode_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_blend_mode_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_clip_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_clip_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_clip_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_clip_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_image_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_image_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_image_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_image_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_origin_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_origin_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_origin_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_origin_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_position_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_position_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_position_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_position_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_position_x_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_position_x_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_position_x_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_position_x_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_position_y_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_position_y_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_position_y_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_position_y_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_repeat_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_repeat_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_repeat_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_repeat_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_size_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_size_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_size_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_size_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(block_size_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(block_size_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(block_size_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(block_size_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_end_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_end_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_end_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_end_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_end_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_end_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_end_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_end_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_end_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_end_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_end_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_end_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_end_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_end_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_end_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_end_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_start_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_start_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_start_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_start_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_start_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_start_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_start_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_start_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_start_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_start_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_start_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_start_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_start_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_start_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_start_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_start_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_block_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_left_radius_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_left_radius_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_left_radius_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_left_radius_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_right_radius_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_right_radius_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_right_radius_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_right_radius_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_bottom_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_collapse_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_collapse_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_collapse_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_collapse_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_end_end_radius_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_end_end_radius_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_end_end_radius_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_end_end_radius_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_end_start_radius_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_end_start_radius_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_end_start_radius_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_end_start_radius_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_outset_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_outset_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_outset_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_outset_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_repeat_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_repeat_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_repeat_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_repeat_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_slice_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_slice_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_slice_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_slice_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_source_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_source_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_source_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_source_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_image_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_end_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_end_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_end_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_end_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_end_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_end_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_end_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_end_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_end_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_end_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_end_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_end_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_end_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_end_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_end_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_end_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_start_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_start_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_start_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_start_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_start_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_start_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_start_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_start_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_start_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_start_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_start_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_start_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_start_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_start_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_start_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_start_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_inline_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_left_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_left_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_left_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_left_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_left_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_left_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_left_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_left_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_left_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_left_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_left_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_left_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_left_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_left_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_left_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_left_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_radius_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_radius_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_radius_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_radius_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_right_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_right_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_right_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_right_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_right_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_right_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_right_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_right_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_right_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_right_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_right_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_right_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_right_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_right_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_right_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_right_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_spacing_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_spacing_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_spacing_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_spacing_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_start_end_radius_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_start_end_radius_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_start_end_radius_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_start_end_radius_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_start_start_radius_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_start_start_radius_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_start_start_radius_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_start_start_radius_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_left_radius_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_left_radius_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_left_radius_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_left_radius_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_right_radius_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_right_radius_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_right_radius_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_right_radius_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_top_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(bottom_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(bottom_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(box_shadow_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(box_shadow_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(box_shadow_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(box_shadow_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(box_sizing_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(box_sizing_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(box_sizing_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(box_sizing_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(caption_side_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(caption_side_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(caption_side_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(caption_side_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(caret_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(caret_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(caret_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(caret_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(clear_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(clear_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(clip_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(clip_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(clip_path_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(clip_path_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(clip_path_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(clip_path_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(clip_rule_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(clip_rule_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(clip_rule_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(clip_rule_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(color_interpolation_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(color_interpolation_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(color_interpolation_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(color_interpolation_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(color_scheme_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(color_scheme_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(color_scheme_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(color_scheme_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(column_count_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(column_count_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(column_count_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(column_count_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(column_gap_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(column_gap_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(column_gap_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(column_gap_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(column_height_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(column_height_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(column_height_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(column_height_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(column_span_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(column_span_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(column_span_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(column_span_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(column_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(column_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(column_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(column_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(columns_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(columns_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(contain_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(contain_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(container_type_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(container_type_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(container_type_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(container_type_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(content_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(content_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(content_visibility_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(content_visibility_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(content_visibility_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(content_visibility_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_block_end_shape_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_block_end_shape_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_block_end_shape_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_block_end_shape_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_block_start_shape_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_block_start_shape_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_block_start_shape_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_block_start_shape_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_bottom_left_shape_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_bottom_left_shape_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_bottom_left_shape_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_bottom_left_shape_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_bottom_right_shape_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_bottom_right_shape_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_bottom_right_shape_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_bottom_right_shape_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_bottom_shape_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_bottom_shape_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_bottom_shape_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_bottom_shape_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_end_end_shape_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_end_end_shape_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_end_end_shape_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_end_end_shape_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_end_start_shape_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_end_start_shape_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_end_start_shape_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_end_start_shape_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_inline_end_shape_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_inline_end_shape_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_inline_end_shape_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_inline_end_shape_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_inline_start_shape_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_inline_start_shape_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_inline_start_shape_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_inline_start_shape_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_left_shape_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_left_shape_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_left_shape_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_left_shape_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_right_shape_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_right_shape_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_right_shape_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_right_shape_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_shape_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_shape_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_shape_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_shape_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_start_end_shape_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_start_end_shape_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_start_end_shape_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_start_end_shape_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_start_start_shape_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_start_start_shape_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_start_start_shape_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_start_start_shape_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_top_left_shape_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_top_left_shape_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_top_left_shape_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_top_left_shape_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_top_right_shape_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_top_right_shape_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_top_right_shape_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_top_right_shape_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_top_shape_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_top_shape_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(corner_top_shape_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(corner_top_shape_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(counter_increment_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(counter_increment_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(counter_increment_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(counter_increment_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(counter_reset_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(counter_reset_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(counter_reset_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(counter_reset_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(counter_set_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(counter_set_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(counter_set_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(counter_set_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(cursor_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(cursor_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(cx_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(cx_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(cy_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(cy_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(direction_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(direction_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(display_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(display_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(dominant_baseline_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(dominant_baseline_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(dominant_baseline_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(dominant_baseline_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(empty_cells_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(empty_cells_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(empty_cells_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(empty_cells_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(fill_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(fill_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(fill_opacity_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(fill_opacity_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(fill_opacity_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(fill_opacity_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(fill_rule_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(fill_rule_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(fill_rule_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(fill_rule_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(filter_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(filter_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(flex_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(flex_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(flex_basis_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(flex_basis_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(flex_basis_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(flex_basis_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(flex_direction_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(flex_direction_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(flex_direction_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(flex_direction_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(flex_flow_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(flex_flow_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(flex_flow_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(flex_flow_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(flex_grow_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(flex_grow_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(flex_grow_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(flex_grow_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(flex_shrink_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(flex_shrink_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(flex_shrink_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(flex_shrink_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(flex_wrap_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(flex_wrap_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(flex_wrap_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(flex_wrap_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(float_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(float_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(flood_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(flood_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(flood_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(flood_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(flood_opacity_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(flood_opacity_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(flood_opacity_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(flood_opacity_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_family_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_family_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_family_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_family_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_feature_settings_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_feature_settings_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_feature_settings_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_feature_settings_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_kerning_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_kerning_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_kerning_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_kerning_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_language_override_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_language_override_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_language_override_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_language_override_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_optical_sizing_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_optical_sizing_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_optical_sizing_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_optical_sizing_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_size_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_size_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_size_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_size_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_stretch_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_stretch_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_stretch_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_stretch_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_alternates_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_alternates_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_alternates_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_alternates_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_caps_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_caps_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_caps_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_caps_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_east_asian_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_east_asian_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_east_asian_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_east_asian_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_emoji_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_emoji_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_emoji_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_emoji_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_ligatures_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_ligatures_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_ligatures_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_ligatures_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_numeric_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_numeric_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_numeric_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_numeric_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_position_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_position_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_position_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variant_position_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variation_settings_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variation_settings_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_variation_settings_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_variation_settings_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_weight_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_weight_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_weight_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_weight_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(font_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(gap_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(gap_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_area_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_area_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_area_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_area_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_auto_columns_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_auto_columns_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_auto_columns_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_auto_columns_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_auto_flow_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_auto_flow_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_auto_flow_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_auto_flow_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_auto_rows_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_auto_rows_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_auto_rows_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_auto_rows_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_column_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_column_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_column_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_column_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_column_end_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_column_end_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_column_end_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_column_end_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_column_gap_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_column_gap_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_column_gap_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_column_gap_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_column_start_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_column_start_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_column_start_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_column_start_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_gap_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_gap_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_gap_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_gap_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_row_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_row_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_row_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_row_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_row_end_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_row_end_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_row_end_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_row_end_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_row_gap_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_row_gap_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_row_gap_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_row_gap_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_row_start_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_row_start_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_row_start_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_row_start_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_template_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_template_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_template_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_template_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_template_areas_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_template_areas_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_template_areas_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_template_areas_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_template_columns_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_template_columns_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_template_columns_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_template_columns_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_template_rows_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_template_rows_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(grid_template_rows_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(grid_template_rows_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(height_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(image_rendering_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(image_rendering_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(image_rendering_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(image_rendering_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(inline_size_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(inline_size_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(inline_size_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(inline_size_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(inset_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(inset_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(inset_block_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(inset_block_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(inset_block_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(inset_block_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(inset_block_end_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(inset_block_end_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(inset_block_end_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(inset_block_end_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(inset_block_start_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(inset_block_start_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(inset_block_start_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(inset_block_start_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(inset_inline_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(inset_inline_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(inset_inline_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(inset_inline_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(inset_inline_end_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(inset_inline_end_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(inset_inline_end_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(inset_inline_end_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(inset_inline_start_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(inset_inline_start_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(inset_inline_start_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(inset_inline_start_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(isolation_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(isolation_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(justify_content_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(justify_content_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(justify_content_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(justify_content_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(justify_items_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(justify_items_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(justify_items_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(justify_items_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(justify_self_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(justify_self_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(justify_self_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(justify_self_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(left_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(left_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(letter_spacing_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(letter_spacing_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(letter_spacing_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(letter_spacing_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(line_height_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(line_height_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(line_height_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(line_height_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(list_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(list_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(list_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(list_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(list_style_image_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(list_style_image_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(list_style_image_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(list_style_image_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(list_style_position_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(list_style_position_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(list_style_position_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(list_style_position_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(list_style_type_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(list_style_type_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(list_style_type_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(list_style_type_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_block_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_block_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_block_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_block_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_block_end_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_block_end_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_block_end_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_block_end_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_block_start_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_block_start_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_block_start_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_block_start_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_bottom_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_bottom_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_bottom_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_bottom_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_inline_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_inline_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_inline_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_inline_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_inline_end_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_inline_end_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_inline_end_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_inline_end_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_inline_start_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_inline_start_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_inline_start_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_inline_start_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_left_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_left_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_left_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_left_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_right_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_right_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_right_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_right_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_top_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_top_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_top_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_top_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_clip_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_clip_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_clip_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_clip_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_composite_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_composite_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_composite_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_composite_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_image_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_image_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_image_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_image_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_mode_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_mode_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_mode_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_mode_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_origin_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_origin_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_origin_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_origin_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_position_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_position_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_position_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_position_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_repeat_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_repeat_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_repeat_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_repeat_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_size_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_size_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_size_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_size_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_type_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_type_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(mask_type_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(mask_type_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(math_depth_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(math_depth_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(math_depth_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(math_depth_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(math_shift_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(math_shift_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(math_shift_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(math_shift_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(math_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(math_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(math_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(math_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(max_block_size_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_block_size_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(max_block_size_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_block_size_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(max_height_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_height_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(max_height_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_height_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(max_inline_size_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_inline_size_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(max_inline_size_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_inline_size_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(max_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(max_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(min_block_size_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(min_block_size_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(min_block_size_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(min_block_size_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(min_height_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(min_height_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(min_height_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(min_height_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(min_inline_size_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(min_inline_size_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(min_inline_size_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(min_inline_size_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(min_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(min_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(min_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(min_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(mix_blend_mode_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(mix_blend_mode_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(mix_blend_mode_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(mix_blend_mode_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(object_fit_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(object_fit_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(object_fit_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(object_fit_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(object_position_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(object_position_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(object_position_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(object_position_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(opacity_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(opacity_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(order_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(order_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(orphans_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(orphans_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(outline_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(outline_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(outline_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(outline_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(outline_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(outline_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(outline_offset_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(outline_offset_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(outline_offset_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(outline_offset_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(outline_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(outline_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(outline_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(outline_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(outline_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(outline_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(outline_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(outline_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_block_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_block_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_block_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_block_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_block_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_block_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_block_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_block_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_block_end_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_block_end_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_block_end_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_block_end_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_block_start_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_block_start_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_block_start_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_block_start_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_bottom_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_bottom_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_bottom_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_bottom_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_inline_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_inline_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_inline_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_inline_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_inline_end_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_inline_end_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_inline_end_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_inline_end_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_inline_start_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_inline_start_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_inline_start_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_inline_start_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_left_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_left_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_left_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_left_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_right_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_right_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_right_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_right_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_top_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_top_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_top_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_clip_margin_top_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_inline_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_inline_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_inline_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_inline_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_wrap_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_wrap_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_wrap_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_wrap_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_x_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_x_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_x_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_x_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_y_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_y_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_y_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(overflow_y_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_block_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_block_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_block_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_block_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_block_end_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_block_end_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_block_end_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_block_end_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_block_start_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_block_start_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_block_start_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_block_start_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_bottom_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_bottom_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_bottom_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_bottom_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_inline_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_inline_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_inline_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_inline_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_inline_end_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_inline_end_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_inline_end_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_inline_end_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_inline_start_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_inline_start_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_inline_start_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_inline_start_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_left_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_left_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_left_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_left_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_right_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_right_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_right_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_right_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_top_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_top_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(padding_top_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(padding_top_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(paint_order_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(paint_order_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(paint_order_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(paint_order_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(perspective_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(perspective_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(perspective_origin_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(perspective_origin_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(perspective_origin_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(perspective_origin_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(place_content_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(place_content_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(place_content_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(place_content_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(place_items_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(place_items_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(place_items_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(place_items_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(place_self_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(place_self_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(place_self_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(place_self_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(pointer_events_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(pointer_events_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(pointer_events_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(pointer_events_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(position_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(position_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(position_anchor_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(position_anchor_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(position_anchor_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(position_anchor_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(position_area_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(position_area_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(position_area_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(position_area_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(position_try_fallbacks_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(position_try_fallbacks_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(position_try_fallbacks_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(position_try_fallbacks_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(position_try_order_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(position_try_order_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(position_try_order_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(position_try_order_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(position_visibility_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(position_visibility_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(position_visibility_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(position_visibility_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(quotes_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(quotes_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(r_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(r_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(resize_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(resize_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(right_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(right_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(rotate_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(rotate_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(row_gap_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(row_gap_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(row_gap_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(row_gap_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(rx_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(rx_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(ry_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(ry_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(scale_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(scale_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_timeline_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_timeline_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_timeline_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_timeline_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_timeline_axis_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_timeline_axis_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_timeline_axis_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_timeline_axis_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_timeline_name_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_timeline_name_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_timeline_name_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_timeline_name_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(scrollbar_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(scrollbar_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(scrollbar_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(scrollbar_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(scrollbar_gutter_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(scrollbar_gutter_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(scrollbar_gutter_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(scrollbar_gutter_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(scrollbar_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(scrollbar_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(scrollbar_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(scrollbar_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(shape_image_threshold_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(shape_image_threshold_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(shape_image_threshold_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(shape_image_threshold_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(shape_margin_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(shape_margin_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(shape_margin_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(shape_margin_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(shape_outside_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(shape_outside_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(shape_outside_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(shape_outside_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(shape_rendering_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(shape_rendering_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(shape_rendering_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(shape_rendering_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(stop_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(stop_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(stop_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(stop_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(stop_opacity_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(stop_opacity_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(stop_opacity_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(stop_opacity_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_dasharray_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_dasharray_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_dasharray_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_dasharray_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_dashoffset_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_dashoffset_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_dashoffset_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_dashoffset_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_linecap_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_linecap_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_linecap_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_linecap_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_linejoin_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_linejoin_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_linejoin_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_linejoin_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_miterlimit_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_miterlimit_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_miterlimit_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_miterlimit_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_opacity_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_opacity_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_opacity_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_opacity_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_width_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(stroke_width_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(tab_size_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(tab_size_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(tab_size_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(tab_size_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(table_layout_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(table_layout_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(table_layout_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(table_layout_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_align_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_align_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_align_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_align_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_anchor_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_anchor_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_anchor_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_anchor_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_color_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_color_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_color_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_color_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_line_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_line_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_line_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_line_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_thickness_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_thickness_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_thickness_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_decoration_thickness_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_indent_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_indent_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_indent_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_indent_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_justify_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_justify_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_justify_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_justify_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_overflow_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_overflow_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_overflow_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_overflow_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_rendering_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_rendering_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_rendering_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_rendering_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_shadow_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_shadow_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_shadow_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_shadow_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_transform_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_transform_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_transform_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_transform_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_underline_offset_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_underline_offset_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_underline_offset_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_underline_offset_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_underline_position_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_underline_position_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_underline_position_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_underline_position_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_wrap_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_wrap_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_wrap_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_wrap_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_wrap_mode_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_wrap_mode_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_wrap_mode_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_wrap_mode_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_wrap_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_wrap_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_wrap_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_wrap_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(timeline_scope_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(timeline_scope_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(timeline_scope_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(timeline_scope_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(top_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(top_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(touch_action_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(touch_action_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(touch_action_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(touch_action_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(transform_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(transform_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(transform_box_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(transform_box_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(transform_box_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(transform_box_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(transform_origin_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(transform_origin_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(transform_origin_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(transform_origin_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(transform_style_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(transform_style_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(transform_style_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(transform_style_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(transition_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(transition_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(transition_behavior_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(transition_behavior_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(transition_behavior_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(transition_behavior_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(transition_delay_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(transition_delay_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(transition_delay_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(transition_delay_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(transition_duration_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(transition_duration_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(transition_duration_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(transition_duration_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(transition_property_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(transition_property_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(transition_property_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(transition_property_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(transition_timing_function_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(transition_timing_function_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(transition_timing_function_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(transition_timing_function_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(translate_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(translate_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(unicode_bidi_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(unicode_bidi_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(unicode_bidi_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(unicode_bidi_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(user_select_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(user_select_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(user_select_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(user_select_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(vertical_align_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(vertical_align_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(vertical_align_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(vertical_align_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(view_timeline_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(view_timeline_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(view_timeline_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(view_timeline_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(view_timeline_axis_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(view_timeline_axis_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(view_timeline_axis_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(view_timeline_axis_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(view_timeline_inset_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(view_timeline_inset_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(view_timeline_inset_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(view_timeline_inset_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(view_timeline_name_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(view_timeline_name_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(view_timeline_name_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(view_timeline_name_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(view_transition_name_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(view_transition_name_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(view_transition_name_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(view_transition_name_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(visibility_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(visibility_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(white_space_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(white_space_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(white_space_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(white_space_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(white_space_collapse_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(white_space_collapse_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(white_space_collapse_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(white_space_collapse_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(white_space_trim_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(white_space_trim_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(white_space_trim_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(white_space_trim_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(widows_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(widows_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(width_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(will_change_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(will_change_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(will_change_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(will_change_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(word_break_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(word_break_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(word_break_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(word_break_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(word_spacing_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(word_spacing_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(word_spacing_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(word_spacing_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(word_wrap_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(word_wrap_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(word_wrap_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(word_wrap_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(writing_mode_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(writing_mode_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(writing_mode_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(writing_mode_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(x_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(x_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(y_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(z_index_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(z_index_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(z_index_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(z_index_dashed_setter);


};


} // namespace Web::Bindings
    