
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLFormElementPrototype : public JS::Object {
    JS_OBJECT(HTMLFormElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLFormElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLFormElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLFormElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(submit);
        
    JS_DECLARE_NATIVE_FUNCTION(request_submit);
        
    JS_DECLARE_NATIVE_FUNCTION(reset);
        
    JS_DECLARE_NATIVE_FUNCTION(check_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(report_validity);
        
    JS_DECLARE_NATIVE_FUNCTION(accept_charset_getter);

    JS_DECLARE_NATIVE_FUNCTION(accept_charset_setter);

    JS_DECLARE_NATIVE_FUNCTION(action_getter);

    JS_DECLARE_NATIVE_FUNCTION(action_setter);

    JS_DECLARE_NATIVE_FUNCTION(autocomplete_getter);

    JS_DECLARE_NATIVE_FUNCTION(autocomplete_setter);

    JS_DECLARE_NATIVE_FUNCTION(enctype_getter);

    JS_DECLARE_NATIVE_FUNCTION(enctype_setter);

    JS_DECLARE_NATIVE_FUNCTION(encoding_getter);

    JS_DECLARE_NATIVE_FUNCTION(encoding_setter);

    JS_DECLARE_NATIVE_FUNCTION(method_getter);

    JS_DECLARE_NATIVE_FUNCTION(method_setter);

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(no_validate_getter);

    JS_DECLARE_NATIVE_FUNCTION(no_validate_setter);

    JS_DECLARE_NATIVE_FUNCTION(target_getter);

    JS_DECLARE_NATIVE_FUNCTION(target_setter);

    JS_DECLARE_NATIVE_FUNCTION(rel_getter);

    JS_DECLARE_NATIVE_FUNCTION(rel_setter);

    JS_DECLARE_NATIVE_FUNCTION(rel_list_getter);

    JS_DECLARE_NATIVE_FUNCTION(rel_list_setter);

    JS_DECLARE_NATIVE_FUNCTION(elements_getter);

    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


enum class EnctypeAttribute : u8 {

    Application_XWwwFormUrlencoded,

    Multipart_FormData,

    Text_Plain,

};

inline String idl_enum_to_string(EnctypeAttribute value)
{
    switch (value) {

    case EnctypeAttribute::Application_XWwwFormUrlencoded:
        return "application/x-www-form-urlencoded"_string;

    case EnctypeAttribute::Multipart_FormData:
        return "multipart/form-data"_string;

    case EnctypeAttribute::Text_Plain:
        return "text/plain"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class MethodAttribute : u8 {

    Get,

    Post,

    Dialog,

};

inline String idl_enum_to_string(MethodAttribute value)
{
    switch (value) {

    case MethodAttribute::Get:
        return "get"_string;

    case MethodAttribute::Post:
        return "post"_string;

    case MethodAttribute::Dialog:
        return "dialog"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class FormEnctypeAttribute : u8 {

    Application_XWwwFormUrlencoded,

    Multipart_FormData,

    Text_Plain,

};

inline String idl_enum_to_string(FormEnctypeAttribute value)
{
    switch (value) {

    case FormEnctypeAttribute::Application_XWwwFormUrlencoded:
        return "application/x-www-form-urlencoded"_string;

    case FormEnctypeAttribute::Multipart_FormData:
        return "multipart/form-data"_string;

    case FormEnctypeAttribute::Text_Plain:
        return "text/plain"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class Autocomplete : u8 {

    On,

    Off,

};

inline String idl_enum_to_string(Autocomplete value)
{
    switch (value) {

    case Autocomplete::On:
        return "on"_string;

    case Autocomplete::Off:
        return "off"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class FormMethodAttribute : u8 {

    Get,

    Post,

    Dialog,

};

inline String idl_enum_to_string(FormMethodAttribute value)
{
    switch (value) {

    case FormMethodAttribute::Get:
        return "get"_string;

    case FormMethodAttribute::Post:
        return "post"_string;

    case FormMethodAttribute::Dialog:
        return "dialog"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class SelectionMode : u8 {

    Select,

    Start,

    End,

    Preserve,

};

inline String idl_enum_to_string(SelectionMode value)
{
    switch (value) {

    case SelectionMode::Select:
        return "select"_string;

    case SelectionMode::Start:
        return "start"_string;

    case SelectionMode::End:
        return "end"_string;

    case SelectionMode::Preserve:
        return "preserve"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    