
#include <LibIDL/Types.h>
#include <LibGC/Heap.h>
#include <LibJS/Runtime/Array.h>
#include <LibJS/Runtime/ArrayBuffer.h>
#include <LibJS/Runtime/DataView.h>
#include <LibJS/Runtime/GlobalObject.h>
#include <LibJS/Runtime/Iterator.h>
#include <LibJS/Runtime/PromiseConstructor.h>
#include <LibJS/Runtime/ValueInlines.h>
#include <LibJS/Runtime/TypedArray.h>
#include <LibWeb/Bindings/NavigateEventConstructor.h>
#include <LibWeb/Bindings/NavigateEventPrototype.h>
#include <LibWeb/Bindings/ExceptionOrUtils.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/HTML/WindowProxy.h>
#include <LibWeb/WebIDL/AbstractOperations.h>
#include <LibWeb/WebIDL/Buffers.h>
#include <LibWeb/WebIDL/CallbackType.h>
#include <LibWeb/WebIDL/OverloadResolution.h>
#include <LibWeb/WebIDL/Tracing.h>
#include <LibWeb/WebIDL/Types.h>

#if __has_include(<LibWeb/Bindings/EventPrototype.h>)
#    include <LibWeb/Bindings/EventPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/NavigateEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/NavigationDestination.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/NavigationType.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XHR/FormData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/File.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/QueuingStrategy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamBYOBReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamDefaultReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStreamDefaultWriter.h>

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

GC_DEFINE_ALLOCATOR(NavigateEventConstructor);

