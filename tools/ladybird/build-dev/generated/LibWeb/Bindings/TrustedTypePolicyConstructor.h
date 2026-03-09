
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class TrustedTypePolicyConstructor : public JS::NativeFunction {
    JS_OBJECT(TrustedTypePolicyConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(TrustedTypePolicyConstructor);
public:
    explicit TrustedTypePolicyConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TrustedTypePolicyConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
