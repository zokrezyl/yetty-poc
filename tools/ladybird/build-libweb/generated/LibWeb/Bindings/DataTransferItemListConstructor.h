
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class DataTransferItemListConstructor : public JS::NativeFunction {
    JS_OBJECT(DataTransferItemListConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(DataTransferItemListConstructor);
public:
    explicit DataTransferItemListConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DataTransferItemListConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
