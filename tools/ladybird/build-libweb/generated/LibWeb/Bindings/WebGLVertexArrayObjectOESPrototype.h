
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLVertexArrayObjectOESPrototype : public JS::Object {
    JS_OBJECT(WebGLVertexArrayObjectOESPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLVertexArrayObjectOESPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLVertexArrayObjectOESPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLVertexArrayObjectOESPrototype() override;
private:


};


} // namespace Web::Bindings
    