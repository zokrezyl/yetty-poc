
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class SpeechSynthesisConstructor : public JS::NativeFunction {
    JS_OBJECT(SpeechSynthesisConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(SpeechSynthesisConstructor);
public:
    explicit SpeechSynthesisConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SpeechSynthesisConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
