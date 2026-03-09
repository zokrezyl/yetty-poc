
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class BlobPrototype : public JS::Object {
    JS_OBJECT(BlobPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(BlobPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit BlobPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~BlobPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(slice);
        
    JS_DECLARE_NATIVE_FUNCTION(stream);
        
    JS_DECLARE_NATIVE_FUNCTION(text);
        
    JS_DECLARE_NATIVE_FUNCTION(array_buffer);
        
    JS_DECLARE_NATIVE_FUNCTION(bytes);
        
    JS_DECLARE_NATIVE_FUNCTION(size_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);


};


enum class EndingType : u8 {

    Transparent,

    Native,

};

inline String idl_enum_to_string(EndingType value)
{
    switch (value) {

    case EndingType::Transparent:
        return "transparent"_string;

    case EndingType::Native:
        return "native"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    