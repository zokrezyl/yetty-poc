
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
#include <LibWeb/Bindings/InternalGamepadPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Internals/InternalGamepad.h>

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

GC_DEFINE_ALLOCATOR(InternalGamepadPrototype);

InternalGamepadPrototype::InternalGamepadPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

InternalGamepadPrototype::~InternalGamepadPrototype()
{
}

void InternalGamepadPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_buttons_getter = JS::NativeFunction::create(realm, buttons_getter, 0, "buttons"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_buttons_setter;

    define_direct_accessor("buttons"_utf16_fly_string, native_buttons_getter, native_buttons_setter, default_attributes);

    auto native_axes_getter = JS::NativeFunction::create(realm, axes_getter, 0, "axes"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_axes_setter;

    define_direct_accessor("axes"_utf16_fly_string, native_axes_getter, native_axes_setter, default_attributes);

    auto native_triggers_getter = JS::NativeFunction::create(realm, triggers_getter, 0, "triggers"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_triggers_setter;

    define_direct_accessor("triggers"_utf16_fly_string, native_triggers_getter, native_triggers_setter, default_attributes);

    define_native_function(realm, "setButton"_utf16_fly_string, set_button, 2, default_attributes);

    define_native_function(realm, "setAxis"_utf16_fly_string, set_axis, 2, default_attributes);

    define_native_function(realm, "getReceivedRumbleEffects"_utf16_fly_string, get_received_rumble_effects, 0, default_attributes);

    define_native_function(realm, "getReceivedRumbleTriggerEffects"_utf16_fly_string, get_received_rumble_trigger_effects, 0, default_attributes);

    define_native_function(realm, "disconnect"_utf16_fly_string, disconnect, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "InternalGamepad"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void InternalGamepadPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Internals::InternalGamepad*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Internals::InternalGamepad>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "InternalGamepad");
    return static_cast<Internals::InternalGamepad*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(InternalGamepadPrototype::buttons_getter)
{
    WebIDL::log_trace(vm, "InternalGamepadPrototype::buttons_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->buttons(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);
JS::Value wrapped_element0;

    wrapped_element0 = JS::Value(static_cast<WebIDL::Long>(element0));

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(InternalGamepadPrototype::axes_getter)
{
    WebIDL::log_trace(vm, "InternalGamepadPrototype::axes_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->axes(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);
JS::Value wrapped_element0;

    wrapped_element0 = JS::Value(static_cast<WebIDL::Long>(element0));

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(InternalGamepadPrototype::triggers_getter)
{
    WebIDL::log_trace(vm, "InternalGamepadPrototype::triggers_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->triggers(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);
JS::Value wrapped_element0;

    wrapped_element0 = JS::Value(static_cast<WebIDL::Long>(element0));

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(InternalGamepadPrototype::set_button)
{
    WebIDL::log_trace(vm, "InternalGamepadPrototype::set_button");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "setButton", "2");

    auto arg0 = vm.argument(0);

    WebIDL::Long button;

    button = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    bool down;

    down = arg1.to_boolean();

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_button(button, down); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalGamepadPrototype::set_axis)
{
    WebIDL::log_trace(vm, "InternalGamepadPrototype::set_axis");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "setAxis", "2");

    auto arg0 = vm.argument(0);

    WebIDL::Long axis;

    axis = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Short value;

    value = TRY(WebIDL::convert_to_int<WebIDL::Short>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_axis(axis, value); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalGamepadPrototype::get_received_rumble_effects)
{
    WebIDL::log_trace(vm, "InternalGamepadPrototype::get_received_rumble_effects");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_received_rumble_effects(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);
JS::Value wrapped_element0;

    wrapped_element0 = JS::Value(const_cast<JS::Object*>(element0));

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(InternalGamepadPrototype::get_received_rumble_trigger_effects)
{
    WebIDL::log_trace(vm, "InternalGamepadPrototype::get_received_rumble_trigger_effects");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_received_rumble_trigger_effects(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);
JS::Value wrapped_element0;

    wrapped_element0 = JS::Value(const_cast<JS::Object*>(element0));

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(InternalGamepadPrototype::disconnect)
{
    WebIDL::log_trace(vm, "InternalGamepadPrototype::disconnect");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->disconnect(); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
