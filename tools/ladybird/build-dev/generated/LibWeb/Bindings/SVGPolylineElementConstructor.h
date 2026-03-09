
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class SVGPolylineElementConstructor : public JS::NativeFunction {
    JS_OBJECT(SVGPolylineElementConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(SVGPolylineElementConstructor);
public:
    explicit SVGPolylineElementConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGPolylineElementConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
