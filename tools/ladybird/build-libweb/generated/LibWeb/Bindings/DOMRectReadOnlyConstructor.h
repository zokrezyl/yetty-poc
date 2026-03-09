
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class DOMRectReadOnlyConstructor : public JS::NativeFunction {
    JS_OBJECT(DOMRectReadOnlyConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(DOMRectReadOnlyConstructor);
public:
    explicit DOMRectReadOnlyConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMRectReadOnlyConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(from_rect);

};

} // namespace Web::Bindings
