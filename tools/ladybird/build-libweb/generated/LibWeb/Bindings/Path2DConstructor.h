
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class Path2DConstructor : public JS::NativeFunction {
    JS_OBJECT(Path2DConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(Path2DConstructor);
public:
    explicit Path2DConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~Path2DConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
