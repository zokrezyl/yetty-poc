
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class XRSessionPrototype : public JS::Object {
    JS_OBJECT(XRSessionPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(XRSessionPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit XRSessionPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~XRSessionPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(end);
        
    JS_DECLARE_NATIVE_FUNCTION(onend_getter);

    JS_DECLARE_NATIVE_FUNCTION(onend_setter);


};


enum class XRVisibilityState : u8 {

    Visible,

    VisibleBlurred,

    Hidden,

};

inline String idl_enum_to_string(XRVisibilityState value)
{
    switch (value) {

    case XRVisibilityState::Visible:
        return "visible"_string;

    case XRVisibilityState::VisibleBlurred:
        return "visible-blurred"_string;

    case XRVisibilityState::Hidden:
        return "hidden"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    