
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class ResizeObserverSizeConstructor : public JS::NativeFunction {
    JS_OBJECT(ResizeObserverSizeConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(ResizeObserverSizeConstructor);
public:
    explicit ResizeObserverSizeConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ResizeObserverSizeConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
