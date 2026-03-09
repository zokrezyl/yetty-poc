
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class CSSSkewConstructor : public JS::NativeFunction {
    JS_OBJECT(CSSSkewConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(CSSSkewConstructor);
public:
    explicit CSSSkewConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSSkewConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
