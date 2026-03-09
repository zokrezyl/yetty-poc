
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
#include <LibWeb/Bindings/AnimationEffectPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/AnimationEffect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSNumericValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSUnitValue.h>

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

GC_DEFINE_ALLOCATOR(AnimationEffectPrototype);

AnimationEffectPrototype::AnimationEffectPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

AnimationEffectPrototype::~AnimationEffectPrototype()
{
}

void AnimationEffectPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    define_native_function(realm, "getTiming"_utf16_fly_string, get_timing, 0, default_attributes);

    define_native_function(realm, "getComputedTiming"_utf16_fly_string, get_computed_timing, 0, default_attributes);

    define_native_function(realm, "updateTiming"_utf16_fly_string, update_timing, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "AnimationEffect"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void AnimationEffectPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<AnimationEffect*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<AnimationEffect>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AnimationEffect");
    return static_cast<AnimationEffect*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(AnimationEffectPrototype::get_timing)
{
    WebIDL::log_trace(vm, "AnimationEffectPrototype::get_timing");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_timing(); }));

    {
        auto dictionary_object0 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_delay0_value;

    wrapped_delay0_value = JS::Value(retval.delay);

        MUST(dictionary_object0->create_data_property("delay"_utf16_fly_string, wrapped_delay0_value));

        JS::Value wrapped_direction0_value;

    wrapped_direction0_value = JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval.direction));

        MUST(dictionary_object0->create_data_property("direction"_utf16_fly_string, wrapped_direction0_value));

        JS::Value wrapped_duration0_value;

    wrapped_duration0_value = retval.duration.visit(

        [&vm, &realm]([[maybe_unused]] double const& visited_union_value1) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::Value(visited_union_value1);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<CSSNumericValue> const& visited_union_value1) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<CSSNumericValue&>(*visited_union_value1);

        },

        [&vm, &realm]([[maybe_unused]] String const& visited_union_value1) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::PrimitiveString::create(vm, visited_union_value1);

        }

    );

        MUST(dictionary_object0->create_data_property("duration"_utf16_fly_string, wrapped_duration0_value));

        JS::Value wrapped_easing0_value;

    wrapped_easing0_value = JS::PrimitiveString::create(vm, retval.easing);

        MUST(dictionary_object0->create_data_property("easing"_utf16_fly_string, wrapped_easing0_value));

        JS::Value wrapped_end_delay0_value;

    wrapped_end_delay0_value = JS::Value(retval.end_delay);

        MUST(dictionary_object0->create_data_property("endDelay"_utf16_fly_string, wrapped_end_delay0_value));

        JS::Value wrapped_fill0_value;

    wrapped_fill0_value = JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval.fill));

        MUST(dictionary_object0->create_data_property("fill"_utf16_fly_string, wrapped_fill0_value));

        JS::Value wrapped_iteration_start0_value;

    wrapped_iteration_start0_value = JS::Value(retval.iteration_start);

        MUST(dictionary_object0->create_data_property("iterationStart"_utf16_fly_string, wrapped_iteration_start0_value));

        JS::Value wrapped_iterations0_value;

    wrapped_iterations0_value = JS::Value(retval.iterations);

        MUST(dictionary_object0->create_data_property("iterations"_utf16_fly_string, wrapped_iterations0_value));

        return dictionary_object0;
    }

}

