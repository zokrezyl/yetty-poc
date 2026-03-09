
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
#include <LibWeb/Bindings/DOMURLConstructor.h>
#include <LibWeb/Bindings/DOMURLPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOMURL/DOMURL.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOMURL/URLSearchParams.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/MediaSourceExtensions/MediaSource.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/MediaSourceExtensions/MediaSourceHandle.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/MediaSourceExtensions/SourceBufferList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/QueuingStrategy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamBYOBReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamDefaultReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/MediaSourceExtensions/SourceBuffer.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStreamDefaultWriter.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/AudioTrackList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TextTrackList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/VideoTrackList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TimeRanges.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/AudioTrack.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/VideoTrack.h>

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

GC_DEFINE_ALLOCATOR(DOMURLConstructor);

DOMURLConstructor::DOMURLConstructor(JS::Realm& realm)
    : NativeFunction("URL"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

DOMURLConstructor::~DOMURLConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> DOMURLConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "URL");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> DOMURLConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "DOMURLConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface URL:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<DOMURLPrototype>(*target_realm, "URL"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "URL");

    auto arg0 = vm.argument(0);

    String url;
    if (!false || !arg0.is_null()) {
        url = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    Optional<String> base;

    if (!arg1.is_undefined()) {
        if (!false || !arg1.is_null())
            base = TRY(WebIDL::to_usv_string(vm, arg1));
    }

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return DOMURL::DOMURL::construct_impl(realm, url, base); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface URL" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within DOMURL::DOMURL::construct_impl().
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


void DOMURLConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    define_direct_property(vm.names.length, JS::Value(1), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "URL"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<DOMURLPrototype>(realm, "URL"_fly_string), 0);


    define_native_function(realm, "parse"_utf16_fly_string, parse, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "canParse"_utf16_fly_string, can_parse, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "createObjectURL"_utf16_fly_string, create_object_url, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "revokeObjectURL"_utf16_fly_string, revoke_object_url, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

}

JS_DEFINE_NATIVE_FUNCTION(DOMURLConstructor::parse)
{
    WebIDL::log_trace(vm, "DOMURLConstructor::parse");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "parse");

    auto arg0 = vm.argument(0);

    String url;
    if (!false || !arg0.is_null()) {
        url = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    Optional<String> base;

    if (!arg1.is_undefined()) {
        if (!false || !arg1.is_null())
            base = TRY(WebIDL::to_usv_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return DOMURL::DOMURL::parse_for_bindings(vm, url, base); }));

    if (retval) {

    return &const_cast<DOMURL::DOMURL&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DOMURLConstructor::can_parse)
{
    WebIDL::log_trace(vm, "DOMURLConstructor::can_parse");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "canParse");

    auto arg0 = vm.argument(0);

    String url;
    if (!false || !arg0.is_null()) {
        url = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    Optional<String> base;

    if (!arg1.is_undefined()) {
        if (!false || !arg1.is_null())
            base = TRY(WebIDL::to_usv_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return DOMURL::DOMURL::can_parse(vm, url, base); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMURLConstructor::create_object_url)
{
    WebIDL::log_trace(vm, "DOMURLConstructor::create_object_url");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "createObjectURL");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<Blob>, GC::Root<MediaSourceExtensions::MediaSource>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<Blob>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<Blob>, GC::Root<MediaSourceExtensions::MediaSource>> obj = TRY(arg0_to_variant(arg0));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return DOMURL::DOMURL::create_object_url(vm, obj); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMURLConstructor::revoke_object_url)
{
    WebIDL::log_trace(vm, "DOMURLConstructor::revoke_object_url");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "revokeObjectURL");

    auto arg0 = vm.argument(0);

    String url;
    if (!false || !arg0.is_null()) {
        url = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return DOMURL::DOMURL::revoke_object_url(vm, url); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
