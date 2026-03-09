
#include <AK/Function.h>
#include <LibIDL/Types.h>
#include <LibJS/Runtime/Array.h>
#include <LibJS/Runtime/ArrayBuffer.h>
#include <LibJS/Runtime/DataView.h>
#include <LibJS/Runtime/Error.h>
#include <LibJS/Runtime/FunctionObject.h>
#include <LibJS/Runtime/GlobalObject.h>
#include <LibJS/Runtime/Iterator.h>
#include <LibJS/Runtime/PromiseConstructor.h>
#include <LibJS/Runtime/TypedArray.h>
#include <LibJS/Runtime/Value.h>
#include <LibJS/Runtime/ValueInlines.h>
#include <LibURL/Origin.h>
#include <LibWeb/Bindings/MediaCapabilitiesPrototype.h>
#include <LibWeb/Bindings/ExceptionOrUtils.h>
#include <LibWeb/Bindings/PrincipalHostDefined.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/DOM/Element.h>
#include <LibWeb/DOM/Event.h>
#include <LibWeb/DOM/IDLEventListener.h>
#include <LibWeb/DOM/NodeFilter.h>
#include <LibWeb/DOM/Range.h>
#include <LibWeb/HTML/Numbers.h>
#include <LibWeb/HTML/Scripting/Environments.h>
#include <LibWeb/HTML/Scripting/SimilarOriginWindowAgent.h>
#include <LibWeb/HTML/Window.h>
#include <LibWeb/HTML/WindowProxy.h>
#include <LibWeb/Infra/Strings.h>
#include <LibWeb/WebIDL/AbstractOperations.h>
#include <LibWeb/WebIDL/Buffers.h>
#include <LibWeb/WebIDL/OverloadResolution.h>
#include <LibWeb/WebIDL/Promise.h>
#include <LibWeb/WebIDL/Tracing.h>
#include <LibWeb/WebIDL/Types.h>

#if __has_include(<LibWeb/Bindings/ObjectPrototype.h>)
#    include <LibWeb/Bindings/ObjectPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/MediaCapabilitiesAPI/MediaCapabilities.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/EncryptedMediaExtensions/MediaKeySystemAccess.h>

// FIXME: This is a total hack until we can figure out the namespace for a given type somehow.
using namespace Web::Animations;
using namespace Web::Clipboard;
using namespace Web::ContentSecurityPolicy;
using namespace Web::CookieStore;
using namespace Web::CredentialManagement;
using namespace Web::Crypto;
using namespace Web::CSS;
using namespace Web::DOM;
using namespace Web::DOMURL;
using namespace Web::Encoding;
using namespace Web::EncryptedMediaExtensions;
using namespace Web::EntriesAPI;
using namespace Web::EventTiming;
using namespace Web::Fetch;
using namespace Web::FileAPI;
using namespace Web::Gamepad;
using namespace Web::Geolocation;
using namespace Web::Geometry;
using namespace Web::HighResolutionTime;
using namespace Web::HTML;
using namespace Web::IndexedDB;
using namespace Web::Internals;
using namespace Web::IntersectionObserver;
using namespace Web::MediaCapabilitiesAPI;
using namespace Web::MediaSourceExtensions;
using namespace Web::NavigationTiming;
using namespace Web::NotificationsAPI;
using namespace Web::PerformanceTimeline;
using namespace Web::RequestIdleCallback;
using namespace Web::ResizeObserver;
using namespace Web::ResourceTiming;
using namespace Web::Selection;
using namespace Web::Serial;
using namespace Web::ServiceWorker;
using namespace Web::Speech;
using namespace Web::StorageAPI;
using namespace Web::Streams;
using namespace Web::SVG;
using namespace Web::TrustedTypes;
using namespace Web::UIEvents;
using namespace Web::URLPattern;
using namespace Web::UserTiming;
using namespace Web::WebAssembly;
using namespace Web::WebAudio;
using namespace Web::WebGL;
using namespace Web::WebGL::Extensions;
using namespace Web::WebIDL;
using namespace Web::WebVTT;
using namespace Web::WebXR;
using namespace Web::XHR;
using namespace Web::XPath;

namespace Web::Bindings {

GC_DEFINE_ALLOCATOR(MediaCapabilitiesPrototype);

MediaCapabilitiesPrototype::MediaCapabilitiesPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

MediaCapabilitiesPrototype::~MediaCapabilitiesPrototype()
{
}

void MediaCapabilitiesPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


