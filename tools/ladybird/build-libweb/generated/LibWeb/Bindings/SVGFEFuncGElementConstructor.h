
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class SVGFEFuncGElementConstructor : public JS::NativeFunction {
    JS_OBJECT(SVGFEFuncGElementConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(SVGFEFuncGElementConstructor);
public:
    explicit SVGFEFuncGElementConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGFEFuncGElementConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
