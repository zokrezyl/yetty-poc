
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SpeechRecognitionPhrasePrototype : public JS::Object {
    JS_OBJECT(SpeechRecognitionPhrasePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SpeechRecognitionPhrasePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SpeechRecognitionPhrasePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SpeechRecognitionPhrasePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(phrase_getter);

    JS_DECLARE_NATIVE_FUNCTION(boost_getter);


};


} // namespace Web::Bindings
    