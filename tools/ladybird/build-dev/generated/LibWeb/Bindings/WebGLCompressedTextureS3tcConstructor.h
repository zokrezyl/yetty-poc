
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class WebGLCompressedTextureS3tcConstructor : public JS::NativeFunction {
    JS_OBJECT(WebGLCompressedTextureS3tcConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(WebGLCompressedTextureS3tcConstructor);
public:
    explicit WebGLCompressedTextureS3tcConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLCompressedTextureS3tcConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
