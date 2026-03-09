
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class HTMLScriptElementConstructor : public JS::NativeFunction {
    JS_OBJECT(HTMLScriptElementConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(HTMLScriptElementConstructor);
public:
    explicit HTMLScriptElementConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLScriptElementConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(supports);

};

} // namespace Web::Bindings
