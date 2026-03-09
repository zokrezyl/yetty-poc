
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class DragEventConstructor : public JS::NativeFunction {
    JS_OBJECT(DragEventConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(DragEventConstructor);
public:
    explicit DragEventConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DragEventConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
