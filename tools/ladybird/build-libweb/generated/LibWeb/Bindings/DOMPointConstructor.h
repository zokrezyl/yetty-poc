
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class DOMPointConstructor : public JS::NativeFunction {
    JS_OBJECT(DOMPointConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(DOMPointConstructor);
public:
    explicit DOMPointConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMPointConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(from_point);

};

} // namespace Web::Bindings
