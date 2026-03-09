
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
#include <LibWeb/Bindings/DOMMatrixPrototype.h>
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

GC_DEFINE_ALLOCATOR(DOMMatrixPrototype);

DOMMatrixPrototype::DOMMatrixPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

DOMMatrixPrototype::~DOMMatrixPrototype()
{
}

void DOMMatrixPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<DOMMatrixReadOnlyPrototype>(realm, "DOMMatrixReadOnly"_fly_string));


    auto native_a_getter = JS::NativeFunction::create(realm, a_getter, 0, "a"_utf16_fly_string, &realm, "get"sv);

    auto native_a_setter = JS::NativeFunction::create(realm, a_setter, 1, "a"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("a"_utf16_fly_string, native_a_getter, native_a_setter, default_attributes);

    auto native_b_getter = JS::NativeFunction::create(realm, b_getter, 0, "b"_utf16_fly_string, &realm, "get"sv);

    auto native_b_setter = JS::NativeFunction::create(realm, b_setter, 1, "b"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("b"_utf16_fly_string, native_b_getter, native_b_setter, default_attributes);

    auto native_c_getter = JS::NativeFunction::create(realm, c_getter, 0, "c"_utf16_fly_string, &realm, "get"sv);

    auto native_c_setter = JS::NativeFunction::create(realm, c_setter, 1, "c"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("c"_utf16_fly_string, native_c_getter, native_c_setter, default_attributes);

    auto native_d_getter = JS::NativeFunction::create(realm, d_getter, 0, "d"_utf16_fly_string, &realm, "get"sv);

    auto native_d_setter = JS::NativeFunction::create(realm, d_setter, 1, "d"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("d"_utf16_fly_string, native_d_getter, native_d_setter, default_attributes);

    auto native_e_getter = JS::NativeFunction::create(realm, e_getter, 0, "e"_utf16_fly_string, &realm, "get"sv);

    auto native_e_setter = JS::NativeFunction::create(realm, e_setter, 1, "e"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("e"_utf16_fly_string, native_e_getter, native_e_setter, default_attributes);

    auto native_f_getter = JS::NativeFunction::create(realm, f_getter, 0, "f"_utf16_fly_string, &realm, "get"sv);

    auto native_f_setter = JS::NativeFunction::create(realm, f_setter, 1, "f"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("f"_utf16_fly_string, native_f_getter, native_f_setter, default_attributes);

    auto native_m11_getter = JS::NativeFunction::create(realm, m11_getter, 0, "m11"_utf16_fly_string, &realm, "get"sv);

    auto native_m11_setter = JS::NativeFunction::create(realm, m11_setter, 1, "m11"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("m11"_utf16_fly_string, native_m11_getter, native_m11_setter, default_attributes);

    auto native_m12_getter = JS::NativeFunction::create(realm, m12_getter, 0, "m12"_utf16_fly_string, &realm, "get"sv);

    auto native_m12_setter = JS::NativeFunction::create(realm, m12_setter, 1, "m12"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("m12"_utf16_fly_string, native_m12_getter, native_m12_setter, default_attributes);

    auto native_m13_getter = JS::NativeFunction::create(realm, m13_getter, 0, "m13"_utf16_fly_string, &realm, "get"sv);

    auto native_m13_setter = JS::NativeFunction::create(realm, m13_setter, 1, "m13"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("m13"_utf16_fly_string, native_m13_getter, native_m13_setter, default_attributes);

    auto native_m14_getter = JS::NativeFunction::create(realm, m14_getter, 0, "m14"_utf16_fly_string, &realm, "get"sv);

    auto native_m14_setter = JS::NativeFunction::create(realm, m14_setter, 1, "m14"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("m14"_utf16_fly_string, native_m14_getter, native_m14_setter, default_attributes);

    auto native_m21_getter = JS::NativeFunction::create(realm, m21_getter, 0, "m21"_utf16_fly_string, &realm, "get"sv);

    auto native_m21_setter = JS::NativeFunction::create(realm, m21_setter, 1, "m21"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("m21"_utf16_fly_string, native_m21_getter, native_m21_setter, default_attributes);

    auto native_m22_getter = JS::NativeFunction::create(realm, m22_getter, 0, "m22"_utf16_fly_string, &realm, "get"sv);

    auto native_m22_setter = JS::NativeFunction::create(realm, m22_setter, 1, "m22"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("m22"_utf16_fly_string, native_m22_getter, native_m22_setter, default_attributes);

    auto native_m23_getter = JS::NativeFunction::create(realm, m23_getter, 0, "m23"_utf16_fly_string, &realm, "get"sv);

    auto native_m23_setter = JS::NativeFunction::create(realm, m23_setter, 1, "m23"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("m23"_utf16_fly_string, native_m23_getter, native_m23_setter, default_attributes);

    auto native_m24_getter = JS::NativeFunction::create(realm, m24_getter, 0, "m24"_utf16_fly_string, &realm, "get"sv);

    auto native_m24_setter = JS::NativeFunction::create(realm, m24_setter, 1, "m24"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("m24"_utf16_fly_string, native_m24_getter, native_m24_setter, default_attributes);

    auto native_m31_getter = JS::NativeFunction::create(realm, m31_getter, 0, "m31"_utf16_fly_string, &realm, "get"sv);

    auto native_m31_setter = JS::NativeFunction::create(realm, m31_setter, 1, "m31"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("m31"_utf16_fly_string, native_m31_getter, native_m31_setter, default_attributes);

    auto native_m32_getter = JS::NativeFunction::create(realm, m32_getter, 0, "m32"_utf16_fly_string, &realm, "get"sv);

    auto native_m32_setter = JS::NativeFunction::create(realm, m32_setter, 1, "m32"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("m32"_utf16_fly_string, native_m32_getter, native_m32_setter, default_attributes);

    auto native_m33_getter = JS::NativeFunction::create(realm, m33_getter, 0, "m33"_utf16_fly_string, &realm, "get"sv);

    auto native_m33_setter = JS::NativeFunction::create(realm, m33_setter, 1, "m33"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("m33"_utf16_fly_string, native_m33_getter, native_m33_setter, default_attributes);

    auto native_m34_getter = JS::NativeFunction::create(realm, m34_getter, 0, "m34"_utf16_fly_string, &realm, "get"sv);

    auto native_m34_setter = JS::NativeFunction::create(realm, m34_setter, 1, "m34"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("m34"_utf16_fly_string, native_m34_getter, native_m34_setter, default_attributes);

    auto native_m41_getter = JS::NativeFunction::create(realm, m41_getter, 0, "m41"_utf16_fly_string, &realm, "get"sv);

    auto native_m41_setter = JS::NativeFunction::create(realm, m41_setter, 1, "m41"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("m41"_utf16_fly_string, native_m41_getter, native_m41_setter, default_attributes);

    auto native_m42_getter = JS::NativeFunction::create(realm, m42_getter, 0, "m42"_utf16_fly_string, &realm, "get"sv);

    auto native_m42_setter = JS::NativeFunction::create(realm, m42_setter, 1, "m42"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("m42"_utf16_fly_string, native_m42_getter, native_m42_setter, default_attributes);

    auto native_m43_getter = JS::NativeFunction::create(realm, m43_getter, 0, "m43"_utf16_fly_string, &realm, "get"sv);

    auto native_m43_setter = JS::NativeFunction::create(realm, m43_setter, 1, "m43"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("m43"_utf16_fly_string, native_m43_getter, native_m43_setter, default_attributes);

    auto native_m44_getter = JS::NativeFunction::create(realm, m44_getter, 0, "m44"_utf16_fly_string, &realm, "get"sv);

    auto native_m44_setter = JS::NativeFunction::create(realm, m44_setter, 1, "m44"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("m44"_utf16_fly_string, native_m44_getter, native_m44_setter, default_attributes);

    define_native_function(realm, "multiplySelf"_utf16_fly_string, multiply_self, 0, default_attributes);

    define_native_function(realm, "preMultiplySelf"_utf16_fly_string, pre_multiply_self, 0, default_attributes);

    define_native_function(realm, "translateSelf"_utf16_fly_string, translate_self, 0, default_attributes);

    define_native_function(realm, "scaleSelf"_utf16_fly_string, scale_self, 0, default_attributes);

    define_native_function(realm, "scale3dSelf"_utf16_fly_string, scale3d_self, 0, default_attributes);

    define_native_function(realm, "rotateSelf"_utf16_fly_string, rotate_self, 0, default_attributes);

    define_native_function(realm, "rotateFromVectorSelf"_utf16_fly_string, rotate_from_vector_self, 0, default_attributes);

    define_native_function(realm, "rotateAxisAngleSelf"_utf16_fly_string, rotate_axis_angle_self, 0, default_attributes);

    define_native_function(realm, "skewXSelf"_utf16_fly_string, skew_x_self, 0, default_attributes);

    define_native_function(realm, "skewYSelf"_utf16_fly_string, skew_y_self, 0, default_attributes);

    define_native_function(realm, "invertSelf"_utf16_fly_string, invert_self, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "DOMMatrix"_string), JS::Attribute::Configurable);

    if (is<HTML::Window>(realm.global_object())) {

    define_native_function(realm, "setMatrixValue"_utf16_fly_string, set_matrix_value, 1, default_attributes);

    }

    Base::initialize(realm);

}

void DOMMatrixPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Geometry::DOMMatrix*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Geometry::DOMMatrix>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMMatrix");
    return static_cast<Geometry::DOMMatrix*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::a_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::a_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->a(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::a_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::a_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_a(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::b_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::b_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->b(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::b_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::b_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_b(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::c_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::c_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->c(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::c_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::c_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_c(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::d_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::d_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->d(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::d_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::d_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_d(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::e_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::e_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->e(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::e_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::e_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_e(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::f_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::f_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->f(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::f_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::f_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_f(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m11_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m11_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m11(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m11_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m11_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_m11(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m12_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m12_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m12(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m12_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m12_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_m12(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m13_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m13_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m13(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m13_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m13_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_m13(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m14_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m14_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m14(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m14_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m14_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_m14(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m21_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m21_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m21(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m21_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m21_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_m21(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m22_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m22_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m22(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m22_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m22_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_m22(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m23_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m23_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m23(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m23_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m23_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_m23(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m24_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m24_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m24(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m24_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m24_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_m24(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m31_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m31_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m31(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m31_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m31_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_m31(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m32_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m32_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m32(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m32_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m32_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_m32(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m33_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m33_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m33(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m33_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m33_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_m33(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m34_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m34_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m34(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m34_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m34_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_m34(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m41_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m41_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m41(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m41_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m41_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_m41(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m42_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m42_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m42(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m42_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m42_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_m42(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m43_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m43_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m43(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m43_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m43_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_m43(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m44_getter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m44_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->m44(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::m44_setter)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::m44_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_m44(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::multiply_self)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::multiply_self");
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

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->multiply_self(other); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::pre_multiply_self)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::pre_multiply_self");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMMatrixInit");

    DOMMatrixInit other {};

    auto is2d_property_value_46 = JS::js_undefined();
    if (arg0.is_object())
        is2d_property_value_46 = TRY(arg0.as_object().get("is2D"_utf16_fly_string));

    if (!is2d_property_value_46.is_undefined()) {

    Optional<bool> is2d_value_46;

    if (!is2d_property_value_46.is_undefined())

    is2d_value_46 = is2d_property_value_46.to_boolean();

    other.is2d = is2d_value_46;

    }

    auto m13_property_value_47 = JS::js_undefined();
    if (arg0.is_object())
        m13_property_value_47 = TRY(arg0.as_object().get("m13"_utf16_fly_string));

    double m13_value_47;

    if (!m13_property_value_47.is_undefined())
        m13_value_47 = TRY(m13_property_value_47.to_double(vm));

    else
        m13_value_47 = 0;

    other.m13 = m13_value_47;

    auto m14_property_value_48 = JS::js_undefined();
    if (arg0.is_object())
        m14_property_value_48 = TRY(arg0.as_object().get("m14"_utf16_fly_string));

    double m14_value_48;

    if (!m14_property_value_48.is_undefined())
        m14_value_48 = TRY(m14_property_value_48.to_double(vm));

    else
        m14_value_48 = 0;

    other.m14 = m14_value_48;

    auto m23_property_value_49 = JS::js_undefined();
    if (arg0.is_object())
        m23_property_value_49 = TRY(arg0.as_object().get("m23"_utf16_fly_string));

    double m23_value_49;

    if (!m23_property_value_49.is_undefined())
        m23_value_49 = TRY(m23_property_value_49.to_double(vm));

    else
        m23_value_49 = 0;

    other.m23 = m23_value_49;

    auto m24_property_value_50 = JS::js_undefined();
    if (arg0.is_object())
        m24_property_value_50 = TRY(arg0.as_object().get("m24"_utf16_fly_string));

    double m24_value_50;

    if (!m24_property_value_50.is_undefined())
        m24_value_50 = TRY(m24_property_value_50.to_double(vm));

    else
        m24_value_50 = 0;

    other.m24 = m24_value_50;

    auto m31_property_value_51 = JS::js_undefined();
    if (arg0.is_object())
        m31_property_value_51 = TRY(arg0.as_object().get("m31"_utf16_fly_string));

    double m31_value_51;

    if (!m31_property_value_51.is_undefined())
        m31_value_51 = TRY(m31_property_value_51.to_double(vm));

    else
        m31_value_51 = 0;

    other.m31 = m31_value_51;

    auto m32_property_value_52 = JS::js_undefined();
    if (arg0.is_object())
        m32_property_value_52 = TRY(arg0.as_object().get("m32"_utf16_fly_string));

    double m32_value_52;

    if (!m32_property_value_52.is_undefined())
        m32_value_52 = TRY(m32_property_value_52.to_double(vm));

    else
        m32_value_52 = 0;

    other.m32 = m32_value_52;

    auto m33_property_value_53 = JS::js_undefined();
    if (arg0.is_object())
        m33_property_value_53 = TRY(arg0.as_object().get("m33"_utf16_fly_string));

    double m33_value_53;

    if (!m33_property_value_53.is_undefined())
        m33_value_53 = TRY(m33_property_value_53.to_double(vm));

    else
        m33_value_53 = 1;

    other.m33 = m33_value_53;

    auto m34_property_value_54 = JS::js_undefined();
    if (arg0.is_object())
        m34_property_value_54 = TRY(arg0.as_object().get("m34"_utf16_fly_string));

    double m34_value_54;

    if (!m34_property_value_54.is_undefined())
        m34_value_54 = TRY(m34_property_value_54.to_double(vm));

    else
        m34_value_54 = 0;

    other.m34 = m34_value_54;

    auto m43_property_value_55 = JS::js_undefined();
    if (arg0.is_object())
        m43_property_value_55 = TRY(arg0.as_object().get("m43"_utf16_fly_string));

    double m43_value_55;

    if (!m43_property_value_55.is_undefined())
        m43_value_55 = TRY(m43_property_value_55.to_double(vm));

    else
        m43_value_55 = 0;

    other.m43 = m43_value_55;

    auto m44_property_value_56 = JS::js_undefined();
    if (arg0.is_object())
        m44_property_value_56 = TRY(arg0.as_object().get("m44"_utf16_fly_string));

    double m44_value_56;

    if (!m44_property_value_56.is_undefined())
        m44_value_56 = TRY(m44_property_value_56.to_double(vm));

    else
        m44_value_56 = 1;

    other.m44 = m44_value_56;

    auto a_property_value_57 = JS::js_undefined();
    if (arg0.is_object())
        a_property_value_57 = TRY(arg0.as_object().get("a"_utf16_fly_string));

    if (!a_property_value_57.is_undefined()) {

    Optional<double> a_value_57;

    if (!a_property_value_57.is_undefined())
        a_value_57 = TRY(a_property_value_57.to_double(vm));


    other.a = a_value_57;

    }

    auto b_property_value_58 = JS::js_undefined();
    if (arg0.is_object())
        b_property_value_58 = TRY(arg0.as_object().get("b"_utf16_fly_string));

    if (!b_property_value_58.is_undefined()) {

    Optional<double> b_value_58;

    if (!b_property_value_58.is_undefined())
        b_value_58 = TRY(b_property_value_58.to_double(vm));


    other.b = b_value_58;

    }

    auto c_property_value_59 = JS::js_undefined();
    if (arg0.is_object())
        c_property_value_59 = TRY(arg0.as_object().get("c"_utf16_fly_string));

    if (!c_property_value_59.is_undefined()) {

    Optional<double> c_value_59;

    if (!c_property_value_59.is_undefined())
        c_value_59 = TRY(c_property_value_59.to_double(vm));


    other.c = c_value_59;

    }

    auto d_property_value_60 = JS::js_undefined();
    if (arg0.is_object())
        d_property_value_60 = TRY(arg0.as_object().get("d"_utf16_fly_string));

    if (!d_property_value_60.is_undefined()) {

    Optional<double> d_value_60;

    if (!d_property_value_60.is_undefined())
        d_value_60 = TRY(d_property_value_60.to_double(vm));


    other.d = d_value_60;

    }

    auto e_property_value_61 = JS::js_undefined();
    if (arg0.is_object())
        e_property_value_61 = TRY(arg0.as_object().get("e"_utf16_fly_string));

    if (!e_property_value_61.is_undefined()) {

    Optional<double> e_value_61;

    if (!e_property_value_61.is_undefined())
        e_value_61 = TRY(e_property_value_61.to_double(vm));


    other.e = e_value_61;

    }

    auto f_property_value_62 = JS::js_undefined();
    if (arg0.is_object())
        f_property_value_62 = TRY(arg0.as_object().get("f"_utf16_fly_string));

    if (!f_property_value_62.is_undefined()) {

    Optional<double> f_value_62;

    if (!f_property_value_62.is_undefined())
        f_value_62 = TRY(f_property_value_62.to_double(vm));


    other.f = f_value_62;

    }

    auto m11_property_value_63 = JS::js_undefined();
    if (arg0.is_object())
        m11_property_value_63 = TRY(arg0.as_object().get("m11"_utf16_fly_string));

    if (!m11_property_value_63.is_undefined()) {

    Optional<double> m11_value_63;

    if (!m11_property_value_63.is_undefined())
        m11_value_63 = TRY(m11_property_value_63.to_double(vm));


    other.m11 = m11_value_63;

    }

    auto m12_property_value_64 = JS::js_undefined();
    if (arg0.is_object())
        m12_property_value_64 = TRY(arg0.as_object().get("m12"_utf16_fly_string));

    if (!m12_property_value_64.is_undefined()) {

    Optional<double> m12_value_64;

    if (!m12_property_value_64.is_undefined())
        m12_value_64 = TRY(m12_property_value_64.to_double(vm));


    other.m12 = m12_value_64;

    }

    auto m21_property_value_65 = JS::js_undefined();
    if (arg0.is_object())
        m21_property_value_65 = TRY(arg0.as_object().get("m21"_utf16_fly_string));

    if (!m21_property_value_65.is_undefined()) {

    Optional<double> m21_value_65;

    if (!m21_property_value_65.is_undefined())
        m21_value_65 = TRY(m21_property_value_65.to_double(vm));


    other.m21 = m21_value_65;

    }

    auto m22_property_value_66 = JS::js_undefined();
    if (arg0.is_object())
        m22_property_value_66 = TRY(arg0.as_object().get("m22"_utf16_fly_string));

    if (!m22_property_value_66.is_undefined()) {

    Optional<double> m22_value_66;

    if (!m22_property_value_66.is_undefined())
        m22_value_66 = TRY(m22_property_value_66.to_double(vm));


    other.m22 = m22_value_66;

    }

    auto m41_property_value_67 = JS::js_undefined();
    if (arg0.is_object())
        m41_property_value_67 = TRY(arg0.as_object().get("m41"_utf16_fly_string));

    if (!m41_property_value_67.is_undefined()) {

    Optional<double> m41_value_67;

    if (!m41_property_value_67.is_undefined())
        m41_value_67 = TRY(m41_property_value_67.to_double(vm));


    other.m41 = m41_value_67;

    }

    auto m42_property_value_68 = JS::js_undefined();
    if (arg0.is_object())
        m42_property_value_68 = TRY(arg0.as_object().get("m42"_utf16_fly_string));

    if (!m42_property_value_68.is_undefined()) {

    Optional<double> m42_value_68;

    if (!m42_property_value_68.is_undefined())
        m42_value_68 = TRY(m42_property_value_68.to_double(vm));


    other.m42 = m42_value_68;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->pre_multiply_self(other); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::translate_self)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::translate_self");
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

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->translate_self(tx, ty, tz); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::scale_self)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::scale_self");
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

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scale_self(scale_x, scale_y, scale_z, origin_x, origin_y, origin_z); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::scale3d_self)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::scale3d_self");
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

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scale3d_self(scale, origin_x, origin_y, origin_z); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::rotate_self)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::rotate_self");
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


    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->rotate_self(rot_x, rot_y, rot_z); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::rotate_from_vector_self)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::rotate_from_vector_self");
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

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->rotate_from_vector_self(x, y); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::rotate_axis_angle_self)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::rotate_axis_angle_self");
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

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->rotate_axis_angle_self(x, y, z, angle); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::skew_x_self)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::skew_x_self");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    double sx;

    if (!arg0.is_undefined())
        sx = TRY(arg0.to_double(vm));

    else
        sx = 0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->skew_x_self(sx); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::skew_y_self)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::skew_y_self");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    double sy;

    if (!arg0.is_undefined())
        sy = TRY(arg0.to_double(vm));

    else
        sy = 0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->skew_y_self(sy); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::invert_self)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::invert_self");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->invert_self(); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMMatrixPrototype::set_matrix_value)
{
    WebIDL::log_trace(vm, "DOMMatrixPrototype::set_matrix_value");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setMatrixValue");

    auto arg0 = vm.argument(0);

    String transform_list;
    if (!false || !arg0.is_null()) {
        transform_list = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_matrix_value(transform_list); }));

    return &const_cast<DOMMatrix&>(*retval);

}

} // namespace Web::Bindings
