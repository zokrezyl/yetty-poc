
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLVertexArrayObjectPrototype : public JS::Object {
    JS_OBJECT(WebGLVertexArrayObjectPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLVertexArrayObjectPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLVertexArrayObjectPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLVertexArrayObjectPrototype() override;
private:


};


} // namespace Web::Bindings
    