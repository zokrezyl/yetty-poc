
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class CanvasPatternConstructor : public JS::NativeFunction {
    JS_OBJECT(CanvasPatternConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(CanvasPatternConstructor);
public:
    explicit CanvasPatternConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CanvasPatternConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
