
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
#include <LibWeb/Bindings/GeolocationPositionPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geolocation/GeolocationPosition.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geolocation/GeolocationCoordinates.h>

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

GC_DEFINE_ALLOCATOR(GeolocationPositionPrototype);

GeolocationPositionPrototype::GeolocationPositionPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

GeolocationPositionPrototype::~GeolocationPositionPrototype()
{
}

void GeolocationPositionPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_coords_getter = JS::NativeFunction::create(realm, coords_getter, 0, "coords"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_coords_setter;

    define_direct_accessor("coords"_utf16_fly_string, native_coords_getter, native_coords_setter, default_attributes);

    auto native_timestamp_getter = JS::NativeFunction::create(realm, timestamp_getter, 0, "timestamp"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_timestamp_setter;

    define_direct_accessor("timestamp"_utf16_fly_string, native_timestamp_getter, native_timestamp_setter, default_attributes);

    define_native_function(realm, "toJSON"_utf16_fly_string, to_json, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "GeolocationPosition"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void GeolocationPositionPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Geolocation::GeolocationPosition*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Geolocation::GeolocationPosition>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "GeolocationPosition");
    return static_cast<Geolocation::GeolocationPosition*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(GeolocationPositionPrototype::coords_getter)
{
    WebIDL::log_trace(vm, "GeolocationPositionPrototype::coords_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->coords(); }));

    return &const_cast<GeolocationCoordinates&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(GeolocationPositionPrototype::timestamp_getter)
{
    WebIDL::log_trace(vm, "GeolocationPositionPrototype::timestamp_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->timestamp(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(GeolocationPositionPrototype::to_json)
{
    WebIDL::log_trace(vm, "GeolocationPositionPrototype::to_json");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

    auto coords_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->coords(); }));

    JS::Value coords_retval_wrapped;

    coords_retval_wrapped = &const_cast<GeolocationCoordinates&>(*coords_retval);

    MUST(result->create_data_property("coords"_utf16_fly_string, coords_retval_wrapped));

    auto timestamp_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->timestamp(); }));

    JS::Value timestamp_retval_wrapped;

    timestamp_retval_wrapped = JS::Value(static_cast<double>(timestamp_retval));

    MUST(result->create_data_property("timestamp"_utf16_fly_string, timestamp_retval_wrapped));

    return result;
}

} // namespace Web::Bindings
