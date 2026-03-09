
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class CSSMathNegateConstructor : public JS::NativeFunction {
    JS_OBJECT(CSSMathNegateConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(CSSMathNegateConstructor);
public:
    explicit CSSMathNegateConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSMathNegateConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
