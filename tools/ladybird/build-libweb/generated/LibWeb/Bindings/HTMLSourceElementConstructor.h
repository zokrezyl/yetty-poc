
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class HTMLSourceElementConstructor : public JS::NativeFunction {
    JS_OBJECT(HTMLSourceElementConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(HTMLSourceElementConstructor);
public:
    explicit HTMLSourceElementConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLSourceElementConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
