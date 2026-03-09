
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
#include <LibWeb/Bindings/SVGTransformPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGTransform.h>

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

GC_DEFINE_ALLOCATOR(SVGTransformPrototype);

SVGTransformPrototype::SVGTransformPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

SVGTransformPrototype::~SVGTransformPrototype()
{
}

void SVGTransformPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_type_getter = JS::NativeFunction::create(realm, type_getter, 0, "type"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_type_setter;

    define_direct_accessor("type"_utf16_fly_string, native_type_getter, native_type_setter, default_attributes);

    define_direct_property("matrix"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    auto native_angle_getter = JS::NativeFunction::create(realm, angle_getter, 0, "angle"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_angle_setter;

    define_direct_accessor("angle"_utf16_fly_string, native_angle_getter, native_angle_setter, default_attributes);

        define_direct_property("setMatrix"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    auto constant_SVG_TRANSFORM_UNKNOWN_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0));

    define_direct_property("SVG_TRANSFORM_UNKNOWN"_utf16_fly_string, constant_SVG_TRANSFORM_UNKNOWN_value, JS::Attribute::Enumerable);

    auto constant_SVG_TRANSFORM_MATRIX_value = JS::Value(static_cast<WebIDL::UnsignedShort>(1));

    define_direct_property("SVG_TRANSFORM_MATRIX"_utf16_fly_string, constant_SVG_TRANSFORM_MATRIX_value, JS::Attribute::Enumerable);

    auto constant_SVG_TRANSFORM_TRANSLATE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(2));

    define_direct_property("SVG_TRANSFORM_TRANSLATE"_utf16_fly_string, constant_SVG_TRANSFORM_TRANSLATE_value, JS::Attribute::Enumerable);

    auto constant_SVG_TRANSFORM_SCALE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(3));

    define_direct_property("SVG_TRANSFORM_SCALE"_utf16_fly_string, constant_SVG_TRANSFORM_SCALE_value, JS::Attribute::Enumerable);

    auto constant_SVG_TRANSFORM_ROTATE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(4));

    define_direct_property("SVG_TRANSFORM_ROTATE"_utf16_fly_string, constant_SVG_TRANSFORM_ROTATE_value, JS::Attribute::Enumerable);

    auto constant_SVG_TRANSFORM_SKEWX_value = JS::Value(static_cast<WebIDL::UnsignedShort>(5));

    define_direct_property("SVG_TRANSFORM_SKEWX"_utf16_fly_string, constant_SVG_TRANSFORM_SKEWX_value, JS::Attribute::Enumerable);

    auto constant_SVG_TRANSFORM_SKEWY_value = JS::Value(static_cast<WebIDL::UnsignedShort>(6));

    define_direct_property("SVG_TRANSFORM_SKEWY"_utf16_fly_string, constant_SVG_TRANSFORM_SKEWY_value, JS::Attribute::Enumerable);

    define_native_function(realm, "setTranslate"_utf16_fly_string, set_translate, 2, default_attributes);

    define_native_function(realm, "setScale"_utf16_fly_string, set_scale, 2, default_attributes);

    define_native_function(realm, "setRotate"_utf16_fly_string, set_rotate, 3, default_attributes);

    define_native_function(realm, "setSkewX"_utf16_fly_string, set_skew_x, 1, default_attributes);

    define_native_function(realm, "setSkewY"_utf16_fly_string, set_skew_y, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "SVGTransform"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void SVGTransformPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<SVG::SVGTransform*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<SVG::SVGTransform>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SVGTransform");
    return static_cast<SVG::SVGTransform*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(SVGTransformPrototype::type_getter)
{
    WebIDL::log_trace(vm, "SVGTransformPrototype::type_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->type(); }));

    return JS::Value(static_cast<WebIDL::UnsignedShort>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(SVGTransformPrototype::angle_getter)
{
    WebIDL::log_trace(vm, "SVGTransformPrototype::angle_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->angle(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGTransformPrototype::set_translate)
{
    WebIDL::log_trace(vm, "SVGTransformPrototype::set_translate");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "setTranslate", "2");

    auto arg0 = vm.argument(0);

    float tx = TRY(arg0.to_double(vm));

    if (isinf(tx) || isnan(tx)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "tx");
    }
    
    auto arg1 = vm.argument(1);

    float ty = TRY(arg1.to_double(vm));

    if (isinf(ty) || isnan(ty)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "ty");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_translate(tx, ty); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(SVGTransformPrototype::set_scale)
{
    WebIDL::log_trace(vm, "SVGTransformPrototype::set_scale");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "setScale", "2");

    auto arg0 = vm.argument(0);

    float sx = TRY(arg0.to_double(vm));

    if (isinf(sx) || isnan(sx)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "sx");
    }
    
    auto arg1 = vm.argument(1);

    float sy = TRY(arg1.to_double(vm));

    if (isinf(sy) || isnan(sy)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "sy");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_scale(sx, sy); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(SVGTransformPrototype::set_rotate)
{
    WebIDL::log_trace(vm, "SVGTransformPrototype::set_rotate");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "setRotate", "3");

    auto arg0 = vm.argument(0);

    float angle = TRY(arg0.to_double(vm));

    if (isinf(angle) || isnan(angle)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "angle");
    }
    
    auto arg1 = vm.argument(1);

    float cx = TRY(arg1.to_double(vm));

    if (isinf(cx) || isnan(cx)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "cx");
    }
    
    auto arg2 = vm.argument(2);

    float cy = TRY(arg2.to_double(vm));

    if (isinf(cy) || isnan(cy)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "cy");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_rotate(angle, cx, cy); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(SVGTransformPrototype::set_skew_x)
{
    WebIDL::log_trace(vm, "SVGTransformPrototype::set_skew_x");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setSkewX");

    auto arg0 = vm.argument(0);

    float angle = TRY(arg0.to_double(vm));

    if (isinf(angle) || isnan(angle)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "angle");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_skew_x(angle); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(SVGTransformPrototype::set_skew_y)
{
    WebIDL::log_trace(vm, "SVGTransformPrototype::set_skew_y");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setSkewY");

    auto arg0 = vm.argument(0);

    float angle = TRY(arg0.to_double(vm));

    if (isinf(angle) || isnan(angle)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "angle");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_skew_y(angle); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
