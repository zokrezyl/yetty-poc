
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLAreaElementPrototype : public JS::Object {
    JS_OBJECT(HTMLAreaElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLAreaElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLAreaElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLAreaElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(to_string);
        
    JS_DECLARE_NATIVE_FUNCTION(alt_getter);

    JS_DECLARE_NATIVE_FUNCTION(alt_setter);

    JS_DECLARE_NATIVE_FUNCTION(coords_getter);

    JS_DECLARE_NATIVE_FUNCTION(coords_setter);

    JS_DECLARE_NATIVE_FUNCTION(shape_getter);

    JS_DECLARE_NATIVE_FUNCTION(shape_setter);

    JS_DECLARE_NATIVE_FUNCTION(target_getter);

    JS_DECLARE_NATIVE_FUNCTION(target_setter);

    JS_DECLARE_NATIVE_FUNCTION(download_getter);

    JS_DECLARE_NATIVE_FUNCTION(download_setter);

    JS_DECLARE_NATIVE_FUNCTION(ping_getter);

    JS_DECLARE_NATIVE_FUNCTION(ping_setter);

    JS_DECLARE_NATIVE_FUNCTION(rel_getter);

    JS_DECLARE_NATIVE_FUNCTION(rel_setter);

    JS_DECLARE_NATIVE_FUNCTION(rel_list_getter);

    JS_DECLARE_NATIVE_FUNCTION(rel_list_setter);

    JS_DECLARE_NATIVE_FUNCTION(referrer_policy_getter);

    JS_DECLARE_NATIVE_FUNCTION(referrer_policy_setter);

    JS_DECLARE_NATIVE_FUNCTION(no_href_getter);

    JS_DECLARE_NATIVE_FUNCTION(no_href_setter);

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

    JS_DECLARE_NATIVE_FUNCTION(hash_getter);

    JS_DECLARE_NATIVE_FUNCTION(hash_setter);


};


} // namespace Web::Bindings
    