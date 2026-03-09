
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class HTMLTableSectionElementConstructor : public JS::NativeFunction {
    JS_OBJECT(HTMLTableSectionElementConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(HTMLTableSectionElementConstructor);
public:
    explicit HTMLTableSectionElementConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLTableSectionElementConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
