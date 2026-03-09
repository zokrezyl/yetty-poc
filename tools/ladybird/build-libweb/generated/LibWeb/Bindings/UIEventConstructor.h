
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class UIEventConstructor : public JS::NativeFunction {
    JS_OBJECT(UIEventConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(UIEventConstructor);
public:
    explicit UIEventConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~UIEventConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
