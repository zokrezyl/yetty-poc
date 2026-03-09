
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSTransformValuePrototype : public JS::Object {
    JS_OBJECT(CSSTransformValuePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSTransformValuePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSTransformValuePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSTransformValuePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(to_matrix);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);

    JS_DECLARE_NATIVE_FUNCTION(is2d_getter);


};


} // namespace Web::Bindings
    