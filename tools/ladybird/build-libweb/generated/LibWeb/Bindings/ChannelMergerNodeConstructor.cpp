
#include <LibIDL/Types.h>
#include <LibGC/Heap.h>
#include <LibJS/Runtime/Array.h>
#include <LibJS/Runtime/ArrayBuffer.h>
#include <LibJS/Runtime/DataView.h>
#include <LibJS/Runtime/GlobalObject.h>
#include <LibJS/Runtime/Iterator.h>
#include <LibJS/Runtime/PromiseConstructor.h>
#include <LibJS/Runtime/ValueInlines.h>
#include <LibJS/Runtime/TypedArray.h>
#include <LibWeb/Bindings/ChannelMergerNodeConstructor.h>
#include <LibWeb/Bindings/ChannelMergerNodePrototype.h>
#include <LibWeb/Bindings/ExceptionOrUtils.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/HTML/WindowProxy.h>
#include <LibWeb/WebIDL/AbstractOperations.h>
#include <LibWeb/WebIDL/Buffers.h>
#include <LibWeb/WebIDL/CallbackType.h>
#include <LibWeb/WebIDL/OverloadResolution.h>
#include <LibWeb/WebIDL/Tracing.h>
#include <LibWeb/WebIDL/Types.h>

#if __has_include(<LibWeb/Bindings/AudioNodePrototype.h>)
#    include <LibWeb/Bindings/AudioNodePrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/ChannelMergerNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/BaseAudioContext.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioParam.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioBuffer.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioBufferSourceNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioDestinationNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioListener.h>

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

GC_DEFINE_ALLOCATOR(ChannelMergerNodeConstructor);

