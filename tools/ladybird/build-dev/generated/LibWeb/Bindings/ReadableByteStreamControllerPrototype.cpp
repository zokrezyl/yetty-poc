
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
#include <LibWeb/Bindings/ReadableByteStreamControllerPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableByteStreamController.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamBYOBRequest.h>

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

GC_DEFINE_ALLOCATOR(ReadableByteStreamControllerPrototype);

ReadableByteStreamControllerPrototype::ReadableByteStreamControllerPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

ReadableByteStreamControllerPrototype::~ReadableByteStreamControllerPrototype()
{
}

void ReadableByteStreamControllerPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_byob_request_getter = JS::NativeFunction::create(realm, byob_request_getter, 0, "byobRequest"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_byob_request_setter;

    define_direct_accessor("byobRequest"_utf16_fly_string, native_byob_request_getter, native_byob_request_setter, default_attributes);

    auto native_desired_size_getter = JS::NativeFunction::create(realm, desired_size_getter, 0, "desiredSize"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_desired_size_setter;

    define_direct_accessor("desiredSize"_utf16_fly_string, native_desired_size_getter, native_desired_size_setter, default_attributes);

    define_native_function(realm, "close"_utf16_fly_string, close, 0, default_attributes);

    define_native_function(realm, "error"_utf16_fly_string, error, 0, default_attributes);

    define_native_function(realm, "enqueue"_utf16_fly_string, enqueue, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "ReadableByteStreamController"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void ReadableByteStreamControllerPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Streams::ReadableByteStreamController*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Streams::ReadableByteStreamController>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ReadableByteStreamController");
    return static_cast<Streams::ReadableByteStreamController*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(ReadableByteStreamControllerPrototype::byob_request_getter)
{
    WebIDL::log_trace(vm, "ReadableByteStreamControllerPrototype::byob_request_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->byob_request(); }));

    if (retval) {

    return &const_cast<ReadableStreamBYOBRequest&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ReadableByteStreamControllerPrototype::desired_size_getter)
{
    WebIDL::log_trace(vm, "ReadableByteStreamControllerPrototype::desired_size_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->desired_size(); }));

    if (retval.has_value()) {

    return JS::Value(retval.release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ReadableByteStreamControllerPrototype::close)
{
    WebIDL::log_trace(vm, "ReadableByteStreamControllerPrototype::close");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->close(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ReadableByteStreamControllerPrototype::error)
{
    WebIDL::log_trace(vm, "ReadableByteStreamControllerPrototype::error");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    JS::Value e = JS::js_undefined();
    if (!arg0.is_undefined())
        e = arg0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->error(e); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ReadableByteStreamControllerPrototype::enqueue)
{
    WebIDL::log_trace(vm, "ReadableByteStreamControllerPrototype::enqueue");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "enqueue");

    auto arg0 = vm.argument(0);

    GC::Root<WebIDL::ArrayBufferView> chunk;

        if (!arg0.is_object() || !(is<JS::TypedArrayBase>(arg0.as_object()) || is<JS::DataView>(arg0.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        chunk = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg0.as_object()));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->enqueue(chunk); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
