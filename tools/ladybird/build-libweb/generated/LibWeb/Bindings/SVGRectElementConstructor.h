
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class SVGRectElementConstructor : public JS::NativeFunction {
    JS_OBJECT(SVGRectElementConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(SVGRectElementConstructor);
public:
    explicit SVGRectElementConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGRectElementConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
