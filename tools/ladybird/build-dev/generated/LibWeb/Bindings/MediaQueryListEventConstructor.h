
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class MediaQueryListEventConstructor : public JS::NativeFunction {
    JS_OBJECT(MediaQueryListEventConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(MediaQueryListEventConstructor);
public:
    explicit MediaQueryListEventConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MediaQueryListEventConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
