
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class AbortSignalConstructor : public JS::NativeFunction {
    JS_OBJECT(AbortSignalConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(AbortSignalConstructor);
public:
    explicit AbortSignalConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AbortSignalConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(abort);

    JS_DECLARE_NATIVE_FUNCTION(timeout);

    JS_DECLARE_NATIVE_FUNCTION(any);

};

} // namespace Web::Bindings
