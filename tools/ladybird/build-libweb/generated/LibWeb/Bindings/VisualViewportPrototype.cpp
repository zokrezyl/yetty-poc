
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
#include <LibWeb/Bindings/VisualViewportPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/VisualViewport.h>

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

GC_DEFINE_ALLOCATOR(VisualViewportPrototype);

VisualViewportPrototype::VisualViewportPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

VisualViewportPrototype::~VisualViewportPrototype()
{
}

void VisualViewportPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    auto native_offset_left_getter = JS::NativeFunction::create(realm, offset_left_getter, 0, "offsetLeft"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_offset_left_setter;

    define_direct_accessor("offsetLeft"_utf16_fly_string, native_offset_left_getter, native_offset_left_setter, default_attributes);

    auto native_offset_top_getter = JS::NativeFunction::create(realm, offset_top_getter, 0, "offsetTop"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_offset_top_setter;

    define_direct_accessor("offsetTop"_utf16_fly_string, native_offset_top_getter, native_offset_top_setter, default_attributes);

    auto native_page_left_getter = JS::NativeFunction::create(realm, page_left_getter, 0, "pageLeft"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_page_left_setter;

    define_direct_accessor("pageLeft"_utf16_fly_string, native_page_left_getter, native_page_left_setter, default_attributes);

    auto native_page_top_getter = JS::NativeFunction::create(realm, page_top_getter, 0, "pageTop"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_page_top_setter;

    define_direct_accessor("pageTop"_utf16_fly_string, native_page_top_getter, native_page_top_setter, default_attributes);

    auto native_width_getter = JS::NativeFunction::create(realm, width_getter, 0, "width"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_width_setter;

    define_direct_accessor("width"_utf16_fly_string, native_width_getter, native_width_setter, default_attributes);

    auto native_height_getter = JS::NativeFunction::create(realm, height_getter, 0, "height"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_height_setter;

    define_direct_accessor("height"_utf16_fly_string, native_height_getter, native_height_setter, default_attributes);

    auto native_scale_getter = JS::NativeFunction::create(realm, scale_getter, 0, "scale"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_scale_setter;

    define_direct_accessor("scale"_utf16_fly_string, native_scale_getter, native_scale_setter, default_attributes);

    auto native_onresize_getter = JS::NativeFunction::create(realm, onresize_getter, 0, "onresize"_utf16_fly_string, &realm, "get"sv);

    auto native_onresize_setter = JS::NativeFunction::create(realm, onresize_setter, 1, "onresize"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onresize"_utf16_fly_string, native_onresize_getter, native_onresize_setter, default_attributes);

    auto native_onscroll_getter = JS::NativeFunction::create(realm, onscroll_getter, 0, "onscroll"_utf16_fly_string, &realm, "get"sv);

    auto native_onscroll_setter = JS::NativeFunction::create(realm, onscroll_setter, 1, "onscroll"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onscroll"_utf16_fly_string, native_onscroll_getter, native_onscroll_setter, default_attributes);

    auto native_onscrollend_getter = JS::NativeFunction::create(realm, onscrollend_getter, 0, "onscrollend"_utf16_fly_string, &realm, "get"sv);

    auto native_onscrollend_setter = JS::NativeFunction::create(realm, onscrollend_setter, 1, "onscrollend"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onscrollend"_utf16_fly_string, native_onscrollend_getter, native_onscrollend_setter, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "VisualViewport"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void VisualViewportPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<CSS::VisualViewport*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<CSS::VisualViewport>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "VisualViewport");
    return static_cast<CSS::VisualViewport*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(VisualViewportPrototype::offset_left_getter)
{
    WebIDL::log_trace(vm, "VisualViewportPrototype::offset_left_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->offset_left(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(VisualViewportPrototype::offset_top_getter)
{
    WebIDL::log_trace(vm, "VisualViewportPrototype::offset_top_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->offset_top(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(VisualViewportPrototype::page_left_getter)
{
    WebIDL::log_trace(vm, "VisualViewportPrototype::page_left_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->page_left(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(VisualViewportPrototype::page_top_getter)
{
    WebIDL::log_trace(vm, "VisualViewportPrototype::page_top_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->page_top(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(VisualViewportPrototype::width_getter)
{
    WebIDL::log_trace(vm, "VisualViewportPrototype::width_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->width(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(VisualViewportPrototype::height_getter)
{
    WebIDL::log_trace(vm, "VisualViewportPrototype::height_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->height(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(VisualViewportPrototype::scale_getter)
{
    WebIDL::log_trace(vm, "VisualViewportPrototype::scale_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scale(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(VisualViewportPrototype::onresize_getter)
{
    WebIDL::log_trace(vm, "VisualViewportPrototype::onresize_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onresize(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(VisualViewportPrototype::onresize_setter)
{
    WebIDL::log_trace(vm, "VisualViewportPrototype::onresize_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onresize(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VisualViewportPrototype::onscroll_getter)
{
    WebIDL::log_trace(vm, "VisualViewportPrototype::onscroll_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onscroll(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(VisualViewportPrototype::onscroll_setter)
{
    WebIDL::log_trace(vm, "VisualViewportPrototype::onscroll_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onscroll(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VisualViewportPrototype::onscrollend_getter)
{
    WebIDL::log_trace(vm, "VisualViewportPrototype::onscrollend_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onscrollend(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(VisualViewportPrototype::onscrollend_setter)
{
    WebIDL::log_trace(vm, "VisualViewportPrototype::onscrollend_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onscrollend(cpp_value); }));

    return JS::js_undefined();
}

} // namespace Web::Bindings
