
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
#include <LibWeb/Bindings/NavigateEventPrototype.h>
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

GC_DEFINE_ALLOCATOR(NavigateEventPrototype);

NavigateEventPrototype::NavigateEventPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

NavigateEventPrototype::~NavigateEventPrototype()
{
}

void NavigateEventPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventPrototype>(realm, "Event"_fly_string));


    auto native_navigation_type_getter = JS::NativeFunction::create(realm, navigation_type_getter, 0, "navigationType"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_navigation_type_setter;

    define_direct_accessor("navigationType"_utf16_fly_string, native_navigation_type_getter, native_navigation_type_setter, default_attributes);

    auto native_destination_getter = JS::NativeFunction::create(realm, destination_getter, 0, "destination"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_destination_setter;

    define_direct_accessor("destination"_utf16_fly_string, native_destination_getter, native_destination_setter, default_attributes);

    auto native_can_intercept_getter = JS::NativeFunction::create(realm, can_intercept_getter, 0, "canIntercept"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_can_intercept_setter;

    define_direct_accessor("canIntercept"_utf16_fly_string, native_can_intercept_getter, native_can_intercept_setter, default_attributes);

    auto native_user_initiated_getter = JS::NativeFunction::create(realm, user_initiated_getter, 0, "userInitiated"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_user_initiated_setter;

    define_direct_accessor("userInitiated"_utf16_fly_string, native_user_initiated_getter, native_user_initiated_setter, default_attributes);

    auto native_hash_change_getter = JS::NativeFunction::create(realm, hash_change_getter, 0, "hashChange"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_hash_change_setter;

    define_direct_accessor("hashChange"_utf16_fly_string, native_hash_change_getter, native_hash_change_setter, default_attributes);

    auto native_signal_getter = JS::NativeFunction::create(realm, signal_getter, 0, "signal"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_signal_setter;

    define_direct_accessor("signal"_utf16_fly_string, native_signal_getter, native_signal_setter, default_attributes);

    auto native_form_data_getter = JS::NativeFunction::create(realm, form_data_getter, 0, "formData"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_form_data_setter;

    define_direct_accessor("formData"_utf16_fly_string, native_form_data_getter, native_form_data_setter, default_attributes);

    auto native_download_request_getter = JS::NativeFunction::create(realm, download_request_getter, 0, "downloadRequest"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_download_request_setter;

    define_direct_accessor("downloadRequest"_utf16_fly_string, native_download_request_getter, native_download_request_setter, default_attributes);

    auto native_info_getter = JS::NativeFunction::create(realm, info_getter, 0, "info"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_info_setter;

    define_direct_accessor("info"_utf16_fly_string, native_info_getter, native_info_setter, default_attributes);

    auto native_has_ua_visual_transition_getter = JS::NativeFunction::create(realm, has_ua_visual_transition_getter, 0, "hasUAVisualTransition"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_has_ua_visual_transition_setter;

    define_direct_accessor("hasUAVisualTransition"_utf16_fly_string, native_has_ua_visual_transition_getter, native_has_ua_visual_transition_setter, default_attributes);

    auto native_source_element_getter = JS::NativeFunction::create(realm, source_element_getter, 0, "sourceElement"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_source_element_setter;

    define_direct_accessor("sourceElement"_utf16_fly_string, native_source_element_getter, native_source_element_setter, default_attributes);

    define_native_function(realm, "intercept"_utf16_fly_string, intercept, 0, default_attributes);

    define_native_function(realm, "scroll"_utf16_fly_string, scroll, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "NavigateEvent"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void NavigateEventPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::NavigateEvent*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::NavigateEvent>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "NavigateEvent");
    return static_cast<HTML::NavigateEvent*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(NavigateEventPrototype::navigation_type_getter)
{
    WebIDL::log_trace(vm, "NavigateEventPrototype::navigation_type_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->navigation_type(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(NavigateEventPrototype::destination_getter)
{
    WebIDL::log_trace(vm, "NavigateEventPrototype::destination_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->destination(); }));

    return &const_cast<NavigationDestination&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigateEventPrototype::can_intercept_getter)
{
    WebIDL::log_trace(vm, "NavigateEventPrototype::can_intercept_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->can_intercept(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigateEventPrototype::user_initiated_getter)
{
    WebIDL::log_trace(vm, "NavigateEventPrototype::user_initiated_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->user_initiated(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigateEventPrototype::hash_change_getter)
{
    WebIDL::log_trace(vm, "NavigateEventPrototype::hash_change_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->hash_change(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigateEventPrototype::signal_getter)
{
    WebIDL::log_trace(vm, "NavigateEventPrototype::signal_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->signal(); }));

    return &const_cast<AbortSignal&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigateEventPrototype::form_data_getter)
{
    WebIDL::log_trace(vm, "NavigateEventPrototype::form_data_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->form_data(); }));

    if (retval) {

    return &const_cast<FormData&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(NavigateEventPrototype::download_request_getter)
{
    WebIDL::log_trace(vm, "NavigateEventPrototype::download_request_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->download_request(); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(NavigateEventPrototype::info_getter)
{
    WebIDL::log_trace(vm, "NavigateEventPrototype::info_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->info(); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(NavigateEventPrototype::has_ua_visual_transition_getter)
{
    WebIDL::log_trace(vm, "NavigateEventPrototype::has_ua_visual_transition_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->has_ua_visual_transition(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NavigateEventPrototype::source_element_getter)
{
    WebIDL::log_trace(vm, "NavigateEventPrototype::source_element_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->source_element(); }));

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(NavigateEventPrototype::intercept)
{
    WebIDL::log_trace(vm, "NavigateEventPrototype::intercept");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "NavigationInterceptOptions");

    NavigationInterceptOptions options {};

    auto focus_reset_property_value_14 = JS::js_undefined();
    if (arg0.is_object())
        focus_reset_property_value_14 = TRY(arg0.as_object().get("focusReset"_utf16_fly_string));

    if (!focus_reset_property_value_14.is_undefined()) {

    NavigationFocusReset focus_reset_value_14 { NavigationFocusReset::AfterTransition };

    if (!focus_reset_property_value_14.is_undefined()) {

    auto focus_reset_property_value_14_string = TRY(focus_reset_property_value_14.to_string(vm));

    if (focus_reset_property_value_14_string == "after-transition"sv)
        focus_reset_value_14 = NavigationFocusReset::AfterTransition;

    else if (focus_reset_property_value_14_string == "manual"sv)
        focus_reset_value_14 = NavigationFocusReset::Manual;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, focus_reset_property_value_14_string, "NavigationFocusReset");

    }

    options.focus_reset = focus_reset_value_14;

    }

    auto handler_property_value_15 = JS::js_undefined();
    if (arg0.is_object())
        handler_property_value_15 = TRY(arg0.as_object().get("handler"_utf16_fly_string));

    if (!handler_property_value_15.is_undefined()) {

    if (!handler_property_value_15.is_function()
&& !handler_property_value_15.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, handler_property_value_15);

    GC::Ptr<WebIDL::CallbackType> handler_value_15;
    if (handler_property_value_15.is_object())
        handler_value_15 = vm.heap().allocate<WebIDL::CallbackType>(handler_property_value_15.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    options.handler = handler_value_15;

    }

    auto scroll_property_value_16 = JS::js_undefined();
    if (arg0.is_object())
        scroll_property_value_16 = TRY(arg0.as_object().get("scroll"_utf16_fly_string));

    if (!scroll_property_value_16.is_undefined()) {

    NavigationScrollBehavior scroll_value_16 { NavigationScrollBehavior::AfterTransition };

    if (!scroll_property_value_16.is_undefined()) {

    auto scroll_property_value_16_string = TRY(scroll_property_value_16.to_string(vm));

    if (scroll_property_value_16_string == "after-transition"sv)
        scroll_value_16 = NavigationScrollBehavior::AfterTransition;

    else if (scroll_property_value_16_string == "manual"sv)
        scroll_value_16 = NavigationScrollBehavior::Manual;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, scroll_property_value_16_string, "NavigationScrollBehavior");

    }

    options.scroll = scroll_value_16;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->intercept(options); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(NavigateEventPrototype::scroll)
{
    WebIDL::log_trace(vm, "NavigateEventPrototype::scroll");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll(); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
