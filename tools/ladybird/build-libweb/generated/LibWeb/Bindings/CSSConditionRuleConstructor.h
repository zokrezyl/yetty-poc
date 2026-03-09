
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class CSSConditionRuleConstructor : public JS::NativeFunction {
    JS_OBJECT(CSSConditionRuleConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(CSSConditionRuleConstructor);
public:
    explicit CSSConditionRuleConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSConditionRuleConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
