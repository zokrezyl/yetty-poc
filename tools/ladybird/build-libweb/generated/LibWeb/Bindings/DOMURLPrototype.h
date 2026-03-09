
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DOMURLPrototype : public JS::Object {
    JS_OBJECT(DOMURLPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DOMURLPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DOMURLPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMURLPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(to_json);
        
    JS_DECLARE_NATIVE_FUNCTION(to_string);
        
    JS_DECLARE_NATIVE_FUNCTION(href_getter);

    JS_DECLARE_NATIVE_FUNCTION(href_setter);

    JS_DECLARE_NATIVE_FUNCTION(origin_getter);

    JS_DECLARE_NATIVE_FUNCTION(protocol_getter);

    JS_DECLARE_NATIVE_FUNCTION(protocol_setter);

    JS_DECLARE_NATIVE_FUNCTION(username_getter);

    JS_DECLARE_NATIVE_FUNCTION(username_setter);

    JS_DECLARE_NATIVE_FUNCTION(password_getter);

    JS_DECLARE_NATIVE_FUNCTION(password_setter);

    JS_DECLARE_NATIVE_FUNCTION(host_getter);

    JS_DECLARE_NATIVE_FUNCTION(host_setter);

    JS_DECLARE_NATIVE_FUNCTION(hostname_getter);

    JS_DECLARE_NATIVE_FUNCTION(hostname_setter);

    JS_DECLARE_NATIVE_FUNCTION(port_getter);

    JS_DECLARE_NATIVE_FUNCTION(port_setter);

    JS_DECLARE_NATIVE_FUNCTION(pathname_getter);

    JS_DECLARE_NATIVE_FUNCTION(pathname_setter);

    JS_DECLARE_NATIVE_FUNCTION(search_getter);

    JS_DECLARE_NATIVE_FUNCTION(search_setter);

    JS_DECLARE_NATIVE_FUNCTION(search_params_getter);

    JS_DECLARE_NATIVE_FUNCTION(hash_getter);

    JS_DECLARE_NATIVE_FUNCTION(hash_setter);


};


} // namespace Web::Bindings
    