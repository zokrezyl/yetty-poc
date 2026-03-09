
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
#include <LibWeb/Bindings/BiquadFilterNodePrototype.h>
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

#if __has_include(<LibWeb/Bindings/AudioNodePrototype.h>)
#    include <LibWeb/Bindings/AudioNodePrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/BiquadFilterNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioParam.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/BaseAudioContext.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioBuffer.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioBufferSourceNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioDestinationNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioListener.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/ChannelMergerNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/DynamicsCompressorNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/GainNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/OscillatorNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/PannerNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebIDL/DOMException.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioScheduledSourceNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/PeriodicWave.h>

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

GC_DEFINE_ALLOCATOR(BiquadFilterNodePrototype);

BiquadFilterNodePrototype::BiquadFilterNodePrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

BiquadFilterNodePrototype::~BiquadFilterNodePrototype()
{
}

void BiquadFilterNodePrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<AudioNodePrototype>(realm, "AudioNode"_fly_string));


    auto native_type_getter = JS::NativeFunction::create(realm, type_getter, 0, "type"_utf16_fly_string, &realm, "get"sv);

    auto native_type_setter = JS::NativeFunction::create(realm, type_setter, 1, "type"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("type"_utf16_fly_string, native_type_getter, native_type_setter, default_attributes);

    auto native_frequency_getter = JS::NativeFunction::create(realm, frequency_getter, 0, "frequency"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_frequency_setter;

    define_direct_accessor("frequency"_utf16_fly_string, native_frequency_getter, native_frequency_setter, default_attributes);

    auto native_detune_getter = JS::NativeFunction::create(realm, detune_getter, 0, "detune"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_detune_setter;

    define_direct_accessor("detune"_utf16_fly_string, native_detune_getter, native_detune_setter, default_attributes);

    auto native_q_getter = JS::NativeFunction::create(realm, q_getter, 0, "Q"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_q_setter;

    define_direct_accessor("Q"_utf16_fly_string, native_q_getter, native_q_setter, default_attributes);

    auto native_gain_getter = JS::NativeFunction::create(realm, gain_getter, 0, "gain"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_gain_setter;

    define_direct_accessor("gain"_utf16_fly_string, native_gain_getter, native_gain_setter, default_attributes);

    define_native_function(realm, "getFrequencyResponse"_utf16_fly_string, get_frequency_response, 3, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "BiquadFilterNode"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void BiquadFilterNodePrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<WebAudio::BiquadFilterNode*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<WebAudio::BiquadFilterNode>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BiquadFilterNode");
    return static_cast<WebAudio::BiquadFilterNode*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(BiquadFilterNodePrototype::type_getter)
{
    WebIDL::log_trace(vm, "BiquadFilterNodePrototype::type_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->type(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(BiquadFilterNodePrototype::type_setter)
{
    WebIDL::log_trace(vm, "BiquadFilterNodePrototype::type_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    BiquadFilterType cpp_value { BiquadFilterType::Lowpass };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == "lowpass"sv)
        cpp_value = BiquadFilterType::Lowpass;

    else if (value_string == "highpass"sv)
        cpp_value = BiquadFilterType::Highpass;

    else if (value_string == "bandpass"sv)
        cpp_value = BiquadFilterType::Bandpass;

    else if (value_string == "lowshelf"sv)
        cpp_value = BiquadFilterType::Lowshelf;

    else if (value_string == "highshelf"sv)
        cpp_value = BiquadFilterType::Highshelf;

    else if (value_string == "peaking"sv)
        cpp_value = BiquadFilterType::Peaking;

    else if (value_string == "notch"sv)
        cpp_value = BiquadFilterType::Notch;

    else if (value_string == "allpass"sv)
        cpp_value = BiquadFilterType::Allpass;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_type(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(BiquadFilterNodePrototype::frequency_getter)
{
    WebIDL::log_trace(vm, "BiquadFilterNodePrototype::frequency_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->frequency(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BiquadFilterNodePrototype::detune_getter)
{
    WebIDL::log_trace(vm, "BiquadFilterNodePrototype::detune_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->detune(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BiquadFilterNodePrototype::q_getter)
{
    WebIDL::log_trace(vm, "BiquadFilterNodePrototype::q_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->q(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BiquadFilterNodePrototype::gain_getter)
{
    WebIDL::log_trace(vm, "BiquadFilterNodePrototype::gain_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->gain(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BiquadFilterNodePrototype::get_frequency_response)
{
    WebIDL::log_trace(vm, "BiquadFilterNodePrototype::get_frequency_response");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getFrequencyResponse", "3");

    auto arg0 = vm.argument(0);

    GC::Root<WebIDL::BufferSource> frequency_hz;

    if (!arg0.is_object() || !(is<JS::TypedArrayBase>(arg0.as_object()) || is<JS::ArrayBuffer>(arg0.as_object()) || is<JS::DataView>(arg0.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Float32Array");

    frequency_hz = GC::make_root(realm.create<WebIDL::BufferSource>(arg0.as_object()));

    auto arg1 = vm.argument(1);

    GC::Root<WebIDL::BufferSource> mag_response;

    if (!arg1.is_object() || !(is<JS::TypedArrayBase>(arg1.as_object()) || is<JS::ArrayBuffer>(arg1.as_object()) || is<JS::DataView>(arg1.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Float32Array");

    mag_response = GC::make_root(realm.create<WebIDL::BufferSource>(arg1.as_object()));

    auto arg2 = vm.argument(2);

    GC::Root<WebIDL::BufferSource> phase_response;

    if (!arg2.is_object() || !(is<JS::TypedArrayBase>(arg2.as_object()) || is<JS::ArrayBuffer>(arg2.as_object()) || is<JS::DataView>(arg2.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Float32Array");

    phase_response = GC::make_root(realm.create<WebIDL::BufferSource>(arg2.as_object()));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_frequency_response(frequency_hz, mag_response, phase_response); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
