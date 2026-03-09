
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class OriginPrototype : public JS::Object {
    JS_OBJECT(OriginPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(OriginPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit OriginPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~OriginPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(is_same_origin);
        
    JS_DECLARE_NATIVE_FUNCTION(is_same_site);
        
    JS_DECLARE_NATIVE_FUNCTION(opaque_getter);


};


} // namespace Web::Bindings
    