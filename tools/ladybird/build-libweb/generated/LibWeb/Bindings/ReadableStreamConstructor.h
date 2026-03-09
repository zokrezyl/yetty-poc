
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class ReadableStreamConstructor : public JS::NativeFunction {
    JS_OBJECT(ReadableStreamConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(ReadableStreamConstructor);
public:
    explicit ReadableStreamConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ReadableStreamConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(from);

};

} // namespace Web::Bindings
