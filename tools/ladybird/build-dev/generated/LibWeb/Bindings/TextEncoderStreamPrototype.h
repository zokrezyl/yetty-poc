
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TextEncoderStreamPrototype : public JS::Object {
    JS_OBJECT(TextEncoderStreamPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TextEncoderStreamPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TextEncoderStreamPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TextEncoderStreamPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(encoding_getter);

    JS_DECLARE_NATIVE_FUNCTION(readable_getter);

    JS_DECLARE_NATIVE_FUNCTION(writable_getter);


};


} // namespace Web::Bindings
    