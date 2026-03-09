
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DOMStringListPrototype : public JS::Object {
    JS_OBJECT(DOMStringListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DOMStringListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DOMStringListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMStringListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(item);
        
    JS_DECLARE_NATIVE_FUNCTION(contains);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    