
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
#include <LibWeb/Bindings/PerformanceEventTimingPrototype.h>
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

#if __has_include(<LibWeb/Bindings/PerformanceEntryPrototype.h>)
#    include <LibWeb/Bindings/PerformanceEntryPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/EventTiming/PerformanceEventTiming.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/PerformanceTimeline/PerformanceEntry.h>

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

GC_DEFINE_ALLOCATOR(PerformanceEventTimingPrototype);

PerformanceEventTimingPrototype::PerformanceEventTimingPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

PerformanceEventTimingPrototype::~PerformanceEventTimingPrototype()
{
}

void PerformanceEventTimingPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<PerformanceEntryPrototype>(realm, "PerformanceEntry"_fly_string));


    auto native_processing_start_getter = JS::NativeFunction::create(realm, processing_start_getter, 0, "processingStart"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_processing_start_setter;

    define_direct_accessor("processingStart"_utf16_fly_string, native_processing_start_getter, native_processing_start_setter, default_attributes);

    auto native_processing_end_getter = JS::NativeFunction::create(realm, processing_end_getter, 0, "processingEnd"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_processing_end_setter;

    define_direct_accessor("processingEnd"_utf16_fly_string, native_processing_end_getter, native_processing_end_setter, default_attributes);

    auto native_cancelable_getter = JS::NativeFunction::create(realm, cancelable_getter, 0, "cancelable"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_cancelable_setter;

    define_direct_accessor("cancelable"_utf16_fly_string, native_cancelable_getter, native_cancelable_setter, default_attributes);

    auto native_target_getter = JS::NativeFunction::create(realm, target_getter, 0, "target"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_target_setter;

    define_direct_accessor("target"_utf16_fly_string, native_target_getter, native_target_setter, default_attributes);

    auto native_interaction_id_getter = JS::NativeFunction::create(realm, interaction_id_getter, 0, "interactionId"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_interaction_id_setter;

    define_direct_accessor("interactionId"_utf16_fly_string, native_interaction_id_getter, native_interaction_id_setter, default_attributes);

    define_native_function(realm, "toJSON"_utf16_fly_string, to_json, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "PerformanceEventTiming"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void PerformanceEventTimingPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<PerformanceEventTiming*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<PerformanceEventTiming>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PerformanceEventTiming");
    return static_cast<PerformanceEventTiming*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(PerformanceEventTimingPrototype::processing_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceEventTimingPrototype::processing_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->processing_start(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceEventTimingPrototype::processing_end_getter)
{
    WebIDL::log_trace(vm, "PerformanceEventTimingPrototype::processing_end_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->processing_end(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceEventTimingPrototype::cancelable_getter)
{
    WebIDL::log_trace(vm, "PerformanceEventTimingPrototype::cancelable_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cancelable(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceEventTimingPrototype::target_getter)
{
    WebIDL::log_trace(vm, "PerformanceEventTimingPrototype::target_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->target(); }));

    if (retval) {

    return &const_cast<Node&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceEventTimingPrototype::interaction_id_getter)
{
    WebIDL::log_trace(vm, "PerformanceEventTimingPrototype::interaction_id_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->interaction_id(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceEventTimingPrototype::to_json)
{
    WebIDL::log_trace(vm, "PerformanceEventTimingPrototype::to_json");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

    auto name_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->name(); }));

    JS::Value name_retval_wrapped;

    name_retval_wrapped = JS::PrimitiveString::create(vm, name_retval);

    MUST(result->create_data_property("name"_utf16_fly_string, name_retval_wrapped));

    auto entry_type_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->entry_type(); }));

    JS::Value entry_type_retval_wrapped;

    entry_type_retval_wrapped = JS::PrimitiveString::create(vm, entry_type_retval);

    MUST(result->create_data_property("entryType"_utf16_fly_string, entry_type_retval_wrapped));

    auto start_time_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->start_time(); }));

    JS::Value start_time_retval_wrapped;

    start_time_retval_wrapped = JS::Value(start_time_retval);

    MUST(result->create_data_property("startTime"_utf16_fly_string, start_time_retval_wrapped));

    auto duration_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->duration(); }));

    JS::Value duration_retval_wrapped;

    duration_retval_wrapped = JS::Value(duration_retval);

    MUST(result->create_data_property("duration"_utf16_fly_string, duration_retval_wrapped));

    auto processing_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->processing_start(); }));

    JS::Value processing_start_retval_wrapped;

    processing_start_retval_wrapped = JS::Value(processing_start_retval);

    MUST(result->create_data_property("processingStart"_utf16_fly_string, processing_start_retval_wrapped));

    auto processing_end_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->processing_end(); }));

    JS::Value processing_end_retval_wrapped;

    processing_end_retval_wrapped = JS::Value(processing_end_retval);

    MUST(result->create_data_property("processingEnd"_utf16_fly_string, processing_end_retval_wrapped));

    auto cancelable_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cancelable(); }));

    JS::Value cancelable_retval_wrapped;

    cancelable_retval_wrapped = JS::Value(cancelable_retval);

    MUST(result->create_data_property("cancelable"_utf16_fly_string, cancelable_retval_wrapped));

    auto interaction_id_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->interaction_id(); }));

    JS::Value interaction_id_retval_wrapped;

    interaction_id_retval_wrapped = JS::Value(static_cast<double>(interaction_id_retval));

    MUST(result->create_data_property("interactionId"_utf16_fly_string, interaction_id_retval_wrapped));

    return result;
}

} // namespace Web::Bindings
