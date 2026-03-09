
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLTexturePrototype : public JS::Object {
    JS_OBJECT(WebGLTexturePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLTexturePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLTexturePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLTexturePrototype() override;
private:


};


} // namespace Web::Bindings
    