ChannelMergerNodeConstructor::ChannelMergerNodeConstructor(JS::Realm& realm)
    : NativeFunction("ChannelMergerNode"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

ChannelMergerNodeConstructor::~ChannelMergerNodeConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> ChannelMergerNodeConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "ChannelMergerNode");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> ChannelMergerNodeConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "ChannelMergerNodeConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface ChannelMergerNode:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<ChannelMergerNodePrototype>(*target_realm, "ChannelMergerNode"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "ChannelMergerNode");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<BaseAudioContext>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BaseAudioContext");

    auto& context = static_cast<BaseAudioContext&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ChannelMergerOptions");

    ChannelMergerOptions options {};

    auto number_of_inputs_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        number_of_inputs_property_value_0 = TRY(arg1.as_object().get("numberOfInputs"_utf16_fly_string));

    WebIDL::UnsignedLong number_of_inputs_value_0;

    if (!number_of_inputs_property_value_0.is_undefined())

    number_of_inputs_value_0 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, number_of_inputs_property_value_0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        number_of_inputs_value_0 = static_cast<WebIDL::UnsignedLong>(6);

    options.number_of_inputs = number_of_inputs_value_0;

    auto channel_count_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        channel_count_property_value_1 = TRY(arg1.as_object().get("channelCount"_utf16_fly_string));

    if (!channel_count_property_value_1.is_undefined()) {

    Optional<WebIDL::UnsignedLong> channel_count_value_1;

    if (!channel_count_property_value_1.is_undefined())

    channel_count_value_1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, channel_count_property_value_1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    options.channel_count = channel_count_value_1;

    }

    auto channel_count_mode_property_value_2 = JS::js_undefined();
    if (arg1.is_object())
        channel_count_mode_property_value_2 = TRY(arg1.as_object().get("channelCountMode"_utf16_fly_string));

    if (!channel_count_mode_property_value_2.is_undefined()) {

    ChannelCountMode channel_count_mode_value_2 { ChannelCountMode::Max };

    if (!channel_count_mode_property_value_2.is_undefined()) {

    auto channel_count_mode_property_value_2_string = TRY(channel_count_mode_property_value_2.to_string(vm));

    if (channel_count_mode_property_value_2_string == "max"sv)
        channel_count_mode_value_2 = ChannelCountMode::Max;

    else if (channel_count_mode_property_value_2_string == "clamped-max"sv)
        channel_count_mode_value_2 = ChannelCountMode::ClampedMax;

    else if (channel_count_mode_property_value_2_string == "explicit"sv)
        channel_count_mode_value_2 = ChannelCountMode::Explicit;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, channel_count_mode_property_value_2_string, "ChannelCountMode");

    }

    options.channel_count_mode = channel_count_mode_value_2;

    }

    auto channel_interpretation_property_value_3 = JS::js_undefined();
    if (arg1.is_object())
        channel_interpretation_property_value_3 = TRY(arg1.as_object().get("channelInterpretation"_utf16_fly_string));

    if (!channel_interpretation_property_value_3.is_undefined()) {

    ChannelInterpretation channel_interpretation_value_3 { ChannelInterpretation::Speakers };

    if (!channel_interpretation_property_value_3.is_undefined()) {

    auto channel_interpretation_property_value_3_string = TRY(channel_interpretation_property_value_3.to_string(vm));

    if (channel_interpretation_property_value_3_string == "speakers"sv)
        channel_interpretation_value_3 = ChannelInterpretation::Speakers;

    else if (channel_interpretation_property_value_3_string == "discrete"sv)
        channel_interpretation_value_3 = ChannelInterpretation::Discrete;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, channel_interpretation_property_value_3_string, "ChannelInterpretation");

    }

    options.channel_interpretation = channel_interpretation_value_3;

    }

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return WebAudio::ChannelMergerNode::construct_impl(realm, context, options); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface ChannelMergerNode" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within WebAudio::ChannelMergerNode::construct_impl().
    //  8. Let interfaces be the inclusive inherited interfaces of interface.
    //  9. For every interface ancestor interface in interfaces:
    //    9.1. Let unforgeables be the value of the [[Unforgeables]] slot of the interface object of ancestor interface in realm.
    //    9.2. Let keys be ! unforgeables.[[OwnPropertyKeys]]().
    //    9.3. For each element key of keys:
    //      9.3.1. Let descriptor be ! unforgeables.[[GetOwnProperty]](key).
    //      9.3.2. Perform ! DefinePropertyOrThrow(instance, key, descriptor).
    //  10. If interface is declared with the [Global] extended attribute, then:
    //    10.1. Define the regular operations of interface on instance, given realm.
    //    10.2. Define the regular attributes of interface on instance, given realm.
    //    10.3. Define the iteration methods of interface on instance given realm.
    //    10.4. Define the asynchronous iteration methods of interface on instance given realm.
    //    10.5. Define the global property references on instance, given realm.
    //    10.6. Set instance.[[SetPrototypeOf]] as defined in § 3.8.1 [[SetPrototypeOf]].
    //  11. Otherwise, if interfaces contains an interface which supports indexed properties, named properties, or both:
    //    11.1. Set instance.[[GetOwnProperty]] as defined in § 3.9.1 [[GetOwnProperty]].
    //    11.2. Set instance.[[Set]] as defined in § 3.9.2 [[Set]].
    //    11.3. Set instance.[[DefineOwnProperty]] as defined in § 3.9.3 [[DefineOwnProperty]].
    //    11.4. Set instance.[[Delete]] as defined in § 3.9.4 [[Delete]].
    //    11.5. Set instance.[[PreventExtensions]] as defined in § 3.9.5 [[PreventExtensions]].
    //    11.6. Set instance.[[OwnPropertyKeys]] as defined in § 3.9.6 [[OwnPropertyKeys]].

    return *impl;
}


void ChannelMergerNodeConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    set_prototype(&ensure_web_constructor<AudioNodePrototype>(realm, "AudioNode"_fly_string));

    define_direct_property(vm.names.length, JS::Value(1), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "ChannelMergerNode"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<ChannelMergerNodePrototype>(realm, "ChannelMergerNode"_fly_string), 0);


}

} // namespace Web::Bindings
