
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
#include <LibWeb/Bindings/PerformanceResourceTimingPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ResourceTiming/PerformanceResourceTiming.h>

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

GC_DEFINE_ALLOCATOR(PerformanceResourceTimingPrototype);

PerformanceResourceTimingPrototype::PerformanceResourceTimingPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

PerformanceResourceTimingPrototype::~PerformanceResourceTimingPrototype()
{
}

void PerformanceResourceTimingPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<PerformanceEntryPrototype>(realm, "PerformanceEntry"_fly_string));


    auto native_initiator_type_getter = JS::NativeFunction::create(realm, initiator_type_getter, 0, "initiatorType"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_initiator_type_setter;

    define_direct_accessor("initiatorType"_utf16_fly_string, native_initiator_type_getter, native_initiator_type_setter, default_attributes);

    auto native_delivery_type_getter = JS::NativeFunction::create(realm, delivery_type_getter, 0, "deliveryType"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_delivery_type_setter;

    define_direct_accessor("deliveryType"_utf16_fly_string, native_delivery_type_getter, native_delivery_type_setter, default_attributes);

    auto native_next_hop_protocol_getter = JS::NativeFunction::create(realm, next_hop_protocol_getter, 0, "nextHopProtocol"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_next_hop_protocol_setter;

    define_direct_accessor("nextHopProtocol"_utf16_fly_string, native_next_hop_protocol_getter, native_next_hop_protocol_setter, default_attributes);

    auto native_worker_start_getter = JS::NativeFunction::create(realm, worker_start_getter, 0, "workerStart"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_worker_start_setter;

    define_direct_accessor("workerStart"_utf16_fly_string, native_worker_start_getter, native_worker_start_setter, default_attributes);

    auto native_redirect_start_getter = JS::NativeFunction::create(realm, redirect_start_getter, 0, "redirectStart"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_redirect_start_setter;

    define_direct_accessor("redirectStart"_utf16_fly_string, native_redirect_start_getter, native_redirect_start_setter, default_attributes);

    auto native_redirect_end_getter = JS::NativeFunction::create(realm, redirect_end_getter, 0, "redirectEnd"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_redirect_end_setter;

    define_direct_accessor("redirectEnd"_utf16_fly_string, native_redirect_end_getter, native_redirect_end_setter, default_attributes);

    auto native_fetch_start_getter = JS::NativeFunction::create(realm, fetch_start_getter, 0, "fetchStart"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_fetch_start_setter;

    define_direct_accessor("fetchStart"_utf16_fly_string, native_fetch_start_getter, native_fetch_start_setter, default_attributes);

    auto native_domain_lookup_start_getter = JS::NativeFunction::create(realm, domain_lookup_start_getter, 0, "domainLookupStart"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_domain_lookup_start_setter;

    define_direct_accessor("domainLookupStart"_utf16_fly_string, native_domain_lookup_start_getter, native_domain_lookup_start_setter, default_attributes);

    auto native_domain_lookup_end_getter = JS::NativeFunction::create(realm, domain_lookup_end_getter, 0, "domainLookupEnd"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_domain_lookup_end_setter;

    define_direct_accessor("domainLookupEnd"_utf16_fly_string, native_domain_lookup_end_getter, native_domain_lookup_end_setter, default_attributes);

    auto native_connect_start_getter = JS::NativeFunction::create(realm, connect_start_getter, 0, "connectStart"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_connect_start_setter;

    define_direct_accessor("connectStart"_utf16_fly_string, native_connect_start_getter, native_connect_start_setter, default_attributes);

    auto native_connect_end_getter = JS::NativeFunction::create(realm, connect_end_getter, 0, "connectEnd"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_connect_end_setter;

    define_direct_accessor("connectEnd"_utf16_fly_string, native_connect_end_getter, native_connect_end_setter, default_attributes);

    auto native_secure_connection_start_getter = JS::NativeFunction::create(realm, secure_connection_start_getter, 0, "secureConnectionStart"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_secure_connection_start_setter;

    define_direct_accessor("secureConnectionStart"_utf16_fly_string, native_secure_connection_start_getter, native_secure_connection_start_setter, default_attributes);

    auto native_request_start_getter = JS::NativeFunction::create(realm, request_start_getter, 0, "requestStart"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_request_start_setter;

    define_direct_accessor("requestStart"_utf16_fly_string, native_request_start_getter, native_request_start_setter, default_attributes);

    auto native_final_response_headers_start_getter = JS::NativeFunction::create(realm, final_response_headers_start_getter, 0, "finalResponseHeadersStart"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_final_response_headers_start_setter;

    define_direct_accessor("finalResponseHeadersStart"_utf16_fly_string, native_final_response_headers_start_getter, native_final_response_headers_start_setter, default_attributes);

    auto native_first_interim_response_start_getter = JS::NativeFunction::create(realm, first_interim_response_start_getter, 0, "firstInterimResponseStart"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_first_interim_response_start_setter;

    define_direct_accessor("firstInterimResponseStart"_utf16_fly_string, native_first_interim_response_start_getter, native_first_interim_response_start_setter, default_attributes);

    auto native_response_start_getter = JS::NativeFunction::create(realm, response_start_getter, 0, "responseStart"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_response_start_setter;

    define_direct_accessor("responseStart"_utf16_fly_string, native_response_start_getter, native_response_start_setter, default_attributes);

    auto native_response_end_getter = JS::NativeFunction::create(realm, response_end_getter, 0, "responseEnd"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_response_end_setter;

    define_direct_accessor("responseEnd"_utf16_fly_string, native_response_end_getter, native_response_end_setter, default_attributes);

    auto native_transfer_size_getter = JS::NativeFunction::create(realm, transfer_size_getter, 0, "transferSize"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_transfer_size_setter;

    define_direct_accessor("transferSize"_utf16_fly_string, native_transfer_size_getter, native_transfer_size_setter, default_attributes);

    auto native_encoded_body_size_getter = JS::NativeFunction::create(realm, encoded_body_size_getter, 0, "encodedBodySize"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_encoded_body_size_setter;

    define_direct_accessor("encodedBodySize"_utf16_fly_string, native_encoded_body_size_getter, native_encoded_body_size_setter, default_attributes);

    auto native_decoded_body_size_getter = JS::NativeFunction::create(realm, decoded_body_size_getter, 0, "decodedBodySize"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_decoded_body_size_setter;

    define_direct_accessor("decodedBodySize"_utf16_fly_string, native_decoded_body_size_getter, native_decoded_body_size_setter, default_attributes);

    auto native_response_status_getter = JS::NativeFunction::create(realm, response_status_getter, 0, "responseStatus"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_response_status_setter;

    define_direct_accessor("responseStatus"_utf16_fly_string, native_response_status_getter, native_response_status_setter, default_attributes);

    auto native_render_blocking_status_getter = JS::NativeFunction::create(realm, render_blocking_status_getter, 0, "renderBlockingStatus"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_render_blocking_status_setter;

    define_direct_accessor("renderBlockingStatus"_utf16_fly_string, native_render_blocking_status_getter, native_render_blocking_status_setter, default_attributes);

    auto native_content_type_getter = JS::NativeFunction::create(realm, content_type_getter, 0, "contentType"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_content_type_setter;

    define_direct_accessor("contentType"_utf16_fly_string, native_content_type_getter, native_content_type_setter, default_attributes);

    define_native_function(realm, "toJSON"_utf16_fly_string, to_json, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "PerformanceResourceTiming"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void PerformanceResourceTimingPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<PerformanceResourceTiming*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<PerformanceResourceTiming>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PerformanceResourceTiming");
    return static_cast<PerformanceResourceTiming*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::initiator_type_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::initiator_type_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->initiator_type(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::delivery_type_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::delivery_type_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delivery_type(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::next_hop_protocol_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::next_hop_protocol_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->next_hop_protocol(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::worker_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::worker_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->worker_start(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::redirect_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::redirect_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->redirect_start(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::redirect_end_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::redirect_end_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->redirect_end(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::fetch_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::fetch_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->fetch_start(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::domain_lookup_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::domain_lookup_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->domain_lookup_start(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::domain_lookup_end_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::domain_lookup_end_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->domain_lookup_end(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::connect_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::connect_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->connect_start(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::connect_end_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::connect_end_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->connect_end(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::secure_connection_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::secure_connection_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->secure_connection_start(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::request_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::request_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->request_start(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::final_response_headers_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::final_response_headers_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->final_response_headers_start(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::first_interim_response_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::first_interim_response_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->first_interim_response_start(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::response_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::response_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->response_start(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::response_end_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::response_end_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->response_end(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::transfer_size_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::transfer_size_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->transfer_size(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::encoded_body_size_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::encoded_body_size_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->encoded_body_size(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::decoded_body_size_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::decoded_body_size_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->decoded_body_size(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::response_status_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::response_status_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->response_status(); }));

    return JS::Value(static_cast<WebIDL::UnsignedShort>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::render_blocking_status_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::render_blocking_status_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->render_blocking_status(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::content_type_getter)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::content_type_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->content_type(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceResourceTimingPrototype::to_json)
{
    WebIDL::log_trace(vm, "PerformanceResourceTimingPrototype::to_json");
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

    auto initiator_type_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->initiator_type(); }));

    JS::Value initiator_type_retval_wrapped;

    initiator_type_retval_wrapped = JS::PrimitiveString::create(vm, initiator_type_retval);

    MUST(result->create_data_property("initiatorType"_utf16_fly_string, initiator_type_retval_wrapped));

    auto delivery_type_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delivery_type(); }));

    JS::Value delivery_type_retval_wrapped;

    delivery_type_retval_wrapped = JS::PrimitiveString::create(vm, delivery_type_retval);

    MUST(result->create_data_property("deliveryType"_utf16_fly_string, delivery_type_retval_wrapped));

    auto next_hop_protocol_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->next_hop_protocol(); }));

    JS::Value next_hop_protocol_retval_wrapped;

    next_hop_protocol_retval_wrapped = JS::PrimitiveString::create(vm, next_hop_protocol_retval);

    MUST(result->create_data_property("nextHopProtocol"_utf16_fly_string, next_hop_protocol_retval_wrapped));

    auto worker_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->worker_start(); }));

    JS::Value worker_start_retval_wrapped;

    worker_start_retval_wrapped = JS::Value(worker_start_retval);

    MUST(result->create_data_property("workerStart"_utf16_fly_string, worker_start_retval_wrapped));

    auto redirect_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->redirect_start(); }));

    JS::Value redirect_start_retval_wrapped;

    redirect_start_retval_wrapped = JS::Value(redirect_start_retval);

    MUST(result->create_data_property("redirectStart"_utf16_fly_string, redirect_start_retval_wrapped));

    auto redirect_end_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->redirect_end(); }));

    JS::Value redirect_end_retval_wrapped;

    redirect_end_retval_wrapped = JS::Value(redirect_end_retval);

    MUST(result->create_data_property("redirectEnd"_utf16_fly_string, redirect_end_retval_wrapped));

    auto fetch_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->fetch_start(); }));

    JS::Value fetch_start_retval_wrapped;

    fetch_start_retval_wrapped = JS::Value(fetch_start_retval);

    MUST(result->create_data_property("fetchStart"_utf16_fly_string, fetch_start_retval_wrapped));

    auto domain_lookup_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->domain_lookup_start(); }));

    JS::Value domain_lookup_start_retval_wrapped;

    domain_lookup_start_retval_wrapped = JS::Value(domain_lookup_start_retval);

    MUST(result->create_data_property("domainLookupStart"_utf16_fly_string, domain_lookup_start_retval_wrapped));

    auto domain_lookup_end_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->domain_lookup_end(); }));

    JS::Value domain_lookup_end_retval_wrapped;

    domain_lookup_end_retval_wrapped = JS::Value(domain_lookup_end_retval);

    MUST(result->create_data_property("domainLookupEnd"_utf16_fly_string, domain_lookup_end_retval_wrapped));

    auto connect_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->connect_start(); }));

    JS::Value connect_start_retval_wrapped;

    connect_start_retval_wrapped = JS::Value(connect_start_retval);

    MUST(result->create_data_property("connectStart"_utf16_fly_string, connect_start_retval_wrapped));

    auto connect_end_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->connect_end(); }));

    JS::Value connect_end_retval_wrapped;

    connect_end_retval_wrapped = JS::Value(connect_end_retval);

    MUST(result->create_data_property("connectEnd"_utf16_fly_string, connect_end_retval_wrapped));

    auto secure_connection_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->secure_connection_start(); }));

    JS::Value secure_connection_start_retval_wrapped;

    secure_connection_start_retval_wrapped = JS::Value(secure_connection_start_retval);

    MUST(result->create_data_property("secureConnectionStart"_utf16_fly_string, secure_connection_start_retval_wrapped));

    auto request_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->request_start(); }));

    JS::Value request_start_retval_wrapped;

    request_start_retval_wrapped = JS::Value(request_start_retval);

    MUST(result->create_data_property("requestStart"_utf16_fly_string, request_start_retval_wrapped));

    auto final_response_headers_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->final_response_headers_start(); }));

    JS::Value final_response_headers_start_retval_wrapped;

    final_response_headers_start_retval_wrapped = JS::Value(final_response_headers_start_retval);

    MUST(result->create_data_property("finalResponseHeadersStart"_utf16_fly_string, final_response_headers_start_retval_wrapped));

    auto first_interim_response_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->first_interim_response_start(); }));

    JS::Value first_interim_response_start_retval_wrapped;

    first_interim_response_start_retval_wrapped = JS::Value(first_interim_response_start_retval);

    MUST(result->create_data_property("firstInterimResponseStart"_utf16_fly_string, first_interim_response_start_retval_wrapped));

    auto response_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->response_start(); }));

    JS::Value response_start_retval_wrapped;

    response_start_retval_wrapped = JS::Value(response_start_retval);

    MUST(result->create_data_property("responseStart"_utf16_fly_string, response_start_retval_wrapped));

    auto response_end_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->response_end(); }));

    JS::Value response_end_retval_wrapped;

    response_end_retval_wrapped = JS::Value(response_end_retval);

    MUST(result->create_data_property("responseEnd"_utf16_fly_string, response_end_retval_wrapped));

    auto transfer_size_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->transfer_size(); }));

    JS::Value transfer_size_retval_wrapped;

    transfer_size_retval_wrapped = JS::Value(static_cast<double>(transfer_size_retval));

    MUST(result->create_data_property("transferSize"_utf16_fly_string, transfer_size_retval_wrapped));

    auto encoded_body_size_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->encoded_body_size(); }));

    JS::Value encoded_body_size_retval_wrapped;

    encoded_body_size_retval_wrapped = JS::Value(static_cast<double>(encoded_body_size_retval));

    MUST(result->create_data_property("encodedBodySize"_utf16_fly_string, encoded_body_size_retval_wrapped));

    auto decoded_body_size_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->decoded_body_size(); }));

    JS::Value decoded_body_size_retval_wrapped;

    decoded_body_size_retval_wrapped = JS::Value(static_cast<double>(decoded_body_size_retval));

    MUST(result->create_data_property("decodedBodySize"_utf16_fly_string, decoded_body_size_retval_wrapped));

    auto response_status_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->response_status(); }));

    JS::Value response_status_retval_wrapped;

    response_status_retval_wrapped = JS::Value(static_cast<WebIDL::UnsignedShort>(response_status_retval));

    MUST(result->create_data_property("responseStatus"_utf16_fly_string, response_status_retval_wrapped));

    auto render_blocking_status_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->render_blocking_status(); }));

    JS::Value render_blocking_status_retval_wrapped;

    render_blocking_status_retval_wrapped = JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(render_blocking_status_retval));

    MUST(result->create_data_property("renderBlockingStatus"_utf16_fly_string, render_blocking_status_retval_wrapped));

    auto content_type_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->content_type(); }));

    JS::Value content_type_retval_wrapped;

    content_type_retval_wrapped = JS::PrimitiveString::create(vm, content_type_retval);

    MUST(result->create_data_property("contentType"_utf16_fly_string, content_type_retval_wrapped));

    return result;
}

} // namespace Web::Bindings
