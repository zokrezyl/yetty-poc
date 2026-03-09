
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
#include <LibWeb/Bindings/IDBTransactionPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBTransaction.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBDatabase.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBObjectStore.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBRequest.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBIndex.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBCursor.h>

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

GC_DEFINE_ALLOCATOR(IDBTransactionPrototype);

IDBTransactionPrototype::IDBTransactionPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

IDBTransactionPrototype::~IDBTransactionPrototype()
{
}

void IDBTransactionPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    auto native_object_store_names_getter = JS::NativeFunction::create(realm, object_store_names_getter, 0, "objectStoreNames"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_object_store_names_setter;

    define_direct_accessor("objectStoreNames"_utf16_fly_string, native_object_store_names_getter, native_object_store_names_setter, default_attributes);

    auto native_mode_getter = JS::NativeFunction::create(realm, mode_getter, 0, "mode"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_mode_setter;

    define_direct_accessor("mode"_utf16_fly_string, native_mode_getter, native_mode_setter, default_attributes);

    auto native_durability_getter = JS::NativeFunction::create(realm, durability_getter, 0, "durability"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_durability_setter;

    define_direct_accessor("durability"_utf16_fly_string, native_durability_getter, native_durability_setter, default_attributes);

    auto native_db_getter = JS::NativeFunction::create(realm, db_getter, 0, "db"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_db_setter;

    define_direct_accessor("db"_utf16_fly_string, native_db_getter, native_db_setter, default_attributes);

    auto native_error_getter = JS::NativeFunction::create(realm, error_getter, 0, "error"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_error_setter;

    define_direct_accessor("error"_utf16_fly_string, native_error_getter, native_error_setter, default_attributes);

    auto native_onabort_getter = JS::NativeFunction::create(realm, onabort_getter, 0, "onabort"_utf16_fly_string, &realm, "get"sv);

    auto native_onabort_setter = JS::NativeFunction::create(realm, onabort_setter, 1, "onabort"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onabort"_utf16_fly_string, native_onabort_getter, native_onabort_setter, default_attributes);

    auto native_oncomplete_getter = JS::NativeFunction::create(realm, oncomplete_getter, 0, "oncomplete"_utf16_fly_string, &realm, "get"sv);

    auto native_oncomplete_setter = JS::NativeFunction::create(realm, oncomplete_setter, 1, "oncomplete"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("oncomplete"_utf16_fly_string, native_oncomplete_getter, native_oncomplete_setter, default_attributes);

    auto native_onerror_getter = JS::NativeFunction::create(realm, onerror_getter, 0, "onerror"_utf16_fly_string, &realm, "get"sv);

    auto native_onerror_setter = JS::NativeFunction::create(realm, onerror_setter, 1, "onerror"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onerror"_utf16_fly_string, native_onerror_getter, native_onerror_setter, default_attributes);

    define_native_function(realm, "objectStore"_utf16_fly_string, object_store, 1, default_attributes);

    define_native_function(realm, "commit"_utf16_fly_string, commit, 0, default_attributes);

    define_native_function(realm, "abort"_utf16_fly_string, abort, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "IDBTransaction"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void IDBTransactionPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<IDBTransaction*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<IDBTransaction>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "IDBTransaction");
    return static_cast<IDBTransaction*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(IDBTransactionPrototype::object_store_names_getter)
{
    WebIDL::log_trace(vm, "IDBTransactionPrototype::object_store_names_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->object_store_names(); }));

    return &const_cast<DOMStringList&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBTransactionPrototype::mode_getter)
{
    WebIDL::log_trace(vm, "IDBTransactionPrototype::mode_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->mode(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(IDBTransactionPrototype::durability_getter)
{
    WebIDL::log_trace(vm, "IDBTransactionPrototype::durability_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->durability(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(IDBTransactionPrototype::db_getter)
{
    WebIDL::log_trace(vm, "IDBTransactionPrototype::db_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->connection(); }));

    return &const_cast<IDBDatabase&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBTransactionPrototype::error_getter)
{
    WebIDL::log_trace(vm, "IDBTransactionPrototype::error_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->error(); }));

    if (retval) {

    return &const_cast<DOMException&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(IDBTransactionPrototype::onabort_getter)
{
    WebIDL::log_trace(vm, "IDBTransactionPrototype::onabort_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onabort(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(IDBTransactionPrototype::onabort_setter)
{
    WebIDL::log_trace(vm, "IDBTransactionPrototype::onabort_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onabort(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(IDBTransactionPrototype::oncomplete_getter)
{
    WebIDL::log_trace(vm, "IDBTransactionPrototype::oncomplete_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncomplete(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(IDBTransactionPrototype::oncomplete_setter)
{
    WebIDL::log_trace(vm, "IDBTransactionPrototype::oncomplete_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_oncomplete(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(IDBTransactionPrototype::onerror_getter)
{
    WebIDL::log_trace(vm, "IDBTransactionPrototype::onerror_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onerror(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(IDBTransactionPrototype::onerror_setter)
{
    WebIDL::log_trace(vm, "IDBTransactionPrototype::onerror_setter");
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

JS_DEFINE_NATIVE_FUNCTION(IDBTransactionPrototype::object_store)
{
    WebIDL::log_trace(vm, "IDBTransactionPrototype::object_store");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "objectStore");

    auto arg0 = vm.argument(0);

    String name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->object_store(name); }));

    return &const_cast<IDBObjectStore&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBTransactionPrototype::commit)
{
    WebIDL::log_trace(vm, "IDBTransactionPrototype::commit");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->commit(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(IDBTransactionPrototype::abort)
{
    WebIDL::log_trace(vm, "IDBTransactionPrototype::abort");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->abort(); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
