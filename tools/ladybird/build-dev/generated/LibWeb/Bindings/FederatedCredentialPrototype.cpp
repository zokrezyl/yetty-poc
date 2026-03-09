
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
#include <LibWeb/Bindings/FederatedCredentialPrototype.h>
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

#if __has_include(<LibWeb/Bindings/CredentialPrototype.h>)
#    include <LibWeb/Bindings/CredentialPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CredentialManagement/FederatedCredential.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CredentialManagement/Credential.h>

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

GC_DEFINE_ALLOCATOR(FederatedCredentialPrototype);

FederatedCredentialPrototype::FederatedCredentialPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

FederatedCredentialPrototype::~FederatedCredentialPrototype()
{
}

void FederatedCredentialPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<CredentialPrototype>(realm, "Credential"_fly_string));


    auto native_provider_getter = JS::NativeFunction::create(realm, provider_getter, 0, "provider"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_provider_setter;

    define_direct_accessor("provider"_utf16_fly_string, native_provider_getter, native_provider_setter, default_attributes);

    auto native_protocol_getter = JS::NativeFunction::create(realm, protocol_getter, 0, "protocol"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_protocol_setter;

    define_direct_accessor("protocol"_utf16_fly_string, native_protocol_getter, native_protocol_setter, default_attributes);

    auto native_name_getter = JS::NativeFunction::create(realm, name_getter, 0, "name"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_name_setter;

    define_direct_accessor("name"_utf16_fly_string, native_name_getter, native_name_setter, default_attributes);

    auto native_icon_url_getter = JS::NativeFunction::create(realm, icon_url_getter, 0, "iconURL"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_icon_url_setter;

    define_direct_accessor("iconURL"_utf16_fly_string, native_icon_url_getter, native_icon_url_setter, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "FederatedCredential"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void FederatedCredentialPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<FederatedCredential*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<FederatedCredential>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "FederatedCredential");
    return static_cast<FederatedCredential*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(FederatedCredentialPrototype::provider_getter)
{
    WebIDL::log_trace(vm, "FederatedCredentialPrototype::provider_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->provider(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(FederatedCredentialPrototype::protocol_getter)
{
    WebIDL::log_trace(vm, "FederatedCredentialPrototype::protocol_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->protocol(); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(FederatedCredentialPrototype::name_getter)
{
    WebIDL::log_trace(vm, "FederatedCredentialPrototype::name_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->name(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(FederatedCredentialPrototype::icon_url_getter)
{
    WebIDL::log_trace(vm, "FederatedCredentialPrototype::icon_url_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->icon_url(); }));

    return JS::PrimitiveString::create(vm, retval);

}

} // namespace Web::Bindings
