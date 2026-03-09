
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLInputElementPrototype : public JS::Object {
    JS_OBJECT(HTMLInputElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLInputElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLInputElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLInputElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(step_up);
        
    JS_DECLARE_NATIVE_FUNCTION(step_down);
        
    JS_DECLARE_NATIVE_FUNCTION(check_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(report_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(set_custom_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(select);
        
    JS_DECLARE_NATIVE_FUNCTION(set_range_text);
        
    JS_DECLARE_NATIVE_FUNCTION(set_range_text0);

    JS_DECLARE_NATIVE_FUNCTION(set_range_text1);

    JS_DECLARE_NATIVE_FUNCTION(set_selection_range);
        
    JS_DECLARE_NATIVE_FUNCTION(show_picker);
        
    JS_DECLARE_NATIVE_FUNCTION(accept_getter);

    JS_DECLARE_NATIVE_FUNCTION(accept_setter);

    JS_DECLARE_NATIVE_FUNCTION(alt_getter);

    JS_DECLARE_NATIVE_FUNCTION(alt_setter);

    JS_DECLARE_NATIVE_FUNCTION(autocomplete_getter);

    JS_DECLARE_NATIVE_FUNCTION(autocomplete_setter);

    JS_DECLARE_NATIVE_FUNCTION(default_checked_getter);

    JS_DECLARE_NATIVE_FUNCTION(default_checked_setter);

    JS_DECLARE_NATIVE_FUNCTION(checked_getter);

    JS_DECLARE_NATIVE_FUNCTION(checked_setter);

    JS_DECLARE_NATIVE_FUNCTION(dir_name_getter);

    JS_DECLARE_NATIVE_FUNCTION(dir_name_setter);

    JS_DECLARE_NATIVE_FUNCTION(disabled_getter);

    JS_DECLARE_NATIVE_FUNCTION(disabled_setter);

    JS_DECLARE_NATIVE_FUNCTION(form_getter);

    JS_DECLARE_NATIVE_FUNCTION(files_getter);

    JS_DECLARE_NATIVE_FUNCTION(files_setter);

    JS_DECLARE_NATIVE_FUNCTION(form_action_getter);

    JS_DECLARE_NATIVE_FUNCTION(form_action_setter);

    JS_DECLARE_NATIVE_FUNCTION(form_enctype_getter);

    JS_DECLARE_NATIVE_FUNCTION(form_enctype_setter);

    JS_DECLARE_NATIVE_FUNCTION(form_method_getter);

    JS_DECLARE_NATIVE_FUNCTION(form_method_setter);

    JS_DECLARE_NATIVE_FUNCTION(form_no_validate_getter);

    JS_DECLARE_NATIVE_FUNCTION(form_no_validate_setter);

    JS_DECLARE_NATIVE_FUNCTION(form_target_getter);

    JS_DECLARE_NATIVE_FUNCTION(form_target_setter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_setter);

    JS_DECLARE_NATIVE_FUNCTION(indeterminate_getter);

    JS_DECLARE_NATIVE_FUNCTION(indeterminate_setter);

    JS_DECLARE_NATIVE_FUNCTION(list_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_setter);

    JS_DECLARE_NATIVE_FUNCTION(max_length_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_length_setter);

    JS_DECLARE_NATIVE_FUNCTION(min_getter);

    JS_DECLARE_NATIVE_FUNCTION(min_setter);

    JS_DECLARE_NATIVE_FUNCTION(min_length_getter);

    JS_DECLARE_NATIVE_FUNCTION(min_length_setter);

    JS_DECLARE_NATIVE_FUNCTION(multiple_getter);

    JS_DECLARE_NATIVE_FUNCTION(multiple_setter);

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(pattern_getter);

    JS_DECLARE_NATIVE_FUNCTION(pattern_setter);

    JS_DECLARE_NATIVE_FUNCTION(placeholder_getter);

    JS_DECLARE_NATIVE_FUNCTION(placeholder_setter);

    JS_DECLARE_NATIVE_FUNCTION(read_only_getter);

    JS_DECLARE_NATIVE_FUNCTION(read_only_setter);

    JS_DECLARE_NATIVE_FUNCTION(required_getter);

    JS_DECLARE_NATIVE_FUNCTION(required_setter);

    JS_DECLARE_NATIVE_FUNCTION(size_getter);

    JS_DECLARE_NATIVE_FUNCTION(size_setter);

    JS_DECLARE_NATIVE_FUNCTION(src_getter);

    JS_DECLARE_NATIVE_FUNCTION(src_setter);

    JS_DECLARE_NATIVE_FUNCTION(step_getter);

    JS_DECLARE_NATIVE_FUNCTION(step_setter);

    JS_DECLARE_NATIVE_FUNCTION(switch_getter);

    JS_DECLARE_NATIVE_FUNCTION(switch_setter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_setter);

    JS_DECLARE_NATIVE_FUNCTION(default_value_getter);

    JS_DECLARE_NATIVE_FUNCTION(default_value_setter);

    JS_DECLARE_NATIVE_FUNCTION(value_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_setter);

    JS_DECLARE_NATIVE_FUNCTION(value_as_date_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_as_date_setter);

    JS_DECLARE_NATIVE_FUNCTION(value_as_number_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_as_number_setter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_setter);

    JS_DECLARE_NATIVE_FUNCTION(will_validate_getter);

    JS_DECLARE_NATIVE_FUNCTION(validity_getter);

    JS_DECLARE_NATIVE_FUNCTION(validation_message_getter);

    JS_DECLARE_NATIVE_FUNCTION(labels_getter);

    JS_DECLARE_NATIVE_FUNCTION(selection_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(selection_start_setter);

    JS_DECLARE_NATIVE_FUNCTION(selection_end_getter);

    JS_DECLARE_NATIVE_FUNCTION(selection_end_setter);

    JS_DECLARE_NATIVE_FUNCTION(selection_direction_getter);

    JS_DECLARE_NATIVE_FUNCTION(selection_direction_setter);

    JS_DECLARE_NATIVE_FUNCTION(align_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_setter);

    JS_DECLARE_NATIVE_FUNCTION(use_map_getter);

    JS_DECLARE_NATIVE_FUNCTION(use_map_setter);

    JS_DECLARE_NATIVE_FUNCTION(popover_target_element_getter);

    JS_DECLARE_NATIVE_FUNCTION(popover_target_element_setter);

    JS_DECLARE_NATIVE_FUNCTION(popover_target_action_getter);

    JS_DECLARE_NATIVE_FUNCTION(popover_target_action_setter);


};


} // namespace Web::Bindings
    