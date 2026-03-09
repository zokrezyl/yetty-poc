
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
#include <LibWeb/Bindings/PerformanceObserverPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/PerformanceTimeline/PerformanceObserver.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/PerformanceTimeline/PerformanceObserverEntryList.h>

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

GC_DEFINE_ALLOCATOR(PerformanceObserverPrototype);

PerformanceObserverPrototype::PerformanceObserverPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

PerformanceObserverPrototype::~PerformanceObserverPrototype()
{
}

void PerformanceObserverPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    define_native_function(realm, "observe"_utf16_fly_string, observe, 0, default_attributes);

    define_native_function(realm, "disconnect"_utf16_fly_string, disconnect, 0, default_attributes);

    define_native_function(realm, "takeRecords"_utf16_fly_string, take_records, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "PerformanceObserver"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void PerformanceObserverPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<PerformanceObserver*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<PerformanceObserver>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PerformanceObserver");
    return static_cast<PerformanceObserver*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(PerformanceObserverPrototype::observe)
{
    WebIDL::log_trace(vm, "PerformanceObserverPrototype::observe");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PerformanceObserverInit");

    PerformanceObserverInit options {};

    auto buffered_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        buffered_property_value_0 = TRY(arg0.as_object().get("buffered"_utf16_fly_string));

    if (!buffered_property_value_0.is_undefined()) {

    Optional<bool> buffered_value_0;

    if (!buffered_property_value_0.is_undefined())

    buffered_value_0 = buffered_property_value_0.to_boolean();

    options.buffered = buffered_value_0;

    }

    auto entry_types_property_value_1 = JS::js_undefined();
    if (arg0.is_object())
        entry_types_property_value_1 = TRY(arg0.as_object().get("entryTypes"_utf16_fly_string));

    if (!entry_types_property_value_1.is_undefined()) {

    Optional<Vector<String>> entry_types_value_1;

    if (!entry_types_property_value_1.is_undefined()) {

    if (!entry_types_property_value_1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, entry_types_property_value_1);

    auto entry_types_property_value_1_iterator_method0 = TRY(entry_types_property_value_1.get_method(vm, vm.well_known_symbol_iterator()));
    if (!entry_types_property_value_1_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, entry_types_property_value_1);

    auto entry_types_property_value_1_iterator1 = TRY(JS::get_iterator_from_method(vm, entry_types_property_value_1, *entry_types_property_value_1_iterator_method0));

    Vector<String> entry_types_value_1_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, entry_types_property_value_1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    String sequence_item1;
    if (!false || !next_item1.is_null()) {
        sequence_item1 = TRY(WebIDL::to_string(vm, next_item1));
    }

    entry_types_value_1_non_optional.append(sequence_item1);
    }

        entry_types_value_1 = move(entry_types_value_1_non_optional);
    }

    options.entry_types = entry_types_value_1;

    }

    auto type_property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        type_property_value_2 = TRY(arg0.as_object().get("type"_utf16_fly_string));

    if (!type_property_value_2.is_undefined()) {

    Optional<String> type_value_2;

    if (!type_property_value_2.is_undefined()) {
        if (!false || !type_property_value_2.is_null())
            type_value_2 = TRY(WebIDL::to_string(vm, type_property_value_2));
    }

    options.type = type_value_2;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->observe(options); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceObserverPrototype::disconnect)
{
    WebIDL::log_trace(vm, "PerformanceObserverPrototype::disconnect");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->disconnect(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceObserverPrototype::take_records)
{
    WebIDL::log_trace(vm, "PerformanceObserverPrototype::take_records");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->take_records(); }));

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
