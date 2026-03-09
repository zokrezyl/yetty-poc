
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
#include <LibWeb/Bindings/ANGLEInstancedArraysPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/Extensions/ANGLEInstancedArrays.h>

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

GC_DEFINE_ALLOCATOR(ANGLEInstancedArraysPrototype);

ANGLEInstancedArraysPrototype::ANGLEInstancedArraysPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

ANGLEInstancedArraysPrototype::~ANGLEInstancedArraysPrototype()
{
}

void ANGLEInstancedArraysPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto constant_VERTEX_ATTRIB_ARRAY_DIVISOR_ANGLE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88FE));

    define_direct_property("VERTEX_ATTRIB_ARRAY_DIVISOR_ANGLE"_utf16_fly_string, constant_VERTEX_ATTRIB_ARRAY_DIVISOR_ANGLE_value, JS::Attribute::Enumerable);

    define_native_function(realm, "drawArraysInstancedANGLE"_utf16_fly_string, draw_arrays_instanced_angle, 4, default_attributes);

    define_native_function(realm, "drawElementsInstancedANGLE"_utf16_fly_string, draw_elements_instanced_angle, 5, default_attributes);

    define_native_function(realm, "vertexAttribDivisorANGLE"_utf16_fly_string, vertex_attrib_divisor_angle, 2, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "ANGLEInstancedArrays"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void ANGLEInstancedArraysPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<ANGLEInstancedArrays*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<ANGLEInstancedArrays>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ANGLEInstancedArrays");
    return static_cast<ANGLEInstancedArrays*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(ANGLEInstancedArraysPrototype::draw_arrays_instanced_angle)
{
    WebIDL::log_trace(vm, "ANGLEInstancedArraysPrototype::draw_arrays_instanced_angle");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "drawArraysInstancedANGLE", "4");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong mode;

    mode = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long first;

    first = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long count;

    count = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long primcount;

    primcount = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->draw_arrays_instanced_angle(mode, first, count, primcount); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ANGLEInstancedArraysPrototype::draw_elements_instanced_angle)
{
    WebIDL::log_trace(vm, "ANGLEInstancedArraysPrototype::draw_elements_instanced_angle");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "drawElementsInstancedANGLE", "5");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong mode;

    mode = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long count;

    count = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong type;

    type = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::LongLong offset;

    offset = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::Long primcount;

    primcount = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->draw_elements_instanced_angle(mode, count, type, offset, primcount); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ANGLEInstancedArraysPrototype::vertex_attrib_divisor_angle)
{
    WebIDL::log_trace(vm, "ANGLEInstancedArraysPrototype::vertex_attrib_divisor_angle");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "vertexAttribDivisorANGLE", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong divisor;

    divisor = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vertex_attrib_divisor_angle(index, divisor); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
