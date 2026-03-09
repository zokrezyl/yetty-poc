
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MediaKeySystemAccessPrototype : public JS::Object {
    JS_OBJECT(MediaKeySystemAccessPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MediaKeySystemAccessPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MediaKeySystemAccessPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MediaKeySystemAccessPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_configuration);
        
    JS_DECLARE_NATIVE_FUNCTION(key_system_getter);


};


enum class MediaKeySessionType : u8 {

    Temporary,

    PersistentLicense,

};

inline String idl_enum_to_string(MediaKeySessionType value)
{
    switch (value) {

    case MediaKeySessionType::Temporary:
        return "temporary"_string;

    case MediaKeySessionType::PersistentLicense:
        return "persistent-license"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class MediaKeysRequirement : u8 {

    Required,

    Optional,

    NotAllowed,

};

inline String idl_enum_to_string(MediaKeysRequirement value)
{
    switch (value) {

    case MediaKeysRequirement::Required:
        return "required"_string;

    case MediaKeysRequirement::Optional:
        return "optional"_string;

    case MediaKeysRequirement::NotAllowed:
        return "not-allowed"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    