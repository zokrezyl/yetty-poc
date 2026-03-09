
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
#include <LibWeb/Bindings/SVGFEBlendElementPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGFEBlendElement.h>

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

GC_DEFINE_ALLOCATOR(SVGFEBlendElementPrototype);

SVGFEBlendElementPrototype::SVGFEBlendElementPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

SVGFEBlendElementPrototype::~SVGFEBlendElementPrototype()
{
}

void SVGFEBlendElementPrototype::initialize(JS::Realm& realm)
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

    auto native_mode_getter = JS::NativeFunction::create(realm, mode_getter, 0, "mode"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_mode_setter;

    define_direct_accessor("mode"_utf16_fly_string, native_mode_getter, native_mode_setter, default_attributes);

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

    auto constant_SVG_FEBLEND_MODE_UNKNOWN_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0));

    define_direct_property("SVG_FEBLEND_MODE_UNKNOWN"_utf16_fly_string, constant_SVG_FEBLEND_MODE_UNKNOWN_value, JS::Attribute::Enumerable);

    auto constant_SVG_FEBLEND_MODE_NORMAL_value = JS::Value(static_cast<WebIDL::UnsignedShort>(1));

    define_direct_property("SVG_FEBLEND_MODE_NORMAL"_utf16_fly_string, constant_SVG_FEBLEND_MODE_NORMAL_value, JS::Attribute::Enumerable);

    auto constant_SVG_FEBLEND_MODE_MULTIPLY_value = JS::Value(static_cast<WebIDL::UnsignedShort>(2));

    define_direct_property("SVG_FEBLEND_MODE_MULTIPLY"_utf16_fly_string, constant_SVG_FEBLEND_MODE_MULTIPLY_value, JS::Attribute::Enumerable);

    auto constant_SVG_FEBLEND_MODE_SCREEN_value = JS::Value(static_cast<WebIDL::UnsignedShort>(3));

    define_direct_property("SVG_FEBLEND_MODE_SCREEN"_utf16_fly_string, constant_SVG_FEBLEND_MODE_SCREEN_value, JS::Attribute::Enumerable);

    auto constant_SVG_FEBLEND_MODE_DARKEN_value = JS::Value(static_cast<WebIDL::UnsignedShort>(4));

    define_direct_property("SVG_FEBLEND_MODE_DARKEN"_utf16_fly_string, constant_SVG_FEBLEND_MODE_DARKEN_value, JS::Attribute::Enumerable);

    auto constant_SVG_FEBLEND_MODE_LIGHTEN_value = JS::Value(static_cast<WebIDL::UnsignedShort>(5));

    define_direct_property("SVG_FEBLEND_MODE_LIGHTEN"_utf16_fly_string, constant_SVG_FEBLEND_MODE_LIGHTEN_value, JS::Attribute::Enumerable);

    auto constant_SVG_FEBLEND_MODE_OVERLAY_value = JS::Value(static_cast<WebIDL::UnsignedShort>(6));

    define_direct_property("SVG_FEBLEND_MODE_OVERLAY"_utf16_fly_string, constant_SVG_FEBLEND_MODE_OVERLAY_value, JS::Attribute::Enumerable);

    auto constant_SVG_FEBLEND_MODE_COLOR_DODGE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(7));

    define_direct_property("SVG_FEBLEND_MODE_COLOR_DODGE"_utf16_fly_string, constant_SVG_FEBLEND_MODE_COLOR_DODGE_value, JS::Attribute::Enumerable);

    auto constant_SVG_FEBLEND_MODE_COLOR_BURN_value = JS::Value(static_cast<WebIDL::UnsignedShort>(8));

    define_direct_property("SVG_FEBLEND_MODE_COLOR_BURN"_utf16_fly_string, constant_SVG_FEBLEND_MODE_COLOR_BURN_value, JS::Attribute::Enumerable);

    auto constant_SVG_FEBLEND_MODE_HARD_LIGHT_value = JS::Value(static_cast<WebIDL::UnsignedShort>(9));

    define_direct_property("SVG_FEBLEND_MODE_HARD_LIGHT"_utf16_fly_string, constant_SVG_FEBLEND_MODE_HARD_LIGHT_value, JS::Attribute::Enumerable);

    auto constant_SVG_FEBLEND_MODE_SOFT_LIGHT_value = JS::Value(static_cast<WebIDL::UnsignedShort>(10));

    define_direct_property("SVG_FEBLEND_MODE_SOFT_LIGHT"_utf16_fly_string, constant_SVG_FEBLEND_MODE_SOFT_LIGHT_value, JS::Attribute::Enumerable);

    auto constant_SVG_FEBLEND_MODE_DIFFERENCE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(11));

    define_direct_property("SVG_FEBLEND_MODE_DIFFERENCE"_utf16_fly_string, constant_SVG_FEBLEND_MODE_DIFFERENCE_value, JS::Attribute::Enumerable);

    auto constant_SVG_FEBLEND_MODE_EXCLUSION_value = JS::Value(static_cast<WebIDL::UnsignedShort>(12));

    define_direct_property("SVG_FEBLEND_MODE_EXCLUSION"_utf16_fly_string, constant_SVG_FEBLEND_MODE_EXCLUSION_value, JS::Attribute::Enumerable);

    auto constant_SVG_FEBLEND_MODE_HUE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(13));

    define_direct_property("SVG_FEBLEND_MODE_HUE"_utf16_fly_string, constant_SVG_FEBLEND_MODE_HUE_value, JS::Attribute::Enumerable);

    auto constant_SVG_FEBLEND_MODE_SATURATION_value = JS::Value(static_cast<WebIDL::UnsignedShort>(14));

    define_direct_property("SVG_FEBLEND_MODE_SATURATION"_utf16_fly_string, constant_SVG_FEBLEND_MODE_SATURATION_value, JS::Attribute::Enumerable);

    auto constant_SVG_FEBLEND_MODE_COLOR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(15));

    define_direct_property("SVG_FEBLEND_MODE_COLOR"_utf16_fly_string, constant_SVG_FEBLEND_MODE_COLOR_value, JS::Attribute::Enumerable);

    auto constant_SVG_FEBLEND_MODE_LUMINOSITY_value = JS::Value(static_cast<WebIDL::UnsignedShort>(16));

    define_direct_property("SVG_FEBLEND_MODE_LUMINOSITY"_utf16_fly_string, constant_SVG_FEBLEND_MODE_LUMINOSITY_value, JS::Attribute::Enumerable);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "SVGFEBlendElement"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void SVGFEBlendElementPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<SVG::SVGFEBlendElement*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<SVG::SVGFEBlendElement>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SVGFEBlendElement");
    return static_cast<SVG::SVGFEBlendElement*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(SVGFEBlendElementPrototype::in1_getter)
{
    WebIDL::log_trace(vm, "SVGFEBlendElementPrototype::in1_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->in1(); }));

    return &const_cast<SVGAnimatedString&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFEBlendElementPrototype::in2_getter)
{
    WebIDL::log_trace(vm, "SVGFEBlendElementPrototype::in2_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->in2(); }));

    return &const_cast<SVGAnimatedString&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFEBlendElementPrototype::mode_getter)
{
    WebIDL::log_trace(vm, "SVGFEBlendElementPrototype::mode_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->mode_for_bindings(); }));

    return &const_cast<SVGAnimatedEnumeration&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFEBlendElementPrototype::x_getter)
{
    WebIDL::log_trace(vm, "SVGFEBlendElementPrototype::x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->x(); }));

    return &const_cast<SVGAnimatedLength&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFEBlendElementPrototype::y_getter)
{
    WebIDL::log_trace(vm, "SVGFEBlendElementPrototype::y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->y(); }));

    return &const_cast<SVGAnimatedLength&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFEBlendElementPrototype::width_getter)
{
    WebIDL::log_trace(vm, "SVGFEBlendElementPrototype::width_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->width(); }));

    return &const_cast<SVGAnimatedLength&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFEBlendElementPrototype::height_getter)
{
    WebIDL::log_trace(vm, "SVGFEBlendElementPrototype::height_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->height(); }));

    return &const_cast<SVGAnimatedLength&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFEBlendElementPrototype::result_getter)
{
    WebIDL::log_trace(vm, "SVGFEBlendElementPrototype::result_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->result(); }));

    return &const_cast<SVGAnimatedString&>(*retval);

}

} // namespace Web::Bindings
