
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class AbortControllerConstructor : public JS::NativeFunction {
    JS_OBJECT(AbortControllerConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(AbortControllerConstructor);
public:
    explicit AbortControllerConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AbortControllerConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
