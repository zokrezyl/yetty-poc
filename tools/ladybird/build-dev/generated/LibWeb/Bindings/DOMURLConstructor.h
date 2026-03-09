
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class DOMURLConstructor : public JS::NativeFunction {
    JS_OBJECT(DOMURLConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(DOMURLConstructor);
public:
    explicit DOMURLConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMURLConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(parse);

    JS_DECLARE_NATIVE_FUNCTION(can_parse);

    JS_DECLARE_NATIVE_FUNCTION(create_object_url);

    JS_DECLARE_NATIVE_FUNCTION(revoke_object_url);

};

} // namespace Web::Bindings
