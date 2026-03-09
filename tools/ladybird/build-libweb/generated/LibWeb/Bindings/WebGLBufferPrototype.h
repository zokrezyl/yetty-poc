
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLBufferPrototype : public JS::Object {
    JS_OBJECT(WebGLBufferPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLBufferPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLBufferPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLBufferPrototype() override;
private:


};


} // namespace Web::Bindings
    