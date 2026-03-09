
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class ErrorEventConstructor : public JS::NativeFunction {
    JS_OBJECT(ErrorEventConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(ErrorEventConstructor);
public:
    explicit ErrorEventConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ErrorEventConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
