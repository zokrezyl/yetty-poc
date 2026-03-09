
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class OffscreenCanvasRenderingContext2DConstructor : public JS::NativeFunction {
    JS_OBJECT(OffscreenCanvasRenderingContext2DConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(OffscreenCanvasRenderingContext2DConstructor);
public:
    explicit OffscreenCanvasRenderingContext2DConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~OffscreenCanvasRenderingContext2DConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
