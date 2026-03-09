
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
#include <LibWeb/Bindings/ReadableStreamPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/QueuingStrategy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamBYOBReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamDefaultReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStreamDefaultWriter.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#   include <LibWeb/Streams/ReadableStreamAsyncIterator.h>

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

GC_DEFINE_ALLOCATOR(ReadableStreamPrototype);

ReadableStreamPrototype::ReadableStreamPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

ReadableStreamPrototype::~ReadableStreamPrototype()
{
}

void ReadableStreamPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_locked_getter = JS::NativeFunction::create(realm, locked_getter, 0, "locked"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_locked_setter;

    define_direct_accessor("locked"_utf16_fly_string, native_locked_getter, native_locked_setter, default_attributes);

    define_native_function(realm, "cancel"_utf16_fly_string, cancel, 0, default_attributes);

    define_native_function(realm, "getReader"_utf16_fly_string, get_reader, 0, default_attributes);

    define_native_function(realm, "pipeThrough"_utf16_fly_string, pipe_through, 1, default_attributes);

    define_native_function(realm, "pipeTo"_utf16_fly_string, pipe_to, 1, default_attributes);

    define_native_function(realm, "tee"_utf16_fly_string, tee, 0, default_attributes);

    define_native_function(realm, vm.names.values, values, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_async_iterator(), get_without_side_effects(vm.names.values), JS::Attribute::Configurable | JS::Attribute::Writable);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "ReadableStream"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void ReadableStreamPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Streams::ReadableStream*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Streams::ReadableStream>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ReadableStream");
    return static_cast<Streams::ReadableStream*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(ReadableStreamPrototype::locked_getter)
{
    WebIDL::log_trace(vm, "ReadableStreamPrototype::locked_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->locked(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ReadableStreamPrototype::cancel)
{
    WebIDL::log_trace(vm, "ReadableStreamPrototype::cancel");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    JS::Value reason = JS::js_undefined();
    if (!arg0.is_undefined())
        reason = arg0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cancel(reason); }));

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

JS_DEFINE_NATIVE_FUNCTION(ReadableStreamPrototype::get_reader)
{
    WebIDL::log_trace(vm, "ReadableStreamPrototype::get_reader");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ReadableStreamGetReaderOptions");

    ReadableStreamGetReaderOptions options {};

    auto mode_property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        mode_property_value_2 = TRY(arg0.as_object().get("mode"_utf16_fly_string));

    if (!mode_property_value_2.is_undefined()) {

    ReadableStreamReaderMode mode_value_2 { ReadableStreamReaderMode::Byob };

    if (!mode_property_value_2.is_undefined()) {

    auto mode_property_value_2_string = TRY(mode_property_value_2.to_string(vm));

    if (mode_property_value_2_string == "byob"sv)
        mode_value_2 = ReadableStreamReaderMode::Byob;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, mode_property_value_2_string, "ReadableStreamReaderMode");

    }

    options.mode = mode_value_2;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_reader(options); }));

    return retval.visit(

        [&vm, &realm]([[maybe_unused]] GC::Root<ReadableStreamDefaultReader> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<ReadableStreamDefaultReader&>(*visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<ReadableStreamBYOBReader> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<ReadableStreamBYOBReader&>(*visited_union_value0);

        }

    );

}

