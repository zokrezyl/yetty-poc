
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
#include <LibWeb/Bindings/NavigatorPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Navigator.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Clipboard/Clipboard.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CredentialManagement/CredentialsContainer.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/EncryptedMediaExtensions/MediaKeySystemAccess.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Gamepad/Gamepad.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geolocation/Geolocation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/MimeTypeArray.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/PluginArray.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/UserActivation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/MediaCapabilitiesAPI/MediaCapabilities.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Serial/Serial.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorkerContainer.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebXR/XRSystem.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Clipboard/ClipboardItem.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CredentialManagement/Credential.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CredentialManagement/FederatedCredential.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CredentialManagement/PasswordCredential.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Gamepad/GamepadButton.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Gamepad/GamepadHapticActuator.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/MimeType.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Plugin.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Serial/SerialPort.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Worker.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorkerRegistration.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedScriptURL.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/StorageAPI/StorageManager.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOMURL/URLSearchParams.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XHR/FormData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/MessagePort.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Request.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorker.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/QueuingStrategy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamBYOBReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamDefaultReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/File.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Headers.h>

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

GC_DEFINE_ALLOCATOR(NavigatorPrototype);

NavigatorPrototype::NavigatorPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

NavigatorPrototype::~NavigatorPrototype()
{
}

void NavigatorPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    if (HTML::is_secure_context(Bindings::principal_host_defined_environment_settings_object(realm))) {
    auto native_clipboard_getter = JS::NativeFunction::create(realm, clipboard_getter, 0, "clipboard"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_clipboard_setter;

    define_direct_accessor("clipboard"_utf16_fly_string, native_clipboard_getter, native_clipboard_setter, default_attributes);

    }
    auto native_geolocation_getter = JS::NativeFunction::create(realm, geolocation_getter, 0, "geolocation"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_geolocation_setter;

    define_direct_accessor("geolocation"_utf16_fly_string, native_geolocation_getter, native_geolocation_setter, default_attributes);

    auto native_max_touch_points_getter = JS::NativeFunction::create(realm, max_touch_points_getter, 0, "maxTouchPoints"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_max_touch_points_setter;

    define_direct_accessor("maxTouchPoints"_utf16_fly_string, native_max_touch_points_getter, native_max_touch_points_setter, default_attributes);

    auto native_serial_getter = JS::NativeFunction::create(realm, serial_getter, 0, "serial"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_serial_setter;

    define_direct_accessor("serial"_utf16_fly_string, native_serial_getter, native_serial_setter, default_attributes);

    auto native_user_activation_getter = JS::NativeFunction::create(realm, user_activation_getter, 0, "userActivation"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_user_activation_setter;

    define_direct_accessor("userActivation"_utf16_fly_string, native_user_activation_getter, native_user_activation_setter, default_attributes);

    if (HTML::is_secure_context(Bindings::principal_host_defined_environment_settings_object(realm))) {
    auto native_service_worker_getter = JS::NativeFunction::create(realm, service_worker_getter, 0, "serviceWorker"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_service_worker_setter;

    define_direct_accessor("serviceWorker"_utf16_fly_string, native_service_worker_getter, native_service_worker_setter, default_attributes);

    }
    auto native_media_capabilities_getter = JS::NativeFunction::create(realm, media_capabilities_getter, 0, "mediaCapabilities"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_media_capabilities_setter;

    define_direct_accessor("mediaCapabilities"_utf16_fly_string, native_media_capabilities_getter, native_media_capabilities_setter, default_attributes);

    if (HTML::is_secure_context(Bindings::principal_host_defined_environment_settings_object(realm))) {
    auto native_credentials_getter = JS::NativeFunction::create(realm, credentials_getter, 0, "credentials"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_credentials_setter;

    define_direct_accessor("credentials"_utf16_fly_string, native_credentials_getter, native_credentials_setter, default_attributes);

    }
    if (HTML::is_secure_context(Bindings::principal_host_defined_environment_settings_object(realm))) {
    auto native_xr_getter = JS::NativeFunction::create(realm, xr_getter, 0, "xr"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_xr_setter;

    define_direct_accessor("xr"_utf16_fly_string, native_xr_getter, native_xr_setter, default_attributes);

    }
    auto native_app_code_name_getter = JS::NativeFunction::create(realm, app_code_name_getter, 0, "appCodeName"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_app_code_name_setter;

    define_direct_accessor("appCodeName"_utf16_fly_string, native_app_code_name_getter, native_app_code_name_setter, default_attributes);

    auto native_app_name_getter = JS::NativeFunction::create(realm, app_name_getter, 0, "appName"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_app_name_setter;

    define_direct_accessor("appName"_utf16_fly_string, native_app_name_getter, native_app_name_setter, default_attributes);

    auto native_app_version_getter = JS::NativeFunction::create(realm, app_version_getter, 0, "appVersion"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_app_version_setter;

    define_direct_accessor("appVersion"_utf16_fly_string, native_app_version_getter, native_app_version_setter, default_attributes);

    auto native_platform_getter = JS::NativeFunction::create(realm, platform_getter, 0, "platform"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_platform_setter;

    define_direct_accessor("platform"_utf16_fly_string, native_platform_getter, native_platform_setter, default_attributes);

    auto native_product_getter = JS::NativeFunction::create(realm, product_getter, 0, "product"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_product_setter;

    define_direct_accessor("product"_utf16_fly_string, native_product_getter, native_product_setter, default_attributes);

    auto native_user_agent_getter = JS::NativeFunction::create(realm, user_agent_getter, 0, "userAgent"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_user_agent_setter;

    define_direct_accessor("userAgent"_utf16_fly_string, native_user_agent_getter, native_user_agent_setter, default_attributes);

    auto native_plugins_getter = JS::NativeFunction::create(realm, plugins_getter, 0, "plugins"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_plugins_setter;

    define_direct_accessor("plugins"_utf16_fly_string, native_plugins_getter, native_plugins_setter, default_attributes);

    auto native_mime_types_getter = JS::NativeFunction::create(realm, mime_types_getter, 0, "mimeTypes"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_mime_types_setter;

    define_direct_accessor("mimeTypes"_utf16_fly_string, native_mime_types_getter, native_mime_types_setter, default_attributes);

    auto native_pdf_viewer_enabled_getter = JS::NativeFunction::create(realm, pdf_viewer_enabled_getter, 0, "pdfViewerEnabled"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_pdf_viewer_enabled_setter;

    define_direct_accessor("pdfViewerEnabled"_utf16_fly_string, native_pdf_viewer_enabled_getter, native_pdf_viewer_enabled_setter, default_attributes);

    auto native_webdriver_getter = JS::NativeFunction::create(realm, webdriver_getter, 0, "webdriver"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_webdriver_setter;

    define_direct_accessor("webdriver"_utf16_fly_string, native_webdriver_getter, native_webdriver_setter, default_attributes);

    auto native_cookie_enabled_getter = JS::NativeFunction::create(realm, cookie_enabled_getter, 0, "cookieEnabled"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_cookie_enabled_setter;

    define_direct_accessor("cookieEnabled"_utf16_fly_string, native_cookie_enabled_getter, native_cookie_enabled_setter, default_attributes);

    auto native_on_line_getter = JS::NativeFunction::create(realm, on_line_getter, 0, "onLine"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_on_line_setter;

    define_direct_accessor("onLine"_utf16_fly_string, native_on_line_getter, native_on_line_setter, default_attributes);

    auto native_storage_getter = JS::NativeFunction::create(realm, storage_getter, 0, "storage"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_storage_setter;

    define_direct_accessor("storage"_utf16_fly_string, native_storage_getter, native_storage_setter, default_attributes);

    auto native_global_privacy_control_getter = JS::NativeFunction::create(realm, global_privacy_control_getter, 0, "globalPrivacyControl"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_global_privacy_control_setter;

    define_direct_accessor("globalPrivacyControl"_utf16_fly_string, native_global_privacy_control_getter, native_global_privacy_control_setter, default_attributes);

    auto native_language_getter = JS::NativeFunction::create(realm, language_getter, 0, "language"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_language_setter;

    define_direct_accessor("language"_utf16_fly_string, native_language_getter, native_language_setter, default_attributes);

    auto native_languages_getter = JS::NativeFunction::create(realm, languages_getter, 0, "languages"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_languages_setter;

    define_direct_accessor("languages"_utf16_fly_string, native_languages_getter, native_languages_setter, default_attributes);

    auto native_hardware_concurrency_getter = JS::NativeFunction::create(realm, hardware_concurrency_getter, 0, "hardwareConcurrency"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_hardware_concurrency_setter;

    define_direct_accessor("hardwareConcurrency"_utf16_fly_string, native_hardware_concurrency_getter, native_hardware_concurrency_setter, default_attributes);

    auto native_device_memory_getter = JS::NativeFunction::create(realm, device_memory_getter, 0, "deviceMemory"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_device_memory_setter;

    define_direct_accessor("deviceMemory"_utf16_fly_string, native_device_memory_getter, native_device_memory_setter, default_attributes);

        define_direct_property("registerProtocolHandler"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("unregisterProtocolHandler"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    if (HTML::is_secure_context(Bindings::principal_host_defined_environment_settings_object(realm))) {
    define_native_function(realm, "getBattery"_utf16_fly_string, get_battery, 0, default_attributes);

    }
    define_native_function(realm, "sendBeacon"_utf16_fly_string, send_beacon, 1, default_attributes);

    define_native_function(realm, "javaEnabled"_utf16_fly_string, java_enabled, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Navigator"_string), JS::Attribute::Configurable);

    if (is<HTML::Window>(realm.global_object())) {

    auto native_product_sub_getter = JS::NativeFunction::create(realm, product_sub_getter, 0, "productSub"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_product_sub_setter;

    define_direct_accessor("productSub"_utf16_fly_string, native_product_sub_getter, native_product_sub_setter, default_attributes);

    auto native_vendor_getter = JS::NativeFunction::create(realm, vendor_getter, 0, "vendor"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_vendor_setter;

    define_direct_accessor("vendor"_utf16_fly_string, native_vendor_getter, native_vendor_setter, default_attributes);

    auto native_vendor_sub_getter = JS::NativeFunction::create(realm, vendor_sub_getter, 0, "vendorSub"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_vendor_sub_setter;

    define_direct_accessor("vendorSub"_utf16_fly_string, native_vendor_sub_getter, native_vendor_sub_setter, default_attributes);

    if (HTML::is_secure_context(Bindings::principal_host_defined_environment_settings_object(realm))) {
    define_native_function(realm, "requestMediaKeySystemAccess"_utf16_fly_string, request_media_key_system_access, 2, default_attributes);

    }
    define_native_function(realm, "getGamepads"_utf16_fly_string, get_gamepads, 0, default_attributes);

    }

    Base::initialize(realm);

}

void NavigatorPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::Navigator*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::Navigator>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Navigator");
    return static_cast<HTML::Navigator*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::clipboard_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::clipboard_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clipboard(); }));

    return &const_cast<Clipboard::Clipboard&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::geolocation_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::geolocation_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->geolocation(); }));

    return &const_cast<Geolocation::Geolocation&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::max_touch_points_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::max_touch_points_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->max_touch_points(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::serial_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::serial_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->serial(); }));

    return &const_cast<Serial::Serial&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::user_activation_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::user_activation_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->user_activation(); }));

    return &const_cast<UserActivation&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::service_worker_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::service_worker_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->service_worker(); }));

    return &const_cast<ServiceWorkerContainer&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::media_capabilities_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::media_capabilities_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->media_capabilities(); }));

    return &const_cast<MediaCapabilities&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::credentials_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::credentials_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->credentials(); }));

    return &const_cast<CredentialsContainer&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::xr_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::xr_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->xr(); }));

    return &const_cast<XRSystem&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::app_code_name_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::app_code_name_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->app_code_name(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::app_name_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::app_name_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->app_name(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::app_version_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::app_version_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->app_version(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::platform_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::platform_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->platform(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::product_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::product_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->product(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::product_sub_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::product_sub_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->product_sub(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::user_agent_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::user_agent_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->user_agent(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::vendor_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::vendor_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vendor(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::vendor_sub_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::vendor_sub_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vendor_sub(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::plugins_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::plugins_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->plugins(); }));

    return &const_cast<PluginArray&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::mime_types_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::mime_types_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->mime_types(); }));

    return &const_cast<MimeTypeArray&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::pdf_viewer_enabled_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::pdf_viewer_enabled_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->pdf_viewer_enabled(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::webdriver_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::webdriver_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->webdriver(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::cookie_enabled_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::cookie_enabled_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cookie_enabled(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::on_line_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::on_line_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->on_line(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::storage_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::storage_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->storage(); }));

    return &const_cast<StorageManager&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::global_privacy_control_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::global_privacy_control_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->global_privacy_control(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::language_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::language_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->language(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::languages_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::languages_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->languages(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);
JS::Value wrapped_element0;

    wrapped_element0 = JS::PrimitiveString::create(vm, element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    TRY(new_array0_0->set_integrity_level(IntegrityLevel::Frozen));

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::hardware_concurrency_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::hardware_concurrency_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->hardware_concurrency(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::device_memory_getter)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::device_memory_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->device_memory(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::get_battery)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::get_battery");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_battery(); }));

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

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::request_media_key_system_access)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::request_media_key_system_access");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "requestMediaKeySystemAccess", "2");

    auto arg0 = vm.argument(0);

    Utf16String key_system;
    if (!false || !arg0.is_null()) {
        key_system = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    if (!arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg1);

    auto arg1_iterator_method0 = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg1_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg1);

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *arg1_iterator_method0));

    Vector<MediaKeySystemConfiguration> supported_configurations;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_nullish() && !next_item1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "MediaKeySystemConfiguration");

    MediaKeySystemConfiguration sequence_item1 {};

    auto audio_capabilities_property_value_0 = JS::js_undefined();
    if (next_item1.is_object())
        audio_capabilities_property_value_0 = TRY(next_item1.as_object().get("audioCapabilities"_utf16_fly_string));

    Vector<MediaKeySystemMediaCapability> audio_capabilities_value_0;

    if (!audio_capabilities_property_value_0.is_undefined()) {

    if (!audio_capabilities_property_value_0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, audio_capabilities_property_value_0);

    auto audio_capabilities_property_value_0_iterator_method0 = TRY(audio_capabilities_property_value_0.get_method(vm, vm.well_known_symbol_iterator()));
    if (!audio_capabilities_property_value_0_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, audio_capabilities_property_value_0);

    auto audio_capabilities_property_value_0_iterator1 = TRY(JS::get_iterator_from_method(vm, audio_capabilities_property_value_0, *audio_capabilities_property_value_0_iterator_method0));

    Vector<MediaKeySystemMediaCapability> audio_capabilities_value_0_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, audio_capabilities_property_value_0_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_nullish() && !next_item1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "MediaKeySystemMediaCapability");

    MediaKeySystemMediaCapability sequence_item1 {};

    auto content_type_property_value_0 = JS::js_undefined();
    if (next_item1.is_object())
        content_type_property_value_0 = TRY(next_item1.as_object().get("contentType"_utf16_fly_string));

    Utf16String content_type_value_0;

    if (!content_type_property_value_0.is_undefined()) {
        if (!false || !content_type_property_value_0.is_null())
            content_type_value_0 = TRY(WebIDL::to_utf16_string(vm, content_type_property_value_0));
    } else {
        content_type_value_0 = ""_utf16;
    }

    sequence_item1.content_type = content_type_value_0;

    auto encryption_scheme_property_value_1 = JS::js_undefined();
    if (next_item1.is_object())
        encryption_scheme_property_value_1 = TRY(next_item1.as_object().get("encryptionScheme"_utf16_fly_string));

    Optional<Utf16String> encryption_scheme_value_1;

    if (!encryption_scheme_property_value_1.is_undefined()) {
        if (!encryption_scheme_property_value_1.is_null())
            encryption_scheme_value_1 = TRY(WebIDL::to_utf16_string(vm, encryption_scheme_property_value_1));
    }

    sequence_item1.encryption_scheme = encryption_scheme_value_1;

    auto robustness_property_value_2 = JS::js_undefined();
    if (next_item1.is_object())
        robustness_property_value_2 = TRY(next_item1.as_object().get("robustness"_utf16_fly_string));

    Utf16String robustness_value_2;

    if (!robustness_property_value_2.is_undefined()) {
        if (!false || !robustness_property_value_2.is_null())
            robustness_value_2 = TRY(WebIDL::to_utf16_string(vm, robustness_property_value_2));
    } else {
        robustness_value_2 = ""_utf16;
    }

    sequence_item1.robustness = robustness_value_2;

    audio_capabilities_value_0_non_optional.append(sequence_item1);
    }

        audio_capabilities_value_0 = move(audio_capabilities_value_0_non_optional);
    }

    sequence_item1.audio_capabilities = audio_capabilities_value_0;

    auto distinctive_identifier_property_value_4 = JS::js_undefined();
    if (next_item1.is_object())
        distinctive_identifier_property_value_4 = TRY(next_item1.as_object().get("distinctiveIdentifier"_utf16_fly_string));

    MediaKeysRequirement distinctive_identifier_value_4 { MediaKeysRequirement::Optional };

    if (!distinctive_identifier_property_value_4.is_undefined()) {

    auto distinctive_identifier_property_value_4_string = TRY(distinctive_identifier_property_value_4.to_string(vm));

    if (distinctive_identifier_property_value_4_string == "required"sv)
        distinctive_identifier_value_4 = MediaKeysRequirement::Required;

    else if (distinctive_identifier_property_value_4_string == "optional"sv)
        distinctive_identifier_value_4 = MediaKeysRequirement::Optional;

    else if (distinctive_identifier_property_value_4_string == "not-allowed"sv)
        distinctive_identifier_value_4 = MediaKeysRequirement::NotAllowed;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, distinctive_identifier_property_value_4_string, "MediaKeysRequirement");

    }

    sequence_item1.distinctive_identifier = distinctive_identifier_value_4;

    auto init_data_types_property_value_5 = JS::js_undefined();
    if (next_item1.is_object())
        init_data_types_property_value_5 = TRY(next_item1.as_object().get("initDataTypes"_utf16_fly_string));

    Vector<Utf16String> init_data_types_value_5;

    if (!init_data_types_property_value_5.is_undefined()) {

    if (!init_data_types_property_value_5.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, init_data_types_property_value_5);

    auto init_data_types_property_value_5_iterator_method0 = TRY(init_data_types_property_value_5.get_method(vm, vm.well_known_symbol_iterator()));
    if (!init_data_types_property_value_5_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, init_data_types_property_value_5);

    auto init_data_types_property_value_5_iterator1 = TRY(JS::get_iterator_from_method(vm, init_data_types_property_value_5, *init_data_types_property_value_5_iterator_method0));

    Vector<Utf16String> init_data_types_value_5_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, init_data_types_property_value_5_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    Utf16String sequence_item1;
    if (!false || !next_item1.is_null()) {
        sequence_item1 = TRY(WebIDL::to_utf16_string(vm, next_item1));
    }

    init_data_types_value_5_non_optional.append(sequence_item1);
    }

        init_data_types_value_5 = move(init_data_types_value_5_non_optional);
    }

    sequence_item1.init_data_types = init_data_types_value_5;

    auto label_property_value_6 = JS::js_undefined();
    if (next_item1.is_object())
        label_property_value_6 = TRY(next_item1.as_object().get("label"_utf16_fly_string));

    Utf16String label_value_6;

    if (!label_property_value_6.is_undefined()) {
        if (!false || !label_property_value_6.is_null())
            label_value_6 = TRY(WebIDL::to_utf16_string(vm, label_property_value_6));
    } else {
        label_value_6 = ""_utf16;
    }

    sequence_item1.label = label_value_6;

    auto persistent_state_property_value_7 = JS::js_undefined();
    if (next_item1.is_object())
        persistent_state_property_value_7 = TRY(next_item1.as_object().get("persistentState"_utf16_fly_string));

    MediaKeysRequirement persistent_state_value_7 { MediaKeysRequirement::Optional };

    if (!persistent_state_property_value_7.is_undefined()) {

    auto persistent_state_property_value_7_string = TRY(persistent_state_property_value_7.to_string(vm));

    if (persistent_state_property_value_7_string == "required"sv)
        persistent_state_value_7 = MediaKeysRequirement::Required;

    else if (persistent_state_property_value_7_string == "optional"sv)
        persistent_state_value_7 = MediaKeysRequirement::Optional;

    else if (persistent_state_property_value_7_string == "not-allowed"sv)
        persistent_state_value_7 = MediaKeysRequirement::NotAllowed;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, persistent_state_property_value_7_string, "MediaKeysRequirement");

    }

    sequence_item1.persistent_state = persistent_state_value_7;

    auto session_types_property_value_8 = JS::js_undefined();
    if (next_item1.is_object())
        session_types_property_value_8 = TRY(next_item1.as_object().get("sessionTypes"_utf16_fly_string));

    if (!session_types_property_value_8.is_undefined()) {

    Optional<Vector<Utf16String>> session_types_value_8;

    if (!session_types_property_value_8.is_undefined()) {

    if (!session_types_property_value_8.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, session_types_property_value_8);

    auto session_types_property_value_8_iterator_method0 = TRY(session_types_property_value_8.get_method(vm, vm.well_known_symbol_iterator()));
    if (!session_types_property_value_8_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, session_types_property_value_8);

    auto session_types_property_value_8_iterator1 = TRY(JS::get_iterator_from_method(vm, session_types_property_value_8, *session_types_property_value_8_iterator_method0));

    Vector<Utf16String> session_types_value_8_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, session_types_property_value_8_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    Utf16String sequence_item1;
    if (!false || !next_item1.is_null()) {
        sequence_item1 = TRY(WebIDL::to_utf16_string(vm, next_item1));
    }

    session_types_value_8_non_optional.append(sequence_item1);
    }

        session_types_value_8 = move(session_types_value_8_non_optional);
    }

    sequence_item1.session_types = session_types_value_8;

    }

    auto video_capabilities_property_value_9 = JS::js_undefined();
    if (next_item1.is_object())
        video_capabilities_property_value_9 = TRY(next_item1.as_object().get("videoCapabilities"_utf16_fly_string));

    Vector<MediaKeySystemMediaCapability> video_capabilities_value_9;

    if (!video_capabilities_property_value_9.is_undefined()) {

    if (!video_capabilities_property_value_9.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, video_capabilities_property_value_9);

    auto video_capabilities_property_value_9_iterator_method0 = TRY(video_capabilities_property_value_9.get_method(vm, vm.well_known_symbol_iterator()));
    if (!video_capabilities_property_value_9_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, video_capabilities_property_value_9);

    auto video_capabilities_property_value_9_iterator1 = TRY(JS::get_iterator_from_method(vm, video_capabilities_property_value_9, *video_capabilities_property_value_9_iterator_method0));

    Vector<MediaKeySystemMediaCapability> video_capabilities_value_9_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, video_capabilities_property_value_9_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_nullish() && !next_item1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "MediaKeySystemMediaCapability");

    MediaKeySystemMediaCapability sequence_item1 {};

    auto content_type_property_value_9 = JS::js_undefined();
    if (next_item1.is_object())
        content_type_property_value_9 = TRY(next_item1.as_object().get("contentType"_utf16_fly_string));

    Utf16String content_type_value_9;

    if (!content_type_property_value_9.is_undefined()) {
        if (!false || !content_type_property_value_9.is_null())
            content_type_value_9 = TRY(WebIDL::to_utf16_string(vm, content_type_property_value_9));
    } else {
        content_type_value_9 = ""_utf16;
    }

    sequence_item1.content_type = content_type_value_9;

    auto encryption_scheme_property_value_10 = JS::js_undefined();
    if (next_item1.is_object())
        encryption_scheme_property_value_10 = TRY(next_item1.as_object().get("encryptionScheme"_utf16_fly_string));

    Optional<Utf16String> encryption_scheme_value_10;

    if (!encryption_scheme_property_value_10.is_undefined()) {
        if (!encryption_scheme_property_value_10.is_null())
            encryption_scheme_value_10 = TRY(WebIDL::to_utf16_string(vm, encryption_scheme_property_value_10));
    }

    sequence_item1.encryption_scheme = encryption_scheme_value_10;

    auto robustness_property_value_11 = JS::js_undefined();
    if (next_item1.is_object())
        robustness_property_value_11 = TRY(next_item1.as_object().get("robustness"_utf16_fly_string));

    Utf16String robustness_value_11;

    if (!robustness_property_value_11.is_undefined()) {
        if (!false || !robustness_property_value_11.is_null())
            robustness_value_11 = TRY(WebIDL::to_utf16_string(vm, robustness_property_value_11));
    } else {
        robustness_value_11 = ""_utf16;
    }

    sequence_item1.robustness = robustness_value_11;

    video_capabilities_value_9_non_optional.append(sequence_item1);
    }

        video_capabilities_value_9 = move(video_capabilities_value_9_non_optional);
    }

    sequence_item1.video_capabilities = video_capabilities_value_9;

    supported_configurations.append(sequence_item1);
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->request_media_key_system_access(key_system, supported_configurations); }));

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

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::get_gamepads)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::get_gamepads");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_gamepads(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::send_beacon)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::send_beacon");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "sendBeacon");

    auto arg0 = vm.argument(0);

    String url;
    if (!false || !arg0.is_null()) {
        url = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<ReadableStream>, GC::Root<Blob>, GC::Root<WebIDL::BufferSource>, GC::Root<FormData>, GC::Root<URLSearchParams>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (is<PlatformObject>(arg1_object)) {

                if (auto* arg1_result = as_if<ReadableStream>(arg1_object))
                    return GC::make_root(*arg1_result);

                if (auto* arg1_result = as_if<Blob>(arg1_object))
                    return GC::make_root(*arg1_result);

                if (auto* arg1_result = as_if<FormData>(arg1_object))
                    return GC::make_root(*arg1_result);

                if (auto* arg1_result = as_if<URLSearchParams>(arg1_object))
                    return GC::make_root(*arg1_result);

            }

            if (is<JS::ArrayBuffer>(arg1_object) || is<JS::DataView>(arg1_object) || is<JS::TypedArrayBase>(arg1_object)) {
                GC::Ref<WebIDL::BufferSource> source_object = realm.create<WebIDL::BufferSource>(arg1_object);
                return GC::make_root(source_object);
            }

        }

    String arg1_string;
    if (!false || !arg1.is_null()) {
        arg1_string = TRY(WebIDL::to_usv_string(vm, arg1));
    }

        return { arg1_string };

    };

    Optional<Variant<GC::Root<ReadableStream>, GC::Root<Blob>, GC::Root<WebIDL::BufferSource>, GC::Root<FormData>, GC::Root<URLSearchParams>, String>> data;
    if (!arg1.is_nullish())
        data = TRY(arg1_to_variant(arg1));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->send_beacon(url, data); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigatorPrototype::java_enabled)
{
    WebIDL::log_trace(vm, "NavigatorPrototype::java_enabled");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->java_enabled(); }));

    return JS::Value(retval);

}

} // namespace Web::Bindings
