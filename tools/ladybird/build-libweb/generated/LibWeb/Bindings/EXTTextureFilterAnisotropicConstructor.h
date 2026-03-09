
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class EXTTextureFilterAnisotropicConstructor : public JS::NativeFunction {
    JS_OBJECT(EXTTextureFilterAnisotropicConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(EXTTextureFilterAnisotropicConstructor);
public:
    explicit EXTTextureFilterAnisotropicConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~EXTTextureFilterAnisotropicConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
