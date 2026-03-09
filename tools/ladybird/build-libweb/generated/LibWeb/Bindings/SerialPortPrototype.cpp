
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
#include <LibWeb/Bindings/SerialPortPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Serial/SerialPort.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

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

GC_DEFINE_ALLOCATOR(SerialPortPrototype);

SerialPortPrototype::SerialPortPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

SerialPortPrototype::~SerialPortPrototype()
{
}

void SerialPortPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    auto native_onconnect_getter = JS::NativeFunction::create(realm, onconnect_getter, 0, "onconnect"_utf16_fly_string, &realm, "get"sv);

    auto native_onconnect_setter = JS::NativeFunction::create(realm, onconnect_setter, 1, "onconnect"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onconnect"_utf16_fly_string, native_onconnect_getter, native_onconnect_setter, default_attributes);

    auto native_ondisconnect_getter = JS::NativeFunction::create(realm, ondisconnect_getter, 0, "ondisconnect"_utf16_fly_string, &realm, "get"sv);

    auto native_ondisconnect_setter = JS::NativeFunction::create(realm, ondisconnect_setter, 1, "ondisconnect"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ondisconnect"_utf16_fly_string, native_ondisconnect_getter, native_ondisconnect_setter, default_attributes);

    auto native_connected_getter = JS::NativeFunction::create(realm, connected_getter, 0, "connected"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_connected_setter;

    define_direct_accessor("connected"_utf16_fly_string, native_connected_getter, native_connected_setter, default_attributes);

    auto native_readable_getter = JS::NativeFunction::create(realm, readable_getter, 0, "readable"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_readable_setter;

    define_direct_accessor("readable"_utf16_fly_string, native_readable_getter, native_readable_setter, default_attributes);

    auto native_writable_getter = JS::NativeFunction::create(realm, writable_getter, 0, "writable"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_writable_setter;

    define_direct_accessor("writable"_utf16_fly_string, native_writable_getter, native_writable_setter, default_attributes);

    define_native_function(realm, "getInfo"_utf16_fly_string, get_info, 0, default_attributes);

    define_native_function(realm, "open"_utf16_fly_string, open, 1, default_attributes);

    define_native_function(realm, "setSignals"_utf16_fly_string, set_signals, 0, default_attributes);

    define_native_function(realm, "getSignals"_utf16_fly_string, get_signals, 0, default_attributes);

    define_native_function(realm, "close"_utf16_fly_string, close, 0, default_attributes);

    define_native_function(realm, "forget"_utf16_fly_string, forget, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "SerialPort"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void SerialPortPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Serial::SerialPort*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Serial::SerialPort>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SerialPort");
    return static_cast<Serial::SerialPort*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(SerialPortPrototype::onconnect_getter)
{
    WebIDL::log_trace(vm, "SerialPortPrototype::onconnect_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onconnect(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(SerialPortPrototype::onconnect_setter)
{
    WebIDL::log_trace(vm, "SerialPortPrototype::onconnect_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onconnect(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(SerialPortPrototype::ondisconnect_getter)
{
    WebIDL::log_trace(vm, "SerialPortPrototype::ondisconnect_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondisconnect(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(SerialPortPrototype::ondisconnect_setter)
{
    WebIDL::log_trace(vm, "SerialPortPrototype::ondisconnect_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ondisconnect(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(SerialPortPrototype::connected_getter)
{
    WebIDL::log_trace(vm, "SerialPortPrototype::connected_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->connected(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(SerialPortPrototype::readable_getter)
{
    WebIDL::log_trace(vm, "SerialPortPrototype::readable_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->readable(); }));

    return &const_cast<ReadableStream&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SerialPortPrototype::writable_getter)
{
    WebIDL::log_trace(vm, "SerialPortPrototype::writable_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->writable(); }));

    return &const_cast<WritableStream&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SerialPortPrototype::get_info)
{
    WebIDL::log_trace(vm, "SerialPortPrototype::get_info");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_info(); }));

    {
        auto dictionary_object0 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        Optional<JS::Value> wrapped_bluetooth_service_class_id0_value;

    if (retval.bluetooth_service_class_id.has_value()) {

    wrapped_bluetooth_service_class_id0_value = JS::PrimitiveString::create(vm, const_cast<decltype(retval.bluetooth_service_class_id)&>(retval.bluetooth_service_class_id).release_value());

    }

        if (wrapped_bluetooth_service_class_id0_value.has_value())
            MUST(dictionary_object0->create_data_property("bluetoothServiceClassId"_utf16_fly_string, wrapped_bluetooth_service_class_id0_value.release_value()));

        Optional<JS::Value> wrapped_usb_product_id0_value;

    if (retval.usb_product_id.has_value()) {

    wrapped_usb_product_id0_value = JS::Value(static_cast<WebIDL::UnsignedShort>(retval.usb_product_id.release_value()));

    }

        if (wrapped_usb_product_id0_value.has_value())
            MUST(dictionary_object0->create_data_property("usbProductId"_utf16_fly_string, wrapped_usb_product_id0_value.release_value()));

        Optional<JS::Value> wrapped_usb_vendor_id0_value;

    if (retval.usb_vendor_id.has_value()) {

    wrapped_usb_vendor_id0_value = JS::Value(static_cast<WebIDL::UnsignedShort>(retval.usb_vendor_id.release_value()));

    }

        if (wrapped_usb_vendor_id0_value.has_value())
            MUST(dictionary_object0->create_data_property("usbVendorId"_utf16_fly_string, wrapped_usb_vendor_id0_value.release_value()));

        return dictionary_object0;
    }

}

JS_DEFINE_NATIVE_FUNCTION(SerialPortPrototype::open)
{
    WebIDL::log_trace(vm, "SerialPortPrototype::open");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "open");

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SerialOptions");

    SerialOptions options {};

    auto baud_rate_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        baud_rate_property_value_0 = TRY(arg0.as_object().get("baudRate"_utf16_fly_string));

    if (!baud_rate_property_value_0.is_undefined()) {

    Optional<WebIDL::UnsignedLong> baud_rate_value_0;

    if (!baud_rate_property_value_0.is_undefined())

    baud_rate_value_0 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, baud_rate_property_value_0, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    options.baud_rate = baud_rate_value_0;

    }

    auto buffer_size_property_value_1 = JS::js_undefined();
    if (arg0.is_object())
        buffer_size_property_value_1 = TRY(arg0.as_object().get("bufferSize"_utf16_fly_string));

    WebIDL::UnsignedLong buffer_size_value_1;

    if (!buffer_size_property_value_1.is_undefined())

    buffer_size_value_1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, buffer_size_property_value_1, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    else
        buffer_size_value_1 = static_cast<WebIDL::UnsignedLong>(255);

    options.buffer_size = buffer_size_value_1;

    auto data_bits_property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        data_bits_property_value_2 = TRY(arg0.as_object().get("dataBits"_utf16_fly_string));

    WebIDL::Octet data_bits_value_2;

    if (!data_bits_property_value_2.is_undefined())

    data_bits_value_2 = TRY(WebIDL::convert_to_int<WebIDL::Octet>(vm, data_bits_property_value_2, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    else
        data_bits_value_2 = static_cast<WebIDL::Octet>(8);

    options.data_bits = data_bits_value_2;

    auto flow_control_property_value_3 = JS::js_undefined();
    if (arg0.is_object())
        flow_control_property_value_3 = TRY(arg0.as_object().get("flowControl"_utf16_fly_string));

    FlowControlType flow_control_value_3 { FlowControlType::None };

    if (!flow_control_property_value_3.is_undefined()) {

    auto flow_control_property_value_3_string = TRY(flow_control_property_value_3.to_string(vm));

    if (flow_control_property_value_3_string == "none"sv)
        flow_control_value_3 = FlowControlType::None;

    else if (flow_control_property_value_3_string == "hardware"sv)
        flow_control_value_3 = FlowControlType::Hardware;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, flow_control_property_value_3_string, "FlowControlType");

    }

    options.flow_control = flow_control_value_3;

    auto parity_property_value_4 = JS::js_undefined();
    if (arg0.is_object())
        parity_property_value_4 = TRY(arg0.as_object().get("parity"_utf16_fly_string));

    ParityType parity_value_4 { ParityType::None };

    if (!parity_property_value_4.is_undefined()) {

    auto parity_property_value_4_string = TRY(parity_property_value_4.to_string(vm));

    if (parity_property_value_4_string == "none"sv)
        parity_value_4 = ParityType::None;

    else if (parity_property_value_4_string == "even"sv)
        parity_value_4 = ParityType::Even;

    else if (parity_property_value_4_string == "odd"sv)
        parity_value_4 = ParityType::Odd;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, parity_property_value_4_string, "ParityType");

    }

    options.parity = parity_value_4;

    auto stop_bits_property_value_5 = JS::js_undefined();
    if (arg0.is_object())
        stop_bits_property_value_5 = TRY(arg0.as_object().get("stopBits"_utf16_fly_string));

    WebIDL::Octet stop_bits_value_5;

    if (!stop_bits_property_value_5.is_undefined())

    stop_bits_value_5 = TRY(WebIDL::convert_to_int<WebIDL::Octet>(vm, stop_bits_property_value_5, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    else
        stop_bits_value_5 = static_cast<WebIDL::Octet>(1);

    options.stop_bits = stop_bits_value_5;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->open(options); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If op has a return type that is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    // NOTE: We know that this is a Promise return type statically by the IDL.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

JS_DEFINE_NATIVE_FUNCTION(SerialPortPrototype::set_signals)
{
    WebIDL::log_trace(vm, "SerialPortPrototype::set_signals");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SerialOutputSignals");

    SerialOutputSignals signals {};

    auto break__property_value_6 = JS::js_undefined();
    if (arg0.is_object())
        break__property_value_6 = TRY(arg0.as_object().get("break"_utf16_fly_string));

    if (!break__property_value_6.is_undefined()) {

    Optional<bool> break__value_6;

    if (!break__property_value_6.is_undefined())

    break__value_6 = break__property_value_6.to_boolean();

    signals.break_ = break__value_6;

    }

    auto data_terminal_ready_property_value_7 = JS::js_undefined();
    if (arg0.is_object())
        data_terminal_ready_property_value_7 = TRY(arg0.as_object().get("dataTerminalReady"_utf16_fly_string));

    if (!data_terminal_ready_property_value_7.is_undefined()) {

    Optional<bool> data_terminal_ready_value_7;

    if (!data_terminal_ready_property_value_7.is_undefined())

    data_terminal_ready_value_7 = data_terminal_ready_property_value_7.to_boolean();

    signals.data_terminal_ready = data_terminal_ready_value_7;

    }

    auto request_to_send_property_value_8 = JS::js_undefined();
    if (arg0.is_object())
        request_to_send_property_value_8 = TRY(arg0.as_object().get("requestToSend"_utf16_fly_string));

    if (!request_to_send_property_value_8.is_undefined()) {

    Optional<bool> request_to_send_value_8;

    if (!request_to_send_property_value_8.is_undefined())

    request_to_send_value_8 = request_to_send_property_value_8.to_boolean();

    signals.request_to_send = request_to_send_value_8;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_signals(signals); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If op has a return type that is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    // NOTE: We know that this is a Promise return type statically by the IDL.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

JS_DEFINE_NATIVE_FUNCTION(SerialPortPrototype::get_signals)
{
    WebIDL::log_trace(vm, "SerialPortPrototype::get_signals");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_signals(); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If op has a return type that is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    // NOTE: We know that this is a Promise return type statically by the IDL.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

JS_DEFINE_NATIVE_FUNCTION(SerialPortPrototype::close)
{
    WebIDL::log_trace(vm, "SerialPortPrototype::close");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->close(); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If op has a return type that is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    // NOTE: We know that this is a Promise return type statically by the IDL.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

JS_DEFINE_NATIVE_FUNCTION(SerialPortPrototype::forget)
{
    WebIDL::log_trace(vm, "SerialPortPrototype::forget");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->forget(); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If op has a return type that is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    // NOTE: We know that this is a Promise return type statically by the IDL.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

} // namespace Web::Bindings
