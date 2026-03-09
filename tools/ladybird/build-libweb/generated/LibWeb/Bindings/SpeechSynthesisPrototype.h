
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SpeechSynthesisPrototype : public JS::Object {
    JS_OBJECT(SpeechSynthesisPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SpeechSynthesisPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SpeechSynthesisPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SpeechSynthesisPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(cancel);
        
    JS_DECLARE_NATIVE_FUNCTION(get_voices);
        
    JS_DECLARE_NATIVE_FUNCTION(pending_getter);

    JS_DECLARE_NATIVE_FUNCTION(speaking_getter);

    JS_DECLARE_NATIVE_FUNCTION(paused_getter);

    JS_DECLARE_NATIVE_FUNCTION(onvoiceschanged_getter);

    JS_DECLARE_NATIVE_FUNCTION(onvoiceschanged_setter);


};


} // namespace Web::Bindings
    