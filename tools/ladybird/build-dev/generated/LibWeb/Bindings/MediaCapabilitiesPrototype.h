
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MediaCapabilitiesPrototype : public JS::Object {
    JS_OBJECT(MediaCapabilitiesPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MediaCapabilitiesPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MediaCapabilitiesPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MediaCapabilitiesPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(decoding_info);
        

};


enum class TransferFunction : u8 {

    Srgb,

    Pq,

    Hlg,

};

inline String idl_enum_to_string(TransferFunction value)
{
    switch (value) {

    case TransferFunction::Srgb:
        return "srgb"_string;

    case TransferFunction::Pq:
        return "pq"_string;

    case TransferFunction::Hlg:
        return "hlg"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class MediaDecodingType : u8 {

    File,

    MediaSource,

    Webrtc,

};

inline String idl_enum_to_string(MediaDecodingType value)
{
    switch (value) {

    case MediaDecodingType::File:
        return "file"_string;

    case MediaDecodingType::MediaSource:
        return "media-source"_string;

    case MediaDecodingType::Webrtc:
        return "webrtc"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class MediaEncodingType : u8 {

    Record,

    Webrtc,

};

inline String idl_enum_to_string(MediaEncodingType value)
{
    switch (value) {

    case MediaEncodingType::Record:
        return "record"_string;

    case MediaEncodingType::Webrtc:
        return "webrtc"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class HdrMetadataType : u8 {

    Smptest2086,

    Smptest209410,

    Smptest209440,

};

inline String idl_enum_to_string(HdrMetadataType value)
{
    switch (value) {

    case HdrMetadataType::Smptest2086:
        return "smpteSt2086"_string;

    case HdrMetadataType::Smptest209410:
        return "smpteSt2094-10"_string;

    case HdrMetadataType::Smptest209440:
        return "smpteSt2094-40"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class ColorGamut : u8 {

    Srgb,

    P3,

    Rec2020,

};

inline String idl_enum_to_string(ColorGamut value)
{
    switch (value) {

    case ColorGamut::Srgb:
        return "srgb"_string;

    case ColorGamut::P3:
        return "p3"_string;

    case ColorGamut::Rec2020:
        return "rec2020"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    