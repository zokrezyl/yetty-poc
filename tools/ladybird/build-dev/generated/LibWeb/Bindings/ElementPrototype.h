
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ElementPrototype : public JS::Object {
    JS_OBJECT(ElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(has_attributes);
        
    JS_DECLARE_NATIVE_FUNCTION(get_attribute_names);
        
    JS_DECLARE_NATIVE_FUNCTION(get_attribute);
        
    JS_DECLARE_NATIVE_FUNCTION(get_attribute_ns);
        
    JS_DECLARE_NATIVE_FUNCTION(set_attribute);
        
    JS_DECLARE_NATIVE_FUNCTION(set_attribute_ns);
        
    JS_DECLARE_NATIVE_FUNCTION(remove_attribute);
        
    JS_DECLARE_NATIVE_FUNCTION(remove_attribute_ns);
        
    JS_DECLARE_NATIVE_FUNCTION(toggle_attribute);
        
    JS_DECLARE_NATIVE_FUNCTION(has_attribute);
        
    JS_DECLARE_NATIVE_FUNCTION(has_attribute_ns);
        
    JS_DECLARE_NATIVE_FUNCTION(get_attribute_node);
        
    JS_DECLARE_NATIVE_FUNCTION(get_attribute_node_ns);
        
    JS_DECLARE_NATIVE_FUNCTION(set_attribute_node);
        
    JS_DECLARE_NATIVE_FUNCTION(set_attribute_node_ns);
        
    JS_DECLARE_NATIVE_FUNCTION(remove_attribute_node);
        
    JS_DECLARE_NATIVE_FUNCTION(attach_shadow);
        
    JS_DECLARE_NATIVE_FUNCTION(matches);
        
    JS_DECLARE_NATIVE_FUNCTION(closest);
        
    JS_DECLARE_NATIVE_FUNCTION(webkit_matches_selector);
        
    JS_DECLARE_NATIVE_FUNCTION(get_elements_by_tag_name);
        
    JS_DECLARE_NATIVE_FUNCTION(get_elements_by_tag_name_ns);
        
    JS_DECLARE_NATIVE_FUNCTION(get_elements_by_class_name);
        
    JS_DECLARE_NATIVE_FUNCTION(insert_adjacent_element);
        
    JS_DECLARE_NATIVE_FUNCTION(insert_adjacent_text);
        
    JS_DECLARE_NATIVE_FUNCTION(get_client_rects);
        
    JS_DECLARE_NATIVE_FUNCTION(get_bounding_client_rect);
        
    JS_DECLARE_NATIVE_FUNCTION(check_visibility);
        
    JS_DECLARE_NATIVE_FUNCTION(scroll_into_view);
        
    JS_DECLARE_NATIVE_FUNCTION(scroll);
        
    JS_DECLARE_NATIVE_FUNCTION(scroll0);

    JS_DECLARE_NATIVE_FUNCTION(scroll1);

    JS_DECLARE_NATIVE_FUNCTION(scroll_to);
        
    JS_DECLARE_NATIVE_FUNCTION(scroll_to0);

    JS_DECLARE_NATIVE_FUNCTION(scroll_to1);

    JS_DECLARE_NATIVE_FUNCTION(scroll_by);
        
    JS_DECLARE_NATIVE_FUNCTION(scroll_by0);

    JS_DECLARE_NATIVE_FUNCTION(scroll_by1);

    JS_DECLARE_NATIVE_FUNCTION(set_html_unsafe);
        
    JS_DECLARE_NATIVE_FUNCTION(get_html);
        
    JS_DECLARE_NATIVE_FUNCTION(insert_adjacent_html);
        
    JS_DECLARE_NATIVE_FUNCTION(set_pointer_capture);
        
    JS_DECLARE_NATIVE_FUNCTION(release_pointer_capture);
        
    JS_DECLARE_NATIVE_FUNCTION(has_pointer_capture);
        
    JS_DECLARE_NATIVE_FUNCTION(request_pointer_lock);
        
    JS_DECLARE_NATIVE_FUNCTION(computed_style_map);
        
    JS_DECLARE_NATIVE_FUNCTION(request_fullscreen);
        
    JS_DECLARE_NATIVE_FUNCTION(before);
        
    JS_DECLARE_NATIVE_FUNCTION(after);
        
    JS_DECLARE_NATIVE_FUNCTION(replace_with);
        
    JS_DECLARE_NATIVE_FUNCTION(remove);
        
    JS_DECLARE_NATIVE_FUNCTION(animate);
        
    JS_DECLARE_NATIVE_FUNCTION(get_animations);
        
    JS_DECLARE_NATIVE_FUNCTION(prepend);
        
    JS_DECLARE_NATIVE_FUNCTION(append);
        
    JS_DECLARE_NATIVE_FUNCTION(replace_children);
        
    JS_DECLARE_NATIVE_FUNCTION(move_before);
        
    JS_DECLARE_NATIVE_FUNCTION(query_selector);
        
    JS_DECLARE_NATIVE_FUNCTION(query_selector_all);
        
    JS_DECLARE_NATIVE_FUNCTION(namespace_uri_getter);

    JS_DECLARE_NATIVE_FUNCTION(prefix_getter);

    JS_DECLARE_NATIVE_FUNCTION(local_name_getter);

    JS_DECLARE_NATIVE_FUNCTION(tag_name_getter);

    JS_DECLARE_NATIVE_FUNCTION(id_getter);

    JS_DECLARE_NATIVE_FUNCTION(id_setter);

    JS_DECLARE_NATIVE_FUNCTION(class_name_getter);

    JS_DECLARE_NATIVE_FUNCTION(class_name_setter);

    JS_DECLARE_NATIVE_FUNCTION(class_list_getter);

    JS_DECLARE_NATIVE_FUNCTION(class_list_setter);

    JS_DECLARE_NATIVE_FUNCTION(slot_getter);

    JS_DECLARE_NATIVE_FUNCTION(slot_setter);

    JS_DECLARE_NATIVE_FUNCTION(attributes_getter);

    JS_DECLARE_NATIVE_FUNCTION(shadow_root_getter);

    JS_DECLARE_NATIVE_FUNCTION(next_element_sibling_getter);

    JS_DECLARE_NATIVE_FUNCTION(previous_element_sibling_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_top_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_top_setter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_left_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_left_setter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_width_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_height_getter);

    JS_DECLARE_NATIVE_FUNCTION(client_top_getter);

    JS_DECLARE_NATIVE_FUNCTION(client_left_getter);

    JS_DECLARE_NATIVE_FUNCTION(client_width_getter);

    JS_DECLARE_NATIVE_FUNCTION(client_height_getter);

    JS_DECLARE_NATIVE_FUNCTION(current_css_zoom_getter);

    JS_DECLARE_NATIVE_FUNCTION(inner_html_getter);

    JS_DECLARE_NATIVE_FUNCTION(inner_html_setter);

    JS_DECLARE_NATIVE_FUNCTION(outer_html_getter);

    JS_DECLARE_NATIVE_FUNCTION(outer_html_setter);

    JS_DECLARE_NATIVE_FUNCTION(part_getter);

    JS_DECLARE_NATIVE_FUNCTION(part_setter);

    JS_DECLARE_NATIVE_FUNCTION(onfullscreenchange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onfullscreenchange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onfullscreenerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onfullscreenerror_setter);

    JS_DECLARE_NATIVE_FUNCTION(assigned_slot_getter);

    JS_DECLARE_NATIVE_FUNCTION(role_getter);

    JS_DECLARE_NATIVE_FUNCTION(role_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_active_descendant_element_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_active_descendant_element_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_atomic_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_atomic_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_auto_complete_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_auto_complete_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_braille_label_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_braille_label_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_braille_role_description_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_braille_role_description_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_busy_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_busy_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_checked_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_checked_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_col_count_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_col_count_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_col_index_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_col_index_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_col_index_text_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_col_index_text_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_col_span_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_col_span_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_controls_elements_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_controls_elements_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_current_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_current_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_described_by_elements_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_described_by_elements_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_description_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_description_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_details_elements_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_details_elements_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_disabled_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_disabled_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_error_message_elements_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_error_message_elements_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_expanded_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_expanded_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_flow_to_elements_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_flow_to_elements_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_has_popup_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_has_popup_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_hidden_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_hidden_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_invalid_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_invalid_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_key_shortcuts_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_key_shortcuts_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_label_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_label_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_labelled_by_elements_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_labelled_by_elements_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_level_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_level_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_live_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_live_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_modal_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_modal_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_multi_line_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_multi_line_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_multi_selectable_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_multi_selectable_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_orientation_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_orientation_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_owns_elements_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_owns_elements_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_placeholder_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_placeholder_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_pos_in_set_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_pos_in_set_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_pressed_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_pressed_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_read_only_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_read_only_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_relevant_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_relevant_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_required_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_required_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_role_description_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_role_description_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_row_count_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_row_count_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_row_index_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_row_index_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_row_index_text_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_row_index_text_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_row_span_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_row_span_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_selected_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_selected_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_set_size_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_set_size_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_sort_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_sort_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_value_max_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_value_max_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_value_min_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_value_min_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_value_now_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_value_now_setter);

    JS_DECLARE_NATIVE_FUNCTION(aria_value_text_getter);

    JS_DECLARE_NATIVE_FUNCTION(aria_value_text_setter);

    JS_DECLARE_NATIVE_FUNCTION(children_getter);

    JS_DECLARE_NATIVE_FUNCTION(first_element_child_getter);

    JS_DECLARE_NATIVE_FUNCTION(last_element_child_getter);

    JS_DECLARE_NATIVE_FUNCTION(child_element_count_getter);


};


enum class ScrollIntoViewContainer : u8 {

    All,

    Nearest,

};

inline String idl_enum_to_string(ScrollIntoViewContainer value)
{
    switch (value) {

    case ScrollIntoViewContainer::All:
        return "all"_string;

    case ScrollIntoViewContainer::Nearest:
        return "nearest"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class ScrollLogicalPosition : u8 {

    Start,

    Center,

    End,

    Nearest,

};

inline String idl_enum_to_string(ScrollLogicalPosition value)
{
    switch (value) {

    case ScrollLogicalPosition::Start:
        return "start"_string;

    case ScrollLogicalPosition::Center:
        return "center"_string;

    case ScrollLogicalPosition::End:
        return "end"_string;

    case ScrollLogicalPosition::Nearest:
        return "nearest"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    