        define_direct_property("encodingInfo"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_native_function(realm, "decodingInfo"_utf16_fly_string, decoding_info, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "MediaCapabilities"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void MediaCapabilitiesPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<MediaCapabilities*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<MediaCapabilities>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "MediaCapabilities");
    return static_cast<MediaCapabilities*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(MediaCapabilitiesPrototype::decoding_info)
{
    WebIDL::log_trace(vm, "MediaCapabilitiesPrototype::decoding_info");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "decodingInfo");

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "MediaDecodingConfiguration");

    MediaDecodingConfiguration configuration {};

    auto key_system_configuration_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        key_system_configuration_property_value_0 = TRY(arg0.as_object().get("keySystemConfiguration"_utf16_fly_string));

    if (!key_system_configuration_property_value_0.is_undefined()) {

    if (!key_system_configuration_property_value_0.is_nullish() && !key_system_configuration_property_value_0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "MediaCapabilitiesKeySystemConfiguration");

    MediaCapabilitiesKeySystemConfiguration key_system_configuration_value_0 {};

    auto audio_property_value_0 = JS::js_undefined();
    if (key_system_configuration_property_value_0.is_object())
        audio_property_value_0 = TRY(key_system_configuration_property_value_0.as_object().get("audio"_utf16_fly_string));

    if (!audio_property_value_0.is_undefined()) {

    if (!audio_property_value_0.is_nullish() && !audio_property_value_0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "KeySystemTrackConfiguration");

    KeySystemTrackConfiguration audio_value_0 {};

    auto encryption_scheme_property_value_0 = JS::js_undefined();
    if (audio_property_value_0.is_object())
        encryption_scheme_property_value_0 = TRY(audio_property_value_0.as_object().get("encryptionScheme"_utf16_fly_string));

    Optional<String> encryption_scheme_value_0;

    if (!encryption_scheme_property_value_0.is_undefined()) {
        if (!encryption_scheme_property_value_0.is_null())
            encryption_scheme_value_0 = TRY(WebIDL::to_string(vm, encryption_scheme_property_value_0));
    }

    audio_value_0.encryption_scheme = encryption_scheme_value_0;

    auto robustness_property_value_1 = JS::js_undefined();
    if (audio_property_value_0.is_object())
        robustness_property_value_1 = TRY(audio_property_value_0.as_object().get("robustness"_utf16_fly_string));

    String robustness_value_1;

    if (!robustness_property_value_1.is_undefined()) {
        if (!false || !robustness_property_value_1.is_null())
            robustness_value_1 = TRY(WebIDL::to_string(vm, robustness_property_value_1));
    } else {
        robustness_value_1 = ""_string;
    }

    audio_value_0.robustness = robustness_value_1;

    key_system_configuration_value_0.audio = audio_value_0;

    }

    auto distinctive_identifier_property_value_3 = JS::js_undefined();
    if (key_system_configuration_property_value_0.is_object())
        distinctive_identifier_property_value_3 = TRY(key_system_configuration_property_value_0.as_object().get("distinctiveIdentifier"_utf16_fly_string));

    MediaKeysRequirement distinctive_identifier_value_3 { MediaKeysRequirement::Optional };

    if (!distinctive_identifier_property_value_3.is_undefined()) {

    auto distinctive_identifier_property_value_3_string = TRY(distinctive_identifier_property_value_3.to_string(vm));

    if (distinctive_identifier_property_value_3_string == "required"sv)
        distinctive_identifier_value_3 = MediaKeysRequirement::Required;

    else if (distinctive_identifier_property_value_3_string == "optional"sv)
        distinctive_identifier_value_3 = MediaKeysRequirement::Optional;

    else if (distinctive_identifier_property_value_3_string == "not-allowed"sv)
        distinctive_identifier_value_3 = MediaKeysRequirement::NotAllowed;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, distinctive_identifier_property_value_3_string, "MediaKeysRequirement");

    }

    key_system_configuration_value_0.distinctive_identifier = distinctive_identifier_value_3;

    auto init_data_type_property_value_4 = JS::js_undefined();
    if (key_system_configuration_property_value_0.is_object())
        init_data_type_property_value_4 = TRY(key_system_configuration_property_value_0.as_object().get("initDataType"_utf16_fly_string));

