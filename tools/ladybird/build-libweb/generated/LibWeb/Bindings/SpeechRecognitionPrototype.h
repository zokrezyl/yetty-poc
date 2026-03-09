
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SpeechRecognitionPrototype : public JS::Object {
    JS_OBJECT(SpeechRecognitionPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SpeechRecognitionPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SpeechRecognitionPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SpeechRecognitionPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(grammars_getter);

    JS_DECLARE_NATIVE_FUNCTION(grammars_setter);

    JS_DECLARE_NATIVE_FUNCTION(lang_getter);

    JS_DECLARE_NATIVE_FUNCTION(lang_setter);

    JS_DECLARE_NATIVE_FUNCTION(continuous_getter);

    JS_DECLARE_NATIVE_FUNCTION(continuous_setter);

    JS_DECLARE_NATIVE_FUNCTION(interim_results_getter);

    JS_DECLARE_NATIVE_FUNCTION(interim_results_setter);

    JS_DECLARE_NATIVE_FUNCTION(max_alternatives_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_alternatives_setter);

    JS_DECLARE_NATIVE_FUNCTION(onaudiostart_getter);

    JS_DECLARE_NATIVE_FUNCTION(onaudiostart_setter);

    JS_DECLARE_NATIVE_FUNCTION(onsoundstart_getter);

    JS_DECLARE_NATIVE_FUNCTION(onsoundstart_setter);

    JS_DECLARE_NATIVE_FUNCTION(onspeechstart_getter);

    JS_DECLARE_NATIVE_FUNCTION(onspeechstart_setter);

    JS_DECLARE_NATIVE_FUNCTION(onspeechend_getter);

    JS_DECLARE_NATIVE_FUNCTION(onspeechend_setter);

    JS_DECLARE_NATIVE_FUNCTION(onsoundend_getter);

    JS_DECLARE_NATIVE_FUNCTION(onsoundend_setter);

    JS_DECLARE_NATIVE_FUNCTION(onaudioend_getter);

    JS_DECLARE_NATIVE_FUNCTION(onaudioend_setter);

    JS_DECLARE_NATIVE_FUNCTION(onresult_getter);

    JS_DECLARE_NATIVE_FUNCTION(onresult_setter);

    JS_DECLARE_NATIVE_FUNCTION(onnomatch_getter);

    JS_DECLARE_NATIVE_FUNCTION(onnomatch_setter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_setter);

    JS_DECLARE_NATIVE_FUNCTION(onstart_getter);

    JS_DECLARE_NATIVE_FUNCTION(onstart_setter);

    JS_DECLARE_NATIVE_FUNCTION(onend_getter);

    JS_DECLARE_NATIVE_FUNCTION(onend_setter);


};


} // namespace Web::Bindings
    