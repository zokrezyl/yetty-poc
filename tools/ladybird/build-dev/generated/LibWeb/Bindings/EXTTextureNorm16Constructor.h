
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class EXTTextureNorm16Constructor : public JS::NativeFunction {
    JS_OBJECT(EXTTextureNorm16Constructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(EXTTextureNorm16Constructor);
public:
    explicit EXTTextureNorm16Constructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~EXTTextureNorm16Constructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
