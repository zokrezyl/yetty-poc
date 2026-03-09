
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class VTTCuePrototype : public JS::Object {
    JS_OBJECT(VTTCuePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(VTTCuePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit VTTCuePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~VTTCuePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(region_getter);

    JS_DECLARE_NATIVE_FUNCTION(region_setter);

    JS_DECLARE_NATIVE_FUNCTION(vertical_getter);

    JS_DECLARE_NATIVE_FUNCTION(vertical_setter);

    JS_DECLARE_NATIVE_FUNCTION(snap_to_lines_getter);

    JS_DECLARE_NATIVE_FUNCTION(snap_to_lines_setter);

    JS_DECLARE_NATIVE_FUNCTION(line_getter);

    JS_DECLARE_NATIVE_FUNCTION(line_setter);

    JS_DECLARE_NATIVE_FUNCTION(line_align_getter);

    JS_DECLARE_NATIVE_FUNCTION(line_align_setter);

    JS_DECLARE_NATIVE_FUNCTION(position_getter);

    JS_DECLARE_NATIVE_FUNCTION(position_setter);

    JS_DECLARE_NATIVE_FUNCTION(position_align_getter);

    JS_DECLARE_NATIVE_FUNCTION(position_align_setter);

    JS_DECLARE_NATIVE_FUNCTION(size_getter);

    JS_DECLARE_NATIVE_FUNCTION(size_setter);

    JS_DECLARE_NATIVE_FUNCTION(align_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_setter);


};


enum class AlignSetting : u8 {

    Start,

    Center,

    End,

    Left,

    Right,

};

inline String idl_enum_to_string(AlignSetting value)
{
    switch (value) {

    case AlignSetting::Start:
        return "start"_string;

    case AlignSetting::Center:
        return "center"_string;

    case AlignSetting::End:
        return "end"_string;

    case AlignSetting::Left:
        return "left"_string;

    case AlignSetting::Right:
        return "right"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class LineAlignSetting : u8 {

    Start,

    Center,

    End,

};

inline String idl_enum_to_string(LineAlignSetting value)
{
    switch (value) {

    case LineAlignSetting::Start:
        return "start"_string;

    case LineAlignSetting::Center:
        return "center"_string;

    case LineAlignSetting::End:
        return "end"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class PositionAlignSetting : u8 {

    LineLeft,

    Center,

    LineRight,

    Auto,

};

inline String idl_enum_to_string(PositionAlignSetting value)
{
    switch (value) {

    case PositionAlignSetting::LineLeft:
        return "line-left"_string;

    case PositionAlignSetting::Center:
        return "center"_string;

    case PositionAlignSetting::LineRight:
        return "line-right"_string;

    case PositionAlignSetting::Auto:
        return "auto"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class AutoKeyword : u8 {

    Auto,

};

inline String idl_enum_to_string(AutoKeyword value)
{
    switch (value) {

    case AutoKeyword::Auto:
        return "auto"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class DirectionSetting : u8 {

    Empty,

    Rl,

    Lr,

};

inline String idl_enum_to_string(DirectionSetting value)
{
    switch (value) {

    case DirectionSetting::Empty:
        return ""_string;

    case DirectionSetting::Rl:
        return "rl"_string;

    case DirectionSetting::Lr:
        return "lr"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    