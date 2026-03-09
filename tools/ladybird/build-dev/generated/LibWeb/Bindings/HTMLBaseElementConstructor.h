
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class HTMLBaseElementConstructor : public JS::NativeFunction {
    JS_OBJECT(HTMLBaseElementConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(HTMLBaseElementConstructor);
public:
    explicit HTMLBaseElementConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLBaseElementConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
