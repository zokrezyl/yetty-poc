
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
#include <LibWeb/Bindings/DOMMatrixConstructor.h>
#include <LibWeb/Bindings/DOMMatrixPrototype.h>
#include <LibWeb/Bindings/ExceptionOrUtils.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/HTML/WindowProxy.h>
#include <LibWeb/WebIDL/AbstractOperations.h>
#include <LibWeb/WebIDL/Buffers.h>
#include <LibWeb/WebIDL/CallbackType.h>
#include <LibWeb/WebIDL/OverloadResolution.h>
#include <LibWeb/WebIDL/Tracing.h>
#include <LibWeb/WebIDL/Types.h>

#if __has_include(<LibWeb/Bindings/DOMMatrixReadOnlyPrototype.h>)
#    include <LibWeb/Bindings/DOMMatrixReadOnlyPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrix.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrixReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPoint.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPointReadOnly.h>

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

GC_DEFINE_ALLOCATOR(DOMMatrixConstructor);

DOMMatrixConstructor::DOMMatrixConstructor(JS::Realm& realm)
    : NativeFunction("DOMMatrix"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

DOMMatrixConstructor::~DOMMatrixConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> DOMMatrixConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "DOMMatrix");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> DOMMatrixConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "DOMMatrixConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface DOMMatrix:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<DOMMatrixPrototype>(*target_realm, "DOMMatrix"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<String, Vector<double>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

        auto method = TRY(arg0.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg0_iterator1 = TRY(JS::get_iterator_from_method(vm, arg0, *method));

    Vector<double> init;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg0_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    double sequence_item1 = TRY(next_item1.to_double(vm));

    init.append(sequence_item1);
    }


            return init;
        }

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_string(vm, arg0));
    }

        return { arg0_string };

    };

    Optional<Variant<String, Vector<double>>> init;
    if (!arg0.is_undefined())
        init = TRY(arg0_to_variant(arg0));

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return Geometry::DOMMatrix::construct_impl(realm, init); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface DOMMatrix" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within Geometry::DOMMatrix::construct_impl().
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


void DOMMatrixConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    set_prototype(&ensure_web_constructor<DOMMatrixReadOnlyPrototype>(realm, "DOMMatrixReadOnly"_fly_string));

    define_direct_property(vm.names.length, JS::Value(0), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "DOMMatrix"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<DOMMatrixPrototype>(realm, "DOMMatrix"_fly_string), 0);


    define_native_function(realm, "fromMatrix"_utf16_fly_string, from_matrix, 0, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "fromFloat32Array"_utf16_fly_string, from_float32_array, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "fromFloat64Array"_utf16_fly_string, from_float64_array, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixConstructor::from_matrix)
{
    WebIDL::log_trace(vm, "DOMMatrixConstructor::from_matrix");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMMatrixInit");

    DOMMatrixInit other {};

    auto is2d_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        is2d_property_value_0 = TRY(arg0.as_object().get("is2D"_utf16_fly_string));

    if (!is2d_property_value_0.is_undefined()) {

    Optional<bool> is2d_value_0;

    if (!is2d_property_value_0.is_undefined())

    is2d_value_0 = is2d_property_value_0.to_boolean();

    other.is2d = is2d_value_0;

    }

    auto m13_property_value_1 = JS::js_undefined();
    if (arg0.is_object())
        m13_property_value_1 = TRY(arg0.as_object().get("m13"_utf16_fly_string));

    double m13_value_1;

    if (!m13_property_value_1.is_undefined())
        m13_value_1 = TRY(m13_property_value_1.to_double(vm));

    else
        m13_value_1 = 0;

    other.m13 = m13_value_1;

    auto m14_property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        m14_property_value_2 = TRY(arg0.as_object().get("m14"_utf16_fly_string));

    double m14_value_2;

    if (!m14_property_value_2.is_undefined())
        m14_value_2 = TRY(m14_property_value_2.to_double(vm));

    else
        m14_value_2 = 0;

    other.m14 = m14_value_2;

    auto m23_property_value_3 = JS::js_undefined();
    if (arg0.is_object())
        m23_property_value_3 = TRY(arg0.as_object().get("m23"_utf16_fly_string));

    double m23_value_3;

    if (!m23_property_value_3.is_undefined())
        m23_value_3 = TRY(m23_property_value_3.to_double(vm));

    else
        m23_value_3 = 0;

    other.m23 = m23_value_3;

    auto m24_property_value_4 = JS::js_undefined();
    if (arg0.is_object())
        m24_property_value_4 = TRY(arg0.as_object().get("m24"_utf16_fly_string));

    double m24_value_4;

    if (!m24_property_value_4.is_undefined())
        m24_value_4 = TRY(m24_property_value_4.to_double(vm));

    else
        m24_value_4 = 0;

    other.m24 = m24_value_4;

    auto m31_property_value_5 = JS::js_undefined();
    if (arg0.is_object())
        m31_property_value_5 = TRY(arg0.as_object().get("m31"_utf16_fly_string));

    double m31_value_5;

    if (!m31_property_value_5.is_undefined())
        m31_value_5 = TRY(m31_property_value_5.to_double(vm));

    else
        m31_value_5 = 0;

    other.m31 = m31_value_5;

    auto m32_property_value_6 = JS::js_undefined();
    if (arg0.is_object())
        m32_property_value_6 = TRY(arg0.as_object().get("m32"_utf16_fly_string));

    double m32_value_6;

    if (!m32_property_value_6.is_undefined())
        m32_value_6 = TRY(m32_property_value_6.to_double(vm));

    else
        m32_value_6 = 0;

    other.m32 = m32_value_6;

    auto m33_property_value_7 = JS::js_undefined();
    if (arg0.is_object())
        m33_property_value_7 = TRY(arg0.as_object().get("m33"_utf16_fly_string));

    double m33_value_7;

    if (!m33_property_value_7.is_undefined())
        m33_value_7 = TRY(m33_property_value_7.to_double(vm));

    else
        m33_value_7 = 1;

    other.m33 = m33_value_7;

    auto m34_property_value_8 = JS::js_undefined();
    if (arg0.is_object())
        m34_property_value_8 = TRY(arg0.as_object().get("m34"_utf16_fly_string));

    double m34_value_8;

    if (!m34_property_value_8.is_undefined())
        m34_value_8 = TRY(m34_property_value_8.to_double(vm));

    else
        m34_value_8 = 0;

    other.m34 = m34_value_8;

    auto m43_property_value_9 = JS::js_undefined();
    if (arg0.is_object())
        m43_property_value_9 = TRY(arg0.as_object().get("m43"_utf16_fly_string));

    double m43_value_9;

    if (!m43_property_value_9.is_undefined())
        m43_value_9 = TRY(m43_property_value_9.to_double(vm));

    else
        m43_value_9 = 0;

    other.m43 = m43_value_9;

    auto m44_property_value_10 = JS::js_undefined();
    if (arg0.is_object())
        m44_property_value_10 = TRY(arg0.as_object().get("m44"_utf16_fly_string));

    double m44_value_10;

    if (!m44_property_value_10.is_undefined())
        m44_value_10 = TRY(m44_property_value_10.to_double(vm));

    else
        m44_value_10 = 1;

    other.m44 = m44_value_10;

    auto a_property_value_11 = JS::js_undefined();
    if (arg0.is_object())
        a_property_value_11 = TRY(arg0.as_object().get("a"_utf16_fly_string));

    if (!a_property_value_11.is_undefined()) {

    Optional<double> a_value_11;

    if (!a_property_value_11.is_undefined())
        a_value_11 = TRY(a_property_value_11.to_double(vm));


    other.a = a_value_11;

    }

    auto b_property_value_12 = JS::js_undefined();
    if (arg0.is_object())
        b_property_value_12 = TRY(arg0.as_object().get("b"_utf16_fly_string));

    if (!b_property_value_12.is_undefined()) {

    Optional<double> b_value_12;

    if (!b_property_value_12.is_undefined())
        b_value_12 = TRY(b_property_value_12.to_double(vm));


    other.b = b_value_12;

    }

    auto c_property_value_13 = JS::js_undefined();
    if (arg0.is_object())
        c_property_value_13 = TRY(arg0.as_object().get("c"_utf16_fly_string));

    if (!c_property_value_13.is_undefined()) {

    Optional<double> c_value_13;

    if (!c_property_value_13.is_undefined())
        c_value_13 = TRY(c_property_value_13.to_double(vm));


    other.c = c_value_13;

    }

    auto d_property_value_14 = JS::js_undefined();
    if (arg0.is_object())
        d_property_value_14 = TRY(arg0.as_object().get("d"_utf16_fly_string));

    if (!d_property_value_14.is_undefined()) {

    Optional<double> d_value_14;

    if (!d_property_value_14.is_undefined())
        d_value_14 = TRY(d_property_value_14.to_double(vm));


    other.d = d_value_14;

    }

    auto e_property_value_15 = JS::js_undefined();
    if (arg0.is_object())
        e_property_value_15 = TRY(arg0.as_object().get("e"_utf16_fly_string));

    if (!e_property_value_15.is_undefined()) {

    Optional<double> e_value_15;

    if (!e_property_value_15.is_undefined())
        e_value_15 = TRY(e_property_value_15.to_double(vm));


    other.e = e_value_15;

    }

    auto f_property_value_16 = JS::js_undefined();
    if (arg0.is_object())
        f_property_value_16 = TRY(arg0.as_object().get("f"_utf16_fly_string));

    if (!f_property_value_16.is_undefined()) {

    Optional<double> f_value_16;

    if (!f_property_value_16.is_undefined())
        f_value_16 = TRY(f_property_value_16.to_double(vm));


    other.f = f_value_16;

    }

    auto m11_property_value_17 = JS::js_undefined();
    if (arg0.is_object())
        m11_property_value_17 = TRY(arg0.as_object().get("m11"_utf16_fly_string));

    if (!m11_property_value_17.is_undefined()) {

    Optional<double> m11_value_17;

    if (!m11_property_value_17.is_undefined())
        m11_value_17 = TRY(m11_property_value_17.to_double(vm));


    other.m11 = m11_value_17;

    }

    auto m12_property_value_18 = JS::js_undefined();
    if (arg0.is_object())
        m12_property_value_18 = TRY(arg0.as_object().get("m12"_utf16_fly_string));

    if (!m12_property_value_18.is_undefined()) {

    Optional<double> m12_value_18;

    if (!m12_property_value_18.is_undefined())
        m12_value_18 = TRY(m12_property_value_18.to_double(vm));


    other.m12 = m12_value_18;

    }

    auto m21_property_value_19 = JS::js_undefined();
    if (arg0.is_object())
        m21_property_value_19 = TRY(arg0.as_object().get("m21"_utf16_fly_string));

    if (!m21_property_value_19.is_undefined()) {

    Optional<double> m21_value_19;

    if (!m21_property_value_19.is_undefined())
        m21_value_19 = TRY(m21_property_value_19.to_double(vm));


    other.m21 = m21_value_19;

    }

    auto m22_property_value_20 = JS::js_undefined();
    if (arg0.is_object())
        m22_property_value_20 = TRY(arg0.as_object().get("m22"_utf16_fly_string));

    if (!m22_property_value_20.is_undefined()) {

    Optional<double> m22_value_20;

    if (!m22_property_value_20.is_undefined())
        m22_value_20 = TRY(m22_property_value_20.to_double(vm));


    other.m22 = m22_value_20;

    }

    auto m41_property_value_21 = JS::js_undefined();
    if (arg0.is_object())
        m41_property_value_21 = TRY(arg0.as_object().get("m41"_utf16_fly_string));

    if (!m41_property_value_21.is_undefined()) {

    Optional<double> m41_value_21;

    if (!m41_property_value_21.is_undefined())
        m41_value_21 = TRY(m41_property_value_21.to_double(vm));


    other.m41 = m41_value_21;

    }

    auto m42_property_value_22 = JS::js_undefined();
    if (arg0.is_object())
        m42_property_value_22 = TRY(arg0.as_object().get("m42"_utf16_fly_string));

    if (!m42_property_value_22.is_undefined()) {

    Optional<double> m42_value_22;

    if (!m42_property_value_22.is_undefined())
        m42_value_22 = TRY(m42_property_value_22.to_double(vm));


    other.m42 = m42_value_22;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return Geometry::DOMMatrix::from_matrix(vm, other); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixConstructor::from_float32_array)
{
    WebIDL::log_trace(vm, "DOMMatrixConstructor::from_float32_array");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "fromFloat32Array");

    auto arg0 = vm.argument(0);

    GC::Root<WebIDL::BufferSource> array32;

    if (!arg0.is_object() || !(is<JS::TypedArrayBase>(arg0.as_object()) || is<JS::ArrayBuffer>(arg0.as_object()) || is<JS::DataView>(arg0.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Float32Array");

    array32 = GC::make_root(realm.create<WebIDL::BufferSource>(arg0.as_object()));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return Geometry::DOMMatrix::from_float32_array(vm, array32); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixConstructor::from_float64_array)
{
    WebIDL::log_trace(vm, "DOMMatrixConstructor::from_float64_array");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "fromFloat64Array");

    auto arg0 = vm.argument(0);

    GC::Root<WebIDL::BufferSource> array64;

    if (!arg0.is_object() || !(is<JS::TypedArrayBase>(arg0.as_object()) || is<JS::ArrayBuffer>(arg0.as_object()) || is<JS::DataView>(arg0.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Float64Array");

    array64 = GC::make_root(realm.create<WebIDL::BufferSource>(arg0.as_object()));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return Geometry::DOMMatrix::from_float64_array(vm, array64); }));

    return &const_cast<DOMMatrix&>(*retval);

}

} // namespace Web::Bindings