JS_DEFINE_NATIVE_FUNCTION(AnimationEffectPrototype::get_computed_timing)
{
    WebIDL::log_trace(vm, "AnimationEffectPrototype::get_computed_timing");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_computed_timing(); }));

    {
        auto dictionary_object0 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_active_duration0_value;

    wrapped_active_duration0_value = retval.active_duration.visit(

        [&vm, &realm]([[maybe_unused]] double const& visited_union_value1) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::Value(visited_union_value1);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<CSSNumericValue> const& visited_union_value1) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<CSSNumericValue&>(*visited_union_value1);

        },

        [](Empty) -> JS::Value {
            return JS::js_null();
        }

    );

        MUST(dictionary_object0->create_data_property("activeDuration"_utf16_fly_string, wrapped_active_duration0_value));

        Optional<JS::Value> wrapped_current_iteration0_value;

    if (retval.current_iteration.has_value()) {

    wrapped_current_iteration0_value = JS::Value(retval.current_iteration.release_value());

    } else {
        wrapped_current_iteration0_value = JS::js_null();
    }

        if (wrapped_current_iteration0_value.has_value())
            MUST(dictionary_object0->create_data_property("currentIteration"_utf16_fly_string, wrapped_current_iteration0_value.release_value()));

        JS::Value wrapped_end_time0_value;

    wrapped_end_time0_value = retval.end_time.visit(

        [&vm, &realm]([[maybe_unused]] double const& visited_union_value1) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::Value(visited_union_value1);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<CSSNumericValue> const& visited_union_value1) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<CSSNumericValue&>(*visited_union_value1);

        },

        [](Empty) -> JS::Value {
            return JS::js_null();
        }

    );

        MUST(dictionary_object0->create_data_property("endTime"_utf16_fly_string, wrapped_end_time0_value));

        Optional<JS::Value> wrapped_local_time0_value;

    wrapped_local_time0_value = retval.local_time.visit(

        [&vm, &realm]([[maybe_unused]] double const& visited_union_value1) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::Value(visited_union_value1);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<CSSNumericValue> const& visited_union_value1) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<CSSNumericValue&>(*visited_union_value1);

        },

        [](Empty) -> JS::Value {
            return JS::js_null();
        }

    );

        if (wrapped_local_time0_value.has_value())
            MUST(dictionary_object0->create_data_property("localTime"_utf16_fly_string, wrapped_local_time0_value.release_value()));

        Optional<JS::Value> wrapped_progress0_value;

    if (retval.progress.has_value()) {

    wrapped_progress0_value = JS::Value(retval.progress.release_value());

    } else {
        wrapped_progress0_value = JS::js_null();
    }

        if (wrapped_progress0_value.has_value())
            MUST(dictionary_object0->create_data_property("progress"_utf16_fly_string, wrapped_progress0_value.release_value()));

        JS::Value wrapped_delay0_value;

    wrapped_delay0_value = JS::Value(retval.delay);

        MUST(dictionary_object0->create_data_property("delay"_utf16_fly_string, wrapped_delay0_value));

        JS::Value wrapped_direction0_value;

    wrapped_direction0_value = JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval.direction));

        MUST(dictionary_object0->create_data_property("direction"_utf16_fly_string, wrapped_direction0_value));

        JS::Value wrapped_duration0_value;

    wrapped_duration0_value = retval.duration.visit(

        [&vm, &realm]([[maybe_unused]] double const& visited_union_value1) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::Value(visited_union_value1);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<CSSNumericValue> const& visited_union_value1) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<CSSNumericValue&>(*visited_union_value1);

        },

        [&vm, &realm]([[maybe_unused]] String const& visited_union_value1) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::PrimitiveString::create(vm, visited_union_value1);

        }

    );

        MUST(dictionary_object0->create_data_property("duration"_utf16_fly_string, wrapped_duration0_value));

        JS::Value wrapped_easing0_value;

    wrapped_easing0_value = JS::PrimitiveString::create(vm, retval.easing);

        MUST(dictionary_object0->create_data_property("easing"_utf16_fly_string, wrapped_easing0_value));

        JS::Value wrapped_end_delay0_value;

    wrapped_end_delay0_value = JS::Value(retval.end_delay);

        MUST(dictionary_object0->create_data_property("endDelay"_utf16_fly_string, wrapped_end_delay0_value));

        JS::Value wrapped_fill0_value;

    wrapped_fill0_value = JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval.fill));

        MUST(dictionary_object0->create_data_property("fill"_utf16_fly_string, wrapped_fill0_value));

        JS::Value wrapped_iteration_start0_value;

    wrapped_iteration_start0_value = JS::Value(retval.iteration_start);

        MUST(dictionary_object0->create_data_property("iterationStart"_utf16_fly_string, wrapped_iteration_start0_value));

        JS::Value wrapped_iterations0_value;

    wrapped_iterations0_value = JS::Value(retval.iterations);

        MUST(dictionary_object0->create_data_property("iterations"_utf16_fly_string, wrapped_iterations0_value));

        return dictionary_object0;
    }

}

