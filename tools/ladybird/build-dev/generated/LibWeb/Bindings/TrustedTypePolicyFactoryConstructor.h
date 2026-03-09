
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class TrustedTypePolicyFactoryConstructor : public JS::NativeFunction {
    JS_OBJECT(TrustedTypePolicyFactoryConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(TrustedTypePolicyFactoryConstructor);
public:
    explicit TrustedTypePolicyFactoryConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TrustedTypePolicyFactoryConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
