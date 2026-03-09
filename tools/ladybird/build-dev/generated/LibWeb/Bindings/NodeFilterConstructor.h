
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class NodeFilterConstructor : public JS::NativeFunction {
    JS_OBJECT(NodeFilterConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(NodeFilterConstructor);
public:
    explicit NodeFilterConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~NodeFilterConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
