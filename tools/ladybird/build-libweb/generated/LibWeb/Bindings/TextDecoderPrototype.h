
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TextDecoderPrototype : public JS::Object {
    JS_OBJECT(TextDecoderPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TextDecoderPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TextDecoderPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TextDecoderPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(decode);
        
    JS_DECLARE_NATIVE_FUNCTION(encoding_getter);

    JS_DECLARE_NATIVE_FUNCTION(fatal_getter);

    JS_DECLARE_NATIVE_FUNCTION(ignore_bom_getter);


};


} // namespace Web::Bindings
    