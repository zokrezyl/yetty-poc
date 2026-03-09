
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLTextAreaElementPrototype : public JS::Object {
    JS_OBJECT(HTMLTextAreaElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLTextAreaElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLTextAreaElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLTextAreaElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(check_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(report_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(set_custom_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(select);
        
    JS_DECLARE_NATIVE_FUNCTION(set_range_text);
        
    JS_DECLARE_NATIVE_FUNCTION(set_range_text0);

    JS_DECLARE_NATIVE_FUNCTION(set_range_text1);

    JS_DECLARE_NATIVE_FUNCTION(set_selection_range);
        
    JS_DECLARE_NATIVE_FUNCTION(autocomplete_getter);

    JS_DECLARE_NATIVE_FUNCTION(autocomplete_setter);

    JS_DECLARE_NATIVE_FUNCTION(cols_getter);

    JS_DECLARE_NATIVE_FUNCTION(cols_setter);

    JS_DECLARE_NATIVE_FUNCTION(dir_name_getter);

    JS_DECLARE_NATIVE_FUNCTION(dir_name_setter);

    JS_DECLARE_NATIVE_FUNCTION(disabled_getter);

    JS_DECLARE_NATIVE_FUNCTION(disabled_setter);

    JS_DECLARE_NATIVE_FUNCTION(form_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_length_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_length_setter);

    JS_DECLARE_NATIVE_FUNCTION(min_length_getter);

    JS_DECLARE_NATIVE_FUNCTION(min_length_setter);

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(placeholder_getter);

    JS_DECLARE_NATIVE_FUNCTION(placeholder_setter);

    JS_DECLARE_NATIVE_FUNCTION(read_only_getter);

    JS_DECLARE_NATIVE_FUNCTION(read_only_setter);

    JS_DECLARE_NATIVE_FUNCTION(required_getter);

    JS_DECLARE_NATIVE_FUNCTION(required_setter);

    JS_DECLARE_NATIVE_FUNCTION(rows_getter);

    JS_DECLARE_NATIVE_FUNCTION(rows_setter);

    JS_DECLARE_NATIVE_FUNCTION(wrap_getter);

    JS_DECLARE_NATIVE_FUNCTION(wrap_setter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(default_value_getter);

    JS_DECLARE_NATIVE_FUNCTION(default_value_setter);

    JS_DECLARE_NATIVE_FUNCTION(value_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_length_getter);

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


};


} // namespace Web::Bindings
    