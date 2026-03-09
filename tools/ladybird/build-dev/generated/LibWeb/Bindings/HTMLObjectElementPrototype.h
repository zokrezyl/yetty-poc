
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLObjectElementPrototype : public JS::Object {
    JS_OBJECT(HTMLObjectElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLObjectElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLObjectElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLObjectElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_svg_document);
        
    JS_DECLARE_NATIVE_FUNCTION(check_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(report_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(set_custom_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(data_getter);

    JS_DECLARE_NATIVE_FUNCTION(data_setter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_setter);

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(form_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_setter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_setter);

    JS_DECLARE_NATIVE_FUNCTION(content_document_getter);

    JS_DECLARE_NATIVE_FUNCTION(content_window_getter);

    JS_DECLARE_NATIVE_FUNCTION(will_validate_getter);

    JS_DECLARE_NATIVE_FUNCTION(validity_getter);

    JS_DECLARE_NATIVE_FUNCTION(validation_message_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_setter);

    JS_DECLARE_NATIVE_FUNCTION(archive_getter);

    JS_DECLARE_NATIVE_FUNCTION(archive_setter);

    JS_DECLARE_NATIVE_FUNCTION(code_getter);

    JS_DECLARE_NATIVE_FUNCTION(code_setter);

    JS_DECLARE_NATIVE_FUNCTION(declare_getter);

    JS_DECLARE_NATIVE_FUNCTION(declare_setter);

    JS_DECLARE_NATIVE_FUNCTION(hspace_getter);

    JS_DECLARE_NATIVE_FUNCTION(hspace_setter);

    JS_DECLARE_NATIVE_FUNCTION(standby_getter);

    JS_DECLARE_NATIVE_FUNCTION(standby_setter);

    JS_DECLARE_NATIVE_FUNCTION(vspace_getter);

    JS_DECLARE_NATIVE_FUNCTION(vspace_setter);

    JS_DECLARE_NATIVE_FUNCTION(code_base_getter);

    JS_DECLARE_NATIVE_FUNCTION(code_base_setter);

    JS_DECLARE_NATIVE_FUNCTION(code_type_getter);

    JS_DECLARE_NATIVE_FUNCTION(code_type_setter);

    JS_DECLARE_NATIVE_FUNCTION(use_map_getter);

    JS_DECLARE_NATIVE_FUNCTION(use_map_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_setter);


};


} // namespace Web::Bindings
    