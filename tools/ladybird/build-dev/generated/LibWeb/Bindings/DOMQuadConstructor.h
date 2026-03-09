
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class DOMQuadConstructor : public JS::NativeFunction {
    JS_OBJECT(DOMQuadConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(DOMQuadConstructor);
public:
    explicit DOMQuadConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMQuadConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(from_rect);

    JS_DECLARE_NATIVE_FUNCTION(from_quad);

};

} // namespace Web::Bindings
