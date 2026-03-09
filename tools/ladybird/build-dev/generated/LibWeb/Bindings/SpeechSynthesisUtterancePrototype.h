
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SpeechSynthesisUtterancePrototype : public JS::Object {
    JS_OBJECT(SpeechSynthesisUtterancePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SpeechSynthesisUtterancePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SpeechSynthesisUtterancePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SpeechSynthesisUtterancePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(text_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_setter);

    JS_DECLARE_NATIVE_FUNCTION(lang_getter);

    JS_DECLARE_NATIVE_FUNCTION(lang_setter);

    JS_DECLARE_NATIVE_FUNCTION(voice_getter);

    JS_DECLARE_NATIVE_FUNCTION(voice_setter);

    JS_DECLARE_NATIVE_FUNCTION(volume_getter);

    JS_DECLARE_NATIVE_FUNCTION(volume_setter);

    JS_DECLARE_NATIVE_FUNCTION(rate_getter);

    JS_DECLARE_NATIVE_FUNCTION(rate_setter);

    JS_DECLARE_NATIVE_FUNCTION(pitch_getter);

    JS_DECLARE_NATIVE_FUNCTION(pitch_setter);

    JS_DECLARE_NATIVE_FUNCTION(onstart_getter);

    JS_DECLARE_NATIVE_FUNCTION(onstart_setter);

    JS_DECLARE_NATIVE_FUNCTION(onend_getter);

    JS_DECLARE_NATIVE_FUNCTION(onend_setter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpause_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpause_setter);

    JS_DECLARE_NATIVE_FUNCTION(onresume_getter);

    JS_DECLARE_NATIVE_FUNCTION(onresume_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmark_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmark_setter);

    JS_DECLARE_NATIVE_FUNCTION(onboundary_getter);

    JS_DECLARE_NATIVE_FUNCTION(onboundary_setter);


};


} // namespace Web::Bindings
    