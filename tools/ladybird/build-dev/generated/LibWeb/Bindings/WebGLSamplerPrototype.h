
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLSamplerPrototype : public JS::Object {
    JS_OBJECT(WebGLSamplerPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLSamplerPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLSamplerPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLSamplerPrototype() override;
private:


};


} // namespace Web::Bindings
    