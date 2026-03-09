
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
#include <LibWeb/Bindings/ServiceWorkerRegistrationPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorkerRegistration.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorker.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/MessagePort.h>

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

GC_DEFINE_ALLOCATOR(ServiceWorkerRegistrationPrototype);

ServiceWorkerRegistrationPrototype::ServiceWorkerRegistrationPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

ServiceWorkerRegistrationPrototype::~ServiceWorkerRegistrationPrototype()
{
}

void ServiceWorkerRegistrationPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    auto native_installing_getter = JS::NativeFunction::create(realm, installing_getter, 0, "installing"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_installing_setter;

    define_direct_accessor("installing"_utf16_fly_string, native_installing_getter, native_installing_setter, default_attributes);

    auto native_waiting_getter = JS::NativeFunction::create(realm, waiting_getter, 0, "waiting"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_waiting_setter;

    define_direct_accessor("waiting"_utf16_fly_string, native_waiting_getter, native_waiting_setter, default_attributes);

    auto native_active_getter = JS::NativeFunction::create(realm, active_getter, 0, "active"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_active_setter;

    define_direct_accessor("active"_utf16_fly_string, native_active_getter, native_active_setter, default_attributes);

    define_direct_property("navigationPreload"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    auto native_scope_getter = JS::NativeFunction::create(realm, scope_getter, 0, "scope"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_scope_setter;

    define_direct_accessor("scope"_utf16_fly_string, native_scope_getter, native_scope_setter, default_attributes);

    auto native_update_via_cache_getter = JS::NativeFunction::create(realm, update_via_cache_getter, 0, "updateViaCache"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_update_via_cache_setter;

    define_direct_accessor("updateViaCache"_utf16_fly_string, native_update_via_cache_getter, native_update_via_cache_setter, default_attributes);

    define_direct_property("onupdatefound"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("update"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("unregister"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "ServiceWorkerRegistration"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void ServiceWorkerRegistrationPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<ServiceWorker::ServiceWorkerRegistration*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<ServiceWorker::ServiceWorkerRegistration>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ServiceWorkerRegistration");
    return static_cast<ServiceWorker::ServiceWorkerRegistration*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(ServiceWorkerRegistrationPrototype::installing_getter)
{
    WebIDL::log_trace(vm, "ServiceWorkerRegistrationPrototype::installing_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->installing(); }));

    if (retval) {

    return &const_cast<ServiceWorker::ServiceWorker&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ServiceWorkerRegistrationPrototype::waiting_getter)
{
    WebIDL::log_trace(vm, "ServiceWorkerRegistrationPrototype::waiting_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->waiting(); }));

    if (retval) {

    return &const_cast<ServiceWorker::ServiceWorker&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ServiceWorkerRegistrationPrototype::active_getter)
{
    WebIDL::log_trace(vm, "ServiceWorkerRegistrationPrototype::active_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->active(); }));

    if (retval) {

    return &const_cast<ServiceWorker::ServiceWorker&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ServiceWorkerRegistrationPrototype::scope_getter)
{
    WebIDL::log_trace(vm, "ServiceWorkerRegistrationPrototype::scope_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scope(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(ServiceWorkerRegistrationPrototype::update_via_cache_getter)
{
    WebIDL::log_trace(vm, "ServiceWorkerRegistrationPrototype::update_via_cache_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->update_via_cache(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

} // namespace Web::Bindings
