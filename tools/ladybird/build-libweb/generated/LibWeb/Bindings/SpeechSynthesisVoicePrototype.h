
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SpeechSynthesisVoicePrototype : public JS::Object {
    JS_OBJECT(SpeechSynthesisVoicePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SpeechSynthesisVoicePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SpeechSynthesisVoicePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SpeechSynthesisVoicePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(voice_uri_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(lang_getter);

    JS_DECLARE_NATIVE_FUNCTION(local_service_getter);

    JS_DECLARE_NATIVE_FUNCTION(default_getter);


};


} // namespace Web::Bindings
    