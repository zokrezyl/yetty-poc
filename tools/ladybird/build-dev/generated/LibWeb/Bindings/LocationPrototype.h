
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class LocationPrototype : public JS::Object {
    JS_OBJECT(LocationPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(LocationPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit LocationPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~LocationPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(assign);
        
    JS_DECLARE_NATIVE_FUNCTION(replace);
        
    JS_DECLARE_NATIVE_FUNCTION(reload);
        
    JS_DECLARE_NATIVE_FUNCTION(to_string);
        
    JS_DECLARE_NATIVE_FUNCTION(href_getter);

    JS_DECLARE_NATIVE_FUNCTION(href_setter);

    JS_DECLARE_NATIVE_FUNCTION(origin_getter);

    JS_DECLARE_NATIVE_FUNCTION(protocol_getter);

    JS_DECLARE_NATIVE_FUNCTION(protocol_setter);

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
    