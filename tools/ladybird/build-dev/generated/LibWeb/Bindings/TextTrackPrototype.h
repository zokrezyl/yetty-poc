
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TextTrackPrototype : public JS::Object {
    JS_OBJECT(TextTrackPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TextTrackPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TextTrackPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TextTrackPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(kind_getter);

    JS_DECLARE_NATIVE_FUNCTION(label_getter);

    JS_DECLARE_NATIVE_FUNCTION(language_getter);

    JS_DECLARE_NATIVE_FUNCTION(id_getter);

    JS_DECLARE_NATIVE_FUNCTION(mode_getter);

    JS_DECLARE_NATIVE_FUNCTION(mode_setter);

    JS_DECLARE_NATIVE_FUNCTION(oncuechange_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncuechange_setter);


};


enum class TextTrackMode : u8 {

    Disabled,

    Hidden,

    Showing,

};

inline String idl_enum_to_string(TextTrackMode value)
{
    switch (value) {

    case TextTrackMode::Disabled:
        return "disabled"_string;

    case TextTrackMode::Hidden:
        return "hidden"_string;

    case TextTrackMode::Showing:
        return "showing"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class TextTrackKind : u8 {

    Subtitles,

    Captions,

    Descriptions,

    Chapters,

    Metadata,

};

inline String idl_enum_to_string(TextTrackKind value)
{
    switch (value) {

    case TextTrackKind::Subtitles:
        return "subtitles"_string;

    case TextTrackKind::Captions:
        return "captions"_string;

    case TextTrackKind::Descriptions:
        return "descriptions"_string;

    case TextTrackKind::Chapters:
        return "chapters"_string;

    case TextTrackKind::Metadata:
        return "metadata"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    