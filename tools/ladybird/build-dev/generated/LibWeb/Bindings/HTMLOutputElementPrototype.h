
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLOutputElementPrototype : public JS::Object {
    JS_OBJECT(HTMLOutputElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLOutputElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLOutputElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLOutputElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(check_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(report_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(set_custom_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(html_for_getter);

    JS_DECLARE_NATIVE_FUNCTION(html_for_setter);

    JS_DECLARE_NATIVE_FUNCTION(form_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(default_value_getter);

    JS_DECLARE_NATIVE_FUNCTION(default_value_setter);

    JS_DECLARE_NATIVE_FUNCTION(value_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_setter);

    JS_DECLARE_NATIVE_FUNCTION(will_validate_getter);

    JS_DECLARE_NATIVE_FUNCTION(validity_getter);

    JS_DECLARE_NATIVE_FUNCTION(validation_message_getter);

    JS_DECLARE_NATIVE_FUNCTION(labels_getter);


};


} // namespace Web::Bindings
    