JS_DEFINE_NATIVE_FUNCTION(ReadableStreamPrototype::pipe_through)
{
    WebIDL::log_trace(vm, "ReadableStreamPrototype::pipe_through");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "pipeThrough");

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ReadableWritablePair");

    ReadableWritablePair transform {};

    auto readable_property_value_3 = JS::js_undefined();
    if (arg0.is_object())
        readable_property_value_3 = TRY(arg0.as_object().get("readable"_utf16_fly_string));

    if (readable_property_value_3.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "readable");

    if (!readable_property_value_3.is_object() || !is<ReadableStream>(readable_property_value_3.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ReadableStream");

    auto& readable_value_3 = static_cast<ReadableStream&>(readable_property_value_3.as_object());

    transform.readable = readable_value_3;

    auto writable_property_value_4 = JS::js_undefined();
    if (arg0.is_object())
        writable_property_value_4 = TRY(arg0.as_object().get("writable"_utf16_fly_string));

    if (writable_property_value_4.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "writable");

    if (!writable_property_value_4.is_object() || !is<WritableStream>(writable_property_value_4.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WritableStream");

    auto& writable_value_4 = static_cast<WritableStream&>(writable_property_value_4.as_object());

    transform.writable = writable_value_4;

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "StreamPipeOptions");

    StreamPipeOptions options {};

    auto prevent_abort_property_value_5 = JS::js_undefined();
    if (arg1.is_object())
        prevent_abort_property_value_5 = TRY(arg1.as_object().get("preventAbort"_utf16_fly_string));

    bool prevent_abort_value_5;

    if (!prevent_abort_property_value_5.is_undefined())

    prevent_abort_value_5 = prevent_abort_property_value_5.to_boolean();

    else
        prevent_abort_value_5 = static_cast<bool>(false);

    options.prevent_abort = prevent_abort_value_5;

    auto prevent_cancel_property_value_6 = JS::js_undefined();
    if (arg1.is_object())
        prevent_cancel_property_value_6 = TRY(arg1.as_object().get("preventCancel"_utf16_fly_string));

    bool prevent_cancel_value_6;

    if (!prevent_cancel_property_value_6.is_undefined())

    prevent_cancel_value_6 = prevent_cancel_property_value_6.to_boolean();

    else
        prevent_cancel_value_6 = static_cast<bool>(false);

    options.prevent_cancel = prevent_cancel_value_6;

    auto prevent_close_property_value_7 = JS::js_undefined();
    if (arg1.is_object())
        prevent_close_property_value_7 = TRY(arg1.as_object().get("preventClose"_utf16_fly_string));

    bool prevent_close_value_7;

    if (!prevent_close_property_value_7.is_undefined())

    prevent_close_value_7 = prevent_close_property_value_7.to_boolean();

    else
        prevent_close_value_7 = static_cast<bool>(false);

    options.prevent_close = prevent_close_value_7;

    auto signal_property_value_8 = JS::js_undefined();
    if (arg1.is_object())
        signal_property_value_8 = TRY(arg1.as_object().get("signal"_utf16_fly_string));

    if (!signal_property_value_8.is_undefined()) {

    GC::Ptr<AbortSignal> signal_value_8;
    if (!signal_property_value_8.is_undefined()) {
        if (!signal_property_value_8.is_object() || !is<AbortSignal>(signal_property_value_8.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AbortSignal");

        signal_value_8 = static_cast<AbortSignal&>(signal_property_value_8.as_object());
    }

    options.signal = signal_value_8;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->pipe_through(transform, options); }));

    return &const_cast<ReadableStream&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ReadableStreamPrototype::pipe_to)
{
    WebIDL::log_trace(vm, "ReadableStreamPrototype::pipe_to");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "pipeTo");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WritableStream>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WritableStream");

    auto& destination = static_cast<WritableStream&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "StreamPipeOptions");

    StreamPipeOptions options {};

    auto prevent_abort_property_value_9 = JS::js_undefined();
    if (arg1.is_object())
        prevent_abort_property_value_9 = TRY(arg1.as_object().get("preventAbort"_utf16_fly_string));

    bool prevent_abort_value_9;

    if (!prevent_abort_property_value_9.is_undefined())

    prevent_abort_value_9 = prevent_abort_property_value_9.to_boolean();

    else
        prevent_abort_value_9 = static_cast<bool>(false);

    options.prevent_abort = prevent_abort_value_9;

    auto prevent_cancel_property_value_10 = JS::js_undefined();
    if (arg1.is_object())
        prevent_cancel_property_value_10 = TRY(arg1.as_object().get("preventCancel"_utf16_fly_string));

    bool prevent_cancel_value_10;

    if (!prevent_cancel_property_value_10.is_undefined())

    prevent_cancel_value_10 = prevent_cancel_property_value_10.to_boolean();

    else
        prevent_cancel_value_10 = static_cast<bool>(false);

    options.prevent_cancel = prevent_cancel_value_10;

    auto prevent_close_property_value_11 = JS::js_undefined();
    if (arg1.is_object())
        prevent_close_property_value_11 = TRY(arg1.as_object().get("preventClose"_utf16_fly_string));

    bool prevent_close_value_11;

    if (!prevent_close_property_value_11.is_undefined())

    prevent_close_value_11 = prevent_close_property_value_11.to_boolean();

    else
        prevent_close_value_11 = static_cast<bool>(false);

    options.prevent_close = prevent_close_value_11;

    auto signal_property_value_12 = JS::js_undefined();
    if (arg1.is_object())
        signal_property_value_12 = TRY(arg1.as_object().get("signal"_utf16_fly_string));

    if (!signal_property_value_12.is_undefined()) {

    GC::Ptr<AbortSignal> signal_value_12;
    if (!signal_property_value_12.is_undefined()) {
        if (!signal_property_value_12.is_object() || !is<AbortSignal>(signal_property_value_12.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AbortSignal");

        signal_value_12 = static_cast<AbortSignal&>(signal_property_value_12.as_object());
    }

    options.signal = signal_value_12;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->pipe_to(destination, options); }));

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

JS_DEFINE_NATIVE_FUNCTION(ReadableStreamPrototype::tee)
{
    WebIDL::log_trace(vm, "ReadableStreamPrototype::tee");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tee(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(ReadableStreamPrototype::values)
{
    WebIDL::log_trace(vm, "ReadableStreamPrototype::values");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ReadableStreamIteratorOptions");

    ReadableStreamIteratorOptions options {};

    auto prevent_cancel_property_value_13 = JS::js_undefined();
    if (arg0.is_object())
        prevent_cancel_property_value_13 = TRY(arg0.as_object().get("preventCancel"_utf16_fly_string));

    bool prevent_cancel_value_13;

    if (!prevent_cancel_property_value_13.is_undefined())

    prevent_cancel_value_13 = prevent_cancel_property_value_13.to_boolean();

    else
        prevent_cancel_value_13 = static_cast<bool>(false);

    options.prevent_cancel = prevent_cancel_value_13;

    return TRY(throw_dom_exception_if_needed(vm, [&] { return ReadableStreamAsyncIterator::create(realm, Object::PropertyKind::Value, *impl, options); }));
}

} // namespace Web::Bindings
