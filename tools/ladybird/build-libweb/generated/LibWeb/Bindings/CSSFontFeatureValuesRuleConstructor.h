
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class CSSFontFeatureValuesRuleConstructor : public JS::NativeFunction {
    JS_OBJECT(CSSFontFeatureValuesRuleConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(CSSFontFeatureValuesRuleConstructor);
public:
    explicit CSSFontFeatureValuesRuleConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSFontFeatureValuesRuleConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
