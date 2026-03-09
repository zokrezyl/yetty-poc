
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLUniformLocationPrototype : public JS::Object {
    JS_OBJECT(WebGLUniformLocationPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLUniformLocationPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLUniformLocationPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLUniformLocationPrototype() override;
private:


};


} // namespace Web::Bindings
    