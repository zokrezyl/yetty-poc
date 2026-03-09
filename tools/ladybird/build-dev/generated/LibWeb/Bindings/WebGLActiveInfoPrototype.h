
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLActiveInfoPrototype : public JS::Object {
    JS_OBJECT(WebGLActiveInfoPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLActiveInfoPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLActiveInfoPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLActiveInfoPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(size_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_getter);


};


} // namespace Web::Bindings
    