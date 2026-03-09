
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
#include <LibWeb/Bindings/LocationPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Location.h>

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

GC_DEFINE_ALLOCATOR(LocationPrototype);

LocationPrototype::LocationPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

LocationPrototype::~LocationPrototype()
{
}

void LocationPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Location"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void LocationPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    auto native_href_getter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Location"_utf16_fly_string, "href"_utf16_fly_string, href_getter, UnforgeableKey::Type::Getter);

    auto native_href_setter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Location"_utf16_fly_string, "href"_utf16_fly_string, href_setter, UnforgeableKey::Type::Setter);

    object.define_direct_accessor("href"_utf16_fly_string, native_href_getter, native_href_setter, default_attributes);

    auto native_origin_getter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Location"_utf16_fly_string, "origin"_utf16_fly_string, origin_getter, UnforgeableKey::Type::Getter);

    GC::Ptr<JS::NativeFunction> native_origin_setter;

    object.define_direct_accessor("origin"_utf16_fly_string, native_origin_getter, native_origin_setter, default_attributes);

    auto native_protocol_getter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Location"_utf16_fly_string, "protocol"_utf16_fly_string, protocol_getter, UnforgeableKey::Type::Getter);

    auto native_protocol_setter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Location"_utf16_fly_string, "protocol"_utf16_fly_string, protocol_setter, UnforgeableKey::Type::Setter);

    object.define_direct_accessor("protocol"_utf16_fly_string, native_protocol_getter, native_protocol_setter, default_attributes);

    auto native_host_getter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Location"_utf16_fly_string, "host"_utf16_fly_string, host_getter, UnforgeableKey::Type::Getter);

    auto native_host_setter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Location"_utf16_fly_string, "host"_utf16_fly_string, host_setter, UnforgeableKey::Type::Setter);

    object.define_direct_accessor("host"_utf16_fly_string, native_host_getter, native_host_setter, default_attributes);

    auto native_hostname_getter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Location"_utf16_fly_string, "hostname"_utf16_fly_string, hostname_getter, UnforgeableKey::Type::Getter);

    auto native_hostname_setter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Location"_utf16_fly_string, "hostname"_utf16_fly_string, hostname_setter, UnforgeableKey::Type::Setter);

    object.define_direct_accessor("hostname"_utf16_fly_string, native_hostname_getter, native_hostname_setter, default_attributes);

    auto native_port_getter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Location"_utf16_fly_string, "port"_utf16_fly_string, port_getter, UnforgeableKey::Type::Getter);

    auto native_port_setter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Location"_utf16_fly_string, "port"_utf16_fly_string, port_setter, UnforgeableKey::Type::Setter);

    object.define_direct_accessor("port"_utf16_fly_string, native_port_getter, native_port_setter, default_attributes);

    auto native_pathname_getter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Location"_utf16_fly_string, "pathname"_utf16_fly_string, pathname_getter, UnforgeableKey::Type::Getter);

    auto native_pathname_setter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Location"_utf16_fly_string, "pathname"_utf16_fly_string, pathname_setter, UnforgeableKey::Type::Setter);

    object.define_direct_accessor("pathname"_utf16_fly_string, native_pathname_getter, native_pathname_setter, default_attributes);

    auto native_search_getter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Location"_utf16_fly_string, "search"_utf16_fly_string, search_getter, UnforgeableKey::Type::Getter);

    auto native_search_setter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Location"_utf16_fly_string, "search"_utf16_fly_string, search_setter, UnforgeableKey::Type::Setter);

    object.define_direct_accessor("search"_utf16_fly_string, native_search_getter, native_search_setter, default_attributes);

    auto native_hash_getter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Location"_utf16_fly_string, "hash"_utf16_fly_string, hash_getter, UnforgeableKey::Type::Getter);

    auto native_hash_setter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Location"_utf16_fly_string, "hash"_utf16_fly_string, hash_setter, UnforgeableKey::Type::Setter);

    object.define_direct_accessor("hash"_utf16_fly_string, native_hash_getter, native_hash_setter, default_attributes);

    object.define_native_function(realm, "assign"_utf16_fly_string, assign, 1, default_attributes);

    object.define_native_function(realm, "replace"_utf16_fly_string, replace, 1, default_attributes);

    object.define_native_function(realm, "reload"_utf16_fly_string, reload, 0, default_attributes);

    object.define_native_function(realm, "toString"_utf16_fly_string, to_string, 0, default_attributes);

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::Location*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::Location>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Location");
    return static_cast<HTML::Location*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::href_getter)
{
    WebIDL::log_trace(vm, "LocationPrototype::href_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->href(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::href_setter)
{
    WebIDL::log_trace(vm, "LocationPrototype::href_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_usv_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_href(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::origin_getter)
{
    WebIDL::log_trace(vm, "LocationPrototype::origin_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->origin(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::protocol_getter)
{
    WebIDL::log_trace(vm, "LocationPrototype::protocol_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->protocol(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::protocol_setter)
{
    WebIDL::log_trace(vm, "LocationPrototype::protocol_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_usv_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_protocol(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::host_getter)
{
    WebIDL::log_trace(vm, "LocationPrototype::host_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->host(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::host_setter)
{
    WebIDL::log_trace(vm, "LocationPrototype::host_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_usv_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_host(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::hostname_getter)
{
    WebIDL::log_trace(vm, "LocationPrototype::hostname_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->hostname(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::hostname_setter)
{
    WebIDL::log_trace(vm, "LocationPrototype::hostname_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_usv_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_hostname(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::port_getter)
{
    WebIDL::log_trace(vm, "LocationPrototype::port_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->port(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::port_setter)
{
    WebIDL::log_trace(vm, "LocationPrototype::port_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_usv_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_port(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::pathname_getter)
{
    WebIDL::log_trace(vm, "LocationPrototype::pathname_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->pathname(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::pathname_setter)
{
    WebIDL::log_trace(vm, "LocationPrototype::pathname_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_usv_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_pathname(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::search_getter)
{
    WebIDL::log_trace(vm, "LocationPrototype::search_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->search(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::search_setter)
{
    WebIDL::log_trace(vm, "LocationPrototype::search_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_usv_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_search(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::hash_getter)
{
    WebIDL::log_trace(vm, "LocationPrototype::hash_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->hash(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::hash_setter)
{
    WebIDL::log_trace(vm, "LocationPrototype::hash_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_usv_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_hash(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::assign)
{
    WebIDL::log_trace(vm, "LocationPrototype::assign");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "assign");

    auto arg0 = vm.argument(0);

    String url;
    if (!false || !arg0.is_null()) {
        url = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->assign(url); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::replace)
{
    WebIDL::log_trace(vm, "LocationPrototype::replace");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "replace");

    auto arg0 = vm.argument(0);

    String url;
    if (!false || !arg0.is_null()) {
        url = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->replace(url); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::reload)
{
    WebIDL::log_trace(vm, "LocationPrototype::reload");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->reload(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(LocationPrototype::to_string)
{
    WebIDL::log_trace(vm, "LocationPrototype::to_string");
    [[maybe_unused]] auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));


    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->href(); }));


    return JS::PrimitiveString::create(vm, move(retval));
}

} // namespace Web::Bindings
