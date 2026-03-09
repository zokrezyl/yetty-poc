
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class NotificationPrototype : public JS::Object {
    JS_OBJECT(NotificationPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(NotificationPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit NotificationPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~NotificationPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(title_getter);

    JS_DECLARE_NATIVE_FUNCTION(dir_getter);

    JS_DECLARE_NATIVE_FUNCTION(lang_getter);

    JS_DECLARE_NATIVE_FUNCTION(body_getter);

    JS_DECLARE_NATIVE_FUNCTION(navigate_getter);

    JS_DECLARE_NATIVE_FUNCTION(tag_getter);

    JS_DECLARE_NATIVE_FUNCTION(image_getter);

    JS_DECLARE_NATIVE_FUNCTION(icon_getter);

    JS_DECLARE_NATIVE_FUNCTION(badge_getter);

    JS_DECLARE_NATIVE_FUNCTION(timestamp_getter);

    JS_DECLARE_NATIVE_FUNCTION(renotify_getter);

    JS_DECLARE_NATIVE_FUNCTION(silent_getter);

    JS_DECLARE_NATIVE_FUNCTION(require_interaction_getter);

    JS_DECLARE_NATIVE_FUNCTION(data_getter);

    JS_DECLARE_NATIVE_FUNCTION(actions_getter);


};


enum class NotificationPermission : u8 {

    Default,

    Denied,

    Granted,

};

inline String idl_enum_to_string(NotificationPermission value)
{
    switch (value) {

    case NotificationPermission::Default:
        return "default"_string;

    case NotificationPermission::Denied:
        return "denied"_string;

    case NotificationPermission::Granted:
        return "granted"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class NotificationDirection : u8 {

    Auto,

    Ltr,

    Rtl,

};

inline String idl_enum_to_string(NotificationDirection value)
{
    switch (value) {

    case NotificationDirection::Auto:
        return "auto"_string;

    case NotificationDirection::Ltr:
        return "ltr"_string;

    case NotificationDirection::Rtl:
        return "rtl"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    