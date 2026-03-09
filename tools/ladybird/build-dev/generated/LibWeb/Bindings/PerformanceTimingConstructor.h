
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class PerformanceTimingConstructor : public JS::NativeFunction {
    JS_OBJECT(PerformanceTimingConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(PerformanceTimingConstructor);
public:
    explicit PerformanceTimingConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PerformanceTimingConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
