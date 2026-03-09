
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLObjectPrototype : public JS::Object {
    JS_OBJECT(WebGLObjectPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLObjectPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLObjectPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLObjectPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(label_getter);

    JS_DECLARE_NATIVE_FUNCTION(label_setter);


};


} // namespace Web::Bindings
    