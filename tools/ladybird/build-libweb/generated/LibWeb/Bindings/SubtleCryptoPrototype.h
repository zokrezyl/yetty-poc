
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SubtleCryptoPrototype : public JS::Object {
    JS_OBJECT(SubtleCryptoPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SubtleCryptoPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SubtleCryptoPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SubtleCryptoPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(encrypt);
        
    JS_DECLARE_NATIVE_FUNCTION(decrypt);
        
    JS_DECLARE_NATIVE_FUNCTION(sign);
        
    JS_DECLARE_NATIVE_FUNCTION(verify);
        
    JS_DECLARE_NATIVE_FUNCTION(digest);
        
    JS_DECLARE_NATIVE_FUNCTION(generate_key);
        
    JS_DECLARE_NATIVE_FUNCTION(derive_key);
        
    JS_DECLARE_NATIVE_FUNCTION(derive_bits);
        
    JS_DECLARE_NATIVE_FUNCTION(import_key);
        
    JS_DECLARE_NATIVE_FUNCTION(export_key);
        
    JS_DECLARE_NATIVE_FUNCTION(wrap_key);
        
    JS_DECLARE_NATIVE_FUNCTION(unwrap_key);
        
    JS_DECLARE_NATIVE_FUNCTION(encapsulate_key);
        
    JS_DECLARE_NATIVE_FUNCTION(encapsulate_bits);
        
    JS_DECLARE_NATIVE_FUNCTION(decapsulate_key);
        
    JS_DECLARE_NATIVE_FUNCTION(decapsulate_bits);
        

};


enum class KeyFormat : u8 {

    RawPublic,

    RawPrivate,

    RawSeed,

    RawSecret,

    Raw,

    Spki,

    Pkcs8,

    Jwk,

};

inline String idl_enum_to_string(KeyFormat value)
{
    switch (value) {

    case KeyFormat::RawPublic:
        return "raw-public"_string;

    case KeyFormat::RawPrivate:
        return "raw-private"_string;

    case KeyFormat::RawSeed:
        return "raw-seed"_string;

    case KeyFormat::RawSecret:
        return "raw-secret"_string;

    case KeyFormat::Raw:
        return "raw"_string;

    case KeyFormat::Spki:
        return "spki"_string;

    case KeyFormat::Pkcs8:
        return "pkcs8"_string;

    case KeyFormat::Jwk:
        return "jwk"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    