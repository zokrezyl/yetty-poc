
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
#include <LibWeb/Bindings/AudioBufferSourceNodePrototype.h>
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

#if __has_include(<LibWeb/Bindings/AudioScheduledSourceNodePrototype.h>)
#    include <LibWeb/Bindings/AudioScheduledSourceNodePrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioBufferSourceNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioBuffer.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioParam.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioScheduledSourceNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/BaseAudioContext.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioDestinationNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioListener.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/ChannelMergerNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/DynamicsCompressorNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/GainNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/OscillatorNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/PannerNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebIDL/DOMException.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/PeriodicWave.h>

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

GC_DEFINE_ALLOCATOR(AudioBufferSourceNodePrototype);

AudioBufferSourceNodePrototype::AudioBufferSourceNodePrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

AudioBufferSourceNodePrototype::~AudioBufferSourceNodePrototype()
{
}

void AudioBufferSourceNodePrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<AudioScheduledSourceNodePrototype>(realm, "AudioScheduledSourceNode"_fly_string));


    auto native_buffer_getter = JS::NativeFunction::create(realm, buffer_getter, 0, "buffer"_utf16_fly_string, &realm, "get"sv);

    auto native_buffer_setter = JS::NativeFunction::create(realm, buffer_setter, 1, "buffer"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("buffer"_utf16_fly_string, native_buffer_getter, native_buffer_setter, default_attributes);

    auto native_playback_rate_getter = JS::NativeFunction::create(realm, playback_rate_getter, 0, "playbackRate"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_playback_rate_setter;

    define_direct_accessor("playbackRate"_utf16_fly_string, native_playback_rate_getter, native_playback_rate_setter, default_attributes);

    auto native_detune_getter = JS::NativeFunction::create(realm, detune_getter, 0, "detune"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_detune_setter;

    define_direct_accessor("detune"_utf16_fly_string, native_detune_getter, native_detune_setter, default_attributes);

    auto native_loop_getter = JS::NativeFunction::create(realm, loop_getter, 0, "loop"_utf16_fly_string, &realm, "get"sv);

    auto native_loop_setter = JS::NativeFunction::create(realm, loop_setter, 1, "loop"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("loop"_utf16_fly_string, native_loop_getter, native_loop_setter, default_attributes);

    auto native_loop_start_getter = JS::NativeFunction::create(realm, loop_start_getter, 0, "loopStart"_utf16_fly_string, &realm, "get"sv);

    auto native_loop_start_setter = JS::NativeFunction::create(realm, loop_start_setter, 1, "loopStart"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("loopStart"_utf16_fly_string, native_loop_start_getter, native_loop_start_setter, default_attributes);

    auto native_loop_end_getter = JS::NativeFunction::create(realm, loop_end_getter, 0, "loopEnd"_utf16_fly_string, &realm, "get"sv);

    auto native_loop_end_setter = JS::NativeFunction::create(realm, loop_end_setter, 1, "loopEnd"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("loopEnd"_utf16_fly_string, native_loop_end_getter, native_loop_end_setter, default_attributes);

    define_native_function(realm, "start"_utf16_fly_string, start, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "AudioBufferSourceNode"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void AudioBufferSourceNodePrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<WebAudio::AudioBufferSourceNode*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<WebAudio::AudioBufferSourceNode>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AudioBufferSourceNode");
    return static_cast<WebAudio::AudioBufferSourceNode*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferSourceNodePrototype::buffer_getter)
{
    WebIDL::log_trace(vm, "AudioBufferSourceNodePrototype::buffer_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->buffer(); }));

    if (retval) {

    return &const_cast<AudioBuffer&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferSourceNodePrototype::buffer_setter)
{
    WebIDL::log_trace(vm, "AudioBufferSourceNodePrototype::buffer_setter");
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

    GC::Ptr<AudioBuffer> cpp_value;

    if (!value.is_nullish()) {
        if (!value.is_object() || !is<AudioBuffer>(value.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AudioBuffer");

        cpp_value = &static_cast<AudioBuffer&>(value.as_object());
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_buffer(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferSourceNodePrototype::playback_rate_getter)
{
    WebIDL::log_trace(vm, "AudioBufferSourceNodePrototype::playback_rate_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->playback_rate(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferSourceNodePrototype::detune_getter)
{
    WebIDL::log_trace(vm, "AudioBufferSourceNodePrototype::detune_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->detune(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferSourceNodePrototype::loop_getter)
{
    WebIDL::log_trace(vm, "AudioBufferSourceNodePrototype::loop_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->loop(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferSourceNodePrototype::loop_setter)
{
    WebIDL::log_trace(vm, "AudioBufferSourceNodePrototype::loop_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_loop(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferSourceNodePrototype::loop_start_getter)
{
    WebIDL::log_trace(vm, "AudioBufferSourceNodePrototype::loop_start_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->loop_start(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferSourceNodePrototype::loop_start_setter)
{
    WebIDL::log_trace(vm, "AudioBufferSourceNodePrototype::loop_start_setter");
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
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "loopStart");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_loop_start(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferSourceNodePrototype::loop_end_getter)
{
    WebIDL::log_trace(vm, "AudioBufferSourceNodePrototype::loop_end_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->loop_end(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferSourceNodePrototype::loop_end_setter)
{
    WebIDL::log_trace(vm, "AudioBufferSourceNodePrototype::loop_end_setter");
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
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "loopEnd");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_loop_end(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AudioBufferSourceNodePrototype::start)
{
    WebIDL::log_trace(vm, "AudioBufferSourceNodePrototype::start");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    double when;

    if (!arg0.is_undefined())
        when = TRY(arg0.to_double(vm));

    else
        when = 0;

    if (isinf(when) || isnan(when)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "when");
    }
    
    auto arg1 = vm.argument(1);

    Optional<double> offset;

    if (!arg1.is_undefined())
        offset = TRY(arg1.to_double(vm));


    if (offset.has_value() && (isinf(*offset) || isnan(*offset))) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "offset");
    }
    
    auto arg2 = vm.argument(2);

    Optional<double> duration;

    if (!arg2.is_undefined())
        duration = TRY(arg2.to_double(vm));


    if (duration.has_value() && (isinf(*duration) || isnan(*duration))) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "duration");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->start(when, offset, duration); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
