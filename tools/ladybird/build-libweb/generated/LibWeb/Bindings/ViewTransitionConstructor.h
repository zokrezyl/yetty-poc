
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class ViewTransitionConstructor : public JS::NativeFunction {
    JS_OBJECT(ViewTransitionConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(ViewTransitionConstructor);
public:
    explicit ViewTransitionConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ViewTransitionConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
