
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class HTMLHeadingElementConstructor : public JS::NativeFunction {
    JS_OBJECT(HTMLHeadingElementConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(HTMLHeadingElementConstructor);
public:
    explicit HTMLHeadingElementConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLHeadingElementConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
