
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
#include <LibWeb/Bindings/URLPatternPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/URLPattern/URLPattern.h>

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

GC_DEFINE_ALLOCATOR(URLPatternPrototype);

URLPatternPrototype::URLPatternPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

URLPatternPrototype::~URLPatternPrototype()
{
}

void URLPatternPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_protocol_getter = JS::NativeFunction::create(realm, protocol_getter, 0, "protocol"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_protocol_setter;

    define_direct_accessor("protocol"_utf16_fly_string, native_protocol_getter, native_protocol_setter, default_attributes);

    auto native_username_getter = JS::NativeFunction::create(realm, username_getter, 0, "username"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_username_setter;

    define_direct_accessor("username"_utf16_fly_string, native_username_getter, native_username_setter, default_attributes);

    auto native_password_getter = JS::NativeFunction::create(realm, password_getter, 0, "password"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_password_setter;

    define_direct_accessor("password"_utf16_fly_string, native_password_getter, native_password_setter, default_attributes);

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

    auto native_has_reg_exp_groups_getter = JS::NativeFunction::create(realm, has_reg_exp_groups_getter, 0, "hasRegExpGroups"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_has_reg_exp_groups_setter;

    define_direct_accessor("hasRegExpGroups"_utf16_fly_string, native_has_reg_exp_groups_getter, native_has_reg_exp_groups_setter, default_attributes);

    define_native_function(realm, "test"_utf16_fly_string, test, 0, default_attributes);

    define_native_function(realm, "exec"_utf16_fly_string, exec, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "URLPattern"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void URLPatternPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<URLPattern::URLPattern*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<URLPattern::URLPattern>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "URLPattern");
    return static_cast<URLPattern::URLPattern*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(URLPatternPrototype::protocol_getter)
{
    WebIDL::log_trace(vm, "URLPatternPrototype::protocol_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->protocol(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(URLPatternPrototype::username_getter)
{
    WebIDL::log_trace(vm, "URLPatternPrototype::username_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->username(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(URLPatternPrototype::password_getter)
{
    WebIDL::log_trace(vm, "URLPatternPrototype::password_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->password(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(URLPatternPrototype::hostname_getter)
{
    WebIDL::log_trace(vm, "URLPatternPrototype::hostname_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->hostname(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(URLPatternPrototype::port_getter)
{
    WebIDL::log_trace(vm, "URLPatternPrototype::port_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->port(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(URLPatternPrototype::pathname_getter)
{
    WebIDL::log_trace(vm, "URLPatternPrototype::pathname_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->pathname(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(URLPatternPrototype::search_getter)
{
    WebIDL::log_trace(vm, "URLPatternPrototype::search_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->search(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(URLPatternPrototype::hash_getter)
{
    WebIDL::log_trace(vm, "URLPatternPrototype::hash_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->hash(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(URLPatternPrototype::has_reg_exp_groups_getter)
{
    WebIDL::log_trace(vm, "URLPatternPrototype::has_reg_exp_groups_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->has_reg_exp_groups(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(URLPatternPrototype::test)
{
    WebIDL::log_trace(vm, "URLPatternPrototype::test");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    auto arg0_to_dictionary = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<URLPatternInit> {
        // This might be unused.
        (void)realm;

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "URLPatternInit");

    URLPatternInit dictionary_union_type {};

    auto base_url_property_value_18 = JS::js_undefined();
    if (arg0.is_object())
        base_url_property_value_18 = TRY(arg0.as_object().get("baseURL"_utf16_fly_string));

    if (!base_url_property_value_18.is_undefined()) {

    Optional<String> base_url_value_18;

    if (!base_url_property_value_18.is_undefined()) {
        if (!false || !base_url_property_value_18.is_null())
            base_url_value_18 = TRY(WebIDL::to_usv_string(vm, base_url_property_value_18));
    }

    dictionary_union_type.base_url = base_url_value_18;

    }

    auto hash_property_value_19 = JS::js_undefined();
    if (arg0.is_object())
        hash_property_value_19 = TRY(arg0.as_object().get("hash"_utf16_fly_string));

    if (!hash_property_value_19.is_undefined()) {

    Optional<String> hash_value_19;

    if (!hash_property_value_19.is_undefined()) {
        if (!false || !hash_property_value_19.is_null())
            hash_value_19 = TRY(WebIDL::to_usv_string(vm, hash_property_value_19));
    }

    dictionary_union_type.hash = hash_value_19;

    }

    auto hostname_property_value_20 = JS::js_undefined();
    if (arg0.is_object())
        hostname_property_value_20 = TRY(arg0.as_object().get("hostname"_utf16_fly_string));

    if (!hostname_property_value_20.is_undefined()) {

    Optional<String> hostname_value_20;

    if (!hostname_property_value_20.is_undefined()) {
        if (!false || !hostname_property_value_20.is_null())
            hostname_value_20 = TRY(WebIDL::to_usv_string(vm, hostname_property_value_20));
    }

    dictionary_union_type.hostname = hostname_value_20;

    }

    auto password_property_value_21 = JS::js_undefined();
    if (arg0.is_object())
        password_property_value_21 = TRY(arg0.as_object().get("password"_utf16_fly_string));

    if (!password_property_value_21.is_undefined()) {

    Optional<String> password_value_21;

    if (!password_property_value_21.is_undefined()) {
        if (!false || !password_property_value_21.is_null())
            password_value_21 = TRY(WebIDL::to_usv_string(vm, password_property_value_21));
    }

    dictionary_union_type.password = password_value_21;

    }

    auto pathname_property_value_22 = JS::js_undefined();
    if (arg0.is_object())
        pathname_property_value_22 = TRY(arg0.as_object().get("pathname"_utf16_fly_string));

    if (!pathname_property_value_22.is_undefined()) {

    Optional<String> pathname_value_22;

    if (!pathname_property_value_22.is_undefined()) {
        if (!false || !pathname_property_value_22.is_null())
            pathname_value_22 = TRY(WebIDL::to_usv_string(vm, pathname_property_value_22));
    }

    dictionary_union_type.pathname = pathname_value_22;

    }

    auto port_property_value_23 = JS::js_undefined();
    if (arg0.is_object())
        port_property_value_23 = TRY(arg0.as_object().get("port"_utf16_fly_string));

    if (!port_property_value_23.is_undefined()) {

    Optional<String> port_value_23;

    if (!port_property_value_23.is_undefined()) {
        if (!false || !port_property_value_23.is_null())
            port_value_23 = TRY(WebIDL::to_usv_string(vm, port_property_value_23));
    }

    dictionary_union_type.port = port_value_23;

    }

    auto protocol_property_value_24 = JS::js_undefined();
    if (arg0.is_object())
        protocol_property_value_24 = TRY(arg0.as_object().get("protocol"_utf16_fly_string));

    if (!protocol_property_value_24.is_undefined()) {

    Optional<String> protocol_value_24;

    if (!protocol_property_value_24.is_undefined()) {
        if (!false || !protocol_property_value_24.is_null())
            protocol_value_24 = TRY(WebIDL::to_usv_string(vm, protocol_property_value_24));
    }

    dictionary_union_type.protocol = protocol_value_24;

    }

    auto search_property_value_25 = JS::js_undefined();
    if (arg0.is_object())
        search_property_value_25 = TRY(arg0.as_object().get("search"_utf16_fly_string));

    if (!search_property_value_25.is_undefined()) {

    Optional<String> search_value_25;

    if (!search_property_value_25.is_undefined()) {
        if (!false || !search_property_value_25.is_null())
            search_value_25 = TRY(WebIDL::to_usv_string(vm, search_property_value_25));
    }

    dictionary_union_type.search = search_value_25;

    }

    auto username_property_value_26 = JS::js_undefined();
    if (arg0.is_object())
        username_property_value_26 = TRY(arg0.as_object().get("username"_utf16_fly_string));

    if (!username_property_value_26.is_undefined()) {

    Optional<String> username_value_26;

    if (!username_property_value_26.is_undefined()) {
        if (!false || !username_property_value_26.is_null())
            username_value_26 = TRY(WebIDL::to_usv_string(vm, username_property_value_26));
    }

    dictionary_union_type.username = username_value_26;

    }

        return dictionary_union_type;
    };

    auto arg0_to_variant = [&vm, &realm, &arg0_to_dictionary](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<String, URLPatternInit>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_nullish())
            return Variant<String, URLPatternInit> { TRY(arg0_to_dictionary(arg0)) };

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

        return Variant<String, URLPatternInit> { TRY(arg0_to_dictionary(arg0)) };

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_usv_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<String, URLPatternInit> input = arg0.is_undefined() ? TRY(arg0_to_dictionary(arg0)) : TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    Optional<String> base_url;

    if (!arg1.is_undefined()) {
        if (!false || !arg1.is_null())
            base_url = TRY(WebIDL::to_usv_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->test(input, base_url); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(URLPatternPrototype::exec)
{
    WebIDL::log_trace(vm, "URLPatternPrototype::exec");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    auto arg0_to_dictionary = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<URLPatternInit> {
        // This might be unused.
        (void)realm;

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "URLPatternInit");

    URLPatternInit dictionary_union_type {};

    auto base_url_property_value_27 = JS::js_undefined();
    if (arg0.is_object())
        base_url_property_value_27 = TRY(arg0.as_object().get("baseURL"_utf16_fly_string));

    if (!base_url_property_value_27.is_undefined()) {

    Optional<String> base_url_value_27;

    if (!base_url_property_value_27.is_undefined()) {
        if (!false || !base_url_property_value_27.is_null())
            base_url_value_27 = TRY(WebIDL::to_usv_string(vm, base_url_property_value_27));
    }

    dictionary_union_type.base_url = base_url_value_27;

    }

    auto hash_property_value_28 = JS::js_undefined();
    if (arg0.is_object())
        hash_property_value_28 = TRY(arg0.as_object().get("hash"_utf16_fly_string));

    if (!hash_property_value_28.is_undefined()) {

    Optional<String> hash_value_28;

    if (!hash_property_value_28.is_undefined()) {
        if (!false || !hash_property_value_28.is_null())
            hash_value_28 = TRY(WebIDL::to_usv_string(vm, hash_property_value_28));
    }

    dictionary_union_type.hash = hash_value_28;

    }

    auto hostname_property_value_29 = JS::js_undefined();
    if (arg0.is_object())
        hostname_property_value_29 = TRY(arg0.as_object().get("hostname"_utf16_fly_string));

    if (!hostname_property_value_29.is_undefined()) {

    Optional<String> hostname_value_29;

    if (!hostname_property_value_29.is_undefined()) {
        if (!false || !hostname_property_value_29.is_null())
            hostname_value_29 = TRY(WebIDL::to_usv_string(vm, hostname_property_value_29));
    }

    dictionary_union_type.hostname = hostname_value_29;

    }

    auto password_property_value_30 = JS::js_undefined();
    if (arg0.is_object())
        password_property_value_30 = TRY(arg0.as_object().get("password"_utf16_fly_string));

    if (!password_property_value_30.is_undefined()) {

    Optional<String> password_value_30;

    if (!password_property_value_30.is_undefined()) {
        if (!false || !password_property_value_30.is_null())
            password_value_30 = TRY(WebIDL::to_usv_string(vm, password_property_value_30));
    }

    dictionary_union_type.password = password_value_30;

    }

    auto pathname_property_value_31 = JS::js_undefined();
    if (arg0.is_object())
        pathname_property_value_31 = TRY(arg0.as_object().get("pathname"_utf16_fly_string));

    if (!pathname_property_value_31.is_undefined()) {

    Optional<String> pathname_value_31;

    if (!pathname_property_value_31.is_undefined()) {
        if (!false || !pathname_property_value_31.is_null())
            pathname_value_31 = TRY(WebIDL::to_usv_string(vm, pathname_property_value_31));
    }

    dictionary_union_type.pathname = pathname_value_31;

    }

    auto port_property_value_32 = JS::js_undefined();
    if (arg0.is_object())
        port_property_value_32 = TRY(arg0.as_object().get("port"_utf16_fly_string));

    if (!port_property_value_32.is_undefined()) {

    Optional<String> port_value_32;

    if (!port_property_value_32.is_undefined()) {
        if (!false || !port_property_value_32.is_null())
            port_value_32 = TRY(WebIDL::to_usv_string(vm, port_property_value_32));
    }

    dictionary_union_type.port = port_value_32;

    }

    auto protocol_property_value_33 = JS::js_undefined();
    if (arg0.is_object())
        protocol_property_value_33 = TRY(arg0.as_object().get("protocol"_utf16_fly_string));

    if (!protocol_property_value_33.is_undefined()) {

    Optional<String> protocol_value_33;

    if (!protocol_property_value_33.is_undefined()) {
        if (!false || !protocol_property_value_33.is_null())
            protocol_value_33 = TRY(WebIDL::to_usv_string(vm, protocol_property_value_33));
    }

    dictionary_union_type.protocol = protocol_value_33;

    }

    auto search_property_value_34 = JS::js_undefined();
    if (arg0.is_object())
        search_property_value_34 = TRY(arg0.as_object().get("search"_utf16_fly_string));

    if (!search_property_value_34.is_undefined()) {

    Optional<String> search_value_34;

    if (!search_property_value_34.is_undefined()) {
        if (!false || !search_property_value_34.is_null())
            search_value_34 = TRY(WebIDL::to_usv_string(vm, search_property_value_34));
    }

    dictionary_union_type.search = search_value_34;

    }

    auto username_property_value_35 = JS::js_undefined();
    if (arg0.is_object())
        username_property_value_35 = TRY(arg0.as_object().get("username"_utf16_fly_string));

    if (!username_property_value_35.is_undefined()) {

    Optional<String> username_value_35;

    if (!username_property_value_35.is_undefined()) {
        if (!false || !username_property_value_35.is_null())
            username_value_35 = TRY(WebIDL::to_usv_string(vm, username_property_value_35));
    }

    dictionary_union_type.username = username_value_35;

    }

        return dictionary_union_type;
    };

    auto arg0_to_variant = [&vm, &realm, &arg0_to_dictionary](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<String, URLPatternInit>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_nullish())
            return Variant<String, URLPatternInit> { TRY(arg0_to_dictionary(arg0)) };

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

        return Variant<String, URLPatternInit> { TRY(arg0_to_dictionary(arg0)) };

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_usv_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<String, URLPatternInit> input = arg0.is_undefined() ? TRY(arg0_to_dictionary(arg0)) : TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    Optional<String> base_url;

    if (!arg1.is_undefined()) {
        if (!false || !arg1.is_null())
            base_url = TRY(WebIDL::to_usv_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->exec(input, base_url); }));

    if (retval.has_value()) {

    {
        auto dictionary_object0 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_hash0_value;

    {
        auto dictionary_object1 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_groups1_value;

    {
        // An IDL record<…> value D is converted to a JavaScript value as follows:
        // 1. Let result be OrdinaryObjectCreate(%Object.prototype%).
        auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

        // 2. For each key → value of D:
        for (auto const& [key, value] : retval->hash.groups) {
            // 1. Let jsKey be key converted to a JavaScript value.
            auto js_key = JS::PropertyKey { Utf16FlyString::from_utf8(key) };

            // 2. Let jsValue be value converted to a JavaScript value.

    auto js_value = value.visit(

        [&vm, &realm]([[maybe_unused]] String const& visited_union_value3) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::PrimitiveString::create(vm, visited_union_value3);

        },

        [&vm, &realm]([[maybe_unused]] Empty const& visited_union_value3) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::js_undefined();

        }

    );


            // 3. Let created be ! CreateDataProperty(result, jsKey, jsValue).
            bool created = MUST(result->create_data_property(js_key, js_value));

            // 4. Assert: created is true.
            VERIFY(created);
        }

        wrapped_groups1_value = result;
    }

        MUST(dictionary_object1->create_data_property("groups"_utf16_fly_string, wrapped_groups1_value));

        JS::Value wrapped_input1_value;

    wrapped_input1_value = JS::PrimitiveString::create(vm, retval->hash.input);

        MUST(dictionary_object1->create_data_property("input"_utf16_fly_string, wrapped_input1_value));

        wrapped_hash0_value = dictionary_object1;
    }

        MUST(dictionary_object0->create_data_property("hash"_utf16_fly_string, wrapped_hash0_value));

        JS::Value wrapped_hostname0_value;

    {
        auto dictionary_object1 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_groups1_value;

    {
        // An IDL record<…> value D is converted to a JavaScript value as follows:
        // 1. Let result be OrdinaryObjectCreate(%Object.prototype%).
        auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

        // 2. For each key → value of D:
        for (auto const& [key, value] : retval->hostname.groups) {
            // 1. Let jsKey be key converted to a JavaScript value.
            auto js_key = JS::PropertyKey { Utf16FlyString::from_utf8(key) };

            // 2. Let jsValue be value converted to a JavaScript value.

    auto js_value = value.visit(

        [&vm, &realm]([[maybe_unused]] String const& visited_union_value3) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::PrimitiveString::create(vm, visited_union_value3);

        },

        [&vm, &realm]([[maybe_unused]] Empty const& visited_union_value3) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::js_undefined();

        }

    );


            // 3. Let created be ! CreateDataProperty(result, jsKey, jsValue).
            bool created = MUST(result->create_data_property(js_key, js_value));

            // 4. Assert: created is true.
            VERIFY(created);
        }

        wrapped_groups1_value = result;
    }

        MUST(dictionary_object1->create_data_property("groups"_utf16_fly_string, wrapped_groups1_value));

        JS::Value wrapped_input1_value;

    wrapped_input1_value = JS::PrimitiveString::create(vm, retval->hostname.input);

        MUST(dictionary_object1->create_data_property("input"_utf16_fly_string, wrapped_input1_value));

        wrapped_hostname0_value = dictionary_object1;
    }

        MUST(dictionary_object0->create_data_property("hostname"_utf16_fly_string, wrapped_hostname0_value));

        JS::Value wrapped_inputs0_value;

    auto new_array1_4 = MUST(JS::Array::create(realm, 0));

    for (size_t i1 = 0; i1 < retval->inputs.size(); ++i1) {
        auto& element1 = retval->inputs.at(i1);
JS::Value wrapped_element1;

    wrapped_element1 = element1.visit(

        [&vm, &realm]([[maybe_unused]] String const& visited_union_value2) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::PrimitiveString::create(vm, visited_union_value2);

        },

        [&vm, &realm]([[maybe_unused]] URLPatternInit const& visited_union_value2) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    {
        auto dictionary_object3 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        Optional<JS::Value> wrapped_base_url3_value;

    if (visited_union_value2.base_url.has_value()) {

    wrapped_base_url3_value = JS::PrimitiveString::create(vm, const_cast<decltype(visited_union_value2.base_url)&>(visited_union_value2.base_url).release_value());

    }

        if (wrapped_base_url3_value.has_value())
            MUST(dictionary_object3->create_data_property("baseURL"_utf16_fly_string, wrapped_base_url3_value.release_value()));

        Optional<JS::Value> wrapped_hash3_value;

    if (visited_union_value2.hash.has_value()) {

    wrapped_hash3_value = JS::PrimitiveString::create(vm, const_cast<decltype(visited_union_value2.hash)&>(visited_union_value2.hash).release_value());

    }

        if (wrapped_hash3_value.has_value())
            MUST(dictionary_object3->create_data_property("hash"_utf16_fly_string, wrapped_hash3_value.release_value()));

        Optional<JS::Value> wrapped_hostname3_value;

    if (visited_union_value2.hostname.has_value()) {

    wrapped_hostname3_value = JS::PrimitiveString::create(vm, const_cast<decltype(visited_union_value2.hostname)&>(visited_union_value2.hostname).release_value());

    }

        if (wrapped_hostname3_value.has_value())
            MUST(dictionary_object3->create_data_property("hostname"_utf16_fly_string, wrapped_hostname3_value.release_value()));

        Optional<JS::Value> wrapped_password3_value;

    if (visited_union_value2.password.has_value()) {

    wrapped_password3_value = JS::PrimitiveString::create(vm, const_cast<decltype(visited_union_value2.password)&>(visited_union_value2.password).release_value());

    }

        if (wrapped_password3_value.has_value())
            MUST(dictionary_object3->create_data_property("password"_utf16_fly_string, wrapped_password3_value.release_value()));

        Optional<JS::Value> wrapped_pathname3_value;

    if (visited_union_value2.pathname.has_value()) {

    wrapped_pathname3_value = JS::PrimitiveString::create(vm, const_cast<decltype(visited_union_value2.pathname)&>(visited_union_value2.pathname).release_value());

    }

        if (wrapped_pathname3_value.has_value())
            MUST(dictionary_object3->create_data_property("pathname"_utf16_fly_string, wrapped_pathname3_value.release_value()));

        Optional<JS::Value> wrapped_port3_value;

    if (visited_union_value2.port.has_value()) {

    wrapped_port3_value = JS::PrimitiveString::create(vm, const_cast<decltype(visited_union_value2.port)&>(visited_union_value2.port).release_value());

    }

        if (wrapped_port3_value.has_value())
            MUST(dictionary_object3->create_data_property("port"_utf16_fly_string, wrapped_port3_value.release_value()));

        Optional<JS::Value> wrapped_protocol3_value;

    if (visited_union_value2.protocol.has_value()) {

    wrapped_protocol3_value = JS::PrimitiveString::create(vm, const_cast<decltype(visited_union_value2.protocol)&>(visited_union_value2.protocol).release_value());

    }

        if (wrapped_protocol3_value.has_value())
            MUST(dictionary_object3->create_data_property("protocol"_utf16_fly_string, wrapped_protocol3_value.release_value()));

        Optional<JS::Value> wrapped_search3_value;

    if (visited_union_value2.search.has_value()) {

    wrapped_search3_value = JS::PrimitiveString::create(vm, const_cast<decltype(visited_union_value2.search)&>(visited_union_value2.search).release_value());

    }

        if (wrapped_search3_value.has_value())
            MUST(dictionary_object3->create_data_property("search"_utf16_fly_string, wrapped_search3_value.release_value()));

        Optional<JS::Value> wrapped_username3_value;

    if (visited_union_value2.username.has_value()) {

    wrapped_username3_value = JS::PrimitiveString::create(vm, const_cast<decltype(visited_union_value2.username)&>(visited_union_value2.username).release_value());

    }

        if (wrapped_username3_value.has_value())
            MUST(dictionary_object3->create_data_property("username"_utf16_fly_string, wrapped_username3_value.release_value()));

        return dictionary_object3;
    }

        }

    );

        auto property_index1 = JS::PropertyKey { i1 };
        MUST(new_array1_4->create_data_property(property_index1, wrapped_element1));
    }

    wrapped_inputs0_value = new_array1_4;

        MUST(dictionary_object0->create_data_property("inputs"_utf16_fly_string, wrapped_inputs0_value));

        JS::Value wrapped_password0_value;

    {
        auto dictionary_object1 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_groups1_value;

    {
        // An IDL record<…> value D is converted to a JavaScript value as follows:
        // 1. Let result be OrdinaryObjectCreate(%Object.prototype%).
        auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

        // 2. For each key → value of D:
        for (auto const& [key, value] : retval->password.groups) {
            // 1. Let jsKey be key converted to a JavaScript value.
            auto js_key = JS::PropertyKey { Utf16FlyString::from_utf8(key) };

            // 2. Let jsValue be value converted to a JavaScript value.

    auto js_value = value.visit(

        [&vm, &realm]([[maybe_unused]] String const& visited_union_value3) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::PrimitiveString::create(vm, visited_union_value3);

        },

        [&vm, &realm]([[maybe_unused]] Empty const& visited_union_value3) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::js_undefined();

        }

    );


            // 3. Let created be ! CreateDataProperty(result, jsKey, jsValue).
            bool created = MUST(result->create_data_property(js_key, js_value));

            // 4. Assert: created is true.
            VERIFY(created);
        }

        wrapped_groups1_value = result;
    }

        MUST(dictionary_object1->create_data_property("groups"_utf16_fly_string, wrapped_groups1_value));

        JS::Value wrapped_input1_value;

    wrapped_input1_value = JS::PrimitiveString::create(vm, retval->password.input);

        MUST(dictionary_object1->create_data_property("input"_utf16_fly_string, wrapped_input1_value));

        wrapped_password0_value = dictionary_object1;
    }

        MUST(dictionary_object0->create_data_property("password"_utf16_fly_string, wrapped_password0_value));

        JS::Value wrapped_pathname0_value;

    {
        auto dictionary_object1 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_groups1_value;

    {
        // An IDL record<…> value D is converted to a JavaScript value as follows:
        // 1. Let result be OrdinaryObjectCreate(%Object.prototype%).
        auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

        // 2. For each key → value of D:
        for (auto const& [key, value] : retval->pathname.groups) {
            // 1. Let jsKey be key converted to a JavaScript value.
            auto js_key = JS::PropertyKey { Utf16FlyString::from_utf8(key) };

            // 2. Let jsValue be value converted to a JavaScript value.

    auto js_value = value.visit(

        [&vm, &realm]([[maybe_unused]] String const& visited_union_value3) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::PrimitiveString::create(vm, visited_union_value3);

        },

        [&vm, &realm]([[maybe_unused]] Empty const& visited_union_value3) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::js_undefined();

        }

    );


            // 3. Let created be ! CreateDataProperty(result, jsKey, jsValue).
            bool created = MUST(result->create_data_property(js_key, js_value));

            // 4. Assert: created is true.
            VERIFY(created);
        }

        wrapped_groups1_value = result;
    }

        MUST(dictionary_object1->create_data_property("groups"_utf16_fly_string, wrapped_groups1_value));

        JS::Value wrapped_input1_value;

    wrapped_input1_value = JS::PrimitiveString::create(vm, retval->pathname.input);

        MUST(dictionary_object1->create_data_property("input"_utf16_fly_string, wrapped_input1_value));

        wrapped_pathname0_value = dictionary_object1;
    }

        MUST(dictionary_object0->create_data_property("pathname"_utf16_fly_string, wrapped_pathname0_value));

        JS::Value wrapped_port0_value;

    {
        auto dictionary_object1 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_groups1_value;

    {
        // An IDL record<…> value D is converted to a JavaScript value as follows:
        // 1. Let result be OrdinaryObjectCreate(%Object.prototype%).
        auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

        // 2. For each key → value of D:
        for (auto const& [key, value] : retval->port.groups) {
            // 1. Let jsKey be key converted to a JavaScript value.
            auto js_key = JS::PropertyKey { Utf16FlyString::from_utf8(key) };

            // 2. Let jsValue be value converted to a JavaScript value.

    auto js_value = value.visit(

        [&vm, &realm]([[maybe_unused]] String const& visited_union_value3) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::PrimitiveString::create(vm, visited_union_value3);

        },

        [&vm, &realm]([[maybe_unused]] Empty const& visited_union_value3) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::js_undefined();

        }

    );


            // 3. Let created be ! CreateDataProperty(result, jsKey, jsValue).
            bool created = MUST(result->create_data_property(js_key, js_value));

            // 4. Assert: created is true.
            VERIFY(created);
        }

        wrapped_groups1_value = result;
    }

        MUST(dictionary_object1->create_data_property("groups"_utf16_fly_string, wrapped_groups1_value));

        JS::Value wrapped_input1_value;

    wrapped_input1_value = JS::PrimitiveString::create(vm, retval->port.input);

        MUST(dictionary_object1->create_data_property("input"_utf16_fly_string, wrapped_input1_value));

        wrapped_port0_value = dictionary_object1;
    }

        MUST(dictionary_object0->create_data_property("port"_utf16_fly_string, wrapped_port0_value));

        JS::Value wrapped_protocol0_value;

    {
        auto dictionary_object1 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_groups1_value;

    {
        // An IDL record<…> value D is converted to a JavaScript value as follows:
        // 1. Let result be OrdinaryObjectCreate(%Object.prototype%).
        auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

        // 2. For each key → value of D:
        for (auto const& [key, value] : retval->protocol.groups) {
            // 1. Let jsKey be key converted to a JavaScript value.
            auto js_key = JS::PropertyKey { Utf16FlyString::from_utf8(key) };

            // 2. Let jsValue be value converted to a JavaScript value.

    auto js_value = value.visit(

        [&vm, &realm]([[maybe_unused]] String const& visited_union_value3) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::PrimitiveString::create(vm, visited_union_value3);

        },

        [&vm, &realm]([[maybe_unused]] Empty const& visited_union_value3) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::js_undefined();

        }

    );


            // 3. Let created be ! CreateDataProperty(result, jsKey, jsValue).
            bool created = MUST(result->create_data_property(js_key, js_value));

            // 4. Assert: created is true.
            VERIFY(created);
        }

        wrapped_groups1_value = result;
    }

        MUST(dictionary_object1->create_data_property("groups"_utf16_fly_string, wrapped_groups1_value));

        JS::Value wrapped_input1_value;

    wrapped_input1_value = JS::PrimitiveString::create(vm, retval->protocol.input);

        MUST(dictionary_object1->create_data_property("input"_utf16_fly_string, wrapped_input1_value));

        wrapped_protocol0_value = dictionary_object1;
    }

        MUST(dictionary_object0->create_data_property("protocol"_utf16_fly_string, wrapped_protocol0_value));

        JS::Value wrapped_search0_value;

    {
        auto dictionary_object1 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_groups1_value;

    {
        // An IDL record<…> value D is converted to a JavaScript value as follows:
        // 1. Let result be OrdinaryObjectCreate(%Object.prototype%).
        auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

        // 2. For each key → value of D:
        for (auto const& [key, value] : retval->search.groups) {
            // 1. Let jsKey be key converted to a JavaScript value.
            auto js_key = JS::PropertyKey { Utf16FlyString::from_utf8(key) };

            // 2. Let jsValue be value converted to a JavaScript value.

    auto js_value = value.visit(

        [&vm, &realm]([[maybe_unused]] String const& visited_union_value3) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::PrimitiveString::create(vm, visited_union_value3);

        },

        [&vm, &realm]([[maybe_unused]] Empty const& visited_union_value3) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::js_undefined();

        }

    );


            // 3. Let created be ! CreateDataProperty(result, jsKey, jsValue).
            bool created = MUST(result->create_data_property(js_key, js_value));

            // 4. Assert: created is true.
            VERIFY(created);
        }

        wrapped_groups1_value = result;
    }

        MUST(dictionary_object1->create_data_property("groups"_utf16_fly_string, wrapped_groups1_value));

        JS::Value wrapped_input1_value;

    wrapped_input1_value = JS::PrimitiveString::create(vm, retval->search.input);

        MUST(dictionary_object1->create_data_property("input"_utf16_fly_string, wrapped_input1_value));

        wrapped_search0_value = dictionary_object1;
    }

        MUST(dictionary_object0->create_data_property("search"_utf16_fly_string, wrapped_search0_value));

        JS::Value wrapped_username0_value;

    {
        auto dictionary_object1 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_groups1_value;

    {
        // An IDL record<…> value D is converted to a JavaScript value as follows:
        // 1. Let result be OrdinaryObjectCreate(%Object.prototype%).
        auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

        // 2. For each key → value of D:
        for (auto const& [key, value] : retval->username.groups) {
            // 1. Let jsKey be key converted to a JavaScript value.
            auto js_key = JS::PropertyKey { Utf16FlyString::from_utf8(key) };

            // 2. Let jsValue be value converted to a JavaScript value.

    auto js_value = value.visit(

        [&vm, &realm]([[maybe_unused]] String const& visited_union_value3) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::PrimitiveString::create(vm, visited_union_value3);

        },

        [&vm, &realm]([[maybe_unused]] Empty const& visited_union_value3) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::js_undefined();

        }

    );


            // 3. Let created be ! CreateDataProperty(result, jsKey, jsValue).
            bool created = MUST(result->create_data_property(js_key, js_value));

            // 4. Assert: created is true.
            VERIFY(created);
        }

        wrapped_groups1_value = result;
    }

        MUST(dictionary_object1->create_data_property("groups"_utf16_fly_string, wrapped_groups1_value));

        JS::Value wrapped_input1_value;

    wrapped_input1_value = JS::PrimitiveString::create(vm, retval->username.input);

        MUST(dictionary_object1->create_data_property("input"_utf16_fly_string, wrapped_input1_value));

        wrapped_username0_value = dictionary_object1;
    }

        MUST(dictionary_object0->create_data_property("username"_utf16_fly_string, wrapped_username0_value));

        return dictionary_object0;
    }

    } else {
        return JS::js_null();
    }

}

} // namespace Web::Bindings
