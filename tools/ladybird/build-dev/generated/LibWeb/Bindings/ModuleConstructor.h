
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class ModuleConstructor : public JS::NativeFunction {
    JS_OBJECT(ModuleConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(ModuleConstructor);
public:
    explicit ModuleConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ModuleConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(exports);

    JS_DECLARE_NATIVE_FUNCTION(imports);

    JS_DECLARE_NATIVE_FUNCTION(custom_sections);

};

} // namespace Web::Bindings
