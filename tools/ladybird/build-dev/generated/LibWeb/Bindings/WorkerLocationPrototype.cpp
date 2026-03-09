
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
#include <LibWeb/Bindings/WorkerLocationPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/WorkerLocation.h>

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

GC_DEFINE_ALLOCATOR(WorkerLocationPrototype);

WorkerLocationPrototype::WorkerLocationPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

WorkerLocationPrototype::~WorkerLocationPrototype()
{
}

void WorkerLocationPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_href_getter = JS::NativeFunction::create(realm, href_getter, 0, "href"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_href_setter;

    define_direct_accessor("href"_utf16_fly_string, native_href_getter, native_href_setter, default_attributes);

    auto native_origin_getter = JS::NativeFunction::create(realm, origin_getter, 0, "origin"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_origin_setter;

    define_direct_accessor("origin"_utf16_fly_string, native_origin_getter, native_origin_setter, default_attributes);

    auto native_protocol_getter = JS::NativeFunction::create(realm, protocol_getter, 0, "protocol"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_protocol_setter;

    define_direct_accessor("protocol"_utf16_fly_string, native_protocol_getter, native_protocol_setter, default_attributes);

    auto native_host_getter = JS::NativeFunction::create(realm, host_getter, 0, "host"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_host_setter;

    define_direct_accessor("host"_utf16_fly_string, native_host_getter, native_host_setter, default_attributes);

    auto native_hostname_getter = JS::NativeFunction::create(realm, hostname_getter, 0, "hostname"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_hostname_setter;

    define_direct_accessor("hostname"_utf16_fly_string, native_hostname_getter, native_hostname_setter, default_attributes);

    auto native_port_getter = JS::NativeFunction::create(realm, port_getter, 0, "port"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_port_setter;

    define_direct_accessor("port"_utf16_fly_string, native_port_getter, native_port_setter, default_attributes);

    auto native_pathname_getter = JS::NativeFunction::create(realm, pathname_getter, 0, "pathname"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_pathname_setter;

    define_direct_accessor("pathname"_utf16_fly_string, native_pathname_getter, native_pathname_setter, default_attributes);

    auto native_search_getter = JS::NativeFunction::create(realm, search_getter, 0, "search"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_search_setter;

    define_direct_accessor("search"_utf16_fly_string, native_search_getter, native_search_setter, default_attributes);

    auto native_hash_getter = JS::NativeFunction::create(realm, hash_getter, 0, "hash"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_hash_setter;

    define_direct_accessor("hash"_utf16_fly_string, native_hash_getter, native_hash_setter, default_attributes);

    define_native_function(realm, "toString"_utf16_fly_string, to_string, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "WorkerLocation"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void WorkerLocationPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::WorkerLocation*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::WorkerLocation>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WorkerLocation");
    return static_cast<HTML::WorkerLocation*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(WorkerLocationPrototype::href_getter)
{
    WebIDL::log_trace(vm, "WorkerLocationPrototype::href_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->href(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerLocationPrototype::origin_getter)
{
    WebIDL::log_trace(vm, "WorkerLocationPrototype::origin_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->origin(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerLocationPrototype::protocol_getter)
{
    WebIDL::log_trace(vm, "WorkerLocationPrototype::protocol_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->protocol(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerLocationPrototype::host_getter)
{
    WebIDL::log_trace(vm, "WorkerLocationPrototype::host_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->host(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerLocationPrototype::hostname_getter)
{
    WebIDL::log_trace(vm, "WorkerLocationPrototype::hostname_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->hostname(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerLocationPrototype::port_getter)
{
    WebIDL::log_trace(vm, "WorkerLocationPrototype::port_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->port(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerLocationPrototype::pathname_getter)
{
    WebIDL::log_trace(vm, "WorkerLocationPrototype::pathname_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->pathname(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerLocationPrototype::search_getter)
{
    WebIDL::log_trace(vm, "WorkerLocationPrototype::search_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->search(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerLocationPrototype::hash_getter)
{
    WebIDL::log_trace(vm, "WorkerLocationPrototype::hash_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->hash(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerLocationPrototype::to_string)
{
    WebIDL::log_trace(vm, "WorkerLocationPrototype::to_string");
    [[maybe_unused]] auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));


    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->href(); }));


    return JS::PrimitiveString::create(vm, move(retval));
}

} // namespace Web::Bindings
