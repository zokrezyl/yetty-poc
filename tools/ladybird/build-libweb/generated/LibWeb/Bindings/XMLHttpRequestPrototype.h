
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class XMLHttpRequestPrototype : public JS::Object {
    JS_OBJECT(XMLHttpRequestPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(XMLHttpRequestPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit XMLHttpRequestPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~XMLHttpRequestPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(open);
        
    JS_DECLARE_NATIVE_FUNCTION(open0);

    JS_DECLARE_NATIVE_FUNCTION(open1);

    JS_DECLARE_NATIVE_FUNCTION(set_request_header);
        
    JS_DECLARE_NATIVE_FUNCTION(send);
        
    JS_DECLARE_NATIVE_FUNCTION(abort);
        
    JS_DECLARE_NATIVE_FUNCTION(get_response_header);
        
    JS_DECLARE_NATIVE_FUNCTION(get_all_response_headers);
        
    JS_DECLARE_NATIVE_FUNCTION(override_mime_type);
        
    JS_DECLARE_NATIVE_FUNCTION(onreadystatechange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onreadystatechange_setter);

    JS_DECLARE_NATIVE_FUNCTION(ready_state_getter);

    JS_DECLARE_NATIVE_FUNCTION(timeout_getter);

    JS_DECLARE_NATIVE_FUNCTION(timeout_setter);

    JS_DECLARE_NATIVE_FUNCTION(with_credentials_getter);

    JS_DECLARE_NATIVE_FUNCTION(with_credentials_setter);

    JS_DECLARE_NATIVE_FUNCTION(upload_getter);

    JS_DECLARE_NATIVE_FUNCTION(response_url_getter);

    JS_DECLARE_NATIVE_FUNCTION(status_getter);

    JS_DECLARE_NATIVE_FUNCTION(status_text_getter);

    JS_DECLARE_NATIVE_FUNCTION(response_type_getter);

    JS_DECLARE_NATIVE_FUNCTION(response_type_setter);

    JS_DECLARE_NATIVE_FUNCTION(response_getter);

    JS_DECLARE_NATIVE_FUNCTION(response_text_getter);

    JS_DECLARE_NATIVE_FUNCTION(response_xml_getter);


};


enum class XMLHttpRequestResponseType : u8 {

    Empty,

    Arraybuffer,

    Blob,

    Document,

    Json,

    Text,

};

inline String idl_enum_to_string(XMLHttpRequestResponseType value)
{
    switch (value) {

    case XMLHttpRequestResponseType::Empty:
        return ""_string;

    case XMLHttpRequestResponseType::Arraybuffer:
        return "arraybuffer"_string;

    case XMLHttpRequestResponseType::Blob:
        return "blob"_string;

    case XMLHttpRequestResponseType::Document:
        return "document"_string;

    case XMLHttpRequestResponseType::Json:
        return "json"_string;

    case XMLHttpRequestResponseType::Text:
        return "text"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    