
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
#include <LibWeb/Bindings/DOMPointReadOnlyConstructor.h>
#include <LibWeb/Bindings/DOMPointReadOnlyPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPointReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrixReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPoint.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrix.h>

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

GC_DEFINE_ALLOCATOR(DOMPointReadOnlyConstructor);

DOMPointReadOnlyConstructor::DOMPointReadOnlyConstructor(JS::Realm& realm)
    : NativeFunction("DOMPointReadOnly"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

DOMPointReadOnlyConstructor::~DOMPointReadOnlyConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> DOMPointReadOnlyConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "DOMPointReadOnly");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> DOMPointReadOnlyConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "DOMPointReadOnlyConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface DOMPointReadOnly:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<DOMPointReadOnlyPrototype>(*target_realm, "DOMPointReadOnly"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    auto arg0 = vm.argument(0);

    double x;

    if (!arg0.is_undefined())
        x = TRY(arg0.to_double(vm));

    else
        x = 0;

    auto arg1 = vm.argument(1);

    double y;

    if (!arg1.is_undefined())
        y = TRY(arg1.to_double(vm));

    else
        y = 0;

    auto arg2 = vm.argument(2);

    double z;

    if (!arg2.is_undefined())
        z = TRY(arg2.to_double(vm));

    else
        z = 0;

    auto arg3 = vm.argument(3);

    double w;

    if (!arg3.is_undefined())
        w = TRY(arg3.to_double(vm));

    else
        w = 1;

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return Geometry::DOMPointReadOnly::construct_impl(realm, x, y, z, w); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface DOMPointReadOnly" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within Geometry::DOMPointReadOnly::construct_impl().
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


void DOMPointReadOnlyConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    define_direct_property(vm.names.length, JS::Value(0), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "DOMPointReadOnly"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<DOMPointReadOnlyPrototype>(realm, "DOMPointReadOnly"_fly_string), 0);


    define_native_function(realm, "fromPoint"_utf16_fly_string, from_point, 0, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

}

JS_DEFINE_NATIVE_FUNCTION(DOMPointReadOnlyConstructor::from_point)
{
    WebIDL::log_trace(vm, "DOMPointReadOnlyConstructor::from_point");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMPointInit");

    DOMPointInit other {};

    auto w_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        w_property_value_0 = TRY(arg0.as_object().get("w"_utf16_fly_string));

    double w_value_0;

    if (!w_property_value_0.is_undefined())
        w_value_0 = TRY(w_property_value_0.to_double(vm));

    else
        w_value_0 = 1;

    other.w = w_value_0;

    auto x_property_value_1 = JS::js_undefined();
    if (arg0.is_object())
        x_property_value_1 = TRY(arg0.as_object().get("x"_utf16_fly_string));

    double x_value_1;

    if (!x_property_value_1.is_undefined())
        x_value_1 = TRY(x_property_value_1.to_double(vm));

    else
        x_value_1 = 0;

    other.x = x_value_1;

    auto y_property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        y_property_value_2 = TRY(arg0.as_object().get("y"_utf16_fly_string));

    double y_value_2;

    if (!y_property_value_2.is_undefined())
        y_value_2 = TRY(y_property_value_2.to_double(vm));

    else
        y_value_2 = 0;

    other.y = y_value_2;

    auto z_property_value_3 = JS::js_undefined();
    if (arg0.is_object())
        z_property_value_3 = TRY(arg0.as_object().get("z"_utf16_fly_string));

    double z_value_3;

    if (!z_property_value_3.is_undefined())
        z_value_3 = TRY(z_property_value_3.to_double(vm));

    else
        z_value_3 = 0;

    other.z = z_value_3;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return Geometry::DOMPointReadOnly::from_point(vm, other); }));

    return &const_cast<DOMPointReadOnly&>(*retval);

}

} // namespace Web::Bindings
