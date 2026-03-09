
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLDrawBuffersPrototype : public JS::Object {
    JS_OBJECT(WebGLDrawBuffersPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLDrawBuffersPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLDrawBuffersPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLDrawBuffersPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(draw_buffers_webgl);
        

};


} // namespace Web::Bindings
    