
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
#include <LibWeb/Bindings/AudioScheduledSourceNodePrototype.h>
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

#if __has_include(<LibWeb/Bindings/AudioNodePrototype.h>)
#    include <LibWeb/Bindings/AudioNodePrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioScheduledSourceNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioParam.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/BaseAudioContext.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioBuffer.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioBufferSourceNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioDestinationNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioListener.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/ChannelMergerNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/DynamicsCompressorNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/GainNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/OscillatorNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/PannerNode.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebIDL/DOMException.h>

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

GC_DEFINE_ALLOCATOR(AudioScheduledSourceNodePrototype);

AudioScheduledSourceNodePrototype::AudioScheduledSourceNodePrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

AudioScheduledSourceNodePrototype::~AudioScheduledSourceNodePrototype()
{
}

void AudioScheduledSourceNodePrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<AudioNodePrototype>(realm, "AudioNode"_fly_string));


    auto native_onended_getter = JS::NativeFunction::create(realm, onended_getter, 0, "onended"_utf16_fly_string, &realm, "get"sv);

    auto native_onended_setter = JS::NativeFunction::create(realm, onended_setter, 1, "onended"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onended"_utf16_fly_string, native_onended_getter, native_onended_setter, default_attributes);

    define_native_function(realm, "start"_utf16_fly_string, start, 0, default_attributes);

    define_native_function(realm, "stop"_utf16_fly_string, stop, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "AudioScheduledSourceNode"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void AudioScheduledSourceNodePrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<WebAudio::AudioScheduledSourceNode*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<WebAudio::AudioScheduledSourceNode>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AudioScheduledSourceNode");
    return static_cast<WebAudio::AudioScheduledSourceNode*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(AudioScheduledSourceNodePrototype::onended_getter)
{
    WebIDL::log_trace(vm, "AudioScheduledSourceNodePrototype::onended_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onended(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(AudioScheduledSourceNodePrototype::onended_setter)
{
    WebIDL::log_trace(vm, "AudioScheduledSourceNodePrototype::onended_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onended(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(AudioScheduledSourceNodePrototype::start)
{
    WebIDL::log_trace(vm, "AudioScheduledSourceNodePrototype::start");
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
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->start(when); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(AudioScheduledSourceNodePrototype::stop)
{
    WebIDL::log_trace(vm, "AudioScheduledSourceNodePrototype::stop");
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
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->stop(when); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
