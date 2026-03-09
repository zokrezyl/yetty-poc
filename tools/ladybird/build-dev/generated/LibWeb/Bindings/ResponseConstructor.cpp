
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
#include <LibWeb/Bindings/ResponseConstructor.h>
#include <LibWeb/Bindings/ResponsePrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Response.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Headers.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XHR/FormData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOMURL/URLSearchParams.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/QueuingStrategy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamBYOBReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamDefaultReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/File.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStreamDefaultWriter.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

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

GC_DEFINE_ALLOCATOR(ResponseConstructor);

ResponseConstructor::ResponseConstructor(JS::Realm& realm)
    : NativeFunction("Response"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

ResponseConstructor::~ResponseConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> ResponseConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "Response");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> ResponseConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "ResponseConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface Response:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<ResponsePrototype>(*target_realm, "Response"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<ReadableStream>, GC::Root<Blob>, GC::Root<WebIDL::BufferSource>, GC::Root<FormData>, GC::Root<URLSearchParams>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<ReadableStream>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<Blob>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<FormData>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<URLSearchParams>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

            if (is<JS::ArrayBuffer>(arg0_object) || is<JS::DataView>(arg0_object) || is<JS::TypedArrayBase>(arg0_object)) {
                GC::Ref<WebIDL::BufferSource> source_object = realm.create<WebIDL::BufferSource>(arg0_object);
                return GC::make_root(source_object);
            }

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_usv_string(vm, arg0));
    }

        return { arg0_string };

    };

    Optional<Variant<GC::Root<ReadableStream>, GC::Root<Blob>, GC::Root<WebIDL::BufferSource>, GC::Root<FormData>, GC::Root<URLSearchParams>, String>> body;
    if (!arg0.is_nullish())
        body = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ResponseInit");

    ResponseInit init {};

    auto headers_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        headers_property_value_0 = TRY(arg1.as_object().get("headers"_utf16_fly_string));

    if (!headers_property_value_0.is_undefined()) {

    auto headers_property_value_0_to_variant = [&vm, &realm](JS::Value headers_property_value_0) -> JS::ThrowCompletionOr<Variant<Vector<Vector<String>>, OrderedHashMap<String, String>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (headers_property_value_0.is_object()) {
            [[maybe_unused]] auto& headers_property_value_0_object = headers_property_value_0.as_object();

        auto method = TRY(headers_property_value_0.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto headers_property_value_0_iterator1 = TRY(JS::get_iterator_from_method(vm, headers_property_value_0, *method));

    Vector<Vector<String>> headers_value_0;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, headers_property_value_0_iterator1));
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

    headers_value_0.append(sequence_item1);
    }


            return headers_value_0;
        }

    OrderedHashMap<String, String> record_union_type;

    auto record_keys1 = TRY(headers_property_value_0_object.internal_own_property_keys());

    for (auto& key1 : record_keys1) {
        auto property_key1 = MUST(JS::PropertyKey::from_value(vm, key1));

        auto descriptor1 = TRY(headers_property_value_0_object.internal_get_own_property(property_key1));

        if (!descriptor1.has_value() || !descriptor1->enumerable.has_value() || !descriptor1->enumerable.value())
            continue;

    String typed_key1;
    if (!false || !key1.is_null()) {
        typed_key1 = TRY(WebIDL::to_byte_string(vm, key1));
    }

        auto value1 = TRY(headers_property_value_0_object.get(property_key1));

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

    Optional<Variant<Vector<Vector<String>>, OrderedHashMap<String, String>>> headers_value_0;
    if (!headers_property_value_0.is_undefined())
        headers_value_0 = TRY(headers_property_value_0_to_variant(headers_property_value_0));

    init.headers = headers_value_0;

    }

    auto status_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        status_property_value_1 = TRY(arg1.as_object().get("status"_utf16_fly_string));

    WebIDL::UnsignedShort status_value_1;

    if (!status_property_value_1.is_undefined())

    status_value_1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, status_property_value_1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        status_value_1 = static_cast<WebIDL::UnsignedShort>(200);

    init.status = status_value_1;

    auto status_text_property_value_2 = JS::js_undefined();
    if (arg1.is_object())
        status_text_property_value_2 = TRY(arg1.as_object().get("statusText"_utf16_fly_string));

    String status_text_value_2;

    if (!status_text_property_value_2.is_undefined()) {
        if (!false || !status_text_property_value_2.is_null())
            status_text_value_2 = TRY(WebIDL::to_byte_string(vm, status_text_property_value_2));
    } else {
        status_text_value_2 = ""_string;
    }

    init.status_text = status_text_value_2;

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return Fetch::Response::construct_impl(realm, body, init); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface Response" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within Fetch::Response::construct_impl().
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


void ResponseConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    define_direct_property(vm.names.length, JS::Value(0), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "Response"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<ResponsePrototype>(realm, "Response"_fly_string), 0);


    define_native_function(realm, "error"_utf16_fly_string, error, 0, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "redirect"_utf16_fly_string, redirect, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "json"_utf16_fly_string, json, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

}

JS_DEFINE_NATIVE_FUNCTION(ResponseConstructor::error)
{
    WebIDL::log_trace(vm, "ResponseConstructor::error");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return Fetch::Response::error(vm); }));

    return &const_cast<Response&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ResponseConstructor::redirect)
{
    WebIDL::log_trace(vm, "ResponseConstructor::redirect");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "redirect");

    auto arg0 = vm.argument(0);

    String url;
    if (!false || !arg0.is_null()) {
        url = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedShort status;

    if (!arg1.is_undefined())

    status = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        status = static_cast<WebIDL::UnsignedShort>(302);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return Fetch::Response::redirect(vm, url, status); }));

    return &const_cast<Response&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ResponseConstructor::json)
{
    WebIDL::log_trace(vm, "ResponseConstructor::json");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "json");

    auto arg0 = vm.argument(0);

    auto data = arg0;

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ResponseInit");

    ResponseInit init {};

    auto headers_property_value_3 = JS::js_undefined();
    if (arg1.is_object())
        headers_property_value_3 = TRY(arg1.as_object().get("headers"_utf16_fly_string));

    if (!headers_property_value_3.is_undefined()) {

    auto headers_property_value_3_to_variant = [&vm, &realm](JS::Value headers_property_value_3) -> JS::ThrowCompletionOr<Variant<Vector<Vector<String>>, OrderedHashMap<String, String>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (headers_property_value_3.is_object()) {
            [[maybe_unused]] auto& headers_property_value_3_object = headers_property_value_3.as_object();

        auto method = TRY(headers_property_value_3.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto headers_property_value_3_iterator1 = TRY(JS::get_iterator_from_method(vm, headers_property_value_3, *method));

    Vector<Vector<String>> headers_value_3;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, headers_property_value_3_iterator1));
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

    headers_value_3.append(sequence_item1);
    }


            return headers_value_3;
        }

    OrderedHashMap<String, String> record_union_type;

    auto record_keys1 = TRY(headers_property_value_3_object.internal_own_property_keys());

    for (auto& key1 : record_keys1) {
        auto property_key1 = MUST(JS::PropertyKey::from_value(vm, key1));

        auto descriptor1 = TRY(headers_property_value_3_object.internal_get_own_property(property_key1));

        if (!descriptor1.has_value() || !descriptor1->enumerable.has_value() || !descriptor1->enumerable.value())
            continue;

    String typed_key1;
    if (!false || !key1.is_null()) {
        typed_key1 = TRY(WebIDL::to_byte_string(vm, key1));
    }

        auto value1 = TRY(headers_property_value_3_object.get(property_key1));

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

    Optional<Variant<Vector<Vector<String>>, OrderedHashMap<String, String>>> headers_value_3;
    if (!headers_property_value_3.is_undefined())
        headers_value_3 = TRY(headers_property_value_3_to_variant(headers_property_value_3));

    init.headers = headers_value_3;

    }

    auto status_property_value_4 = JS::js_undefined();
    if (arg1.is_object())
        status_property_value_4 = TRY(arg1.as_object().get("status"_utf16_fly_string));

    WebIDL::UnsignedShort status_value_4;

    if (!status_property_value_4.is_undefined())

    status_value_4 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, status_property_value_4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        status_value_4 = static_cast<WebIDL::UnsignedShort>(200);

    init.status = status_value_4;

    auto status_text_property_value_5 = JS::js_undefined();
    if (arg1.is_object())
        status_text_property_value_5 = TRY(arg1.as_object().get("statusText"_utf16_fly_string));

    String status_text_value_5;

    if (!status_text_property_value_5.is_undefined()) {
        if (!false || !status_text_property_value_5.is_null())
            status_text_value_5 = TRY(WebIDL::to_byte_string(vm, status_text_property_value_5));
    } else {
        status_text_value_5 = ""_string;
    }

    init.status_text = status_text_value_5;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return Fetch::Response::json(vm, data, init); }));

    return &const_cast<Response&>(*retval);

}

} // namespace Web::Bindings
