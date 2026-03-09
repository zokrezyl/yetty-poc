
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class VTTRegionPrototype : public JS::Object {
    JS_OBJECT(VTTRegionPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(VTTRegionPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit VTTRegionPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~VTTRegionPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(id_getter);

    JS_DECLARE_NATIVE_FUNCTION(id_setter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_setter);

    JS_DECLARE_NATIVE_FUNCTION(lines_getter);

    JS_DECLARE_NATIVE_FUNCTION(lines_setter);

    JS_DECLARE_NATIVE_FUNCTION(region_anchor_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(region_anchor_x_setter);

    JS_DECLARE_NATIVE_FUNCTION(region_anchor_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(region_anchor_y_setter);

    JS_DECLARE_NATIVE_FUNCTION(viewport_anchor_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(viewport_anchor_x_setter);

    JS_DECLARE_NATIVE_FUNCTION(viewport_anchor_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(viewport_anchor_y_setter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_setter);


};


enum class ScrollSetting : u8 {

    Empty,

    Up,

};

inline String idl_enum_to_string(ScrollSetting value)
{
    switch (value) {

    case ScrollSetting::Empty:
        return ""_string;

    case ScrollSetting::Up:
        return "up"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    