
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class CSSStyleValueConstructor : public JS::NativeFunction {
    JS_OBJECT(CSSStyleValueConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(CSSStyleValueConstructor);
public:
    explicit CSSStyleValueConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSStyleValueConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(parse);

    JS_DECLARE_NATIVE_FUNCTION(parse_all);

};

} // namespace Web::Bindings
