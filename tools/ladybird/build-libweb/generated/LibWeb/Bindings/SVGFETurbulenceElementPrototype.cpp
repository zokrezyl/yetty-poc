
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
#include <LibWeb/Bindings/SVGFETurbulenceElementPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGFETurbulenceElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGAnimatedEnumeration.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGAnimatedInteger.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGAnimatedNumber.h>

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

GC_DEFINE_ALLOCATOR(SVGFETurbulenceElementPrototype);

SVGFETurbulenceElementPrototype::SVGFETurbulenceElementPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

SVGFETurbulenceElementPrototype::~SVGFETurbulenceElementPrototype()
{
}

void SVGFETurbulenceElementPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<SVGElementPrototype>(realm, "SVGElement"_fly_string));


    auto native_base_frequency_x_getter = JS::NativeFunction::create(realm, base_frequency_x_getter, 0, "baseFrequencyX"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_base_frequency_x_setter;

    define_direct_accessor("baseFrequencyX"_utf16_fly_string, native_base_frequency_x_getter, native_base_frequency_x_setter, default_attributes);

    auto native_base_frequency_y_getter = JS::NativeFunction::create(realm, base_frequency_y_getter, 0, "baseFrequencyY"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_base_frequency_y_setter;

    define_direct_accessor("baseFrequencyY"_utf16_fly_string, native_base_frequency_y_getter, native_base_frequency_y_setter, default_attributes);

    auto native_num_octaves_getter = JS::NativeFunction::create(realm, num_octaves_getter, 0, "numOctaves"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_num_octaves_setter;

    define_direct_accessor("numOctaves"_utf16_fly_string, native_num_octaves_getter, native_num_octaves_setter, default_attributes);

    auto native_seed_getter = JS::NativeFunction::create(realm, seed_getter, 0, "seed"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_seed_setter;

    define_direct_accessor("seed"_utf16_fly_string, native_seed_getter, native_seed_setter, default_attributes);

    auto native_stitch_tiles_getter = JS::NativeFunction::create(realm, stitch_tiles_getter, 0, "stitchTiles"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_stitch_tiles_setter;

    define_direct_accessor("stitchTiles"_utf16_fly_string, native_stitch_tiles_getter, native_stitch_tiles_setter, default_attributes);

    auto native_type_getter = JS::NativeFunction::create(realm, type_getter, 0, "type"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_type_setter;

    define_direct_accessor("type"_utf16_fly_string, native_type_getter, native_type_setter, default_attributes);

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

    auto constant_SVG_TURBULENCE_TYPE_UNKNOWN_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0));

    define_direct_property("SVG_TURBULENCE_TYPE_UNKNOWN"_utf16_fly_string, constant_SVG_TURBULENCE_TYPE_UNKNOWN_value, JS::Attribute::Enumerable);

    auto constant_SVG_TURBULENCE_TYPE_FRACTALNOISE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(1));

    define_direct_property("SVG_TURBULENCE_TYPE_FRACTALNOISE"_utf16_fly_string, constant_SVG_TURBULENCE_TYPE_FRACTALNOISE_value, JS::Attribute::Enumerable);

    auto constant_SVG_TURBULENCE_TYPE_TURBULENCE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(2));

    define_direct_property("SVG_TURBULENCE_TYPE_TURBULENCE"_utf16_fly_string, constant_SVG_TURBULENCE_TYPE_TURBULENCE_value, JS::Attribute::Enumerable);

    auto constant_SVG_STITCHTYPE_UNKNOWN_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0));

    define_direct_property("SVG_STITCHTYPE_UNKNOWN"_utf16_fly_string, constant_SVG_STITCHTYPE_UNKNOWN_value, JS::Attribute::Enumerable);

    auto constant_SVG_STITCHTYPE_STITCH_value = JS::Value(static_cast<WebIDL::UnsignedShort>(1));

    define_direct_property("SVG_STITCHTYPE_STITCH"_utf16_fly_string, constant_SVG_STITCHTYPE_STITCH_value, JS::Attribute::Enumerable);

    auto constant_SVG_STITCHTYPE_NOSTITCH_value = JS::Value(static_cast<WebIDL::UnsignedShort>(2));

    define_direct_property("SVG_STITCHTYPE_NOSTITCH"_utf16_fly_string, constant_SVG_STITCHTYPE_NOSTITCH_value, JS::Attribute::Enumerable);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "SVGFETurbulenceElement"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void SVGFETurbulenceElementPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<SVG::SVGFETurbulenceElement*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<SVG::SVGFETurbulenceElement>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SVGFETurbulenceElement");
    return static_cast<SVG::SVGFETurbulenceElement*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(SVGFETurbulenceElementPrototype::base_frequency_x_getter)
{
    WebIDL::log_trace(vm, "SVGFETurbulenceElementPrototype::base_frequency_x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->base_frequency_x(); }));

    return &const_cast<SVGAnimatedNumber&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFETurbulenceElementPrototype::base_frequency_y_getter)
{
    WebIDL::log_trace(vm, "SVGFETurbulenceElementPrototype::base_frequency_y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->base_frequency_y(); }));

    return &const_cast<SVGAnimatedNumber&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFETurbulenceElementPrototype::num_octaves_getter)
{
    WebIDL::log_trace(vm, "SVGFETurbulenceElementPrototype::num_octaves_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->num_octaves(); }));

    return &const_cast<SVGAnimatedInteger&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFETurbulenceElementPrototype::seed_getter)
{
    WebIDL::log_trace(vm, "SVGFETurbulenceElementPrototype::seed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->seed(); }));

    return &const_cast<SVGAnimatedNumber&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFETurbulenceElementPrototype::stitch_tiles_getter)
{
    WebIDL::log_trace(vm, "SVGFETurbulenceElementPrototype::stitch_tiles_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->stitch_tiles(); }));

    return &const_cast<SVGAnimatedEnumeration&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFETurbulenceElementPrototype::type_getter)
{
    WebIDL::log_trace(vm, "SVGFETurbulenceElementPrototype::type_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->type(); }));

    return &const_cast<SVGAnimatedEnumeration&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFETurbulenceElementPrototype::x_getter)
{
    WebIDL::log_trace(vm, "SVGFETurbulenceElementPrototype::x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->x(); }));

    return &const_cast<SVGAnimatedLength&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFETurbulenceElementPrototype::y_getter)
{
    WebIDL::log_trace(vm, "SVGFETurbulenceElementPrototype::y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->y(); }));

    return &const_cast<SVGAnimatedLength&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFETurbulenceElementPrototype::width_getter)
{
    WebIDL::log_trace(vm, "SVGFETurbulenceElementPrototype::width_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->width(); }));

    return &const_cast<SVGAnimatedLength&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFETurbulenceElementPrototype::height_getter)
{
    WebIDL::log_trace(vm, "SVGFETurbulenceElementPrototype::height_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->height(); }));

    return &const_cast<SVGAnimatedLength&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGFETurbulenceElementPrototype::result_getter)
{
    WebIDL::log_trace(vm, "SVGFETurbulenceElementPrototype::result_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->result(); }));

    return &const_cast<SVGAnimatedString&>(*retval);

}

} // namespace Web::Bindings
