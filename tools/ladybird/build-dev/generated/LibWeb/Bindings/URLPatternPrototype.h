
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class URLPatternPrototype : public JS::Object {
    JS_OBJECT(URLPatternPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(URLPatternPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit URLPatternPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~URLPatternPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(test);
        
    JS_DECLARE_NATIVE_FUNCTION(exec);
        
    JS_DECLARE_NATIVE_FUNCTION(protocol_getter);

    JS_DECLARE_NATIVE_FUNCTION(username_getter);

    JS_DECLARE_NATIVE_FUNCTION(password_getter);

    JS_DECLARE_NATIVE_FUNCTION(hostname_getter);

    JS_DECLARE_NATIVE_FUNCTION(port_getter);

    JS_DECLARE_NATIVE_FUNCTION(pathname_getter);

    JS_DECLARE_NATIVE_FUNCTION(search_getter);

    JS_DECLARE_NATIVE_FUNCTION(hash_getter);

    JS_DECLARE_NATIVE_FUNCTION(has_reg_exp_groups_getter);


};


} // namespace Web::Bindings
    