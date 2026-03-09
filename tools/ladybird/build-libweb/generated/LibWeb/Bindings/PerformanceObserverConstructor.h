
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class PerformanceObserverConstructor : public JS::NativeFunction {
    JS_OBJECT(PerformanceObserverConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(PerformanceObserverConstructor);
public:
    explicit PerformanceObserverConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PerformanceObserverConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(supported_entry_types_getter);

};

} // namespace Web::Bindings
