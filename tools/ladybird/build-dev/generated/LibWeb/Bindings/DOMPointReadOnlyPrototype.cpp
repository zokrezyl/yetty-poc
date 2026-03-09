
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
#include <LibWeb/Bindings/DOMPointReadOnlyPrototype.h>
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

GC_DEFINE_ALLOCATOR(DOMPointReadOnlyPrototype);

DOMPointReadOnlyPrototype::DOMPointReadOnlyPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

DOMPointReadOnlyPrototype::~DOMPointReadOnlyPrototype()
{
}

void DOMPointReadOnlyPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_x_getter = JS::NativeFunction::create(realm, x_getter, 0, "x"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_x_setter;

    define_direct_accessor("x"_utf16_fly_string, native_x_getter, native_x_setter, default_attributes);

    auto native_y_getter = JS::NativeFunction::create(realm, y_getter, 0, "y"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_y_setter;

    define_direct_accessor("y"_utf16_fly_string, native_y_getter, native_y_setter, default_attributes);

    auto native_z_getter = JS::NativeFunction::create(realm, z_getter, 0, "z"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_z_setter;

    define_direct_accessor("z"_utf16_fly_string, native_z_getter, native_z_setter, default_attributes);

    auto native_w_getter = JS::NativeFunction::create(realm, w_getter, 0, "w"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_w_setter;

    define_direct_accessor("w"_utf16_fly_string, native_w_getter, native_w_setter, default_attributes);

    define_native_function(realm, "matrixTransform"_utf16_fly_string, matrix_transform, 0, default_attributes);

    define_native_function(realm, "toJSON"_utf16_fly_string, to_json, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "DOMPointReadOnly"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void DOMPointReadOnlyPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Geometry::DOMPointReadOnly*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Geometry::DOMPointReadOnly>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMPointReadOnly");
    return static_cast<Geometry::DOMPointReadOnly*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(DOMPointReadOnlyPrototype::x_getter)
{
    WebIDL::log_trace(vm, "DOMPointReadOnlyPrototype::x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->x(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMPointReadOnlyPrototype::y_getter)
{
    WebIDL::log_trace(vm, "DOMPointReadOnlyPrototype::y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->y(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMPointReadOnlyPrototype::z_getter)
{
    WebIDL::log_trace(vm, "DOMPointReadOnlyPrototype::z_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->z(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMPointReadOnlyPrototype::w_getter)
{
    WebIDL::log_trace(vm, "DOMPointReadOnlyPrototype::w_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->w(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMPointReadOnlyPrototype::matrix_transform)
{
    WebIDL::log_trace(vm, "DOMPointReadOnlyPrototype::matrix_transform");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMMatrixInit");

    DOMMatrixInit matrix {};

    auto is2d_property_value_4 = JS::js_undefined();
    if (arg0.is_object())
        is2d_property_value_4 = TRY(arg0.as_object().get("is2D"_utf16_fly_string));

    if (!is2d_property_value_4.is_undefined()) {

    Optional<bool> is2d_value_4;

    if (!is2d_property_value_4.is_undefined())

    is2d_value_4 = is2d_property_value_4.to_boolean();

    matrix.is2d = is2d_value_4;

    }

    auto m13_property_value_5 = JS::js_undefined();
    if (arg0.is_object())
        m13_property_value_5 = TRY(arg0.as_object().get("m13"_utf16_fly_string));

    double m13_value_5;

    if (!m13_property_value_5.is_undefined())
        m13_value_5 = TRY(m13_property_value_5.to_double(vm));

    else
        m13_value_5 = 0;

    matrix.m13 = m13_value_5;

    auto m14_property_value_6 = JS::js_undefined();
    if (arg0.is_object())
        m14_property_value_6 = TRY(arg0.as_object().get("m14"_utf16_fly_string));

    double m14_value_6;

    if (!m14_property_value_6.is_undefined())
        m14_value_6 = TRY(m14_property_value_6.to_double(vm));

    else
        m14_value_6 = 0;

    matrix.m14 = m14_value_6;

    auto m23_property_value_7 = JS::js_undefined();
    if (arg0.is_object())
        m23_property_value_7 = TRY(arg0.as_object().get("m23"_utf16_fly_string));

    double m23_value_7;

    if (!m23_property_value_7.is_undefined())
        m23_value_7 = TRY(m23_property_value_7.to_double(vm));

    else
        m23_value_7 = 0;

    matrix.m23 = m23_value_7;

    auto m24_property_value_8 = JS::js_undefined();
    if (arg0.is_object())
        m24_property_value_8 = TRY(arg0.as_object().get("m24"_utf16_fly_string));

    double m24_value_8;

    if (!m24_property_value_8.is_undefined())
        m24_value_8 = TRY(m24_property_value_8.to_double(vm));

    else
        m24_value_8 = 0;

    matrix.m24 = m24_value_8;

    auto m31_property_value_9 = JS::js_undefined();
    if (arg0.is_object())
        m31_property_value_9 = TRY(arg0.as_object().get("m31"_utf16_fly_string));

    double m31_value_9;

    if (!m31_property_value_9.is_undefined())
        m31_value_9 = TRY(m31_property_value_9.to_double(vm));

    else
        m31_value_9 = 0;

    matrix.m31 = m31_value_9;

    auto m32_property_value_10 = JS::js_undefined();
    if (arg0.is_object())
        m32_property_value_10 = TRY(arg0.as_object().get("m32"_utf16_fly_string));

    double m32_value_10;

    if (!m32_property_value_10.is_undefined())
        m32_value_10 = TRY(m32_property_value_10.to_double(vm));

    else
        m32_value_10 = 0;

    matrix.m32 = m32_value_10;

    auto m33_property_value_11 = JS::js_undefined();
    if (arg0.is_object())
        m33_property_value_11 = TRY(arg0.as_object().get("m33"_utf16_fly_string));

    double m33_value_11;

    if (!m33_property_value_11.is_undefined())
        m33_value_11 = TRY(m33_property_value_11.to_double(vm));

    else
        m33_value_11 = 1;

    matrix.m33 = m33_value_11;

    auto m34_property_value_12 = JS::js_undefined();
    if (arg0.is_object())
        m34_property_value_12 = TRY(arg0.as_object().get("m34"_utf16_fly_string));

    double m34_value_12;

    if (!m34_property_value_12.is_undefined())
        m34_value_12 = TRY(m34_property_value_12.to_double(vm));

    else
        m34_value_12 = 0;

    matrix.m34 = m34_value_12;

    auto m43_property_value_13 = JS::js_undefined();
    if (arg0.is_object())
        m43_property_value_13 = TRY(arg0.as_object().get("m43"_utf16_fly_string));

    double m43_value_13;

    if (!m43_property_value_13.is_undefined())
        m43_value_13 = TRY(m43_property_value_13.to_double(vm));

    else
        m43_value_13 = 0;

    matrix.m43 = m43_value_13;

    auto m44_property_value_14 = JS::js_undefined();
    if (arg0.is_object())
        m44_property_value_14 = TRY(arg0.as_object().get("m44"_utf16_fly_string));

    double m44_value_14;

    if (!m44_property_value_14.is_undefined())
        m44_value_14 = TRY(m44_property_value_14.to_double(vm));

    else
        m44_value_14 = 1;

    matrix.m44 = m44_value_14;

    auto a_property_value_15 = JS::js_undefined();
    if (arg0.is_object())
        a_property_value_15 = TRY(arg0.as_object().get("a"_utf16_fly_string));

    if (!a_property_value_15.is_undefined()) {

    Optional<double> a_value_15;

    if (!a_property_value_15.is_undefined())
        a_value_15 = TRY(a_property_value_15.to_double(vm));


    matrix.a = a_value_15;

    }

    auto b_property_value_16 = JS::js_undefined();
    if (arg0.is_object())
        b_property_value_16 = TRY(arg0.as_object().get("b"_utf16_fly_string));

    if (!b_property_value_16.is_undefined()) {

    Optional<double> b_value_16;

    if (!b_property_value_16.is_undefined())
        b_value_16 = TRY(b_property_value_16.to_double(vm));


    matrix.b = b_value_16;

    }

    auto c_property_value_17 = JS::js_undefined();
    if (arg0.is_object())
        c_property_value_17 = TRY(arg0.as_object().get("c"_utf16_fly_string));

    if (!c_property_value_17.is_undefined()) {

    Optional<double> c_value_17;

    if (!c_property_value_17.is_undefined())
        c_value_17 = TRY(c_property_value_17.to_double(vm));


    matrix.c = c_value_17;

    }

    auto d_property_value_18 = JS::js_undefined();
    if (arg0.is_object())
        d_property_value_18 = TRY(arg0.as_object().get("d"_utf16_fly_string));

    if (!d_property_value_18.is_undefined()) {

    Optional<double> d_value_18;

    if (!d_property_value_18.is_undefined())
        d_value_18 = TRY(d_property_value_18.to_double(vm));


    matrix.d = d_value_18;

    }

    auto e_property_value_19 = JS::js_undefined();
    if (arg0.is_object())
        e_property_value_19 = TRY(arg0.as_object().get("e"_utf16_fly_string));

    if (!e_property_value_19.is_undefined()) {

    Optional<double> e_value_19;

    if (!e_property_value_19.is_undefined())
        e_value_19 = TRY(e_property_value_19.to_double(vm));


    matrix.e = e_value_19;

    }

    auto f_property_value_20 = JS::js_undefined();
    if (arg0.is_object())
        f_property_value_20 = TRY(arg0.as_object().get("f"_utf16_fly_string));

    if (!f_property_value_20.is_undefined()) {

    Optional<double> f_value_20;

    if (!f_property_value_20.is_undefined())
        f_value_20 = TRY(f_property_value_20.to_double(vm));


    matrix.f = f_value_20;

    }

    auto m11_property_value_21 = JS::js_undefined();
    if (arg0.is_object())
        m11_property_value_21 = TRY(arg0.as_object().get("m11"_utf16_fly_string));

    if (!m11_property_value_21.is_undefined()) {

    Optional<double> m11_value_21;

    if (!m11_property_value_21.is_undefined())
        m11_value_21 = TRY(m11_property_value_21.to_double(vm));


    matrix.m11 = m11_value_21;

    }

    auto m12_property_value_22 = JS::js_undefined();
    if (arg0.is_object())
        m12_property_value_22 = TRY(arg0.as_object().get("m12"_utf16_fly_string));

    if (!m12_property_value_22.is_undefined()) {

    Optional<double> m12_value_22;

    if (!m12_property_value_22.is_undefined())
        m12_value_22 = TRY(m12_property_value_22.to_double(vm));


    matrix.m12 = m12_value_22;

    }

    auto m21_property_value_23 = JS::js_undefined();
    if (arg0.is_object())
        m21_property_value_23 = TRY(arg0.as_object().get("m21"_utf16_fly_string));

    if (!m21_property_value_23.is_undefined()) {

    Optional<double> m21_value_23;

    if (!m21_property_value_23.is_undefined())
        m21_value_23 = TRY(m21_property_value_23.to_double(vm));


    matrix.m21 = m21_value_23;

    }

    auto m22_property_value_24 = JS::js_undefined();
    if (arg0.is_object())
        m22_property_value_24 = TRY(arg0.as_object().get("m22"_utf16_fly_string));

    if (!m22_property_value_24.is_undefined()) {

    Optional<double> m22_value_24;

    if (!m22_property_value_24.is_undefined())
        m22_value_24 = TRY(m22_property_value_24.to_double(vm));


    matrix.m22 = m22_value_24;

    }

    auto m41_property_value_25 = JS::js_undefined();
    if (arg0.is_object())
        m41_property_value_25 = TRY(arg0.as_object().get("m41"_utf16_fly_string));

    if (!m41_property_value_25.is_undefined()) {

    Optional<double> m41_value_25;

    if (!m41_property_value_25.is_undefined())
        m41_value_25 = TRY(m41_property_value_25.to_double(vm));


    matrix.m41 = m41_value_25;

    }

    auto m42_property_value_26 = JS::js_undefined();
    if (arg0.is_object())
        m42_property_value_26 = TRY(arg0.as_object().get("m42"_utf16_fly_string));

    if (!m42_property_value_26.is_undefined()) {

    Optional<double> m42_value_26;

    if (!m42_property_value_26.is_undefined())
        m42_value_26 = TRY(m42_property_value_26.to_double(vm));


    matrix.m42 = m42_value_26;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->matrix_transform(matrix); }));

    return &const_cast<DOMPoint&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMPointReadOnlyPrototype::to_json)
{
    WebIDL::log_trace(vm, "DOMPointReadOnlyPrototype::to_json");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

    auto x_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->x(); }));

    JS::Value x_retval_wrapped;

    x_retval_wrapped = JS::Value(x_retval);

    MUST(result->create_data_property("x"_utf16_fly_string, x_retval_wrapped));

    auto y_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->y(); }));

    JS::Value y_retval_wrapped;

    y_retval_wrapped = JS::Value(y_retval);

    MUST(result->create_data_property("y"_utf16_fly_string, y_retval_wrapped));

    auto z_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->z(); }));

    JS::Value z_retval_wrapped;

    z_retval_wrapped = JS::Value(z_retval);

    MUST(result->create_data_property("z"_utf16_fly_string, z_retval_wrapped));

    auto w_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->w(); }));

    JS::Value w_retval_wrapped;

    w_retval_wrapped = JS::Value(w_retval);

    MUST(result->create_data_property("w"_utf16_fly_string, w_retval_wrapped));

    return result;
}

} // namespace Web::Bindings
