
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
#include <LibWeb/Bindings/TransformStreamConstructor.h>
#include <LibWeb/Bindings/TransformStreamPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/TransformStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/QueuingStrategy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamBYOBReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamDefaultReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStreamDefaultWriter.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

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

GC_DEFINE_ALLOCATOR(TransformStreamConstructor);

TransformStreamConstructor::TransformStreamConstructor(JS::Realm& realm)
    : NativeFunction("TransformStream"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

TransformStreamConstructor::~TransformStreamConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> TransformStreamConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "TransformStream");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> TransformStreamConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "TransformStreamConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface TransformStream:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<TransformStreamPrototype>(*target_realm, "TransformStream"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    auto arg0 = vm.argument(0);

    Optional<GC::Root<JS::Object>> transformer;
    if (!arg0.is_undefined()) {
        if (!arg0.is_object())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg0);
        transformer = GC::make_root(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "QueuingStrategy");

    QueuingStrategy writable_strategy {};

    auto high_water_mark_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        high_water_mark_property_value_0 = TRY(arg1.as_object().get("highWaterMark"_utf16_fly_string));

    if (!high_water_mark_property_value_0.is_undefined()) {

    Optional<double> high_water_mark_value_0;

    if (!high_water_mark_property_value_0.is_undefined())
        high_water_mark_value_0 = TRY(high_water_mark_property_value_0.to_double(vm));


    writable_strategy.high_water_mark = high_water_mark_value_0;

    }

    auto size_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        size_property_value_1 = TRY(arg1.as_object().get("size"_utf16_fly_string));

    if (!size_property_value_1.is_undefined()) {

    if (!size_property_value_1.is_function()
&& !size_property_value_1.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, size_property_value_1);

    GC::Ptr<WebIDL::CallbackType> size_value_1;
    if (size_property_value_1.is_object())
        size_value_1 = vm.heap().allocate<WebIDL::CallbackType>(size_property_value_1.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    writable_strategy.size = size_value_1;

    }

    auto arg2 = vm.argument(2);

    if (!arg2.is_nullish() && !arg2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "QueuingStrategy");

    QueuingStrategy readable_strategy {};

    auto high_water_mark_property_value_2 = JS::js_undefined();
    if (arg2.is_object())
        high_water_mark_property_value_2 = TRY(arg2.as_object().get("highWaterMark"_utf16_fly_string));

    if (!high_water_mark_property_value_2.is_undefined()) {

    Optional<double> high_water_mark_value_2;

    if (!high_water_mark_property_value_2.is_undefined())
        high_water_mark_value_2 = TRY(high_water_mark_property_value_2.to_double(vm));


    readable_strategy.high_water_mark = high_water_mark_value_2;

    }

    auto size_property_value_3 = JS::js_undefined();
    if (arg2.is_object())
        size_property_value_3 = TRY(arg2.as_object().get("size"_utf16_fly_string));

    if (!size_property_value_3.is_undefined()) {

    if (!size_property_value_3.is_function()
&& !size_property_value_3.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, size_property_value_3);

    GC::Ptr<WebIDL::CallbackType> size_value_3;
    if (size_property_value_3.is_object())
        size_value_3 = vm.heap().allocate<WebIDL::CallbackType>(size_property_value_3.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    readable_strategy.size = size_value_3;

    }

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return Streams::TransformStream::construct_impl(realm, transformer, writable_strategy, readable_strategy); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface TransformStream" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within Streams::TransformStream::construct_impl().
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


void TransformStreamConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    define_direct_property(vm.names.length, JS::Value(0), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "TransformStream"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<TransformStreamPrototype>(realm, "TransformStream"_fly_string), 0);


}

} // namespace Web::Bindings
