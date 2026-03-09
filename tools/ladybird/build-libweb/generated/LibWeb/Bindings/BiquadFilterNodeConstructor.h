
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class BiquadFilterNodeConstructor : public JS::NativeFunction {
    JS_OBJECT(BiquadFilterNodeConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(BiquadFilterNodeConstructor);
public:
    explicit BiquadFilterNodeConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~BiquadFilterNodeConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
