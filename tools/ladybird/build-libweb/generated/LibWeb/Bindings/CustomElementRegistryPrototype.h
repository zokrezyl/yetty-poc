
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CustomElementRegistryPrototype : public JS::Object {
    JS_OBJECT(CustomElementRegistryPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CustomElementRegistryPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CustomElementRegistryPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CustomElementRegistryPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(define);
        
    JS_DECLARE_NATIVE_FUNCTION(get);
        
    JS_DECLARE_NATIVE_FUNCTION(get_name);
        
    JS_DECLARE_NATIVE_FUNCTION(when_defined);
        
    JS_DECLARE_NATIVE_FUNCTION(upgrade);
        

};


} // namespace Web::Bindings
    