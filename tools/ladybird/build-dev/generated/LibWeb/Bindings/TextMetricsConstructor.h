
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class TextMetricsConstructor : public JS::NativeFunction {
    JS_OBJECT(TextMetricsConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(TextMetricsConstructor);
public:
    explicit TextMetricsConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TextMetricsConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
