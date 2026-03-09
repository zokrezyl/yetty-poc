
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
#include <LibWeb/Bindings/MediaQueryListPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/MediaQueryList.h>

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

GC_DEFINE_ALLOCATOR(MediaQueryListPrototype);

MediaQueryListPrototype::MediaQueryListPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

MediaQueryListPrototype::~MediaQueryListPrototype()
{
}

void MediaQueryListPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    auto native_media_getter = JS::NativeFunction::create(realm, media_getter, 0, "media"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_media_setter;

    define_direct_accessor("media"_utf16_fly_string, native_media_getter, native_media_setter, default_attributes);

    auto native_matches_getter = JS::NativeFunction::create(realm, matches_getter, 0, "matches"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_matches_setter;

    define_direct_accessor("matches"_utf16_fly_string, native_matches_getter, native_matches_setter, default_attributes);

    auto native_onchange_getter = JS::NativeFunction::create(realm, onchange_getter, 0, "onchange"_utf16_fly_string, &realm, "get"sv);

    auto native_onchange_setter = JS::NativeFunction::create(realm, onchange_setter, 1, "onchange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onchange"_utf16_fly_string, native_onchange_getter, native_onchange_setter, default_attributes);

    define_native_function(realm, "addListener"_utf16_fly_string, add_listener, 1, default_attributes);

    define_native_function(realm, "removeListener"_utf16_fly_string, remove_listener, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "MediaQueryList"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void MediaQueryListPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<CSS::MediaQueryList*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<CSS::MediaQueryList>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "MediaQueryList");
    return static_cast<CSS::MediaQueryList*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(MediaQueryListPrototype::media_getter)
{
    WebIDL::log_trace(vm, "MediaQueryListPrototype::media_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->media(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(MediaQueryListPrototype::matches_getter)
{
    WebIDL::log_trace(vm, "MediaQueryListPrototype::matches_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->matches(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MediaQueryListPrototype::onchange_getter)
{
    WebIDL::log_trace(vm, "MediaQueryListPrototype::onchange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onchange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(MediaQueryListPrototype::onchange_setter)
{
    WebIDL::log_trace(vm, "MediaQueryListPrototype::onchange_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onchange(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(MediaQueryListPrototype::add_listener)
{
    WebIDL::log_trace(vm, "MediaQueryListPrototype::add_listener");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "addListener");

    auto arg0 = vm.argument(0);

    IDLEventListener* callback = nullptr;
    if (!arg0.is_nullish()) {
        if (!arg0.is_object())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg0);

        auto callback_type = vm.heap().allocate<WebIDL::CallbackType>(arg0.as_object(), HTML::incumbent_realm());
        callback = TRY(throw_dom_exception_if_needed(vm, [&] { return IDLEventListener::create(realm, *callback_type); }));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->add_listener(callback); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(MediaQueryListPrototype::remove_listener)
{
    WebIDL::log_trace(vm, "MediaQueryListPrototype::remove_listener");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "removeListener");

    auto arg0 = vm.argument(0);

    IDLEventListener* callback = nullptr;
    if (!arg0.is_nullish()) {
        if (!arg0.is_object())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg0);

        auto callback_type = vm.heap().allocate<WebIDL::CallbackType>(arg0.as_object(), HTML::incumbent_realm());
        callback = TRY(throw_dom_exception_if_needed(vm, [&] { return IDLEventListener::create(realm, *callback_type); }));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->remove_listener(callback); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
