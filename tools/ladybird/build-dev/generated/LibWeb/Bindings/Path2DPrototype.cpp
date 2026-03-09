
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
#include <LibWeb/Bindings/Path2DPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Path2D.h>

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

GC_DEFINE_ALLOCATOR(Path2DPrototype);

Path2DPrototype::Path2DPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

Path2DPrototype::~Path2DPrototype()
{
}

void Path2DPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    define_native_function(realm, "addPath"_utf16_fly_string, add_path, 1, default_attributes);

    define_native_function(realm, "closePath"_utf16_fly_string, close_path, 0, default_attributes);

    define_native_function(realm, "moveTo"_utf16_fly_string, move_to, 2, default_attributes);

    define_native_function(realm, "lineTo"_utf16_fly_string, line_to, 2, default_attributes);

    define_native_function(realm, "quadraticCurveTo"_utf16_fly_string, quadratic_curve_to, 4, default_attributes);

    define_native_function(realm, "bezierCurveTo"_utf16_fly_string, bezier_curve_to, 6, default_attributes);

    define_native_function(realm, "arcTo"_utf16_fly_string, arc_to, 5, default_attributes);

    define_native_function(realm, "rect"_utf16_fly_string, rect, 4, default_attributes);

    define_native_function(realm, "roundRect"_utf16_fly_string, round_rect, 4, default_attributes);

    define_native_function(realm, "arc"_utf16_fly_string, arc, 5, default_attributes);

    define_native_function(realm, "ellipse"_utf16_fly_string, ellipse, 7, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Path2D"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void Path2DPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::Path2D*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::Path2D>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Path2D");
    return static_cast<HTML::Path2D*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(Path2DPrototype::add_path)
{
    WebIDL::log_trace(vm, "Path2DPrototype::add_path");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "addPath");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Path2D>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Path2D");

    auto& path = static_cast<Path2D&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMMatrix2DInit");

    DOMMatrix2DInit transform {};

    auto a_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        a_property_value_0 = TRY(arg1.as_object().get("a"_utf16_fly_string));

    if (!a_property_value_0.is_undefined()) {

    Optional<double> a_value_0;

    if (!a_property_value_0.is_undefined())
        a_value_0 = TRY(a_property_value_0.to_double(vm));


    transform.a = a_value_0;

    }

    auto b_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        b_property_value_1 = TRY(arg1.as_object().get("b"_utf16_fly_string));

    if (!b_property_value_1.is_undefined()) {

    Optional<double> b_value_1;

    if (!b_property_value_1.is_undefined())
        b_value_1 = TRY(b_property_value_1.to_double(vm));


    transform.b = b_value_1;

    }

    auto c_property_value_2 = JS::js_undefined();
    if (arg1.is_object())
        c_property_value_2 = TRY(arg1.as_object().get("c"_utf16_fly_string));

    if (!c_property_value_2.is_undefined()) {

    Optional<double> c_value_2;

    if (!c_property_value_2.is_undefined())
        c_value_2 = TRY(c_property_value_2.to_double(vm));


    transform.c = c_value_2;

    }

    auto d_property_value_3 = JS::js_undefined();
    if (arg1.is_object())
        d_property_value_3 = TRY(arg1.as_object().get("d"_utf16_fly_string));

    if (!d_property_value_3.is_undefined()) {

    Optional<double> d_value_3;

    if (!d_property_value_3.is_undefined())
        d_value_3 = TRY(d_property_value_3.to_double(vm));


    transform.d = d_value_3;

    }

    auto e_property_value_4 = JS::js_undefined();
    if (arg1.is_object())
        e_property_value_4 = TRY(arg1.as_object().get("e"_utf16_fly_string));

    if (!e_property_value_4.is_undefined()) {

    Optional<double> e_value_4;

    if (!e_property_value_4.is_undefined())
        e_value_4 = TRY(e_property_value_4.to_double(vm));


    transform.e = e_value_4;

    }

    auto f_property_value_5 = JS::js_undefined();
    if (arg1.is_object())
        f_property_value_5 = TRY(arg1.as_object().get("f"_utf16_fly_string));

    if (!f_property_value_5.is_undefined()) {

    Optional<double> f_value_5;

    if (!f_property_value_5.is_undefined())
        f_value_5 = TRY(f_property_value_5.to_double(vm));


    transform.f = f_value_5;

    }

    auto m11_property_value_6 = JS::js_undefined();
    if (arg1.is_object())
        m11_property_value_6 = TRY(arg1.as_object().get("m11"_utf16_fly_string));

    if (!m11_property_value_6.is_undefined()) {

    Optional<double> m11_value_6;

    if (!m11_property_value_6.is_undefined())
        m11_value_6 = TRY(m11_property_value_6.to_double(vm));


    transform.m11 = m11_value_6;

    }

    auto m12_property_value_7 = JS::js_undefined();
    if (arg1.is_object())
        m12_property_value_7 = TRY(arg1.as_object().get("m12"_utf16_fly_string));

    if (!m12_property_value_7.is_undefined()) {

    Optional<double> m12_value_7;

    if (!m12_property_value_7.is_undefined())
        m12_value_7 = TRY(m12_property_value_7.to_double(vm));


    transform.m12 = m12_value_7;

    }

    auto m21_property_value_8 = JS::js_undefined();
    if (arg1.is_object())
        m21_property_value_8 = TRY(arg1.as_object().get("m21"_utf16_fly_string));

    if (!m21_property_value_8.is_undefined()) {

    Optional<double> m21_value_8;

    if (!m21_property_value_8.is_undefined())
        m21_value_8 = TRY(m21_property_value_8.to_double(vm));


    transform.m21 = m21_value_8;

    }

    auto m22_property_value_9 = JS::js_undefined();
    if (arg1.is_object())
        m22_property_value_9 = TRY(arg1.as_object().get("m22"_utf16_fly_string));

    if (!m22_property_value_9.is_undefined()) {

    Optional<double> m22_value_9;

    if (!m22_property_value_9.is_undefined())
        m22_value_9 = TRY(m22_property_value_9.to_double(vm));


    transform.m22 = m22_value_9;

    }

    auto m41_property_value_10 = JS::js_undefined();
    if (arg1.is_object())
        m41_property_value_10 = TRY(arg1.as_object().get("m41"_utf16_fly_string));

    if (!m41_property_value_10.is_undefined()) {

    Optional<double> m41_value_10;

    if (!m41_property_value_10.is_undefined())
        m41_value_10 = TRY(m41_property_value_10.to_double(vm));


    transform.m41 = m41_value_10;

    }

    auto m42_property_value_11 = JS::js_undefined();
    if (arg1.is_object())
        m42_property_value_11 = TRY(arg1.as_object().get("m42"_utf16_fly_string));

    if (!m42_property_value_11.is_undefined()) {

    Optional<double> m42_value_11;

    if (!m42_property_value_11.is_undefined())
        m42_value_11 = TRY(m42_property_value_11.to_double(vm));


    transform.m42 = m42_value_11;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->add_path(path, transform); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(Path2DPrototype::close_path)
{
    WebIDL::log_trace(vm, "Path2DPrototype::close_path");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->close_path(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(Path2DPrototype::move_to)
{
    WebIDL::log_trace(vm, "Path2DPrototype::move_to");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "moveTo", "2");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->move_to(x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(Path2DPrototype::line_to)
{
    WebIDL::log_trace(vm, "Path2DPrototype::line_to");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "lineTo", "2");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->line_to(x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(Path2DPrototype::quadratic_curve_to)
{
    WebIDL::log_trace(vm, "Path2DPrototype::quadratic_curve_to");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "quadraticCurveTo", "4");

    auto arg0 = vm.argument(0);

    double cpx = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double cpy = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double x = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double y = TRY(arg3.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->quadratic_curve_to(cpx, cpy, x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(Path2DPrototype::bezier_curve_to)
{
    WebIDL::log_trace(vm, "Path2DPrototype::bezier_curve_to");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 6)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "bezierCurveTo", "6");

    auto arg0 = vm.argument(0);

    double cp1x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double cp1y = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double cp2x = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double cp2y = TRY(arg3.to_double(vm));

    auto arg4 = vm.argument(4);

    double x = TRY(arg4.to_double(vm));

    auto arg5 = vm.argument(5);

    double y = TRY(arg5.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bezier_curve_to(cp1x, cp1y, cp2x, cp2y, x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(Path2DPrototype::arc_to)
{
    WebIDL::log_trace(vm, "Path2DPrototype::arc_to");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "arcTo", "5");

    auto arg0 = vm.argument(0);

    double x1 = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y1 = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double x2 = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double y2 = TRY(arg3.to_double(vm));

    auto arg4 = vm.argument(4);

    double radius = TRY(arg4.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->arc_to(x1, y1, x2, y2, radius); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(Path2DPrototype::rect)
{
    WebIDL::log_trace(vm, "Path2DPrototype::rect");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "rect", "4");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double w = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double h = TRY(arg3.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->rect(x, y, w, h); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(Path2DPrototype::round_rect)
{
    WebIDL::log_trace(vm, "Path2DPrototype::round_rect");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "roundRect", "4");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double w = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double h = TRY(arg3.to_double(vm));

    auto arg4 = vm.argument(4);

    auto arg4_to_dictionary = [&vm, &realm](JS::Value arg4) -> JS::ThrowCompletionOr<DOMPointInit> {
        // This might be unused.
        (void)realm;

    if (!arg4.is_nullish() && !arg4.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMPointInit");

    DOMPointInit dictionary_union_type {};

    auto w_property_value_0 = JS::js_undefined();
    if (arg4.is_object())
        w_property_value_0 = TRY(arg4.as_object().get("w"_utf16_fly_string));

    double w_value_0;

    if (!w_property_value_0.is_undefined())
        w_value_0 = TRY(w_property_value_0.to_double(vm));

    else
        w_value_0 = 1;

    dictionary_union_type.w = w_value_0;

    auto x_property_value_1 = JS::js_undefined();
    if (arg4.is_object())
        x_property_value_1 = TRY(arg4.as_object().get("x"_utf16_fly_string));

    double x_value_1;

    if (!x_property_value_1.is_undefined())
        x_value_1 = TRY(x_property_value_1.to_double(vm));

    else
        x_value_1 = 0;

    dictionary_union_type.x = x_value_1;

    auto y_property_value_2 = JS::js_undefined();
    if (arg4.is_object())
        y_property_value_2 = TRY(arg4.as_object().get("y"_utf16_fly_string));

    double y_value_2;

    if (!y_property_value_2.is_undefined())
        y_value_2 = TRY(y_property_value_2.to_double(vm));

    else
        y_value_2 = 0;

    dictionary_union_type.y = y_value_2;

    auto z_property_value_3 = JS::js_undefined();
    if (arg4.is_object())
        z_property_value_3 = TRY(arg4.as_object().get("z"_utf16_fly_string));

    double z_value_3;

    if (!z_property_value_3.is_undefined())
        z_value_3 = TRY(z_property_value_3.to_double(vm));

    else
        z_value_3 = 0;

    dictionary_union_type.z = z_value_3;

        return dictionary_union_type;
    };

    auto arg4_to_variant = [&vm, &realm, &arg4_to_dictionary](JS::Value arg4) -> JS::ThrowCompletionOr<Variant<double, DOMPointInit, Vector<Variant<double, DOMPointInit>>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg4.is_nullish())
            return Variant<double, DOMPointInit, Vector<Variant<double, DOMPointInit>>> { TRY(arg4_to_dictionary(arg4)) };

        if (arg4.is_object()) {
            [[maybe_unused]] auto& arg4_object = arg4.as_object();

        auto method = TRY(arg4.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg4_iterator1 = TRY(JS::get_iterator_from_method(vm, arg4, *method));

    Vector<Variant<double, DOMPointInit>> radii;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg4_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    auto next_item1_to_dictionary = [&vm, &realm](JS::Value next_item1) -> JS::ThrowCompletionOr<DOMPointInit> {
        // This might be unused.
        (void)realm;

    if (!next_item1.is_nullish() && !next_item1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMPointInit");

    DOMPointInit dictionary_union_type {};

    auto w_property_value_4 = JS::js_undefined();
    if (next_item1.is_object())
        w_property_value_4 = TRY(next_item1.as_object().get("w"_utf16_fly_string));

    double w_value_4;

    if (!w_property_value_4.is_undefined())
        w_value_4 = TRY(w_property_value_4.to_double(vm));

    else
        w_value_4 = 1;

    dictionary_union_type.w = w_value_4;

    auto x_property_value_5 = JS::js_undefined();
    if (next_item1.is_object())
        x_property_value_5 = TRY(next_item1.as_object().get("x"_utf16_fly_string));

    double x_value_5;

    if (!x_property_value_5.is_undefined())
        x_value_5 = TRY(x_property_value_5.to_double(vm));

    else
        x_value_5 = 0;

    dictionary_union_type.x = x_value_5;

    auto y_property_value_6 = JS::js_undefined();
    if (next_item1.is_object())
        y_property_value_6 = TRY(next_item1.as_object().get("y"_utf16_fly_string));

    double y_value_6;

    if (!y_property_value_6.is_undefined())
        y_value_6 = TRY(y_property_value_6.to_double(vm));

    else
        y_value_6 = 0;

    dictionary_union_type.y = y_value_6;

    auto z_property_value_7 = JS::js_undefined();
    if (next_item1.is_object())
        z_property_value_7 = TRY(next_item1.as_object().get("z"_utf16_fly_string));

    double z_value_7;

    if (!z_property_value_7.is_undefined())
        z_value_7 = TRY(z_property_value_7.to_double(vm));

    else
        z_value_7 = 0;

    dictionary_union_type.z = z_value_7;

        return dictionary_union_type;
    };

    auto next_item1_to_variant = [&vm, &realm, &next_item1_to_dictionary](JS::Value next_item1) -> JS::ThrowCompletionOr<Variant<double, DOMPointInit>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (next_item1.is_nullish())
            return Variant<double, DOMPointInit> { TRY(next_item1_to_dictionary(next_item1)) };

        if (next_item1.is_object()) {
            [[maybe_unused]] auto& next_item1_object = next_item1.as_object();

        return Variant<double, DOMPointInit> { TRY(next_item1_to_dictionary(next_item1)) };

        }

        if (next_item1.is_number()) {

    double next_item1_number = TRY(next_item1.to_double(vm));

            return { next_item1_number };
        }

    double next_item1_number = TRY(next_item1.to_double(vm));

        return { next_item1_number };

    };

    Variant<double, DOMPointInit> sequence_item1 = TRY(next_item1_to_variant(next_item1));

    radii.append(sequence_item1);
    }


            return radii;
        }

        return Variant<double, DOMPointInit, Vector<Variant<double, DOMPointInit>>> { TRY(arg4_to_dictionary(arg4)) };

        }

        if (arg4.is_number()) {

    double arg4_number = TRY(arg4.to_double(vm));

            return { arg4_number };
        }

    double arg4_number = TRY(arg4.to_double(vm));

        return { arg4_number };

    };

    Variant<double, DOMPointInit, Vector<Variant<double, DOMPointInit>>> radii = arg4.is_undefined() ? 0 : TRY(arg4_to_variant(arg4));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->round_rect(x, y, w, h, radii); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(Path2DPrototype::arc)
{
    WebIDL::log_trace(vm, "Path2DPrototype::arc");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "arc", "5");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double radius = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double start_angle = TRY(arg3.to_double(vm));

    auto arg4 = vm.argument(4);

    double end_angle = TRY(arg4.to_double(vm));

    auto arg5 = vm.argument(5);

    bool counterclockwise;

    if (!arg5.is_undefined())

    counterclockwise = arg5.to_boolean();

    else
        counterclockwise = static_cast<bool>(false);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->arc(x, y, radius, start_angle, end_angle, counterclockwise); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(Path2DPrototype::ellipse)
{
    WebIDL::log_trace(vm, "Path2DPrototype::ellipse");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 7)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "ellipse", "7");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double radius_x = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double radius_y = TRY(arg3.to_double(vm));

    auto arg4 = vm.argument(4);

    double rotation = TRY(arg4.to_double(vm));

    auto arg5 = vm.argument(5);

    double start_angle = TRY(arg5.to_double(vm));

    auto arg6 = vm.argument(6);

    double end_angle = TRY(arg6.to_double(vm));

    auto arg7 = vm.argument(7);

    bool counterclockwise;

    if (!arg7.is_undefined())

    counterclockwise = arg7.to_boolean();

    else
        counterclockwise = static_cast<bool>(false);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ellipse(x, y, radius_x, radius_y, rotation, start_angle, end_angle, counterclockwise); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
