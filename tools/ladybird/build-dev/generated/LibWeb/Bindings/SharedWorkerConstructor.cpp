
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
#include <LibWeb/Bindings/SharedWorkerConstructor.h>
#include <LibWeb/Bindings/SharedWorkerPrototype.h>
#include <LibWeb/Bindings/ExceptionOrUtils.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/HTML/WindowProxy.h>
#include <LibWeb/WebIDL/AbstractOperations.h>
#include <LibWeb/WebIDL/Buffers.h>
#include <LibWeb/WebIDL/CallbackType.h>
#include <LibWeb/WebIDL/OverloadResolution.h>
#include <LibWeb/WebIDL/Tracing.h>
#include <LibWeb/WebIDL/Types.h>

#if __has_include(<LibWeb/Bindings/EventTargetPrototype.h>)
#    include <LibWeb/Bindings/EventTargetPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/SharedWorker.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/MessagePort.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Worker.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedScriptURL.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Request.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Headers.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XHR/FormData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOMURL/URLSearchParams.h>

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

GC_DEFINE_ALLOCATOR(SharedWorkerConstructor);

SharedWorkerConstructor::SharedWorkerConstructor(JS::Realm& realm)
    : NativeFunction("SharedWorker"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

SharedWorkerConstructor::~SharedWorkerConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> SharedWorkerConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "SharedWorker");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> SharedWorkerConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "SharedWorkerConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface SharedWorker:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<SharedWorkerPrototype>(*target_realm, "SharedWorker"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "SharedWorker");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<TrustedScriptURL>, Utf16String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<TrustedScriptURL>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

    Utf16String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_utf16_usv_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<GC::Root<TrustedScriptURL>, Utf16String> script_url = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    auto arg1_to_dictionary = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<WorkerOptions> {
        // This might be unused.
        (void)realm;

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WorkerOptions");

    WorkerOptions dictionary_union_type {};

    auto credentials_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        credentials_property_value_0 = TRY(arg1.as_object().get("credentials"_utf16_fly_string));

    RequestCredentials credentials_value_0 { RequestCredentials::SameOrigin };

    if (!credentials_property_value_0.is_undefined()) {

    auto credentials_property_value_0_string = TRY(credentials_property_value_0.to_string(vm));

    if (credentials_property_value_0_string == "omit"sv)
        credentials_value_0 = RequestCredentials::Omit;

    else if (credentials_property_value_0_string == "same-origin"sv)
        credentials_value_0 = RequestCredentials::SameOrigin;

    else if (credentials_property_value_0_string == "include"sv)
        credentials_value_0 = RequestCredentials::Include;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, credentials_property_value_0_string, "RequestCredentials");

    }

    dictionary_union_type.credentials = credentials_value_0;

    auto name_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        name_property_value_1 = TRY(arg1.as_object().get("name"_utf16_fly_string));

    String name_value_1;

    if (!name_property_value_1.is_undefined()) {
        if (!false || !name_property_value_1.is_null())
            name_value_1 = TRY(WebIDL::to_string(vm, name_property_value_1));
    } else {
        name_value_1 = ""_string;
    }

    dictionary_union_type.name = name_value_1;

    auto type_property_value_2 = JS::js_undefined();
    if (arg1.is_object())
        type_property_value_2 = TRY(arg1.as_object().get("type"_utf16_fly_string));

    WorkerType type_value_2 { WorkerType::Classic };

    if (!type_property_value_2.is_undefined()) {

    auto type_property_value_2_string = TRY(type_property_value_2.to_string(vm));

    if (type_property_value_2_string == "classic"sv)
        type_value_2 = WorkerType::Classic;

    else if (type_property_value_2_string == "module"sv)
        type_value_2 = WorkerType::Module;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, type_property_value_2_string, "WorkerType");

    }

    dictionary_union_type.type = type_value_2;

        return dictionary_union_type;
    };

    auto arg1_to_variant = [&vm, &realm, &arg1_to_dictionary](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<String, WorkerOptions>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_nullish())
            return Variant<String, WorkerOptions> { TRY(arg1_to_dictionary(arg1)) };

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

        return Variant<String, WorkerOptions> { TRY(arg1_to_dictionary(arg1)) };

        }

    String arg1_string;
    if (!false || !arg1.is_null()) {
        arg1_string = TRY(WebIDL::to_string(vm, arg1));
    }

        return { arg1_string };

    };

    Variant<String, WorkerOptions> options = arg1.is_undefined() ? TRY(arg1_to_dictionary(arg1)) : TRY(arg1_to_variant(arg1));

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return HTML::SharedWorker::construct_impl(realm, script_url, options); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface SharedWorker" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within HTML::SharedWorker::construct_impl().
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


void SharedWorkerConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    set_prototype(&ensure_web_constructor<EventTargetPrototype>(realm, "EventTarget"_fly_string));

    define_direct_property(vm.names.length, JS::Value(1), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "SharedWorker"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<SharedWorkerPrototype>(realm, "SharedWorker"_fly_string), 0);


}

} // namespace Web::Bindings
