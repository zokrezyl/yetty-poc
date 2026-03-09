
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
#include <LibWeb/Bindings/VTTCuePrototype.h>
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

#if __has_include(<LibWeb/Bindings/TextTrackCuePrototype.h>)
#    include <LibWeb/Bindings/TextTrackCuePrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebVTT/VTTCue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TextTrackCue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebVTT/VTTRegion.h>

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

GC_DEFINE_ALLOCATOR(VTTCuePrototype);

VTTCuePrototype::VTTCuePrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

VTTCuePrototype::~VTTCuePrototype()
{
}

void VTTCuePrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<TextTrackCuePrototype>(realm, "TextTrackCue"_fly_string));


    auto native_region_getter = JS::NativeFunction::create(realm, region_getter, 0, "region"_utf16_fly_string, &realm, "get"sv);

    auto native_region_setter = JS::NativeFunction::create(realm, region_setter, 1, "region"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("region"_utf16_fly_string, native_region_getter, native_region_setter, default_attributes);

    auto native_vertical_getter = JS::NativeFunction::create(realm, vertical_getter, 0, "vertical"_utf16_fly_string, &realm, "get"sv);

    auto native_vertical_setter = JS::NativeFunction::create(realm, vertical_setter, 1, "vertical"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("vertical"_utf16_fly_string, native_vertical_getter, native_vertical_setter, default_attributes);

    auto native_snap_to_lines_getter = JS::NativeFunction::create(realm, snap_to_lines_getter, 0, "snapToLines"_utf16_fly_string, &realm, "get"sv);

    auto native_snap_to_lines_setter = JS::NativeFunction::create(realm, snap_to_lines_setter, 1, "snapToLines"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("snapToLines"_utf16_fly_string, native_snap_to_lines_getter, native_snap_to_lines_setter, default_attributes);

    auto native_line_getter = JS::NativeFunction::create(realm, line_getter, 0, "line"_utf16_fly_string, &realm, "get"sv);

    auto native_line_setter = JS::NativeFunction::create(realm, line_setter, 1, "line"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("line"_utf16_fly_string, native_line_getter, native_line_setter, default_attributes);

    auto native_line_align_getter = JS::NativeFunction::create(realm, line_align_getter, 0, "lineAlign"_utf16_fly_string, &realm, "get"sv);

    auto native_line_align_setter = JS::NativeFunction::create(realm, line_align_setter, 1, "lineAlign"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("lineAlign"_utf16_fly_string, native_line_align_getter, native_line_align_setter, default_attributes);

    auto native_position_getter = JS::NativeFunction::create(realm, position_getter, 0, "position"_utf16_fly_string, &realm, "get"sv);

    auto native_position_setter = JS::NativeFunction::create(realm, position_setter, 1, "position"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("position"_utf16_fly_string, native_position_getter, native_position_setter, default_attributes);

    auto native_position_align_getter = JS::NativeFunction::create(realm, position_align_getter, 0, "positionAlign"_utf16_fly_string, &realm, "get"sv);

    auto native_position_align_setter = JS::NativeFunction::create(realm, position_align_setter, 1, "positionAlign"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("positionAlign"_utf16_fly_string, native_position_align_getter, native_position_align_setter, default_attributes);

    auto native_size_getter = JS::NativeFunction::create(realm, size_getter, 0, "size"_utf16_fly_string, &realm, "get"sv);

    auto native_size_setter = JS::NativeFunction::create(realm, size_setter, 1, "size"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("size"_utf16_fly_string, native_size_getter, native_size_setter, default_attributes);

    auto native_align_getter = JS::NativeFunction::create(realm, align_getter, 0, "align"_utf16_fly_string, &realm, "get"sv);

    auto native_align_setter = JS::NativeFunction::create(realm, align_setter, 1, "align"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("align"_utf16_fly_string, native_align_getter, native_align_setter, default_attributes);

    auto native_text_getter = JS::NativeFunction::create(realm, text_getter, 0, "text"_utf16_fly_string, &realm, "get"sv);

    auto native_text_setter = JS::NativeFunction::create(realm, text_setter, 1, "text"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("text"_utf16_fly_string, native_text_getter, native_text_setter, default_attributes);

        define_direct_property("getCueAsHTML"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "VTTCue"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void VTTCuePrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<VTTCue*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<VTTCue>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "VTTCue");
    return static_cast<VTTCue*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::region_getter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::region_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->region(); }));

    if (retval) {

    return &const_cast<VTTRegion&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::region_setter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::region_setter");
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

    GC::Ptr<VTTRegion> cpp_value;

    if (!value.is_nullish()) {
        if (!value.is_object() || !is<VTTRegion>(value.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "VTTRegion");

        cpp_value = &static_cast<VTTRegion&>(value.as_object());
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_region(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::vertical_getter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::vertical_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vertical(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::vertical_setter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::vertical_setter");
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

    DirectionSetting cpp_value { DirectionSetting::Rl };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == ""sv)
        cpp_value = DirectionSetting::Empty;

    else if (value_string == "rl"sv)
        cpp_value = DirectionSetting::Rl;

    else if (value_string == "lr"sv)
        cpp_value = DirectionSetting::Lr;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_vertical(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::snap_to_lines_getter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::snap_to_lines_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->snap_to_lines(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::snap_to_lines_setter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::snap_to_lines_setter");
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

    bool cpp_value;

    cpp_value = value.to_boolean();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_snap_to_lines(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::line_getter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::line_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->line(); }));

    return retval.visit(

        [&vm, &realm]([[maybe_unused]] double const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::Value(visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] AutoKeyword const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(visited_union_value0));

        }

    );

}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::line_setter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::line_setter");
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

    auto value_to_variant = [&vm, &realm](JS::Value value) -> JS::ThrowCompletionOr<Variant<double, AutoKeyword>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (value.is_object()) {
            [[maybe_unused]] auto& value_object = value.as_object();

        }

        if (value.is_number()) {

    double value_number = TRY(value.to_double(vm));

    if (isinf(value_number) || isnan(value_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "line");
    }
    
            return { value_number };
        }

        if (value.is_string()) {
            auto value_enum_string = TRY(value.to_string(vm));

            if (value_enum_string == "auto"sv)
                return Variant<double, AutoKeyword> { AutoKeyword::Auto };

        }

    double value_number = TRY(value.to_double(vm));

    if (isinf(value_number) || isnan(value_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "line");
    }
    
        return { value_number };

    };

    Variant<double, AutoKeyword> cpp_value = TRY(value_to_variant(value));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_line(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::line_align_getter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::line_align_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->line_align(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::line_align_setter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::line_align_setter");
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

    LineAlignSetting cpp_value { LineAlignSetting::Start };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == "start"sv)
        cpp_value = LineAlignSetting::Start;

    else if (value_string == "center"sv)
        cpp_value = LineAlignSetting::Center;

    else if (value_string == "end"sv)
        cpp_value = LineAlignSetting::End;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_line_align(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::position_getter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::position_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->position(); }));

    return retval.visit(

        [&vm, &realm]([[maybe_unused]] double const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::Value(visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] AutoKeyword const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(visited_union_value0));

        }

    );

}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::position_setter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::position_setter");
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

    auto value_to_variant = [&vm, &realm](JS::Value value) -> JS::ThrowCompletionOr<Variant<double, AutoKeyword>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (value.is_object()) {
            [[maybe_unused]] auto& value_object = value.as_object();

        }

        if (value.is_number()) {

    double value_number = TRY(value.to_double(vm));

    if (isinf(value_number) || isnan(value_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "position");
    }
    
            return { value_number };
        }

        if (value.is_string()) {
            auto value_enum_string = TRY(value.to_string(vm));

            if (value_enum_string == "auto"sv)
                return Variant<double, AutoKeyword> { AutoKeyword::Auto };

        }

    double value_number = TRY(value.to_double(vm));

    if (isinf(value_number) || isnan(value_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "position");
    }
    
        return { value_number };

    };

    Variant<double, AutoKeyword> cpp_value = TRY(value_to_variant(value));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_position(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::position_align_getter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::position_align_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->position_align(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::position_align_setter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::position_align_setter");
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

    PositionAlignSetting cpp_value { PositionAlignSetting::LineLeft };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == "line-left"sv)
        cpp_value = PositionAlignSetting::LineLeft;

    else if (value_string == "center"sv)
        cpp_value = PositionAlignSetting::Center;

    else if (value_string == "line-right"sv)
        cpp_value = PositionAlignSetting::LineRight;

    else if (value_string == "auto"sv)
        cpp_value = PositionAlignSetting::Auto;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_position_align(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::size_getter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::size_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->size(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::size_setter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::size_setter");
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
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "size");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_size(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::align_getter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::align_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->align(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::align_setter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::align_setter");
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

    AlignSetting cpp_value { AlignSetting::Start };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == "start"sv)
        cpp_value = AlignSetting::Start;

    else if (value_string == "center"sv)
        cpp_value = AlignSetting::Center;

    else if (value_string == "end"sv)
        cpp_value = AlignSetting::End;

    else if (value_string == "left"sv)
        cpp_value = AlignSetting::Left;

    else if (value_string == "right"sv)
        cpp_value = AlignSetting::Right;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_align(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::text_getter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::text_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->text(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(VTTCuePrototype::text_setter)
{
    WebIDL::log_trace(vm, "VTTCuePrototype::text_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_text(cpp_value); }));

    return JS::js_undefined();
}

} // namespace Web::Bindings
