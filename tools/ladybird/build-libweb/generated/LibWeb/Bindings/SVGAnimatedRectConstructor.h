
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class SVGAnimatedRectConstructor : public JS::NativeFunction {
    JS_OBJECT(SVGAnimatedRectConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(SVGAnimatedRectConstructor);
public:
    explicit SVGAnimatedRectConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGAnimatedRectConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