JS_DEFINE_NATIVE_FUNCTION(AnimationEffectPrototype::update_timing)
{
    WebIDL::log_trace(vm, "AnimationEffectPrototype::update_timing");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "OptionalEffectTiming");

    OptionalEffectTiming timing {};

    auto delay_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        delay_property_value_0 = TRY(arg0.as_object().get("delay"_utf16_fly_string));

    if (!delay_property_value_0.is_undefined()) {

    Optional<double> delay_value_0;

    if (!delay_property_value_0.is_undefined())
        delay_value_0 = TRY(delay_property_value_0.to_double(vm));


    if (delay_value_0.has_value() && (isinf(*delay_value_0) || isnan(*delay_value_0))) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "delay");
    }
    
    timing.delay = delay_value_0;

    }

    auto direction_property_value_1 = JS::js_undefined();
    if (arg0.is_object())
        direction_property_value_1 = TRY(arg0.as_object().get("direction"_utf16_fly_string));

    if (!direction_property_value_1.is_undefined()) {

    PlaybackDirection direction_value_1 { PlaybackDirection::Normal };

    if (!direction_property_value_1.is_undefined()) {

    auto direction_property_value_1_string = TRY(direction_property_value_1.to_string(vm));

    if (direction_property_value_1_string == "normal"sv)
        direction_value_1 = PlaybackDirection::Normal;

    else if (direction_property_value_1_string == "reverse"sv)
        direction_value_1 = PlaybackDirection::Reverse;

    else if (direction_property_value_1_string == "alternate"sv)
        direction_value_1 = PlaybackDirection::Alternate;

    else if (direction_property_value_1_string == "alternate-reverse"sv)
        direction_value_1 = PlaybackDirection::AlternateReverse;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, direction_property_value_1_string, "PlaybackDirection");

    }

    timing.direction = direction_value_1;

    }

    auto duration_property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        duration_property_value_2 = TRY(arg0.as_object().get("duration"_utf16_fly_string));

    if (!duration_property_value_2.is_undefined()) {

    auto duration_property_value_2_to_variant = [&vm, &realm](JS::Value duration_property_value_2) -> JS::ThrowCompletionOr<Variant<double, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (duration_property_value_2.is_object()) {
            [[maybe_unused]] auto& duration_property_value_2_object = duration_property_value_2.as_object();

        }

        if (duration_property_value_2.is_number()) {

    double duration_property_value_2_number = TRY(duration_property_value_2.to_double(vm));

            return { duration_property_value_2_number };
        }

    String duration_property_value_2_string;
    if (!false || !duration_property_value_2.is_null()) {
        duration_property_value_2_string = TRY(WebIDL::to_string(vm, duration_property_value_2));
    }

        return { duration_property_value_2_string };

    };

    Optional<Variant<double, String>> duration_value_2;
    if (!duration_property_value_2.is_undefined())
        duration_value_2 = TRY(duration_property_value_2_to_variant(duration_property_value_2));

    timing.duration = duration_value_2;

    }

    auto easing_property_value_3 = JS::js_undefined();
    if (arg0.is_object())
        easing_property_value_3 = TRY(arg0.as_object().get("easing"_utf16_fly_string));

    if (!easing_property_value_3.is_undefined()) {

    Optional<String> easing_value_3;

    if (!easing_property_value_3.is_undefined()) {
        if (!false || !easing_property_value_3.is_null())
            easing_value_3 = TRY(WebIDL::to_string(vm, easing_property_value_3));
    }

    timing.easing = easing_value_3;

    }

    auto end_delay_property_value_4 = JS::js_undefined();
    if (arg0.is_object())
        end_delay_property_value_4 = TRY(arg0.as_object().get("endDelay"_utf16_fly_string));

    if (!end_delay_property_value_4.is_undefined()) {

    Optional<double> end_delay_value_4;

    if (!end_delay_property_value_4.is_undefined())
        end_delay_value_4 = TRY(end_delay_property_value_4.to_double(vm));


    if (end_delay_value_4.has_value() && (isinf(*end_delay_value_4) || isnan(*end_delay_value_4))) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "endDelay");
    }
    
    timing.end_delay = end_delay_value_4;

    }

    auto fill_property_value_5 = JS::js_undefined();
    if (arg0.is_object())
        fill_property_value_5 = TRY(arg0.as_object().get("fill"_utf16_fly_string));

    if (!fill_property_value_5.is_undefined()) {

    FillMode fill_value_5 { FillMode::None };

    if (!fill_property_value_5.is_undefined()) {

    auto fill_property_value_5_string = TRY(fill_property_value_5.to_string(vm));

    if (fill_property_value_5_string == "none"sv)
        fill_value_5 = FillMode::None;

    else if (fill_property_value_5_string == "forwards"sv)
        fill_value_5 = FillMode::Forwards;

    else if (fill_property_value_5_string == "backwards"sv)
        fill_value_5 = FillMode::Backwards;

    else if (fill_property_value_5_string == "both"sv)
        fill_value_5 = FillMode::Both;

    else if (fill_property_value_5_string == "auto"sv)
        fill_value_5 = FillMode::Auto;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, fill_property_value_5_string, "FillMode");

    }

    timing.fill = fill_value_5;

    }

    auto iteration_start_property_value_6 = JS::js_undefined();
    if (arg0.is_object())
        iteration_start_property_value_6 = TRY(arg0.as_object().get("iterationStart"_utf16_fly_string));

    if (!iteration_start_property_value_6.is_undefined()) {

    Optional<double> iteration_start_value_6;

    if (!iteration_start_property_value_6.is_undefined())
        iteration_start_value_6 = TRY(iteration_start_property_value_6.to_double(vm));


    if (iteration_start_value_6.has_value() && (isinf(*iteration_start_value_6) || isnan(*iteration_start_value_6))) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "iterationStart");
    }
    
    timing.iteration_start = iteration_start_value_6;

    }

    auto iterations_property_value_7 = JS::js_undefined();
    if (arg0.is_object())
        iterations_property_value_7 = TRY(arg0.as_object().get("iterations"_utf16_fly_string));

    if (!iterations_property_value_7.is_undefined()) {

    Optional<double> iterations_value_7;

    if (!iterations_property_value_7.is_undefined())
        iterations_value_7 = TRY(iterations_property_value_7.to_double(vm));


    timing.iterations = iterations_value_7;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->update_timing(timing); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
