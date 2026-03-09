
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
#include <LibWeb/Bindings/WorkerPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Worker.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/MessagePort.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Request.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedScriptURL.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Headers.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XHR/FormData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOMURL/URLSearchParams.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/QueuingStrategy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamBYOBReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamDefaultReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/File.h>

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

GC_DEFINE_ALLOCATOR(WorkerPrototype);

WorkerPrototype::WorkerPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

WorkerPrototype::~WorkerPrototype()
{
}

void WorkerPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    auto native_onmessage_getter = JS::NativeFunction::create(realm, onmessage_getter, 0, "onmessage"_utf16_fly_string, &realm, "get"sv);

    auto native_onmessage_setter = JS::NativeFunction::create(realm, onmessage_setter, 1, "onmessage"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onmessage"_utf16_fly_string, native_onmessage_getter, native_onmessage_setter, default_attributes);

    auto native_onmessageerror_getter = JS::NativeFunction::create(realm, onmessageerror_getter, 0, "onmessageerror"_utf16_fly_string, &realm, "get"sv);

    auto native_onmessageerror_setter = JS::NativeFunction::create(realm, onmessageerror_setter, 1, "onmessageerror"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onmessageerror"_utf16_fly_string, native_onmessageerror_getter, native_onmessageerror_setter, default_attributes);

    auto native_onerror_getter = JS::NativeFunction::create(realm, onerror_getter, 0, "onerror"_utf16_fly_string, &realm, "get"sv);

    auto native_onerror_setter = JS::NativeFunction::create(realm, onerror_setter, 1, "onerror"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onerror"_utf16_fly_string, native_onerror_getter, native_onerror_setter, default_attributes);

    define_native_function(realm, "terminate"_utf16_fly_string, terminate, 0, default_attributes);

    define_native_function(realm, "postMessage"_utf16_fly_string, post_message, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Worker"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void WorkerPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::Worker*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::Worker>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Worker");
    return static_cast<HTML::Worker*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(WorkerPrototype::onmessage_getter)
{
    WebIDL::log_trace(vm, "WorkerPrototype::onmessage_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmessage(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WorkerPrototype::onmessage_setter)
{
    WebIDL::log_trace(vm, "WorkerPrototype::onmessage_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onmessage(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WorkerPrototype::onmessageerror_getter)
{
    WebIDL::log_trace(vm, "WorkerPrototype::onmessageerror_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmessageerror(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WorkerPrototype::onmessageerror_setter)
{
    WebIDL::log_trace(vm, "WorkerPrototype::onmessageerror_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onmessageerror(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WorkerPrototype::onerror_getter)
{
    WebIDL::log_trace(vm, "WorkerPrototype::onerror_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onerror(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WorkerPrototype::onerror_setter)
{
    WebIDL::log_trace(vm, "WorkerPrototype::onerror_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onerror(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WorkerPrototype::terminate)
{
    WebIDL::log_trace(vm, "WorkerPrototype::terminate");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->terminate(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WorkerPrototype::post_message0)
{
    WebIDL::log_trace(vm, "WorkerPrototype::post_message0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    auto message = arg0;

    auto arg1 = vm.argument(1);

    if (!arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg1);

    auto arg1_iterator_method0 = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg1_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg1);

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *arg1_iterator_method0));

    Vector<GC::Root<JS::Object>> transfer;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, next_item1);
    auto sequence_item1 = GC::make_root(next_item1.as_object());

    transfer.append(sequence_item1);
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->post_message(message, transfer); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WorkerPrototype::post_message1)
{
    WebIDL::log_trace(vm, "WorkerPrototype::post_message1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    auto message = arg0;

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "StructuredSerializeOptions");

    StructuredSerializeOptions options {};

    auto transfer_property_value_3 = JS::js_undefined();
    if (arg1.is_object())
        transfer_property_value_3 = TRY(arg1.as_object().get("transfer"_utf16_fly_string));

    Vector<GC::Root<JS::Object>> transfer_value_3;

    if (!transfer_property_value_3.is_undefined()) {

    if (!transfer_property_value_3.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, transfer_property_value_3);

    auto transfer_property_value_3_iterator_method0 = TRY(transfer_property_value_3.get_method(vm, vm.well_known_symbol_iterator()));
    if (!transfer_property_value_3_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, transfer_property_value_3);

    auto transfer_property_value_3_iterator1 = TRY(JS::get_iterator_from_method(vm, transfer_property_value_3, *transfer_property_value_3_iterator_method0));

    Vector<GC::Root<JS::Object>> transfer_value_3_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, transfer_property_value_3_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, next_item1);
    auto sequence_item1 = GC::make_root(next_item1.as_object());

    transfer_value_3_non_optional.append(sequence_item1);
    }

        transfer_value_3 = move(transfer_value_3_non_optional);
    }

    options.transfer = transfer_value_3;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->post_message(message, options); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WorkerPrototype::post_message)
{
    WebIDL::log_trace(vm, "WorkerPrototype::post_message");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(2, vm.argument_count())) {

    case 1:
        chosen_overload_callable_id = 1;
        break;


    case 2: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("any", false), make_ref_counted<IDL::ParameterizedType>("sequence", false, Vector<NonnullRefPtr<IDL::Type const>> {make_ref_counted<IDL::Type>("object", false)})}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Required});
        overloads.empend(1, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("any", false), make_ref_counted<IDL::Type>("StructuredSerializeOptions", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Optional});

        effective_overload_set.emplace(move(overloads), 1);
        break;
    }

    }

    Vector<StringView> dictionary_types {
    "StructuredSerializeOptions"sv,
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return post_message0(vm);

    case 1:

        return post_message1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

} // namespace Web::Bindings
