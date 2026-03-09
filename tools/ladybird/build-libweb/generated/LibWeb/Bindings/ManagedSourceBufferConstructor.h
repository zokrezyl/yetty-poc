
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class ManagedSourceBufferConstructor : public JS::NativeFunction {
    JS_OBJECT(ManagedSourceBufferConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(ManagedSourceBufferConstructor);
public:
    explicit ManagedSourceBufferConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ManagedSourceBufferConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
