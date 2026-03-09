
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class HTMLObjectElementConstructor : public JS::NativeFunction {
    JS_OBJECT(HTMLObjectElementConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(HTMLObjectElementConstructor);
public:
    explicit HTMLObjectElementConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLObjectElementConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
