
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class PerformanceObserverEntryListConstructor : public JS::NativeFunction {
    JS_OBJECT(PerformanceObserverEntryListConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(PerformanceObserverEntryListConstructor);
public:
    explicit PerformanceObserverEntryListConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PerformanceObserverEntryListConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
