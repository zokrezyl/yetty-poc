
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
#include <LibWeb/Bindings/DOMQuadPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMQuad.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPoint.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRectReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPointReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrixReadOnly.h>

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

GC_DEFINE_ALLOCATOR(DOMQuadPrototype);

DOMQuadPrototype::DOMQuadPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

DOMQuadPrototype::~DOMQuadPrototype()
{
}

void DOMQuadPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_p1_getter = JS::NativeFunction::create(realm, p1_getter, 0, "p1"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_p1_setter;

    define_direct_accessor("p1"_utf16_fly_string, native_p1_getter, native_p1_setter, default_attributes);

    auto native_p2_getter = JS::NativeFunction::create(realm, p2_getter, 0, "p2"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_p2_setter;

    define_direct_accessor("p2"_utf16_fly_string, native_p2_getter, native_p2_setter, default_attributes);

    auto native_p3_getter = JS::NativeFunction::create(realm, p3_getter, 0, "p3"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_p3_setter;

    define_direct_accessor("p3"_utf16_fly_string, native_p3_getter, native_p3_setter, default_attributes);

    auto native_p4_getter = JS::NativeFunction::create(realm, p4_getter, 0, "p4"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_p4_setter;

    define_direct_accessor("p4"_utf16_fly_string, native_p4_getter, native_p4_setter, default_attributes);

    define_native_function(realm, "getBounds"_utf16_fly_string, get_bounds, 0, default_attributes);

    define_native_function(realm, "toJSON"_utf16_fly_string, to_json, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "DOMQuad"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void DOMQuadPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Geometry::DOMQuad*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Geometry::DOMQuad>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMQuad");
    return static_cast<Geometry::DOMQuad*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(DOMQuadPrototype::p1_getter)
{
    WebIDL::log_trace(vm, "DOMQuadPrototype::p1_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->p1(); }));

    return &const_cast<DOMPoint&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMQuadPrototype::p2_getter)
{
    WebIDL::log_trace(vm, "DOMQuadPrototype::p2_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->p2(); }));

    return &const_cast<DOMPoint&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMQuadPrototype::p3_getter)
{
    WebIDL::log_trace(vm, "DOMQuadPrototype::p3_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->p3(); }));

    return &const_cast<DOMPoint&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMQuadPrototype::p4_getter)
{
    WebIDL::log_trace(vm, "DOMQuadPrototype::p4_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->p4(); }));

    return &const_cast<DOMPoint&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMQuadPrototype::get_bounds)
{
    WebIDL::log_trace(vm, "DOMQuadPrototype::get_bounds");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_bounds(); }));

    return &const_cast<DOMRect&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMQuadPrototype::to_json)
{
    WebIDL::log_trace(vm, "DOMQuadPrototype::to_json");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

    auto p1_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->p1(); }));

    JS::Value p1_retval_wrapped;

    p1_retval_wrapped = &const_cast<DOMPoint&>(*p1_retval);

    MUST(result->create_data_property("p1"_utf16_fly_string, p1_retval_wrapped));

    auto p2_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->p2(); }));

    JS::Value p2_retval_wrapped;

    p2_retval_wrapped = &const_cast<DOMPoint&>(*p2_retval);

    MUST(result->create_data_property("p2"_utf16_fly_string, p2_retval_wrapped));

    auto p3_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->p3(); }));

    JS::Value p3_retval_wrapped;

    p3_retval_wrapped = &const_cast<DOMPoint&>(*p3_retval);

    MUST(result->create_data_property("p3"_utf16_fly_string, p3_retval_wrapped));

    auto p4_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->p4(); }));

    JS::Value p4_retval_wrapped;

    p4_retval_wrapped = &const_cast<DOMPoint&>(*p4_retval);

    MUST(result->create_data_property("p4"_utf16_fly_string, p4_retval_wrapped));

    return result;
}

} // namespace Web::Bindings
