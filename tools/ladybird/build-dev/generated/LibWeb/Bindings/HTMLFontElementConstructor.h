
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class HTMLFontElementConstructor : public JS::NativeFunction {
    JS_OBJECT(HTMLFontElementConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(HTMLFontElementConstructor);
public:
    explicit HTMLFontElementConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLFontElementConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
