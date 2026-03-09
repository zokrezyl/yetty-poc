
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ElementInternalsPrototype : public JS::Object {
    JS_OBJECT(ElementInternalsPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ElementInternalsPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ElementInternalsPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ElementInternalsPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(set_form_value);
        
    JS_DECLARE_NATIVE_FUNCTION(set_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(check_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(report_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(shadow_root_getter);

    JS_DECLARE_NATIVE_FUNCTION(form_getter);

    JS_DECLARE_NATIVE_FUNCTION(will_validate_getter);

    JS_DECLARE_NATIVE_FUNCTION(validity_getter);

    JS_DECLARE_NATIVE_FUNCTION(validation_message_getter);

    JS_DECLARE_NATIVE_FUNCTION(labels_getter);

    JS_DECLARE_NATIVE_FUNCTION(states_getter);


};


} // namespace Web::Bindings
    