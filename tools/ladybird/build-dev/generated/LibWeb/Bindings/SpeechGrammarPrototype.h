
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SpeechGrammarPrototype : public JS::Object {
    JS_OBJECT(SpeechGrammarPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SpeechGrammarPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SpeechGrammarPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SpeechGrammarPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(src_getter);

    JS_DECLARE_NATIVE_FUNCTION(src_setter);

    JS_DECLARE_NATIVE_FUNCTION(weight_getter);

    JS_DECLARE_NATIVE_FUNCTION(weight_setter);


};


} // namespace Web::Bindings
    