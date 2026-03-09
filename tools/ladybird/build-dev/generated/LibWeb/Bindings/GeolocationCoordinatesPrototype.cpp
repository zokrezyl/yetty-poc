
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
#include <LibWeb/Bindings/GeolocationCoordinatesPrototype.h>
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

GC_DEFINE_ALLOCATOR(GeolocationCoordinatesPrototype);

GeolocationCoordinatesPrototype::GeolocationCoordinatesPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

GeolocationCoordinatesPrototype::~GeolocationCoordinatesPrototype()
{
}

void GeolocationCoordinatesPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_accuracy_getter = JS::NativeFunction::create(realm, accuracy_getter, 0, "accuracy"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_accuracy_setter;

    define_direct_accessor("accuracy"_utf16_fly_string, native_accuracy_getter, native_accuracy_setter, default_attributes);

    auto native_latitude_getter = JS::NativeFunction::create(realm, latitude_getter, 0, "latitude"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_latitude_setter;

    define_direct_accessor("latitude"_utf16_fly_string, native_latitude_getter, native_latitude_setter, default_attributes);

    auto native_longitude_getter = JS::NativeFunction::create(realm, longitude_getter, 0, "longitude"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_longitude_setter;

    define_direct_accessor("longitude"_utf16_fly_string, native_longitude_getter, native_longitude_setter, default_attributes);

    auto native_altitude_getter = JS::NativeFunction::create(realm, altitude_getter, 0, "altitude"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_altitude_setter;

    define_direct_accessor("altitude"_utf16_fly_string, native_altitude_getter, native_altitude_setter, default_attributes);

    auto native_altitude_accuracy_getter = JS::NativeFunction::create(realm, altitude_accuracy_getter, 0, "altitudeAccuracy"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_altitude_accuracy_setter;

    define_direct_accessor("altitudeAccuracy"_utf16_fly_string, native_altitude_accuracy_getter, native_altitude_accuracy_setter, default_attributes);

    auto native_heading_getter = JS::NativeFunction::create(realm, heading_getter, 0, "heading"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_heading_setter;

    define_direct_accessor("heading"_utf16_fly_string, native_heading_getter, native_heading_setter, default_attributes);

    auto native_speed_getter = JS::NativeFunction::create(realm, speed_getter, 0, "speed"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_speed_setter;

    define_direct_accessor("speed"_utf16_fly_string, native_speed_getter, native_speed_setter, default_attributes);

    define_native_function(realm, "toJSON"_utf16_fly_string, to_json, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "GeolocationCoordinates"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void GeolocationCoordinatesPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Geolocation::GeolocationCoordinates*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Geolocation::GeolocationCoordinates>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "GeolocationCoordinates");
    return static_cast<Geolocation::GeolocationCoordinates*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(GeolocationCoordinatesPrototype::accuracy_getter)
{
    WebIDL::log_trace(vm, "GeolocationCoordinatesPrototype::accuracy_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->accuracy(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(GeolocationCoordinatesPrototype::latitude_getter)
{
    WebIDL::log_trace(vm, "GeolocationCoordinatesPrototype::latitude_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->latitude(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(GeolocationCoordinatesPrototype::longitude_getter)
{
    WebIDL::log_trace(vm, "GeolocationCoordinatesPrototype::longitude_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->longitude(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(GeolocationCoordinatesPrototype::altitude_getter)
{
    WebIDL::log_trace(vm, "GeolocationCoordinatesPrototype::altitude_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->altitude(); }));

    if (retval.has_value()) {

    return JS::Value(retval.release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(GeolocationCoordinatesPrototype::altitude_accuracy_getter)
{
    WebIDL::log_trace(vm, "GeolocationCoordinatesPrototype::altitude_accuracy_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->altitude_accuracy(); }));

    if (retval.has_value()) {

    return JS::Value(retval.release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(GeolocationCoordinatesPrototype::heading_getter)
{
    WebIDL::log_trace(vm, "GeolocationCoordinatesPrototype::heading_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->heading(); }));

    if (retval.has_value()) {

    return JS::Value(retval.release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(GeolocationCoordinatesPrototype::speed_getter)
{
    WebIDL::log_trace(vm, "GeolocationCoordinatesPrototype::speed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->speed(); }));

    if (retval.has_value()) {

    return JS::Value(retval.release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(GeolocationCoordinatesPrototype::to_json)
{
    WebIDL::log_trace(vm, "GeolocationCoordinatesPrototype::to_json");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

    auto accuracy_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->accuracy(); }));

    JS::Value accuracy_retval_wrapped;

    accuracy_retval_wrapped = JS::Value(accuracy_retval);

    MUST(result->create_data_property("accuracy"_utf16_fly_string, accuracy_retval_wrapped));

    auto latitude_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->latitude(); }));

    JS::Value latitude_retval_wrapped;

    latitude_retval_wrapped = JS::Value(latitude_retval);

    MUST(result->create_data_property("latitude"_utf16_fly_string, latitude_retval_wrapped));

    auto longitude_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->longitude(); }));

    JS::Value longitude_retval_wrapped;

    longitude_retval_wrapped = JS::Value(longitude_retval);

    MUST(result->create_data_property("longitude"_utf16_fly_string, longitude_retval_wrapped));

    auto altitude_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->altitude(); }));

    JS::Value altitude_retval_wrapped;

    if (altitude_retval.has_value()) {

    altitude_retval_wrapped = JS::Value(altitude_retval.release_value());

    } else {
        altitude_retval_wrapped = JS::js_null();
    }

    MUST(result->create_data_property("altitude"_utf16_fly_string, altitude_retval_wrapped));

    auto altitude_accuracy_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->altitude_accuracy(); }));

    JS::Value altitude_accuracy_retval_wrapped;

    if (altitude_accuracy_retval.has_value()) {

    altitude_accuracy_retval_wrapped = JS::Value(altitude_accuracy_retval.release_value());

    } else {
        altitude_accuracy_retval_wrapped = JS::js_null();
    }

    MUST(result->create_data_property("altitudeAccuracy"_utf16_fly_string, altitude_accuracy_retval_wrapped));

    auto heading_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->heading(); }));

    JS::Value heading_retval_wrapped;

    if (heading_retval.has_value()) {

    heading_retval_wrapped = JS::Value(heading_retval.release_value());

    } else {
        heading_retval_wrapped = JS::js_null();
    }

    MUST(result->create_data_property("heading"_utf16_fly_string, heading_retval_wrapped));

    auto speed_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->speed(); }));

    JS::Value speed_retval_wrapped;

    if (speed_retval.has_value()) {

    speed_retval_wrapped = JS::Value(speed_retval.release_value());

    } else {
        speed_retval_wrapped = JS::js_null();
    }

    MUST(result->create_data_property("speed"_utf16_fly_string, speed_retval_wrapped));

    return result;
}

} // namespace Web::Bindings
