
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class RequestPrototype : public JS::Object {
    JS_OBJECT(RequestPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(RequestPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit RequestPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~RequestPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(clone);
        
    JS_DECLARE_NATIVE_FUNCTION(array_buffer);
        
    JS_DECLARE_NATIVE_FUNCTION(blob);
        
    JS_DECLARE_NATIVE_FUNCTION(bytes);
        
    JS_DECLARE_NATIVE_FUNCTION(form_data);
        
    JS_DECLARE_NATIVE_FUNCTION(json);
        
    JS_DECLARE_NATIVE_FUNCTION(text);
        
    JS_DECLARE_NATIVE_FUNCTION(method_getter);

    JS_DECLARE_NATIVE_FUNCTION(url_getter);

    JS_DECLARE_NATIVE_FUNCTION(headers_getter);

    JS_DECLARE_NATIVE_FUNCTION(destination_getter);

    JS_DECLARE_NATIVE_FUNCTION(referrer_getter);

    JS_DECLARE_NATIVE_FUNCTION(referrer_policy_getter);

    JS_DECLARE_NATIVE_FUNCTION(mode_getter);

    JS_DECLARE_NATIVE_FUNCTION(credentials_getter);

    JS_DECLARE_NATIVE_FUNCTION(cache_getter);

    JS_DECLARE_NATIVE_FUNCTION(redirect_getter);

    JS_DECLARE_NATIVE_FUNCTION(integrity_getter);

    JS_DECLARE_NATIVE_FUNCTION(keepalive_getter);

    JS_DECLARE_NATIVE_FUNCTION(is_reload_navigation_getter);

    JS_DECLARE_NATIVE_FUNCTION(is_history_navigation_getter);

    JS_DECLARE_NATIVE_FUNCTION(signal_getter);

    JS_DECLARE_NATIVE_FUNCTION(duplex_getter);

    JS_DECLARE_NATIVE_FUNCTION(body_getter);

    JS_DECLARE_NATIVE_FUNCTION(body_used_getter);


};


enum class RequestCredentials : u8 {

    Omit,

    SameOrigin,

    Include,

};

inline String idl_enum_to_string(RequestCredentials value)
{
    switch (value) {

    case RequestCredentials::Omit:
        return "omit"_string;

    case RequestCredentials::SameOrigin:
        return "same-origin"_string;

    case RequestCredentials::Include:
        return "include"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class ReferrerPolicy : u8 {

    Empty,

    NoReferrer,

    NoReferrerWhenDowngrade,

    SameOrigin,

    Origin,

    StrictOrigin,

    OriginWhenCrossOrigin,

    StrictOriginWhenCrossOrigin,

    UnsafeUrl,

};

inline String idl_enum_to_string(ReferrerPolicy value)
{
    switch (value) {

    case ReferrerPolicy::Empty:
        return ""_string;

    case ReferrerPolicy::NoReferrer:
        return "no-referrer"_string;

    case ReferrerPolicy::NoReferrerWhenDowngrade:
        return "no-referrer-when-downgrade"_string;

    case ReferrerPolicy::SameOrigin:
        return "same-origin"_string;

    case ReferrerPolicy::Origin:
        return "origin"_string;

    case ReferrerPolicy::StrictOrigin:
        return "strict-origin"_string;

    case ReferrerPolicy::OriginWhenCrossOrigin:
        return "origin-when-cross-origin"_string;

    case ReferrerPolicy::StrictOriginWhenCrossOrigin:
        return "strict-origin-when-cross-origin"_string;

    case ReferrerPolicy::UnsafeUrl:
        return "unsafe-url"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class RequestCache : u8 {

    Default,

    NoStore,

    Reload,

    NoCache,

    ForceCache,

    OnlyIfCached,

};

inline String idl_enum_to_string(RequestCache value)
{
    switch (value) {

    case RequestCache::Default:
        return "default"_string;

    case RequestCache::NoStore:
        return "no-store"_string;

    case RequestCache::Reload:
        return "reload"_string;

    case RequestCache::NoCache:
        return "no-cache"_string;

    case RequestCache::ForceCache:
        return "force-cache"_string;

    case RequestCache::OnlyIfCached:
        return "only-if-cached"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class RequestMode : u8 {

    Navigate,

    SameOrigin,

    NoCors,

    Cors,

};

inline String idl_enum_to_string(RequestMode value)
{
    switch (value) {

    case RequestMode::Navigate:
        return "navigate"_string;

    case RequestMode::SameOrigin:
        return "same-origin"_string;

    case RequestMode::NoCors:
        return "no-cors"_string;

    case RequestMode::Cors:
        return "cors"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class RequestPriority : u8 {

    High,

    Low,

    Auto,

};

inline String idl_enum_to_string(RequestPriority value)
{
    switch (value) {

    case RequestPriority::High:
        return "high"_string;

    case RequestPriority::Low:
        return "low"_string;

    case RequestPriority::Auto:
        return "auto"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class RequestDestination : u8 {

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

    Sharedworker,

    Style,

    Track,

    Video,

    Worker,

    Xslt,

};

inline String idl_enum_to_string(RequestDestination value)
{
    switch (value) {

    case RequestDestination::Empty:
        return ""_string;

    case RequestDestination::Audio:
        return "audio"_string;

    case RequestDestination::Audioworklet:
        return "audioworklet"_string;

    case RequestDestination::Document:
        return "document"_string;

    case RequestDestination::Embed:
        return "embed"_string;

    case RequestDestination::Font:
        return "font"_string;

    case RequestDestination::Frame:
        return "frame"_string;

    case RequestDestination::Iframe:
        return "iframe"_string;

    case RequestDestination::Image:
        return "image"_string;

    case RequestDestination::Json:
        return "json"_string;

    case RequestDestination::Manifest:
        return "manifest"_string;

    case RequestDestination::Object:
        return "object"_string;

    case RequestDestination::Paintworklet:
        return "paintworklet"_string;

    case RequestDestination::Report:
        return "report"_string;

    case RequestDestination::Script:
        return "script"_string;

    case RequestDestination::Sharedworker:
        return "sharedworker"_string;

    case RequestDestination::Style:
        return "style"_string;

    case RequestDestination::Track:
        return "track"_string;

    case RequestDestination::Video:
        return "video"_string;

    case RequestDestination::Worker:
        return "worker"_string;

    case RequestDestination::Xslt:
        return "xslt"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class RequestRedirect : u8 {

    Follow,

    Error,

    Manual,

};

inline String idl_enum_to_string(RequestRedirect value)
{
    switch (value) {

    case RequestRedirect::Follow:
        return "follow"_string;

    case RequestRedirect::Error:
        return "error"_string;

    case RequestRedirect::Manual:
        return "manual"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class RequestDuplex : u8 {

    Half,

};

inline String idl_enum_to_string(RequestDuplex value)
{
    switch (value) {

    case RequestDuplex::Half:
        return "half"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    