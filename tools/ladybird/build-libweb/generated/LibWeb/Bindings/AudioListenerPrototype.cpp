
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
#include <LibWeb/Bindings/AudioListenerPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAudio/AudioListener.h>

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

GC_DEFINE_ALLOCATOR(AudioListenerPrototype);

AudioListenerPrototype::AudioListenerPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

AudioListenerPrototype::~AudioListenerPrototype()
{
}

void AudioListenerPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_position_x_getter = JS::NativeFunction::create(realm, position_x_getter, 0, "positionX"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_position_x_setter;

    define_direct_accessor("positionX"_utf16_fly_string, native_position_x_getter, native_position_x_setter, default_attributes);

    auto native_position_y_getter = JS::NativeFunction::create(realm, position_y_getter, 0, "positionY"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_position_y_setter;

    define_direct_accessor("positionY"_utf16_fly_string, native_position_y_getter, native_position_y_setter, default_attributes);

    auto native_position_z_getter = JS::NativeFunction::create(realm, position_z_getter, 0, "positionZ"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_position_z_setter;

    define_direct_accessor("positionZ"_utf16_fly_string, native_position_z_getter, native_position_z_setter, default_attributes);

    auto native_forward_x_getter = JS::NativeFunction::create(realm, forward_x_getter, 0, "forwardX"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_forward_x_setter;

    define_direct_accessor("forwardX"_utf16_fly_string, native_forward_x_getter, native_forward_x_setter, default_attributes);

    auto native_forward_y_getter = JS::NativeFunction::create(realm, forward_y_getter, 0, "forwardY"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_forward_y_setter;

    define_direct_accessor("forwardY"_utf16_fly_string, native_forward_y_getter, native_forward_y_setter, default_attributes);

    auto native_forward_z_getter = JS::NativeFunction::create(realm, forward_z_getter, 0, "forwardZ"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_forward_z_setter;

    define_direct_accessor("forwardZ"_utf16_fly_string, native_forward_z_getter, native_forward_z_setter, default_attributes);

    auto native_up_x_getter = JS::NativeFunction::create(realm, up_x_getter, 0, "upX"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_up_x_setter;

    define_direct_accessor("upX"_utf16_fly_string, native_up_x_getter, native_up_x_setter, default_attributes);

    auto native_up_y_getter = JS::NativeFunction::create(realm, up_y_getter, 0, "upY"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_up_y_setter;

    define_direct_accessor("upY"_utf16_fly_string, native_up_y_getter, native_up_y_setter, default_attributes);

    auto native_up_z_getter = JS::NativeFunction::create(realm, up_z_getter, 0, "upZ"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_up_z_setter;

    define_direct_accessor("upZ"_utf16_fly_string, native_up_z_getter, native_up_z_setter, default_attributes);

    define_native_function(realm, "setPosition"_utf16_fly_string, set_position, 3, default_attributes);

    define_native_function(realm, "setOrientation"_utf16_fly_string, set_orientation, 6, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "AudioListener"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void AudioListenerPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<WebAudio::AudioListener*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<WebAudio::AudioListener>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AudioListener");
    return static_cast<WebAudio::AudioListener*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(AudioListenerPrototype::position_x_getter)
{
    WebIDL::log_trace(vm, "AudioListenerPrototype::position_x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->position_x(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioListenerPrototype::position_y_getter)
{
    WebIDL::log_trace(vm, "AudioListenerPrototype::position_y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->position_y(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioListenerPrototype::position_z_getter)
{
    WebIDL::log_trace(vm, "AudioListenerPrototype::position_z_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->position_z(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioListenerPrototype::forward_x_getter)
{
    WebIDL::log_trace(vm, "AudioListenerPrototype::forward_x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->forward_x(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioListenerPrototype::forward_y_getter)
{
    WebIDL::log_trace(vm, "AudioListenerPrototype::forward_y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->forward_y(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioListenerPrototype::forward_z_getter)
{
    WebIDL::log_trace(vm, "AudioListenerPrototype::forward_z_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->forward_z(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioListenerPrototype::up_x_getter)
{
    WebIDL::log_trace(vm, "AudioListenerPrototype::up_x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->up_x(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioListenerPrototype::up_y_getter)
{
    WebIDL::log_trace(vm, "AudioListenerPrototype::up_y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->up_y(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioListenerPrototype::up_z_getter)
{
    WebIDL::log_trace(vm, "AudioListenerPrototype::up_z_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->up_z(); }));

    return &const_cast<AudioParam&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(AudioListenerPrototype::set_position)
{
    WebIDL::log_trace(vm, "AudioListenerPrototype::set_position");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "setPosition", "3");

    auto arg0 = vm.argument(0);

    float x = TRY(arg0.to_double(vm));

    if (isinf(x) || isnan(x)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
    auto arg1 = vm.argument(1);

    float y = TRY(arg1.to_double(vm));

    if (isinf(y) || isnan(y)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
    auto arg2 = vm.argument(2);

    float z = TRY(arg2.to_double(vm));

    if (isinf(z) || isnan(z)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "z");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_position(x, y, z); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(AudioListenerPrototype::set_orientation)
{
    WebIDL::log_trace(vm, "AudioListenerPrototype::set_orientation");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 6)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "setOrientation", "6");

    auto arg0 = vm.argument(0);

    float x = TRY(arg0.to_double(vm));

    if (isinf(x) || isnan(x)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
    auto arg1 = vm.argument(1);

    float y = TRY(arg1.to_double(vm));

    if (isinf(y) || isnan(y)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
    auto arg2 = vm.argument(2);

    float z = TRY(arg2.to_double(vm));

    if (isinf(z) || isnan(z)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "z");
    }
    
    auto arg3 = vm.argument(3);

    float x_up = TRY(arg3.to_double(vm));

    if (isinf(x_up) || isnan(x_up)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "xUp");
    }
    
    auto arg4 = vm.argument(4);

    float y_up = TRY(arg4.to_double(vm));

    if (isinf(y_up) || isnan(y_up)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "yUp");
    }
    
    auto arg5 = vm.argument(5);

    float z_up = TRY(arg5.to_double(vm));

    if (isinf(z_up) || isnan(z_up)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "zUp");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_orientation(x, y, z, x_up, y_up, z_up); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
