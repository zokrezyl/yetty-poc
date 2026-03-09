
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SpeechRecognitionEventPrototype : public JS::Object {
    JS_OBJECT(SpeechRecognitionEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SpeechRecognitionEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SpeechRecognitionEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SpeechRecognitionEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(result_index_getter);

    JS_DECLARE_NATIVE_FUNCTION(results_getter);


};


} // namespace Web::Bindings
    