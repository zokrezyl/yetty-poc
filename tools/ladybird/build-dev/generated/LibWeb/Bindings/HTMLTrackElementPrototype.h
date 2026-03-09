
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLTrackElementPrototype : public JS::Object {
    JS_OBJECT(HTMLTrackElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLTrackElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLTrackElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLTrackElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(kind_getter);

    JS_DECLARE_NATIVE_FUNCTION(kind_setter);

    JS_DECLARE_NATIVE_FUNCTION(src_getter);

    JS_DECLARE_NATIVE_FUNCTION(src_setter);

    JS_DECLARE_NATIVE_FUNCTION(srclang_getter);

    JS_DECLARE_NATIVE_FUNCTION(srclang_setter);

    JS_DECLARE_NATIVE_FUNCTION(label_getter);

    JS_DECLARE_NATIVE_FUNCTION(label_setter);

    JS_DECLARE_NATIVE_FUNCTION(default_getter);

    JS_DECLARE_NATIVE_FUNCTION(default_setter);

    JS_DECLARE_NATIVE_FUNCTION(ready_state_getter);

    JS_DECLARE_NATIVE_FUNCTION(track_getter);


};


enum class TrackKindAttribute : u8 {

    Subtitles,

    Captions,

    Descriptions,

    Chapters,

    Metadata,

};

inline String idl_enum_to_string(TrackKindAttribute value)
{
    switch (value) {

    case TrackKindAttribute::Subtitles:
        return "subtitles"_string;

    case TrackKindAttribute::Captions:
        return "captions"_string;

    case TrackKindAttribute::Descriptions:
        return "descriptions"_string;

    case TrackKindAttribute::Chapters:
        return "chapters"_string;

    case TrackKindAttribute::Metadata:
        return "metadata"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    