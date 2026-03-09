
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class XMLDocumentConstructor : public JS::NativeFunction {
    JS_OBJECT(XMLDocumentConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(XMLDocumentConstructor);
public:
    explicit XMLDocumentConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~XMLDocumentConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
