
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
#include <LibWeb/Bindings/IDBObjectStorePrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBObjectStore.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBRequest.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBIndex.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBCursor.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

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

GC_DEFINE_ALLOCATOR(IDBObjectStorePrototype);

IDBObjectStorePrototype::IDBObjectStorePrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

IDBObjectStorePrototype::~IDBObjectStorePrototype()
{
}

void IDBObjectStorePrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_name_getter = JS::NativeFunction::create(realm, name_getter, 0, "name"_utf16_fly_string, &realm, "get"sv);

    auto native_name_setter = JS::NativeFunction::create(realm, name_setter, 1, "name"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("name"_utf16_fly_string, native_name_getter, native_name_setter, default_attributes);

    auto native_key_path_getter = JS::NativeFunction::create(realm, key_path_getter, 0, "keyPath"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_key_path_setter;

    define_direct_accessor("keyPath"_utf16_fly_string, native_key_path_getter, native_key_path_setter, default_attributes);

    auto native_index_names_getter = JS::NativeFunction::create(realm, index_names_getter, 0, "indexNames"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_index_names_setter;

    define_direct_accessor("indexNames"_utf16_fly_string, native_index_names_getter, native_index_names_setter, default_attributes);

    auto native_transaction_getter = JS::NativeFunction::create(realm, transaction_getter, 0, "transaction"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_transaction_setter;

    define_direct_accessor("transaction"_utf16_fly_string, native_transaction_getter, native_transaction_setter, default_attributes);

    auto native_auto_increment_getter = JS::NativeFunction::create(realm, auto_increment_getter, 0, "autoIncrement"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_auto_increment_setter;

    define_direct_accessor("autoIncrement"_utf16_fly_string, native_auto_increment_getter, native_auto_increment_setter, default_attributes);

    define_native_function(realm, "put"_utf16_fly_string, put, 1, default_attributes);

    define_native_function(realm, "add"_utf16_fly_string, add, 1, default_attributes);

    define_native_function(realm, "delete"_utf16_fly_string, delete_, 1, default_attributes);

    define_native_function(realm, "clear"_utf16_fly_string, clear, 0, default_attributes);

    define_native_function(realm, "get"_utf16_fly_string, get, 1, default_attributes);

    define_native_function(realm, "getKey"_utf16_fly_string, get_key, 1, default_attributes);

    define_native_function(realm, "getAll"_utf16_fly_string, get_all, 0, default_attributes);

    define_native_function(realm, "getAllKeys"_utf16_fly_string, get_all_keys, 0, default_attributes);

    define_native_function(realm, "getAllRecords"_utf16_fly_string, get_all_records, 0, default_attributes);

    define_native_function(realm, "count"_utf16_fly_string, count, 0, default_attributes);

    define_native_function(realm, "openCursor"_utf16_fly_string, open_cursor, 0, default_attributes);

    define_native_function(realm, "openKeyCursor"_utf16_fly_string, open_key_cursor, 0, default_attributes);

    define_native_function(realm, "index"_utf16_fly_string, index, 1, default_attributes);

    define_native_function(realm, "createIndex"_utf16_fly_string, create_index, 2, default_attributes);

    define_native_function(realm, "deleteIndex"_utf16_fly_string, delete_index, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "IDBObjectStore"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void IDBObjectStorePrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<IDBObjectStore*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<IDBObjectStore>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "IDBObjectStore");
    return static_cast<IDBObjectStore*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::name_getter)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::name_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->name(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::name_setter)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::name_setter");
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

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_name(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::key_path_getter)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::key_path_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->key_path(); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::index_names_getter)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::index_names_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->index_names(); }));

    return &const_cast<DOMStringList&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::transaction_getter)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::transaction_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->transaction(); }));

    return &const_cast<IDBTransaction&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::auto_increment_getter)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::auto_increment_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->auto_increment(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::put)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::put");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "put");

    auto arg0 = vm.argument(0);

    auto value = arg0;

    auto arg1 = vm.argument(1);

    JS::Value key = JS::js_undefined();
    if (!arg1.is_undefined())
        key = arg1;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->put(value, key); }));

    return &const_cast<IDBRequest&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::add)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::add");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "add");

    auto arg0 = vm.argument(0);

    auto value = arg0;

    auto arg1 = vm.argument(1);

    JS::Value key = JS::js_undefined();
    if (!arg1.is_undefined())
        key = arg1;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->add(value, key); }));

    return &const_cast<IDBRequest&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::delete_)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::delete_");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "delete");

    auto arg0 = vm.argument(0);

    auto query = arg0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_(query); }));

    return &const_cast<IDBRequest&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::clear)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::clear");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear(); }));

    return &const_cast<IDBRequest&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::get)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::get");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "get");

    auto arg0 = vm.argument(0);

    auto query = arg0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get(query); }));

    return &const_cast<IDBRequest&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::get_key)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::get_key");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getKey");

    auto arg0 = vm.argument(0);

    auto query = arg0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_key(query); }));

    return &const_cast<IDBRequest&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::get_all)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::get_all");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    JS::Value query = JS::js_undefined();
    if (!arg0.is_undefined())
        query = arg0;

    auto arg1 = vm.argument(1);

    Optional<WebIDL::UnsignedLong> count;

    if (!arg1.is_undefined())

    count = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_all(query, count); }));

    return &const_cast<IDBRequest&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::get_all_keys)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::get_all_keys");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    JS::Value query = JS::js_undefined();
    if (!arg0.is_undefined())
        query = arg0;

    auto arg1 = vm.argument(1);

    Optional<WebIDL::UnsignedLong> count;

    if (!arg1.is_undefined())

    count = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_all_keys(query, count); }));

    return &const_cast<IDBRequest&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::get_all_records)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::get_all_records");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "IDBGetAllOptions");

    IDBGetAllOptions options {};

    auto count_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        count_property_value_0 = TRY(arg0.as_object().get("count"_utf16_fly_string));

    if (!count_property_value_0.is_undefined()) {

    Optional<WebIDL::UnsignedLong> count_value_0;

    if (!count_property_value_0.is_undefined())

    count_value_0 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, count_property_value_0, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    options.count = count_value_0;

    }

    auto direction_property_value_1 = JS::js_undefined();
    if (arg0.is_object())
        direction_property_value_1 = TRY(arg0.as_object().get("direction"_utf16_fly_string));

    IDBCursorDirection direction_value_1 { IDBCursorDirection::Next };

    if (!direction_property_value_1.is_undefined()) {

    auto direction_property_value_1_string = TRY(direction_property_value_1.to_string(vm));

    if (direction_property_value_1_string == "next"sv)
        direction_value_1 = IDBCursorDirection::Next;

    else if (direction_property_value_1_string == "nextunique"sv)
        direction_value_1 = IDBCursorDirection::Nextunique;

    else if (direction_property_value_1_string == "prev"sv)
        direction_value_1 = IDBCursorDirection::Prev;

    else if (direction_property_value_1_string == "prevunique"sv)
        direction_value_1 = IDBCursorDirection::Prevunique;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, direction_property_value_1_string, "IDBCursorDirection");

    }

    options.direction = direction_value_1;

    auto query_property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        query_property_value_2 = TRY(arg0.as_object().get("query"_utf16_fly_string));

    JS::Value query_value_2 = JS::js_undefined();
    if (!query_property_value_2.is_undefined())
        query_value_2 = query_property_value_2;

    else
        query_value_2 = JS::js_null();

    options.query = query_value_2;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_all_records(options); }));

    return &const_cast<IDBRequest&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::count)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::count");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    JS::Value query = JS::js_undefined();
    if (!arg0.is_undefined())
        query = arg0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->count(query); }));

    return &const_cast<IDBRequest&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::open_cursor)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::open_cursor");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    JS::Value query = JS::js_undefined();
    if (!arg0.is_undefined())
        query = arg0;

    auto arg1 = vm.argument(1);

    IDBCursorDirection direction { IDBCursorDirection::Next };

    if (!arg1.is_undefined()) {

    auto arg1_string = TRY(arg1.to_string(vm));

    if (arg1_string == "next"sv)
        direction = IDBCursorDirection::Next;

    else if (arg1_string == "nextunique"sv)
        direction = IDBCursorDirection::Nextunique;

    else if (arg1_string == "prev"sv)
        direction = IDBCursorDirection::Prev;

    else if (arg1_string == "prevunique"sv)
        direction = IDBCursorDirection::Prevunique;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, arg1_string, "IDBCursorDirection");

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->open_cursor(query, direction); }));

    return &const_cast<IDBRequest&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::open_key_cursor)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::open_key_cursor");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    JS::Value query = JS::js_undefined();
    if (!arg0.is_undefined())
        query = arg0;

    auto arg1 = vm.argument(1);

    IDBCursorDirection direction { IDBCursorDirection::Next };

    if (!arg1.is_undefined()) {

    auto arg1_string = TRY(arg1.to_string(vm));

    if (arg1_string == "next"sv)
        direction = IDBCursorDirection::Next;

    else if (arg1_string == "nextunique"sv)
        direction = IDBCursorDirection::Nextunique;

    else if (arg1_string == "prev"sv)
        direction = IDBCursorDirection::Prev;

    else if (arg1_string == "prevunique"sv)
        direction = IDBCursorDirection::Prevunique;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, arg1_string, "IDBCursorDirection");

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->open_key_cursor(query, direction); }));

    return &const_cast<IDBRequest&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::index)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::index");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "index");

    auto arg0 = vm.argument(0);

    String name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->index(name); }));

    return &const_cast<IDBIndex&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::create_index)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::create_index");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "createIndex", "2");

    auto arg0 = vm.argument(0);

    String name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<String, Vector<String>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<String> key_path;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    String sequence_item1;
    if (!false || !next_item1.is_null()) {
        sequence_item1 = TRY(WebIDL::to_string(vm, next_item1));
    }

    key_path.append(sequence_item1);
    }


            return key_path;
        }

        }

    String arg1_string;
    if (!false || !arg1.is_null()) {
        arg1_string = TRY(WebIDL::to_string(vm, arg1));
    }

        return { arg1_string };

    };

    Variant<String, Vector<String>> key_path = TRY(arg1_to_variant(arg1));

    auto arg2 = vm.argument(2);

    if (!arg2.is_nullish() && !arg2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "IDBIndexParameters");

    IDBIndexParameters options {};

    auto multi_entry_property_value_3 = JS::js_undefined();
    if (arg2.is_object())
        multi_entry_property_value_3 = TRY(arg2.as_object().get("multiEntry"_utf16_fly_string));

    bool multi_entry_value_3;

    if (!multi_entry_property_value_3.is_undefined())

    multi_entry_value_3 = multi_entry_property_value_3.to_boolean();

    else
        multi_entry_value_3 = static_cast<bool>(false);

    options.multi_entry = multi_entry_value_3;

    auto unique_property_value_4 = JS::js_undefined();
    if (arg2.is_object())
        unique_property_value_4 = TRY(arg2.as_object().get("unique"_utf16_fly_string));

    bool unique_value_4;

    if (!unique_property_value_4.is_undefined())

    unique_value_4 = unique_property_value_4.to_boolean();

    else
        unique_value_4 = static_cast<bool>(false);

    options.unique = unique_value_4;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_index(name, key_path, options); }));

    return &const_cast<IDBIndex&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBObjectStorePrototype::delete_index)
{
    WebIDL::log_trace(vm, "IDBObjectStorePrototype::delete_index");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "deleteIndex");

    auto arg0 = vm.argument(0);

    String name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_index(name); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
