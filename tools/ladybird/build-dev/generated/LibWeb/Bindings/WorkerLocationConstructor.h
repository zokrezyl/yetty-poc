
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class WorkerLocationConstructor : public JS::NativeFunction {
    JS_OBJECT(WorkerLocationConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(WorkerLocationConstructor);
public:
    explicit WorkerLocationConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WorkerLocationConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
