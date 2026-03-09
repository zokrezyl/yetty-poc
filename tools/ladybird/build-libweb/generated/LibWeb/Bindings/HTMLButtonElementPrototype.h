
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLButtonElementPrototype : public JS::Object {
    JS_OBJECT(HTMLButtonElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLButtonElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLButtonElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLButtonElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(check_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(report_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(set_custom_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(command_getter);

    JS_DECLARE_NATIVE_FUNCTION(command_setter);

    JS_DECLARE_NATIVE_FUNCTION(command_for_element_getter);

    JS_DECLARE_NATIVE_FUNCTION(command_for_element_setter);

    JS_DECLARE_NATIVE_FUNCTION(disabled_getter);

    JS_DECLARE_NATIVE_FUNCTION(disabled_setter);

    JS_DECLARE_NATIVE_FUNCTION(form_getter);

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

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_setter);

    JS_DECLARE_NATIVE_FUNCTION(value_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_setter);

    JS_DECLARE_NATIVE_FUNCTION(will_validate_getter);

    JS_DECLARE_NATIVE_FUNCTION(validity_getter);

    JS_DECLARE_NATIVE_FUNCTION(validation_message_getter);

    JS_DECLARE_NATIVE_FUNCTION(labels_getter);

    JS_DECLARE_NATIVE_FUNCTION(popover_target_element_getter);

    JS_DECLARE_NATIVE_FUNCTION(popover_target_element_setter);

    JS_DECLARE_NATIVE_FUNCTION(popover_target_action_getter);

    JS_DECLARE_NATIVE_FUNCTION(popover_target_action_setter);


};


enum class ButtonTypeState : u8 {

    Submit,

    Reset,

    Button,

};

inline String idl_enum_to_string(ButtonTypeState value)
{
    switch (value) {

    case ButtonTypeState::Submit:
        return "submit"_string;

    case ButtonTypeState::Reset:
        return "reset"_string;

    case ButtonTypeState::Button:
        return "button"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    