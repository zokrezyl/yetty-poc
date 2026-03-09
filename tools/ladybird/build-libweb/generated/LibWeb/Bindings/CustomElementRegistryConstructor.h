
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class CustomElementRegistryConstructor : public JS::NativeFunction {
    JS_OBJECT(CustomElementRegistryConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(CustomElementRegistryConstructor);
public:
    explicit CustomElementRegistryConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CustomElementRegistryConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
