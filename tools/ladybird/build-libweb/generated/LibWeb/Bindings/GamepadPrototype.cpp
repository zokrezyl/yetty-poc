
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
#include <LibWeb/Bindings/GamepadPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Gamepad/Gamepad.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Gamepad/GamepadButton.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Gamepad/GamepadHapticActuator.h>

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

GC_DEFINE_ALLOCATOR(GamepadPrototype);

GamepadPrototype::GamepadPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

GamepadPrototype::~GamepadPrototype()
{
}

void GamepadPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_id_getter = JS::NativeFunction::create(realm, id_getter, 0, "id"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_id_setter;

    define_direct_accessor("id"_utf16_fly_string, native_id_getter, native_id_setter, default_attributes);

    auto native_index_getter = JS::NativeFunction::create(realm, index_getter, 0, "index"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_index_setter;

    define_direct_accessor("index"_utf16_fly_string, native_index_getter, native_index_setter, default_attributes);

    auto native_connected_getter = JS::NativeFunction::create(realm, connected_getter, 0, "connected"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_connected_setter;

    define_direct_accessor("connected"_utf16_fly_string, native_connected_getter, native_connected_setter, default_attributes);

    auto native_timestamp_getter = JS::NativeFunction::create(realm, timestamp_getter, 0, "timestamp"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_timestamp_setter;

    define_direct_accessor("timestamp"_utf16_fly_string, native_timestamp_getter, native_timestamp_setter, default_attributes);

    auto native_mapping_getter = JS::NativeFunction::create(realm, mapping_getter, 0, "mapping"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_mapping_setter;

    define_direct_accessor("mapping"_utf16_fly_string, native_mapping_getter, native_mapping_setter, default_attributes);

    auto native_axes_getter = JS::NativeFunction::create(realm, axes_getter, 0, "axes"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_axes_setter;

    define_direct_accessor("axes"_utf16_fly_string, native_axes_getter, native_axes_setter, default_attributes);

    auto native_buttons_getter = JS::NativeFunction::create(realm, buttons_getter, 0, "buttons"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_buttons_setter;

    define_direct_accessor("buttons"_utf16_fly_string, native_buttons_getter, native_buttons_setter, default_attributes);

    define_direct_property("touches"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    auto native_vibration_actuator_getter = JS::NativeFunction::create(realm, vibration_actuator_getter, 0, "vibrationActuator"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_vibration_actuator_setter;

    define_direct_accessor("vibrationActuator"_utf16_fly_string, native_vibration_actuator_getter, native_vibration_actuator_setter, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Gamepad"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void GamepadPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Gamepad::Gamepad*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Gamepad::Gamepad>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Gamepad");
    return static_cast<Gamepad::Gamepad*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(GamepadPrototype::id_getter)
{
    WebIDL::log_trace(vm, "GamepadPrototype::id_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->id(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(GamepadPrototype::index_getter)
{
    WebIDL::log_trace(vm, "GamepadPrototype::index_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->index(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(GamepadPrototype::connected_getter)
{
    WebIDL::log_trace(vm, "GamepadPrototype::connected_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->connected(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(GamepadPrototype::timestamp_getter)
{
    WebIDL::log_trace(vm, "GamepadPrototype::timestamp_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->timestamp(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(GamepadPrototype::mapping_getter)
{
    WebIDL::log_trace(vm, "GamepadPrototype::mapping_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->mapping(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(GamepadPrototype::axes_getter)
{
    WebIDL::log_trace(vm, "GamepadPrototype::axes_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->axes(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);
JS::Value wrapped_element0;

    wrapped_element0 = JS::Value(element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    TRY(new_array0_0->set_integrity_level(IntegrityLevel::Frozen));

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(GamepadPrototype::buttons_getter)
{
    WebIDL::log_trace(vm, "GamepadPrototype::buttons_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->buttons(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    TRY(new_array0_0->set_integrity_level(IntegrityLevel::Frozen));

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(GamepadPrototype::vibration_actuator_getter)
{
    WebIDL::log_trace(vm, "GamepadPrototype::vibration_actuator_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vibration_actuator(); }));

    return &const_cast<GamepadHapticActuator&>(*retval);

}

} // namespace Web::Bindings
