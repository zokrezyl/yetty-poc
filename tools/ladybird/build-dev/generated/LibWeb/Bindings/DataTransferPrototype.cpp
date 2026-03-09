
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
#include <LibWeb/Bindings/DataTransferPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/DataTransfer.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/FileList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/DataTransferItemList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/File.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/DataTransferItem.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/EntriesAPI/FileSystemEntry.h>

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

GC_DEFINE_ALLOCATOR(DataTransferPrototype);

DataTransferPrototype::DataTransferPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

DataTransferPrototype::~DataTransferPrototype()
{
}

void DataTransferPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_drop_effect_getter = JS::NativeFunction::create(realm, drop_effect_getter, 0, "dropEffect"_utf16_fly_string, &realm, "get"sv);

    auto native_drop_effect_setter = JS::NativeFunction::create(realm, drop_effect_setter, 1, "dropEffect"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("dropEffect"_utf16_fly_string, native_drop_effect_getter, native_drop_effect_setter, default_attributes);

    auto native_effect_allowed_getter = JS::NativeFunction::create(realm, effect_allowed_getter, 0, "effectAllowed"_utf16_fly_string, &realm, "get"sv);

    auto native_effect_allowed_setter = JS::NativeFunction::create(realm, effect_allowed_setter, 1, "effectAllowed"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("effectAllowed"_utf16_fly_string, native_effect_allowed_getter, native_effect_allowed_setter, default_attributes);

    auto native_items_getter = JS::NativeFunction::create(realm, items_getter, 0, "items"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_items_setter;

    define_direct_accessor("items"_utf16_fly_string, native_items_getter, native_items_setter, default_attributes);

    auto native_types_getter = JS::NativeFunction::create(realm, types_getter, 0, "types"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_types_setter;

    define_direct_accessor("types"_utf16_fly_string, native_types_getter, native_types_setter, default_attributes);

    auto native_files_getter = JS::NativeFunction::create(realm, files_getter, 0, "files"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_files_setter;

    define_direct_accessor("files"_utf16_fly_string, native_files_getter, native_files_setter, default_attributes);

        define_direct_property("setDragImage"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_native_function(realm, "getData"_utf16_fly_string, get_data, 1, default_attributes);

    define_native_function(realm, "setData"_utf16_fly_string, set_data, 2, default_attributes);

    define_native_function(realm, "clearData"_utf16_fly_string, clear_data, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "DataTransfer"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void DataTransferPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::DataTransfer*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::DataTransfer>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DataTransfer");
    return static_cast<HTML::DataTransfer*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(DataTransferPrototype::drop_effect_getter)
{
    WebIDL::log_trace(vm, "DataTransferPrototype::drop_effect_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->drop_effect(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DataTransferPrototype::drop_effect_setter)
{
    WebIDL::log_trace(vm, "DataTransferPrototype::drop_effect_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_drop_effect(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DataTransferPrototype::effect_allowed_getter)
{
    WebIDL::log_trace(vm, "DataTransferPrototype::effect_allowed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->effect_allowed(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DataTransferPrototype::effect_allowed_setter)
{
    WebIDL::log_trace(vm, "DataTransferPrototype::effect_allowed_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_effect_allowed(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DataTransferPrototype::items_getter)
{
    WebIDL::log_trace(vm, "DataTransferPrototype::items_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->items(); }));

    return &const_cast<DataTransferItemList&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DataTransferPrototype::types_getter)
{
    WebIDL::log_trace(vm, "DataTransferPrototype::types_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto cached_types = impl->cached_types();
    if (cached_types)
        return cached_types;

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->types(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);
JS::Value wrapped_element0;

    wrapped_element0 = JS::PrimitiveString::create(vm, element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    TRY(new_array0_0->set_integrity_level(IntegrityLevel::Frozen));

    cached_types = new_array0_0;

    impl->set_cached_types(cached_types);
    return cached_types;

}

JS_DEFINE_NATIVE_FUNCTION(DataTransferPrototype::files_getter)
{
    WebIDL::log_trace(vm, "DataTransferPrototype::files_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->files(); }));

    return &const_cast<FileList&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DataTransferPrototype::get_data)
{
    WebIDL::log_trace(vm, "DataTransferPrototype::get_data");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getData");

    auto arg0 = vm.argument(0);

    String format;
    if (!false || !arg0.is_null()) {
        format = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_data(format); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DataTransferPrototype::set_data)
{
    WebIDL::log_trace(vm, "DataTransferPrototype::set_data");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "setData", "2");

    auto arg0 = vm.argument(0);

    String format;
    if (!false || !arg0.is_null()) {
        format = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    String data;
    if (!false || !arg1.is_null()) {
        data = TRY(WebIDL::to_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_data(format, data); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(DataTransferPrototype::clear_data)
{
    WebIDL::log_trace(vm, "DataTransferPrototype::clear_data");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    Optional<String> format;

    if (!arg0.is_undefined()) {
        if (!false || !arg0.is_null())
            format = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear_data(format); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
