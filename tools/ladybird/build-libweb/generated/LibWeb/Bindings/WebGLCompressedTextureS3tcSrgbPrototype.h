
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLCompressedTextureS3tcSrgbPrototype : public JS::Object {
    JS_OBJECT(WebGLCompressedTextureS3tcSrgbPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLCompressedTextureS3tcSrgbPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLCompressedTextureS3tcSrgbPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLCompressedTextureS3tcSrgbPrototype() override;
private:


};


} // namespace Web::Bindings
    