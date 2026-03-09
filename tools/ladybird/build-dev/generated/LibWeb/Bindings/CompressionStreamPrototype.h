
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CompressionStreamPrototype : public JS::Object {
    JS_OBJECT(CompressionStreamPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CompressionStreamPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CompressionStreamPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CompressionStreamPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(readable_getter);

    JS_DECLARE_NATIVE_FUNCTION(writable_getter);


};


enum class CompressionFormat : u8 {

    Deflate,

    DeflateRaw,

    Gzip,

};

inline String idl_enum_to_string(CompressionFormat value)
{
    switch (value) {

    case CompressionFormat::Deflate:
        return "deflate"_string;

    case CompressionFormat::DeflateRaw:
        return "deflate-raw"_string;

    case CompressionFormat::Gzip:
        return "gzip"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    