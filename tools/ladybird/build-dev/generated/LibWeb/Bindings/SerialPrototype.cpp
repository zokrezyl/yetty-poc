
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
#include <LibWeb/Bindings/SerialPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Serial/Serial.h>

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

GC_DEFINE_ALLOCATOR(SerialPrototype);

SerialPrototype::SerialPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

SerialPrototype::~SerialPrototype()
{
}

void SerialPrototype::initialize(JS::Realm& realm)
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

    define_native_function(realm, "getPorts"_utf16_fly_string, get_ports, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Serial"_string), JS::Attribute::Configurable);

    if (is<HTML::Window>(realm.global_object())) {

    define_native_function(realm, "requestPort"_utf16_fly_string, request_port, 0, default_attributes);

    }

    Base::initialize(realm);

}

void SerialPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Serial::Serial*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Serial::Serial>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Serial");
    return static_cast<Serial::Serial*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(SerialPrototype::onconnect_getter)
{
    WebIDL::log_trace(vm, "SerialPrototype::onconnect_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onconnect(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(SerialPrototype::onconnect_setter)
{
    WebIDL::log_trace(vm, "SerialPrototype::onconnect_setter");
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

JS_DEFINE_NATIVE_FUNCTION(SerialPrototype::ondisconnect_getter)
{
    WebIDL::log_trace(vm, "SerialPrototype::ondisconnect_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondisconnect(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(SerialPrototype::ondisconnect_setter)
{
    WebIDL::log_trace(vm, "SerialPrototype::ondisconnect_setter");
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

JS_DEFINE_NATIVE_FUNCTION(SerialPrototype::get_ports)
{
    WebIDL::log_trace(vm, "SerialPrototype::get_ports");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_ports(); }));

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

JS_DEFINE_NATIVE_FUNCTION(SerialPrototype::request_port)
{
    WebIDL::log_trace(vm, "SerialPrototype::request_port");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SerialPortRequestOptions");

    SerialPortRequestOptions options {};

    auto allowed_bluetooth_service_class_ids_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        allowed_bluetooth_service_class_ids_property_value_0 = TRY(arg0.as_object().get("allowedBluetoothServiceClassIds"_utf16_fly_string));

    if (!allowed_bluetooth_service_class_ids_property_value_0.is_undefined()) {

    Optional<Vector<String>> allowed_bluetooth_service_class_ids_value_0;

    if (!allowed_bluetooth_service_class_ids_property_value_0.is_undefined()) {

    if (!allowed_bluetooth_service_class_ids_property_value_0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, allowed_bluetooth_service_class_ids_property_value_0);

    auto allowed_bluetooth_service_class_ids_property_value_0_iterator_method0 = TRY(allowed_bluetooth_service_class_ids_property_value_0.get_method(vm, vm.well_known_symbol_iterator()));
    if (!allowed_bluetooth_service_class_ids_property_value_0_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, allowed_bluetooth_service_class_ids_property_value_0);

    auto allowed_bluetooth_service_class_ids_property_value_0_iterator1 = TRY(JS::get_iterator_from_method(vm, allowed_bluetooth_service_class_ids_property_value_0, *allowed_bluetooth_service_class_ids_property_value_0_iterator_method0));

    Vector<String> allowed_bluetooth_service_class_ids_value_0_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, allowed_bluetooth_service_class_ids_property_value_0_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    String sequence_item1;
    if (!false || !next_item1.is_null()) {
        sequence_item1 = TRY(WebIDL::to_string(vm, next_item1));
    }

    allowed_bluetooth_service_class_ids_value_0_non_optional.append(sequence_item1);
    }

        allowed_bluetooth_service_class_ids_value_0 = move(allowed_bluetooth_service_class_ids_value_0_non_optional);
    }

    options.allowed_bluetooth_service_class_ids = allowed_bluetooth_service_class_ids_value_0;

    }

    auto filters_property_value_1 = JS::js_undefined();
    if (arg0.is_object())
        filters_property_value_1 = TRY(arg0.as_object().get("filters"_utf16_fly_string));

    if (!filters_property_value_1.is_undefined()) {

    Optional<Vector<SerialPortFilter>> filters_value_1;

    if (!filters_property_value_1.is_undefined()) {

    if (!filters_property_value_1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, filters_property_value_1);

    auto filters_property_value_1_iterator_method0 = TRY(filters_property_value_1.get_method(vm, vm.well_known_symbol_iterator()));
    if (!filters_property_value_1_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, filters_property_value_1);

    auto filters_property_value_1_iterator1 = TRY(JS::get_iterator_from_method(vm, filters_property_value_1, *filters_property_value_1_iterator_method0));

    Vector<SerialPortFilter> filters_value_1_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, filters_property_value_1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_nullish() && !next_item1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SerialPortFilter");

    SerialPortFilter sequence_item1 {};

    auto bluetooth_service_class_id_property_value_0 = JS::js_undefined();
    if (next_item1.is_object())
        bluetooth_service_class_id_property_value_0 = TRY(next_item1.as_object().get("bluetoothServiceClassId"_utf16_fly_string));

    if (!bluetooth_service_class_id_property_value_0.is_undefined()) {

    Optional<String> bluetooth_service_class_id_value_0;

    if (!bluetooth_service_class_id_property_value_0.is_undefined()) {
        if (!false || !bluetooth_service_class_id_property_value_0.is_null())
            bluetooth_service_class_id_value_0 = TRY(WebIDL::to_string(vm, bluetooth_service_class_id_property_value_0));
    }

    sequence_item1.bluetooth_service_class_id = bluetooth_service_class_id_value_0;

    }

    auto usb_product_id_property_value_1 = JS::js_undefined();
    if (next_item1.is_object())
        usb_product_id_property_value_1 = TRY(next_item1.as_object().get("usbProductId"_utf16_fly_string));

    if (!usb_product_id_property_value_1.is_undefined()) {

    Optional<WebIDL::UnsignedShort> usb_product_id_value_1;

    if (!usb_product_id_property_value_1.is_undefined())

    usb_product_id_value_1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, usb_product_id_property_value_1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    sequence_item1.usb_product_id = usb_product_id_value_1;

    }

    auto usb_vendor_id_property_value_2 = JS::js_undefined();
    if (next_item1.is_object())
        usb_vendor_id_property_value_2 = TRY(next_item1.as_object().get("usbVendorId"_utf16_fly_string));

    if (!usb_vendor_id_property_value_2.is_undefined()) {

    Optional<WebIDL::UnsignedShort> usb_vendor_id_value_2;

    if (!usb_vendor_id_property_value_2.is_undefined())

    usb_vendor_id_value_2 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, usb_vendor_id_property_value_2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    sequence_item1.usb_vendor_id = usb_vendor_id_value_2;

    }

    filters_value_1_non_optional.append(sequence_item1);
    }

        filters_value_1 = move(filters_value_1_non_optional);
    }

    options.filters = filters_value_1;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->request_port(options); }));

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
