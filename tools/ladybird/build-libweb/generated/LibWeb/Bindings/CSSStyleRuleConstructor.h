
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class CSSStyleRuleConstructor : public JS::NativeFunction {
    JS_OBJECT(CSSStyleRuleConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(CSSStyleRuleConstructor);
public:
    explicit CSSStyleRuleConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSStyleRuleConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
