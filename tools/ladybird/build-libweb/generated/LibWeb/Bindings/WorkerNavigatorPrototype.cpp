
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
#include <LibWeb/Bindings/WorkerNavigatorPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/WorkerNavigator.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/MediaCapabilitiesAPI/MediaCapabilities.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Serial/Serial.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/EncryptedMediaExtensions/MediaKeySystemAccess.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Serial/SerialPort.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/StorageAPI/StorageManager.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

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

GC_DEFINE_ALLOCATOR(WorkerNavigatorPrototype);

WorkerNavigatorPrototype::WorkerNavigatorPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

WorkerNavigatorPrototype::~WorkerNavigatorPrototype()
{
}

void WorkerNavigatorPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_media_capabilities_getter = JS::NativeFunction::create(realm, media_capabilities_getter, 0, "mediaCapabilities"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_media_capabilities_setter;

    define_direct_accessor("mediaCapabilities"_utf16_fly_string, native_media_capabilities_getter, native_media_capabilities_setter, default_attributes);

    auto native_serial_getter = JS::NativeFunction::create(realm, serial_getter, 0, "serial"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_serial_setter;

    define_direct_accessor("serial"_utf16_fly_string, native_serial_getter, native_serial_setter, default_attributes);

    if (HTML::is_secure_context(Bindings::principal_host_defined_environment_settings_object(realm))) {
    auto native_service_worker_getter = JS::NativeFunction::create(realm, service_worker_getter, 0, "serviceWorker"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_service_worker_setter;

    define_direct_accessor("serviceWorker"_utf16_fly_string, native_service_worker_getter, native_service_worker_setter, default_attributes);

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

    auto native_on_line_getter = JS::NativeFunction::create(realm, on_line_getter, 0, "onLine"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_on_line_setter;

    define_direct_accessor("onLine"_utf16_fly_string, native_on_line_getter, native_on_line_setter, default_attributes);

    auto native_storage_getter = JS::NativeFunction::create(realm, storage_getter, 0, "storage"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_storage_setter;

    define_direct_accessor("storage"_utf16_fly_string, native_storage_getter, native_storage_setter, default_attributes);

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

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "WorkerNavigator"_string), JS::Attribute::Configurable);

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

    }

    Base::initialize(realm);

}

void WorkerNavigatorPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::WorkerNavigator*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::WorkerNavigator>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WorkerNavigator");
    return static_cast<HTML::WorkerNavigator*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::media_capabilities_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::media_capabilities_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->media_capabilities(); }));

    return &const_cast<MediaCapabilities&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::serial_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::serial_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->serial(); }));

    return &const_cast<Serial::Serial&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::service_worker_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::service_worker_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->service_worker(); }));

    return &const_cast<ServiceWorkerContainer&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::app_code_name_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::app_code_name_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->app_code_name(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::app_name_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::app_name_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->app_name(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::app_version_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::app_version_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->app_version(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::platform_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::platform_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->platform(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::product_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::product_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->product(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::product_sub_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::product_sub_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->product_sub(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::user_agent_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::user_agent_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->user_agent(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::vendor_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::vendor_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vendor(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::vendor_sub_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::vendor_sub_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vendor_sub(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::on_line_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::on_line_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->on_line(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::storage_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::storage_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->storage(); }));

    return &const_cast<StorageManager&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::language_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::language_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->language(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::languages_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::languages_getter");
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

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::hardware_concurrency_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::hardware_concurrency_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->hardware_concurrency(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WorkerNavigatorPrototype::device_memory_getter)
{
    WebIDL::log_trace(vm, "WorkerNavigatorPrototype::device_memory_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->device_memory(); }));

    return JS::Value(retval);

}

} // namespace Web::Bindings
