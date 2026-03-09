
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
#include <LibWeb/Bindings/RequestPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Request.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Headers.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XHR/FormData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOMURL/URLSearchParams.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/QueuingStrategy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamBYOBReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamDefaultReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/File.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStreamDefaultWriter.h>

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

GC_DEFINE_ALLOCATOR(RequestPrototype);

RequestPrototype::RequestPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

RequestPrototype::~RequestPrototype()
{
}

void RequestPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_method_getter = JS::NativeFunction::create(realm, method_getter, 0, "method"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_method_setter;

    define_direct_accessor("method"_utf16_fly_string, native_method_getter, native_method_setter, default_attributes);

    auto native_url_getter = JS::NativeFunction::create(realm, url_getter, 0, "url"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_url_setter;

    define_direct_accessor("url"_utf16_fly_string, native_url_getter, native_url_setter, default_attributes);

    auto native_headers_getter = JS::NativeFunction::create(realm, headers_getter, 0, "headers"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_headers_setter;

    define_direct_accessor("headers"_utf16_fly_string, native_headers_getter, native_headers_setter, default_attributes);

    auto native_destination_getter = JS::NativeFunction::create(realm, destination_getter, 0, "destination"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_destination_setter;

    define_direct_accessor("destination"_utf16_fly_string, native_destination_getter, native_destination_setter, default_attributes);

    auto native_referrer_getter = JS::NativeFunction::create(realm, referrer_getter, 0, "referrer"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_referrer_setter;

    define_direct_accessor("referrer"_utf16_fly_string, native_referrer_getter, native_referrer_setter, default_attributes);

    auto native_referrer_policy_getter = JS::NativeFunction::create(realm, referrer_policy_getter, 0, "referrerPolicy"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_referrer_policy_setter;

    define_direct_accessor("referrerPolicy"_utf16_fly_string, native_referrer_policy_getter, native_referrer_policy_setter, default_attributes);

    auto native_mode_getter = JS::NativeFunction::create(realm, mode_getter, 0, "mode"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_mode_setter;

    define_direct_accessor("mode"_utf16_fly_string, native_mode_getter, native_mode_setter, default_attributes);

    auto native_credentials_getter = JS::NativeFunction::create(realm, credentials_getter, 0, "credentials"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_credentials_setter;

    define_direct_accessor("credentials"_utf16_fly_string, native_credentials_getter, native_credentials_setter, default_attributes);

    auto native_cache_getter = JS::NativeFunction::create(realm, cache_getter, 0, "cache"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_cache_setter;

    define_direct_accessor("cache"_utf16_fly_string, native_cache_getter, native_cache_setter, default_attributes);

    auto native_redirect_getter = JS::NativeFunction::create(realm, redirect_getter, 0, "redirect"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_redirect_setter;

    define_direct_accessor("redirect"_utf16_fly_string, native_redirect_getter, native_redirect_setter, default_attributes);

    auto native_integrity_getter = JS::NativeFunction::create(realm, integrity_getter, 0, "integrity"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_integrity_setter;

    define_direct_accessor("integrity"_utf16_fly_string, native_integrity_getter, native_integrity_setter, default_attributes);

    auto native_keepalive_getter = JS::NativeFunction::create(realm, keepalive_getter, 0, "keepalive"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_keepalive_setter;

    define_direct_accessor("keepalive"_utf16_fly_string, native_keepalive_getter, native_keepalive_setter, default_attributes);

    auto native_is_reload_navigation_getter = JS::NativeFunction::create(realm, is_reload_navigation_getter, 0, "isReloadNavigation"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_is_reload_navigation_setter;

    define_direct_accessor("isReloadNavigation"_utf16_fly_string, native_is_reload_navigation_getter, native_is_reload_navigation_setter, default_attributes);

    auto native_is_history_navigation_getter = JS::NativeFunction::create(realm, is_history_navigation_getter, 0, "isHistoryNavigation"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_is_history_navigation_setter;

    define_direct_accessor("isHistoryNavigation"_utf16_fly_string, native_is_history_navigation_getter, native_is_history_navigation_setter, default_attributes);

    auto native_signal_getter = JS::NativeFunction::create(realm, signal_getter, 0, "signal"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_signal_setter;

    define_direct_accessor("signal"_utf16_fly_string, native_signal_getter, native_signal_setter, default_attributes);

    auto native_duplex_getter = JS::NativeFunction::create(realm, duplex_getter, 0, "duplex"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_duplex_setter;

    define_direct_accessor("duplex"_utf16_fly_string, native_duplex_getter, native_duplex_setter, default_attributes);

    auto native_body_getter = JS::NativeFunction::create(realm, body_getter, 0, "body"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_body_setter;

    define_direct_accessor("body"_utf16_fly_string, native_body_getter, native_body_setter, default_attributes);

    auto native_body_used_getter = JS::NativeFunction::create(realm, body_used_getter, 0, "bodyUsed"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_body_used_setter;

    define_direct_accessor("bodyUsed"_utf16_fly_string, native_body_used_getter, native_body_used_setter, default_attributes);

    define_native_function(realm, "clone"_utf16_fly_string, clone, 0, default_attributes);

    define_native_function(realm, "arrayBuffer"_utf16_fly_string, array_buffer, 0, default_attributes);

    define_native_function(realm, "blob"_utf16_fly_string, blob, 0, default_attributes);

    define_native_function(realm, "bytes"_utf16_fly_string, bytes, 0, default_attributes);

    define_native_function(realm, "formData"_utf16_fly_string, form_data, 0, default_attributes);

    define_native_function(realm, "json"_utf16_fly_string, json, 0, default_attributes);

    define_native_function(realm, "text"_utf16_fly_string, text, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Request"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void RequestPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Fetch::Request*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Fetch::Request>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Request");
    return static_cast<Fetch::Request*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::method_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::method_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->method(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::url_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::url_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->url(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::headers_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::headers_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->headers(); }));

    return &const_cast<Headers&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::destination_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::destination_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->destination(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::referrer_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::referrer_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->referrer(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::referrer_policy_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::referrer_policy_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->referrer_policy(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::mode_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::mode_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->mode(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::credentials_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::credentials_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->credentials(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::cache_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::cache_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cache(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::redirect_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::redirect_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->redirect(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::integrity_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::integrity_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->integrity(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::keepalive_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::keepalive_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->keepalive(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::is_reload_navigation_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::is_reload_navigation_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_reload_navigation(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::is_history_navigation_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::is_history_navigation_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_history_navigation(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::signal_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::signal_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->signal(); }));

    return &const_cast<AbortSignal&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::duplex_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::duplex_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->duplex(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::body_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::body_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->body(); }));

    if (retval) {

    return &const_cast<ReadableStream&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::body_used_getter)
{
    WebIDL::log_trace(vm, "RequestPrototype::body_used_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->body_used(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::clone)
{
    WebIDL::log_trace(vm, "RequestPrototype::clone");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clone(); }));

    return &const_cast<Request&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::array_buffer)
{
    WebIDL::log_trace(vm, "RequestPrototype::array_buffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->array_buffer(); }));

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

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::blob)
{
    WebIDL::log_trace(vm, "RequestPrototype::blob");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->blob(); }));

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

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::bytes)
{
    WebIDL::log_trace(vm, "RequestPrototype::bytes");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bytes(); }));

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

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::form_data)
{
    WebIDL::log_trace(vm, "RequestPrototype::form_data");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->form_data(); }));

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

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::json)
{
    WebIDL::log_trace(vm, "RequestPrototype::json");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->json(); }));

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

JS_DEFINE_NATIVE_FUNCTION(RequestPrototype::text)
{
    WebIDL::log_trace(vm, "RequestPrototype::text");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->text(); }));

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
