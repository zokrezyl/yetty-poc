
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLShaderPrototype : public JS::Object {
    JS_OBJECT(WebGLShaderPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLShaderPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLShaderPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLShaderPrototype() override;
private:


};


} // namespace Web::Bindings
    