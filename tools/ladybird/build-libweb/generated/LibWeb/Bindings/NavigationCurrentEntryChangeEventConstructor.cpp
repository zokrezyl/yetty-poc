
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
#include <LibWeb/Bindings/NavigationCurrentEntryChangeEventConstructor.h>
#include <LibWeb/Bindings/NavigationCurrentEntryChangeEventPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/NavigationCurrentEntryChangeEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/NavigationHistoryEntry.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/NavigationType.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

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

GC_DEFINE_ALLOCATOR(NavigationCurrentEntryChangeEventConstructor);

NavigationCurrentEntryChangeEventConstructor::NavigationCurrentEntryChangeEventConstructor(JS::Realm& realm)
    : NativeFunction("NavigationCurrentEntryChangeEvent"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

NavigationCurrentEntryChangeEventConstructor::~NavigationCurrentEntryChangeEventConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> NavigationCurrentEntryChangeEventConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "NavigationCurrentEntryChangeEvent");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> NavigationCurrentEntryChangeEventConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "NavigationCurrentEntryChangeEventConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface NavigationCurrentEntryChangeEvent:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<NavigationCurrentEntryChangeEventPrototype>(*target_realm, "NavigationCurrentEntryChangeEvent"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "NavigationCurrentEntryChangeEvent", "2");

    auto arg0 = vm.argument(0);

    String type;
    if (!false || !arg0.is_null()) {
        type = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "NavigationCurrentEntryChangeEventInit");

    NavigationCurrentEntryChangeEventInit event_init_dict {};

    auto from_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        from_property_value_0 = TRY(arg1.as_object().get("from"_utf16_fly_string));

    if (from_property_value_0.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "from");

    if (!from_property_value_0.is_object() || !is<NavigationHistoryEntry>(from_property_value_0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "NavigationHistoryEntry");

    auto& from_value_0 = static_cast<NavigationHistoryEntry&>(from_property_value_0.as_object());

    event_init_dict.from = from_value_0;

    auto navigation_type_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        navigation_type_property_value_1 = TRY(arg1.as_object().get("navigationType"_utf16_fly_string));

    if (!navigation_type_property_value_1.is_undefined()) {

    NavigationType navigation_type_value_1 { NavigationType::Push };

    if (!navigation_type_property_value_1.is_undefined()) {

    auto navigation_type_property_value_1_string = TRY(navigation_type_property_value_1.to_string(vm));

    if (navigation_type_property_value_1_string == "push"sv)
        navigation_type_value_1 = NavigationType::Push;

    else if (navigation_type_property_value_1_string == "replace"sv)
        navigation_type_value_1 = NavigationType::Replace;

    else if (navigation_type_property_value_1_string == "reload"sv)
        navigation_type_value_1 = NavigationType::Reload;

    else if (navigation_type_property_value_1_string == "traverse"sv)
        navigation_type_value_1 = NavigationType::Traverse;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, navigation_type_property_value_1_string, "NavigationType");

    }

    event_init_dict.navigation_type = navigation_type_value_1;

    }

    auto bubbles_property_value_2 = JS::js_undefined();
    if (arg1.is_object())
        bubbles_property_value_2 = TRY(arg1.as_object().get("bubbles"_utf16_fly_string));

    bool bubbles_value_2;

    if (!bubbles_property_value_2.is_undefined())

    bubbles_value_2 = bubbles_property_value_2.to_boolean();

    else
        bubbles_value_2 = static_cast<bool>(false);

    event_init_dict.bubbles = bubbles_value_2;

    auto cancelable_property_value_3 = JS::js_undefined();
    if (arg1.is_object())
        cancelable_property_value_3 = TRY(arg1.as_object().get("cancelable"_utf16_fly_string));

    bool cancelable_value_3;

    if (!cancelable_property_value_3.is_undefined())

    cancelable_value_3 = cancelable_property_value_3.to_boolean();

    else
        cancelable_value_3 = static_cast<bool>(false);

    event_init_dict.cancelable = cancelable_value_3;

    auto composed_property_value_4 = JS::js_undefined();
    if (arg1.is_object())
        composed_property_value_4 = TRY(arg1.as_object().get("composed"_utf16_fly_string));

    bool composed_value_4;

    if (!composed_property_value_4.is_undefined())

    composed_value_4 = composed_property_value_4.to_boolean();

    else
        composed_value_4 = static_cast<bool>(false);

    event_init_dict.composed = composed_value_4;

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return HTML::NavigationCurrentEntryChangeEvent::construct_impl(realm, type, event_init_dict); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface NavigationCurrentEntryChangeEvent" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within HTML::NavigationCurrentEntryChangeEvent::construct_impl().
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


void NavigationCurrentEntryChangeEventConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    set_prototype(&ensure_web_constructor<EventPrototype>(realm, "Event"_fly_string));

    define_direct_property(vm.names.length, JS::Value(2), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "NavigationCurrentEntryChangeEvent"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<NavigationCurrentEntryChangeEventPrototype>(realm, "NavigationCurrentEntryChangeEvent"_fly_string), 0);


}

} // namespace Web::Bindings
