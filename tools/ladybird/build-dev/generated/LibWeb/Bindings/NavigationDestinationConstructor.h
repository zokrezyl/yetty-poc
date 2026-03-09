
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class NavigationDestinationConstructor : public JS::NativeFunction {
    JS_OBJECT(NavigationDestinationConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(NavigationDestinationConstructor);
public:
    explicit NavigationDestinationConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~NavigationDestinationConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
