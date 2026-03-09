
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
#include <LibWeb/Bindings/CSSPageDescriptorsPrototype.h>
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

#if __has_include(<LibWeb/Bindings/CSSStyleDeclarationPrototype.h>)
#    include <LibWeb/Bindings/CSSStyleDeclarationPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSPageDescriptors.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleDeclaration.h>

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

GC_DEFINE_ALLOCATOR(CSSPageDescriptorsPrototype);

CSSPageDescriptorsPrototype::CSSPageDescriptorsPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

CSSPageDescriptorsPrototype::~CSSPageDescriptorsPrototype()
{
}

void CSSPageDescriptorsPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<CSSStyleDeclarationPrototype>(realm, "CSSStyleDeclaration"_fly_string));


    auto native_margin_getter = JS::NativeFunction::create(realm, margin_getter, 0, "margin"_utf16_fly_string, &realm, "get"sv);

    auto native_margin_setter = JS::NativeFunction::create(realm, margin_setter, 1, "margin"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("margin"_utf16_fly_string, native_margin_getter, native_margin_setter, default_attributes);

    auto native_margin_top_regular_getter = JS::NativeFunction::create(realm, margin_top_regular_getter, 0, "marginTop"_utf16_fly_string, &realm, "get"sv);

    auto native_margin_top_regular_setter = JS::NativeFunction::create(realm, margin_top_regular_setter, 1, "marginTop"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("marginTop"_utf16_fly_string, native_margin_top_regular_getter, native_margin_top_regular_setter, default_attributes);

    auto native_margin_right_regular_getter = JS::NativeFunction::create(realm, margin_right_regular_getter, 0, "marginRight"_utf16_fly_string, &realm, "get"sv);

    auto native_margin_right_regular_setter = JS::NativeFunction::create(realm, margin_right_regular_setter, 1, "marginRight"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("marginRight"_utf16_fly_string, native_margin_right_regular_getter, native_margin_right_regular_setter, default_attributes);

    auto native_margin_bottom_regular_getter = JS::NativeFunction::create(realm, margin_bottom_regular_getter, 0, "marginBottom"_utf16_fly_string, &realm, "get"sv);

    auto native_margin_bottom_regular_setter = JS::NativeFunction::create(realm, margin_bottom_regular_setter, 1, "marginBottom"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("marginBottom"_utf16_fly_string, native_margin_bottom_regular_getter, native_margin_bottom_regular_setter, default_attributes);

    auto native_margin_left_regular_getter = JS::NativeFunction::create(realm, margin_left_regular_getter, 0, "marginLeft"_utf16_fly_string, &realm, "get"sv);

    auto native_margin_left_regular_setter = JS::NativeFunction::create(realm, margin_left_regular_setter, 1, "marginLeft"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("marginLeft"_utf16_fly_string, native_margin_left_regular_getter, native_margin_left_regular_setter, default_attributes);

    auto native_margin_top_dashed_getter = JS::NativeFunction::create(realm, margin_top_dashed_getter, 0, "margin-top"_utf16_fly_string, &realm, "get"sv);

    auto native_margin_top_dashed_setter = JS::NativeFunction::create(realm, margin_top_dashed_setter, 1, "margin-top"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("margin-top"_utf16_fly_string, native_margin_top_dashed_getter, native_margin_top_dashed_setter, default_attributes);

    auto native_margin_right_dashed_getter = JS::NativeFunction::create(realm, margin_right_dashed_getter, 0, "margin-right"_utf16_fly_string, &realm, "get"sv);

    auto native_margin_right_dashed_setter = JS::NativeFunction::create(realm, margin_right_dashed_setter, 1, "margin-right"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("margin-right"_utf16_fly_string, native_margin_right_dashed_getter, native_margin_right_dashed_setter, default_attributes);

    auto native_margin_bottom_dashed_getter = JS::NativeFunction::create(realm, margin_bottom_dashed_getter, 0, "margin-bottom"_utf16_fly_string, &realm, "get"sv);

    auto native_margin_bottom_dashed_setter = JS::NativeFunction::create(realm, margin_bottom_dashed_setter, 1, "margin-bottom"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("margin-bottom"_utf16_fly_string, native_margin_bottom_dashed_getter, native_margin_bottom_dashed_setter, default_attributes);

    auto native_margin_left_dashed_getter = JS::NativeFunction::create(realm, margin_left_dashed_getter, 0, "margin-left"_utf16_fly_string, &realm, "get"sv);

    auto native_margin_left_dashed_setter = JS::NativeFunction::create(realm, margin_left_dashed_setter, 1, "margin-left"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("margin-left"_utf16_fly_string, native_margin_left_dashed_getter, native_margin_left_dashed_setter, default_attributes);

    auto native_size_getter = JS::NativeFunction::create(realm, size_getter, 0, "size"_utf16_fly_string, &realm, "get"sv);

    auto native_size_setter = JS::NativeFunction::create(realm, size_setter, 1, "size"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("size"_utf16_fly_string, native_size_getter, native_size_setter, default_attributes);

    auto native_page_orientation_regular_getter = JS::NativeFunction::create(realm, page_orientation_regular_getter, 0, "pageOrientation"_utf16_fly_string, &realm, "get"sv);

    auto native_page_orientation_regular_setter = JS::NativeFunction::create(realm, page_orientation_regular_setter, 1, "pageOrientation"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("pageOrientation"_utf16_fly_string, native_page_orientation_regular_getter, native_page_orientation_regular_setter, default_attributes);

    auto native_page_orientation_dashed_getter = JS::NativeFunction::create(realm, page_orientation_dashed_getter, 0, "page-orientation"_utf16_fly_string, &realm, "get"sv);

    auto native_page_orientation_dashed_setter = JS::NativeFunction::create(realm, page_orientation_dashed_setter, 1, "page-orientation"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("page-orientation"_utf16_fly_string, native_page_orientation_dashed_getter, native_page_orientation_dashed_setter, default_attributes);

    auto native_marks_getter = JS::NativeFunction::create(realm, marks_getter, 0, "marks"_utf16_fly_string, &realm, "get"sv);

    auto native_marks_setter = JS::NativeFunction::create(realm, marks_setter, 1, "marks"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("marks"_utf16_fly_string, native_marks_getter, native_marks_setter, default_attributes);

    auto native_bleed_getter = JS::NativeFunction::create(realm, bleed_getter, 0, "bleed"_utf16_fly_string, &realm, "get"sv);

    auto native_bleed_setter = JS::NativeFunction::create(realm, bleed_setter, 1, "bleed"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("bleed"_utf16_fly_string, native_bleed_getter, native_bleed_setter, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "CSSPageDescriptors"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void CSSPageDescriptorsPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<CSS::CSSPageDescriptors*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<CSS::CSSPageDescriptors>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CSSPageDescriptors");
    return static_cast<CSS::CSSPageDescriptors*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_getter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->margin(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_setter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_margin(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_top_regular_getter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_top_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->margin_top(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_top_regular_setter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_top_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_margin_top(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_right_regular_getter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_right_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->margin_right(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_right_regular_setter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_right_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_margin_right(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_bottom_regular_getter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_bottom_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->margin_bottom(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_bottom_regular_setter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_bottom_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_margin_bottom(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_left_regular_getter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_left_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->margin_left(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_left_regular_setter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_left_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_margin_left(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_top_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_top_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->margin_top(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_top_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_top_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_margin_top(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_right_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_right_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->margin_right(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_right_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_right_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_margin_right(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_bottom_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_bottom_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->margin_bottom(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_bottom_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_bottom_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_margin_bottom(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_left_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_left_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->margin_left(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::margin_left_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::margin_left_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_margin_left(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::size_getter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::size_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->size(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::size_setter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::size_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_size(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::page_orientation_regular_getter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::page_orientation_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->page_orientation(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::page_orientation_regular_setter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::page_orientation_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_page_orientation(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::page_orientation_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::page_orientation_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->page_orientation(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::page_orientation_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::page_orientation_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_page_orientation(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::marks_getter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::marks_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->marks(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::marks_setter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::marks_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_marks(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::bleed_getter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::bleed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bleed(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSPageDescriptorsPrototype::bleed_setter)
{
    WebIDL::log_trace(vm, "CSSPageDescriptorsPrototype::bleed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_bleed(cpp_value); }));

    return JS::js_undefined();
}

} // namespace Web::Bindings
