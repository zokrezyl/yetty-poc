
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
#include <LibWeb/Bindings/PerformanceTimingPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/NavigationTiming/PerformanceTiming.h>

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

GC_DEFINE_ALLOCATOR(PerformanceTimingPrototype);

PerformanceTimingPrototype::PerformanceTimingPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

PerformanceTimingPrototype::~PerformanceTimingPrototype()
{
}

void PerformanceTimingPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_navigation_start_getter = JS::NativeFunction::create(realm, navigation_start_getter, 0, "navigationStart"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_navigation_start_setter;

    define_direct_accessor("navigationStart"_utf16_fly_string, native_navigation_start_getter, native_navigation_start_setter, default_attributes);

    auto native_unload_event_start_getter = JS::NativeFunction::create(realm, unload_event_start_getter, 0, "unloadEventStart"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_unload_event_start_setter;

    define_direct_accessor("unloadEventStart"_utf16_fly_string, native_unload_event_start_getter, native_unload_event_start_setter, default_attributes);

    auto native_unload_event_end_getter = JS::NativeFunction::create(realm, unload_event_end_getter, 0, "unloadEventEnd"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_unload_event_end_setter;

    define_direct_accessor("unloadEventEnd"_utf16_fly_string, native_unload_event_end_getter, native_unload_event_end_setter, default_attributes);

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

    auto native_response_start_getter = JS::NativeFunction::create(realm, response_start_getter, 0, "responseStart"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_response_start_setter;

    define_direct_accessor("responseStart"_utf16_fly_string, native_response_start_getter, native_response_start_setter, default_attributes);

    auto native_response_end_getter = JS::NativeFunction::create(realm, response_end_getter, 0, "responseEnd"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_response_end_setter;

    define_direct_accessor("responseEnd"_utf16_fly_string, native_response_end_getter, native_response_end_setter, default_attributes);

    auto native_dom_loading_getter = JS::NativeFunction::create(realm, dom_loading_getter, 0, "domLoading"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_dom_loading_setter;

    define_direct_accessor("domLoading"_utf16_fly_string, native_dom_loading_getter, native_dom_loading_setter, default_attributes);

    auto native_dom_interactive_getter = JS::NativeFunction::create(realm, dom_interactive_getter, 0, "domInteractive"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_dom_interactive_setter;

    define_direct_accessor("domInteractive"_utf16_fly_string, native_dom_interactive_getter, native_dom_interactive_setter, default_attributes);

    auto native_dom_content_loaded_event_start_getter = JS::NativeFunction::create(realm, dom_content_loaded_event_start_getter, 0, "domContentLoadedEventStart"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_dom_content_loaded_event_start_setter;

    define_direct_accessor("domContentLoadedEventStart"_utf16_fly_string, native_dom_content_loaded_event_start_getter, native_dom_content_loaded_event_start_setter, default_attributes);

    auto native_dom_content_loaded_event_end_getter = JS::NativeFunction::create(realm, dom_content_loaded_event_end_getter, 0, "domContentLoadedEventEnd"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_dom_content_loaded_event_end_setter;

    define_direct_accessor("domContentLoadedEventEnd"_utf16_fly_string, native_dom_content_loaded_event_end_getter, native_dom_content_loaded_event_end_setter, default_attributes);

    auto native_dom_complete_getter = JS::NativeFunction::create(realm, dom_complete_getter, 0, "domComplete"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_dom_complete_setter;

    define_direct_accessor("domComplete"_utf16_fly_string, native_dom_complete_getter, native_dom_complete_setter, default_attributes);

    auto native_load_event_start_getter = JS::NativeFunction::create(realm, load_event_start_getter, 0, "loadEventStart"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_load_event_start_setter;

    define_direct_accessor("loadEventStart"_utf16_fly_string, native_load_event_start_getter, native_load_event_start_setter, default_attributes);

    auto native_load_event_end_getter = JS::NativeFunction::create(realm, load_event_end_getter, 0, "loadEventEnd"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_load_event_end_setter;

    define_direct_accessor("loadEventEnd"_utf16_fly_string, native_load_event_end_getter, native_load_event_end_setter, default_attributes);

    define_native_function(realm, "toJSON"_utf16_fly_string, to_json, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "PerformanceTiming"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void PerformanceTimingPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<NavigationTiming::PerformanceTiming*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<NavigationTiming::PerformanceTiming>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PerformanceTiming");
    return static_cast<NavigationTiming::PerformanceTiming*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::navigation_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::navigation_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->navigation_start(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::unload_event_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::unload_event_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->unload_event_start(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::unload_event_end_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::unload_event_end_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->unload_event_end(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::redirect_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::redirect_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->redirect_start(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::redirect_end_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::redirect_end_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->redirect_end(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::fetch_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::fetch_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->fetch_start(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::domain_lookup_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::domain_lookup_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->domain_lookup_start(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::domain_lookup_end_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::domain_lookup_end_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->domain_lookup_end(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::connect_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::connect_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->connect_start(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::connect_end_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::connect_end_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->connect_end(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::secure_connection_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::secure_connection_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->secure_connection_start(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::request_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::request_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->request_start(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::response_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::response_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->response_start(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::response_end_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::response_end_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->response_end(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::dom_loading_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::dom_loading_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dom_loading(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::dom_interactive_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::dom_interactive_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dom_interactive(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::dom_content_loaded_event_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::dom_content_loaded_event_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dom_content_loaded_event_start(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::dom_content_loaded_event_end_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::dom_content_loaded_event_end_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dom_content_loaded_event_end(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::dom_complete_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::dom_complete_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dom_complete(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::load_event_start_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::load_event_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->load_event_start(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::load_event_end_getter)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::load_event_end_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->load_event_end(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceTimingPrototype::to_json)
{
    WebIDL::log_trace(vm, "PerformanceTimingPrototype::to_json");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

    auto navigation_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->navigation_start(); }));

    JS::Value navigation_start_retval_wrapped;

    navigation_start_retval_wrapped = JS::Value(static_cast<double>(navigation_start_retval));

    MUST(result->create_data_property("navigationStart"_utf16_fly_string, navigation_start_retval_wrapped));

    auto unload_event_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->unload_event_start(); }));

    JS::Value unload_event_start_retval_wrapped;

    unload_event_start_retval_wrapped = JS::Value(static_cast<double>(unload_event_start_retval));

    MUST(result->create_data_property("unloadEventStart"_utf16_fly_string, unload_event_start_retval_wrapped));

    auto unload_event_end_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->unload_event_end(); }));

    JS::Value unload_event_end_retval_wrapped;

    unload_event_end_retval_wrapped = JS::Value(static_cast<double>(unload_event_end_retval));

    MUST(result->create_data_property("unloadEventEnd"_utf16_fly_string, unload_event_end_retval_wrapped));

    auto redirect_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->redirect_start(); }));

    JS::Value redirect_start_retval_wrapped;

    redirect_start_retval_wrapped = JS::Value(static_cast<double>(redirect_start_retval));

    MUST(result->create_data_property("redirectStart"_utf16_fly_string, redirect_start_retval_wrapped));

    auto redirect_end_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->redirect_end(); }));

    JS::Value redirect_end_retval_wrapped;

    redirect_end_retval_wrapped = JS::Value(static_cast<double>(redirect_end_retval));

    MUST(result->create_data_property("redirectEnd"_utf16_fly_string, redirect_end_retval_wrapped));

    auto fetch_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->fetch_start(); }));

    JS::Value fetch_start_retval_wrapped;

    fetch_start_retval_wrapped = JS::Value(static_cast<double>(fetch_start_retval));

    MUST(result->create_data_property("fetchStart"_utf16_fly_string, fetch_start_retval_wrapped));

    auto domain_lookup_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->domain_lookup_start(); }));

    JS::Value domain_lookup_start_retval_wrapped;

    domain_lookup_start_retval_wrapped = JS::Value(static_cast<double>(domain_lookup_start_retval));

    MUST(result->create_data_property("domainLookupStart"_utf16_fly_string, domain_lookup_start_retval_wrapped));

    auto domain_lookup_end_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->domain_lookup_end(); }));

    JS::Value domain_lookup_end_retval_wrapped;

    domain_lookup_end_retval_wrapped = JS::Value(static_cast<double>(domain_lookup_end_retval));

    MUST(result->create_data_property("domainLookupEnd"_utf16_fly_string, domain_lookup_end_retval_wrapped));

    auto connect_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->connect_start(); }));

    JS::Value connect_start_retval_wrapped;

    connect_start_retval_wrapped = JS::Value(static_cast<double>(connect_start_retval));

    MUST(result->create_data_property("connectStart"_utf16_fly_string, connect_start_retval_wrapped));

    auto connect_end_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->connect_end(); }));

    JS::Value connect_end_retval_wrapped;

    connect_end_retval_wrapped = JS::Value(static_cast<double>(connect_end_retval));

    MUST(result->create_data_property("connectEnd"_utf16_fly_string, connect_end_retval_wrapped));

    auto secure_connection_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->secure_connection_start(); }));

    JS::Value secure_connection_start_retval_wrapped;

    secure_connection_start_retval_wrapped = JS::Value(static_cast<double>(secure_connection_start_retval));

    MUST(result->create_data_property("secureConnectionStart"_utf16_fly_string, secure_connection_start_retval_wrapped));

    auto request_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->request_start(); }));

    JS::Value request_start_retval_wrapped;

    request_start_retval_wrapped = JS::Value(static_cast<double>(request_start_retval));

    MUST(result->create_data_property("requestStart"_utf16_fly_string, request_start_retval_wrapped));

    auto response_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->response_start(); }));

    JS::Value response_start_retval_wrapped;

    response_start_retval_wrapped = JS::Value(static_cast<double>(response_start_retval));

    MUST(result->create_data_property("responseStart"_utf16_fly_string, response_start_retval_wrapped));

    auto response_end_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->response_end(); }));

    JS::Value response_end_retval_wrapped;

    response_end_retval_wrapped = JS::Value(static_cast<double>(response_end_retval));

    MUST(result->create_data_property("responseEnd"_utf16_fly_string, response_end_retval_wrapped));

    auto dom_loading_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dom_loading(); }));

    JS::Value dom_loading_retval_wrapped;

    dom_loading_retval_wrapped = JS::Value(static_cast<double>(dom_loading_retval));

    MUST(result->create_data_property("domLoading"_utf16_fly_string, dom_loading_retval_wrapped));

    auto dom_interactive_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dom_interactive(); }));

    JS::Value dom_interactive_retval_wrapped;

    dom_interactive_retval_wrapped = JS::Value(static_cast<double>(dom_interactive_retval));

    MUST(result->create_data_property("domInteractive"_utf16_fly_string, dom_interactive_retval_wrapped));

    auto dom_content_loaded_event_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dom_content_loaded_event_start(); }));

    JS::Value dom_content_loaded_event_start_retval_wrapped;

    dom_content_loaded_event_start_retval_wrapped = JS::Value(static_cast<double>(dom_content_loaded_event_start_retval));

    MUST(result->create_data_property("domContentLoadedEventStart"_utf16_fly_string, dom_content_loaded_event_start_retval_wrapped));

    auto dom_content_loaded_event_end_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dom_content_loaded_event_end(); }));

    JS::Value dom_content_loaded_event_end_retval_wrapped;

    dom_content_loaded_event_end_retval_wrapped = JS::Value(static_cast<double>(dom_content_loaded_event_end_retval));

    MUST(result->create_data_property("domContentLoadedEventEnd"_utf16_fly_string, dom_content_loaded_event_end_retval_wrapped));

    auto dom_complete_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dom_complete(); }));

    JS::Value dom_complete_retval_wrapped;

    dom_complete_retval_wrapped = JS::Value(static_cast<double>(dom_complete_retval));

    MUST(result->create_data_property("domComplete"_utf16_fly_string, dom_complete_retval_wrapped));

    auto load_event_start_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->load_event_start(); }));

    JS::Value load_event_start_retval_wrapped;

    load_event_start_retval_wrapped = JS::Value(static_cast<double>(load_event_start_retval));

    MUST(result->create_data_property("loadEventStart"_utf16_fly_string, load_event_start_retval_wrapped));

    auto load_event_end_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->load_event_end(); }));

    JS::Value load_event_end_retval_wrapped;

    load_event_end_retval_wrapped = JS::Value(static_cast<double>(load_event_end_retval));

    MUST(result->create_data_property("loadEventEnd"_utf16_fly_string, load_event_end_retval_wrapped));

    return result;
}

} // namespace Web::Bindings
