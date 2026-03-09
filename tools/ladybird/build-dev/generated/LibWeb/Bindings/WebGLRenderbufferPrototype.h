
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLRenderbufferPrototype : public JS::Object {
    JS_OBJECT(WebGLRenderbufferPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLRenderbufferPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLRenderbufferPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLRenderbufferPrototype() override;
private:


};


} // namespace Web::Bindings
    