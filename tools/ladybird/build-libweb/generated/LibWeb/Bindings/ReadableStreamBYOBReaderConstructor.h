
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class ReadableStreamBYOBReaderConstructor : public JS::NativeFunction {
    JS_OBJECT(ReadableStreamBYOBReaderConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(ReadableStreamBYOBReaderConstructor);
public:
    explicit ReadableStreamBYOBReaderConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ReadableStreamBYOBReaderConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
