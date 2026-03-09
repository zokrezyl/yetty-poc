
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
#include <LibWeb/Bindings/XRTestPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Internals/XRTest.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Internals/FakeXRDevice.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebXR/XRSystem.h>

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

GC_DEFINE_ALLOCATOR(XRTestPrototype);

XRTestPrototype::XRTestPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

XRTestPrototype::~XRTestPrototype()
{
}

void XRTestPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    define_native_function(realm, "simulateDeviceConnection"_utf16_fly_string, simulate_device_connection, 1, default_attributes);

    define_native_function(realm, "simulateUserActivation"_utf16_fly_string, simulate_user_activation, 1, default_attributes);

    define_native_function(realm, "disconnectAllDevices"_utf16_fly_string, disconnect_all_devices, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "XRTest"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void XRTestPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Internals::XRTest*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Internals::XRTest>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "XRTest");
    return static_cast<Internals::XRTest*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(XRTestPrototype::simulate_device_connection)
{
    WebIDL::log_trace(vm, "XRTestPrototype::simulate_device_connection");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "simulateDeviceConnection");

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "FakeXRDeviceInit");

    FakeXRDeviceInit init {};

    auto supported_features_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        supported_features_property_value_0 = TRY(arg0.as_object().get("supportedFeatures"_utf16_fly_string));

    if (!supported_features_property_value_0.is_undefined()) {

    Optional<Vector<String>> supported_features_value_0;

    if (!supported_features_property_value_0.is_undefined()) {

    if (!supported_features_property_value_0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, supported_features_property_value_0);

    auto supported_features_property_value_0_iterator_method0 = TRY(supported_features_property_value_0.get_method(vm, vm.well_known_symbol_iterator()));
    if (!supported_features_property_value_0_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, supported_features_property_value_0);

    auto supported_features_property_value_0_iterator1 = TRY(JS::get_iterator_from_method(vm, supported_features_property_value_0, *supported_features_property_value_0_iterator_method0));

    Vector<String> supported_features_value_0_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, supported_features_property_value_0_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    String sequence_item1;
    if (!false || !next_item1.is_null()) {
        sequence_item1 = TRY(WebIDL::to_string(vm, next_item1));
    }

    supported_features_value_0_non_optional.append(sequence_item1);
    }

        supported_features_value_0 = move(supported_features_value_0_non_optional);
    }

    init.supported_features = supported_features_value_0;

    }

    auto supported_modes_property_value_1 = JS::js_undefined();
    if (arg0.is_object())
        supported_modes_property_value_1 = TRY(arg0.as_object().get("supportedModes"_utf16_fly_string));

    if (!supported_modes_property_value_1.is_undefined()) {

    Optional<Vector<XRSessionMode>> supported_modes_value_1;

    if (!supported_modes_property_value_1.is_undefined()) {

    if (!supported_modes_property_value_1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, supported_modes_property_value_1);

    auto supported_modes_property_value_1_iterator_method0 = TRY(supported_modes_property_value_1.get_method(vm, vm.well_known_symbol_iterator()));
    if (!supported_modes_property_value_1_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, supported_modes_property_value_1);

    auto supported_modes_property_value_1_iterator1 = TRY(JS::get_iterator_from_method(vm, supported_modes_property_value_1, *supported_modes_property_value_1_iterator_method0));

    Vector<XRSessionMode> supported_modes_value_1_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, supported_modes_property_value_1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    XRSessionMode sequence_item1 { XRSessionMode::Inline };

    auto next_item1_string = TRY(next_item1.to_string(vm));

    if (next_item1_string == "inline"sv)
        sequence_item1 = XRSessionMode::Inline;

    else if (next_item1_string == "immersive-vr"sv)
        sequence_item1 = XRSessionMode::ImmersiveVr;

    else if (next_item1_string == "immersive-ar"sv)
        sequence_item1 = XRSessionMode::ImmersiveAr;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, next_item1_string, "XRSessionMode");

    supported_modes_value_1_non_optional.append(sequence_item1);
    }

        supported_modes_value_1 = move(supported_modes_value_1_non_optional);
    }

    init.supported_modes = supported_modes_value_1;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->simulate_device_connection(init); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If op has a return type that is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    // NOTE: We know that this is a Promise return type statically by the IDL.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

JS_DEFINE_NATIVE_FUNCTION(XRTestPrototype::simulate_user_activation)
{
    WebIDL::log_trace(vm, "XRTestPrototype::simulate_user_activation");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "simulateUserActivation");

    auto arg0 = vm.argument(0);

    if (!arg0.is_function()
)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, arg0);

    auto f = vm.heap().allocate<WebIDL::CallbackType>(arg0.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->simulate_user_activation(f); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(XRTestPrototype::disconnect_all_devices)
{
    WebIDL::log_trace(vm, "XRTestPrototype::disconnect_all_devices");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->disconnect_all_devices(); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If op has a return type that is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    // NOTE: We know that this is a Promise return type statically by the IDL.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

} // namespace Web::Bindings
