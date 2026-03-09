
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HistoryPrototype : public JS::Object {
    JS_OBJECT(HistoryPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HistoryPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HistoryPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HistoryPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(go);
        
    JS_DECLARE_NATIVE_FUNCTION(back);
        
    JS_DECLARE_NATIVE_FUNCTION(forward);
        
    JS_DECLARE_NATIVE_FUNCTION(push_state);
        
    JS_DECLARE_NATIVE_FUNCTION(replace_state);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_restoration_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_restoration_setter);

    JS_DECLARE_NATIVE_FUNCTION(state_getter);


};


enum class ScrollRestoration : u8 {

    Auto,

    Manual,

};

inline String idl_enum_to_string(ScrollRestoration value)
{
    switch (value) {

    case ScrollRestoration::Auto:
        return "auto"_string;

    case ScrollRestoration::Manual:
        return "manual"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    