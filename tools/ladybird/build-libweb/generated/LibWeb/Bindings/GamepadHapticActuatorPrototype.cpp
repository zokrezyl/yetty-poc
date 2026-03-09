
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
#include <LibWeb/Bindings/GamepadHapticActuatorPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Gamepad/GamepadHapticActuator.h>

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

GC_DEFINE_ALLOCATOR(GamepadHapticActuatorPrototype);

GamepadHapticActuatorPrototype::GamepadHapticActuatorPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

GamepadHapticActuatorPrototype::~GamepadHapticActuatorPrototype()
{
}

void GamepadHapticActuatorPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_effects_getter = JS::NativeFunction::create(realm, effects_getter, 0, "effects"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_effects_setter;

    define_direct_accessor("effects"_utf16_fly_string, native_effects_getter, native_effects_setter, default_attributes);

    define_native_function(realm, "playEffect"_utf16_fly_string, play_effect, 1, default_attributes);

    define_native_function(realm, "reset"_utf16_fly_string, reset, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "GamepadHapticActuator"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void GamepadHapticActuatorPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Gamepad::GamepadHapticActuator*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Gamepad::GamepadHapticActuator>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "GamepadHapticActuator");
    return static_cast<Gamepad::GamepadHapticActuator*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(GamepadHapticActuatorPrototype::effects_getter)
{
    WebIDL::log_trace(vm, "GamepadHapticActuatorPrototype::effects_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->effects(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);
JS::Value wrapped_element0;

    wrapped_element0 = JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(element0));

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    TRY(new_array0_0->set_integrity_level(IntegrityLevel::Frozen));

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(GamepadHapticActuatorPrototype::play_effect)
{
    WebIDL::log_trace(vm, "GamepadHapticActuatorPrototype::play_effect");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "playEffect");

    auto arg0 = vm.argument(0);

    GamepadHapticEffectType type { GamepadHapticEffectType::DualRumble };

    auto arg0_string = TRY(arg0.to_string(vm));

    if (arg0_string == "dual-rumble"sv)
        type = GamepadHapticEffectType::DualRumble;

    else if (arg0_string == "trigger-rumble"sv)
        type = GamepadHapticEffectType::TriggerRumble;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, arg0_string, "GamepadHapticEffectType");

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "GamepadEffectParameters");

    GamepadEffectParameters params {};

    auto duration_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        duration_property_value_0 = TRY(arg1.as_object().get("duration"_utf16_fly_string));

    WebIDL::UnsignedLongLong duration_value_0;

    if (!duration_property_value_0.is_undefined())

    duration_value_0 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, duration_property_value_0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        duration_value_0 = static_cast<WebIDL::UnsignedLongLong>(0);

    params.duration = duration_value_0;

    auto left_trigger_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        left_trigger_property_value_1 = TRY(arg1.as_object().get("leftTrigger"_utf16_fly_string));

    double left_trigger_value_1;

    if (!left_trigger_property_value_1.is_undefined())
        left_trigger_value_1 = TRY(left_trigger_property_value_1.to_double(vm));

    else
        left_trigger_value_1 = 0.0;

    if (isinf(left_trigger_value_1) || isnan(left_trigger_value_1)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "leftTrigger");
    }
    
    params.left_trigger = left_trigger_value_1;

    auto right_trigger_property_value_2 = JS::js_undefined();
    if (arg1.is_object())
        right_trigger_property_value_2 = TRY(arg1.as_object().get("rightTrigger"_utf16_fly_string));

    double right_trigger_value_2;

    if (!right_trigger_property_value_2.is_undefined())
        right_trigger_value_2 = TRY(right_trigger_property_value_2.to_double(vm));

    else
        right_trigger_value_2 = 0.0;

    if (isinf(right_trigger_value_2) || isnan(right_trigger_value_2)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "rightTrigger");
    }
    
    params.right_trigger = right_trigger_value_2;

    auto start_delay_property_value_3 = JS::js_undefined();
    if (arg1.is_object())
        start_delay_property_value_3 = TRY(arg1.as_object().get("startDelay"_utf16_fly_string));

    WebIDL::UnsignedLongLong start_delay_value_3;

    if (!start_delay_property_value_3.is_undefined())

    start_delay_value_3 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, start_delay_property_value_3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        start_delay_value_3 = static_cast<WebIDL::UnsignedLongLong>(0);

    params.start_delay = start_delay_value_3;

    auto strong_magnitude_property_value_4 = JS::js_undefined();
    if (arg1.is_object())
        strong_magnitude_property_value_4 = TRY(arg1.as_object().get("strongMagnitude"_utf16_fly_string));

    double strong_magnitude_value_4;

    if (!strong_magnitude_property_value_4.is_undefined())
        strong_magnitude_value_4 = TRY(strong_magnitude_property_value_4.to_double(vm));

    else
        strong_magnitude_value_4 = 0.0;

    if (isinf(strong_magnitude_value_4) || isnan(strong_magnitude_value_4)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "strongMagnitude");
    }
    
    params.strong_magnitude = strong_magnitude_value_4;

    auto weak_magnitude_property_value_5 = JS::js_undefined();
    if (arg1.is_object())
        weak_magnitude_property_value_5 = TRY(arg1.as_object().get("weakMagnitude"_utf16_fly_string));

    double weak_magnitude_value_5;

    if (!weak_magnitude_property_value_5.is_undefined())
        weak_magnitude_value_5 = TRY(weak_magnitude_property_value_5.to_double(vm));

    else
        weak_magnitude_value_5 = 0.0;

    if (isinf(weak_magnitude_value_5) || isnan(weak_magnitude_value_5)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "weakMagnitude");
    }
    
    params.weak_magnitude = weak_magnitude_value_5;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->play_effect(type, params); }));

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

JS_DEFINE_NATIVE_FUNCTION(GamepadHapticActuatorPrototype::reset)
{
    WebIDL::log_trace(vm, "GamepadHapticActuatorPrototype::reset");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->reset(); }));

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
