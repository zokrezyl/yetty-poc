
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class NavigateEventPrototype : public JS::Object {
    JS_OBJECT(NavigateEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(NavigateEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit NavigateEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~NavigateEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(intercept);
        
    JS_DECLARE_NATIVE_FUNCTION(scroll);
        
    JS_DECLARE_NATIVE_FUNCTION(navigation_type_getter);

    JS_DECLARE_NATIVE_FUNCTION(destination_getter);

    JS_DECLARE_NATIVE_FUNCTION(can_intercept_getter);

    JS_DECLARE_NATIVE_FUNCTION(user_initiated_getter);

    JS_DECLARE_NATIVE_FUNCTION(hash_change_getter);

    JS_DECLARE_NATIVE_FUNCTION(signal_getter);

    JS_DECLARE_NATIVE_FUNCTION(form_data_getter);

    JS_DECLARE_NATIVE_FUNCTION(download_request_getter);

    JS_DECLARE_NATIVE_FUNCTION(info_getter);

    JS_DECLARE_NATIVE_FUNCTION(has_ua_visual_transition_getter);

    JS_DECLARE_NATIVE_FUNCTION(source_element_getter);


};


enum class NavigationFocusReset : u8 {

    AfterTransition,

    Manual,

};

inline String idl_enum_to_string(NavigationFocusReset value)
{
    switch (value) {

    case NavigationFocusReset::AfterTransition:
        return "after-transition"_string;

    case NavigationFocusReset::Manual:
        return "manual"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class NavigationScrollBehavior : u8 {

    AfterTransition,

    Manual,

};

inline String idl_enum_to_string(NavigationScrollBehavior value)
{
    switch (value) {

    case NavigationScrollBehavior::AfterTransition:
        return "after-transition"_string;

    case NavigationScrollBehavior::Manual:
        return "manual"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    