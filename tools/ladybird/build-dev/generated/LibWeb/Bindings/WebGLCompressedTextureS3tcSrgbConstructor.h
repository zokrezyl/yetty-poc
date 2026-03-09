
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class WebGLCompressedTextureS3tcSrgbConstructor : public JS::NativeFunction {
    JS_OBJECT(WebGLCompressedTextureS3tcSrgbConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(WebGLCompressedTextureS3tcSrgbConstructor);
public:
    explicit WebGLCompressedTextureS3tcSrgbConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLCompressedTextureS3tcSrgbConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
