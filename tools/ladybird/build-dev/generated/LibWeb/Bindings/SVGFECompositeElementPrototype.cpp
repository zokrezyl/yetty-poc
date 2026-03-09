
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
#include <LibWeb/Bindings/SVGFECompositeElementPrototype.h>
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

#if __has_include(<LibWeb/Bindings/SVGElementPrototype.h>)
#    include <LibWeb/Bindings/SVGElementPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGFECompositeElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGAnimatedEnumeration.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGAnimatedString.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGAnimatedLength.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGLength.h>

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

GC_DEFINE_ALLOCATOR(SVGFECompositeElementPrototype);

SVGFECompositeElementPrototype::SVGFECompositeElementPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

SVGFECompositeElementPrototype::~SVGFECompositeElementPrototype()
{
}

void SVGFECompositeElementPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<SVGElementPrototype>(realm, "SVGElement"_fly_string));


    auto native_in1_getter = JS::NativeFunction::create(realm, in1_getter, 0, "in1"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_in1_setter;

    define_direct_accessor("in1"_utf16_fly_string, native_in1_getter, native_in1_setter, default_attributes);

    auto native_in2_getter = JS::NativeFunction::create(realm, in2_getter, 0, "in2"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_in2_setter;

    define_direct_accessor("in2"_utf16_fly_string, native_in2_getter, native_in2_setter, default_attributes);

    auto native_operator_getter = JS::NativeFunction::create(realm, operator_getter, 0, "operator"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_operator_setter;

    define_direct_accessor("operator"_utf16_fly_string, native_operator_getter, native_operator_setter, default_attributes);

    auto native_k2_getter = JS::NativeFunction::create(realm, k2_getter, 0, "k2"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_k2_setter;

    define_direct_accessor("k2"_utf16_fly_string, native_k2_getter, native_k2_setter, default_attributes);

    auto native_k3_getter = JS::NativeFunction::create(realm, k3_getter, 0, "k3"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_k3_setter;

    define_direct_accessor("k3"_utf16_fly_string, native_k3_getter, native_k3_setter, default_attributes);

    auto native_k1_getter = JS::NativeFunction::create(realm, k1_getter, 0, "k1"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_k1_setter;

    define_direct_accessor("k1"_utf16_fly_string, native_k1_getter, native_k1_setter, default_attributes);

    auto native_k4_getter = JS::NativeFunction::create(realm, k4_getter, 0, "k4"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_k4_setter;

    define_direct_accessor("k4"_utf16_fly_string, native_k4_getter, native_k4_setter, default_attributes);

    auto native_x_getter = JS::NativeFunction::create(realm, x_getter, 0, "x"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_x_setter;

    define_direct_accessor("x"_utf16_fly_string, native_x_getter, native_x_setter, default_attributes);

    auto native_y_getter = JS::NativeFunction::create(realm, y_getter, 0, "y"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_y_setter;

    define_direct_accessor("y"_utf16_fly_string, native_y_getter, native_y_setter, default_attributes);

    auto native_width_getter = JS::NativeFunction::create(realm, width_getter, 0, "width"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_width_setter;

    define_direct_accessor("width"_utf16_fly_string, native_width_getter, native_width_setter, default_attributes);

    auto native_height_getter = JS::NativeFunction::create(realm, height_getter, 0, "height"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_height_setter;

    define_direct_accessor("height"_utf16_fly_string, native_height_getter, native_height_setter, default_attributes);

    auto native_result_getter = JS::NativeFunction::create(realm, result_getter, 0, "result"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_result_setter;

    define_direct_accessor("result"_utf16_fly_string, native_result_getter, native_result_setter, default_attributes);

    auto constant_SVG_FECOMPOSITE_OPERATOR_UNKNOWN_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0));

    define_direct_property("SVG_FECOMPOSITE_OPERATOR_UNKNOWN"_utf16_fly_string, constant_SVG_FECOMPOSITE_OPERATOR_UNKNOWN_value, JS::Attribute::Enumerable);

    auto constant_SVG_FECOMPOSITE_OPERATOR_OVER_value = JS::Value(static_cast<WebIDL::UnsignedShort>(1));

    define_direct_property("SVG_FECOMPOSITE_OPERATOR_OVER"_utf16_fly_string, constant_SVG_FECOMPOSITE_OPERATOR_OVER_value, JS::Attribute::Enumerable);

    auto constant_SVG_FECOMPOSITE_OPERATOR_IN_value = JS::Value(static_cast<WebIDL::UnsignedShort>(2));

    define_direct_property("SVG_FECOMPOSITE_OPERATOR_IN"_utf16_fly_string, constant_SVG_FECOMPOSITE_OPERATOR_IN_value, JS::Attribute::Enumerable);

    auto constant_SVG_FECOMPOSITE_OPERATOR_OUT_value = JS::Value(static_cast<WebIDL::UnsignedShort>(3));

    define_direct_property("SVG_FECOMPOSITE_OPERATOR_OUT"_utf16_fly_string, constant_SVG_FECOMPOSITE_OPERATOR_OUT_value, JS::Attribute::Enumerable);

    auto constant_SVG_FECOMPOSITE_OPERATOR_ATOP_value = JS::Value(static_cast<WebIDL::UnsignedShort>(4));

    define_direct_property("SVG_FECOMPOSITE_OPERATOR_ATOP"_utf16_fly_string, constant_SVG_FECOMPOSITE_OPERATOR_ATOP_value, JS::Attribute::Enumerable);

    auto constant_SVG_FECOMPOSITE_OPERATOR_XOR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(5));

    define_direct_property("SVG_FECOMPOSITE_OPERATOR_XOR"_utf16_fly_string, constant_SVG_FECOMPOSITE_OPERATOR_XOR_value, JS::Attribute::Enumerable);

    auto constant_SVG_FECOMPOSITE_OPERATOR_ARITHMETIC_value = JS::Value(static_cast<WebIDL::UnsignedShort>(6));

    define_direct_property("SVG_FECOMPOSITE_OPERATOR_ARITHMETIC"_utf16_fly_string, constant_SVG_FECOMPOSITE_OPERATOR_ARITHMETIC_value, JS::Attribute::Enumerable);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "SVGFECompositeElement"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void SVGFECompositeElementPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<SVG::SVGFECompositeElement*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<SVG::SVGFECompositeElement>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SVGFECompositeElement");
    return static_cast<SVG::SVGFECompositeElement*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(SVGFECompositeElementPrototype::in1_getter)
{
    WebIDL::log_trace(vm, "SVGFECompositeElementPrototype::in1_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->in1(); }));

    return &const_cast<SVGAnimatedString&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFECompositeElementPrototype::in2_getter)
{
    WebIDL::log_trace(vm, "SVGFECompositeElementPrototype::in2_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->in2(); }));

    return &const_cast<SVGAnimatedString&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFECompositeElementPrototype::operator_getter)
{
    WebIDL::log_trace(vm, "SVGFECompositeElementPrototype::operator_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->operator_for_bindings(); }));

    return &const_cast<SVGAnimatedEnumeration&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFECompositeElementPrototype::k2_getter)
{
    WebIDL::log_trace(vm, "SVGFECompositeElementPrototype::k2_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->k2(); }));

    return &const_cast<SVGAnimatedNumber&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFECompositeElementPrototype::k3_getter)
{
    WebIDL::log_trace(vm, "SVGFECompositeElementPrototype::k3_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->k3(); }));

    return &const_cast<SVGAnimatedNumber&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFECompositeElementPrototype::k1_getter)
{
    WebIDL::log_trace(vm, "SVGFECompositeElementPrototype::k1_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->k1(); }));

    return &const_cast<SVGAnimatedNumber&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFECompositeElementPrototype::k4_getter)
{
    WebIDL::log_trace(vm, "SVGFECompositeElementPrototype::k4_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->k4(); }));

    return &const_cast<SVGAnimatedNumber&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFECompositeElementPrototype::x_getter)
{
    WebIDL::log_trace(vm, "SVGFECompositeElementPrototype::x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->x(); }));

    return &const_cast<SVGAnimatedLength&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFECompositeElementPrototype::y_getter)
{
    WebIDL::log_trace(vm, "SVGFECompositeElementPrototype::y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->y(); }));

    return &const_cast<SVGAnimatedLength&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFECompositeElementPrototype::width_getter)
{
    WebIDL::log_trace(vm, "SVGFECompositeElementPrototype::width_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->width(); }));

    return &const_cast<SVGAnimatedLength&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFECompositeElementPrototype::height_getter)
{
    WebIDL::log_trace(vm, "SVGFECompositeElementPrototype::height_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->height(); }));

    return &const_cast<SVGAnimatedLength&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFECompositeElementPrototype::result_getter)
{
    WebIDL::log_trace(vm, "SVGFECompositeElementPrototype::result_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->result(); }));

    return &const_cast<SVGAnimatedString&>(*retval);

}

} // namespace Web::Bindings
