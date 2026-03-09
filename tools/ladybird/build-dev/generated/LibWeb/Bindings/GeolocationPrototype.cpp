
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
#include <LibWeb/Bindings/GeolocationPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geolocation/Geolocation.h>

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

GC_DEFINE_ALLOCATOR(GeolocationPrototype);

GeolocationPrototype::GeolocationPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

GeolocationPrototype::~GeolocationPrototype()
{
}

void GeolocationPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    define_native_function(realm, "getCurrentPosition"_utf16_fly_string, get_current_position, 1, default_attributes);

    define_native_function(realm, "watchPosition"_utf16_fly_string, watch_position, 1, default_attributes);

    define_native_function(realm, "clearWatch"_utf16_fly_string, clear_watch, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Geolocation"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void GeolocationPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Geolocation::Geolocation*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Geolocation::Geolocation>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Geolocation");
    return static_cast<Geolocation::Geolocation*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(GeolocationPrototype::get_current_position)
{
    WebIDL::log_trace(vm, "GeolocationPrototype::get_current_position");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getCurrentPosition");

    auto arg0 = vm.argument(0);

    if (!arg0.is_function()
)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, arg0);

    auto success_callback = vm.heap().allocate<WebIDL::CallbackType>(arg0.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    auto arg1 = vm.argument(1);

    GC::Ptr<WebIDL::CallbackType> error_callback;
    if (arg1.is_object())
        error_callback = vm.heap().allocate<WebIDL::CallbackType>(arg1.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    auto arg2 = vm.argument(2);

    if (!arg2.is_nullish() && !arg2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PositionOptions");

    PositionOptions options {};

    auto enable_high_accuracy_property_value_0 = JS::js_undefined();
    if (arg2.is_object())
        enable_high_accuracy_property_value_0 = TRY(arg2.as_object().get("enableHighAccuracy"_utf16_fly_string));

    bool enable_high_accuracy_value_0;

    if (!enable_high_accuracy_property_value_0.is_undefined())

    enable_high_accuracy_value_0 = enable_high_accuracy_property_value_0.to_boolean();

    else
        enable_high_accuracy_value_0 = static_cast<bool>(false);

    options.enable_high_accuracy = enable_high_accuracy_value_0;

    auto maximum_age_property_value_1 = JS::js_undefined();
    if (arg2.is_object())
        maximum_age_property_value_1 = TRY(arg2.as_object().get("maximumAge"_utf16_fly_string));

    WebIDL::UnsignedLong maximum_age_value_1;

    if (!maximum_age_property_value_1.is_undefined())

    maximum_age_value_1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, maximum_age_property_value_1, WebIDL::EnforceRange::No, WebIDL::Clamp::Yes));

    else
        maximum_age_value_1 = static_cast<WebIDL::UnsignedLong>(0);

    options.maximum_age = maximum_age_value_1;

    auto timeout_property_value_2 = JS::js_undefined();
    if (arg2.is_object())
        timeout_property_value_2 = TRY(arg2.as_object().get("timeout"_utf16_fly_string));

    WebIDL::UnsignedLong timeout_value_2;

    if (!timeout_property_value_2.is_undefined())

    timeout_value_2 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, timeout_property_value_2, WebIDL::EnforceRange::No, WebIDL::Clamp::Yes));

    else
        timeout_value_2 = static_cast<WebIDL::UnsignedLong>(0xFFFFFFFF);

    options.timeout = timeout_value_2;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_current_position(success_callback, error_callback, options); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(GeolocationPrototype::watch_position)
{
    WebIDL::log_trace(vm, "GeolocationPrototype::watch_position");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "watchPosition");

    auto arg0 = vm.argument(0);

    if (!arg0.is_function()
)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, arg0);

    auto success_callback = vm.heap().allocate<WebIDL::CallbackType>(arg0.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    auto arg1 = vm.argument(1);

    GC::Ptr<WebIDL::CallbackType> error_callback;
    if (arg1.is_object())
        error_callback = vm.heap().allocate<WebIDL::CallbackType>(arg1.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    auto arg2 = vm.argument(2);

    if (!arg2.is_nullish() && !arg2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PositionOptions");

    PositionOptions options {};

    auto enable_high_accuracy_property_value_3 = JS::js_undefined();
    if (arg2.is_object())
        enable_high_accuracy_property_value_3 = TRY(arg2.as_object().get("enableHighAccuracy"_utf16_fly_string));

    bool enable_high_accuracy_value_3;

    if (!enable_high_accuracy_property_value_3.is_undefined())

    enable_high_accuracy_value_3 = enable_high_accuracy_property_value_3.to_boolean();

    else
        enable_high_accuracy_value_3 = static_cast<bool>(false);

    options.enable_high_accuracy = enable_high_accuracy_value_3;

    auto maximum_age_property_value_4 = JS::js_undefined();
    if (arg2.is_object())
        maximum_age_property_value_4 = TRY(arg2.as_object().get("maximumAge"_utf16_fly_string));

    WebIDL::UnsignedLong maximum_age_value_4;

    if (!maximum_age_property_value_4.is_undefined())

    maximum_age_value_4 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, maximum_age_property_value_4, WebIDL::EnforceRange::No, WebIDL::Clamp::Yes));

    else
        maximum_age_value_4 = static_cast<WebIDL::UnsignedLong>(0);

    options.maximum_age = maximum_age_value_4;

    auto timeout_property_value_5 = JS::js_undefined();
    if (arg2.is_object())
        timeout_property_value_5 = TRY(arg2.as_object().get("timeout"_utf16_fly_string));

    WebIDL::UnsignedLong timeout_value_5;

    if (!timeout_property_value_5.is_undefined())

    timeout_value_5 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, timeout_property_value_5, WebIDL::EnforceRange::No, WebIDL::Clamp::Yes));

    else
        timeout_value_5 = static_cast<WebIDL::UnsignedLong>(0xFFFFFFFF);

    options.timeout = timeout_value_5;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->watch_position(success_callback, error_callback, options); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(GeolocationPrototype::clear_watch)
{
    WebIDL::log_trace(vm, "GeolocationPrototype::clear_watch");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "clearWatch");

    auto arg0 = vm.argument(0);

    WebIDL::Long watch_id;

    watch_id = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear_watch(watch_id); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
