
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DOMQuadPrototype : public JS::Object {
    JS_OBJECT(DOMQuadPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DOMQuadPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DOMQuadPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMQuadPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_bounds);
        
    JS_DECLARE_NATIVE_FUNCTION(to_json);
        
    JS_DECLARE_NATIVE_FUNCTION(p1_getter);

    JS_DECLARE_NATIVE_FUNCTION(p2_getter);

    JS_DECLARE_NATIVE_FUNCTION(p3_getter);

    JS_DECLARE_NATIVE_FUNCTION(p4_getter);


};


} // namespace Web::Bindings
    