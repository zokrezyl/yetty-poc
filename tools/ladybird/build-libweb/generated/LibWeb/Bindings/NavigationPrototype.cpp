
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
#include <LibWeb/Bindings/NavigationPrototype.h>
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

#if __has_include(<LibWeb/Bindings/EventTargetPrototype.h>)
#    include <LibWeb/Bindings/EventTargetPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Navigation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/NavigationHistoryEntry.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/NavigationTransition.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/NavigationDestination.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/NavigationType.h>

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

GC_DEFINE_ALLOCATOR(NavigationPrototype);

NavigationPrototype::NavigationPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

NavigationPrototype::~NavigationPrototype()
{
}

void NavigationPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    auto native_current_entry_getter = JS::NativeFunction::create(realm, current_entry_getter, 0, "currentEntry"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_current_entry_setter;

    define_direct_accessor("currentEntry"_utf16_fly_string, native_current_entry_getter, native_current_entry_setter, default_attributes);

    auto native_transition_getter = JS::NativeFunction::create(realm, transition_getter, 0, "transition"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_transition_setter;

    define_direct_accessor("transition"_utf16_fly_string, native_transition_getter, native_transition_setter, default_attributes);

    auto native_can_go_back_getter = JS::NativeFunction::create(realm, can_go_back_getter, 0, "canGoBack"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_can_go_back_setter;

    define_direct_accessor("canGoBack"_utf16_fly_string, native_can_go_back_getter, native_can_go_back_setter, default_attributes);

    auto native_can_go_forward_getter = JS::NativeFunction::create(realm, can_go_forward_getter, 0, "canGoForward"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_can_go_forward_setter;

    define_direct_accessor("canGoForward"_utf16_fly_string, native_can_go_forward_getter, native_can_go_forward_setter, default_attributes);

    auto native_onnavigate_getter = JS::NativeFunction::create(realm, onnavigate_getter, 0, "onnavigate"_utf16_fly_string, &realm, "get"sv);

    auto native_onnavigate_setter = JS::NativeFunction::create(realm, onnavigate_setter, 1, "onnavigate"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onnavigate"_utf16_fly_string, native_onnavigate_getter, native_onnavigate_setter, default_attributes);

    auto native_onnavigatesuccess_getter = JS::NativeFunction::create(realm, onnavigatesuccess_getter, 0, "onnavigatesuccess"_utf16_fly_string, &realm, "get"sv);

    auto native_onnavigatesuccess_setter = JS::NativeFunction::create(realm, onnavigatesuccess_setter, 1, "onnavigatesuccess"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onnavigatesuccess"_utf16_fly_string, native_onnavigatesuccess_getter, native_onnavigatesuccess_setter, default_attributes);

    auto native_onnavigateerror_getter = JS::NativeFunction::create(realm, onnavigateerror_getter, 0, "onnavigateerror"_utf16_fly_string, &realm, "get"sv);

    auto native_onnavigateerror_setter = JS::NativeFunction::create(realm, onnavigateerror_setter, 1, "onnavigateerror"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onnavigateerror"_utf16_fly_string, native_onnavigateerror_getter, native_onnavigateerror_setter, default_attributes);

    auto native_oncurrententrychange_getter = JS::NativeFunction::create(realm, oncurrententrychange_getter, 0, "oncurrententrychange"_utf16_fly_string, &realm, "get"sv);

    auto native_oncurrententrychange_setter = JS::NativeFunction::create(realm, oncurrententrychange_setter, 1, "oncurrententrychange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("oncurrententrychange"_utf16_fly_string, native_oncurrententrychange_getter, native_oncurrententrychange_setter, default_attributes);

    define_native_function(realm, "entries"_utf16_fly_string, entries, 0, default_attributes);

    define_native_function(realm, "updateCurrentEntry"_utf16_fly_string, update_current_entry, 1, default_attributes);

    define_native_function(realm, "navigate"_utf16_fly_string, navigate, 1, default_attributes);

    define_native_function(realm, "reload"_utf16_fly_string, reload, 0, default_attributes);

    define_native_function(realm, "traverseTo"_utf16_fly_string, traverse_to, 1, default_attributes);

    define_native_function(realm, "back"_utf16_fly_string, back, 0, default_attributes);

    define_native_function(realm, "forward"_utf16_fly_string, forward, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Navigation"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void NavigationPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::Navigation*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::Navigation>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Navigation");
    return static_cast<HTML::Navigation*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::current_entry_getter)
{
    WebIDL::log_trace(vm, "NavigationPrototype::current_entry_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->current_entry(); }));

    if (retval) {

    return &const_cast<NavigationHistoryEntry&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::transition_getter)
{
    WebIDL::log_trace(vm, "NavigationPrototype::transition_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->transition(); }));

    if (retval) {

    return &const_cast<NavigationTransition&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::can_go_back_getter)
{
    WebIDL::log_trace(vm, "NavigationPrototype::can_go_back_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->can_go_back(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::can_go_forward_getter)
{
    WebIDL::log_trace(vm, "NavigationPrototype::can_go_forward_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->can_go_forward(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::onnavigate_getter)
{
    WebIDL::log_trace(vm, "NavigationPrototype::onnavigate_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onnavigate(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::onnavigate_setter)
{
    WebIDL::log_trace(vm, "NavigationPrototype::onnavigate_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    GC::Ptr<WebIDL::CallbackType> cpp_value;
    if (value.is_object())
        cpp_value = vm.heap().allocate<WebIDL::CallbackType>(value.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onnavigate(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::onnavigatesuccess_getter)
{
    WebIDL::log_trace(vm, "NavigationPrototype::onnavigatesuccess_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onnavigatesuccess(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::onnavigatesuccess_setter)
{
    WebIDL::log_trace(vm, "NavigationPrototype::onnavigatesuccess_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    GC::Ptr<WebIDL::CallbackType> cpp_value;
    if (value.is_object())
        cpp_value = vm.heap().allocate<WebIDL::CallbackType>(value.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onnavigatesuccess(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::onnavigateerror_getter)
{
    WebIDL::log_trace(vm, "NavigationPrototype::onnavigateerror_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onnavigateerror(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::onnavigateerror_setter)
{
    WebIDL::log_trace(vm, "NavigationPrototype::onnavigateerror_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    GC::Ptr<WebIDL::CallbackType> cpp_value;
    if (value.is_object())
        cpp_value = vm.heap().allocate<WebIDL::CallbackType>(value.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onnavigateerror(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::oncurrententrychange_getter)
{
    WebIDL::log_trace(vm, "NavigationPrototype::oncurrententrychange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncurrententrychange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::oncurrententrychange_setter)
{
    WebIDL::log_trace(vm, "NavigationPrototype::oncurrententrychange_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    GC::Ptr<WebIDL::CallbackType> cpp_value;
    if (value.is_object())
        cpp_value = vm.heap().allocate<WebIDL::CallbackType>(value.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_oncurrententrychange(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::entries)
{
    WebIDL::log_trace(vm, "NavigationPrototype::entries");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->entries(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::update_current_entry)
{
    WebIDL::log_trace(vm, "NavigationPrototype::update_current_entry");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "updateCurrentEntry");

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "NavigationUpdateCurrentEntryOptions");

    NavigationUpdateCurrentEntryOptions options {};

    auto state_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        state_property_value_0 = TRY(arg0.as_object().get("state"_utf16_fly_string));

    if (state_property_value_0.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "state");

    auto state_value_0 = state_property_value_0;

    options.state = state_value_0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->update_current_entry(options); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::navigate)
{
    WebIDL::log_trace(vm, "NavigationPrototype::navigate");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "navigate");

    auto arg0 = vm.argument(0);

    String url;
    if (!false || !arg0.is_null()) {
        url = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "NavigationNavigateOptions");

    NavigationNavigateOptions options {};

    auto history_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        history_property_value_1 = TRY(arg1.as_object().get("history"_utf16_fly_string));

    NavigationHistoryBehavior history_value_1 { NavigationHistoryBehavior::Auto };

    if (!history_property_value_1.is_undefined()) {

    auto history_property_value_1_string = TRY(history_property_value_1.to_string(vm));

    if (history_property_value_1_string == "auto"sv)
        history_value_1 = NavigationHistoryBehavior::Auto;

    else if (history_property_value_1_string == "push"sv)
        history_value_1 = NavigationHistoryBehavior::Push;

    else if (history_property_value_1_string == "replace"sv)
        history_value_1 = NavigationHistoryBehavior::Replace;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, history_property_value_1_string, "NavigationHistoryBehavior");

    }

    options.history = history_value_1;

    auto state_property_value_2 = JS::js_undefined();
    if (arg1.is_object())
        state_property_value_2 = TRY(arg1.as_object().get("state"_utf16_fly_string));

    if (!state_property_value_2.is_undefined()) {

    JS::Value state_value_2 = JS::js_undefined();
    if (!state_property_value_2.is_undefined())
        state_value_2 = state_property_value_2;

    options.state = state_value_2;

    }

    auto info_property_value_3 = JS::js_undefined();
    if (arg1.is_object())
        info_property_value_3 = TRY(arg1.as_object().get("info"_utf16_fly_string));

    if (!info_property_value_3.is_undefined()) {

    JS::Value info_value_3 = JS::js_undefined();
    if (!info_property_value_3.is_undefined())
        info_value_3 = info_property_value_3;

    options.info = info_value_3;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->navigate(url, options); }));

    {
        auto dictionary_object0 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_committed0_value;

    wrapped_committed0_value = GC::Ref { as<JS::Promise>(*retval.committed->promise()) };

        MUST(dictionary_object0->create_data_property("committed"_utf16_fly_string, wrapped_committed0_value));

        JS::Value wrapped_finished0_value;

    wrapped_finished0_value = GC::Ref { as<JS::Promise>(*retval.finished->promise()) };

        MUST(dictionary_object0->create_data_property("finished"_utf16_fly_string, wrapped_finished0_value));

        return dictionary_object0;
    }

}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::reload)
{
    WebIDL::log_trace(vm, "NavigationPrototype::reload");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "NavigationReloadOptions");

    NavigationReloadOptions options {};

    auto state_property_value_4 = JS::js_undefined();
    if (arg0.is_object())
        state_property_value_4 = TRY(arg0.as_object().get("state"_utf16_fly_string));

    if (!state_property_value_4.is_undefined()) {

    JS::Value state_value_4 = JS::js_undefined();
    if (!state_property_value_4.is_undefined())
        state_value_4 = state_property_value_4;

    options.state = state_value_4;

    }

    auto info_property_value_5 = JS::js_undefined();
    if (arg0.is_object())
        info_property_value_5 = TRY(arg0.as_object().get("info"_utf16_fly_string));

    if (!info_property_value_5.is_undefined()) {

    JS::Value info_value_5 = JS::js_undefined();
    if (!info_property_value_5.is_undefined())
        info_value_5 = info_property_value_5;

    options.info = info_value_5;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->reload(options); }));

    {
        auto dictionary_object0 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_committed0_value;

    wrapped_committed0_value = GC::Ref { as<JS::Promise>(*retval.committed->promise()) };

        MUST(dictionary_object0->create_data_property("committed"_utf16_fly_string, wrapped_committed0_value));

        JS::Value wrapped_finished0_value;

    wrapped_finished0_value = GC::Ref { as<JS::Promise>(*retval.finished->promise()) };

        MUST(dictionary_object0->create_data_property("finished"_utf16_fly_string, wrapped_finished0_value));

        return dictionary_object0;
    }

}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::traverse_to)
{
    WebIDL::log_trace(vm, "NavigationPrototype::traverse_to");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "traverseTo");

    auto arg0 = vm.argument(0);

    String key;
    if (!false || !arg0.is_null()) {
        key = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "NavigationOptions");

    NavigationOptions options {};

    auto info_property_value_6 = JS::js_undefined();
    if (arg1.is_object())
        info_property_value_6 = TRY(arg1.as_object().get("info"_utf16_fly_string));

    if (!info_property_value_6.is_undefined()) {

    JS::Value info_value_6 = JS::js_undefined();
    if (!info_property_value_6.is_undefined())
        info_value_6 = info_property_value_6;

    options.info = info_value_6;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->traverse_to(key, options); }));

    {
        auto dictionary_object0 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_committed0_value;

    wrapped_committed0_value = GC::Ref { as<JS::Promise>(*retval.committed->promise()) };

        MUST(dictionary_object0->create_data_property("committed"_utf16_fly_string, wrapped_committed0_value));

        JS::Value wrapped_finished0_value;

    wrapped_finished0_value = GC::Ref { as<JS::Promise>(*retval.finished->promise()) };

        MUST(dictionary_object0->create_data_property("finished"_utf16_fly_string, wrapped_finished0_value));

        return dictionary_object0;
    }

}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::back)
{
    WebIDL::log_trace(vm, "NavigationPrototype::back");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "NavigationOptions");

    NavigationOptions options {};

    auto info_property_value_7 = JS::js_undefined();
    if (arg0.is_object())
        info_property_value_7 = TRY(arg0.as_object().get("info"_utf16_fly_string));

    if (!info_property_value_7.is_undefined()) {

    JS::Value info_value_7 = JS::js_undefined();
    if (!info_property_value_7.is_undefined())
        info_value_7 = info_property_value_7;

    options.info = info_value_7;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->back(options); }));

    {
        auto dictionary_object0 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_committed0_value;

    wrapped_committed0_value = GC::Ref { as<JS::Promise>(*retval.committed->promise()) };

        MUST(dictionary_object0->create_data_property("committed"_utf16_fly_string, wrapped_committed0_value));

        JS::Value wrapped_finished0_value;

    wrapped_finished0_value = GC::Ref { as<JS::Promise>(*retval.finished->promise()) };

        MUST(dictionary_object0->create_data_property("finished"_utf16_fly_string, wrapped_finished0_value));

        return dictionary_object0;
    }

}

JS_DEFINE_NATIVE_FUNCTION(NavigationPrototype::forward)
{
    WebIDL::log_trace(vm, "NavigationPrototype::forward");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "NavigationOptions");

    NavigationOptions options {};

    auto info_property_value_8 = JS::js_undefined();
    if (arg0.is_object())
        info_property_value_8 = TRY(arg0.as_object().get("info"_utf16_fly_string));

    if (!info_property_value_8.is_undefined()) {

    JS::Value info_value_8 = JS::js_undefined();
    if (!info_property_value_8.is_undefined())
        info_value_8 = info_property_value_8;

    options.info = info_value_8;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->forward(options); }));

    {
        auto dictionary_object0 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_committed0_value;

    wrapped_committed0_value = GC::Ref { as<JS::Promise>(*retval.committed->promise()) };

        MUST(dictionary_object0->create_data_property("committed"_utf16_fly_string, wrapped_committed0_value));

        JS::Value wrapped_finished0_value;

    wrapped_finished0_value = GC::Ref { as<JS::Promise>(*retval.finished->promise()) };

        MUST(dictionary_object0->create_data_property("finished"_utf16_fly_string, wrapped_finished0_value));

        return dictionary_object0;
    }

}

} // namespace Web::Bindings
