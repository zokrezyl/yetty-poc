
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class DOMMatrixReadOnlyConstructor : public JS::NativeFunction {
    JS_OBJECT(DOMMatrixReadOnlyConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(DOMMatrixReadOnlyConstructor);
public:
    explicit DOMMatrixReadOnlyConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMMatrixReadOnlyConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(from_matrix);

    JS_DECLARE_NATIVE_FUNCTION(from_float32_array);

    JS_DECLARE_NATIVE_FUNCTION(from_float64_array);

};

} // namespace Web::Bindings
