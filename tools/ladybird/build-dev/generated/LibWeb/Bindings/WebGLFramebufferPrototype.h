
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLFramebufferPrototype : public JS::Object {
    JS_OBJECT(WebGLFramebufferPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLFramebufferPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLFramebufferPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLFramebufferPrototype() override;
private:


};


} // namespace Web::Bindings
    