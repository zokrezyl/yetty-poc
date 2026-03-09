
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ResponsePrototype : public JS::Object {
    JS_OBJECT(ResponsePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ResponsePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ResponsePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ResponsePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(clone);
        
    JS_DECLARE_NATIVE_FUNCTION(array_buffer);
        
    JS_DECLARE_NATIVE_FUNCTION(blob);
        
    JS_DECLARE_NATIVE_FUNCTION(bytes);
        
    JS_DECLARE_NATIVE_FUNCTION(form_data);
        
    JS_DECLARE_NATIVE_FUNCTION(json);
        
    JS_DECLARE_NATIVE_FUNCTION(text);
        
    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(url_getter);

    JS_DECLARE_NATIVE_FUNCTION(redirected_getter);

    JS_DECLARE_NATIVE_FUNCTION(status_getter);

    JS_DECLARE_NATIVE_FUNCTION(ok_getter);

    JS_DECLARE_NATIVE_FUNCTION(status_text_getter);

    JS_DECLARE_NATIVE_FUNCTION(headers_getter);

    JS_DECLARE_NATIVE_FUNCTION(body_getter);

    JS_DECLARE_NATIVE_FUNCTION(body_used_getter);


};


enum class ResponseType : u8 {

    Basic,

    Cors,

    Default,

    Error,

    Opaque,

    Opaqueredirect,

};

inline String idl_enum_to_string(ResponseType value)
{
    switch (value) {

    case ResponseType::Basic:
        return "basic"_string;

    case ResponseType::Cors:
        return "cors"_string;

    case ResponseType::Default:
        return "default"_string;

    case ResponseType::Error:
        return "error"_string;

    case ResponseType::Opaque:
        return "opaque"_string;

    case ResponseType::Opaqueredirect:
        return "opaqueredirect"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    