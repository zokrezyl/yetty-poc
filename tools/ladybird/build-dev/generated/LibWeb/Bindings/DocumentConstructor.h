
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class DocumentConstructor : public JS::NativeFunction {
    JS_OBJECT(DocumentConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(DocumentConstructor);
public:
    explicit DocumentConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DocumentConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(parse_html_unsafe);

};

} // namespace Web::Bindings
