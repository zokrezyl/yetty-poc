
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class CSSImageValueConstructor : public JS::NativeFunction {
    JS_OBJECT(CSSImageValueConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(CSSImageValueConstructor);
public:
    explicit CSSImageValueConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSImageValueConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
