
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class IDBKeyRangeConstructor : public JS::NativeFunction {
    JS_OBJECT(IDBKeyRangeConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(IDBKeyRangeConstructor);
public:
    explicit IDBKeyRangeConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~IDBKeyRangeConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(only);

    JS_DECLARE_NATIVE_FUNCTION(lower_bound);

    JS_DECLARE_NATIVE_FUNCTION(upper_bound);

    JS_DECLARE_NATIVE_FUNCTION(bound);

};

} // namespace Web::Bindings
