
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
#include <LibWeb/Bindings/VTTRegionPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebVTT/VTTRegion.h>

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

GC_DEFINE_ALLOCATOR(VTTRegionPrototype);

VTTRegionPrototype::VTTRegionPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

VTTRegionPrototype::~VTTRegionPrototype()
{
}

void VTTRegionPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_id_getter = JS::NativeFunction::create(realm, id_getter, 0, "id"_utf16_fly_string, &realm, "get"sv);

    auto native_id_setter = JS::NativeFunction::create(realm, id_setter, 1, "id"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("id"_utf16_fly_string, native_id_getter, native_id_setter, default_attributes);

    auto native_width_getter = JS::NativeFunction::create(realm, width_getter, 0, "width"_utf16_fly_string, &realm, "get"sv);

    auto native_width_setter = JS::NativeFunction::create(realm, width_setter, 1, "width"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("width"_utf16_fly_string, native_width_getter, native_width_setter, default_attributes);

    auto native_lines_getter = JS::NativeFunction::create(realm, lines_getter, 0, "lines"_utf16_fly_string, &realm, "get"sv);

    auto native_lines_setter = JS::NativeFunction::create(realm, lines_setter, 1, "lines"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("lines"_utf16_fly_string, native_lines_getter, native_lines_setter, default_attributes);

    auto native_region_anchor_x_getter = JS::NativeFunction::create(realm, region_anchor_x_getter, 0, "regionAnchorX"_utf16_fly_string, &realm, "get"sv);

    auto native_region_anchor_x_setter = JS::NativeFunction::create(realm, region_anchor_x_setter, 1, "regionAnchorX"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("regionAnchorX"_utf16_fly_string, native_region_anchor_x_getter, native_region_anchor_x_setter, default_attributes);

    auto native_region_anchor_y_getter = JS::NativeFunction::create(realm, region_anchor_y_getter, 0, "regionAnchorY"_utf16_fly_string, &realm, "get"sv);

    auto native_region_anchor_y_setter = JS::NativeFunction::create(realm, region_anchor_y_setter, 1, "regionAnchorY"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("regionAnchorY"_utf16_fly_string, native_region_anchor_y_getter, native_region_anchor_y_setter, default_attributes);

    auto native_viewport_anchor_x_getter = JS::NativeFunction::create(realm, viewport_anchor_x_getter, 0, "viewportAnchorX"_utf16_fly_string, &realm, "get"sv);

    auto native_viewport_anchor_x_setter = JS::NativeFunction::create(realm, viewport_anchor_x_setter, 1, "viewportAnchorX"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("viewportAnchorX"_utf16_fly_string, native_viewport_anchor_x_getter, native_viewport_anchor_x_setter, default_attributes);

    auto native_viewport_anchor_y_getter = JS::NativeFunction::create(realm, viewport_anchor_y_getter, 0, "viewportAnchorY"_utf16_fly_string, &realm, "get"sv);

    auto native_viewport_anchor_y_setter = JS::NativeFunction::create(realm, viewport_anchor_y_setter, 1, "viewportAnchorY"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("viewportAnchorY"_utf16_fly_string, native_viewport_anchor_y_getter, native_viewport_anchor_y_setter, default_attributes);

    auto native_scroll_getter = JS::NativeFunction::create(realm, scroll_getter, 0, "scroll"_utf16_fly_string, &realm, "get"sv);

    auto native_scroll_setter = JS::NativeFunction::create(realm, scroll_setter, 1, "scroll"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("scroll"_utf16_fly_string, native_scroll_getter, native_scroll_setter, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "VTTRegion"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void VTTRegionPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<VTTRegion*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<VTTRegion>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "VTTRegion");
    return static_cast<VTTRegion*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(VTTRegionPrototype::id_getter)
{
    WebIDL::log_trace(vm, "VTTRegionPrototype::id_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->id(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(VTTRegionPrototype::id_setter)
{
    WebIDL::log_trace(vm, "VTTRegionPrototype::id_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_id(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VTTRegionPrototype::width_getter)
{
    WebIDL::log_trace(vm, "VTTRegionPrototype::width_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->width(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(VTTRegionPrototype::width_setter)
{
    WebIDL::log_trace(vm, "VTTRegionPrototype::width_setter");
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

    double cpp_value = TRY(value.to_double(vm));

    if (isinf(cpp_value) || isnan(cpp_value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "width");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_width(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VTTRegionPrototype::lines_getter)
{
    WebIDL::log_trace(vm, "VTTRegionPrototype::lines_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->lines(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(VTTRegionPrototype::lines_setter)
{
    WebIDL::log_trace(vm, "VTTRegionPrototype::lines_setter");
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

    WebIDL::UnsignedLong cpp_value;

    cpp_value = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, value, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_lines(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VTTRegionPrototype::region_anchor_x_getter)
{
    WebIDL::log_trace(vm, "VTTRegionPrototype::region_anchor_x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->region_anchor_x(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(VTTRegionPrototype::region_anchor_x_setter)
{
    WebIDL::log_trace(vm, "VTTRegionPrototype::region_anchor_x_setter");
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

    double cpp_value = TRY(value.to_double(vm));

    if (isinf(cpp_value) || isnan(cpp_value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "regionAnchorX");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_region_anchor_x(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VTTRegionPrototype::region_anchor_y_getter)
{
    WebIDL::log_trace(vm, "VTTRegionPrototype::region_anchor_y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->region_anchor_y(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(VTTRegionPrototype::region_anchor_y_setter)
{
    WebIDL::log_trace(vm, "VTTRegionPrototype::region_anchor_y_setter");
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

    double cpp_value = TRY(value.to_double(vm));

    if (isinf(cpp_value) || isnan(cpp_value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "regionAnchorY");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_region_anchor_y(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VTTRegionPrototype::viewport_anchor_x_getter)
{
    WebIDL::log_trace(vm, "VTTRegionPrototype::viewport_anchor_x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->viewport_anchor_x(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(VTTRegionPrototype::viewport_anchor_x_setter)
{
    WebIDL::log_trace(vm, "VTTRegionPrototype::viewport_anchor_x_setter");
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

    double cpp_value = TRY(value.to_double(vm));

    if (isinf(cpp_value) || isnan(cpp_value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "viewportAnchorX");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_viewport_anchor_x(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VTTRegionPrototype::viewport_anchor_y_getter)
{
    WebIDL::log_trace(vm, "VTTRegionPrototype::viewport_anchor_y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->viewport_anchor_y(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(VTTRegionPrototype::viewport_anchor_y_setter)
{
    WebIDL::log_trace(vm, "VTTRegionPrototype::viewport_anchor_y_setter");
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

    double cpp_value = TRY(value.to_double(vm));

    if (isinf(cpp_value) || isnan(cpp_value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "viewportAnchorY");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_viewport_anchor_y(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VTTRegionPrototype::scroll_getter)
{
    WebIDL::log_trace(vm, "VTTRegionPrototype::scroll_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(VTTRegionPrototype::scroll_setter)
{
    WebIDL::log_trace(vm, "VTTRegionPrototype::scroll_setter");
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

    ScrollSetting cpp_value { ScrollSetting::Up };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == ""sv)
        cpp_value = ScrollSetting::Empty;

    else if (value_string == "up"sv)
        cpp_value = ScrollSetting::Up;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_scroll(cpp_value); }));

    return JS::js_undefined();
}

} // namespace Web::Bindings
