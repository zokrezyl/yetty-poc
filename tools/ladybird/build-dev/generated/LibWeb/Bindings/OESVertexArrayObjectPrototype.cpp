
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
#include <LibWeb/Bindings/OESVertexArrayObjectPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/Extensions/OESVertexArrayObject.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/Extensions/WebGLVertexArrayObjectOES.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLObject.h>

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

GC_DEFINE_ALLOCATOR(OESVertexArrayObjectPrototype);

OESVertexArrayObjectPrototype::OESVertexArrayObjectPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

OESVertexArrayObjectPrototype::~OESVertexArrayObjectPrototype()
{
}

void OESVertexArrayObjectPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto constant_VERTEX_ARRAY_BINDING_OES_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x85B5));

    define_direct_property("VERTEX_ARRAY_BINDING_OES"_utf16_fly_string, constant_VERTEX_ARRAY_BINDING_OES_value, JS::Attribute::Enumerable);

    define_native_function(realm, "createVertexArrayOES"_utf16_fly_string, create_vertex_array_oes, 0, default_attributes);

    define_native_function(realm, "deleteVertexArrayOES"_utf16_fly_string, delete_vertex_array_oes, 1, default_attributes);

    define_native_function(realm, "isVertexArrayOES"_utf16_fly_string, is_vertex_array_oes, 1, default_attributes);

    define_native_function(realm, "bindVertexArrayOES"_utf16_fly_string, bind_vertex_array_oes, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "OESVertexArrayObject"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void OESVertexArrayObjectPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<OESVertexArrayObject*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<OESVertexArrayObject>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "OESVertexArrayObject");
    return static_cast<OESVertexArrayObject*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(OESVertexArrayObjectPrototype::create_vertex_array_oes)
{
    WebIDL::log_trace(vm, "OESVertexArrayObjectPrototype::create_vertex_array_oes");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_vertex_array_oes(); }));

    return &const_cast<WebGLVertexArrayObjectOES&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(OESVertexArrayObjectPrototype::delete_vertex_array_oes)
{
    WebIDL::log_trace(vm, "OESVertexArrayObjectPrototype::delete_vertex_array_oes");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "deleteVertexArrayOES");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLVertexArrayObjectOES> array_object;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLVertexArrayObjectOES>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLVertexArrayObjectOES");

        array_object = &static_cast<WebGLVertexArrayObjectOES&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_vertex_array_oes(array_object); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OESVertexArrayObjectPrototype::is_vertex_array_oes)
{
    WebIDL::log_trace(vm, "OESVertexArrayObjectPrototype::is_vertex_array_oes");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "isVertexArrayOES");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLVertexArrayObjectOES> array_object;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLVertexArrayObjectOES>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLVertexArrayObjectOES");

        array_object = &static_cast<WebGLVertexArrayObjectOES&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_vertex_array_oes(array_object); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(OESVertexArrayObjectPrototype::bind_vertex_array_oes)
{
    WebIDL::log_trace(vm, "OESVertexArrayObjectPrototype::bind_vertex_array_oes");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "bindVertexArrayOES");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLVertexArrayObjectOES> array_object;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLVertexArrayObjectOES>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLVertexArrayObjectOES");

        array_object = &static_cast<WebGLVertexArrayObjectOES&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bind_vertex_array_oes(array_object); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
