
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
#include <LibWeb/Bindings/XRSystemPrototype.h>
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

GC_DEFINE_ALLOCATOR(XRSystemPrototype);

XRSystemPrototype::XRSystemPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

XRSystemPrototype::~XRSystemPrototype()
{
}

void XRSystemPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    define_direct_property("ondevicechange"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_native_function(realm, "isSessionSupported"_utf16_fly_string, is_session_supported, 1, default_attributes);

    define_native_function(realm, "requestSession"_utf16_fly_string, request_session, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "XRSystem"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void XRSystemPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<XRSystem*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<XRSystem>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "XRSystem");
    return static_cast<XRSystem*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(XRSystemPrototype::is_session_supported)
{
    WebIDL::log_trace(vm, "XRSystemPrototype::is_session_supported");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "isSessionSupported");

    auto arg0 = vm.argument(0);

    XRSessionMode mode { XRSessionMode::Inline };

    auto arg0_string = TRY(arg0.to_string(vm));

    if (arg0_string == "inline"sv)
        mode = XRSessionMode::Inline;

    else if (arg0_string == "immersive-vr"sv)
        mode = XRSessionMode::ImmersiveVr;

    else if (arg0_string == "immersive-ar"sv)
        mode = XRSessionMode::ImmersiveAr;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, arg0_string, "XRSessionMode");

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_session_supported(mode); }));

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

JS_DEFINE_NATIVE_FUNCTION(XRSystemPrototype::request_session)
{
    WebIDL::log_trace(vm, "XRSystemPrototype::request_session");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "requestSession");

    auto arg0 = vm.argument(0);

    XRSessionMode mode { XRSessionMode::Inline };

    auto arg0_string = TRY(arg0.to_string(vm));

    if (arg0_string == "inline"sv)
        mode = XRSessionMode::Inline;

    else if (arg0_string == "immersive-vr"sv)
        mode = XRSessionMode::ImmersiveVr;

    else if (arg0_string == "immersive-ar"sv)
        mode = XRSessionMode::ImmersiveAr;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, arg0_string, "XRSessionMode");

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "XRSessionInit");

    XRSessionInit options {};

    auto optional_features_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        optional_features_property_value_0 = TRY(arg1.as_object().get("optionalFeatures"_utf16_fly_string));

    if (!optional_features_property_value_0.is_undefined()) {

    Optional<Vector<String>> optional_features_value_0;

    if (!optional_features_property_value_0.is_undefined()) {

    if (!optional_features_property_value_0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, optional_features_property_value_0);

    auto optional_features_property_value_0_iterator_method0 = TRY(optional_features_property_value_0.get_method(vm, vm.well_known_symbol_iterator()));
    if (!optional_features_property_value_0_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, optional_features_property_value_0);

    auto optional_features_property_value_0_iterator1 = TRY(JS::get_iterator_from_method(vm, optional_features_property_value_0, *optional_features_property_value_0_iterator_method0));

    Vector<String> optional_features_value_0_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, optional_features_property_value_0_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    String sequence_item1;
    if (!false || !next_item1.is_null()) {
        sequence_item1 = TRY(WebIDL::to_string(vm, next_item1));
    }

    optional_features_value_0_non_optional.append(sequence_item1);
    }

        optional_features_value_0 = move(optional_features_value_0_non_optional);
    }

    options.optional_features = optional_features_value_0;

    }

    auto required_features_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        required_features_property_value_1 = TRY(arg1.as_object().get("requiredFeatures"_utf16_fly_string));

    if (!required_features_property_value_1.is_undefined()) {

    Optional<Vector<String>> required_features_value_1;

    if (!required_features_property_value_1.is_undefined()) {

    if (!required_features_property_value_1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, required_features_property_value_1);

    auto required_features_property_value_1_iterator_method0 = TRY(required_features_property_value_1.get_method(vm, vm.well_known_symbol_iterator()));
    if (!required_features_property_value_1_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, required_features_property_value_1);

    auto required_features_property_value_1_iterator1 = TRY(JS::get_iterator_from_method(vm, required_features_property_value_1, *required_features_property_value_1_iterator_method0));

    Vector<String> required_features_value_1_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, required_features_property_value_1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    String sequence_item1;
    if (!false || !next_item1.is_null()) {
        sequence_item1 = TRY(WebIDL::to_string(vm, next_item1));
    }

    required_features_value_1_non_optional.append(sequence_item1);
    }

        required_features_value_1 = move(required_features_value_1_non_optional);
    }

    options.required_features = required_features_value_1;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->request_session(mode, options); }));

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
