
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
#include <LibWeb/Bindings/RequestConstructor.h>
#include <LibWeb/Bindings/RequestPrototype.h>
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

GC_DEFINE_ALLOCATOR(RequestConstructor);

RequestConstructor::RequestConstructor(JS::Realm& realm)
    : NativeFunction("Request"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

RequestConstructor::~RequestConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> RequestConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "Request");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> RequestConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "RequestConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface Request:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<RequestPrototype>(*target_realm, "Request"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "Request");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<Request>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<Request>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_usv_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<GC::Root<Request>, String> input = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "RequestInit");

    RequestInit init {};

    auto body_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        body_property_value_0 = TRY(arg1.as_object().get("body"_utf16_fly_string));

    if (!body_property_value_0.is_undefined()) {

    auto body_property_value_0_to_variant = [&vm, &realm](JS::Value body_property_value_0) -> JS::ThrowCompletionOr<Variant<GC::Root<ReadableStream>, GC::Root<Blob>, GC::Root<WebIDL::BufferSource>, GC::Root<FormData>, GC::Root<URLSearchParams>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (body_property_value_0.is_object()) {
            [[maybe_unused]] auto& body_property_value_0_object = body_property_value_0.as_object();

            if (is<PlatformObject>(body_property_value_0_object)) {

                if (auto* body_property_value_0_result = as_if<ReadableStream>(body_property_value_0_object))
                    return GC::make_root(*body_property_value_0_result);

                if (auto* body_property_value_0_result = as_if<Blob>(body_property_value_0_object))
                    return GC::make_root(*body_property_value_0_result);

                if (auto* body_property_value_0_result = as_if<FormData>(body_property_value_0_object))
                    return GC::make_root(*body_property_value_0_result);

                if (auto* body_property_value_0_result = as_if<URLSearchParams>(body_property_value_0_object))
                    return GC::make_root(*body_property_value_0_result);

            }

            if (is<JS::ArrayBuffer>(body_property_value_0_object) || is<JS::DataView>(body_property_value_0_object) || is<JS::TypedArrayBase>(body_property_value_0_object)) {
                GC::Ref<WebIDL::BufferSource> source_object = realm.create<WebIDL::BufferSource>(body_property_value_0_object);
                return GC::make_root(source_object);
            }

        }

    String body_property_value_0_string;
    if (!false || !body_property_value_0.is_null()) {
        body_property_value_0_string = TRY(WebIDL::to_usv_string(vm, body_property_value_0));
    }

        return { body_property_value_0_string };

    };

    Optional<Variant<GC::Root<ReadableStream>, GC::Root<Blob>, GC::Root<WebIDL::BufferSource>, GC::Root<FormData>, GC::Root<URLSearchParams>, String>> body_value_0;
    if (!body_property_value_0.is_nullish())
        body_value_0 = TRY(body_property_value_0_to_variant(body_property_value_0));

    init.body = body_value_0;

    }

    auto cache_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        cache_property_value_1 = TRY(arg1.as_object().get("cache"_utf16_fly_string));

    if (!cache_property_value_1.is_undefined()) {

    RequestCache cache_value_1 { RequestCache::Default };

    if (!cache_property_value_1.is_undefined()) {

    auto cache_property_value_1_string = TRY(cache_property_value_1.to_string(vm));

    if (cache_property_value_1_string == "default"sv)
        cache_value_1 = RequestCache::Default;

    else if (cache_property_value_1_string == "no-store"sv)
        cache_value_1 = RequestCache::NoStore;

    else if (cache_property_value_1_string == "reload"sv)
        cache_value_1 = RequestCache::Reload;

    else if (cache_property_value_1_string == "no-cache"sv)
        cache_value_1 = RequestCache::NoCache;

    else if (cache_property_value_1_string == "force-cache"sv)
        cache_value_1 = RequestCache::ForceCache;

    else if (cache_property_value_1_string == "only-if-cached"sv)
        cache_value_1 = RequestCache::OnlyIfCached;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, cache_property_value_1_string, "RequestCache");

    }

    init.cache = cache_value_1;

    }

    auto credentials_property_value_2 = JS::js_undefined();
    if (arg1.is_object())
        credentials_property_value_2 = TRY(arg1.as_object().get("credentials"_utf16_fly_string));

    if (!credentials_property_value_2.is_undefined()) {

    RequestCredentials credentials_value_2 { RequestCredentials::Omit };

    if (!credentials_property_value_2.is_undefined()) {

    auto credentials_property_value_2_string = TRY(credentials_property_value_2.to_string(vm));

    if (credentials_property_value_2_string == "omit"sv)
        credentials_value_2 = RequestCredentials::Omit;

    else if (credentials_property_value_2_string == "same-origin"sv)
        credentials_value_2 = RequestCredentials::SameOrigin;

    else if (credentials_property_value_2_string == "include"sv)
        credentials_value_2 = RequestCredentials::Include;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, credentials_property_value_2_string, "RequestCredentials");

    }

    init.credentials = credentials_value_2;

    }

    auto duplex_property_value_3 = JS::js_undefined();
    if (arg1.is_object())
        duplex_property_value_3 = TRY(arg1.as_object().get("duplex"_utf16_fly_string));

    if (!duplex_property_value_3.is_undefined()) {

    RequestDuplex duplex_value_3 { RequestDuplex::Half };

    if (!duplex_property_value_3.is_undefined()) {

    auto duplex_property_value_3_string = TRY(duplex_property_value_3.to_string(vm));

    if (duplex_property_value_3_string == "half"sv)
        duplex_value_3 = RequestDuplex::Half;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, duplex_property_value_3_string, "RequestDuplex");

    }

    init.duplex = duplex_value_3;

    }

    auto headers_property_value_4 = JS::js_undefined();
    if (arg1.is_object())
        headers_property_value_4 = TRY(arg1.as_object().get("headers"_utf16_fly_string));

    if (!headers_property_value_4.is_undefined()) {

    auto headers_property_value_4_to_variant = [&vm, &realm](JS::Value headers_property_value_4) -> JS::ThrowCompletionOr<Variant<Vector<Vector<String>>, OrderedHashMap<String, String>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (headers_property_value_4.is_object()) {
            [[maybe_unused]] auto& headers_property_value_4_object = headers_property_value_4.as_object();

        auto method = TRY(headers_property_value_4.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto headers_property_value_4_iterator1 = TRY(JS::get_iterator_from_method(vm, headers_property_value_4, *method));

    Vector<Vector<String>> headers_value_4;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, headers_property_value_4_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, next_item1);

    auto next_item1_iterator_method1 = TRY(next_item1.get_method(vm, vm.well_known_symbol_iterator()));
    if (!next_item1_iterator_method1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, next_item1);

    auto next_item1_iterator2 = TRY(JS::get_iterator_from_method(vm, next_item1, *next_item1_iterator_method1));

    Vector<String> sequence_item1;

    for (;;) {
        auto next2 = TRY(JS::iterator_step(vm, next_item1_iterator2));
        if (!next2.has<JS::IterationResult>())
            break;

        auto next_item2 = TRY(next2.get<JS::IterationResult>().value);

    String sequence_item2;
    if (!false || !next_item2.is_null()) {
        sequence_item2 = TRY(WebIDL::to_byte_string(vm, next_item2));
    }

    sequence_item1.append(sequence_item2);
    }

    headers_value_4.append(sequence_item1);
    }


            return headers_value_4;
        }

    OrderedHashMap<String, String> record_union_type;

    auto record_keys1 = TRY(headers_property_value_4_object.internal_own_property_keys());

    for (auto& key1 : record_keys1) {
        auto property_key1 = MUST(JS::PropertyKey::from_value(vm, key1));

        auto descriptor1 = TRY(headers_property_value_4_object.internal_get_own_property(property_key1));

        if (!descriptor1.has_value() || !descriptor1->enumerable.has_value() || !descriptor1->enumerable.value())
            continue;

    String typed_key1;
    if (!false || !key1.is_null()) {
        typed_key1 = TRY(WebIDL::to_byte_string(vm, key1));
    }

        auto value1 = TRY(headers_property_value_4_object.get(property_key1));

    String typed_value1;
    if (!false || !value1.is_null()) {
        typed_value1 = TRY(WebIDL::to_byte_string(vm, value1));
    }

        record_union_type.set(typed_key1, typed_value1);
    }

        return record_union_type;

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Optional<Variant<Vector<Vector<String>>, OrderedHashMap<String, String>>> headers_value_4;
    if (!headers_property_value_4.is_undefined())
        headers_value_4 = TRY(headers_property_value_4_to_variant(headers_property_value_4));

    init.headers = headers_value_4;

    }

    auto integrity_property_value_5 = JS::js_undefined();
    if (arg1.is_object())
        integrity_property_value_5 = TRY(arg1.as_object().get("integrity"_utf16_fly_string));

    if (!integrity_property_value_5.is_undefined()) {

    Optional<String> integrity_value_5;

    if (!integrity_property_value_5.is_undefined()) {
        if (!false || !integrity_property_value_5.is_null())
            integrity_value_5 = TRY(WebIDL::to_string(vm, integrity_property_value_5));
    }

    init.integrity = integrity_value_5;

    }

    auto keepalive_property_value_6 = JS::js_undefined();
    if (arg1.is_object())
        keepalive_property_value_6 = TRY(arg1.as_object().get("keepalive"_utf16_fly_string));

    if (!keepalive_property_value_6.is_undefined()) {

    Optional<bool> keepalive_value_6;

    if (!keepalive_property_value_6.is_undefined())

    keepalive_value_6 = keepalive_property_value_6.to_boolean();

    init.keepalive = keepalive_value_6;

    }

    auto method_property_value_7 = JS::js_undefined();
    if (arg1.is_object())
        method_property_value_7 = TRY(arg1.as_object().get("method"_utf16_fly_string));

    if (!method_property_value_7.is_undefined()) {

    Optional<String> method_value_7;

    if (!method_property_value_7.is_undefined()) {
        if (!false || !method_property_value_7.is_null())
            method_value_7 = TRY(WebIDL::to_byte_string(vm, method_property_value_7));
    }

    init.method = method_value_7;

    }

    auto mode_property_value_8 = JS::js_undefined();
    if (arg1.is_object())
        mode_property_value_8 = TRY(arg1.as_object().get("mode"_utf16_fly_string));

    if (!mode_property_value_8.is_undefined()) {

    RequestMode mode_value_8 { RequestMode::Navigate };

    if (!mode_property_value_8.is_undefined()) {

    auto mode_property_value_8_string = TRY(mode_property_value_8.to_string(vm));

    if (mode_property_value_8_string == "navigate"sv)
        mode_value_8 = RequestMode::Navigate;

    else if (mode_property_value_8_string == "same-origin"sv)
        mode_value_8 = RequestMode::SameOrigin;

    else if (mode_property_value_8_string == "no-cors"sv)
        mode_value_8 = RequestMode::NoCors;

    else if (mode_property_value_8_string == "cors"sv)
        mode_value_8 = RequestMode::Cors;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, mode_property_value_8_string, "RequestMode");

    }

    init.mode = mode_value_8;

    }

    auto priority_property_value_9 = JS::js_undefined();
    if (arg1.is_object())
        priority_property_value_9 = TRY(arg1.as_object().get("priority"_utf16_fly_string));

    if (!priority_property_value_9.is_undefined()) {

    RequestPriority priority_value_9 { RequestPriority::High };

    if (!priority_property_value_9.is_undefined()) {

    auto priority_property_value_9_string = TRY(priority_property_value_9.to_string(vm));

    if (priority_property_value_9_string == "high"sv)
        priority_value_9 = RequestPriority::High;

    else if (priority_property_value_9_string == "low"sv)
        priority_value_9 = RequestPriority::Low;

    else if (priority_property_value_9_string == "auto"sv)
        priority_value_9 = RequestPriority::Auto;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, priority_property_value_9_string, "RequestPriority");

    }

    init.priority = priority_value_9;

    }

    auto redirect_property_value_10 = JS::js_undefined();
    if (arg1.is_object())
        redirect_property_value_10 = TRY(arg1.as_object().get("redirect"_utf16_fly_string));

    if (!redirect_property_value_10.is_undefined()) {

    RequestRedirect redirect_value_10 { RequestRedirect::Follow };

    if (!redirect_property_value_10.is_undefined()) {

    auto redirect_property_value_10_string = TRY(redirect_property_value_10.to_string(vm));

    if (redirect_property_value_10_string == "follow"sv)
        redirect_value_10 = RequestRedirect::Follow;

    else if (redirect_property_value_10_string == "error"sv)
        redirect_value_10 = RequestRedirect::Error;

    else if (redirect_property_value_10_string == "manual"sv)
        redirect_value_10 = RequestRedirect::Manual;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, redirect_property_value_10_string, "RequestRedirect");

    }

    init.redirect = redirect_value_10;

    }

    auto referrer_property_value_11 = JS::js_undefined();
    if (arg1.is_object())
        referrer_property_value_11 = TRY(arg1.as_object().get("referrer"_utf16_fly_string));

    if (!referrer_property_value_11.is_undefined()) {

    Optional<String> referrer_value_11;

    if (!referrer_property_value_11.is_undefined()) {
        if (!false || !referrer_property_value_11.is_null())
            referrer_value_11 = TRY(WebIDL::to_usv_string(vm, referrer_property_value_11));
    }

    init.referrer = referrer_value_11;

    }

    auto referrer_policy_property_value_12 = JS::js_undefined();
    if (arg1.is_object())
        referrer_policy_property_value_12 = TRY(arg1.as_object().get("referrerPolicy"_utf16_fly_string));

    if (!referrer_policy_property_value_12.is_undefined()) {

    ReferrerPolicy referrer_policy_value_12 { ReferrerPolicy::NoReferrer };

    if (!referrer_policy_property_value_12.is_undefined()) {

    auto referrer_policy_property_value_12_string = TRY(referrer_policy_property_value_12.to_string(vm));

    if (referrer_policy_property_value_12_string == ""sv)
        referrer_policy_value_12 = ReferrerPolicy::Empty;

    else if (referrer_policy_property_value_12_string == "no-referrer"sv)
        referrer_policy_value_12 = ReferrerPolicy::NoReferrer;

    else if (referrer_policy_property_value_12_string == "no-referrer-when-downgrade"sv)
        referrer_policy_value_12 = ReferrerPolicy::NoReferrerWhenDowngrade;

    else if (referrer_policy_property_value_12_string == "same-origin"sv)
        referrer_policy_value_12 = ReferrerPolicy::SameOrigin;

    else if (referrer_policy_property_value_12_string == "origin"sv)
        referrer_policy_value_12 = ReferrerPolicy::Origin;

    else if (referrer_policy_property_value_12_string == "strict-origin"sv)
        referrer_policy_value_12 = ReferrerPolicy::StrictOrigin;

    else if (referrer_policy_property_value_12_string == "origin-when-cross-origin"sv)
        referrer_policy_value_12 = ReferrerPolicy::OriginWhenCrossOrigin;

    else if (referrer_policy_property_value_12_string == "strict-origin-when-cross-origin"sv)
        referrer_policy_value_12 = ReferrerPolicy::StrictOriginWhenCrossOrigin;

    else if (referrer_policy_property_value_12_string == "unsafe-url"sv)
        referrer_policy_value_12 = ReferrerPolicy::UnsafeUrl;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, referrer_policy_property_value_12_string, "ReferrerPolicy");

    }

    init.referrer_policy = referrer_policy_value_12;

    }

    auto signal_property_value_13 = JS::js_undefined();
    if (arg1.is_object())
        signal_property_value_13 = TRY(arg1.as_object().get("signal"_utf16_fly_string));

    if (!signal_property_value_13.is_undefined()) {

    GC::Ptr<AbortSignal> signal_value_13;

    if (!signal_property_value_13.is_nullish()) {
        if (!signal_property_value_13.is_object() || !is<AbortSignal>(signal_property_value_13.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AbortSignal");

        signal_value_13 = &static_cast<AbortSignal&>(signal_property_value_13.as_object());
    }

    init.signal = signal_value_13;

    }

    auto window_property_value_14 = JS::js_undefined();
    if (arg1.is_object())
        window_property_value_14 = TRY(arg1.as_object().get("window"_utf16_fly_string));

    if (!window_property_value_14.is_undefined()) {

    JS::Value window_value_14 = JS::js_undefined();
    if (!window_property_value_14.is_undefined())
        window_value_14 = window_property_value_14;

    init.window = window_value_14;

    }

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return Fetch::Request::construct_impl(realm, input, init); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface Request" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within Fetch::Request::construct_impl().
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


void RequestConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    define_direct_property(vm.names.length, JS::Value(1), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "Request"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<RequestPrototype>(realm, "Request"_fly_string), 0);


}

} // namespace Web::Bindings
