
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLCompressedTextureS3tcPrototype : public JS::Object {
    JS_OBJECT(WebGLCompressedTextureS3tcPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLCompressedTextureS3tcPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLCompressedTextureS3tcPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLCompressedTextureS3tcPrototype() override;
private:


};


} // namespace Web::Bindings
    