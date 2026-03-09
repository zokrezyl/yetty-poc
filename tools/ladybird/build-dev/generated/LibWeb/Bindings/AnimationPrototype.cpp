
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
#include <LibWeb/Bindings/AnimationPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/Animation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/AnimationEffect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/AnimationTimeline.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSNumericValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSUnitValue.h>

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

GC_DEFINE_ALLOCATOR(AnimationPrototype);

AnimationPrototype::AnimationPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

AnimationPrototype::~AnimationPrototype()
{
}

void AnimationPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    auto native_id_getter = JS::NativeFunction::create(realm, id_getter, 0, "id"_utf16_fly_string, &realm, "get"sv);

    auto native_id_setter = JS::NativeFunction::create(realm, id_setter, 1, "id"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("id"_utf16_fly_string, native_id_getter, native_id_setter, default_attributes);

    auto native_effect_getter = JS::NativeFunction::create(realm, effect_getter, 0, "effect"_utf16_fly_string, &realm, "get"sv);

    auto native_effect_setter = JS::NativeFunction::create(realm, effect_setter, 1, "effect"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("effect"_utf16_fly_string, native_effect_getter, native_effect_setter, default_attributes);

    auto native_timeline_getter = JS::NativeFunction::create(realm, timeline_getter, 0, "timeline"_utf16_fly_string, &realm, "get"sv);

    auto native_timeline_setter = JS::NativeFunction::create(realm, timeline_setter, 1, "timeline"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("timeline"_utf16_fly_string, native_timeline_getter, native_timeline_setter, default_attributes);

    auto native_start_time_getter = JS::NativeFunction::create(realm, start_time_getter, 0, "startTime"_utf16_fly_string, &realm, "get"sv);

    auto native_start_time_setter = JS::NativeFunction::create(realm, start_time_setter, 1, "startTime"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("startTime"_utf16_fly_string, native_start_time_getter, native_start_time_setter, default_attributes);

    auto native_current_time_getter = JS::NativeFunction::create(realm, current_time_getter, 0, "currentTime"_utf16_fly_string, &realm, "get"sv);

    auto native_current_time_setter = JS::NativeFunction::create(realm, current_time_setter, 1, "currentTime"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("currentTime"_utf16_fly_string, native_current_time_getter, native_current_time_setter, default_attributes);

    auto native_playback_rate_getter = JS::NativeFunction::create(realm, playback_rate_getter, 0, "playbackRate"_utf16_fly_string, &realm, "get"sv);

    auto native_playback_rate_setter = JS::NativeFunction::create(realm, playback_rate_setter, 1, "playbackRate"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("playbackRate"_utf16_fly_string, native_playback_rate_getter, native_playback_rate_setter, default_attributes);

    auto native_play_state_getter = JS::NativeFunction::create(realm, play_state_getter, 0, "playState"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_play_state_setter;

    define_direct_accessor("playState"_utf16_fly_string, native_play_state_getter, native_play_state_setter, default_attributes);

    auto native_replace_state_getter = JS::NativeFunction::create(realm, replace_state_getter, 0, "replaceState"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_replace_state_setter;

    define_direct_accessor("replaceState"_utf16_fly_string, native_replace_state_getter, native_replace_state_setter, default_attributes);

    auto native_pending_getter = JS::NativeFunction::create(realm, pending_getter, 0, "pending"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_pending_setter;

    define_direct_accessor("pending"_utf16_fly_string, native_pending_getter, native_pending_setter, default_attributes);

    auto native_ready_getter = JS::NativeFunction::create(realm, ready_getter, 0, "ready"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_ready_setter;

    define_direct_accessor("ready"_utf16_fly_string, native_ready_getter, native_ready_setter, default_attributes);

    auto native_finished_getter = JS::NativeFunction::create(realm, finished_getter, 0, "finished"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_finished_setter;

    define_direct_accessor("finished"_utf16_fly_string, native_finished_getter, native_finished_setter, default_attributes);

    auto native_onfinish_getter = JS::NativeFunction::create(realm, onfinish_getter, 0, "onfinish"_utf16_fly_string, &realm, "get"sv);

    auto native_onfinish_setter = JS::NativeFunction::create(realm, onfinish_setter, 1, "onfinish"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onfinish"_utf16_fly_string, native_onfinish_getter, native_onfinish_setter, default_attributes);

    auto native_oncancel_getter = JS::NativeFunction::create(realm, oncancel_getter, 0, "oncancel"_utf16_fly_string, &realm, "get"sv);

    auto native_oncancel_setter = JS::NativeFunction::create(realm, oncancel_setter, 1, "oncancel"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("oncancel"_utf16_fly_string, native_oncancel_getter, native_oncancel_setter, default_attributes);

    auto native_onremove_getter = JS::NativeFunction::create(realm, onremove_getter, 0, "onremove"_utf16_fly_string, &realm, "get"sv);

    auto native_onremove_setter = JS::NativeFunction::create(realm, onremove_setter, 1, "onremove"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onremove"_utf16_fly_string, native_onremove_getter, native_onremove_setter, default_attributes);

        define_direct_property("commitStyles"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_native_function(realm, "cancel"_utf16_fly_string, cancel, 0, default_attributes);

    define_native_function(realm, "finish"_utf16_fly_string, finish, 0, default_attributes);

    define_native_function(realm, "play"_utf16_fly_string, play, 0, default_attributes);

    define_native_function(realm, "pause"_utf16_fly_string, pause, 0, default_attributes);

    define_native_function(realm, "updatePlaybackRate"_utf16_fly_string, update_playback_rate, 1, default_attributes);

    define_native_function(realm, "reverse"_utf16_fly_string, reverse, 0, default_attributes);

    define_native_function(realm, "persist"_utf16_fly_string, persist, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Animation"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void AnimationPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Animation*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Animation>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Animation");
    return static_cast<Animation*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::id_getter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::id_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->id(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::id_setter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::id_setter");
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

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::effect_getter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::effect_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->effect(); }));

    if (retval) {

    return &const_cast<AnimationEffect&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::effect_setter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::effect_setter");
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

    GC::Ptr<AnimationEffect> cpp_value;

    if (!value.is_nullish()) {
        if (!value.is_object() || !is<AnimationEffect>(value.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AnimationEffect");

        cpp_value = &static_cast<AnimationEffect&>(value.as_object());
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_effect(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::timeline_getter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::timeline_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->timeline_for_bindings(); }));

    if (retval) {

    return &const_cast<AnimationTimeline&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::timeline_setter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::timeline_setter");
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

    GC::Ptr<AnimationTimeline> cpp_value;

    if (!value.is_nullish()) {
        if (!value.is_object() || !is<AnimationTimeline>(value.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AnimationTimeline");

        cpp_value = &static_cast<AnimationTimeline&>(value.as_object());
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_timeline_for_bindings(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::start_time_getter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::start_time_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->start_time_for_bindings(); }));

    return retval.visit(

        [&vm, &realm]([[maybe_unused]] double const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::Value(visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<CSSNumericValue> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<CSSNumericValue&>(*visited_union_value0);

        },

        [](Empty) -> JS::Value {
            return JS::js_null();
        }

    );

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::start_time_setter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::start_time_setter");
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

    auto value_to_variant = [&vm, &realm](JS::Value value) -> JS::ThrowCompletionOr<Variant<double, GC::Root<CSSNumericValue>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (value.is_object()) {
            [[maybe_unused]] auto& value_object = value.as_object();

            if (is<PlatformObject>(value_object)) {

                if (auto* value_result = as_if<CSSNumericValue>(value_object))
                    return GC::make_root(*value_result);

            }

        }

        if (value.is_number()) {

    double value_number = TRY(value.to_double(vm));

    if (isinf(value_number) || isnan(value_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "startTime");
    }
    
            return { value_number };
        }

    double value_number = TRY(value.to_double(vm));

    if (isinf(value_number) || isnan(value_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "startTime");
    }
    
        return { value_number };

    };

    Variant<double, GC::Root<CSSNumericValue>> cpp_value = TRY(value_to_variant(value));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_start_time_for_bindings(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::current_time_getter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::current_time_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->current_time_for_bindings(); }));

    return retval.visit(

        [&vm, &realm]([[maybe_unused]] double const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::Value(visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<CSSNumericValue> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<CSSNumericValue&>(*visited_union_value0);

        },

        [](Empty) -> JS::Value {
            return JS::js_null();
        }

    );

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::current_time_setter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::current_time_setter");
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

    auto value_to_variant = [&vm, &realm](JS::Value value) -> JS::ThrowCompletionOr<Variant<double, GC::Root<CSSNumericValue>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (value.is_object()) {
            [[maybe_unused]] auto& value_object = value.as_object();

            if (is<PlatformObject>(value_object)) {

                if (auto* value_result = as_if<CSSNumericValue>(value_object))
                    return GC::make_root(*value_result);

            }

        }

        if (value.is_number()) {

    double value_number = TRY(value.to_double(vm));

    if (isinf(value_number) || isnan(value_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "currentTime");
    }
    
            return { value_number };
        }

    double value_number = TRY(value.to_double(vm));

    if (isinf(value_number) || isnan(value_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "currentTime");
    }
    
        return { value_number };

    };

    Variant<double, GC::Root<CSSNumericValue>> cpp_value = TRY(value_to_variant(value));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_current_time_for_bindings(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::playback_rate_getter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::playback_rate_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->playback_rate(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::playback_rate_setter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::playback_rate_setter");
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
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "playbackRate");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_playback_rate(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::play_state_getter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::play_state_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->play_state_for_bindings(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::replace_state_getter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::replace_state_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->replace_state(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::pending_getter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::pending_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->pending(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::ready_getter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::ready_getter");
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

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::finished_getter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::finished_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&]() -> JS::ThrowCompletionOr<GC::Ptr<WebIDL::Promise>> {

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->finished(); }));

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

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::onfinish_getter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::onfinish_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onfinish(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::onfinish_setter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::onfinish_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onfinish(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::oncancel_getter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::oncancel_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncancel(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::oncancel_setter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::oncancel_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_oncancel(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::onremove_getter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::onremove_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onremove(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::onremove_setter)
{
    WebIDL::log_trace(vm, "AnimationPrototype::onremove_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onremove(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::cancel)
{
    WebIDL::log_trace(vm, "AnimationPrototype::cancel");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cancel(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::finish)
{
    WebIDL::log_trace(vm, "AnimationPrototype::finish");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->finish(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::play)
{
    WebIDL::log_trace(vm, "AnimationPrototype::play");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->play(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::pause)
{
    WebIDL::log_trace(vm, "AnimationPrototype::pause");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->pause(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::update_playback_rate)
{
    WebIDL::log_trace(vm, "AnimationPrototype::update_playback_rate");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "updatePlaybackRate");

    auto arg0 = vm.argument(0);

    double playback_rate = TRY(arg0.to_double(vm));

    if (isinf(playback_rate) || isnan(playback_rate)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "playbackRate");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->update_playback_rate(playback_rate); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::reverse)
{
    WebIDL::log_trace(vm, "AnimationPrototype::reverse");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->reverse(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(AnimationPrototype::persist)
{
    WebIDL::log_trace(vm, "AnimationPrototype::persist");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->persist(); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
