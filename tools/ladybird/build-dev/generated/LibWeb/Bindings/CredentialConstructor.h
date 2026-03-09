
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class CredentialConstructor : public JS::NativeFunction {
    JS_OBJECT(CredentialConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(CredentialConstructor);
public:
    explicit CredentialConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CredentialConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(is_conditional_mediation_available);

};

} // namespace Web::Bindings
