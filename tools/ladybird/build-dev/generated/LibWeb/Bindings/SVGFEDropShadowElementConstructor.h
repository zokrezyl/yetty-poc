
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class SVGFEDropShadowElementConstructor : public JS::NativeFunction {
    JS_OBJECT(SVGFEDropShadowElementConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(SVGFEDropShadowElementConstructor);
public:
    explicit SVGFEDropShadowElementConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGFEDropShadowElementConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
