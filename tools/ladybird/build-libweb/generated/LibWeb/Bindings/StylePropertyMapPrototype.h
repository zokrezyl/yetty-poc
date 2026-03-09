
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class StylePropertyMapPrototype : public JS::Object {
    JS_OBJECT(StylePropertyMapPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(StylePropertyMapPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit StylePropertyMapPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~StylePropertyMapPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(set);
        
    JS_DECLARE_NATIVE_FUNCTION(append);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_);
        
    JS_DECLARE_NATIVE_FUNCTION(clear);
        

};


} // namespace Web::Bindings
    