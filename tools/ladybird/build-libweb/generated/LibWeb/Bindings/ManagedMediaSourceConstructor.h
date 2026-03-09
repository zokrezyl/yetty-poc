
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class ManagedMediaSourceConstructor : public JS::NativeFunction {
    JS_OBJECT(ManagedMediaSourceConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(ManagedMediaSourceConstructor);
public:
    explicit ManagedMediaSourceConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ManagedMediaSourceConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
