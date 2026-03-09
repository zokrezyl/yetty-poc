
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
#include <LibWeb/Bindings/EventPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

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

GC_DEFINE_ALLOCATOR(EventPrototype);

EventPrototype::EventPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

EventPrototype::~EventPrototype()
{
}

void EventPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_type_getter = JS::NativeFunction::create(realm, type_getter, 0, "type"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_type_setter;

    define_direct_accessor("type"_utf16_fly_string, native_type_getter, native_type_setter, default_attributes);

    auto native_target_getter = JS::NativeFunction::create(realm, target_getter, 0, "target"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_target_setter;

    define_direct_accessor("target"_utf16_fly_string, native_target_getter, native_target_setter, default_attributes);

    auto native_src_element_getter = JS::NativeFunction::create(realm, src_element_getter, 0, "srcElement"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_src_element_setter;

    define_direct_accessor("srcElement"_utf16_fly_string, native_src_element_getter, native_src_element_setter, default_attributes);

    auto native_current_target_getter = JS::NativeFunction::create(realm, current_target_getter, 0, "currentTarget"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_current_target_setter;

    define_direct_accessor("currentTarget"_utf16_fly_string, native_current_target_getter, native_current_target_setter, default_attributes);

    auto native_event_phase_getter = JS::NativeFunction::create(realm, event_phase_getter, 0, "eventPhase"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_event_phase_setter;

    define_direct_accessor("eventPhase"_utf16_fly_string, native_event_phase_getter, native_event_phase_setter, default_attributes);

    auto native_cancel_bubble_getter = JS::NativeFunction::create(realm, cancel_bubble_getter, 0, "cancelBubble"_utf16_fly_string, &realm, "get"sv);

    auto native_cancel_bubble_setter = JS::NativeFunction::create(realm, cancel_bubble_setter, 1, "cancelBubble"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("cancelBubble"_utf16_fly_string, native_cancel_bubble_getter, native_cancel_bubble_setter, default_attributes);

    auto native_bubbles_getter = JS::NativeFunction::create(realm, bubbles_getter, 0, "bubbles"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_bubbles_setter;

    define_direct_accessor("bubbles"_utf16_fly_string, native_bubbles_getter, native_bubbles_setter, default_attributes);

    auto native_cancelable_getter = JS::NativeFunction::create(realm, cancelable_getter, 0, "cancelable"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_cancelable_setter;

    define_direct_accessor("cancelable"_utf16_fly_string, native_cancelable_getter, native_cancelable_setter, default_attributes);

    auto native_return_value_getter = JS::NativeFunction::create(realm, return_value_getter, 0, "returnValue"_utf16_fly_string, &realm, "get"sv);

    auto native_return_value_setter = JS::NativeFunction::create(realm, return_value_setter, 1, "returnValue"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("returnValue"_utf16_fly_string, native_return_value_getter, native_return_value_setter, default_attributes);

    auto native_default_prevented_getter = JS::NativeFunction::create(realm, default_prevented_getter, 0, "defaultPrevented"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_default_prevented_setter;

    define_direct_accessor("defaultPrevented"_utf16_fly_string, native_default_prevented_getter, native_default_prevented_setter, default_attributes);

    auto native_composed_getter = JS::NativeFunction::create(realm, composed_getter, 0, "composed"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_composed_setter;

    define_direct_accessor("composed"_utf16_fly_string, native_composed_getter, native_composed_setter, default_attributes);

    auto native_time_stamp_getter = JS::NativeFunction::create(realm, time_stamp_getter, 0, "timeStamp"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_time_stamp_setter;

    define_direct_accessor("timeStamp"_utf16_fly_string, native_time_stamp_getter, native_time_stamp_setter, default_attributes);

    auto constant_NONE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0));

    define_direct_property("NONE"_utf16_fly_string, constant_NONE_value, JS::Attribute::Enumerable);

    auto constant_CAPTURING_PHASE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(1));

    define_direct_property("CAPTURING_PHASE"_utf16_fly_string, constant_CAPTURING_PHASE_value, JS::Attribute::Enumerable);

    auto constant_AT_TARGET_value = JS::Value(static_cast<WebIDL::UnsignedShort>(2));

    define_direct_property("AT_TARGET"_utf16_fly_string, constant_AT_TARGET_value, JS::Attribute::Enumerable);

    auto constant_BUBBLING_PHASE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(3));

    define_direct_property("BUBBLING_PHASE"_utf16_fly_string, constant_BUBBLING_PHASE_value, JS::Attribute::Enumerable);

    define_native_function(realm, "composedPath"_utf16_fly_string, composed_path, 0, default_attributes);

    define_native_function(realm, "stopPropagation"_utf16_fly_string, stop_propagation, 0, default_attributes);

    define_native_function(realm, "stopImmediatePropagation"_utf16_fly_string, stop_immediate_propagation, 0, default_attributes);

    define_native_function(realm, "preventDefault"_utf16_fly_string, prevent_default, 0, default_attributes);

    define_native_function(realm, "initEvent"_utf16_fly_string, init_event, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Event"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void EventPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    auto native_is_trusted_getter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Event"_utf16_fly_string, "isTrusted"_utf16_fly_string, is_trusted_getter, UnforgeableKey::Type::Getter);

    GC::Ptr<JS::NativeFunction> native_is_trusted_setter;

    object.define_direct_accessor("isTrusted"_utf16_fly_string, native_is_trusted_getter, native_is_trusted_setter, default_attributes);

}

[[maybe_unused]] static JS::ThrowCompletionOr<DOM::Event*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<DOM::Event>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Event");
    return static_cast<DOM::Event*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::type_getter)
{
    WebIDL::log_trace(vm, "EventPrototype::type_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->type(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::target_getter)
{
    WebIDL::log_trace(vm, "EventPrototype::target_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->target(); }));

    if (retval) {

    return &const_cast<EventTarget&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::src_element_getter)
{
    WebIDL::log_trace(vm, "EventPrototype::src_element_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->src_element(); }));

    if (retval) {

    return &const_cast<EventTarget&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::current_target_getter)
{
    WebIDL::log_trace(vm, "EventPrototype::current_target_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->current_target_for_bindings(); }));

    if (retval) {

    return &const_cast<EventTarget&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::event_phase_getter)
{
    WebIDL::log_trace(vm, "EventPrototype::event_phase_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->event_phase(); }));

    return JS::Value(static_cast<WebIDL::UnsignedShort>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::cancel_bubble_getter)
{
    WebIDL::log_trace(vm, "EventPrototype::cancel_bubble_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cancel_bubble(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::cancel_bubble_setter)
{
    WebIDL::log_trace(vm, "EventPrototype::cancel_bubble_setter");
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

    bool cpp_value;

    cpp_value = value.to_boolean();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_cancel_bubble(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::bubbles_getter)
{
    WebIDL::log_trace(vm, "EventPrototype::bubbles_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bubbles(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::cancelable_getter)
{
    WebIDL::log_trace(vm, "EventPrototype::cancelable_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cancelable(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::return_value_getter)
{
    WebIDL::log_trace(vm, "EventPrototype::return_value_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->return_value(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::return_value_setter)
{
    WebIDL::log_trace(vm, "EventPrototype::return_value_setter");
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

    bool cpp_value;

    cpp_value = value.to_boolean();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_return_value(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::default_prevented_getter)
{
    WebIDL::log_trace(vm, "EventPrototype::default_prevented_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->default_prevented(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::composed_getter)
{
    WebIDL::log_trace(vm, "EventPrototype::composed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->composed(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::is_trusted_getter)
{
    WebIDL::log_trace(vm, "EventPrototype::is_trusted_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_trusted(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::time_stamp_getter)
{
    WebIDL::log_trace(vm, "EventPrototype::time_stamp_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->time_stamp(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::composed_path)
{
    WebIDL::log_trace(vm, "EventPrototype::composed_path");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->composed_path(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::stop_propagation)
{
    WebIDL::log_trace(vm, "EventPrototype::stop_propagation");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->stop_propagation(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::stop_immediate_propagation)
{
    WebIDL::log_trace(vm, "EventPrototype::stop_immediate_propagation");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->stop_immediate_propagation(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::prevent_default)
{
    WebIDL::log_trace(vm, "EventPrototype::prevent_default");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->prevent_default(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(EventPrototype::init_event)
{
    WebIDL::log_trace(vm, "EventPrototype::init_event");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "initEvent");

    auto arg0 = vm.argument(0);

    String type;
    if (!false || !arg0.is_null()) {
        type = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    bool bubbles;

    if (!arg1.is_undefined())

    bubbles = arg1.to_boolean();

    else
        bubbles = static_cast<bool>(false);

    auto arg2 = vm.argument(2);

    bool cancelable;

    if (!arg2.is_undefined())

    cancelable = arg2.to_boolean();

    else
        cancelable = static_cast<bool>(false);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->init_event(type, bubbles, cancelable); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
