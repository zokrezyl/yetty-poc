
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class XRSystemPrototype : public JS::Object {
    JS_OBJECT(XRSystemPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(XRSystemPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit XRSystemPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~XRSystemPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(is_session_supported);
        
    JS_DECLARE_NATIVE_FUNCTION(request_session);
        

};


enum class XRSessionMode : u8 {

    Inline,

    ImmersiveVr,

    ImmersiveAr,

};

inline String idl_enum_to_string(XRSessionMode value)
{
    switch (value) {

    case XRSessionMode::Inline:
        return "inline"_string;

    case XRSessionMode::ImmersiveVr:
        return "immersive-vr"_string;

    case XRSessionMode::ImmersiveAr:
        return "immersive-ar"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    