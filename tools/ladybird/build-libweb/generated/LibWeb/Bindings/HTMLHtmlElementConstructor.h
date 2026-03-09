
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class HTMLHtmlElementConstructor : public JS::NativeFunction {
    JS_OBJECT(HTMLHtmlElementConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(HTMLHtmlElementConstructor);
public:
    explicit HTMLHtmlElementConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLHtmlElementConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
