
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
#include <LibWeb/Bindings/PannerNodeConstructor.h>
#include <LibWeb/Bindings/PannerNodePrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/PannerNode.h>

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

GC_DEFINE_ALLOCATOR(PannerNodeConstructor);

PannerNodeConstructor::PannerNodeConstructor(JS::Realm& realm)
    : NativeFunction("PannerNode"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

PannerNodeConstructor::~PannerNodeConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> PannerNodeConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "PannerNode");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> PannerNodeConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "PannerNodeConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface PannerNode:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<PannerNodePrototype>(*target_realm, "PannerNode"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "PannerNode");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<BaseAudioContext>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BaseAudioContext");

    auto& context = static_cast<BaseAudioContext&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PannerOptions");

    PannerOptions options {};

    auto cone_inner_angle_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        cone_inner_angle_property_value_0 = TRY(arg1.as_object().get("coneInnerAngle"_utf16_fly_string));

    double cone_inner_angle_value_0;

    if (!cone_inner_angle_property_value_0.is_undefined())
        cone_inner_angle_value_0 = TRY(cone_inner_angle_property_value_0.to_double(vm));

    else
        cone_inner_angle_value_0 = 360;

    if (isinf(cone_inner_angle_value_0) || isnan(cone_inner_angle_value_0)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "coneInnerAngle");
    }
    
    options.cone_inner_angle = cone_inner_angle_value_0;

    auto cone_outer_angle_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        cone_outer_angle_property_value_1 = TRY(arg1.as_object().get("coneOuterAngle"_utf16_fly_string));

    double cone_outer_angle_value_1;

    if (!cone_outer_angle_property_value_1.is_undefined())
        cone_outer_angle_value_1 = TRY(cone_outer_angle_property_value_1.to_double(vm));

    else
        cone_outer_angle_value_1 = 360;

    if (isinf(cone_outer_angle_value_1) || isnan(cone_outer_angle_value_1)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "coneOuterAngle");
    }
    
    options.cone_outer_angle = cone_outer_angle_value_1;

    auto cone_outer_gain_property_value_2 = JS::js_undefined();
    if (arg1.is_object())
        cone_outer_gain_property_value_2 = TRY(arg1.as_object().get("coneOuterGain"_utf16_fly_string));

    double cone_outer_gain_value_2;

    if (!cone_outer_gain_property_value_2.is_undefined())
        cone_outer_gain_value_2 = TRY(cone_outer_gain_property_value_2.to_double(vm));

    else
        cone_outer_gain_value_2 = 0;

    if (isinf(cone_outer_gain_value_2) || isnan(cone_outer_gain_value_2)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "coneOuterGain");
    }
    
    options.cone_outer_gain = cone_outer_gain_value_2;

    auto distance_model_property_value_3 = JS::js_undefined();
    if (arg1.is_object())
        distance_model_property_value_3 = TRY(arg1.as_object().get("distanceModel"_utf16_fly_string));

    DistanceModelType distance_model_value_3 { DistanceModelType::Inverse };

    if (!distance_model_property_value_3.is_undefined()) {

    auto distance_model_property_value_3_string = TRY(distance_model_property_value_3.to_string(vm));

    if (distance_model_property_value_3_string == "linear"sv)
        distance_model_value_3 = DistanceModelType::Linear;

    else if (distance_model_property_value_3_string == "inverse"sv)
        distance_model_value_3 = DistanceModelType::Inverse;

    else if (distance_model_property_value_3_string == "exponential"sv)
        distance_model_value_3 = DistanceModelType::Exponential;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, distance_model_property_value_3_string, "DistanceModelType");

    }

    options.distance_model = distance_model_value_3;

    auto max_distance_property_value_4 = JS::js_undefined();
    if (arg1.is_object())
        max_distance_property_value_4 = TRY(arg1.as_object().get("maxDistance"_utf16_fly_string));

    double max_distance_value_4;

    if (!max_distance_property_value_4.is_undefined())
        max_distance_value_4 = TRY(max_distance_property_value_4.to_double(vm));

    else
        max_distance_value_4 = 10000;

    if (isinf(max_distance_value_4) || isnan(max_distance_value_4)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "maxDistance");
    }
    
    options.max_distance = max_distance_value_4;

    auto orientation_x_property_value_5 = JS::js_undefined();
    if (arg1.is_object())
        orientation_x_property_value_5 = TRY(arg1.as_object().get("orientationX"_utf16_fly_string));

    float orientation_x_value_5;

    if (!orientation_x_property_value_5.is_undefined())
        orientation_x_value_5 = TRY(orientation_x_property_value_5.to_double(vm));

    else
        orientation_x_value_5 = 1;

    if (isinf(orientation_x_value_5) || isnan(orientation_x_value_5)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "orientationX");
    }
    
    options.orientation_x = orientation_x_value_5;

    auto orientation_y_property_value_6 = JS::js_undefined();
    if (arg1.is_object())
        orientation_y_property_value_6 = TRY(arg1.as_object().get("orientationY"_utf16_fly_string));

    float orientation_y_value_6;

    if (!orientation_y_property_value_6.is_undefined())
        orientation_y_value_6 = TRY(orientation_y_property_value_6.to_double(vm));

    else
        orientation_y_value_6 = 0;

    if (isinf(orientation_y_value_6) || isnan(orientation_y_value_6)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "orientationY");
    }
    
    options.orientation_y = orientation_y_value_6;

    auto orientation_z_property_value_7 = JS::js_undefined();
    if (arg1.is_object())
        orientation_z_property_value_7 = TRY(arg1.as_object().get("orientationZ"_utf16_fly_string));

    float orientation_z_value_7;

    if (!orientation_z_property_value_7.is_undefined())
        orientation_z_value_7 = TRY(orientation_z_property_value_7.to_double(vm));

    else
        orientation_z_value_7 = 0;

    if (isinf(orientation_z_value_7) || isnan(orientation_z_value_7)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "orientationZ");
    }
    
    options.orientation_z = orientation_z_value_7;

    auto panning_model_property_value_8 = JS::js_undefined();
    if (arg1.is_object())
        panning_model_property_value_8 = TRY(arg1.as_object().get("panningModel"_utf16_fly_string));

    PanningModelType panning_model_value_8 { PanningModelType::Equalpower };

    if (!panning_model_property_value_8.is_undefined()) {

    auto panning_model_property_value_8_string = TRY(panning_model_property_value_8.to_string(vm));

    if (panning_model_property_value_8_string == "equalpower"sv)
        panning_model_value_8 = PanningModelType::Equalpower;

    else if (panning_model_property_value_8_string == "HRTF"sv)
        panning_model_value_8 = PanningModelType::Hrtf;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, panning_model_property_value_8_string, "PanningModelType");

    }

    options.panning_model = panning_model_value_8;

    auto position_x_property_value_9 = JS::js_undefined();
    if (arg1.is_object())
        position_x_property_value_9 = TRY(arg1.as_object().get("positionX"_utf16_fly_string));

    float position_x_value_9;

    if (!position_x_property_value_9.is_undefined())
        position_x_value_9 = TRY(position_x_property_value_9.to_double(vm));

    else
        position_x_value_9 = 0;

    if (isinf(position_x_value_9) || isnan(position_x_value_9)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "positionX");
    }
    
    options.position_x = position_x_value_9;

    auto position_y_property_value_10 = JS::js_undefined();
    if (arg1.is_object())
        position_y_property_value_10 = TRY(arg1.as_object().get("positionY"_utf16_fly_string));

    float position_y_value_10;

    if (!position_y_property_value_10.is_undefined())
        position_y_value_10 = TRY(position_y_property_value_10.to_double(vm));

    else
        position_y_value_10 = 0;

    if (isinf(position_y_value_10) || isnan(position_y_value_10)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "positionY");
    }
    
    options.position_y = position_y_value_10;

    auto position_z_property_value_11 = JS::js_undefined();
    if (arg1.is_object())
        position_z_property_value_11 = TRY(arg1.as_object().get("positionZ"_utf16_fly_string));

    float position_z_value_11;

    if (!position_z_property_value_11.is_undefined())
        position_z_value_11 = TRY(position_z_property_value_11.to_double(vm));

    else
        position_z_value_11 = 0;

    if (isinf(position_z_value_11) || isnan(position_z_value_11)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "positionZ");
    }
    
    options.position_z = position_z_value_11;

    auto ref_distance_property_value_12 = JS::js_undefined();
    if (arg1.is_object())
        ref_distance_property_value_12 = TRY(arg1.as_object().get("refDistance"_utf16_fly_string));

    double ref_distance_value_12;

    if (!ref_distance_property_value_12.is_undefined())
        ref_distance_value_12 = TRY(ref_distance_property_value_12.to_double(vm));

    else
        ref_distance_value_12 = 1;

    if (isinf(ref_distance_value_12) || isnan(ref_distance_value_12)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "refDistance");
    }
    
    options.ref_distance = ref_distance_value_12;

    auto rolloff_factor_property_value_13 = JS::js_undefined();
    if (arg1.is_object())
        rolloff_factor_property_value_13 = TRY(arg1.as_object().get("rolloffFactor"_utf16_fly_string));

    double rolloff_factor_value_13;

    if (!rolloff_factor_property_value_13.is_undefined())
        rolloff_factor_value_13 = TRY(rolloff_factor_property_value_13.to_double(vm));

    else
        rolloff_factor_value_13 = 1;

    if (isinf(rolloff_factor_value_13) || isnan(rolloff_factor_value_13)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "rolloffFactor");
    }
    
    options.rolloff_factor = rolloff_factor_value_13;

    auto channel_count_property_value_14 = JS::js_undefined();
    if (arg1.is_object())
        channel_count_property_value_14 = TRY(arg1.as_object().get("channelCount"_utf16_fly_string));

    if (!channel_count_property_value_14.is_undefined()) {

    Optional<WebIDL::UnsignedLong> channel_count_value_14;

    if (!channel_count_property_value_14.is_undefined())

    channel_count_value_14 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, channel_count_property_value_14, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    options.channel_count = channel_count_value_14;

    }

    auto channel_count_mode_property_value_15 = JS::js_undefined();
    if (arg1.is_object())
        channel_count_mode_property_value_15 = TRY(arg1.as_object().get("channelCountMode"_utf16_fly_string));

    if (!channel_count_mode_property_value_15.is_undefined()) {

    ChannelCountMode channel_count_mode_value_15 { ChannelCountMode::Max };

    if (!channel_count_mode_property_value_15.is_undefined()) {

    auto channel_count_mode_property_value_15_string = TRY(channel_count_mode_property_value_15.to_string(vm));

    if (channel_count_mode_property_value_15_string == "max"sv)
        channel_count_mode_value_15 = ChannelCountMode::Max;

    else if (channel_count_mode_property_value_15_string == "clamped-max"sv)
        channel_count_mode_value_15 = ChannelCountMode::ClampedMax;

    else if (channel_count_mode_property_value_15_string == "explicit"sv)
        channel_count_mode_value_15 = ChannelCountMode::Explicit;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, channel_count_mode_property_value_15_string, "ChannelCountMode");

    }

    options.channel_count_mode = channel_count_mode_value_15;

    }

    auto channel_interpretation_property_value_16 = JS::js_undefined();
    if (arg1.is_object())
        channel_interpretation_property_value_16 = TRY(arg1.as_object().get("channelInterpretation"_utf16_fly_string));

    if (!channel_interpretation_property_value_16.is_undefined()) {

    ChannelInterpretation channel_interpretation_value_16 { ChannelInterpretation::Speakers };

    if (!channel_interpretation_property_value_16.is_undefined()) {

    auto channel_interpretation_property_value_16_string = TRY(channel_interpretation_property_value_16.to_string(vm));

    if (channel_interpretation_property_value_16_string == "speakers"sv)
        channel_interpretation_value_16 = ChannelInterpretation::Speakers;

    else if (channel_interpretation_property_value_16_string == "discrete"sv)
        channel_interpretation_value_16 = ChannelInterpretation::Discrete;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, channel_interpretation_property_value_16_string, "ChannelInterpretation");

    }

    options.channel_interpretation = channel_interpretation_value_16;

    }

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return WebAudio::PannerNode::construct_impl(realm, context, options); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface PannerNode" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within WebAudio::PannerNode::construct_impl().
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


void PannerNodeConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    set_prototype(&ensure_web_constructor<AudioNodePrototype>(realm, "AudioNode"_fly_string));

    define_direct_property(vm.names.length, JS::Value(1), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "PannerNode"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<PannerNodePrototype>(realm, "PannerNode"_fly_string), 0);


}

} // namespace Web::Bindings
