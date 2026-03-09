
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
#include <LibWeb/Bindings/PerformancePrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HighResolutionTime/Performance.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/NavigationTiming/PerformanceNavigation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/NavigationTiming/PerformanceTiming.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/PerformanceTimeline/PerformanceEntry.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/UserTiming/PerformanceMark.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/UserTiming/PerformanceMeasure.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

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

GC_DEFINE_ALLOCATOR(PerformancePrototype);

PerformancePrototype::PerformancePrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

PerformancePrototype::~PerformancePrototype()
{
}

void PerformancePrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    auto native_time_origin_getter = JS::NativeFunction::create(realm, time_origin_getter, 0, "timeOrigin"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_time_origin_setter;

    define_direct_accessor("timeOrigin"_utf16_fly_string, native_time_origin_getter, native_time_origin_setter, default_attributes);

    auto native_onresourcetimingbufferfull_getter = JS::NativeFunction::create(realm, onresourcetimingbufferfull_getter, 0, "onresourcetimingbufferfull"_utf16_fly_string, &realm, "get"sv);

    auto native_onresourcetimingbufferfull_setter = JS::NativeFunction::create(realm, onresourcetimingbufferfull_setter, 1, "onresourcetimingbufferfull"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onresourcetimingbufferfull"_utf16_fly_string, native_onresourcetimingbufferfull_getter, native_onresourcetimingbufferfull_setter, default_attributes);

    define_native_function(realm, "now"_utf16_fly_string, now, 0, default_attributes);

    define_native_function(realm, "toJSON"_utf16_fly_string, to_json, 0, default_attributes);

    define_native_function(realm, "mark"_utf16_fly_string, mark, 1, default_attributes);

    define_native_function(realm, "clearMarks"_utf16_fly_string, clear_marks, 0, default_attributes);

    define_native_function(realm, "measure"_utf16_fly_string, measure, 1, default_attributes);

    define_native_function(realm, "clearMeasures"_utf16_fly_string, clear_measures, 0, default_attributes);

    define_native_function(realm, "clearResourceTimings"_utf16_fly_string, clear_resource_timings, 0, default_attributes);

    define_native_function(realm, "setResourceTimingBufferSize"_utf16_fly_string, set_resource_timing_buffer_size, 1, default_attributes);

    define_native_function(realm, "getEntries"_utf16_fly_string, get_entries, 0, default_attributes);

    define_native_function(realm, "getEntriesByType"_utf16_fly_string, get_entries_by_type, 1, default_attributes);

    define_native_function(realm, "getEntriesByName"_utf16_fly_string, get_entries_by_name, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Performance"_string), JS::Attribute::Configurable);

    if (is<HTML::Window>(realm.global_object())) {

    auto native_timing_getter = JS::NativeFunction::create(realm, timing_getter, 0, "timing"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_timing_setter;

    define_direct_accessor("timing"_utf16_fly_string, native_timing_getter, native_timing_setter, default_attributes);

    auto native_navigation_getter = JS::NativeFunction::create(realm, navigation_getter, 0, "navigation"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_navigation_setter;

    define_direct_accessor("navigation"_utf16_fly_string, native_navigation_getter, native_navigation_setter, default_attributes);

    }

    Base::initialize(realm);

}

void PerformancePrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HighResolutionTime::Performance*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HighResolutionTime::Performance>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Performance");
    return static_cast<HighResolutionTime::Performance*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(PerformancePrototype::time_origin_getter)
{
    WebIDL::log_trace(vm, "PerformancePrototype::time_origin_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->time_origin(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformancePrototype::onresourcetimingbufferfull_getter)
{
    WebIDL::log_trace(vm, "PerformancePrototype::onresourcetimingbufferfull_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onresourcetimingbufferfull(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(PerformancePrototype::onresourcetimingbufferfull_setter)
{
    WebIDL::log_trace(vm, "PerformancePrototype::onresourcetimingbufferfull_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onresourcetimingbufferfull(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(PerformancePrototype::timing_getter)
{
    WebIDL::log_trace(vm, "PerformancePrototype::timing_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->timing(); }));

    return &const_cast<PerformanceTiming&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformancePrototype::navigation_getter)
{
    WebIDL::log_trace(vm, "PerformancePrototype::navigation_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->navigation(); }));

    return &const_cast<PerformanceNavigation&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformancePrototype::now)
{
    WebIDL::log_trace(vm, "PerformancePrototype::now");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->now(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformancePrototype::to_json)
{
    WebIDL::log_trace(vm, "PerformancePrototype::to_json");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

    auto time_origin_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->time_origin(); }));

    JS::Value time_origin_retval_wrapped;

    time_origin_retval_wrapped = JS::Value(time_origin_retval);

    MUST(result->create_data_property("timeOrigin"_utf16_fly_string, time_origin_retval_wrapped));

    if (is<HTML::Window>(realm.global_object())) {

    auto timing_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->timing(); }));

    JS::Value timing_retval_wrapped;

    timing_retval_wrapped = &const_cast<PerformanceTiming&>(*timing_retval);

    MUST(result->create_data_property("timing"_utf16_fly_string, timing_retval_wrapped));

    auto navigation_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->navigation(); }));

    JS::Value navigation_retval_wrapped;

    navigation_retval_wrapped = &const_cast<PerformanceNavigation&>(*navigation_retval);

    MUST(result->create_data_property("navigation"_utf16_fly_string, navigation_retval_wrapped));

    }

    return result;
}

JS_DEFINE_NATIVE_FUNCTION(PerformancePrototype::mark)
{
    WebIDL::log_trace(vm, "PerformancePrototype::mark");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "mark");

    auto arg0 = vm.argument(0);

    String mark_name;
    if (!false || !arg0.is_null()) {
        mark_name = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PerformanceMarkOptions");

    PerformanceMarkOptions mark_options {};

    auto detail_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        detail_property_value_0 = TRY(arg1.as_object().get("detail"_utf16_fly_string));

    if (!detail_property_value_0.is_undefined()) {

    JS::Value detail_value_0 = JS::js_undefined();
    if (!detail_property_value_0.is_undefined())
        detail_value_0 = detail_property_value_0;

    mark_options.detail = detail_value_0;

    }

    auto start_time_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        start_time_property_value_1 = TRY(arg1.as_object().get("startTime"_utf16_fly_string));

    if (!start_time_property_value_1.is_undefined()) {

    Optional<double> start_time_value_1;

    if (!start_time_property_value_1.is_undefined())
        start_time_value_1 = TRY(start_time_property_value_1.to_double(vm));


    if (start_time_value_1.has_value() && (isinf(*start_time_value_1) || isnan(*start_time_value_1))) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "startTime");
    }
    
    mark_options.start_time = start_time_value_1;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->mark(mark_name, mark_options); }));

    return &const_cast<PerformanceMark&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformancePrototype::clear_marks)
{
    WebIDL::log_trace(vm, "PerformancePrototype::clear_marks");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    Optional<String> mark_name;

    if (!arg0.is_undefined()) {
        if (!false || !arg0.is_null())
            mark_name = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear_marks(mark_name); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(PerformancePrototype::measure)
{
    WebIDL::log_trace(vm, "PerformancePrototype::measure");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "measure");

    auto arg0 = vm.argument(0);

    String measure_name;
    if (!false || !arg0.is_null()) {
        measure_name = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_dictionary = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<PerformanceMeasureOptions> {
        // This might be unused.
        (void)realm;

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PerformanceMeasureOptions");

    PerformanceMeasureOptions dictionary_union_type {};

    auto detail_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        detail_property_value_0 = TRY(arg1.as_object().get("detail"_utf16_fly_string));

    if (!detail_property_value_0.is_undefined()) {

    JS::Value detail_value_0 = JS::js_undefined();
    if (!detail_property_value_0.is_undefined())
        detail_value_0 = detail_property_value_0;

    dictionary_union_type.detail = detail_value_0;

    }

    auto duration_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        duration_property_value_1 = TRY(arg1.as_object().get("duration"_utf16_fly_string));

    if (!duration_property_value_1.is_undefined()) {

    Optional<double> duration_value_1;

    if (!duration_property_value_1.is_undefined())
        duration_value_1 = TRY(duration_property_value_1.to_double(vm));


    if (duration_value_1.has_value() && (isinf(*duration_value_1) || isnan(*duration_value_1))) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "duration");
    }
    
    dictionary_union_type.duration = duration_value_1;

    }

    auto end_property_value_2 = JS::js_undefined();
    if (arg1.is_object())
        end_property_value_2 = TRY(arg1.as_object().get("end"_utf16_fly_string));

    if (!end_property_value_2.is_undefined()) {

    auto end_property_value_2_to_variant = [&vm, &realm](JS::Value end_property_value_2) -> JS::ThrowCompletionOr<Variant<String, double>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (end_property_value_2.is_object()) {
            [[maybe_unused]] auto& end_property_value_2_object = end_property_value_2.as_object();

        }

        if (end_property_value_2.is_number()) {

    double end_property_value_2_number = TRY(end_property_value_2.to_double(vm));

    if (isinf(end_property_value_2_number) || isnan(end_property_value_2_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "end");
    }
    
            return { end_property_value_2_number };
        }

    String end_property_value_2_string;
    if (!false || !end_property_value_2.is_null()) {
        end_property_value_2_string = TRY(WebIDL::to_string(vm, end_property_value_2));
    }

        return { end_property_value_2_string };

    };

    Optional<Variant<String, double>> end_value_2;
    if (!end_property_value_2.is_undefined())
        end_value_2 = TRY(end_property_value_2_to_variant(end_property_value_2));

    dictionary_union_type.end = end_value_2;

    }

    auto start_property_value_3 = JS::js_undefined();
    if (arg1.is_object())
        start_property_value_3 = TRY(arg1.as_object().get("start"_utf16_fly_string));

    if (!start_property_value_3.is_undefined()) {

    auto start_property_value_3_to_variant = [&vm, &realm](JS::Value start_property_value_3) -> JS::ThrowCompletionOr<Variant<String, double>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (start_property_value_3.is_object()) {
            [[maybe_unused]] auto& start_property_value_3_object = start_property_value_3.as_object();

        }

        if (start_property_value_3.is_number()) {

    double start_property_value_3_number = TRY(start_property_value_3.to_double(vm));

    if (isinf(start_property_value_3_number) || isnan(start_property_value_3_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "start");
    }
    
            return { start_property_value_3_number };
        }

    String start_property_value_3_string;
    if (!false || !start_property_value_3.is_null()) {
        start_property_value_3_string = TRY(WebIDL::to_string(vm, start_property_value_3));
    }

        return { start_property_value_3_string };

    };

    Optional<Variant<String, double>> start_value_3;
    if (!start_property_value_3.is_undefined())
        start_value_3 = TRY(start_property_value_3_to_variant(start_property_value_3));

    dictionary_union_type.start = start_value_3;

    }

        return dictionary_union_type;
    };

    auto arg1_to_variant = [&vm, &realm, &arg1_to_dictionary](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<String, PerformanceMeasureOptions>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_nullish())
            return Variant<String, PerformanceMeasureOptions> { TRY(arg1_to_dictionary(arg1)) };

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

        return Variant<String, PerformanceMeasureOptions> { TRY(arg1_to_dictionary(arg1)) };

        }

    String arg1_string;
    if (!false || !arg1.is_null()) {
        arg1_string = TRY(WebIDL::to_string(vm, arg1));
    }

        return { arg1_string };

    };

    Variant<String, PerformanceMeasureOptions> start_or_measure_options = arg1.is_undefined() ? TRY(arg1_to_dictionary(arg1)) : TRY(arg1_to_variant(arg1));

    auto arg2 = vm.argument(2);

    Optional<String> end_mark;

    if (!arg2.is_undefined()) {
        if (!false || !arg2.is_null())
            end_mark = TRY(WebIDL::to_string(vm, arg2));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->measure(measure_name, start_or_measure_options, end_mark); }));

    return &const_cast<PerformanceMeasure&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformancePrototype::clear_measures)
{
    WebIDL::log_trace(vm, "PerformancePrototype::clear_measures");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    Optional<String> measure_name;

    if (!arg0.is_undefined()) {
        if (!false || !arg0.is_null())
            measure_name = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear_measures(measure_name); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(PerformancePrototype::clear_resource_timings)
{
    WebIDL::log_trace(vm, "PerformancePrototype::clear_resource_timings");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear_resource_timings(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(PerformancePrototype::set_resource_timing_buffer_size)
{
    WebIDL::log_trace(vm, "PerformancePrototype::set_resource_timing_buffer_size");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setResourceTimingBufferSize");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong max_size;

    max_size = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_resource_timing_buffer_size(max_size); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(PerformancePrototype::get_entries)
{
    WebIDL::log_trace(vm, "PerformancePrototype::get_entries");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_entries(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(PerformancePrototype::get_entries_by_type)
{
    WebIDL::log_trace(vm, "PerformancePrototype::get_entries_by_type");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getEntriesByType");

    auto arg0 = vm.argument(0);

    String type;
    if (!false || !arg0.is_null()) {
        type = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_entries_by_type(type); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(PerformancePrototype::get_entries_by_name)
{
    WebIDL::log_trace(vm, "PerformancePrototype::get_entries_by_name");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getEntriesByName");

    auto arg0 = vm.argument(0);

    String name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    Optional<String> type;

    if (!arg1.is_undefined()) {
        if (!false || !arg1.is_null())
            type = TRY(WebIDL::to_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_entries_by_name(name, type); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

} // namespace Web::Bindings
