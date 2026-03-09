
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
#include <LibWeb/Bindings/IDBDatabasePrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBDatabase.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBTransaction.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBObjectStore.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBRequest.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBIndex.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBCursor.h>

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

GC_DEFINE_ALLOCATOR(IDBDatabasePrototype);

IDBDatabasePrototype::IDBDatabasePrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

IDBDatabasePrototype::~IDBDatabasePrototype()
{
}

void IDBDatabasePrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    auto native_name_getter = JS::NativeFunction::create(realm, name_getter, 0, "name"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_name_setter;

    define_direct_accessor("name"_utf16_fly_string, native_name_getter, native_name_setter, default_attributes);

    auto native_version_getter = JS::NativeFunction::create(realm, version_getter, 0, "version"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_version_setter;

    define_direct_accessor("version"_utf16_fly_string, native_version_getter, native_version_setter, default_attributes);

    auto native_object_store_names_getter = JS::NativeFunction::create(realm, object_store_names_getter, 0, "objectStoreNames"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_object_store_names_setter;

    define_direct_accessor("objectStoreNames"_utf16_fly_string, native_object_store_names_getter, native_object_store_names_setter, default_attributes);

    auto native_onabort_getter = JS::NativeFunction::create(realm, onabort_getter, 0, "onabort"_utf16_fly_string, &realm, "get"sv);

    auto native_onabort_setter = JS::NativeFunction::create(realm, onabort_setter, 1, "onabort"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onabort"_utf16_fly_string, native_onabort_getter, native_onabort_setter, default_attributes);

    auto native_onclose_getter = JS::NativeFunction::create(realm, onclose_getter, 0, "onclose"_utf16_fly_string, &realm, "get"sv);

    auto native_onclose_setter = JS::NativeFunction::create(realm, onclose_setter, 1, "onclose"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onclose"_utf16_fly_string, native_onclose_getter, native_onclose_setter, default_attributes);

    auto native_onerror_getter = JS::NativeFunction::create(realm, onerror_getter, 0, "onerror"_utf16_fly_string, &realm, "get"sv);

    auto native_onerror_setter = JS::NativeFunction::create(realm, onerror_setter, 1, "onerror"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onerror"_utf16_fly_string, native_onerror_getter, native_onerror_setter, default_attributes);

    auto native_onversionchange_getter = JS::NativeFunction::create(realm, onversionchange_getter, 0, "onversionchange"_utf16_fly_string, &realm, "get"sv);

    auto native_onversionchange_setter = JS::NativeFunction::create(realm, onversionchange_setter, 1, "onversionchange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onversionchange"_utf16_fly_string, native_onversionchange_getter, native_onversionchange_setter, default_attributes);

    define_native_function(realm, "transaction"_utf16_fly_string, transaction, 1, default_attributes);

    define_native_function(realm, "close"_utf16_fly_string, close, 0, default_attributes);

    define_native_function(realm, "createObjectStore"_utf16_fly_string, create_object_store, 1, default_attributes);

    define_native_function(realm, "deleteObjectStore"_utf16_fly_string, delete_object_store, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "IDBDatabase"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void IDBDatabasePrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<IDBDatabase*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<IDBDatabase>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "IDBDatabase");
    return static_cast<IDBDatabase*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(IDBDatabasePrototype::name_getter)
{
    WebIDL::log_trace(vm, "IDBDatabasePrototype::name_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->name(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBDatabasePrototype::version_getter)
{
    WebIDL::log_trace(vm, "IDBDatabasePrototype::version_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->version(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(IDBDatabasePrototype::object_store_names_getter)
{
    WebIDL::log_trace(vm, "IDBDatabasePrototype::object_store_names_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->object_store_names(); }));

    return &const_cast<DOMStringList&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBDatabasePrototype::onabort_getter)
{
    WebIDL::log_trace(vm, "IDBDatabasePrototype::onabort_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onabort(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(IDBDatabasePrototype::onabort_setter)
{
    WebIDL::log_trace(vm, "IDBDatabasePrototype::onabort_setter");
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

JS_DEFINE_NATIVE_FUNCTION(IDBDatabasePrototype::onclose_getter)
{
    WebIDL::log_trace(vm, "IDBDatabasePrototype::onclose_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onclose(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(IDBDatabasePrototype::onclose_setter)
{
    WebIDL::log_trace(vm, "IDBDatabasePrototype::onclose_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onclose(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(IDBDatabasePrototype::onerror_getter)
{
    WebIDL::log_trace(vm, "IDBDatabasePrototype::onerror_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onerror(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(IDBDatabasePrototype::onerror_setter)
{
    WebIDL::log_trace(vm, "IDBDatabasePrototype::onerror_setter");
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

JS_DEFINE_NATIVE_FUNCTION(IDBDatabasePrototype::onversionchange_getter)
{
    WebIDL::log_trace(vm, "IDBDatabasePrototype::onversionchange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onversionchange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(IDBDatabasePrototype::onversionchange_setter)
{
    WebIDL::log_trace(vm, "IDBDatabasePrototype::onversionchange_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onversionchange(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(IDBDatabasePrototype::transaction)
{
    WebIDL::log_trace(vm, "IDBDatabasePrototype::transaction");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "transaction");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<String, Vector<String>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

        auto method = TRY(arg0.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg0_iterator1 = TRY(JS::get_iterator_from_method(vm, arg0, *method));

    Vector<String> store_names;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg0_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    String sequence_item1;
    if (!false || !next_item1.is_null()) {
        sequence_item1 = TRY(WebIDL::to_string(vm, next_item1));
    }

    store_names.append(sequence_item1);
    }


            return store_names;
        }

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<String, Vector<String>> store_names = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    IDBTransactionMode mode { IDBTransactionMode::Readonly };

    if (!arg1.is_undefined()) {

    auto arg1_string = TRY(arg1.to_string(vm));

    if (arg1_string == "readonly"sv)
        mode = IDBTransactionMode::Readonly;

    else if (arg1_string == "readwrite"sv)
        mode = IDBTransactionMode::Readwrite;

    else if (arg1_string == "versionchange"sv)
        mode = IDBTransactionMode::Versionchange;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, arg1_string, "IDBTransactionMode");

    }

    auto arg2 = vm.argument(2);

    if (!arg2.is_nullish() && !arg2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "IDBTransactionOptions");

    IDBTransactionOptions options {};

    auto durability_property_value_0 = JS::js_undefined();
    if (arg2.is_object())
        durability_property_value_0 = TRY(arg2.as_object().get("durability"_utf16_fly_string));

    IDBTransactionDurability durability_value_0 { IDBTransactionDurability::Default };

    if (!durability_property_value_0.is_undefined()) {

    auto durability_property_value_0_string = TRY(durability_property_value_0.to_string(vm));

    if (durability_property_value_0_string == "default"sv)
        durability_value_0 = IDBTransactionDurability::Default;

    else if (durability_property_value_0_string == "strict"sv)
        durability_value_0 = IDBTransactionDurability::Strict;

    else if (durability_property_value_0_string == "relaxed"sv)
        durability_value_0 = IDBTransactionDurability::Relaxed;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, durability_property_value_0_string, "IDBTransactionDurability");

    }

    options.durability = durability_value_0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->transaction(store_names, mode, options); }));

    return &const_cast<IDBTransaction&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBDatabasePrototype::close)
{
    WebIDL::log_trace(vm, "IDBDatabasePrototype::close");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->close(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(IDBDatabasePrototype::create_object_store)
{
    WebIDL::log_trace(vm, "IDBDatabasePrototype::create_object_store");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "createObjectStore");

    auto arg0 = vm.argument(0);

    String name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "IDBObjectStoreParameters");

    IDBObjectStoreParameters options {};

    auto auto_increment_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        auto_increment_property_value_1 = TRY(arg1.as_object().get("autoIncrement"_utf16_fly_string));

    bool auto_increment_value_1;

    if (!auto_increment_property_value_1.is_undefined())

    auto_increment_value_1 = auto_increment_property_value_1.to_boolean();

    else
        auto_increment_value_1 = static_cast<bool>(false);

    options.auto_increment = auto_increment_value_1;

    auto key_path_property_value_2 = JS::js_undefined();
    if (arg1.is_object())
        key_path_property_value_2 = TRY(arg1.as_object().get("keyPath"_utf16_fly_string));

    auto key_path_property_value_2_to_variant = [&vm, &realm](JS::Value key_path_property_value_2) -> JS::ThrowCompletionOr<Variant<String, Vector<String>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (key_path_property_value_2.is_object()) {
            [[maybe_unused]] auto& key_path_property_value_2_object = key_path_property_value_2.as_object();

        auto method = TRY(key_path_property_value_2.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto key_path_property_value_2_iterator1 = TRY(JS::get_iterator_from_method(vm, key_path_property_value_2, *method));

    Vector<String> key_path_value_2;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, key_path_property_value_2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    String sequence_item1;
    if (!false || !next_item1.is_null()) {
        sequence_item1 = TRY(WebIDL::to_string(vm, next_item1));
    }

    key_path_value_2.append(sequence_item1);
    }


            return key_path_value_2;
        }

        }

    String key_path_property_value_2_string;
    if (!false || !key_path_property_value_2.is_null()) {
        key_path_property_value_2_string = TRY(WebIDL::to_string(vm, key_path_property_value_2));
    }

        return { key_path_property_value_2_string };

    };

    Optional<Variant<String, Vector<String>>> key_path_value_2;
    if (!key_path_property_value_2.is_nullish())
        key_path_value_2 = TRY(key_path_property_value_2_to_variant(key_path_property_value_2));

    options.key_path = key_path_value_2;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_object_store(name, options); }));

    return &const_cast<IDBObjectStore&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBDatabasePrototype::delete_object_store)
{
    WebIDL::log_trace(vm, "IDBDatabasePrototype::delete_object_store");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "deleteObjectStore");

    auto arg0 = vm.argument(0);

    String name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_object_store(name); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
