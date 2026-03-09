
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ShadowRootPrototype : public JS::Object {
    JS_OBJECT(ShadowRootPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ShadowRootPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ShadowRootPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ShadowRootPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(set_html_unsafe);
        
    JS_DECLARE_NATIVE_FUNCTION(get_html);
        
    JS_DECLARE_NATIVE_FUNCTION(get_animations);
        
    JS_DECLARE_NATIVE_FUNCTION(mode_getter);

    JS_DECLARE_NATIVE_FUNCTION(delegates_focus_getter);

    JS_DECLARE_NATIVE_FUNCTION(slot_assignment_getter);

    JS_DECLARE_NATIVE_FUNCTION(clonable_getter);

    JS_DECLARE_NATIVE_FUNCTION(serializable_getter);

    JS_DECLARE_NATIVE_FUNCTION(host_getter);

    JS_DECLARE_NATIVE_FUNCTION(onslotchange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onslotchange_setter);

    JS_DECLARE_NATIVE_FUNCTION(inner_html_getter);

    JS_DECLARE_NATIVE_FUNCTION(inner_html_setter);

    JS_DECLARE_NATIVE_FUNCTION(active_element_getter);

    JS_DECLARE_NATIVE_FUNCTION(style_sheets_getter);

    JS_DECLARE_NATIVE_FUNCTION(adopted_style_sheets_getter);

    JS_DECLARE_NATIVE_FUNCTION(adopted_style_sheets_setter);

    JS_DECLARE_NATIVE_FUNCTION(fullscreen_element_getter);

    JS_DECLARE_NATIVE_FUNCTION(fullscreen_element_setter);


};


enum class SlotAssignmentMode : u8 {

    Manual,

    Named,

};

inline String idl_enum_to_string(SlotAssignmentMode value)
{
    switch (value) {

    case SlotAssignmentMode::Manual:
        return "manual"_string;

    case SlotAssignmentMode::Named:
        return "named"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class ShadowRootMode : u8 {

    Open,

    Closed,

};

inline String idl_enum_to_string(ShadowRootMode value)
{
    switch (value) {

    case ShadowRootMode::Open:
        return "open"_string;

    case ShadowRootMode::Closed:
        return "closed"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    