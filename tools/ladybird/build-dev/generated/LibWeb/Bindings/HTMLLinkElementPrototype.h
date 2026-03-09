
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLLinkElementPrototype : public JS::Object {
    JS_OBJECT(HTMLLinkElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLLinkElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLLinkElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLLinkElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(href_getter);

    JS_DECLARE_NATIVE_FUNCTION(href_setter);

    JS_DECLARE_NATIVE_FUNCTION(cross_origin_getter);

    JS_DECLARE_NATIVE_FUNCTION(cross_origin_setter);

    JS_DECLARE_NATIVE_FUNCTION(rel_getter);

    JS_DECLARE_NATIVE_FUNCTION(rel_setter);

    JS_DECLARE_NATIVE_FUNCTION(as_getter);

    JS_DECLARE_NATIVE_FUNCTION(as_setter);

    JS_DECLARE_NATIVE_FUNCTION(rel_list_getter);

    JS_DECLARE_NATIVE_FUNCTION(rel_list_setter);

    JS_DECLARE_NATIVE_FUNCTION(media_getter);

    JS_DECLARE_NATIVE_FUNCTION(media_setter);

    JS_DECLARE_NATIVE_FUNCTION(integrity_getter);

    JS_DECLARE_NATIVE_FUNCTION(integrity_setter);

    JS_DECLARE_NATIVE_FUNCTION(hreflang_getter);

    JS_DECLARE_NATIVE_FUNCTION(hreflang_setter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_setter);

    JS_DECLARE_NATIVE_FUNCTION(sizes_getter);

    JS_DECLARE_NATIVE_FUNCTION(sizes_setter);

    JS_DECLARE_NATIVE_FUNCTION(image_srcset_getter);

    JS_DECLARE_NATIVE_FUNCTION(image_srcset_setter);

    JS_DECLARE_NATIVE_FUNCTION(image_sizes_getter);

    JS_DECLARE_NATIVE_FUNCTION(image_sizes_setter);

    JS_DECLARE_NATIVE_FUNCTION(referrer_policy_getter);

    JS_DECLARE_NATIVE_FUNCTION(referrer_policy_setter);

    JS_DECLARE_NATIVE_FUNCTION(disabled_getter);

    JS_DECLARE_NATIVE_FUNCTION(disabled_setter);

    JS_DECLARE_NATIVE_FUNCTION(fetch_priority_getter);

    JS_DECLARE_NATIVE_FUNCTION(fetch_priority_setter);

    JS_DECLARE_NATIVE_FUNCTION(charset_getter);

    JS_DECLARE_NATIVE_FUNCTION(charset_setter);

    JS_DECLARE_NATIVE_FUNCTION(rev_getter);

    JS_DECLARE_NATIVE_FUNCTION(rev_setter);

    JS_DECLARE_NATIVE_FUNCTION(target_getter);

    JS_DECLARE_NATIVE_FUNCTION(target_setter);

    JS_DECLARE_NATIVE_FUNCTION(sheet_getter);


};


enum class PotentialDestination : u8 {

    Empty,

    Audio,

    Audioworklet,

    Document,

    Embed,

    Font,

    Frame,

    Iframe,

    Image,

    Json,

    Manifest,

    Object,

    Paintworklet,

    Report,

    Script,

    Serviceworker,

    Sharedworker,

    Style,

    Track,

    Video,

    Webidentity,

    Worker,

    Xslt,

    Fetch,

};

inline String idl_enum_to_string(PotentialDestination value)
{
    switch (value) {

    case PotentialDestination::Empty:
        return ""_string;

    case PotentialDestination::Audio:
        return "audio"_string;

    case PotentialDestination::Audioworklet:
        return "audioworklet"_string;

    case PotentialDestination::Document:
        return "document"_string;

    case PotentialDestination::Embed:
        return "embed"_string;

    case PotentialDestination::Font:
        return "font"_string;

    case PotentialDestination::Frame:
        return "frame"_string;

    case PotentialDestination::Iframe:
        return "iframe"_string;

    case PotentialDestination::Image:
        return "image"_string;

    case PotentialDestination::Json:
        return "json"_string;

    case PotentialDestination::Manifest:
        return "manifest"_string;

    case PotentialDestination::Object:
        return "object"_string;

    case PotentialDestination::Paintworklet:
        return "paintworklet"_string;

    case PotentialDestination::Report:
        return "report"_string;

    case PotentialDestination::Script:
        return "script"_string;

    case PotentialDestination::Serviceworker:
        return "serviceworker"_string;

    case PotentialDestination::Sharedworker:
        return "sharedworker"_string;

    case PotentialDestination::Style:
        return "style"_string;

    case PotentialDestination::Track:
        return "track"_string;

    case PotentialDestination::Video:
        return "video"_string;

    case PotentialDestination::Webidentity:
        return "webidentity"_string;

    case PotentialDestination::Worker:
        return "worker"_string;

    case PotentialDestination::Xslt:
        return "xslt"_string;

    case PotentialDestination::Fetch:
        return "fetch"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    