
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
#include <LibWeb/Bindings/FontFaceSetPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/FontFaceSet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/FontFace.h>

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

GC_DEFINE_ALLOCATOR(FontFaceSetPrototype);

FontFaceSetPrototype::FontFaceSetPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

FontFaceSetPrototype::~FontFaceSetPrototype()
{
}

void FontFaceSetPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    auto native_onloading_getter = JS::NativeFunction::create(realm, onloading_getter, 0, "onloading"_utf16_fly_string, &realm, "get"sv);

    auto native_onloading_setter = JS::NativeFunction::create(realm, onloading_setter, 1, "onloading"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onloading"_utf16_fly_string, native_onloading_getter, native_onloading_setter, default_attributes);

    auto native_onloadingdone_getter = JS::NativeFunction::create(realm, onloadingdone_getter, 0, "onloadingdone"_utf16_fly_string, &realm, "get"sv);

    auto native_onloadingdone_setter = JS::NativeFunction::create(realm, onloadingdone_setter, 1, "onloadingdone"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onloadingdone"_utf16_fly_string, native_onloadingdone_getter, native_onloadingdone_setter, default_attributes);

    auto native_onloadingerror_getter = JS::NativeFunction::create(realm, onloadingerror_getter, 0, "onloadingerror"_utf16_fly_string, &realm, "get"sv);

    auto native_onloadingerror_setter = JS::NativeFunction::create(realm, onloadingerror_setter, 1, "onloadingerror"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onloadingerror"_utf16_fly_string, native_onloadingerror_getter, native_onloadingerror_setter, default_attributes);

    auto native_ready_getter = JS::NativeFunction::create(realm, ready_getter, 0, "ready"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_ready_setter;

    define_direct_accessor("ready"_utf16_fly_string, native_ready_getter, native_ready_setter, default_attributes);

    auto native_status_getter = JS::NativeFunction::create(realm, status_getter, 0, "status"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_status_setter;

    define_direct_accessor("status"_utf16_fly_string, native_status_getter, native_status_setter, default_attributes);

        define_direct_property("check"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_native_function(realm, "add"_utf16_fly_string, add, 1, default_attributes);

    define_native_function(realm, "delete"_utf16_fly_string, delete_, 1, default_attributes);

    define_native_function(realm, "clear"_utf16_fly_string, clear, 0, default_attributes);

    define_native_function(realm, "load"_utf16_fly_string, load, 1, default_attributes);

    define_native_accessor(realm, vm.names.size, get_size, nullptr, JS::Attribute::Enumerable | JS::Attribute::Configurable);
    define_native_function(realm, vm.names.entries, entries, 0, default_attributes);
    // NOTE: Keys intentionally returns values for setlike
    define_native_function(realm, vm.names.keys, values, 0, default_attributes);
    define_native_function(realm, vm.names.values, values, 0, default_attributes);
    define_direct_property(vm.well_known_symbol_iterator(), get_without_side_effects(vm.names.values), JS::Attribute::Configurable | JS::Attribute::Writable);
    define_native_function(realm, vm.names.forEach, for_each, 1, default_attributes);
    define_native_function(realm, vm.names.has, has, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "FontFaceSet"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void FontFaceSetPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<CSS::FontFaceSet*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<CSS::FontFaceSet>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "FontFaceSet");
    return static_cast<CSS::FontFaceSet*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(FontFaceSetPrototype::onloading_getter)
{
    WebIDL::log_trace(vm, "FontFaceSetPrototype::onloading_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onloading(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(FontFaceSetPrototype::onloading_setter)
{
    WebIDL::log_trace(vm, "FontFaceSetPrototype::onloading_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onloading(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(FontFaceSetPrototype::onloadingdone_getter)
{
    WebIDL::log_trace(vm, "FontFaceSetPrototype::onloadingdone_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onloadingdone(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(FontFaceSetPrototype::onloadingdone_setter)
{
    WebIDL::log_trace(vm, "FontFaceSetPrototype::onloadingdone_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onloadingdone(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(FontFaceSetPrototype::onloadingerror_getter)
{
    WebIDL::log_trace(vm, "FontFaceSetPrototype::onloadingerror_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onloadingerror(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(FontFaceSetPrototype::onloadingerror_setter)
{
    WebIDL::log_trace(vm, "FontFaceSetPrototype::onloadingerror_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onloadingerror(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(FontFaceSetPrototype::ready_getter)
{
    WebIDL::log_trace(vm, "FontFaceSetPrototype::ready_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&]() -> JS::ThrowCompletionOr<GC::Ptr<WebIDL::Promise>> {

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ready(); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If attribute’s type is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

JS_DEFINE_NATIVE_FUNCTION(FontFaceSetPrototype::status_getter)
{
    WebIDL::log_trace(vm, "FontFaceSetPrototype::status_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->status(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(FontFaceSetPrototype::add)
{
    WebIDL::log_trace(vm, "FontFaceSetPrototype::add");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "add");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<FontFace>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "FontFace");

    auto& font = static_cast<FontFace&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->add(font); }));

    return &const_cast<FontFaceSet&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(FontFaceSetPrototype::delete_)
{
    WebIDL::log_trace(vm, "FontFaceSetPrototype::delete_");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "delete");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<FontFace>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "FontFace");

    auto& font = static_cast<FontFace&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_(font); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(FontFaceSetPrototype::clear)
{
    WebIDL::log_trace(vm, "FontFaceSetPrototype::clear");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(FontFaceSetPrototype::load)
{
    WebIDL::log_trace(vm, "FontFaceSetPrototype::load");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "load");

    auto arg0 = vm.argument(0);

    String font;
    if (!false || !arg0.is_null()) {
        font = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    String text;

    if (!arg1.is_undefined()) {
        if (!false || !arg1.is_null())
            text = TRY(WebIDL::to_string(vm, arg1));
    } else {
        text = ""_string;
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->load(font, text); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If op has a return type that is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    // NOTE: We know that this is a Promise return type statically by the IDL.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

// https://webidl.spec.whatwg.org/#js-set-size
JS_DEFINE_NATIVE_FUNCTION(FontFaceSetPrototype::get_size)
{
    WebIDL::log_trace(vm, "FontFaceSetPrototype::size");
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Set> set = impl->set_entries();

    return set->set_size();
}

// https://webidl.spec.whatwg.org/#js-set-entries
JS_DEFINE_NATIVE_FUNCTION(FontFaceSetPrototype::entries)
{
    WebIDL::log_trace(vm, "FontFaceSetPrototype::entries");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Set> set = impl->set_entries();

    return TRY(throw_dom_exception_if_needed(vm, [&] { return JS::SetIterator::create(realm, *set, Object::PropertyKind::KeyAndValue); }));
}

// https://webidl.spec.whatwg.org/#js-set-values
JS_DEFINE_NATIVE_FUNCTION(FontFaceSetPrototype::values)
{
    WebIDL::log_trace(vm, "FontFaceSetPrototype::values");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Set> set = impl->set_entries();

    return TRY(throw_dom_exception_if_needed(vm, [&] { return JS::SetIterator::create(realm, *set, Object::PropertyKind::Value); }));
}

// https://webidl.spec.whatwg.org/#js-set-forEach
JS_DEFINE_NATIVE_FUNCTION(FontFaceSetPrototype::for_each)
{
    WebIDL::log_trace(vm, "FontFaceSetPrototype::for_each");
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Set> set = impl->set_entries();

    auto callback = vm.argument(0);
    if (!callback.is_function())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, callback);

    for (auto& entry : *set) {
        auto value = entry.key;
        TRY(JS::call(vm, callback.as_function(), vm.argument(1), value, value, impl));
    }

    return JS::js_undefined();
}

// https://webidl.spec.whatwg.org/#js-set-has
JS_DEFINE_NATIVE_FUNCTION(FontFaceSetPrototype::has)
{
    WebIDL::log_trace(vm, "FontFaceSetPrototype::has");
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Set> set = impl->set_entries();

    auto value_arg = vm.argument(0);
    
    if (!value_arg.is_object() || !is<FontFace>(value_arg.as_object())) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "FontFace");
    }


    // FIXME: If value is -0, set value to +0.
    // What? Which interfaces have a number as their set type?

    return set->set_has(value_arg);
}

} // namespace Web::Bindings
