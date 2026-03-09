
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class AudioDestinationNodeConstructor : public JS::NativeFunction {
    JS_OBJECT(AudioDestinationNodeConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(AudioDestinationNodeConstructor);
public:
    explicit AudioDestinationNodeConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AudioDestinationNodeConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
