
#include <LibIDL/Types.h>
#include <LibGC/Heap.h>
#include <LibJS/Runtime/Array.h>
#include <LibJS/Runtime/ArrayBuffer.h>
#include <LibJS/Runtime/DataView.h>
#include <LibJS/Runtime/GlobalObject.h>
#include <LibJS/Runtime/Iterator.h>
#include <LibJS/Runtime/PromiseConstructor.h>
#include <LibJS/Runtime/ValueInlines.h>
#include <LibJS/Runtime/TypedArray.h>
#include <LibWeb/Bindings/URLPatternConstructor.h>
#include <LibWeb/Bindings/URLPatternPrototype.h>
#include <LibWeb/Bindings/ExceptionOrUtils.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/HTML/WindowProxy.h>
#include <LibWeb/WebIDL/AbstractOperations.h>
#include <LibWeb/WebIDL/Buffers.h>
#include <LibWeb/WebIDL/CallbackType.h>
#include <LibWeb/WebIDL/OverloadResolution.h>
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

GC_DEFINE_ALLOCATOR(URLPatternConstructor);

URLPatternConstructor::URLPatternConstructor(JS::Realm& realm)
    : NativeFunction("URLPattern"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

URLPatternConstructor::~URLPatternConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> URLPatternConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "URLPattern");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> URLPatternConstructor::construct0([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "URLPatternConstructor::construct0");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface URLPattern:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<URLPatternPrototype>(*target_realm, "URLPattern"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "URLPattern", "2");

    auto arg0 = vm.argument(0);

    auto arg0_to_dictionary = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<URLPatternInit> {
        // This might be unused.
        (void)realm;

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "URLPatternInit");

    URLPatternInit dictionary_union_type {};

    auto base_url_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        base_url_property_value_0 = TRY(arg0.as_object().get("baseURL"_utf16_fly_string));

    if (!base_url_property_value_0.is_undefined()) {

    Optional<String> base_url_value_0;

    if (!base_url_property_value_0.is_undefined()) {
        if (!false || !base_url_property_value_0.is_null())
            base_url_value_0 = TRY(WebIDL::to_usv_string(vm, base_url_property_value_0));
    }

    dictionary_union_type.base_url = base_url_value_0;

    }

    auto hash_property_value_1 = JS::js_undefined();
    if (arg0.is_object())
        hash_property_value_1 = TRY(arg0.as_object().get("hash"_utf16_fly_string));

    if (!hash_property_value_1.is_undefined()) {

    Optional<String> hash_value_1;

    if (!hash_property_value_1.is_undefined()) {
        if (!false || !hash_property_value_1.is_null())
            hash_value_1 = TRY(WebIDL::to_usv_string(vm, hash_property_value_1));
    }

    dictionary_union_type.hash = hash_value_1;

    }

    auto hostname_property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        hostname_property_value_2 = TRY(arg0.as_object().get("hostname"_utf16_fly_string));

    if (!hostname_property_value_2.is_undefined()) {

    Optional<String> hostname_value_2;

    if (!hostname_property_value_2.is_undefined()) {
        if (!false || !hostname_property_value_2.is_null())
            hostname_value_2 = TRY(WebIDL::to_usv_string(vm, hostname_property_value_2));
    }

    dictionary_union_type.hostname = hostname_value_2;

    }

    auto password_property_value_3 = JS::js_undefined();
    if (arg0.is_object())
        password_property_value_3 = TRY(arg0.as_object().get("password"_utf16_fly_string));

    if (!password_property_value_3.is_undefined()) {

    Optional<String> password_value_3;

    if (!password_property_value_3.is_undefined()) {
        if (!false || !password_property_value_3.is_null())
            password_value_3 = TRY(WebIDL::to_usv_string(vm, password_property_value_3));
    }

    dictionary_union_type.password = password_value_3;

    }

    auto pathname_property_value_4 = JS::js_undefined();
    if (arg0.is_object())
        pathname_property_value_4 = TRY(arg0.as_object().get("pathname"_utf16_fly_string));

    if (!pathname_property_value_4.is_undefined()) {

    Optional<String> pathname_value_4;

    if (!pathname_property_value_4.is_undefined()) {
        if (!false || !pathname_property_value_4.is_null())
            pathname_value_4 = TRY(WebIDL::to_usv_string(vm, pathname_property_value_4));
    }

    dictionary_union_type.pathname = pathname_value_4;

    }

    auto port_property_value_5 = JS::js_undefined();
    if (arg0.is_object())
        port_property_value_5 = TRY(arg0.as_object().get("port"_utf16_fly_string));

    if (!port_property_value_5.is_undefined()) {

    Optional<String> port_value_5;

    if (!port_property_value_5.is_undefined()) {
        if (!false || !port_property_value_5.is_null())
            port_value_5 = TRY(WebIDL::to_usv_string(vm, port_property_value_5));
    }

    dictionary_union_type.port = port_value_5;

    }

    auto protocol_property_value_6 = JS::js_undefined();
    if (arg0.is_object())
        protocol_property_value_6 = TRY(arg0.as_object().get("protocol"_utf16_fly_string));

    if (!protocol_property_value_6.is_undefined()) {

    Optional<String> protocol_value_6;

    if (!protocol_property_value_6.is_undefined()) {
        if (!false || !protocol_property_value_6.is_null())
            protocol_value_6 = TRY(WebIDL::to_usv_string(vm, protocol_property_value_6));
    }

    dictionary_union_type.protocol = protocol_value_6;

    }

    auto search_property_value_7 = JS::js_undefined();
    if (arg0.is_object())
        search_property_value_7 = TRY(arg0.as_object().get("search"_utf16_fly_string));

    if (!search_property_value_7.is_undefined()) {

    Optional<String> search_value_7;

    if (!search_property_value_7.is_undefined()) {
        if (!false || !search_property_value_7.is_null())
            search_value_7 = TRY(WebIDL::to_usv_string(vm, search_property_value_7));
    }

    dictionary_union_type.search = search_value_7;

    }

    auto username_property_value_8 = JS::js_undefined();
    if (arg0.is_object())
        username_property_value_8 = TRY(arg0.as_object().get("username"_utf16_fly_string));

    if (!username_property_value_8.is_undefined()) {

    Optional<String> username_value_8;

    if (!username_property_value_8.is_undefined()) {
        if (!false || !username_property_value_8.is_null())
            username_value_8 = TRY(WebIDL::to_usv_string(vm, username_property_value_8));
    }

    dictionary_union_type.username = username_value_8;

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

    Variant<String, URLPatternInit> input = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    String base_url;
    if (!false || !arg1.is_null()) {
        base_url = TRY(WebIDL::to_usv_string(vm, arg1));
    }

    auto arg2 = vm.argument(2);

    if (!arg2.is_nullish() && !arg2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "URLPatternOptions");

    URLPatternOptions options {};

    auto ignore_case_property_value_0 = JS::js_undefined();
    if (arg2.is_object())
        ignore_case_property_value_0 = TRY(arg2.as_object().get("ignoreCase"_utf16_fly_string));

    bool ignore_case_value_0;

    if (!ignore_case_property_value_0.is_undefined())

    ignore_case_value_0 = ignore_case_property_value_0.to_boolean();

    else
        ignore_case_value_0 = static_cast<bool>(false);

    options.ignore_case = ignore_case_value_0;

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return URLPattern::URLPattern::construct_impl(realm, input, base_url, options); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface URLPattern" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within URLPattern::URLPattern::construct_impl().
    //  8. Let interfaces be the inclusive inherited interfaces of interface.
    //  9. For every interface ancestor interface in interfaces:
    //    9.1. Let unforgeables be the value of the [[Unforgeables]] slot of the interface object of ancestor interface in realm.
    //    9.2. Let keys be ! unforgeables.[[OwnPropertyKeys]]().
    //    9.3. For each element key of keys:
    //      9.3.1. Let descriptor be ! unforgeables.[[GetOwnProperty]](key).
    //      9.3.2. Perform ! DefinePropertyOrThrow(instance, key, descriptor).
    //  10. If interface is declared with the [Global] extended attribute, then:
    //    10.1. Define the regular operations of interface on instance, given realm.
    //    10.2. Define the regular attributes of interface on instance, given realm.
    //    10.3. Define the iteration methods of interface on instance given realm.
    //    10.4. Define the asynchronous iteration methods of interface on instance given realm.
    //    10.5. Define the global property references on instance, given realm.
    //    10.6. Set instance.[[SetPrototypeOf]] as defined in § 3.8.1 [[SetPrototypeOf]].
    //  11. Otherwise, if interfaces contains an interface which supports indexed properties, named properties, or both:
    //    11.1. Set instance.[[GetOwnProperty]] as defined in § 3.9.1 [[GetOwnProperty]].
    //    11.2. Set instance.[[Set]] as defined in § 3.9.2 [[Set]].
    //    11.3. Set instance.[[DefineOwnProperty]] as defined in § 3.9.3 [[DefineOwnProperty]].
    //    11.4. Set instance.[[Delete]] as defined in § 3.9.4 [[Delete]].
    //    11.5. Set instance.[[PreventExtensions]] as defined in § 3.9.5 [[PreventExtensions]].
    //    11.6. Set instance.[[OwnPropertyKeys]] as defined in § 3.9.6 [[OwnPropertyKeys]].

    return *impl;
}

JS::ThrowCompletionOr<GC::Ref<JS::Object>> URLPatternConstructor::construct1([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "URLPatternConstructor::construct1");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface URLPattern:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<URLPatternPrototype>(*target_realm, "URLPattern"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    auto arg0 = vm.argument(0);

    auto arg0_to_dictionary = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<URLPatternInit> {
        // This might be unused.
        (void)realm;

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "URLPatternInit");

    URLPatternInit dictionary_union_type {};

    auto base_url_property_value_9 = JS::js_undefined();
    if (arg0.is_object())
        base_url_property_value_9 = TRY(arg0.as_object().get("baseURL"_utf16_fly_string));

    if (!base_url_property_value_9.is_undefined()) {

    Optional<String> base_url_value_9;

    if (!base_url_property_value_9.is_undefined()) {
        if (!false || !base_url_property_value_9.is_null())
            base_url_value_9 = TRY(WebIDL::to_usv_string(vm, base_url_property_value_9));
    }

    dictionary_union_type.base_url = base_url_value_9;

    }

    auto hash_property_value_10 = JS::js_undefined();
    if (arg0.is_object())
        hash_property_value_10 = TRY(arg0.as_object().get("hash"_utf16_fly_string));

    if (!hash_property_value_10.is_undefined()) {

    Optional<String> hash_value_10;

    if (!hash_property_value_10.is_undefined()) {
        if (!false || !hash_property_value_10.is_null())
            hash_value_10 = TRY(WebIDL::to_usv_string(vm, hash_property_value_10));
    }

    dictionary_union_type.hash = hash_value_10;

    }

    auto hostname_property_value_11 = JS::js_undefined();
    if (arg0.is_object())
        hostname_property_value_11 = TRY(arg0.as_object().get("hostname"_utf16_fly_string));

    if (!hostname_property_value_11.is_undefined()) {

    Optional<String> hostname_value_11;

    if (!hostname_property_value_11.is_undefined()) {
        if (!false || !hostname_property_value_11.is_null())
            hostname_value_11 = TRY(WebIDL::to_usv_string(vm, hostname_property_value_11));
    }

    dictionary_union_type.hostname = hostname_value_11;

    }

    auto password_property_value_12 = JS::js_undefined();
    if (arg0.is_object())
        password_property_value_12 = TRY(arg0.as_object().get("password"_utf16_fly_string));

    if (!password_property_value_12.is_undefined()) {

    Optional<String> password_value_12;

    if (!password_property_value_12.is_undefined()) {
        if (!false || !password_property_value_12.is_null())
            password_value_12 = TRY(WebIDL::to_usv_string(vm, password_property_value_12));
    }

    dictionary_union_type.password = password_value_12;

    }

    auto pathname_property_value_13 = JS::js_undefined();
    if (arg0.is_object())
        pathname_property_value_13 = TRY(arg0.as_object().get("pathname"_utf16_fly_string));

    if (!pathname_property_value_13.is_undefined()) {

    Optional<String> pathname_value_13;

    if (!pathname_property_value_13.is_undefined()) {
        if (!false || !pathname_property_value_13.is_null())
            pathname_value_13 = TRY(WebIDL::to_usv_string(vm, pathname_property_value_13));
    }

    dictionary_union_type.pathname = pathname_value_13;

    }

    auto port_property_value_14 = JS::js_undefined();
    if (arg0.is_object())
        port_property_value_14 = TRY(arg0.as_object().get("port"_utf16_fly_string));

    if (!port_property_value_14.is_undefined()) {

    Optional<String> port_value_14;

    if (!port_property_value_14.is_undefined()) {
        if (!false || !port_property_value_14.is_null())
            port_value_14 = TRY(WebIDL::to_usv_string(vm, port_property_value_14));
    }

    dictionary_union_type.port = port_value_14;

    }

    auto protocol_property_value_15 = JS::js_undefined();
    if (arg0.is_object())
        protocol_property_value_15 = TRY(arg0.as_object().get("protocol"_utf16_fly_string));

    if (!protocol_property_value_15.is_undefined()) {

    Optional<String> protocol_value_15;

    if (!protocol_property_value_15.is_undefined()) {
        if (!false || !protocol_property_value_15.is_null())
            protocol_value_15 = TRY(WebIDL::to_usv_string(vm, protocol_property_value_15));
    }

    dictionary_union_type.protocol = protocol_value_15;

    }

    auto search_property_value_16 = JS::js_undefined();
    if (arg0.is_object())
        search_property_value_16 = TRY(arg0.as_object().get("search"_utf16_fly_string));

    if (!search_property_value_16.is_undefined()) {

    Optional<String> search_value_16;

    if (!search_property_value_16.is_undefined()) {
        if (!false || !search_property_value_16.is_null())
            search_value_16 = TRY(WebIDL::to_usv_string(vm, search_property_value_16));
    }

    dictionary_union_type.search = search_value_16;

    }

    auto username_property_value_17 = JS::js_undefined();
    if (arg0.is_object())
        username_property_value_17 = TRY(arg0.as_object().get("username"_utf16_fly_string));

    if (!username_property_value_17.is_undefined()) {

    Optional<String> username_value_17;

    if (!username_property_value_17.is_undefined()) {
        if (!false || !username_property_value_17.is_null())
            username_value_17 = TRY(WebIDL::to_usv_string(vm, username_property_value_17));
    }

    dictionary_union_type.username = username_value_17;

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

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "URLPatternOptions");

    URLPatternOptions options {};

    auto ignore_case_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        ignore_case_property_value_1 = TRY(arg1.as_object().get("ignoreCase"_utf16_fly_string));

    bool ignore_case_value_1;

    if (!ignore_case_property_value_1.is_undefined())

    ignore_case_value_1 = ignore_case_property_value_1.to_boolean();

    else
        ignore_case_value_1 = static_cast<bool>(false);

    options.ignore_case = ignore_case_value_1;

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return URLPattern::URLPattern::construct_impl(realm, input, options); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface URLPattern" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within URLPattern::URLPattern::construct_impl().
    //  8. Let interfaces be the inclusive inherited interfaces of interface.
    //  9. For every interface ancestor interface in interfaces:
    //    9.1. Let unforgeables be the value of the [[Unforgeables]] slot of the interface object of ancestor interface in realm.
    //    9.2. Let keys be ! unforgeables.[[OwnPropertyKeys]]().
    //    9.3. For each element key of keys:
    //      9.3.1. Let descriptor be ! unforgeables.[[GetOwnProperty]](key).
    //      9.3.2. Perform ! DefinePropertyOrThrow(instance, key, descriptor).
    //  10. If interface is declared with the [Global] extended attribute, then:
    //    10.1. Define the regular operations of interface on instance, given realm.
    //    10.2. Define the regular attributes of interface on instance, given realm.
    //    10.3. Define the iteration methods of interface on instance given realm.
    //    10.4. Define the asynchronous iteration methods of interface on instance given realm.
    //    10.5. Define the global property references on instance, given realm.
    //    10.6. Set instance.[[SetPrototypeOf]] as defined in § 3.8.1 [[SetPrototypeOf]].
    //  11. Otherwise, if interfaces contains an interface which supports indexed properties, named properties, or both:
    //    11.1. Set instance.[[GetOwnProperty]] as defined in § 3.9.1 [[GetOwnProperty]].
    //    11.2. Set instance.[[Set]] as defined in § 3.9.2 [[Set]].
    //    11.3. Set instance.[[DefineOwnProperty]] as defined in § 3.9.3 [[DefineOwnProperty]].
    //    11.4. Set instance.[[Delete]] as defined in § 3.9.4 [[Delete]].
    //    11.5. Set instance.[[PreventExtensions]] as defined in § 3.9.5 [[PreventExtensions]].
    //    11.6. Set instance.[[OwnPropertyKeys]] as defined in § 3.9.6 [[OwnPropertyKeys]].

    return *impl;
}

JS::ThrowCompletionOr<GC::Ref<JS::Object>> URLPatternConstructor::construct(JS::FunctionObject& new_target)
{
    auto& vm = this->vm();
    WebIDL::log_trace(vm, "URLPatternConstructor::construct");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(3, vm.argument_count())) {

    case 0:
        chosen_overload_callable_id = 1;
        break;


    case 1:
        chosen_overload_callable_id = 1;
        break;


    case 2: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::UnionType>("", false, Vector<NonnullRefPtr<IDL::Type const>> {make_ref_counted<IDL::Type>("USVString", false), make_ref_counted<IDL::Type>("URLPatternInit", false)}), make_ref_counted<IDL::Type>("USVString", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Required});
        overloads.empend(1, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::UnionType>("", false, Vector<NonnullRefPtr<IDL::Type const>> {make_ref_counted<IDL::Type>("USVString", false), make_ref_counted<IDL::Type>("URLPatternInit", false)}), make_ref_counted<IDL::Type>("URLPatternOptions", false)}, Vector<IDL::Optionality> { IDL::Optionality::Optional, IDL::Optionality::Optional});

        effective_overload_set.emplace(move(overloads), 1);
        break;
    }

    case 3:
        chosen_overload_callable_id = 0;
        break;


    }

    Vector<StringView> dictionary_types {
    "URLPatternOptions"sv,
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return construct0(new_target);

    case 1:

        return construct1(new_target);

    default:
        VERIFY_NOT_REACHED();
    }
}


void URLPatternConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    define_direct_property(vm.names.length, JS::Value(0), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "URLPattern"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<URLPatternPrototype>(realm, "URLPattern"_fly_string), 0);


}

} // namespace Web::Bindings
