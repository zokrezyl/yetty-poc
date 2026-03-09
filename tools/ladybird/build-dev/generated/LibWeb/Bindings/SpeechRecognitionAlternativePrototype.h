
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SpeechRecognitionAlternativePrototype : public JS::Object {
    JS_OBJECT(SpeechRecognitionAlternativePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SpeechRecognitionAlternativePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SpeechRecognitionAlternativePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SpeechRecognitionAlternativePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(transcript_getter);

    JS_DECLARE_NATIVE_FUNCTION(confidence_getter);


};


} // namespace Web::Bindings
    