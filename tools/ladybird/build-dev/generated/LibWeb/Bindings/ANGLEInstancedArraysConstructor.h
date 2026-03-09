
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class ANGLEInstancedArraysConstructor : public JS::NativeFunction {
    JS_OBJECT(ANGLEInstancedArraysConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(ANGLEInstancedArraysConstructor);
public:
    explicit ANGLEInstancedArraysConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ANGLEInstancedArraysConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
