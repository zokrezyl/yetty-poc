
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class ClipboardItemConstructor : public JS::NativeFunction {
    JS_OBJECT(ClipboardItemConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(ClipboardItemConstructor);
public:
    explicit ClipboardItemConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ClipboardItemConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(supports);

};

} // namespace Web::Bindings
