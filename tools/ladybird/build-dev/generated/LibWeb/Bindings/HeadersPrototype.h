
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HeadersPrototype : public JS::Object {
    JS_OBJECT(HeadersPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HeadersPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HeadersPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HeadersPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(append);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_);
        
    JS_DECLARE_NATIVE_FUNCTION(get);
        
    JS_DECLARE_NATIVE_FUNCTION(get_set_cookie);
        
    JS_DECLARE_NATIVE_FUNCTION(has);
        
    JS_DECLARE_NATIVE_FUNCTION(set);
        
    JS_DECLARE_NATIVE_FUNCTION(entries);
    JS_DECLARE_NATIVE_FUNCTION(for_each);
    JS_DECLARE_NATIVE_FUNCTION(keys);
    JS_DECLARE_NATIVE_FUNCTION(values);
        

};


} // namespace Web::Bindings
    