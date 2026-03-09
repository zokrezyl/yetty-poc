
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CryptoKeyPrototype : public JS::Object {
    JS_OBJECT(CryptoKeyPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CryptoKeyPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CryptoKeyPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CryptoKeyPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(extractable_getter);

    JS_DECLARE_NATIVE_FUNCTION(algorithm_getter);

    JS_DECLARE_NATIVE_FUNCTION(usages_getter);


};


enum class KeyType : u8 {

    Public,

    Private,

    Secret,

};

inline String idl_enum_to_string(KeyType value)
{
    switch (value) {

    case KeyType::Public:
        return "public"_string;

    case KeyType::Private:
        return "private"_string;

    case KeyType::Secret:
        return "secret"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class KeyUsage : u8 {

    Encrypt,

    Decrypt,

    Sign,

    Verify,

    Derivekey,

    Derivebits,

    Wrapkey,

    Unwrapkey,

    Encapsulatekey,

    Encapsulatebits,

    Decapsulatekey,

    Decapsulatebits,

};

inline String idl_enum_to_string(KeyUsage value)
{
    switch (value) {

    case KeyUsage::Encrypt:
        return "encrypt"_string;

    case KeyUsage::Decrypt:
        return "decrypt"_string;

    case KeyUsage::Sign:
        return "sign"_string;

    case KeyUsage::Verify:
        return "verify"_string;

    case KeyUsage::Derivekey:
        return "deriveKey"_string;

    case KeyUsage::Derivebits:
        return "deriveBits"_string;

    case KeyUsage::Wrapkey:
        return "wrapKey"_string;

    case KeyUsage::Unwrapkey:
        return "unwrapKey"_string;

    case KeyUsage::Encapsulatekey:
        return "encapsulateKey"_string;

    case KeyUsage::Encapsulatebits:
        return "encapsulateBits"_string;

    case KeyUsage::Decapsulatekey:
        return "decapsulateKey"_string;

    case KeyUsage::Decapsulatebits:
        return "decapsulateBits"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    