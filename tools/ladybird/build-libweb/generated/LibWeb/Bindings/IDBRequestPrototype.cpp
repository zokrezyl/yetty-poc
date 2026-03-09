
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
#include <LibWeb/Bindings/IDBRequestPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBRequest.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBCursor.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBIndex.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBObjectStore.h>

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

GC_DEFINE_ALLOCATOR(IDBRequestPrototype);

IDBRequestPrototype::IDBRequestPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

IDBRequestPrototype::~IDBRequestPrototype()
{
}

void IDBRequestPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    auto native_result_getter = JS::NativeFunction::create(realm, result_getter, 0, "result"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_result_setter;

    define_direct_accessor("result"_utf16_fly_string, native_result_getter, native_result_setter, default_attributes);

    auto native_error_getter = JS::NativeFunction::create(realm, error_getter, 0, "error"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_error_setter;

    define_direct_accessor("error"_utf16_fly_string, native_error_getter, native_error_setter, default_attributes);

    auto native_source_getter = JS::NativeFunction::create(realm, source_getter, 0, "source"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_source_setter;

    define_direct_accessor("source"_utf16_fly_string, native_source_getter, native_source_setter, default_attributes);

    auto native_transaction_getter = JS::NativeFunction::create(realm, transaction_getter, 0, "transaction"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_transaction_setter;

    define_direct_accessor("transaction"_utf16_fly_string, native_transaction_getter, native_transaction_setter, default_attributes);

    auto native_ready_state_getter = JS::NativeFunction::create(realm, ready_state_getter, 0, "readyState"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_ready_state_setter;

    define_direct_accessor("readyState"_utf16_fly_string, native_ready_state_getter, native_ready_state_setter, default_attributes);

    auto native_onsuccess_getter = JS::NativeFunction::create(realm, onsuccess_getter, 0, "onsuccess"_utf16_fly_string, &realm, "get"sv);

    auto native_onsuccess_setter = JS::NativeFunction::create(realm, onsuccess_setter, 1, "onsuccess"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onsuccess"_utf16_fly_string, native_onsuccess_getter, native_onsuccess_setter, default_attributes);

    auto native_onerror_getter = JS::NativeFunction::create(realm, onerror_getter, 0, "onerror"_utf16_fly_string, &realm, "get"sv);

    auto native_onerror_setter = JS::NativeFunction::create(realm, onerror_setter, 1, "onerror"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onerror"_utf16_fly_string, native_onerror_getter, native_onerror_setter, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "IDBRequest"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void IDBRequestPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<IDBRequest*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<IDBRequest>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "IDBRequest");
    return static_cast<IDBRequest*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(IDBRequestPrototype::result_getter)
{
    WebIDL::log_trace(vm, "IDBRequestPrototype::result_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->result(); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(IDBRequestPrototype::error_getter)
{
    WebIDL::log_trace(vm, "IDBRequestPrototype::error_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->error(); }));

    if (retval) {

    return &const_cast<DOMException&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(IDBRequestPrototype::source_getter)
{
    WebIDL::log_trace(vm, "IDBRequestPrototype::source_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->source(); }));

    return retval.visit(

        [&vm, &realm]([[maybe_unused]] GC::Root<IDBObjectStore> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<IDBObjectStore&>(*visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<IDBIndex> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<IDBIndex&>(*visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<IDBCursor> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<IDBCursor&>(*visited_union_value0);

        },

        [](Empty) -> JS::Value {
            return JS::js_null();
        }

    );

}

JS_DEFINE_NATIVE_FUNCTION(IDBRequestPrototype::transaction_getter)
{
    WebIDL::log_trace(vm, "IDBRequestPrototype::transaction_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->transaction(); }));

    if (retval) {

    return &const_cast<IDBTransaction&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(IDBRequestPrototype::ready_state_getter)
{
    WebIDL::log_trace(vm, "IDBRequestPrototype::ready_state_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ready_state(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(IDBRequestPrototype::onsuccess_getter)
{
    WebIDL::log_trace(vm, "IDBRequestPrototype::onsuccess_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onsuccess(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(IDBRequestPrototype::onsuccess_setter)
{
    WebIDL::log_trace(vm, "IDBRequestPrototype::onsuccess_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onsuccess(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(IDBRequestPrototype::onerror_getter)
{
    WebIDL::log_trace(vm, "IDBRequestPrototype::onerror_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onerror(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(IDBRequestPrototype::onerror_setter)
{
    WebIDL::log_trace(vm, "IDBRequestPrototype::onerror_setter");
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

} // namespace Web::Bindings
