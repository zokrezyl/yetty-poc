
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class WebGLRenderbufferConstructor : public JS::NativeFunction {
    JS_OBJECT(WebGLRenderbufferConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(WebGLRenderbufferConstructor);
public:
    explicit WebGLRenderbufferConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLRenderbufferConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
