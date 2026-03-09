
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
#include <LibWeb/Bindings/HistoryPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/History.h>

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

GC_DEFINE_ALLOCATOR(HistoryPrototype);

HistoryPrototype::HistoryPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

HistoryPrototype::~HistoryPrototype()
{
}

void HistoryPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_length_getter = JS::NativeFunction::create(realm, length_getter, 0, "length"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_length_setter;

    define_direct_accessor("length"_utf16_fly_string, native_length_getter, native_length_setter, default_attributes);

    auto native_scroll_restoration_getter = JS::NativeFunction::create(realm, scroll_restoration_getter, 0, "scrollRestoration"_utf16_fly_string, &realm, "get"sv);

    auto native_scroll_restoration_setter = JS::NativeFunction::create(realm, scroll_restoration_setter, 1, "scrollRestoration"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("scrollRestoration"_utf16_fly_string, native_scroll_restoration_getter, native_scroll_restoration_setter, default_attributes);

    auto native_state_getter = JS::NativeFunction::create(realm, state_getter, 0, "state"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_state_setter;

    define_direct_accessor("state"_utf16_fly_string, native_state_getter, native_state_setter, default_attributes);

    define_native_function(realm, "go"_utf16_fly_string, go, 0, default_attributes);

    define_native_function(realm, "back"_utf16_fly_string, back, 0, default_attributes);

    define_native_function(realm, "forward"_utf16_fly_string, forward, 0, default_attributes);

    define_native_function(realm, "pushState"_utf16_fly_string, push_state, 2, default_attributes);

    define_native_function(realm, "replaceState"_utf16_fly_string, replace_state, 2, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "History"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void HistoryPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::History*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::History>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "History");
    return static_cast<HTML::History*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(HistoryPrototype::length_getter)
{
    WebIDL::log_trace(vm, "HistoryPrototype::length_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->length(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(HistoryPrototype::scroll_restoration_getter)
{
    WebIDL::log_trace(vm, "HistoryPrototype::scroll_restoration_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll_restoration(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(HistoryPrototype::scroll_restoration_setter)
{
    WebIDL::log_trace(vm, "HistoryPrototype::scroll_restoration_setter");
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

    ScrollRestoration cpp_value { ScrollRestoration::Auto };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == "auto"sv)
        cpp_value = ScrollRestoration::Auto;

    else if (value_string == "manual"sv)
        cpp_value = ScrollRestoration::Manual;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_scroll_restoration(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HistoryPrototype::state_getter)
{
    WebIDL::log_trace(vm, "HistoryPrototype::state_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->state(); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(HistoryPrototype::go)
{
    WebIDL::log_trace(vm, "HistoryPrototype::go");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::Long delta;

    if (!arg0.is_undefined())

    delta = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        delta = static_cast<WebIDL::Long>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->go(delta); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(HistoryPrototype::back)
{
    WebIDL::log_trace(vm, "HistoryPrototype::back");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->back(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(HistoryPrototype::forward)
{
    WebIDL::log_trace(vm, "HistoryPrototype::forward");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->forward(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(HistoryPrototype::push_state)
{
    WebIDL::log_trace(vm, "HistoryPrototype::push_state");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "pushState", "2");

    auto arg0 = vm.argument(0);

    auto data = arg0;

    auto arg1 = vm.argument(1);

    String unused;
    if (!false || !arg1.is_null()) {
        unused = TRY(WebIDL::to_string(vm, arg1));
    }

    auto arg2 = vm.argument(2);

    Optional<String> url;

    if (!arg2.is_undefined()) {
        if (!arg2.is_null())
            url = TRY(WebIDL::to_usv_string(vm, arg2));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->push_state(data, unused, url); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(HistoryPrototype::replace_state)
{
    WebIDL::log_trace(vm, "HistoryPrototype::replace_state");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "replaceState", "2");

    auto arg0 = vm.argument(0);

    auto data = arg0;

    auto arg1 = vm.argument(1);

    String unused;
    if (!false || !arg1.is_null()) {
        unused = TRY(WebIDL::to_string(vm, arg1));
    }

    auto arg2 = vm.argument(2);

    Optional<String> url;

    if (!arg2.is_undefined()) {
        if (!arg2.is_null())
            url = TRY(WebIDL::to_usv_string(vm, arg2));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->replace_state(data, unused, url); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
