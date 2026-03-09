
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ResizeObserverPrototype : public JS::Object {
    JS_OBJECT(ResizeObserverPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ResizeObserverPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ResizeObserverPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ResizeObserverPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(observe);
        
    JS_DECLARE_NATIVE_FUNCTION(unobserve);
        
    JS_DECLARE_NATIVE_FUNCTION(disconnect);
        

};


enum class ResizeObserverBoxOptions : u8 {

    BorderBox,

    ContentBox,

    DevicePixelContentBox,

};

inline String idl_enum_to_string(ResizeObserverBoxOptions value)
{
    switch (value) {

    case ResizeObserverBoxOptions::BorderBox:
        return "border-box"_string;

    case ResizeObserverBoxOptions::ContentBox:
        return "content-box"_string;

    case ResizeObserverBoxOptions::DevicePixelContentBox:
        return "device-pixel-content-box"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    