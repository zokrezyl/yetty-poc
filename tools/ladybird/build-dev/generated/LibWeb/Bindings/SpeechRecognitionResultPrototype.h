
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SpeechRecognitionResultPrototype : public JS::Object {
    JS_OBJECT(SpeechRecognitionResultPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SpeechRecognitionResultPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SpeechRecognitionResultPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SpeechRecognitionResultPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(item);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);

    JS_DECLARE_NATIVE_FUNCTION(is_final_getter);


};


} // namespace Web::Bindings
    