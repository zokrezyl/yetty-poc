
#include <AK/Function.h>
#include <LibIDL/Types.h>
#include <LibJS/Runtime/ArrayBuffer.h>
#include <LibJS/Runtime/DataView.h>
#include <LibJS/Runtime/Error.h>
#include <LibJS/Runtime/PrimitiveString.h>
#include <LibJS/Runtime/PromiseConstructor.h>
#include <LibJS/Runtime/TypedArray.h>
#include <LibJS/Runtime/Value.h>
#include <LibJS/Runtime/ValueInlines.h>
#include <LibWeb/Bindings/WebAssemblyNamespace.h>
#include <LibWeb/Bindings/ExceptionOrUtils.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/HTML/Window.h>
#include <LibWeb/HTML/WindowProxy.h>
#include <LibWeb/WebIDL/AbstractOperations.h>
#include <LibWeb/WebIDL/Buffers.h>
#include <LibWeb/WebIDL/OverloadResolution.h>
#include <LibWeb/WebIDL/Promise.h>
#include <LibWeb/WebIDL/Tracing.h>
#include <LibWeb/WebIDL/Types.h>


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAssembly/WebAssembly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAssembly/Instance.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAssembly/Module.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Response.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Headers.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XHR/FormData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOMURL/URLSearchParams.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/QueuingStrategy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamBYOBReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamDefaultReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/File.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStreamDefaultWriter.h>

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

GC_DEFINE_ALLOCATOR(WebAssemblyNamespace);

WebAssemblyNamespace::WebAssemblyNamespace(JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())
{
}

WebAssemblyNamespace::~WebAssemblyNamespace()
{
}

void WebAssemblyNamespace::initialize(JS::Realm& realm)
{
    [[maybe_unused]] auto& vm = this->vm();

    Base::initialize(realm);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "WebAssembly"_string), JS::Attribute::Configurable);


    define_native_function(realm, "validate"_utf16_fly_string, validate, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "compile"_utf16_fly_string, compile, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "compileStreaming"_utf16_fly_string, compile_streaming, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "instantiate"_utf16_fly_string, instantiate, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "instantiateStreaming"_utf16_fly_string, instantiate_streaming, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);


    WebAssembly::initialize(*this, realm);

}

void WebAssemblyNamespace::visit_edges(JS::Cell::Visitor& visitor)
{
    Base::visit_edges(visitor);
    WebAssembly::visit_edges(*this, visitor);
}

void WebAssemblyNamespace::finalize()
{
    Base::finalize();
    WebAssembly::finalize(*this);
}

JS_DEFINE_NATIVE_FUNCTION(WebAssemblyNamespace::validate)
{
    WebIDL::log_trace(vm, "WebAssemblyNamespace::validate");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "validate");

    auto arg0 = vm.argument(0);

    GC::Root<WebIDL::BufferSource> bytes;

    if (!arg0.is_object() || !(is<JS::TypedArrayBase>(arg0.as_object()) || is<JS::ArrayBuffer>(arg0.as_object()) || is<JS::DataView>(arg0.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BufferSource");

    bytes = GC::make_root(realm.create<WebIDL::BufferSource>(arg0.as_object()));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return WebAssembly::validate(vm, bytes); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WebAssemblyNamespace::compile)
{
    WebIDL::log_trace(vm, "WebAssemblyNamespace::compile");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "compile");

    auto arg0 = vm.argument(0);

    GC::Root<WebIDL::BufferSource> bytes;

    if (!arg0.is_object() || !(is<JS::TypedArrayBase>(arg0.as_object()) || is<JS::ArrayBuffer>(arg0.as_object()) || is<JS::DataView>(arg0.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BufferSource");

    bytes = GC::make_root(realm.create<WebIDL::BufferSource>(arg0.as_object()));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return WebAssembly::compile(vm, bytes); }));

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

JS_DEFINE_NATIVE_FUNCTION(WebAssemblyNamespace::compile_streaming)
{
    WebIDL::log_trace(vm, "WebAssemblyNamespace::compile_streaming");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "compileStreaming");

    auto arg0 = vm.argument(0);

    // 1. Let promiseCapability be ? NewPromiseCapability(%Promise%).
    auto promise_capability = TRY(JS::new_promise_capability(vm, realm.intrinsics().promise_constructor()));
    // 2. Perform ? Call(promiseCapability.[[Resolve]], undefined, « V »).
    TRY(JS::call(vm, *promise_capability->resolve(), JS::js_undefined(), arg0));
    // 3. Return promiseCapability.
    auto source = GC::make_root(promise_capability);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return WebAssembly::compile_streaming(vm, source); }));

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

