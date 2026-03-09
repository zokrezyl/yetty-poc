
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
#include <LibWeb/Bindings/StorageEventPrototype.h>
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

#if __has_include(<LibWeb/Bindings/EventPrototype.h>)
#    include <LibWeb/Bindings/EventPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/StorageEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Storage.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

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

GC_DEFINE_ALLOCATOR(StorageEventPrototype);

StorageEventPrototype::StorageEventPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

StorageEventPrototype::~StorageEventPrototype()
{
}

void StorageEventPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventPrototype>(realm, "Event"_fly_string));


    auto native_key_getter = JS::NativeFunction::create(realm, key_getter, 0, "key"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_key_setter;

    define_direct_accessor("key"_utf16_fly_string, native_key_getter, native_key_setter, default_attributes);

    auto native_old_value_getter = JS::NativeFunction::create(realm, old_value_getter, 0, "oldValue"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_old_value_setter;

    define_direct_accessor("oldValue"_utf16_fly_string, native_old_value_getter, native_old_value_setter, default_attributes);

    auto native_new_value_getter = JS::NativeFunction::create(realm, new_value_getter, 0, "newValue"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_new_value_setter;

    define_direct_accessor("newValue"_utf16_fly_string, native_new_value_getter, native_new_value_setter, default_attributes);

    auto native_url_getter = JS::NativeFunction::create(realm, url_getter, 0, "url"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_url_setter;

    define_direct_accessor("url"_utf16_fly_string, native_url_getter, native_url_setter, default_attributes);

    auto native_storage_area_getter = JS::NativeFunction::create(realm, storage_area_getter, 0, "storageArea"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_storage_area_setter;

    define_direct_accessor("storageArea"_utf16_fly_string, native_storage_area_getter, native_storage_area_setter, default_attributes);

    define_native_function(realm, "initStorageEvent"_utf16_fly_string, init_storage_event, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "StorageEvent"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void StorageEventPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::StorageEvent*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::StorageEvent>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "StorageEvent");
    return static_cast<HTML::StorageEvent*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(StorageEventPrototype::key_getter)
{
    WebIDL::log_trace(vm, "StorageEventPrototype::key_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->key(); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(StorageEventPrototype::old_value_getter)
{
    WebIDL::log_trace(vm, "StorageEventPrototype::old_value_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->old_value(); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(StorageEventPrototype::new_value_getter)
{
    WebIDL::log_trace(vm, "StorageEventPrototype::new_value_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->new_value(); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(StorageEventPrototype::url_getter)
{
    WebIDL::log_trace(vm, "StorageEventPrototype::url_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->url(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(StorageEventPrototype::storage_area_getter)
{
    WebIDL::log_trace(vm, "StorageEventPrototype::storage_area_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->storage_area(); }));

    if (retval) {

    return &const_cast<Storage&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(StorageEventPrototype::init_storage_event)
{
    WebIDL::log_trace(vm, "StorageEventPrototype::init_storage_event");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "initStorageEvent");

    auto arg0 = vm.argument(0);

    String type;
    if (!false || !arg0.is_null()) {
        type = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    bool bubbles;

    if (!arg1.is_undefined())

    bubbles = arg1.to_boolean();

    else
        bubbles = static_cast<bool>(false);

    auto arg2 = vm.argument(2);

    bool cancelable;

    if (!arg2.is_undefined())

    cancelable = arg2.to_boolean();

    else
        cancelable = static_cast<bool>(false);

    auto arg3 = vm.argument(3);

    Optional<String> key;

    if (!arg3.is_undefined()) {
        if (!arg3.is_null())
            key = TRY(WebIDL::to_string(vm, arg3));
    }

    auto arg4 = vm.argument(4);

    Optional<String> old_value;

    if (!arg4.is_undefined()) {
        if (!arg4.is_null())
            old_value = TRY(WebIDL::to_string(vm, arg4));
    }

    auto arg5 = vm.argument(5);

    Optional<String> new_value;

    if (!arg5.is_undefined()) {
        if (!arg5.is_null())
            new_value = TRY(WebIDL::to_string(vm, arg5));
    }

    auto arg6 = vm.argument(6);

    String url;

    if (!arg6.is_undefined()) {
        if (!false || !arg6.is_null())
            url = TRY(WebIDL::to_usv_string(vm, arg6));
    } else {
        url = ""_string;
    }

    auto arg7 = vm.argument(7);

    GC::Ptr<Storage> storage_area;

    if (!arg7.is_nullish()) {
        if (!arg7.is_object() || !is<Storage>(arg7.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Storage");

        storage_area = &static_cast<Storage&>(arg7.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->init_storage_event(type, bubbles, cancelable, key, old_value, new_value, url, storage_area); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
