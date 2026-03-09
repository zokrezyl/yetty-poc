
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
#include <LibWeb/Bindings/DOMMatrixReadOnlyPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrixReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrix.h>

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

GC_DEFINE_ALLOCATOR(DOMMatrixReadOnlyPrototype);

DOMMatrixReadOnlyPrototype::DOMMatrixReadOnlyPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

DOMMatrixReadOnlyPrototype::~DOMMatrixReadOnlyPrototype()
{
}

void DOMMatrixReadOnlyPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_a_getter = JS::NativeFunction::create(realm, a_getter, 0, "a"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_a_setter;

    define_direct_accessor("a"_utf16_fly_string, native_a_getter, native_a_setter, default_attributes);

    auto native_b_getter = JS::NativeFunction::create(realm, b_getter, 0, "b"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_b_setter;

    define_direct_accessor("b"_utf16_fly_string, native_b_getter, native_b_setter, default_attributes);

    auto native_c_getter = JS::NativeFunction::create(realm, c_getter, 0, "c"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_c_setter;

    define_direct_accessor("c"_utf16_fly_string, native_c_getter, native_c_setter, default_attributes);

    auto native_d_getter = JS::NativeFunction::create(realm, d_getter, 0, "d"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_d_setter;

    define_direct_accessor("d"_utf16_fly_string, native_d_getter, native_d_setter, default_attributes);

    auto native_e_getter = JS::NativeFunction::create(realm, e_getter, 0, "e"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_e_setter;

    define_direct_accessor("e"_utf16_fly_string, native_e_getter, native_e_setter, default_attributes);

    auto native_f_getter = JS::NativeFunction::create(realm, f_getter, 0, "f"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_f_setter;

    define_direct_accessor("f"_utf16_fly_string, native_f_getter, native_f_setter, default_attributes);

    auto native_m11_getter = JS::NativeFunction::create(realm, m11_getter, 0, "m11"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_m11_setter;

    define_direct_accessor("m11"_utf16_fly_string, native_m11_getter, native_m11_setter, default_attributes);

    auto native_m12_getter = JS::NativeFunction::create(realm, m12_getter, 0, "m12"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_m12_setter;

    define_direct_accessor("m12"_utf16_fly_string, native_m12_getter, native_m12_setter, default_attributes);

    auto native_m13_getter = JS::NativeFunction::create(realm, m13_getter, 0, "m13"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_m13_setter;

    define_direct_accessor("m13"_utf16_fly_string, native_m13_getter, native_m13_setter, default_attributes);

    auto native_m14_getter = JS::NativeFunction::create(realm, m14_getter, 0, "m14"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_m14_setter;

    define_direct_accessor("m14"_utf16_fly_string, native_m14_getter, native_m14_setter, default_attributes);

    auto native_m21_getter = JS::NativeFunction::create(realm, m21_getter, 0, "m21"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_m21_setter;

    define_direct_accessor("m21"_utf16_fly_string, native_m21_getter, native_m21_setter, default_attributes);

    auto native_m22_getter = JS::NativeFunction::create(realm, m22_getter, 0, "m22"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_m22_setter;

    define_direct_accessor("m22"_utf16_fly_string, native_m22_getter, native_m22_setter, default_attributes);

    auto native_m23_getter = JS::NativeFunction::create(realm, m23_getter, 0, "m23"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_m23_setter;

    define_direct_accessor("m23"_utf16_fly_string, native_m23_getter, native_m23_setter, default_attributes);

    auto native_m24_getter = JS::NativeFunction::create(realm, m24_getter, 0, "m24"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_m24_setter;

    define_direct_accessor("m24"_utf16_fly_string, native_m24_getter, native_m24_setter, default_attributes);

    auto native_m31_getter = JS::NativeFunction::create(realm, m31_getter, 0, "m31"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_m31_setter;

    define_direct_accessor("m31"_utf16_fly_string, native_m31_getter, native_m31_setter, default_attributes);

    auto native_m32_getter = JS::NativeFunction::create(realm, m32_getter, 0, "m32"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_m32_setter;

    define_direct_accessor("m32"_utf16_fly_string, native_m32_getter, native_m32_setter, default_attributes);

    auto native_m33_getter = JS::NativeFunction::create(realm, m33_getter, 0, "m33"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_m33_setter;

    define_direct_accessor("m33"_utf16_fly_string, native_m33_getter, native_m33_setter, default_attributes);

    auto native_m34_getter = JS::NativeFunction::create(realm, m34_getter, 0, "m34"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_m34_setter;

    define_direct_accessor("m34"_utf16_fly_string, native_m34_getter, native_m34_setter, default_attributes);

    auto native_m41_getter = JS::NativeFunction::create(realm, m41_getter, 0, "m41"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_m41_setter;

    define_direct_accessor("m41"_utf16_fly_string, native_m41_getter, native_m41_setter, default_attributes);

    auto native_m42_getter = JS::NativeFunction::create(realm, m42_getter, 0, "m42"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_m42_setter;

    define_direct_accessor("m42"_utf16_fly_string, native_m42_getter, native_m42_setter, default_attributes);

    auto native_m43_getter = JS::NativeFunction::create(realm, m43_getter, 0, "m43"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_m43_setter;

    define_direct_accessor("m43"_utf16_fly_string, native_m43_getter, native_m43_setter, default_attributes);

    auto native_m44_getter = JS::NativeFunction::create(realm, m44_getter, 0, "m44"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_m44_setter;

    define_direct_accessor("m44"_utf16_fly_string, native_m44_getter, native_m44_setter, default_attributes);

    auto native_is2d_getter = JS::NativeFunction::create(realm, is2d_getter, 0, "is2D"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_is2d_setter;

    define_direct_accessor("is2D"_utf16_fly_string, native_is2d_getter, native_is2d_setter, default_attributes);

    auto native_is_identity_getter = JS::NativeFunction::create(realm, is_identity_getter, 0, "isIdentity"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_is_identity_setter;

    define_direct_accessor("isIdentity"_utf16_fly_string, native_is_identity_getter, native_is_identity_setter, default_attributes);

    define_native_function(realm, "translate"_utf16_fly_string, translate, 0, default_attributes);

    define_native_function(realm, "scale"_utf16_fly_string, scale, 0, default_attributes);

    define_native_function(realm, "scaleNonUniform"_utf16_fly_string, scale_non_uniform, 0, default_attributes);

    define_native_function(realm, "scale3d"_utf16_fly_string, scale3d, 0, default_attributes);

    define_native_function(realm, "rotate"_utf16_fly_string, rotate, 0, default_attributes);

    define_native_function(realm, "rotateFromVector"_utf16_fly_string, rotate_from_vector, 0, default_attributes);

    define_native_function(realm, "rotateAxisAngle"_utf16_fly_string, rotate_axis_angle, 0, default_attributes);

    define_native_function(realm, "skewX"_utf16_fly_string, skew_x, 0, default_attributes);

    define_native_function(realm, "skewY"_utf16_fly_string, skew_y, 0, default_attributes);

    define_native_function(realm, "multiply"_utf16_fly_string, multiply, 0, default_attributes);

    define_native_function(realm, "flipX"_utf16_fly_string, flip_x, 0, default_attributes);

    define_native_function(realm, "flipY"_utf16_fly_string, flip_y, 0, default_attributes);

    define_native_function(realm, "inverse"_utf16_fly_string, inverse, 0, default_attributes);

    define_native_function(realm, "transformPoint"_utf16_fly_string, transform_point, 0, default_attributes);

    define_native_function(realm, "toFloat32Array"_utf16_fly_string, to_float32_array, 0, default_attributes);

    define_native_function(realm, "toFloat64Array"_utf16_fly_string, to_float64_array, 0, default_attributes);

    define_native_function(realm, "toJSON"_utf16_fly_string, to_json, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "DOMMatrixReadOnly"_string), JS::Attribute::Configurable);

    if (is<HTML::Window>(realm.global_object())) {

    define_native_function(realm, "toString"_utf16_fly_string, to_string, 0, default_attributes);

    }

    Base::initialize(realm);

}

void DOMMatrixReadOnlyPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    if (is<HTML::Window>(realm.global_object())) {

    object.define_native_function(realm, "toString"_utf16_fly_string, to_string, 0, default_attributes);

    }

}

[[maybe_unused]] static JS::ThrowCompletionOr<Geometry::DOMMatrixReadOnly*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Geometry::DOMMatrixReadOnly>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMMatrixReadOnly");
    return static_cast<Geometry::DOMMatrixReadOnly*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::a_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::a_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->a(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::b_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::b_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->b(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::c_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::c_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->c(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::d_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::d_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->d(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::e_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::e_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->e(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::f_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::f_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->f(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::m11_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::m11_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m11(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::m12_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::m12_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m12(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::m13_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::m13_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m13(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::m14_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::m14_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m14(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::m21_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::m21_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m21(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::m22_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::m22_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m22(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::m23_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::m23_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m23(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::m24_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::m24_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m24(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::m31_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::m31_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m31(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::m32_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::m32_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m32(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::m33_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::m33_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m33(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::m34_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::m34_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m34(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::m41_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::m41_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m41(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::m42_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::m42_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m42(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::m43_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::m43_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m43(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::m44_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::m44_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m44(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::is2d_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::is2d_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is2d(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::is_identity_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::is_identity_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_identity(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::translate)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::translate");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    double tx;

    if (!arg0.is_undefined())
        tx = TRY(arg0.to_double(vm));

    else
        tx = 0;

    auto arg1 = vm.argument(1);

    double ty;

    if (!arg1.is_undefined())
        ty = TRY(arg1.to_double(vm));

    else
        ty = 0;

    auto arg2 = vm.argument(2);

    double tz;

    if (!arg2.is_undefined())
        tz = TRY(arg2.to_double(vm));

    else
        tz = 0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->translate(tx, ty, tz); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::scale)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::scale");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    double scale_x;

    if (!arg0.is_undefined())
        scale_x = TRY(arg0.to_double(vm));

    else
        scale_x = 1;

    auto arg1 = vm.argument(1);

    Optional<double> scale_y;

    if (!arg1.is_undefined())
        scale_y = TRY(arg1.to_double(vm));


    auto arg2 = vm.argument(2);

    double scale_z;

    if (!arg2.is_undefined())
        scale_z = TRY(arg2.to_double(vm));

    else
        scale_z = 1;

    auto arg3 = vm.argument(3);

    double origin_x;

    if (!arg3.is_undefined())
        origin_x = TRY(arg3.to_double(vm));

    else
        origin_x = 0;

    auto arg4 = vm.argument(4);

    double origin_y;

    if (!arg4.is_undefined())
        origin_y = TRY(arg4.to_double(vm));

    else
        origin_y = 0;

    auto arg5 = vm.argument(5);

    double origin_z;

    if (!arg5.is_undefined())
        origin_z = TRY(arg5.to_double(vm));

    else
        origin_z = 0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scale(scale_x, scale_y, scale_z, origin_x, origin_y, origin_z); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::scale_non_uniform)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::scale_non_uniform");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    double scale_x;

    if (!arg0.is_undefined())
        scale_x = TRY(arg0.to_double(vm));

    else
        scale_x = 1;

    auto arg1 = vm.argument(1);

    double scale_y;

    if (!arg1.is_undefined())
        scale_y = TRY(arg1.to_double(vm));

    else
        scale_y = 1;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scale_non_uniform(scale_x, scale_y); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::scale3d)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::scale3d");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    double scale;

    if (!arg0.is_undefined())
        scale = TRY(arg0.to_double(vm));

    else
        scale = 1;

    auto arg1 = vm.argument(1);

    double origin_x;

    if (!arg1.is_undefined())
        origin_x = TRY(arg1.to_double(vm));

    else
        origin_x = 0;

    auto arg2 = vm.argument(2);

    double origin_y;

    if (!arg2.is_undefined())
        origin_y = TRY(arg2.to_double(vm));

    else
        origin_y = 0;

    auto arg3 = vm.argument(3);

    double origin_z;

    if (!arg3.is_undefined())
        origin_z = TRY(arg3.to_double(vm));

    else
        origin_z = 0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scale3d(scale, origin_x, origin_y, origin_z); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::rotate)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::rotate");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    double rot_x;

    if (!arg0.is_undefined())
        rot_x = TRY(arg0.to_double(vm));

    else
        rot_x = 0;

    auto arg1 = vm.argument(1);

    Optional<double> rot_y;

    if (!arg1.is_undefined())
        rot_y = TRY(arg1.to_double(vm));


    auto arg2 = vm.argument(2);

    Optional<double> rot_z;

    if (!arg2.is_undefined())
        rot_z = TRY(arg2.to_double(vm));


    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->rotate(rot_x, rot_y, rot_z); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::rotate_from_vector)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::rotate_from_vector");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

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

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->rotate_from_vector(x, y); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::rotate_axis_angle)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::rotate_axis_angle");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

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

    double angle;

    if (!arg3.is_undefined())
        angle = TRY(arg3.to_double(vm));

    else
        angle = 0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->rotate_axis_angle(x, y, z, angle); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::skew_x)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::skew_x");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    double sx;

    if (!arg0.is_undefined())
        sx = TRY(arg0.to_double(vm));

    else
        sx = 0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->skew_x(sx); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::skew_y)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::skew_y");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    double sy;

    if (!arg0.is_undefined())
        sy = TRY(arg0.to_double(vm));

    else
        sy = 0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->skew_y(sy); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::multiply)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::multiply");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMMatrixInit");

    DOMMatrixInit other {};

    auto is2d_property_value_23 = JS::js_undefined();
    if (arg0.is_object())
        is2d_property_value_23 = TRY(arg0.as_object().get("is2D"_utf16_fly_string));

    if (!is2d_property_value_23.is_undefined()) {

    Optional<bool> is2d_value_23;

    if (!is2d_property_value_23.is_undefined())

    is2d_value_23 = is2d_property_value_23.to_boolean();

    other.is2d = is2d_value_23;

    }

    auto m13_property_value_24 = JS::js_undefined();
    if (arg0.is_object())
        m13_property_value_24 = TRY(arg0.as_object().get("m13"_utf16_fly_string));

    double m13_value_24;

    if (!m13_property_value_24.is_undefined())
        m13_value_24 = TRY(m13_property_value_24.to_double(vm));

    else
        m13_value_24 = 0;

    other.m13 = m13_value_24;

    auto m14_property_value_25 = JS::js_undefined();
    if (arg0.is_object())
        m14_property_value_25 = TRY(arg0.as_object().get("m14"_utf16_fly_string));

    double m14_value_25;

    if (!m14_property_value_25.is_undefined())
        m14_value_25 = TRY(m14_property_value_25.to_double(vm));

    else
        m14_value_25 = 0;

    other.m14 = m14_value_25;

    auto m23_property_value_26 = JS::js_undefined();
    if (arg0.is_object())
        m23_property_value_26 = TRY(arg0.as_object().get("m23"_utf16_fly_string));

    double m23_value_26;

    if (!m23_property_value_26.is_undefined())
        m23_value_26 = TRY(m23_property_value_26.to_double(vm));

    else
        m23_value_26 = 0;

    other.m23 = m23_value_26;

    auto m24_property_value_27 = JS::js_undefined();
    if (arg0.is_object())
        m24_property_value_27 = TRY(arg0.as_object().get("m24"_utf16_fly_string));

    double m24_value_27;

    if (!m24_property_value_27.is_undefined())
        m24_value_27 = TRY(m24_property_value_27.to_double(vm));

    else
        m24_value_27 = 0;

    other.m24 = m24_value_27;

    auto m31_property_value_28 = JS::js_undefined();
    if (arg0.is_object())
        m31_property_value_28 = TRY(arg0.as_object().get("m31"_utf16_fly_string));

    double m31_value_28;

    if (!m31_property_value_28.is_undefined())
        m31_value_28 = TRY(m31_property_value_28.to_double(vm));

    else
        m31_value_28 = 0;

    other.m31 = m31_value_28;

    auto m32_property_value_29 = JS::js_undefined();
    if (arg0.is_object())
        m32_property_value_29 = TRY(arg0.as_object().get("m32"_utf16_fly_string));

    double m32_value_29;

    if (!m32_property_value_29.is_undefined())
        m32_value_29 = TRY(m32_property_value_29.to_double(vm));

    else
        m32_value_29 = 0;

    other.m32 = m32_value_29;

    auto m33_property_value_30 = JS::js_undefined();
    if (arg0.is_object())
        m33_property_value_30 = TRY(arg0.as_object().get("m33"_utf16_fly_string));

    double m33_value_30;

    if (!m33_property_value_30.is_undefined())
        m33_value_30 = TRY(m33_property_value_30.to_double(vm));

    else
        m33_value_30 = 1;

    other.m33 = m33_value_30;

    auto m34_property_value_31 = JS::js_undefined();
    if (arg0.is_object())
        m34_property_value_31 = TRY(arg0.as_object().get("m34"_utf16_fly_string));

    double m34_value_31;

    if (!m34_property_value_31.is_undefined())
        m34_value_31 = TRY(m34_property_value_31.to_double(vm));

    else
        m34_value_31 = 0;

    other.m34 = m34_value_31;

    auto m43_property_value_32 = JS::js_undefined();
    if (arg0.is_object())
        m43_property_value_32 = TRY(arg0.as_object().get("m43"_utf16_fly_string));

    double m43_value_32;

    if (!m43_property_value_32.is_undefined())
        m43_value_32 = TRY(m43_property_value_32.to_double(vm));

    else
        m43_value_32 = 0;

    other.m43 = m43_value_32;

    auto m44_property_value_33 = JS::js_undefined();
    if (arg0.is_object())
        m44_property_value_33 = TRY(arg0.as_object().get("m44"_utf16_fly_string));

    double m44_value_33;

    if (!m44_property_value_33.is_undefined())
        m44_value_33 = TRY(m44_property_value_33.to_double(vm));

    else
        m44_value_33 = 1;

    other.m44 = m44_value_33;

    auto a_property_value_34 = JS::js_undefined();
    if (arg0.is_object())
        a_property_value_34 = TRY(arg0.as_object().get("a"_utf16_fly_string));

    if (!a_property_value_34.is_undefined()) {

    Optional<double> a_value_34;

    if (!a_property_value_34.is_undefined())
        a_value_34 = TRY(a_property_value_34.to_double(vm));


    other.a = a_value_34;

    }

    auto b_property_value_35 = JS::js_undefined();
    if (arg0.is_object())
        b_property_value_35 = TRY(arg0.as_object().get("b"_utf16_fly_string));

    if (!b_property_value_35.is_undefined()) {

    Optional<double> b_value_35;

    if (!b_property_value_35.is_undefined())
        b_value_35 = TRY(b_property_value_35.to_double(vm));


    other.b = b_value_35;

    }

    auto c_property_value_36 = JS::js_undefined();
    if (arg0.is_object())
        c_property_value_36 = TRY(arg0.as_object().get("c"_utf16_fly_string));

    if (!c_property_value_36.is_undefined()) {

    Optional<double> c_value_36;

    if (!c_property_value_36.is_undefined())
        c_value_36 = TRY(c_property_value_36.to_double(vm));


    other.c = c_value_36;

    }

    auto d_property_value_37 = JS::js_undefined();
    if (arg0.is_object())
        d_property_value_37 = TRY(arg0.as_object().get("d"_utf16_fly_string));

    if (!d_property_value_37.is_undefined()) {

    Optional<double> d_value_37;

    if (!d_property_value_37.is_undefined())
        d_value_37 = TRY(d_property_value_37.to_double(vm));


    other.d = d_value_37;

    }

    auto e_property_value_38 = JS::js_undefined();
    if (arg0.is_object())
        e_property_value_38 = TRY(arg0.as_object().get("e"_utf16_fly_string));

    if (!e_property_value_38.is_undefined()) {

    Optional<double> e_value_38;

    if (!e_property_value_38.is_undefined())
        e_value_38 = TRY(e_property_value_38.to_double(vm));


    other.e = e_value_38;

    }

    auto f_property_value_39 = JS::js_undefined();
    if (arg0.is_object())
        f_property_value_39 = TRY(arg0.as_object().get("f"_utf16_fly_string));

    if (!f_property_value_39.is_undefined()) {

    Optional<double> f_value_39;

    if (!f_property_value_39.is_undefined())
        f_value_39 = TRY(f_property_value_39.to_double(vm));


    other.f = f_value_39;

    }

    auto m11_property_value_40 = JS::js_undefined();
    if (arg0.is_object())
        m11_property_value_40 = TRY(arg0.as_object().get("m11"_utf16_fly_string));

    if (!m11_property_value_40.is_undefined()) {

    Optional<double> m11_value_40;

    if (!m11_property_value_40.is_undefined())
        m11_value_40 = TRY(m11_property_value_40.to_double(vm));


    other.m11 = m11_value_40;

    }

    auto m12_property_value_41 = JS::js_undefined();
    if (arg0.is_object())
        m12_property_value_41 = TRY(arg0.as_object().get("m12"_utf16_fly_string));

    if (!m12_property_value_41.is_undefined()) {

    Optional<double> m12_value_41;

    if (!m12_property_value_41.is_undefined())
        m12_value_41 = TRY(m12_property_value_41.to_double(vm));


    other.m12 = m12_value_41;

    }

    auto m21_property_value_42 = JS::js_undefined();
    if (arg0.is_object())
        m21_property_value_42 = TRY(arg0.as_object().get("m21"_utf16_fly_string));

    if (!m21_property_value_42.is_undefined()) {

    Optional<double> m21_value_42;

    if (!m21_property_value_42.is_undefined())
        m21_value_42 = TRY(m21_property_value_42.to_double(vm));


    other.m21 = m21_value_42;

    }

    auto m22_property_value_43 = JS::js_undefined();
    if (arg0.is_object())
        m22_property_value_43 = TRY(arg0.as_object().get("m22"_utf16_fly_string));

    if (!m22_property_value_43.is_undefined()) {

    Optional<double> m22_value_43;

    if (!m22_property_value_43.is_undefined())
        m22_value_43 = TRY(m22_property_value_43.to_double(vm));


    other.m22 = m22_value_43;

    }

    auto m41_property_value_44 = JS::js_undefined();
    if (arg0.is_object())
        m41_property_value_44 = TRY(arg0.as_object().get("m41"_utf16_fly_string));

    if (!m41_property_value_44.is_undefined()) {

    Optional<double> m41_value_44;

    if (!m41_property_value_44.is_undefined())
        m41_value_44 = TRY(m41_property_value_44.to_double(vm));


    other.m41 = m41_value_44;

    }

    auto m42_property_value_45 = JS::js_undefined();
    if (arg0.is_object())
        m42_property_value_45 = TRY(arg0.as_object().get("m42"_utf16_fly_string));

    if (!m42_property_value_45.is_undefined()) {

    Optional<double> m42_value_45;

    if (!m42_property_value_45.is_undefined())
        m42_value_45 = TRY(m42_property_value_45.to_double(vm));


    other.m42 = m42_value_45;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->multiply(other); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::flip_x)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::flip_x");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->flip_x(); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::flip_y)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::flip_y");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->flip_y(); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::inverse)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::inverse");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->inverse(); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::transform_point)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::transform_point");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMPointInit");

    DOMPointInit point {};

    auto w_property_value_46 = JS::js_undefined();
    if (arg0.is_object())
        w_property_value_46 = TRY(arg0.as_object().get("w"_utf16_fly_string));

    double w_value_46;

    if (!w_property_value_46.is_undefined())
        w_value_46 = TRY(w_property_value_46.to_double(vm));

    else
        w_value_46 = 1;

    point.w = w_value_46;

    auto x_property_value_47 = JS::js_undefined();
    if (arg0.is_object())
        x_property_value_47 = TRY(arg0.as_object().get("x"_utf16_fly_string));

    double x_value_47;

    if (!x_property_value_47.is_undefined())
        x_value_47 = TRY(x_property_value_47.to_double(vm));

    else
        x_value_47 = 0;

    point.x = x_value_47;

    auto y_property_value_48 = JS::js_undefined();
    if (arg0.is_object())
        y_property_value_48 = TRY(arg0.as_object().get("y"_utf16_fly_string));

    double y_value_48;

    if (!y_property_value_48.is_undefined())
        y_value_48 = TRY(y_property_value_48.to_double(vm));

    else
        y_value_48 = 0;

    point.y = y_value_48;

    auto z_property_value_49 = JS::js_undefined();
    if (arg0.is_object())
        z_property_value_49 = TRY(arg0.as_object().get("z"_utf16_fly_string));

    double z_value_49;

    if (!z_property_value_49.is_undefined())
        z_value_49 = TRY(z_property_value_49.to_double(vm));

    else
        z_value_49 = 0;

    point.z = z_value_49;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->transform_point(point); }));

    return &const_cast<DOMPoint&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::to_float32_array)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::to_float32_array");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->to_float32_array(); }));

    return &const_cast<JS::Float32Array&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::to_float64_array)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::to_float64_array");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->to_float64_array(); }));

    return &const_cast<JS::Float64Array&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::to_json)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::to_json");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

    auto a_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->a(); }));

    JS::Value a_retval_wrapped;

    a_retval_wrapped = JS::Value(a_retval);

    MUST(result->create_data_property("a"_utf16_fly_string, a_retval_wrapped));

    auto b_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->b(); }));

    JS::Value b_retval_wrapped;

    b_retval_wrapped = JS::Value(b_retval);

    MUST(result->create_data_property("b"_utf16_fly_string, b_retval_wrapped));

    auto c_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->c(); }));

    JS::Value c_retval_wrapped;

    c_retval_wrapped = JS::Value(c_retval);

    MUST(result->create_data_property("c"_utf16_fly_string, c_retval_wrapped));

    auto d_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->d(); }));

    JS::Value d_retval_wrapped;

    d_retval_wrapped = JS::Value(d_retval);

    MUST(result->create_data_property("d"_utf16_fly_string, d_retval_wrapped));

    auto e_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->e(); }));

    JS::Value e_retval_wrapped;

    e_retval_wrapped = JS::Value(e_retval);

    MUST(result->create_data_property("e"_utf16_fly_string, e_retval_wrapped));

    auto f_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->f(); }));

    JS::Value f_retval_wrapped;

    f_retval_wrapped = JS::Value(f_retval);

    MUST(result->create_data_property("f"_utf16_fly_string, f_retval_wrapped));

    auto m11_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m11(); }));

    JS::Value m11_retval_wrapped;

    m11_retval_wrapped = JS::Value(m11_retval);

    MUST(result->create_data_property("m11"_utf16_fly_string, m11_retval_wrapped));

    auto m12_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m12(); }));

    JS::Value m12_retval_wrapped;

    m12_retval_wrapped = JS::Value(m12_retval);

    MUST(result->create_data_property("m12"_utf16_fly_string, m12_retval_wrapped));

    auto m13_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m13(); }));

    JS::Value m13_retval_wrapped;

    m13_retval_wrapped = JS::Value(m13_retval);

    MUST(result->create_data_property("m13"_utf16_fly_string, m13_retval_wrapped));

    auto m14_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m14(); }));

    JS::Value m14_retval_wrapped;

    m14_retval_wrapped = JS::Value(m14_retval);

    MUST(result->create_data_property("m14"_utf16_fly_string, m14_retval_wrapped));

    auto m21_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m21(); }));

    JS::Value m21_retval_wrapped;

    m21_retval_wrapped = JS::Value(m21_retval);

    MUST(result->create_data_property("m21"_utf16_fly_string, m21_retval_wrapped));

    auto m22_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m22(); }));

    JS::Value m22_retval_wrapped;

    m22_retval_wrapped = JS::Value(m22_retval);

    MUST(result->create_data_property("m22"_utf16_fly_string, m22_retval_wrapped));

    auto m23_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m23(); }));

    JS::Value m23_retval_wrapped;

    m23_retval_wrapped = JS::Value(m23_retval);

    MUST(result->create_data_property("m23"_utf16_fly_string, m23_retval_wrapped));

    auto m24_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m24(); }));

    JS::Value m24_retval_wrapped;

    m24_retval_wrapped = JS::Value(m24_retval);

    MUST(result->create_data_property("m24"_utf16_fly_string, m24_retval_wrapped));

    auto m31_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m31(); }));

    JS::Value m31_retval_wrapped;

    m31_retval_wrapped = JS::Value(m31_retval);

    MUST(result->create_data_property("m31"_utf16_fly_string, m31_retval_wrapped));

    auto m32_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m32(); }));

    JS::Value m32_retval_wrapped;

    m32_retval_wrapped = JS::Value(m32_retval);

    MUST(result->create_data_property("m32"_utf16_fly_string, m32_retval_wrapped));

    auto m33_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m33(); }));

    JS::Value m33_retval_wrapped;

    m33_retval_wrapped = JS::Value(m33_retval);

    MUST(result->create_data_property("m33"_utf16_fly_string, m33_retval_wrapped));

    auto m34_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m34(); }));

    JS::Value m34_retval_wrapped;

    m34_retval_wrapped = JS::Value(m34_retval);

    MUST(result->create_data_property("m34"_utf16_fly_string, m34_retval_wrapped));

    auto m41_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m41(); }));

    JS::Value m41_retval_wrapped;

    m41_retval_wrapped = JS::Value(m41_retval);

    MUST(result->create_data_property("m41"_utf16_fly_string, m41_retval_wrapped));

    auto m42_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m42(); }));

    JS::Value m42_retval_wrapped;

    m42_retval_wrapped = JS::Value(m42_retval);

    MUST(result->create_data_property("m42"_utf16_fly_string, m42_retval_wrapped));

    auto m43_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m43(); }));

    JS::Value m43_retval_wrapped;

    m43_retval_wrapped = JS::Value(m43_retval);

    MUST(result->create_data_property("m43"_utf16_fly_string, m43_retval_wrapped));

    auto m44_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m44(); }));

    JS::Value m44_retval_wrapped;

    m44_retval_wrapped = JS::Value(m44_retval);

    MUST(result->create_data_property("m44"_utf16_fly_string, m44_retval_wrapped));

    auto is2d_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is2d(); }));

    JS::Value is2d_retval_wrapped;

    is2d_retval_wrapped = JS::Value(is2d_retval);

    MUST(result->create_data_property("is2D"_utf16_fly_string, is2d_retval_wrapped));

    auto is_identity_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_identity(); }));

    JS::Value is_identity_retval_wrapped;

    is_identity_retval_wrapped = JS::Value(is_identity_retval);

    MUST(result->create_data_property("isIdentity"_utf16_fly_string, is_identity_retval_wrapped));

    return result;
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixReadOnlyPrototype::to_string)
{
    WebIDL::log_trace(vm, "DOMMatrixReadOnlyPrototype::to_string");
    [[maybe_unused]] auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));


    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->to_string(); }));


    return JS::PrimitiveString::create(vm, move(retval));
}

} // namespace Web::Bindings
