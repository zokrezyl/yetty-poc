
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class GamepadEventConstructor : public JS::NativeFunction {
    JS_OBJECT(GamepadEventConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(GamepadEventConstructor);
public:
    explicit GamepadEventConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~GamepadEventConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
