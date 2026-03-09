
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
#include <LibWeb/Bindings/PannerNodePrototype.h>
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

GC_DEFINE_ALLOCATOR(PannerNodePrototype);

PannerNodePrototype::PannerNodePrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

PannerNodePrototype::~PannerNodePrototype()
{
}

void PannerNodePrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<AudioNodePrototype>(realm, "AudioNode"_fly_string));


    auto native_panning_model_getter = JS::NativeFunction::create(realm, panning_model_getter, 0, "panningModel"_utf16_fly_string, &realm, "get"sv);

    auto native_panning_model_setter = JS::NativeFunction::create(realm, panning_model_setter, 1, "panningModel"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("panningModel"_utf16_fly_string, native_panning_model_getter, native_panning_model_setter, default_attributes);

    auto native_position_x_getter = JS::NativeFunction::create(realm, position_x_getter, 0, "positionX"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_position_x_setter;

    define_direct_accessor("positionX"_utf16_fly_string, native_position_x_getter, native_position_x_setter, default_attributes);

    auto native_position_y_getter = JS::NativeFunction::create(realm, position_y_getter, 0, "positionY"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_position_y_setter;

    define_direct_accessor("positionY"_utf16_fly_string, native_position_y_getter, native_position_y_setter, default_attributes);

    auto native_position_z_getter = JS::NativeFunction::create(realm, position_z_getter, 0, "positionZ"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_position_z_setter;

    define_direct_accessor("positionZ"_utf16_fly_string, native_position_z_getter, native_position_z_setter, default_attributes);

    auto native_orientation_x_getter = JS::NativeFunction::create(realm, orientation_x_getter, 0, "orientationX"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_orientation_x_setter;

    define_direct_accessor("orientationX"_utf16_fly_string, native_orientation_x_getter, native_orientation_x_setter, default_attributes);

    auto native_orientation_y_getter = JS::NativeFunction::create(realm, orientation_y_getter, 0, "orientationY"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_orientation_y_setter;

    define_direct_accessor("orientationY"_utf16_fly_string, native_orientation_y_getter, native_orientation_y_setter, default_attributes);

    auto native_orientation_z_getter = JS::NativeFunction::create(realm, orientation_z_getter, 0, "orientationZ"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_orientation_z_setter;

    define_direct_accessor("orientationZ"_utf16_fly_string, native_orientation_z_getter, native_orientation_z_setter, default_attributes);

    auto native_distance_model_getter = JS::NativeFunction::create(realm, distance_model_getter, 0, "distanceModel"_utf16_fly_string, &realm, "get"sv);

    auto native_distance_model_setter = JS::NativeFunction::create(realm, distance_model_setter, 1, "distanceModel"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("distanceModel"_utf16_fly_string, native_distance_model_getter, native_distance_model_setter, default_attributes);

    auto native_ref_distance_getter = JS::NativeFunction::create(realm, ref_distance_getter, 0, "refDistance"_utf16_fly_string, &realm, "get"sv);

    auto native_ref_distance_setter = JS::NativeFunction::create(realm, ref_distance_setter, 1, "refDistance"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("refDistance"_utf16_fly_string, native_ref_distance_getter, native_ref_distance_setter, default_attributes);

    auto native_max_distance_getter = JS::NativeFunction::create(realm, max_distance_getter, 0, "maxDistance"_utf16_fly_string, &realm, "get"sv);

    auto native_max_distance_setter = JS::NativeFunction::create(realm, max_distance_setter, 1, "maxDistance"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("maxDistance"_utf16_fly_string, native_max_distance_getter, native_max_distance_setter, default_attributes);

    auto native_rolloff_factor_getter = JS::NativeFunction::create(realm, rolloff_factor_getter, 0, "rolloffFactor"_utf16_fly_string, &realm, "get"sv);

    auto native_rolloff_factor_setter = JS::NativeFunction::create(realm, rolloff_factor_setter, 1, "rolloffFactor"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("rolloffFactor"_utf16_fly_string, native_rolloff_factor_getter, native_rolloff_factor_setter, default_attributes);

    auto native_cone_inner_angle_getter = JS::NativeFunction::create(realm, cone_inner_angle_getter, 0, "coneInnerAngle"_utf16_fly_string, &realm, "get"sv);

    auto native_cone_inner_angle_setter = JS::NativeFunction::create(realm, cone_inner_angle_setter, 1, "coneInnerAngle"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("coneInnerAngle"_utf16_fly_string, native_cone_inner_angle_getter, native_cone_inner_angle_setter, default_attributes);

    auto native_cone_outer_angle_getter = JS::NativeFunction::create(realm, cone_outer_angle_getter, 0, "coneOuterAngle"_utf16_fly_string, &realm, "get"sv);

    auto native_cone_outer_angle_setter = JS::NativeFunction::create(realm, cone_outer_angle_setter, 1, "coneOuterAngle"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("coneOuterAngle"_utf16_fly_string, native_cone_outer_angle_getter, native_cone_outer_angle_setter, default_attributes);

    auto native_cone_outer_gain_getter = JS::NativeFunction::create(realm, cone_outer_gain_getter, 0, "coneOuterGain"_utf16_fly_string, &realm, "get"sv);

    auto native_cone_outer_gain_setter = JS::NativeFunction::create(realm, cone_outer_gain_setter, 1, "coneOuterGain"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("coneOuterGain"_utf16_fly_string, native_cone_outer_gain_getter, native_cone_outer_gain_setter, default_attributes);

    define_native_function(realm, "setPosition"_utf16_fly_string, set_position, 3, default_attributes);

    define_native_function(realm, "setOrientation"_utf16_fly_string, set_orientation, 3, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "PannerNode"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void PannerNodePrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<WebAudio::PannerNode*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<WebAudio::PannerNode>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PannerNode");
    return static_cast<WebAudio::PannerNode*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::panning_model_getter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::panning_model_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->panning_model(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::panning_model_setter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::panning_model_setter");
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

    PanningModelType cpp_value { PanningModelType::Equalpower };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == "equalpower"sv)
        cpp_value = PanningModelType::Equalpower;

    else if (value_string == "HRTF"sv)
        cpp_value = PanningModelType::Hrtf;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_panning_model(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::position_x_getter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::position_x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->position_x(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::position_y_getter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::position_y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->position_y(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::position_z_getter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::position_z_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->position_z(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::orientation_x_getter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::orientation_x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->orientation_x(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::orientation_y_getter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::orientation_y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->orientation_y(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::orientation_z_getter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::orientation_z_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->orientation_z(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::distance_model_getter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::distance_model_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->distance_model(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::distance_model_setter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::distance_model_setter");
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

    DistanceModelType cpp_value { DistanceModelType::Linear };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == "linear"sv)
        cpp_value = DistanceModelType::Linear;

    else if (value_string == "inverse"sv)
        cpp_value = DistanceModelType::Inverse;

    else if (value_string == "exponential"sv)
        cpp_value = DistanceModelType::Exponential;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_distance_model(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::ref_distance_getter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::ref_distance_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ref_distance(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::ref_distance_setter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::ref_distance_setter");
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

    double cpp_value = TRY(value.to_double(vm));

    if (isinf(cpp_value) || isnan(cpp_value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "refDistance");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ref_distance(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::max_distance_getter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::max_distance_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->max_distance(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::max_distance_setter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::max_distance_setter");
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

    double cpp_value = TRY(value.to_double(vm));

    if (isinf(cpp_value) || isnan(cpp_value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "maxDistance");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_max_distance(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::rolloff_factor_getter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::rolloff_factor_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->rolloff_factor(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::rolloff_factor_setter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::rolloff_factor_setter");
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

    double cpp_value = TRY(value.to_double(vm));

    if (isinf(cpp_value) || isnan(cpp_value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "rolloffFactor");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_rolloff_factor(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::cone_inner_angle_getter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::cone_inner_angle_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cone_inner_angle(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::cone_inner_angle_setter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::cone_inner_angle_setter");
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

    double cpp_value = TRY(value.to_double(vm));

    if (isinf(cpp_value) || isnan(cpp_value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "coneInnerAngle");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_cone_inner_angle(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::cone_outer_angle_getter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::cone_outer_angle_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cone_outer_angle(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::cone_outer_angle_setter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::cone_outer_angle_setter");
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

    double cpp_value = TRY(value.to_double(vm));

    if (isinf(cpp_value) || isnan(cpp_value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "coneOuterAngle");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_cone_outer_angle(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::cone_outer_gain_getter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::cone_outer_gain_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cone_outer_gain(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::cone_outer_gain_setter)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::cone_outer_gain_setter");
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

    double cpp_value = TRY(value.to_double(vm));

    if (isinf(cpp_value) || isnan(cpp_value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "coneOuterGain");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_cone_outer_gain(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::set_position)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::set_position");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "setPosition", "3");

    auto arg0 = vm.argument(0);

    float x = TRY(arg0.to_double(vm));

    if (isinf(x) || isnan(x)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
    auto arg1 = vm.argument(1);

    float y = TRY(arg1.to_double(vm));

    if (isinf(y) || isnan(y)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
    auto arg2 = vm.argument(2);

    float z = TRY(arg2.to_double(vm));

    if (isinf(z) || isnan(z)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "z");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_position(x, y, z); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(PannerNodePrototype::set_orientation)
{
    WebIDL::log_trace(vm, "PannerNodePrototype::set_orientation");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "setOrientation", "3");

    auto arg0 = vm.argument(0);

    float x = TRY(arg0.to_double(vm));

    if (isinf(x) || isnan(x)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
    auto arg1 = vm.argument(1);

    float y = TRY(arg1.to_double(vm));

    if (isinf(y) || isnan(y)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
    auto arg2 = vm.argument(2);

    float z = TRY(arg2.to_double(vm));

    if (isinf(z) || isnan(z)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "z");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_orientation(x, y, z); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
