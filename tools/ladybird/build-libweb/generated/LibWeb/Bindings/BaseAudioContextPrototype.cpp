
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
#include <LibWeb/Bindings/BaseAudioContextPrototype.h>
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

#if __has_include(<LibWeb/Bindings/EventTargetPrototype.h>)
#    include <LibWeb/Bindings/EventTargetPrototype.h>
#endif


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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioParam.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioScheduledSourceNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioNode.h>

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

GC_DEFINE_ALLOCATOR(BaseAudioContextPrototype);

BaseAudioContextPrototype::BaseAudioContextPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

BaseAudioContextPrototype::~BaseAudioContextPrototype()
{
}

void BaseAudioContextPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    auto native_destination_getter = JS::NativeFunction::create(realm, destination_getter, 0, "destination"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_destination_setter;

    define_direct_accessor("destination"_utf16_fly_string, native_destination_getter, native_destination_setter, default_attributes);

    auto native_sample_rate_getter = JS::NativeFunction::create(realm, sample_rate_getter, 0, "sampleRate"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_sample_rate_setter;

    define_direct_accessor("sampleRate"_utf16_fly_string, native_sample_rate_getter, native_sample_rate_setter, default_attributes);

    auto native_current_time_getter = JS::NativeFunction::create(realm, current_time_getter, 0, "currentTime"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_current_time_setter;

    define_direct_accessor("currentTime"_utf16_fly_string, native_current_time_getter, native_current_time_setter, default_attributes);

    auto native_listener_getter = JS::NativeFunction::create(realm, listener_getter, 0, "listener"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_listener_setter;

    define_direct_accessor("listener"_utf16_fly_string, native_listener_getter, native_listener_setter, default_attributes);

    auto native_state_getter = JS::NativeFunction::create(realm, state_getter, 0, "state"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_state_setter;

    define_direct_accessor("state"_utf16_fly_string, native_state_getter, native_state_setter, default_attributes);

    auto native_render_quantum_size_getter = JS::NativeFunction::create(realm, render_quantum_size_getter, 0, "renderQuantumSize"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_render_quantum_size_setter;

    define_direct_accessor("renderQuantumSize"_utf16_fly_string, native_render_quantum_size_getter, native_render_quantum_size_setter, default_attributes);

    define_direct_property("audioWorklet"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    auto native_onstatechange_getter = JS::NativeFunction::create(realm, onstatechange_getter, 0, "onstatechange"_utf16_fly_string, &realm, "get"sv);

    auto native_onstatechange_setter = JS::NativeFunction::create(realm, onstatechange_setter, 1, "onstatechange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onstatechange"_utf16_fly_string, native_onstatechange_getter, native_onstatechange_setter, default_attributes);

        define_direct_property("createConvolver"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("createIIRFilter"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("createWaveShaper"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_native_function(realm, "createAnalyser"_utf16_fly_string, create_analyser, 0, default_attributes);

    define_native_function(realm, "createBiquadFilter"_utf16_fly_string, create_biquad_filter, 0, default_attributes);

    define_native_function(realm, "createBuffer"_utf16_fly_string, create_buffer, 3, default_attributes);

    define_native_function(realm, "createBufferSource"_utf16_fly_string, create_buffer_source, 0, default_attributes);

    define_native_function(realm, "createChannelMerger"_utf16_fly_string, create_channel_merger, 0, default_attributes);

    define_native_function(realm, "createChannelSplitter"_utf16_fly_string, create_channel_splitter, 0, default_attributes);

    define_native_function(realm, "createConstantSource"_utf16_fly_string, create_constant_source, 0, default_attributes);

    define_native_function(realm, "createDelay"_utf16_fly_string, create_delay, 0, default_attributes);

    define_native_function(realm, "createDynamicsCompressor"_utf16_fly_string, create_dynamics_compressor, 0, default_attributes);

    define_native_function(realm, "createGain"_utf16_fly_string, create_gain, 0, default_attributes);

    define_native_function(realm, "createOscillator"_utf16_fly_string, create_oscillator, 0, default_attributes);

    define_native_function(realm, "createPanner"_utf16_fly_string, create_panner, 0, default_attributes);

    define_native_function(realm, "createPeriodicWave"_utf16_fly_string, create_periodic_wave, 2, default_attributes);

    define_native_function(realm, "createScriptProcessor"_utf16_fly_string, create_script_processor, 0, default_attributes);

    define_native_function(realm, "createStereoPanner"_utf16_fly_string, create_stereo_panner, 0, default_attributes);

    define_native_function(realm, "decodeAudioData"_utf16_fly_string, decode_audio_data, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "BaseAudioContext"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void BaseAudioContextPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<WebAudio::BaseAudioContext*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<WebAudio::BaseAudioContext>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BaseAudioContext");
    return static_cast<WebAudio::BaseAudioContext*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::destination_getter)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::destination_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->destination(); }));

    return &const_cast<AudioDestinationNode&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::sample_rate_getter)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::sample_rate_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->sample_rate(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::current_time_getter)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::current_time_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->current_time(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::listener_getter)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::listener_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->listener(); }));

    return &const_cast<AudioListener&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::state_getter)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::state_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->state(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::render_quantum_size_getter)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::render_quantum_size_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->render_quantum_size(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::onstatechange_getter)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::onstatechange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onstatechange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::onstatechange_setter)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::onstatechange_setter");
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

    GC::Ptr<WebIDL::CallbackType> cpp_value;
    if (value.is_object())
        cpp_value = vm.heap().allocate<WebIDL::CallbackType>(value.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onstatechange(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::create_analyser)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::create_analyser");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_analyser(); }));

    return &const_cast<AnalyserNode&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::create_biquad_filter)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::create_biquad_filter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_biquad_filter(); }));

    return &const_cast<BiquadFilterNode&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::create_buffer)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::create_buffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "createBuffer", "3");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong number_of_channels;

    number_of_channels = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong length;

    length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    float sample_rate = TRY(arg2.to_double(vm));

    if (isinf(sample_rate) || isnan(sample_rate)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "sampleRate");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_buffer(number_of_channels, length, sample_rate); }));

    return &const_cast<AudioBuffer&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::create_buffer_source)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::create_buffer_source");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_buffer_source(); }));

    return &const_cast<AudioBufferSourceNode&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::create_channel_merger)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::create_channel_merger");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong number_of_inputs;

    if (!arg0.is_undefined())

    number_of_inputs = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        number_of_inputs = static_cast<WebIDL::UnsignedLong>(6);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_channel_merger(number_of_inputs); }));

    return &const_cast<ChannelMergerNode&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::create_channel_splitter)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::create_channel_splitter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong number_of_outputs;

    if (!arg0.is_undefined())

    number_of_outputs = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        number_of_outputs = static_cast<WebIDL::UnsignedLong>(6);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_channel_splitter(number_of_outputs); }));

    return &const_cast<ChannelSplitterNode&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::create_constant_source)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::create_constant_source");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_constant_source(); }));

    return &const_cast<ConstantSourceNode&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::create_delay)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::create_delay");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    double max_delay_time;

    if (!arg0.is_undefined())
        max_delay_time = TRY(arg0.to_double(vm));

    else
        max_delay_time = 1.0;

    if (isinf(max_delay_time) || isnan(max_delay_time)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "maxDelayTime");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_delay(max_delay_time); }));

    return &const_cast<DelayNode&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::create_dynamics_compressor)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::create_dynamics_compressor");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_dynamics_compressor(); }));

    return &const_cast<DynamicsCompressorNode&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::create_gain)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::create_gain");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_gain(); }));

    return &const_cast<GainNode&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::create_oscillator)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::create_oscillator");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_oscillator(); }));

    return &const_cast<OscillatorNode&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::create_panner)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::create_panner");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_panner(); }));

    return &const_cast<PannerNode&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::create_periodic_wave)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::create_periodic_wave");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "createPeriodicWave", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg0);

    auto arg0_iterator_method0 = TRY(arg0.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg0_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg0);

    auto arg0_iterator1 = TRY(JS::get_iterator_from_method(vm, arg0, *arg0_iterator_method0));

    Vector<float> real;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg0_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    if (isinf(sequence_item1) || isnan(sequence_item1)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "arg0");
    }
    
    real.append(sequence_item1);
    }

    auto arg1 = vm.argument(1);

    if (!arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg1);

    auto arg1_iterator_method0 = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg1_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg1);

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *arg1_iterator_method0));

    Vector<float> imag;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    if (isinf(sequence_item1) || isnan(sequence_item1)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "arg1");
    }
    
    imag.append(sequence_item1);
    }

    auto arg2 = vm.argument(2);

    if (!arg2.is_nullish() && !arg2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PeriodicWaveConstraints");

    PeriodicWaveConstraints constraints {};

    auto disable_normalization_property_value_0 = JS::js_undefined();
    if (arg2.is_object())
        disable_normalization_property_value_0 = TRY(arg2.as_object().get("disableNormalization"_utf16_fly_string));

    bool disable_normalization_value_0;

    if (!disable_normalization_property_value_0.is_undefined())

    disable_normalization_value_0 = disable_normalization_property_value_0.to_boolean();

    else
        disable_normalization_value_0 = static_cast<bool>(false);

    constraints.disable_normalization = disable_normalization_value_0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_periodic_wave(real, imag, constraints); }));

    return &const_cast<PeriodicWave&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::create_script_processor)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::create_script_processor");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong buffer_size;

    if (!arg0.is_undefined())

    buffer_size = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        buffer_size = static_cast<WebIDL::UnsignedLong>(0);

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong number_of_input_channels;

    if (!arg1.is_undefined())

    number_of_input_channels = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        number_of_input_channels = static_cast<WebIDL::UnsignedLong>(2);

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong number_of_output_channels;

    if (!arg2.is_undefined())

    number_of_output_channels = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        number_of_output_channels = static_cast<WebIDL::UnsignedLong>(2);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_script_processor(buffer_size, number_of_input_channels, number_of_output_channels); }));

    return &const_cast<ScriptProcessorNode&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::create_stereo_panner)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::create_stereo_panner");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_stereo_panner(); }));

    return &const_cast<StereoPannerNode&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(BaseAudioContextPrototype::decode_audio_data)
{
    WebIDL::log_trace(vm, "BaseAudioContextPrototype::decode_audio_data");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "decodeAudioData");

    auto arg0 = vm.argument(0);

    GC::Root<WebIDL::BufferSource> audio_data;

    if (!arg0.is_object() || !(is<JS::TypedArrayBase>(arg0.as_object()) || is<JS::ArrayBuffer>(arg0.as_object()) || is<JS::DataView>(arg0.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBuffer");

    audio_data = GC::make_root(realm.create<WebIDL::BufferSource>(arg0.as_object()));

    auto arg1 = vm.argument(1);

    GC::Ptr<WebIDL::CallbackType> success_callback;
    if (arg1.is_object())
        success_callback = vm.heap().allocate<WebIDL::CallbackType>(arg1.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    auto arg2 = vm.argument(2);

    GC::Ptr<WebIDL::CallbackType> error_callback;
    if (arg2.is_object())
        error_callback = vm.heap().allocate<WebIDL::CallbackType>(arg2.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->decode_audio_data(audio_data, success_callback, error_callback); }));

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