NavigateEventConstructor::NavigateEventConstructor(JS::Realm& realm)
    : NativeFunction("NavigateEvent"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

NavigateEventConstructor::~NavigateEventConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> NavigateEventConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "NavigateEvent");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> NavigateEventConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "NavigateEventConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface NavigateEvent:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<NavigateEventPrototype>(*target_realm, "NavigateEvent"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "NavigateEvent", "2");

    auto arg0 = vm.argument(0);

    String type;
    if (!false || !arg0.is_null()) {
        type = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "NavigateEventInit");

    NavigateEventInit event_init_dict {};

    auto can_intercept_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        can_intercept_property_value_0 = TRY(arg1.as_object().get("canIntercept"_utf16_fly_string));

    bool can_intercept_value_0;

    if (!can_intercept_property_value_0.is_undefined())

    can_intercept_value_0 = can_intercept_property_value_0.to_boolean();

    else
        can_intercept_value_0 = static_cast<bool>(false);

    event_init_dict.can_intercept = can_intercept_value_0;

    auto destination_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        destination_property_value_1 = TRY(arg1.as_object().get("destination"_utf16_fly_string));

    if (destination_property_value_1.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "destination");

    if (!destination_property_value_1.is_object() || !is<NavigationDestination>(destination_property_value_1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "NavigationDestination");

    auto& destination_value_1 = static_cast<NavigationDestination&>(destination_property_value_1.as_object());

    event_init_dict.destination = destination_value_1;

    auto download_request_property_value_2 = JS::js_undefined();
    if (arg1.is_object())
        download_request_property_value_2 = TRY(arg1.as_object().get("downloadRequest"_utf16_fly_string));

    Optional<String> download_request_value_2;

    if (!download_request_property_value_2.is_undefined()) {
        if (!download_request_property_value_2.is_null())
            download_request_value_2 = TRY(WebIDL::to_string(vm, download_request_property_value_2));
    }

    event_init_dict.download_request = download_request_value_2;

    auto form_data_property_value_3 = JS::js_undefined();
    if (arg1.is_object())
        form_data_property_value_3 = TRY(arg1.as_object().get("formData"_utf16_fly_string));

    GC::Ptr<FormData> form_data_value_3;

    if (!form_data_property_value_3.is_nullish()) {
        if (!form_data_property_value_3.is_object() || !is<FormData>(form_data_property_value_3.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "FormData");

        form_data_value_3 = &static_cast<FormData&>(form_data_property_value_3.as_object());
    }

    event_init_dict.form_data = form_data_value_3;

    auto has_ua_visual_transition_property_value_4 = JS::js_undefined();
    if (arg1.is_object())
        has_ua_visual_transition_property_value_4 = TRY(arg1.as_object().get("hasUAVisualTransition"_utf16_fly_string));

    bool has_ua_visual_transition_value_4;

    if (!has_ua_visual_transition_property_value_4.is_undefined())

    has_ua_visual_transition_value_4 = has_ua_visual_transition_property_value_4.to_boolean();

    else
        has_ua_visual_transition_value_4 = static_cast<bool>(false);

    event_init_dict.has_ua_visual_transition = has_ua_visual_transition_value_4;

    auto hash_change_property_value_5 = JS::js_undefined();
    if (arg1.is_object())
        hash_change_property_value_5 = TRY(arg1.as_object().get("hashChange"_utf16_fly_string));

    bool hash_change_value_5;

    if (!hash_change_property_value_5.is_undefined())

    hash_change_value_5 = hash_change_property_value_5.to_boolean();

    else
        hash_change_value_5 = static_cast<bool>(false);

    event_init_dict.hash_change = hash_change_value_5;

    auto info_property_value_6 = JS::js_undefined();
    if (arg1.is_object())
        info_property_value_6 = TRY(arg1.as_object().get("info"_utf16_fly_string));

    if (!info_property_value_6.is_undefined()) {

    JS::Value info_value_6 = JS::js_undefined();
    if (!info_property_value_6.is_undefined())
        info_value_6 = info_property_value_6;

    event_init_dict.info = info_value_6;

    }

    auto navigation_type_property_value_7 = JS::js_undefined();
    if (arg1.is_object())
        navigation_type_property_value_7 = TRY(arg1.as_object().get("navigationType"_utf16_fly_string));

    NavigationType navigation_type_value_7 { NavigationType::Push };

    if (!navigation_type_property_value_7.is_undefined()) {

    auto navigation_type_property_value_7_string = TRY(navigation_type_property_value_7.to_string(vm));

    if (navigation_type_property_value_7_string == "push"sv)
        navigation_type_value_7 = NavigationType::Push;

    else if (navigation_type_property_value_7_string == "replace"sv)
        navigation_type_value_7 = NavigationType::Replace;

    else if (navigation_type_property_value_7_string == "reload"sv)
        navigation_type_value_7 = NavigationType::Reload;

    else if (navigation_type_property_value_7_string == "traverse"sv)
        navigation_type_value_7 = NavigationType::Traverse;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, navigation_type_property_value_7_string, "NavigationType");

    }

    event_init_dict.navigation_type = navigation_type_value_7;

    auto signal_property_value_8 = JS::js_undefined();
    if (arg1.is_object())
        signal_property_value_8 = TRY(arg1.as_object().get("signal"_utf16_fly_string));

    if (signal_property_value_8.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "signal");

    if (!signal_property_value_8.is_object() || !is<AbortSignal>(signal_property_value_8.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AbortSignal");

    auto& signal_value_8 = static_cast<AbortSignal&>(signal_property_value_8.as_object());

    event_init_dict.signal = signal_value_8;

    auto source_element_property_value_9 = JS::js_undefined();
    if (arg1.is_object())
        source_element_property_value_9 = TRY(arg1.as_object().get("sourceElement"_utf16_fly_string));

    GC::Ptr<Element> source_element_value_9;

    if (!source_element_property_value_9.is_nullish()) {
        if (!source_element_property_value_9.is_object() || !is<Element>(source_element_property_value_9.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Element");

        source_element_value_9 = &static_cast<Element&>(source_element_property_value_9.as_object());
    }

    event_init_dict.source_element = source_element_value_9;

    auto user_initiated_property_value_10 = JS::js_undefined();
    if (arg1.is_object())
        user_initiated_property_value_10 = TRY(arg1.as_object().get("userInitiated"_utf16_fly_string));

    bool user_initiated_value_10;

    if (!user_initiated_property_value_10.is_undefined())

    user_initiated_value_10 = user_initiated_property_value_10.to_boolean();

    else
        user_initiated_value_10 = static_cast<bool>(false);

    event_init_dict.user_initiated = user_initiated_value_10;

    auto bubbles_property_value_11 = JS::js_undefined();
    if (arg1.is_object())
        bubbles_property_value_11 = TRY(arg1.as_object().get("bubbles"_utf16_fly_string));

    bool bubbles_value_11;

    if (!bubbles_property_value_11.is_undefined())

    bubbles_value_11 = bubbles_property_value_11.to_boolean();

    else
        bubbles_value_11 = static_cast<bool>(false);

    event_init_dict.bubbles = bubbles_value_11;

    auto cancelable_property_value_12 = JS::js_undefined();
    if (arg1.is_object())
        cancelable_property_value_12 = TRY(arg1.as_object().get("cancelable"_utf16_fly_string));

    bool cancelable_value_12;

    if (!cancelable_property_value_12.is_undefined())

    cancelable_value_12 = cancelable_property_value_12.to_boolean();

    else
        cancelable_value_12 = static_cast<bool>(false);

    event_init_dict.cancelable = cancelable_value_12;

    auto composed_property_value_13 = JS::js_undefined();
    if (arg1.is_object())
        composed_property_value_13 = TRY(arg1.as_object().get("composed"_utf16_fly_string));

    bool composed_value_13;

    if (!composed_property_value_13.is_undefined())

    composed_value_13 = composed_property_value_13.to_boolean();

    else
        composed_value_13 = static_cast<bool>(false);

    event_init_dict.composed = composed_value_13;

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return HTML::NavigateEvent::construct_impl(realm, type, event_init_dict); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface NavigateEvent" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within HTML::NavigateEvent::construct_impl().
    //  8. Let interfaces be the inclusive inherited interfaces of interface.
    //  9. For every interface ancestor interface in interfaces:
    //    9.1. Let unforgeables be the value of the [[Unforgeables]] slot of the interface object of ancestor interface in realm.
    //    9.2. Let keys be ! unforgeables.[[OwnPropertyKeys]]().
    //    9.3. For each element key of keys:
    //      9.3.1. Let descriptor be ! unforgeables.[[GetOwnProperty]](key).
    //      9.3.2. Perform ! DefinePropertyOrThrow(instance, key, descriptor).
    //  10. If interface is declared with the [Global] extended attribute, then:
    //    10.1. Define the regular operations of interface on instance, given realm.
    //    10.2. Define the regular attributes of interface on instance, given realm.
    //    10.3. Define the iteration methods of interface on instance given realm.
    //    10.4. Define the asynchronous iteration methods of interface on instance given realm.
    //    10.5. Define the global property references on instance, given realm.
    //    10.6. Set instance.[[SetPrototypeOf]] as defined in § 3.8.1 [[SetPrototypeOf]].
    //  11. Otherwise, if interfaces contains an interface which supports indexed properties, named properties, or both:
    //    11.1. Set instance.[[GetOwnProperty]] as defined in § 3.9.1 [[GetOwnProperty]].
    //    11.2. Set instance.[[Set]] as defined in § 3.9.2 [[Set]].
    //    11.3. Set instance.[[DefineOwnProperty]] as defined in § 3.9.3 [[DefineOwnProperty]].
    //    11.4. Set instance.[[Delete]] as defined in § 3.9.4 [[Delete]].
    //    11.5. Set instance.[[PreventExtensions]] as defined in § 3.9.5 [[PreventExtensions]].
    //    11.6. Set instance.[[OwnPropertyKeys]] as defined in § 3.9.6 [[OwnPropertyKeys]].

    return *impl;
}


void NavigateEventConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    set_prototype(&ensure_web_constructor<EventPrototype>(realm, "Event"_fly_string));

    define_direct_property(vm.names.length, JS::Value(2), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "NavigateEvent"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<NavigateEventPrototype>(realm, "NavigateEvent"_fly_string), 0);


}

} // namespace Web::Bindings
