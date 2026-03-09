
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class PeriodicWaveConstructor : public JS::NativeFunction {
    JS_OBJECT(PeriodicWaveConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(PeriodicWaveConstructor);
public:
    explicit PeriodicWaveConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PeriodicWaveConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
