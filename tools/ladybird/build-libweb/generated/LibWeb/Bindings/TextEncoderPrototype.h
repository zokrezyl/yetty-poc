
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TextEncoderPrototype : public JS::Object {
    JS_OBJECT(TextEncoderPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TextEncoderPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TextEncoderPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TextEncoderPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(encode);
        
    JS_DECLARE_NATIVE_FUNCTION(encode_into);
        
    JS_DECLARE_NATIVE_FUNCTION(encoding_getter);


};


} // namespace Web::Bindings
    