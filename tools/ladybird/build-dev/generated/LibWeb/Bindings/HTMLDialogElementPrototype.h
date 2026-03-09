
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLDialogElementPrototype : public JS::Object {
    JS_OBJECT(HTMLDialogElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLDialogElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLDialogElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLDialogElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(show);
        
    JS_DECLARE_NATIVE_FUNCTION(show_modal);
        
    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(request_close);
        
    JS_DECLARE_NATIVE_FUNCTION(open_getter);

    JS_DECLARE_NATIVE_FUNCTION(open_setter);

    JS_DECLARE_NATIVE_FUNCTION(return_value_getter);

    JS_DECLARE_NATIVE_FUNCTION(return_value_setter);

    JS_DECLARE_NATIVE_FUNCTION(closed_by_getter);

    JS_DECLARE_NATIVE_FUNCTION(closed_by_setter);


};


enum class ClosedByAttribute : u8 {

    Any,

    Closerequest,

    None,

};

inline String idl_enum_to_string(ClosedByAttribute value)
{
    switch (value) {

    case ClosedByAttribute::Any:
        return "any"_string;

    case ClosedByAttribute::Closerequest:
        return "closerequest"_string;

    case ClosedByAttribute::None:
        return "none"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    