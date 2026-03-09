
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class NavigationCurrentEntryChangeEventConstructor : public JS::NativeFunction {
    JS_OBJECT(NavigationCurrentEntryChangeEventConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(NavigationCurrentEntryChangeEventConstructor);
public:
    explicit NavigationCurrentEntryChangeEventConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~NavigationCurrentEntryChangeEventConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
