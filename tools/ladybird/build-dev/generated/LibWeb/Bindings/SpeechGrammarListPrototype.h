
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SpeechGrammarListPrototype : public JS::Object {
    JS_OBJECT(SpeechGrammarListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SpeechGrammarListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SpeechGrammarListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SpeechGrammarListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(item);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    