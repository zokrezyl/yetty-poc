
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
#include <LibWeb/Bindings/AudioParamPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioParam.h>

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

GC_DEFINE_ALLOCATOR(AudioParamPrototype);

AudioParamPrototype::AudioParamPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

AudioParamPrototype::~AudioParamPrototype()
{
}

void AudioParamPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_value_getter = JS::NativeFunction::create(realm, value_getter, 0, "value"_utf16_fly_string, &realm, "get"sv);

    auto native_value_setter = JS::NativeFunction::create(realm, value_setter, 1, "value"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("value"_utf16_fly_string, native_value_getter, native_value_setter, default_attributes);

    auto native_automation_rate_getter = JS::NativeFunction::create(realm, automation_rate_getter, 0, "automationRate"_utf16_fly_string, &realm, "get"sv);

    auto native_automation_rate_setter = JS::NativeFunction::create(realm, automation_rate_setter, 1, "automationRate"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("automationRate"_utf16_fly_string, native_automation_rate_getter, native_automation_rate_setter, default_attributes);

    auto native_default_value_getter = JS::NativeFunction::create(realm, default_value_getter, 0, "defaultValue"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_default_value_setter;

    define_direct_accessor("defaultValue"_utf16_fly_string, native_default_value_getter, native_default_value_setter, default_attributes);

    auto native_min_value_getter = JS::NativeFunction::create(realm, min_value_getter, 0, "minValue"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_min_value_setter;

    define_direct_accessor("minValue"_utf16_fly_string, native_min_value_getter, native_min_value_setter, default_attributes);

    auto native_max_value_getter = JS::NativeFunction::create(realm, max_value_getter, 0, "maxValue"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_max_value_setter;

    define_direct_accessor("maxValue"_utf16_fly_string, native_max_value_getter, native_max_value_setter, default_attributes);

    define_native_function(realm, "setValueAtTime"_utf16_fly_string, set_value_at_time, 2, default_attributes);

    define_native_function(realm, "linearRampToValueAtTime"_utf16_fly_string, linear_ramp_to_value_at_time, 2, default_attributes);

    define_native_function(realm, "exponentialRampToValueAtTime"_utf16_fly_string, exponential_ramp_to_value_at_time, 2, default_attributes);

    define_native_function(realm, "setTargetAtTime"_utf16_fly_string, set_target_at_time, 3, default_attributes);

    define_native_function(realm, "setValueCurveAtTime"_utf16_fly_string, set_value_curve_at_time, 3, default_attributes);

    define_native_function(realm, "cancelScheduledValues"_utf16_fly_string, cancel_scheduled_values, 1, default_attributes);

    define_native_function(realm, "cancelAndHoldAtTime"_utf16_fly_string, cancel_and_hold_at_time, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "AudioParam"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void AudioParamPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<WebAudio::AudioParam*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<WebAudio::AudioParam>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AudioParam");
    return static_cast<WebAudio::AudioParam*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(AudioParamPrototype::value_getter)
{
    WebIDL::log_trace(vm, "AudioParamPrototype::value_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->value(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioParamPrototype::value_setter)
{
    WebIDL::log_trace(vm, "AudioParamPrototype::value_setter");
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

    float cpp_value = TRY(value.to_double(vm));

    if (isinf(cpp_value) || isnan(cpp_value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_value(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AudioParamPrototype::automation_rate_getter)
{
    WebIDL::log_trace(vm, "AudioParamPrototype::automation_rate_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->automation_rate(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(AudioParamPrototype::automation_rate_setter)
{
    WebIDL::log_trace(vm, "AudioParamPrototype::automation_rate_setter");
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

    AutomationRate cpp_value { AutomationRate::ARate };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == "a-rate"sv)
        cpp_value = AutomationRate::ARate;

    else if (value_string == "k-rate"sv)
        cpp_value = AutomationRate::KRate;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_automation_rate(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AudioParamPrototype::default_value_getter)
{
    WebIDL::log_trace(vm, "AudioParamPrototype::default_value_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->default_value(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioParamPrototype::min_value_getter)
{
    WebIDL::log_trace(vm, "AudioParamPrototype::min_value_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->min_value(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioParamPrototype::max_value_getter)
{
    WebIDL::log_trace(vm, "AudioParamPrototype::max_value_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->max_value(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioParamPrototype::set_value_at_time)
{
    WebIDL::log_trace(vm, "AudioParamPrototype::set_value_at_time");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "setValueAtTime", "2");

    auto arg0 = vm.argument(0);

    float value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    auto arg1 = vm.argument(1);

    double start_time = TRY(arg1.to_double(vm));

    if (isinf(start_time) || isnan(start_time)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "startTime");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_value_at_time(value, start_time); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioParamPrototype::linear_ramp_to_value_at_time)
{
    WebIDL::log_trace(vm, "AudioParamPrototype::linear_ramp_to_value_at_time");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "linearRampToValueAtTime", "2");

    auto arg0 = vm.argument(0);

    float value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    auto arg1 = vm.argument(1);

    double end_time = TRY(arg1.to_double(vm));

    if (isinf(end_time) || isnan(end_time)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "endTime");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->linear_ramp_to_value_at_time(value, end_time); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioParamPrototype::exponential_ramp_to_value_at_time)
{
    WebIDL::log_trace(vm, "AudioParamPrototype::exponential_ramp_to_value_at_time");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "exponentialRampToValueAtTime", "2");

    auto arg0 = vm.argument(0);

    float value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    auto arg1 = vm.argument(1);

    double end_time = TRY(arg1.to_double(vm));

    if (isinf(end_time) || isnan(end_time)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "endTime");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->exponential_ramp_to_value_at_time(value, end_time); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioParamPrototype::set_target_at_time)
{
    WebIDL::log_trace(vm, "AudioParamPrototype::set_target_at_time");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "setTargetAtTime", "3");

    auto arg0 = vm.argument(0);

    float target = TRY(arg0.to_double(vm));

    if (isinf(target) || isnan(target)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "target");
    }
    
    auto arg1 = vm.argument(1);

    double start_time = TRY(arg1.to_double(vm));

    if (isinf(start_time) || isnan(start_time)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "startTime");
    }
    
    auto arg2 = vm.argument(2);

    float time_constant = TRY(arg2.to_double(vm));

    if (isinf(time_constant) || isnan(time_constant)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "timeConstant");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_target_at_time(target, start_time, time_constant); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioParamPrototype::set_value_curve_at_time)
{
    WebIDL::log_trace(vm, "AudioParamPrototype::set_value_curve_at_time");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "setValueCurveAtTime", "3");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg0);

    auto arg0_iterator_method0 = TRY(arg0.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg0_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg0);

    auto arg0_iterator1 = TRY(JS::get_iterator_from_method(vm, arg0, *arg0_iterator_method0));

    Vector<float> values;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg0_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    if (isinf(sequence_item1) || isnan(sequence_item1)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "arg0");
    }
    
    values.append(sequence_item1);
    }

    auto arg1 = vm.argument(1);

    double start_time = TRY(arg1.to_double(vm));

    if (isinf(start_time) || isnan(start_time)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "startTime");
    }
    
    auto arg2 = vm.argument(2);

    double duration = TRY(arg2.to_double(vm));

    if (isinf(duration) || isnan(duration)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "duration");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_value_curve_at_time(values, start_time, duration); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioParamPrototype::cancel_scheduled_values)
{
    WebIDL::log_trace(vm, "AudioParamPrototype::cancel_scheduled_values");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "cancelScheduledValues");

    auto arg0 = vm.argument(0);

    double cancel_time = TRY(arg0.to_double(vm));

    if (isinf(cancel_time) || isnan(cancel_time)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "cancelTime");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cancel_scheduled_values(cancel_time); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioParamPrototype::cancel_and_hold_at_time)
{
    WebIDL::log_trace(vm, "AudioParamPrototype::cancel_and_hold_at_time");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "cancelAndHoldAtTime");

    auto arg0 = vm.argument(0);

    double cancel_time = TRY(arg0.to_double(vm));

    if (isinf(cancel_time) || isnan(cancel_time)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "cancelTime");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cancel_and_hold_at_time(cancel_time); }));

    return &const_cast<AudioParam&>(*retval);

}

} // namespace Web::Bindings