JS_DEFINE_NATIVE_FUNCTION(WebAssemblyNamespace::instantiate0)
{
    WebIDL::log_trace(vm, "WebAssemblyNamespace::instantiate0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto arg0 = vm.argument(0);

    GC::Root<WebIDL::BufferSource> bytes;

    if (!arg0.is_object() || !(is<JS::TypedArrayBase>(arg0.as_object()) || is<JS::ArrayBuffer>(arg0.as_object()) || is<JS::DataView>(arg0.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BufferSource");

    bytes = GC::make_root(realm.create<WebIDL::BufferSource>(arg0.as_object()));

    auto arg1 = vm.argument(1);

    Optional<GC::Root<JS::Object>> import_object;
    if (!arg1.is_undefined()) {
        if (!arg1.is_object())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg1);
        import_object = GC::make_root(arg1.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return WebAssembly::instantiate(vm, bytes, import_object); }));

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

JS_DEFINE_NATIVE_FUNCTION(WebAssemblyNamespace::instantiate_streaming)
{
    WebIDL::log_trace(vm, "WebAssemblyNamespace::instantiate_streaming");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "instantiateStreaming");

    auto arg0 = vm.argument(0);

    // 1. Let promiseCapability be ? NewPromiseCapability(%Promise%).
    auto promise_capability = TRY(JS::new_promise_capability(vm, realm.intrinsics().promise_constructor()));
    // 2. Perform ? Call(promiseCapability.[[Resolve]], undefined, « V »).
    TRY(JS::call(vm, *promise_capability->resolve(), JS::js_undefined(), arg0));
    // 3. Return promiseCapability.
    auto source = GC::make_root(promise_capability);

    auto arg1 = vm.argument(1);

    Optional<GC::Root<JS::Object>> import_object;
    if (!arg1.is_undefined()) {
        if (!arg1.is_object())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg1);
        import_object = GC::make_root(arg1.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return WebAssembly::instantiate_streaming(vm, source, import_object); }));

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

JS_DEFINE_NATIVE_FUNCTION(WebAssemblyNamespace::instantiate1)
{
    WebIDL::log_trace(vm, "WebAssemblyNamespace::instantiate1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Module>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Module");

    auto& module_object = static_cast<Module&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    Optional<GC::Root<JS::Object>> import_object;
    if (!arg1.is_undefined()) {
        if (!arg1.is_object())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg1);
        import_object = GC::make_root(arg1.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return WebAssembly::instantiate(vm, module_object, import_object); }));

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

JS_DEFINE_NATIVE_FUNCTION(WebAssemblyNamespace::instantiate)
{
    WebIDL::log_trace(vm, "WebAssemblyNamespace::instantiate");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(2, vm.argument_count())) {

    case 1: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("BufferSource", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required});
        overloads.empend(1, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("Module", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required});

        effective_overload_set.emplace(move(overloads), 0);
        break;
    }

    case 2: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("BufferSource", false), make_ref_counted<IDL::Type>("object", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Optional});
        overloads.empend(1, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("Module", false), make_ref_counted<IDL::Type>("object", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Optional});

        effective_overload_set.emplace(move(overloads), 0);
        break;
    }

    }

    Vector<StringView> dictionary_types {
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return instantiate0(vm);

    case 1:

        return instantiate1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

} // namespace Web::Bindings
