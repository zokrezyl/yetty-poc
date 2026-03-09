
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
#include <LibWeb/Bindings/DOMQuadConstructor.h>
#include <LibWeb/Bindings/DOMQuadPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMQuad.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPoint.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRectReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPointReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrixReadOnly.h>

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

GC_DEFINE_ALLOCATOR(DOMQuadConstructor);

DOMQuadConstructor::DOMQuadConstructor(JS::Realm& realm)
    : NativeFunction("DOMQuad"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

DOMQuadConstructor::~DOMQuadConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> DOMQuadConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "DOMQuad");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> DOMQuadConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "DOMQuadConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface DOMQuad:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<DOMQuadPrototype>(*target_realm, "DOMQuad"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMPointInit");

    DOMPointInit p1 {};

    auto w_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        w_property_value_0 = TRY(arg0.as_object().get("w"_utf16_fly_string));

    double w_value_0;

    if (!w_property_value_0.is_undefined())
        w_value_0 = TRY(w_property_value_0.to_double(vm));

    else
        w_value_0 = 1;

    p1.w = w_value_0;

    auto x_property_value_1 = JS::js_undefined();
    if (arg0.is_object())
        x_property_value_1 = TRY(arg0.as_object().get("x"_utf16_fly_string));

    double x_value_1;

    if (!x_property_value_1.is_undefined())
        x_value_1 = TRY(x_property_value_1.to_double(vm));

    else
        x_value_1 = 0;

    p1.x = x_value_1;

    auto y_property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        y_property_value_2 = TRY(arg0.as_object().get("y"_utf16_fly_string));

    double y_value_2;

    if (!y_property_value_2.is_undefined())
        y_value_2 = TRY(y_property_value_2.to_double(vm));

    else
        y_value_2 = 0;

    p1.y = y_value_2;

    auto z_property_value_3 = JS::js_undefined();
    if (arg0.is_object())
        z_property_value_3 = TRY(arg0.as_object().get("z"_utf16_fly_string));

    double z_value_3;

    if (!z_property_value_3.is_undefined())
        z_value_3 = TRY(z_property_value_3.to_double(vm));

    else
        z_value_3 = 0;

    p1.z = z_value_3;

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMPointInit");

    DOMPointInit p2 {};

    auto w_property_value_4 = JS::js_undefined();
    if (arg1.is_object())
        w_property_value_4 = TRY(arg1.as_object().get("w"_utf16_fly_string));

    double w_value_4;

    if (!w_property_value_4.is_undefined())
        w_value_4 = TRY(w_property_value_4.to_double(vm));

    else
        w_value_4 = 1;

    p2.w = w_value_4;

    auto x_property_value_5 = JS::js_undefined();
    if (arg1.is_object())
        x_property_value_5 = TRY(arg1.as_object().get("x"_utf16_fly_string));

    double x_value_5;

    if (!x_property_value_5.is_undefined())
        x_value_5 = TRY(x_property_value_5.to_double(vm));

    else
        x_value_5 = 0;

    p2.x = x_value_5;

    auto y_property_value_6 = JS::js_undefined();
    if (arg1.is_object())
        y_property_value_6 = TRY(arg1.as_object().get("y"_utf16_fly_string));

    double y_value_6;

    if (!y_property_value_6.is_undefined())
        y_value_6 = TRY(y_property_value_6.to_double(vm));

    else
        y_value_6 = 0;

    p2.y = y_value_6;

    auto z_property_value_7 = JS::js_undefined();
    if (arg1.is_object())
        z_property_value_7 = TRY(arg1.as_object().get("z"_utf16_fly_string));

    double z_value_7;

    if (!z_property_value_7.is_undefined())
        z_value_7 = TRY(z_property_value_7.to_double(vm));

    else
        z_value_7 = 0;

    p2.z = z_value_7;

    auto arg2 = vm.argument(2);

    if (!arg2.is_nullish() && !arg2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMPointInit");

    DOMPointInit p3 {};

    auto w_property_value_8 = JS::js_undefined();
    if (arg2.is_object())
        w_property_value_8 = TRY(arg2.as_object().get("w"_utf16_fly_string));

    double w_value_8;

    if (!w_property_value_8.is_undefined())
        w_value_8 = TRY(w_property_value_8.to_double(vm));

    else
        w_value_8 = 1;

    p3.w = w_value_8;

    auto x_property_value_9 = JS::js_undefined();
    if (arg2.is_object())
        x_property_value_9 = TRY(arg2.as_object().get("x"_utf16_fly_string));

    double x_value_9;

    if (!x_property_value_9.is_undefined())
        x_value_9 = TRY(x_property_value_9.to_double(vm));

    else
        x_value_9 = 0;

    p3.x = x_value_9;

    auto y_property_value_10 = JS::js_undefined();
    if (arg2.is_object())
        y_property_value_10 = TRY(arg2.as_object().get("y"_utf16_fly_string));

    double y_value_10;

    if (!y_property_value_10.is_undefined())
        y_value_10 = TRY(y_property_value_10.to_double(vm));

    else
        y_value_10 = 0;

    p3.y = y_value_10;

    auto z_property_value_11 = JS::js_undefined();
    if (arg2.is_object())
        z_property_value_11 = TRY(arg2.as_object().get("z"_utf16_fly_string));

    double z_value_11;

    if (!z_property_value_11.is_undefined())
        z_value_11 = TRY(z_property_value_11.to_double(vm));

    else
        z_value_11 = 0;

    p3.z = z_value_11;

    auto arg3 = vm.argument(3);

    if (!arg3.is_nullish() && !arg3.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMPointInit");

    DOMPointInit p4 {};

    auto w_property_value_12 = JS::js_undefined();
    if (arg3.is_object())
        w_property_value_12 = TRY(arg3.as_object().get("w"_utf16_fly_string));

    double w_value_12;

    if (!w_property_value_12.is_undefined())
        w_value_12 = TRY(w_property_value_12.to_double(vm));

    else
        w_value_12 = 1;

    p4.w = w_value_12;

    auto x_property_value_13 = JS::js_undefined();
    if (arg3.is_object())
        x_property_value_13 = TRY(arg3.as_object().get("x"_utf16_fly_string));

    double x_value_13;

    if (!x_property_value_13.is_undefined())
        x_value_13 = TRY(x_property_value_13.to_double(vm));

    else
        x_value_13 = 0;

    p4.x = x_value_13;

    auto y_property_value_14 = JS::js_undefined();
    if (arg3.is_object())
        y_property_value_14 = TRY(arg3.as_object().get("y"_utf16_fly_string));

    double y_value_14;

    if (!y_property_value_14.is_undefined())
        y_value_14 = TRY(y_property_value_14.to_double(vm));

    else
        y_value_14 = 0;

    p4.y = y_value_14;

    auto z_property_value_15 = JS::js_undefined();
    if (arg3.is_object())
        z_property_value_15 = TRY(arg3.as_object().get("z"_utf16_fly_string));

    double z_value_15;

    if (!z_property_value_15.is_undefined())
        z_value_15 = TRY(z_property_value_15.to_double(vm));

    else
        z_value_15 = 0;

    p4.z = z_value_15;

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return Geometry::DOMQuad::construct_impl(realm, p1, p2, p3, p4); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface DOMQuad" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within Geometry::DOMQuad::construct_impl().
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


void DOMQuadConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    define_direct_property(vm.names.length, JS::Value(0), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "DOMQuad"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<DOMQuadPrototype>(realm, "DOMQuad"_fly_string), 0);


    define_native_function(realm, "fromRect"_utf16_fly_string, from_rect, 0, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "fromQuad"_utf16_fly_string, from_quad, 0, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

}

JS_DEFINE_NATIVE_FUNCTION(DOMQuadConstructor::from_rect)
{
    WebIDL::log_trace(vm, "DOMQuadConstructor::from_rect");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMRectInit");

    DOMRectInit other {};

    auto height_property_value_16 = JS::js_undefined();
    if (arg0.is_object())
        height_property_value_16 = TRY(arg0.as_object().get("height"_utf16_fly_string));

    double height_value_16;

    if (!height_property_value_16.is_undefined())
        height_value_16 = TRY(height_property_value_16.to_double(vm));

    else
        height_value_16 = 0;

    other.height = height_value_16;

    auto width_property_value_17 = JS::js_undefined();
    if (arg0.is_object())
        width_property_value_17 = TRY(arg0.as_object().get("width"_utf16_fly_string));

    double width_value_17;

    if (!width_property_value_17.is_undefined())
        width_value_17 = TRY(width_property_value_17.to_double(vm));

    else
        width_value_17 = 0;

    other.width = width_value_17;

    auto x_property_value_18 = JS::js_undefined();
    if (arg0.is_object())
        x_property_value_18 = TRY(arg0.as_object().get("x"_utf16_fly_string));

    double x_value_18;

    if (!x_property_value_18.is_undefined())
        x_value_18 = TRY(x_property_value_18.to_double(vm));

    else
        x_value_18 = 0;

    other.x = x_value_18;

    auto y_property_value_19 = JS::js_undefined();
    if (arg0.is_object())
        y_property_value_19 = TRY(arg0.as_object().get("y"_utf16_fly_string));

    double y_value_19;

    if (!y_property_value_19.is_undefined())
        y_value_19 = TRY(y_property_value_19.to_double(vm));

    else
        y_value_19 = 0;

    other.y = y_value_19;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return Geometry::DOMQuad::from_rect(vm, other); }));

    return &const_cast<DOMQuad&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMQuadConstructor::from_quad)
{
    WebIDL::log_trace(vm, "DOMQuadConstructor::from_quad");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMQuadInit");

    DOMQuadInit other {};

    auto p1_property_value_20 = JS::js_undefined();
    if (arg0.is_object())
        p1_property_value_20 = TRY(arg0.as_object().get("p1"_utf16_fly_string));

    if (!p1_property_value_20.is_undefined()) {

    if (!p1_property_value_20.is_nullish() && !p1_property_value_20.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMPointInit");

    DOMPointInit p1_value_20 {};

    auto w_property_value_0 = JS::js_undefined();
    if (p1_property_value_20.is_object())
        w_property_value_0 = TRY(p1_property_value_20.as_object().get("w"_utf16_fly_string));

    double w_value_0;

    if (!w_property_value_0.is_undefined())
        w_value_0 = TRY(w_property_value_0.to_double(vm));

    else
        w_value_0 = 1;

    p1_value_20.w = w_value_0;

    auto x_property_value_1 = JS::js_undefined();
    if (p1_property_value_20.is_object())
        x_property_value_1 = TRY(p1_property_value_20.as_object().get("x"_utf16_fly_string));

    double x_value_1;

    if (!x_property_value_1.is_undefined())
        x_value_1 = TRY(x_property_value_1.to_double(vm));

    else
        x_value_1 = 0;

    p1_value_20.x = x_value_1;

    auto y_property_value_2 = JS::js_undefined();
    if (p1_property_value_20.is_object())
        y_property_value_2 = TRY(p1_property_value_20.as_object().get("y"_utf16_fly_string));

    double y_value_2;

    if (!y_property_value_2.is_undefined())
        y_value_2 = TRY(y_property_value_2.to_double(vm));

    else
        y_value_2 = 0;

    p1_value_20.y = y_value_2;

    auto z_property_value_3 = JS::js_undefined();
    if (p1_property_value_20.is_object())
        z_property_value_3 = TRY(p1_property_value_20.as_object().get("z"_utf16_fly_string));

    double z_value_3;

    if (!z_property_value_3.is_undefined())
        z_value_3 = TRY(z_property_value_3.to_double(vm));

    else
        z_value_3 = 0;

    p1_value_20.z = z_value_3;

    other.p1 = p1_value_20;

    }

    auto p2_property_value_21 = JS::js_undefined();
    if (arg0.is_object())
        p2_property_value_21 = TRY(arg0.as_object().get("p2"_utf16_fly_string));

    if (!p2_property_value_21.is_undefined()) {

    if (!p2_property_value_21.is_nullish() && !p2_property_value_21.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMPointInit");

    DOMPointInit p2_value_21 {};

    auto w_property_value_4 = JS::js_undefined();
    if (p2_property_value_21.is_object())
        w_property_value_4 = TRY(p2_property_value_21.as_object().get("w"_utf16_fly_string));

    double w_value_4;

    if (!w_property_value_4.is_undefined())
        w_value_4 = TRY(w_property_value_4.to_double(vm));

    else
        w_value_4 = 1;

    p2_value_21.w = w_value_4;

    auto x_property_value_5 = JS::js_undefined();
    if (p2_property_value_21.is_object())
        x_property_value_5 = TRY(p2_property_value_21.as_object().get("x"_utf16_fly_string));

    double x_value_5;

    if (!x_property_value_5.is_undefined())
        x_value_5 = TRY(x_property_value_5.to_double(vm));

    else
        x_value_5 = 0;

    p2_value_21.x = x_value_5;

    auto y_property_value_6 = JS::js_undefined();
    if (p2_property_value_21.is_object())
        y_property_value_6 = TRY(p2_property_value_21.as_object().get("y"_utf16_fly_string));

    double y_value_6;

    if (!y_property_value_6.is_undefined())
        y_value_6 = TRY(y_property_value_6.to_double(vm));

    else
        y_value_6 = 0;

    p2_value_21.y = y_value_6;

    auto z_property_value_7 = JS::js_undefined();
    if (p2_property_value_21.is_object())
        z_property_value_7 = TRY(p2_property_value_21.as_object().get("z"_utf16_fly_string));

    double z_value_7;

    if (!z_property_value_7.is_undefined())
        z_value_7 = TRY(z_property_value_7.to_double(vm));

    else
        z_value_7 = 0;

    p2_value_21.z = z_value_7;

    other.p2 = p2_value_21;

    }

    auto p3_property_value_22 = JS::js_undefined();
    if (arg0.is_object())
        p3_property_value_22 = TRY(arg0.as_object().get("p3"_utf16_fly_string));

    if (!p3_property_value_22.is_undefined()) {

    if (!p3_property_value_22.is_nullish() && !p3_property_value_22.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMPointInit");

    DOMPointInit p3_value_22 {};

    auto w_property_value_8 = JS::js_undefined();
    if (p3_property_value_22.is_object())
        w_property_value_8 = TRY(p3_property_value_22.as_object().get("w"_utf16_fly_string));

    double w_value_8;

    if (!w_property_value_8.is_undefined())
        w_value_8 = TRY(w_property_value_8.to_double(vm));

    else
        w_value_8 = 1;

    p3_value_22.w = w_value_8;

    auto x_property_value_9 = JS::js_undefined();
    if (p3_property_value_22.is_object())
        x_property_value_9 = TRY(p3_property_value_22.as_object().get("x"_utf16_fly_string));

    double x_value_9;

    if (!x_property_value_9.is_undefined())
        x_value_9 = TRY(x_property_value_9.to_double(vm));

    else
        x_value_9 = 0;

    p3_value_22.x = x_value_9;

    auto y_property_value_10 = JS::js_undefined();
    if (p3_property_value_22.is_object())
        y_property_value_10 = TRY(p3_property_value_22.as_object().get("y"_utf16_fly_string));

    double y_value_10;

    if (!y_property_value_10.is_undefined())
        y_value_10 = TRY(y_property_value_10.to_double(vm));

    else
        y_value_10 = 0;

    p3_value_22.y = y_value_10;

    auto z_property_value_11 = JS::js_undefined();
    if (p3_property_value_22.is_object())
        z_property_value_11 = TRY(p3_property_value_22.as_object().get("z"_utf16_fly_string));

    double z_value_11;

    if (!z_property_value_11.is_undefined())
        z_value_11 = TRY(z_property_value_11.to_double(vm));

    else
        z_value_11 = 0;

    p3_value_22.z = z_value_11;

    other.p3 = p3_value_22;

    }

    auto p4_property_value_23 = JS::js_undefined();
    if (arg0.is_object())
        p4_property_value_23 = TRY(arg0.as_object().get("p4"_utf16_fly_string));

    if (!p4_property_value_23.is_undefined()) {

    if (!p4_property_value_23.is_nullish() && !p4_property_value_23.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMPointInit");

    DOMPointInit p4_value_23 {};

    auto w_property_value_12 = JS::js_undefined();
    if (p4_property_value_23.is_object())
        w_property_value_12 = TRY(p4_property_value_23.as_object().get("w"_utf16_fly_string));

    double w_value_12;

    if (!w_property_value_12.is_undefined())
        w_value_12 = TRY(w_property_value_12.to_double(vm));

    else
        w_value_12 = 1;

    p4_value_23.w = w_value_12;

    auto x_property_value_13 = JS::js_undefined();
    if (p4_property_value_23.is_object())
        x_property_value_13 = TRY(p4_property_value_23.as_object().get("x"_utf16_fly_string));

    double x_value_13;

    if (!x_property_value_13.is_undefined())
        x_value_13 = TRY(x_property_value_13.to_double(vm));

    else
        x_value_13 = 0;

    p4_value_23.x = x_value_13;

    auto y_property_value_14 = JS::js_undefined();
    if (p4_property_value_23.is_object())
        y_property_value_14 = TRY(p4_property_value_23.as_object().get("y"_utf16_fly_string));

    double y_value_14;

    if (!y_property_value_14.is_undefined())
        y_value_14 = TRY(y_property_value_14.to_double(vm));

    else
        y_value_14 = 0;

    p4_value_23.y = y_value_14;

    auto z_property_value_15 = JS::js_undefined();
    if (p4_property_value_23.is_object())
        z_property_value_15 = TRY(p4_property_value_23.as_object().get("z"_utf16_fly_string));

    double z_value_15;

    if (!z_property_value_15.is_undefined())
        z_value_15 = TRY(z_property_value_15.to_double(vm));

    else
        z_value_15 = 0;

    p4_value_23.z = z_value_15;

    other.p4 = p4_value_23;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return Geometry::DOMQuad::from_quad(vm, other); }));

    return &const_cast<DOMQuad&>(*retval);

}

} // namespace Web::Bindings
