
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
#include <LibWeb/Bindings/AudioBufferPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioBuffer.h>

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

GC_DEFINE_ALLOCATOR(AudioBufferPrototype);

AudioBufferPrototype::AudioBufferPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

AudioBufferPrototype::~AudioBufferPrototype()
{
}

void AudioBufferPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_sample_rate_getter = JS::NativeFunction::create(realm, sample_rate_getter, 0, "sampleRate"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_sample_rate_setter;

    define_direct_accessor("sampleRate"_utf16_fly_string, native_sample_rate_getter, native_sample_rate_setter, default_attributes);

    auto native_length_getter = JS::NativeFunction::create(realm, length_getter, 0, "length"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_length_setter;

    define_direct_accessor("length"_utf16_fly_string, native_length_getter, native_length_setter, default_attributes);

    auto native_duration_getter = JS::NativeFunction::create(realm, duration_getter, 0, "duration"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_duration_setter;

    define_direct_accessor("duration"_utf16_fly_string, native_duration_getter, native_duration_setter, default_attributes);

    auto native_number_of_channels_getter = JS::NativeFunction::create(realm, number_of_channels_getter, 0, "numberOfChannels"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_number_of_channels_setter;

    define_direct_accessor("numberOfChannels"_utf16_fly_string, native_number_of_channels_getter, native_number_of_channels_setter, default_attributes);

    define_native_function(realm, "getChannelData"_utf16_fly_string, get_channel_data, 1, default_attributes);

    define_native_function(realm, "copyFromChannel"_utf16_fly_string, copy_from_channel, 2, default_attributes);

    define_native_function(realm, "copyToChannel"_utf16_fly_string, copy_to_channel, 2, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "AudioBuffer"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void AudioBufferPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<WebAudio::AudioBuffer*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<WebAudio::AudioBuffer>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AudioBuffer");
    return static_cast<WebAudio::AudioBuffer*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferPrototype::sample_rate_getter)
{
    WebIDL::log_trace(vm, "AudioBufferPrototype::sample_rate_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->sample_rate(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferPrototype::length_getter)
{
    WebIDL::log_trace(vm, "AudioBufferPrototype::length_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->length(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferPrototype::duration_getter)
{
    WebIDL::log_trace(vm, "AudioBufferPrototype::duration_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->duration(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferPrototype::number_of_channels_getter)
{
    WebIDL::log_trace(vm, "AudioBufferPrototype::number_of_channels_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->number_of_channels(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferPrototype::get_channel_data)
{
    WebIDL::log_trace(vm, "AudioBufferPrototype::get_channel_data");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getChannelData");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong channel;

    channel = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_channel_data(channel); }));

    return &const_cast<JS::Float32Array&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferPrototype::copy_from_channel)
{
    WebIDL::log_trace(vm, "AudioBufferPrototype::copy_from_channel");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "copyFromChannel", "2");

    auto arg0 = vm.argument(0);

    GC::Root<WebIDL::BufferSource> destination;

    if (!arg0.is_object() || !(is<JS::TypedArrayBase>(arg0.as_object()) || is<JS::ArrayBuffer>(arg0.as_object()) || is<JS::DataView>(arg0.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Float32Array");

    destination = GC::make_root(realm.create<WebIDL::BufferSource>(arg0.as_object()));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong channel_number;

    channel_number = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong buffer_offset;

    if (!arg2.is_undefined())

    buffer_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        buffer_offset = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->copy_from_channel(destination, channel_number, buffer_offset); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferPrototype::copy_to_channel)
{
    WebIDL::log_trace(vm, "AudioBufferPrototype::copy_to_channel");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "copyToChannel", "2");

    auto arg0 = vm.argument(0);

    GC::Root<WebIDL::BufferSource> source;

    if (!arg0.is_object() || !(is<JS::TypedArrayBase>(arg0.as_object()) || is<JS::ArrayBuffer>(arg0.as_object()) || is<JS::DataView>(arg0.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Float32Array");

    source = GC::make_root(realm.create<WebIDL::BufferSource>(arg0.as_object()));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong channel_number;

    channel_number = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong buffer_offset;

    if (!arg2.is_undefined())

    buffer_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        buffer_offset = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->copy_to_channel(source, channel_number, buffer_offset); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
