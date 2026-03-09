
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class CanvasGradientConstructor : public JS::NativeFunction {
    JS_OBJECT(CanvasGradientConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(CanvasGradientConstructor);
public:
    explicit CanvasGradientConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CanvasGradientConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