    String init_data_type_value_4;

    if (!init_data_type_property_value_4.is_undefined()) {
        if (!false || !init_data_type_property_value_4.is_null())
            init_data_type_value_4 = TRY(WebIDL::to_string(vm, init_data_type_property_value_4));
    } else {
        init_data_type_value_4 = ""_string;
    }

    key_system_configuration_value_0.init_data_type = init_data_type_value_4;

    auto key_system_property_value_5 = JS::js_undefined();
    if (key_system_configuration_property_value_0.is_object())
        key_system_property_value_5 = TRY(key_system_configuration_property_value_0.as_object().get("keySystem"_utf16_fly_string));

    if (key_system_property_value_5.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "keySystem");

    String key_system_value_5;
    if (!false || !key_system_property_value_5.is_null()) {
        key_system_value_5 = TRY(WebIDL::to_string(vm, key_system_property_value_5));
    }

    key_system_configuration_value_0.key_system = key_system_value_5;

    auto persistent_state_property_value_6 = JS::js_undefined();
    if (key_system_configuration_property_value_0.is_object())
        persistent_state_property_value_6 = TRY(key_system_configuration_property_value_0.as_object().get("persistentState"_utf16_fly_string));

    MediaKeysRequirement persistent_state_value_6 { MediaKeysRequirement::Optional };

