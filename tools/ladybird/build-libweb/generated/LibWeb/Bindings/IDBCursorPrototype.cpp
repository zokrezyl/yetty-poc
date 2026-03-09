
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
#include <LibWeb/Bindings/IDBCursorPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBCursor.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBRequest.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

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

GC_DEFINE_ALLOCATOR(IDBCursorPrototype);

IDBCursorPrototype::IDBCursorPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

IDBCursorPrototype::~IDBCursorPrototype()
{
}

void IDBCursorPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_source_getter = JS::NativeFunction::create(realm, source_getter, 0, "source"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_source_setter;

    define_direct_accessor("source"_utf16_fly_string, native_source_getter, native_source_setter, default_attributes);

    auto native_direction_getter = JS::NativeFunction::create(realm, direction_getter, 0, "direction"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_direction_setter;

    define_direct_accessor("direction"_utf16_fly_string, native_direction_getter, native_direction_setter, default_attributes);

    auto native_key_getter = JS::NativeFunction::create(realm, key_getter, 0, "key"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_key_setter;

    define_direct_accessor("key"_utf16_fly_string, native_key_getter, native_key_setter, default_attributes);

    auto native_primary_key_getter = JS::NativeFunction::create(realm, primary_key_getter, 0, "primaryKey"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_primary_key_setter;

    define_direct_accessor("primaryKey"_utf16_fly_string, native_primary_key_getter, native_primary_key_setter, default_attributes);

    auto native_request_getter = JS::NativeFunction::create(realm, request_getter, 0, "request"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_request_setter;

    define_direct_accessor("request"_utf16_fly_string, native_request_getter, native_request_setter, default_attributes);

    define_native_function(realm, "advance"_utf16_fly_string, advance, 1, default_attributes);

    define_native_function(realm, "continue"_utf16_fly_string, continue_, 0, default_attributes);

    define_native_function(realm, "continuePrimaryKey"_utf16_fly_string, continue_primary_key, 2, default_attributes);

    define_native_function(realm, "update"_utf16_fly_string, update, 1, default_attributes);

    define_native_function(realm, "delete"_utf16_fly_string, delete_, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "IDBCursor"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void IDBCursorPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<IDBCursor*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<IDBCursor>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "IDBCursor");
    return static_cast<IDBCursor*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(IDBCursorPrototype::source_getter)
{
    WebIDL::log_trace(vm, "IDBCursorPrototype::source_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->source_handle(); }));

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

        }

    );

}

JS_DEFINE_NATIVE_FUNCTION(IDBCursorPrototype::direction_getter)
{
    WebIDL::log_trace(vm, "IDBCursorPrototype::direction_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->direction(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(IDBCursorPrototype::key_getter)
{
    WebIDL::log_trace(vm, "IDBCursorPrototype::key_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->key(); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(IDBCursorPrototype::primary_key_getter)
{
    WebIDL::log_trace(vm, "IDBCursorPrototype::primary_key_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->primary_key(); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(IDBCursorPrototype::request_getter)
{
    WebIDL::log_trace(vm, "IDBCursorPrototype::request_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->request(); }));

    return &const_cast<IDBRequest&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBCursorPrototype::advance)
{
    WebIDL::log_trace(vm, "IDBCursorPrototype::advance");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "advance");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong count;

    count = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->advance(count); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(IDBCursorPrototype::continue_)
{
    WebIDL::log_trace(vm, "IDBCursorPrototype::continue_");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    JS::Value key = JS::js_undefined();
    if (!arg0.is_undefined())
        key = arg0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->continue_(key); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(IDBCursorPrototype::continue_primary_key)
{
    WebIDL::log_trace(vm, "IDBCursorPrototype::continue_primary_key");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "continuePrimaryKey", "2");

    auto arg0 = vm.argument(0);

    auto key = arg0;

    auto arg1 = vm.argument(1);

    auto primary_key = arg1;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->continue_primary_key(key, primary_key); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(IDBCursorPrototype::update)
{
    WebIDL::log_trace(vm, "IDBCursorPrototype::update");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "update");

    auto arg0 = vm.argument(0);

    auto value = arg0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->update(value); }));

    return &const_cast<IDBRequest&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBCursorPrototype::delete_)
{
    WebIDL::log_trace(vm, "IDBCursorPrototype::delete_");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_(); }));

    return &const_cast<IDBRequest&>(*retval);

}

} // namespace Web::Bindings
