
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
#include <LibWeb/Bindings/EventTargetPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

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

GC_DEFINE_ALLOCATOR(EventTargetPrototype);

EventTargetPrototype::EventTargetPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

EventTargetPrototype::~EventTargetPrototype()
{
}

void EventTargetPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    define_native_function(realm, "addEventListener"_utf16_fly_string, add_event_listener, 2, default_attributes);

    define_native_function(realm, "removeEventListener"_utf16_fly_string, remove_event_listener, 2, default_attributes);

    define_native_function(realm, "dispatchEvent"_utf16_fly_string, dispatch_event, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "EventTarget"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void EventTargetPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<DOM::EventTarget*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (is<HTML::Window>(this_object)) {
        return static_cast<HTML::Window*>(this_object);
    }
    if (is<HTML::WindowProxy>(this_object)) {
        return static_cast<HTML::WindowProxy*>(this_object)->window().ptr();
    }

    if (!is<DOM::EventTarget>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "EventTarget");
    return static_cast<DOM::EventTarget*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(EventTargetPrototype::add_event_listener)
{
    WebIDL::log_trace(vm, "EventTargetPrototype::add_event_listener");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "addEventListener", "2");

    auto arg0 = vm.argument(0);

    String type;
    if (!false || !arg0.is_null()) {
        type = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    IDLEventListener* callback = nullptr;
    if (!arg1.is_nullish()) {
        if (!arg1.is_object())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg1);

        auto callback_type = vm.heap().allocate<WebIDL::CallbackType>(arg1.as_object(), HTML::incumbent_realm());
        callback = TRY(throw_dom_exception_if_needed(vm, [&] { return IDLEventListener::create(realm, *callback_type); }));
    }

    auto arg2 = vm.argument(2);

    auto arg2_to_dictionary = [&vm, &realm](JS::Value arg2) -> JS::ThrowCompletionOr<AddEventListenerOptions> {
        // This might be unused.
        (void)realm;

    if (!arg2.is_nullish() && !arg2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AddEventListenerOptions");

    AddEventListenerOptions dictionary_union_type {};

    auto once_property_value_0 = JS::js_undefined();
    if (arg2.is_object())
        once_property_value_0 = TRY(arg2.as_object().get("once"_utf16_fly_string));

    bool once_value_0;

    if (!once_property_value_0.is_undefined())

    once_value_0 = once_property_value_0.to_boolean();

    else
        once_value_0 = static_cast<bool>(false);

    dictionary_union_type.once = once_value_0;

    auto passive_property_value_1 = JS::js_undefined();
    if (arg2.is_object())
        passive_property_value_1 = TRY(arg2.as_object().get("passive"_utf16_fly_string));

    if (!passive_property_value_1.is_undefined()) {

    Optional<bool> passive_value_1;

    if (!passive_property_value_1.is_undefined())

    passive_value_1 = passive_property_value_1.to_boolean();

    dictionary_union_type.passive = passive_value_1;

    }

    auto signal_property_value_2 = JS::js_undefined();
    if (arg2.is_object())
        signal_property_value_2 = TRY(arg2.as_object().get("signal"_utf16_fly_string));

    if (!signal_property_value_2.is_undefined()) {

    GC::Ptr<AbortSignal> signal_value_2;
    if (!signal_property_value_2.is_undefined()) {
        if (!signal_property_value_2.is_object() || !is<AbortSignal>(signal_property_value_2.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AbortSignal");

        signal_value_2 = static_cast<AbortSignal&>(signal_property_value_2.as_object());
    }

    dictionary_union_type.signal = signal_value_2;

    }

    auto capture_property_value_3 = JS::js_undefined();
    if (arg2.is_object())
        capture_property_value_3 = TRY(arg2.as_object().get("capture"_utf16_fly_string));

    bool capture_value_3;

    if (!capture_property_value_3.is_undefined())

    capture_value_3 = capture_property_value_3.to_boolean();

    else
        capture_value_3 = static_cast<bool>(false);

    dictionary_union_type.capture = capture_value_3;

        return dictionary_union_type;
    };

    auto arg2_to_variant = [&vm, &realm, &arg2_to_dictionary](JS::Value arg2) -> JS::ThrowCompletionOr<Variant<AddEventListenerOptions, bool>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg2.is_nullish())
            return Variant<AddEventListenerOptions, bool> { TRY(arg2_to_dictionary(arg2)) };

        if (arg2.is_object()) {
            [[maybe_unused]] auto& arg2_object = arg2.as_object();

        return Variant<AddEventListenerOptions, bool> { TRY(arg2_to_dictionary(arg2)) };

        }

        if (arg2.is_boolean())
            return Variant<AddEventListenerOptions, bool> { arg2.as_bool() };

        return Variant<AddEventListenerOptions, bool> { arg2.to_boolean() };

    };

    Variant<AddEventListenerOptions, bool> options = arg2.is_undefined() ? TRY(arg2_to_dictionary(arg2)) : TRY(arg2_to_variant(arg2));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->add_event_listener(type, callback, options); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(EventTargetPrototype::remove_event_listener)
{
    WebIDL::log_trace(vm, "EventTargetPrototype::remove_event_listener");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "removeEventListener", "2");

    auto arg0 = vm.argument(0);

    String type;
    if (!false || !arg0.is_null()) {
        type = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    IDLEventListener* callback = nullptr;
    if (!arg1.is_nullish()) {
        if (!arg1.is_object())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg1);

        auto callback_type = vm.heap().allocate<WebIDL::CallbackType>(arg1.as_object(), HTML::incumbent_realm());
        callback = TRY(throw_dom_exception_if_needed(vm, [&] { return IDLEventListener::create(realm, *callback_type); }));
    }

    auto arg2 = vm.argument(2);

    auto arg2_to_dictionary = [&vm, &realm](JS::Value arg2) -> JS::ThrowCompletionOr<EventListenerOptions> {
        // This might be unused.
        (void)realm;

    if (!arg2.is_nullish() && !arg2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "EventListenerOptions");

    EventListenerOptions dictionary_union_type {};

    auto capture_property_value_4 = JS::js_undefined();
    if (arg2.is_object())
        capture_property_value_4 = TRY(arg2.as_object().get("capture"_utf16_fly_string));

    bool capture_value_4;

    if (!capture_property_value_4.is_undefined())

    capture_value_4 = capture_property_value_4.to_boolean();

    else
        capture_value_4 = static_cast<bool>(false);

    dictionary_union_type.capture = capture_value_4;

        return dictionary_union_type;
    };

    auto arg2_to_variant = [&vm, &realm, &arg2_to_dictionary](JS::Value arg2) -> JS::ThrowCompletionOr<Variant<EventListenerOptions, bool>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg2.is_nullish())
            return Variant<EventListenerOptions, bool> { TRY(arg2_to_dictionary(arg2)) };

        if (arg2.is_object()) {
            [[maybe_unused]] auto& arg2_object = arg2.as_object();

        return Variant<EventListenerOptions, bool> { TRY(arg2_to_dictionary(arg2)) };

        }

        if (arg2.is_boolean())
            return Variant<EventListenerOptions, bool> { arg2.as_bool() };

        return Variant<EventListenerOptions, bool> { arg2.to_boolean() };

    };

    Variant<EventListenerOptions, bool> options = arg2.is_undefined() ? TRY(arg2_to_dictionary(arg2)) : TRY(arg2_to_variant(arg2));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->remove_event_listener(type, callback, options); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(EventTargetPrototype::dispatch_event)
{
    WebIDL::log_trace(vm, "EventTargetPrototype::dispatch_event");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "dispatchEvent");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Event>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Event");

    auto& event = static_cast<Event&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dispatch_event_binding(event); }));

    return JS::Value(retval);

}

} // namespace Web::Bindings
