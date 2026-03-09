
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class StorageManagerConstructor : public JS::NativeFunction {
    JS_OBJECT(StorageManagerConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(StorageManagerConstructor);
public:
    explicit StorageManagerConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~StorageManagerConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
