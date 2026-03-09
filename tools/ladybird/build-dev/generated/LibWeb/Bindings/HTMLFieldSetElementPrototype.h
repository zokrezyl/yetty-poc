
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLFieldSetElementPrototype : public JS::Object {
    JS_OBJECT(HTMLFieldSetElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLFieldSetElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLFieldSetElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLFieldSetElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(check_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(report_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(set_custom_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(disabled_getter);

    JS_DECLARE_NATIVE_FUNCTION(disabled_setter);

    JS_DECLARE_NATIVE_FUNCTION(form_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(elements_getter);

    JS_DECLARE_NATIVE_FUNCTION(will_validate_getter);

    JS_DECLARE_NATIVE_FUNCTION(validity_getter);

    JS_DECLARE_NATIVE_FUNCTION(validation_message_getter);


};


} // namespace Web::Bindings
    