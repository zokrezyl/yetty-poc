
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class HTMLMediaElementConstructor : public JS::NativeFunction {
    JS_OBJECT(HTMLMediaElementConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(HTMLMediaElementConstructor);
public:
    explicit HTMLMediaElementConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLMediaElementConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
