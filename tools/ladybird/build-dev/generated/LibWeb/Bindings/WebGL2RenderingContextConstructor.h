
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class WebGL2RenderingContextConstructor : public JS::NativeFunction {
    JS_OBJECT(WebGL2RenderingContextConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(WebGL2RenderingContextConstructor);
public:
    explicit WebGL2RenderingContextConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGL2RenderingContextConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
