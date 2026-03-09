
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class NavigationPrototype : public JS::Object {
    JS_OBJECT(NavigationPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(NavigationPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit NavigationPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~NavigationPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(entries);
        
    JS_DECLARE_NATIVE_FUNCTION(update_current_entry);
        
    JS_DECLARE_NATIVE_FUNCTION(navigate);
        
    JS_DECLARE_NATIVE_FUNCTION(reload);
        
    JS_DECLARE_NATIVE_FUNCTION(traverse_to);
        
    JS_DECLARE_NATIVE_FUNCTION(back);
        
    JS_DECLARE_NATIVE_FUNCTION(forward);
        
    JS_DECLARE_NATIVE_FUNCTION(current_entry_getter);

    JS_DECLARE_NATIVE_FUNCTION(transition_getter);

    JS_DECLARE_NATIVE_FUNCTION(can_go_back_getter);

    JS_DECLARE_NATIVE_FUNCTION(can_go_forward_getter);

    JS_DECLARE_NATIVE_FUNCTION(onnavigate_getter);

    JS_DECLARE_NATIVE_FUNCTION(onnavigate_setter);

    JS_DECLARE_NATIVE_FUNCTION(onnavigatesuccess_getter);

    JS_DECLARE_NATIVE_FUNCTION(onnavigatesuccess_setter);

    JS_DECLARE_NATIVE_FUNCTION(onnavigateerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onnavigateerror_setter);

    JS_DECLARE_NATIVE_FUNCTION(oncurrententrychange_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncurrententrychange_setter);


};


enum class NavigationHistoryBehavior : u8 {

    Auto,

    Push,

    Replace,

};

inline String idl_enum_to_string(NavigationHistoryBehavior value)
{
    switch (value) {

    case NavigationHistoryBehavior::Auto:
        return "auto"_string;

    case NavigationHistoryBehavior::Push:
        return "push"_string;

    case NavigationHistoryBehavior::Replace:
        return "replace"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    