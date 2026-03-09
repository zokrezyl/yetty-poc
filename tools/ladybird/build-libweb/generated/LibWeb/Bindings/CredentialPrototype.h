
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CredentialPrototype : public JS::Object {
    JS_OBJECT(CredentialPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CredentialPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CredentialPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CredentialPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(id_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);


};


enum class CredentialMediationRequirement : u8 {

    Silent,

    Optional,

    Conditional,

    Required,

};

inline String idl_enum_to_string(CredentialMediationRequirement value)
{
    switch (value) {

    case CredentialMediationRequirement::Silent:
        return "silent"_string;

    case CredentialMediationRequirement::Optional:
        return "optional"_string;

    case CredentialMediationRequirement::Conditional:
        return "conditional"_string;

    case CredentialMediationRequirement::Required:
        return "required"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    