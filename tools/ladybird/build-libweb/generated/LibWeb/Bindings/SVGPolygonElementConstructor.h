
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class SVGPolygonElementConstructor : public JS::NativeFunction {
    JS_OBJECT(SVGPolygonElementConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(SVGPolygonElementConstructor);
public:
    explicit SVGPolygonElementConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGPolygonElementConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