    if (!persistent_state_property_value_6.is_undefined()) {

    auto persistent_state_property_value_6_string = TRY(persistent_state_property_value_6.to_string(vm));

    if (persistent_state_property_value_6_string == "required"sv)
        persistent_state_value_6 = MediaKeysRequirement::Required;

    else if (persistent_state_property_value_6_string == "optional"sv)
        persistent_state_value_6 = MediaKeysRequirement::Optional;

    else if (persistent_state_property_value_6_string == "not-allowed"sv)
        persistent_state_value_6 = MediaKeysRequirement::NotAllowed;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, persistent_state_property_value_6_string, "MediaKeysRequirement");

    }

    key_system_configuration_value_0.persistent_state = persistent_state_value_6;

    auto session_types_property_value_7 = JS::js_undefined();
    if (key_system_configuration_property_value_0.is_object())
        session_types_property_value_7 = TRY(key_system_configuration_property_value_0.as_object().get("sessionTypes"_utf16_fly_string));

    if (!session_types_property_value_7.is_undefined()) {

    Optional<Vector<String>> session_types_value_7;

    if (!session_types_property_value_7.is_undefined()) {

    if (!session_types_property_value_7.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, session_types_property_value_7);

    auto session_types_property_value_7_iterator_method0 = TRY(session_types_property_value_7.get_method(vm, vm.well_known_symbol_iterator()));
    if (!session_types_property_value_7_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, session_types_property_value_7);

    auto session_types_property_value_7_iterator1 = TRY(JS::get_iterator_from_method(vm, session_types_property_value_7, *session_types_property_value_7_iterator_method0));

    Vector<String> session_types_value_7_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, session_types_property_value_7_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    String sequence_item1;
    if (!false || !next_item1.is_null()) {
        sequence_item1 = TRY(WebIDL::to_string(vm, next_item1));
    }

    session_types_value_7_non_optional.append(sequence_item1);
    }

        session_types_value_7 = move(session_types_value_7_non_optional);
    }

    key_system_configuration_value_0.session_types = session_types_value_7;

    }

    auto video_property_value_8 = JS::js_undefined();
    if (key_system_configuration_property_value_0.is_object())
        video_property_value_8 = TRY(key_system_configuration_property_value_0.as_object().get("video"_utf16_fly_string));

    if (!video_property_value_8.is_undefined()) {

    if (!video_property_value_8.is_nullish() && !video_property_value_8.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "KeySystemTrackConfiguration");

    KeySystemTrackConfiguration video_value_8 {};

    auto encryption_scheme_property_value_8 = JS::js_undefined();
    if (video_property_value_8.is_object())
        encryption_scheme_property_value_8 = TRY(video_property_value_8.as_object().get("encryptionScheme"_utf16_fly_string));

    Optional<String> encryption_scheme_value_8;

    if (!encryption_scheme_property_value_8.is_undefined()) {
        if (!encryption_scheme_property_value_8.is_null())
            encryption_scheme_value_8 = TRY(WebIDL::to_string(vm, encryption_scheme_property_value_8));
    }

    video_value_8.encryption_scheme = encryption_scheme_value_8;

    auto robustness_property_value_9 = JS::js_undefined();
    if (video_property_value_8.is_object())
        robustness_property_value_9 = TRY(video_property_value_8.as_object().get("robustness"_utf16_fly_string));

    String robustness_value_9;

    if (!robustness_property_value_9.is_undefined()) {
        if (!false || !robustness_property_value_9.is_null())
            robustness_value_9 = TRY(WebIDL::to_string(vm, robustness_property_value_9));
    } else {
        robustness_value_9 = ""_string;
    }

    video_value_8.robustness = robustness_value_9;

    key_system_configuration_value_0.video = video_value_8;

    }

    configuration.key_system_configuration = key_system_configuration_value_0;

    }

    auto type_property_value_1 = JS::js_undefined();
    if (arg0.is_object())
        type_property_value_1 = TRY(arg0.as_object().get("type"_utf16_fly_string));

    if (type_property_value_1.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "type");

    MediaDecodingType type_value_1 { MediaDecodingType::File };

    auto type_property_value_1_string = TRY(type_property_value_1.to_string(vm));

    if (type_property_value_1_string == "file"sv)
        type_value_1 = MediaDecodingType::File;

    else if (type_property_value_1_string == "media-source"sv)
        type_value_1 = MediaDecodingType::MediaSource;

    else if (type_property_value_1_string == "webrtc"sv)
        type_value_1 = MediaDecodingType::Webrtc;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, type_property_value_1_string, "MediaDecodingType");

    configuration.type = type_value_1;

    auto audio_property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        audio_property_value_2 = TRY(arg0.as_object().get("audio"_utf16_fly_string));

    if (!audio_property_value_2.is_undefined()) {

    if (!audio_property_value_2.is_nullish() && !audio_property_value_2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AudioConfiguration");

    AudioConfiguration audio_value_2 {};

    auto bitrate_property_value_11 = JS::js_undefined();
    if (audio_property_value_2.is_object())
        bitrate_property_value_11 = TRY(audio_property_value_2.as_object().get("bitrate"_utf16_fly_string));

    if (!bitrate_property_value_11.is_undefined()) {

    Optional<WebIDL::UnsignedLongLong> bitrate_value_11;

    if (!bitrate_property_value_11.is_undefined())

    bitrate_value_11 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, bitrate_property_value_11, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    audio_value_2.bitrate = bitrate_value_11;

    }

    auto channels_property_value_12 = JS::js_undefined();
    if (audio_property_value_2.is_object())
        channels_property_value_12 = TRY(audio_property_value_2.as_object().get("channels"_utf16_fly_string));

    if (!channels_property_value_12.is_undefined()) {

    Optional<String> channels_value_12;

    if (!channels_property_value_12.is_undefined()) {
        if (!false || !channels_property_value_12.is_null())
            channels_value_12 = TRY(WebIDL::to_string(vm, channels_property_value_12));
    }

    if (channels_value_12.has_value())
        audio_value_2.channels = channels_value_12.release_value();

    }

    auto content_type_property_value_13 = JS::js_undefined();
    if (audio_property_value_2.is_object())
        content_type_property_value_13 = TRY(audio_property_value_2.as_object().get("contentType"_utf16_fly_string));

    if (content_type_property_value_13.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "contentType");

    String content_type_value_13;
    if (!false || !content_type_property_value_13.is_null()) {
        content_type_value_13 = TRY(WebIDL::to_string(vm, content_type_property_value_13));
    }

    audio_value_2.content_type = content_type_value_13;

    auto samplerate_property_value_14 = JS::js_undefined();
    if (audio_property_value_2.is_object())
        samplerate_property_value_14 = TRY(audio_property_value_2.as_object().get("samplerate"_utf16_fly_string));

    if (!samplerate_property_value_14.is_undefined()) {

    Optional<WebIDL::UnsignedLong> samplerate_value_14;

    if (!samplerate_property_value_14.is_undefined())

    samplerate_value_14 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, samplerate_property_value_14, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    audio_value_2.samplerate = samplerate_value_14;

    }

    auto spatial_rendering_property_value_15 = JS::js_undefined();
    if (audio_property_value_2.is_object())
        spatial_rendering_property_value_15 = TRY(audio_property_value_2.as_object().get("spatialRendering"_utf16_fly_string));

    if (!spatial_rendering_property_value_15.is_undefined()) {

    Optional<bool> spatial_rendering_value_15;

    if (!spatial_rendering_property_value_15.is_undefined())

    spatial_rendering_value_15 = spatial_rendering_property_value_15.to_boolean();

    audio_value_2.spatial_rendering = spatial_rendering_value_15;

    }

    configuration.audio = audio_value_2;

    }

    auto video_property_value_3 = JS::js_undefined();
    if (arg0.is_object())
        video_property_value_3 = TRY(arg0.as_object().get("video"_utf16_fly_string));

    if (!video_property_value_3.is_undefined()) {

    if (!video_property_value_3.is_nullish() && !video_property_value_3.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "VideoConfiguration");

    VideoConfiguration video_value_3 {};

    auto bitrate_property_value_16 = JS::js_undefined();
    if (video_property_value_3.is_object())
        bitrate_property_value_16 = TRY(video_property_value_3.as_object().get("bitrate"_utf16_fly_string));

    if (bitrate_property_value_16.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "bitrate");

    WebIDL::UnsignedLongLong bitrate_value_16;

    bitrate_value_16 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, bitrate_property_value_16, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    video_value_3.bitrate = bitrate_value_16;

    auto color_gamut_property_value_17 = JS::js_undefined();
    if (video_property_value_3.is_object())
        color_gamut_property_value_17 = TRY(video_property_value_3.as_object().get("colorGamut"_utf16_fly_string));

    if (!color_gamut_property_value_17.is_undefined()) {

    ColorGamut color_gamut_value_17 { ColorGamut::Srgb };

    if (!color_gamut_property_value_17.is_undefined()) {

    auto color_gamut_property_value_17_string = TRY(color_gamut_property_value_17.to_string(vm));

    if (color_gamut_property_value_17_string == "srgb"sv)
        color_gamut_value_17 = ColorGamut::Srgb;

    else if (color_gamut_property_value_17_string == "p3"sv)
        color_gamut_value_17 = ColorGamut::P3;

    else if (color_gamut_property_value_17_string == "rec2020"sv)
        color_gamut_value_17 = ColorGamut::Rec2020;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, color_gamut_property_value_17_string, "ColorGamut");

    }

    video_value_3.color_gamut = color_gamut_value_17;

    }

    auto content_type_property_value_18 = JS::js_undefined();
    if (video_property_value_3.is_object())
        content_type_property_value_18 = TRY(video_property_value_3.as_object().get("contentType"_utf16_fly_string));

    if (content_type_property_value_18.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "contentType");

    String content_type_value_18;
    if (!false || !content_type_property_value_18.is_null()) {
        content_type_value_18 = TRY(WebIDL::to_string(vm, content_type_property_value_18));
    }

    video_value_3.content_type = content_type_value_18;

    auto framerate_property_value_19 = JS::js_undefined();
    if (video_property_value_3.is_object())
        framerate_property_value_19 = TRY(video_property_value_3.as_object().get("framerate"_utf16_fly_string));

    if (framerate_property_value_19.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "framerate");

    double framerate_value_19 = TRY(framerate_property_value_19.to_double(vm));

    if (isinf(framerate_value_19) || isnan(framerate_value_19)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "framerate");
    }
    
    video_value_3.framerate = framerate_value_19;

    auto has_alpha_channel_property_value_20 = JS::js_undefined();
    if (video_property_value_3.is_object())
        has_alpha_channel_property_value_20 = TRY(video_property_value_3.as_object().get("hasAlphaChannel"_utf16_fly_string));

    if (!has_alpha_channel_property_value_20.is_undefined()) {

    Optional<bool> has_alpha_channel_value_20;

    if (!has_alpha_channel_property_value_20.is_undefined())

    has_alpha_channel_value_20 = has_alpha_channel_property_value_20.to_boolean();

    video_value_3.has_alpha_channel = has_alpha_channel_value_20;

    }

    auto hdr_metadata_type_property_value_21 = JS::js_undefined();
    if (video_property_value_3.is_object())
        hdr_metadata_type_property_value_21 = TRY(video_property_value_3.as_object().get("hdrMetadataType"_utf16_fly_string));

    if (!hdr_metadata_type_property_value_21.is_undefined()) {

    HdrMetadataType hdr_metadata_type_value_21 { HdrMetadataType::Smptest2086 };

    if (!hdr_metadata_type_property_value_21.is_undefined()) {

    auto hdr_metadata_type_property_value_21_string = TRY(hdr_metadata_type_property_value_21.to_string(vm));

    if (hdr_metadata_type_property_value_21_string == "smpteSt2086"sv)
        hdr_metadata_type_value_21 = HdrMetadataType::Smptest2086;

    else if (hdr_metadata_type_property_value_21_string == "smpteSt2094-10"sv)
        hdr_metadata_type_value_21 = HdrMetadataType::Smptest209410;

    else if (hdr_metadata_type_property_value_21_string == "smpteSt2094-40"sv)
        hdr_metadata_type_value_21 = HdrMetadataType::Smptest209440;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, hdr_metadata_type_property_value_21_string, "HdrMetadataType");

    }

    video_value_3.hdr_metadata_type = hdr_metadata_type_value_21;

    }

    auto height_property_value_22 = JS::js_undefined();
    if (video_property_value_3.is_object())
        height_property_value_22 = TRY(video_property_value_3.as_object().get("height"_utf16_fly_string));

    if (height_property_value_22.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "height");

    WebIDL::UnsignedLong height_value_22;

    height_value_22 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, height_property_value_22, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    video_value_3.height = height_value_22;

    auto scalability_mode_property_value_23 = JS::js_undefined();
    if (video_property_value_3.is_object())
        scalability_mode_property_value_23 = TRY(video_property_value_3.as_object().get("scalabilityMode"_utf16_fly_string));

    if (!scalability_mode_property_value_23.is_undefined()) {

    Optional<String> scalability_mode_value_23;

    if (!scalability_mode_property_value_23.is_undefined()) {
        if (!false || !scalability_mode_property_value_23.is_null())
            scalability_mode_value_23 = TRY(WebIDL::to_string(vm, scalability_mode_property_value_23));
    }

    if (scalability_mode_value_23.has_value())
        video_value_3.scalability_mode = scalability_mode_value_23.release_value();

    }

    auto spatial_scalability_property_value_24 = JS::js_undefined();
    if (video_property_value_3.is_object())
        spatial_scalability_property_value_24 = TRY(video_property_value_3.as_object().get("spatialScalability"_utf16_fly_string));

    if (!spatial_scalability_property_value_24.is_undefined()) {

    Optional<bool> spatial_scalability_value_24;

    if (!spatial_scalability_property_value_24.is_undefined())

    spatial_scalability_value_24 = spatial_scalability_property_value_24.to_boolean();

    video_value_3.spatial_scalability = spatial_scalability_value_24;

    }

    auto transfer_function_property_value_25 = JS::js_undefined();
    if (video_property_value_3.is_object())
        transfer_function_property_value_25 = TRY(video_property_value_3.as_object().get("transferFunction"_utf16_fly_string));

    if (!transfer_function_property_value_25.is_undefined()) {

    TransferFunction transfer_function_value_25 { TransferFunction::Srgb };

    if (!transfer_function_property_value_25.is_undefined()) {

    auto transfer_function_property_value_25_string = TRY(transfer_function_property_value_25.to_string(vm));

    if (transfer_function_property_value_25_string == "srgb"sv)
        transfer_function_value_25 = TransferFunction::Srgb;

    else if (transfer_function_property_value_25_string == "pq"sv)
        transfer_function_value_25 = TransferFunction::Pq;

    else if (transfer_function_property_value_25_string == "hlg"sv)
        transfer_function_value_25 = TransferFunction::Hlg;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, transfer_function_property_value_25_string, "TransferFunction");

    }

    video_value_3.transfer_function = transfer_function_value_25;

    }

    auto width_property_value_26 = JS::js_undefined();
    if (video_property_value_3.is_object())
        width_property_value_26 = TRY(video_property_value_3.as_object().get("width"_utf16_fly_string));

    if (width_property_value_26.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "width");

    WebIDL::UnsignedLong width_value_26;

    width_value_26 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, width_property_value_26, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    video_value_3.width = width_value_26;

    configuration.video = video_value_3;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->decoding_info(configuration); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If op has a return type that is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    // NOTE: We know that this is a Promise return type statically by the IDL.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

} // namespace Web::Bindings
