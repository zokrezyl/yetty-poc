
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
#include <LibWeb/Bindings/AudioNodePrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioParam.h>

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

GC_DEFINE_ALLOCATOR(AudioNodePrototype);

AudioNodePrototype::AudioNodePrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

AudioNodePrototype::~AudioNodePrototype()
{
}

void AudioNodePrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    auto native_context_getter = JS::NativeFunction::create(realm, context_getter, 0, "context"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_context_setter;

    define_direct_accessor("context"_utf16_fly_string, native_context_getter, native_context_setter, default_attributes);

    auto native_number_of_inputs_getter = JS::NativeFunction::create(realm, number_of_inputs_getter, 0, "numberOfInputs"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_number_of_inputs_setter;

    define_direct_accessor("numberOfInputs"_utf16_fly_string, native_number_of_inputs_getter, native_number_of_inputs_setter, default_attributes);

    auto native_number_of_outputs_getter = JS::NativeFunction::create(realm, number_of_outputs_getter, 0, "numberOfOutputs"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_number_of_outputs_setter;

    define_direct_accessor("numberOfOutputs"_utf16_fly_string, native_number_of_outputs_getter, native_number_of_outputs_setter, default_attributes);

    auto native_channel_count_getter = JS::NativeFunction::create(realm, channel_count_getter, 0, "channelCount"_utf16_fly_string, &realm, "get"sv);

    auto native_channel_count_setter = JS::NativeFunction::create(realm, channel_count_setter, 1, "channelCount"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("channelCount"_utf16_fly_string, native_channel_count_getter, native_channel_count_setter, default_attributes);

    auto native_channel_count_mode_getter = JS::NativeFunction::create(realm, channel_count_mode_getter, 0, "channelCountMode"_utf16_fly_string, &realm, "get"sv);

    auto native_channel_count_mode_setter = JS::NativeFunction::create(realm, channel_count_mode_setter, 1, "channelCountMode"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("channelCountMode"_utf16_fly_string, native_channel_count_mode_getter, native_channel_count_mode_setter, default_attributes);

    auto native_channel_interpretation_getter = JS::NativeFunction::create(realm, channel_interpretation_getter, 0, "channelInterpretation"_utf16_fly_string, &realm, "get"sv);

    auto native_channel_interpretation_setter = JS::NativeFunction::create(realm, channel_interpretation_setter, 1, "channelInterpretation"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("channelInterpretation"_utf16_fly_string, native_channel_interpretation_getter, native_channel_interpretation_setter, default_attributes);

    define_native_function(realm, "connect"_utf16_fly_string, connect, 1, default_attributes);

    define_native_function(realm, "disconnect"_utf16_fly_string, disconnect, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "AudioNode"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void AudioNodePrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<WebAudio::AudioNode*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<WebAudio::AudioNode>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AudioNode");
    return static_cast<WebAudio::AudioNode*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::context_getter)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::context_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->context(); }));

    return &const_cast<BaseAudioContext&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::number_of_inputs_getter)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::number_of_inputs_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->number_of_inputs(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::number_of_outputs_getter)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::number_of_outputs_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->number_of_outputs(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::channel_count_getter)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::channel_count_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->channel_count(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::channel_count_setter)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::channel_count_setter");
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

    WebIDL::UnsignedLong cpp_value;

    cpp_value = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, value, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_channel_count(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::channel_count_mode_getter)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::channel_count_mode_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->channel_count_mode(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::channel_count_mode_setter)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::channel_count_mode_setter");
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

    ChannelCountMode cpp_value { ChannelCountMode::Max };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == "max"sv)
        cpp_value = ChannelCountMode::Max;

    else if (value_string == "clamped-max"sv)
        cpp_value = ChannelCountMode::ClampedMax;

    else if (value_string == "explicit"sv)
        cpp_value = ChannelCountMode::Explicit;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_channel_count_mode(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::channel_interpretation_getter)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::channel_interpretation_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->channel_interpretation(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::channel_interpretation_setter)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::channel_interpretation_setter");
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

    ChannelInterpretation cpp_value { ChannelInterpretation::Speakers };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == "speakers"sv)
        cpp_value = ChannelInterpretation::Speakers;

    else if (value_string == "discrete"sv)
        cpp_value = ChannelInterpretation::Discrete;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_channel_interpretation(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::connect0)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::connect0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<AudioNode>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AudioNode");

    auto& destination_node = static_cast<AudioNode&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong output;

    if (!arg1.is_undefined())

    output = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        output = static_cast<WebIDL::UnsignedLong>(0);

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong input;

    if (!arg2.is_undefined())

    input = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        input = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->connect(destination_node, output, input); }));

    return &const_cast<AudioNode&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::connect1)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::connect1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<AudioParam>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AudioParam");

    auto& destination_param = static_cast<AudioParam&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong output;

    if (!arg1.is_undefined())

    output = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        output = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->connect(destination_param, output); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::disconnect0)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::disconnect0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->disconnect(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::disconnect1)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::disconnect1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong output;

    output = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->disconnect(output); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::disconnect2)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::disconnect2");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<AudioNode>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AudioNode");

    auto& destination_node = static_cast<AudioNode&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->disconnect(destination_node); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::disconnect3)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::disconnect3");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<AudioNode>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AudioNode");

    auto& destination_node = static_cast<AudioNode&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong output;

    output = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->disconnect(destination_node, output); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::disconnect4)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::disconnect4");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<AudioNode>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AudioNode");

    auto& destination_node = static_cast<AudioNode&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong output;

    output = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong input;

    input = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->disconnect(destination_node, output, input); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::disconnect5)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::disconnect5");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<AudioParam>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AudioParam");

    auto& destination_param = static_cast<AudioParam&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->disconnect(destination_param); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::disconnect6)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::disconnect6");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<AudioParam>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AudioParam");

    auto& destination_param = static_cast<AudioParam&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong output;

    output = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->disconnect(destination_param, output); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::connect)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::connect");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(3, vm.argument_count())) {

    case 1: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("AudioNode", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required});
        overloads.empend(1, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("AudioParam", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required});

        effective_overload_set.emplace(move(overloads), 0);
        break;
    }

    case 2: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("AudioNode", false), make_ref_counted<IDL::Type>("unsigned long", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Optional});
        overloads.empend(1, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("AudioParam", false), make_ref_counted<IDL::Type>("unsigned long", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Optional});

        effective_overload_set.emplace(move(overloads), 0);
        break;
    }

    case 3:
        chosen_overload_callable_id = 0;
        break;


    }

    Vector<StringView> dictionary_types {
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return connect0(vm);

    case 1:

        return connect1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(AudioNodePrototype::disconnect)
{
    WebIDL::log_trace(vm, "AudioNodePrototype::disconnect");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(3, vm.argument_count())) {

    case 0:
        chosen_overload_callable_id = 0;
        break;


    case 1: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(3);

        overloads.empend(1, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("unsigned long", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required});
        overloads.empend(2, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("AudioNode", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required});
        overloads.empend(5, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("AudioParam", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required});

        effective_overload_set.emplace(move(overloads), 0);
        break;
    }

    case 2: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(3, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("AudioNode", false), make_ref_counted<IDL::Type>("unsigned long", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Required});
        overloads.empend(6, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("AudioParam", false), make_ref_counted<IDL::Type>("unsigned long", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Required});

        effective_overload_set.emplace(move(overloads), 0);
        break;
    }

    case 3:
        chosen_overload_callable_id = 4;
        break;


    }

    Vector<StringView> dictionary_types {
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return disconnect0(vm);

    case 1:

        return disconnect1(vm);

    case 2:

        return disconnect2(vm);

    case 3:

        return disconnect3(vm);

    case 4:

        return disconnect4(vm);

    case 5:

        return disconnect5(vm);

    case 6:

        return disconnect6(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

} // namespace Web::Bindings
