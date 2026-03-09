
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSTransformComponentPrototype : public JS::Object {
    JS_OBJECT(CSSTransformComponentPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSTransformComponentPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSTransformComponentPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSTransformComponentPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(to_matrix);
        
    JS_DECLARE_NATIVE_FUNCTION(to_string);
        
    JS_DECLARE_NATIVE_FUNCTION(is2d_getter);

    JS_DECLARE_NATIVE_FUNCTION(is2d_setter);


};


} // namespace Web::Bindings
    