
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ScreenOrientationPrototype : public JS::Object {
    JS_OBJECT(ScreenOrientationPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ScreenOrientationPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ScreenOrientationPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ScreenOrientationPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(lock);
        
    JS_DECLARE_NATIVE_FUNCTION(unlock);
        
    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(angle_getter);

    JS_DECLARE_NATIVE_FUNCTION(onchange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onchange_setter);


};


enum class OrientationLockType : u8 {

    Any,

    Natural,

    Landscape,

    Portrait,

    PortraitPrimary,

    PortraitSecondary,

    LandscapePrimary,

    LandscapeSecondary,

};

inline String idl_enum_to_string(OrientationLockType value)
{
    switch (value) {

    case OrientationLockType::Any:
        return "any"_string;

    case OrientationLockType::Natural:
        return "natural"_string;

    case OrientationLockType::Landscape:
        return "landscape"_string;

    case OrientationLockType::Portrait:
        return "portrait"_string;

    case OrientationLockType::PortraitPrimary:
        return "portrait-primary"_string;

    case OrientationLockType::PortraitSecondary:
        return "portrait-secondary"_string;

    case OrientationLockType::LandscapePrimary:
        return "landscape-primary"_string;

    case OrientationLockType::LandscapeSecondary:
        return "landscape-secondary"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class OrientationType : u8 {

    PortraitPrimary,

    PortraitSecondary,

    LandscapePrimary,

    LandscapeSecondary,

};

inline String idl_enum_to_string(OrientationType value)
{
    switch (value) {

    case OrientationType::PortraitPrimary:
        return "portrait-primary"_string;

    case OrientationType::PortraitSecondary:
        return "portrait-secondary"_string;

    case OrientationType::LandscapePrimary:
        return "landscape-primary"_string;

    case OrientationType::LandscapeSecondary:
        return "landscape-secondary"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    