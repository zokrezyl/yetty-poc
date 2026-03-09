
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
#include <LibWeb/Bindings/CredentialsContainerPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CredentialManagement/CredentialsContainer.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CredentialManagement/Credential.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CredentialManagement/FederatedCredential.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CredentialManagement/PasswordCredential.h>

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

GC_DEFINE_ALLOCATOR(CredentialsContainerPrototype);

CredentialsContainerPrototype::CredentialsContainerPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

CredentialsContainerPrototype::~CredentialsContainerPrototype()
{
}

void CredentialsContainerPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    define_native_function(realm, "get"_utf16_fly_string, get, 0, default_attributes);

    define_native_function(realm, "store"_utf16_fly_string, store, 1, default_attributes);

    define_native_function(realm, "create"_utf16_fly_string, create, 0, default_attributes);

    define_native_function(realm, "preventSilentAccess"_utf16_fly_string, prevent_silent_access, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "CredentialsContainer"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void CredentialsContainerPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<CredentialsContainer*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<CredentialsContainer>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CredentialsContainer");
    return static_cast<CredentialsContainer*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(CredentialsContainerPrototype::get)
{
    WebIDL::log_trace(vm, "CredentialsContainerPrototype::get");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CredentialRequestOptions");

    CredentialRequestOptions options {};

    auto mediation_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        mediation_property_value_0 = TRY(arg0.as_object().get("mediation"_utf16_fly_string));

    CredentialMediationRequirement mediation_value_0 { CredentialMediationRequirement::Optional };

    if (!mediation_property_value_0.is_undefined()) {

    auto mediation_property_value_0_string = TRY(mediation_property_value_0.to_string(vm));

    if (mediation_property_value_0_string == "silent"sv)
        mediation_value_0 = CredentialMediationRequirement::Silent;

    else if (mediation_property_value_0_string == "optional"sv)
        mediation_value_0 = CredentialMediationRequirement::Optional;

    else if (mediation_property_value_0_string == "conditional"sv)
        mediation_value_0 = CredentialMediationRequirement::Conditional;

    else if (mediation_property_value_0_string == "required"sv)
        mediation_value_0 = CredentialMediationRequirement::Required;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, mediation_property_value_0_string, "CredentialMediationRequirement");

    }

    options.mediation = mediation_value_0;

    auto signal_property_value_1 = JS::js_undefined();
    if (arg0.is_object())
        signal_property_value_1 = TRY(arg0.as_object().get("signal"_utf16_fly_string));

    if (!signal_property_value_1.is_undefined()) {

    GC::Ptr<AbortSignal> signal_value_1;
    if (!signal_property_value_1.is_undefined()) {
        if (!signal_property_value_1.is_object() || !is<AbortSignal>(signal_property_value_1.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AbortSignal");

        signal_value_1 = static_cast<AbortSignal&>(signal_property_value_1.as_object());
    }

    options.signal = signal_value_1;

    }

    auto federated_property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        federated_property_value_2 = TRY(arg0.as_object().get("federated"_utf16_fly_string));

    if (!federated_property_value_2.is_undefined()) {

    if (!federated_property_value_2.is_nullish() && !federated_property_value_2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "FederatedCredentialRequestOptions");

    FederatedCredentialRequestOptions federated_value_2 {};

    auto protocols_property_value_0 = JS::js_undefined();
    if (federated_property_value_2.is_object())
        protocols_property_value_0 = TRY(federated_property_value_2.as_object().get("protocols"_utf16_fly_string));

    if (!protocols_property_value_0.is_undefined()) {

    Optional<Vector<String>> protocols_value_0;

    if (!protocols_property_value_0.is_undefined()) {

    if (!protocols_property_value_0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, protocols_property_value_0);

    auto protocols_property_value_0_iterator_method0 = TRY(protocols_property_value_0.get_method(vm, vm.well_known_symbol_iterator()));
    if (!protocols_property_value_0_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, protocols_property_value_0);

    auto protocols_property_value_0_iterator1 = TRY(JS::get_iterator_from_method(vm, protocols_property_value_0, *protocols_property_value_0_iterator_method0));

    Vector<String> protocols_value_0_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, protocols_property_value_0_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    String sequence_item1;
    if (!false || !next_item1.is_null()) {
        sequence_item1 = TRY(WebIDL::to_string(vm, next_item1));
    }

    protocols_value_0_non_optional.append(sequence_item1);
    }

        protocols_value_0 = move(protocols_value_0_non_optional);
    }

    federated_value_2.protocols = protocols_value_0;

    }

    auto providers_property_value_1 = JS::js_undefined();
    if (federated_property_value_2.is_object())
        providers_property_value_1 = TRY(federated_property_value_2.as_object().get("providers"_utf16_fly_string));

    if (!providers_property_value_1.is_undefined()) {

    Optional<Vector<String>> providers_value_1;

    if (!providers_property_value_1.is_undefined()) {

    if (!providers_property_value_1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, providers_property_value_1);

    auto providers_property_value_1_iterator_method0 = TRY(providers_property_value_1.get_method(vm, vm.well_known_symbol_iterator()));
    if (!providers_property_value_1_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, providers_property_value_1);

    auto providers_property_value_1_iterator1 = TRY(JS::get_iterator_from_method(vm, providers_property_value_1, *providers_property_value_1_iterator_method0));

    Vector<String> providers_value_1_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, providers_property_value_1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    String sequence_item1;
    if (!false || !next_item1.is_null()) {
        sequence_item1 = TRY(WebIDL::to_usv_string(vm, next_item1));
    }

    providers_value_1_non_optional.append(sequence_item1);
    }

        providers_value_1 = move(providers_value_1_non_optional);
    }

    federated_value_2.providers = providers_value_1;

    }

    options.federated = federated_value_2;

    }

    auto password_property_value_3 = JS::js_undefined();
    if (arg0.is_object())
        password_property_value_3 = TRY(arg0.as_object().get("password"_utf16_fly_string));

    bool password_value_3;

    if (!password_property_value_3.is_undefined())

    password_value_3 = password_property_value_3.to_boolean();

    else
        password_value_3 = static_cast<bool>(false);

    options.password = password_value_3;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get(options); }));

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

JS_DEFINE_NATIVE_FUNCTION(CredentialsContainerPrototype::store)
{
    WebIDL::log_trace(vm, "CredentialsContainerPrototype::store");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "store");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Credential>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Credential");

    auto& credential = static_cast<Credential&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->store(credential); }));

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

JS_DEFINE_NATIVE_FUNCTION(CredentialsContainerPrototype::create)
{
    WebIDL::log_trace(vm, "CredentialsContainerPrototype::create");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CredentialCreationOptions");

    CredentialCreationOptions options {};

    auto mediation_property_value_4 = JS::js_undefined();
    if (arg0.is_object())
        mediation_property_value_4 = TRY(arg0.as_object().get("mediation"_utf16_fly_string));

    CredentialMediationRequirement mediation_value_4 { CredentialMediationRequirement::Optional };

    if (!mediation_property_value_4.is_undefined()) {

    auto mediation_property_value_4_string = TRY(mediation_property_value_4.to_string(vm));

    if (mediation_property_value_4_string == "silent"sv)
        mediation_value_4 = CredentialMediationRequirement::Silent;

    else if (mediation_property_value_4_string == "optional"sv)
        mediation_value_4 = CredentialMediationRequirement::Optional;

    else if (mediation_property_value_4_string == "conditional"sv)
        mediation_value_4 = CredentialMediationRequirement::Conditional;

    else if (mediation_property_value_4_string == "required"sv)
        mediation_value_4 = CredentialMediationRequirement::Required;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, mediation_property_value_4_string, "CredentialMediationRequirement");

    }

    options.mediation = mediation_value_4;

    auto signal_property_value_5 = JS::js_undefined();
    if (arg0.is_object())
        signal_property_value_5 = TRY(arg0.as_object().get("signal"_utf16_fly_string));

    if (!signal_property_value_5.is_undefined()) {

    GC::Ptr<AbortSignal> signal_value_5;
    if (!signal_property_value_5.is_undefined()) {
        if (!signal_property_value_5.is_object() || !is<AbortSignal>(signal_property_value_5.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AbortSignal");

        signal_value_5 = static_cast<AbortSignal&>(signal_property_value_5.as_object());
    }

    options.signal = signal_value_5;

    }

    auto federated_property_value_6 = JS::js_undefined();
    if (arg0.is_object())
        federated_property_value_6 = TRY(arg0.as_object().get("federated"_utf16_fly_string));

    if (!federated_property_value_6.is_undefined()) {

    if (!federated_property_value_6.is_nullish() && !federated_property_value_6.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "FederatedCredentialInit");

    FederatedCredentialInit federated_value_6 {};

    auto icon_url_property_value_2 = JS::js_undefined();
    if (federated_property_value_6.is_object())
        icon_url_property_value_2 = TRY(federated_property_value_6.as_object().get("iconURL"_utf16_fly_string));

    if (!icon_url_property_value_2.is_undefined()) {

    Optional<String> icon_url_value_2;

    if (!icon_url_property_value_2.is_undefined()) {
        if (!false || !icon_url_property_value_2.is_null())
            icon_url_value_2 = TRY(WebIDL::to_usv_string(vm, icon_url_property_value_2));
    }

    if (icon_url_value_2.has_value())
        federated_value_6.icon_url = icon_url_value_2.release_value();

    }

    auto name_property_value_3 = JS::js_undefined();
    if (federated_property_value_6.is_object())
        name_property_value_3 = TRY(federated_property_value_6.as_object().get("name"_utf16_fly_string));

    if (!name_property_value_3.is_undefined()) {

    Optional<String> name_value_3;

    if (!name_property_value_3.is_undefined()) {
        if (!false || !name_property_value_3.is_null())
            name_value_3 = TRY(WebIDL::to_usv_string(vm, name_property_value_3));
    }

    if (name_value_3.has_value())
        federated_value_6.name = name_value_3.release_value();

    }

    auto protocol_property_value_4 = JS::js_undefined();
    if (federated_property_value_6.is_object())
        protocol_property_value_4 = TRY(federated_property_value_6.as_object().get("protocol"_utf16_fly_string));

    if (!protocol_property_value_4.is_undefined()) {

    Optional<String> protocol_value_4;

    if (!protocol_property_value_4.is_undefined()) {
        if (!false || !protocol_property_value_4.is_null())
            protocol_value_4 = TRY(WebIDL::to_string(vm, protocol_property_value_4));
    }

    if (protocol_value_4.has_value())
        federated_value_6.protocol = protocol_value_4.release_value();

    }

    auto provider_property_value_5 = JS::js_undefined();
    if (federated_property_value_6.is_object())
        provider_property_value_5 = TRY(federated_property_value_6.as_object().get("provider"_utf16_fly_string));

    if (provider_property_value_5.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "provider");

    String provider_value_5;
    if (!false || !provider_property_value_5.is_null()) {
        provider_value_5 = TRY(WebIDL::to_usv_string(vm, provider_property_value_5));
    }

    federated_value_6.provider = provider_value_5;

    auto id_property_value_6 = JS::js_undefined();
    if (federated_property_value_6.is_object())
        id_property_value_6 = TRY(federated_property_value_6.as_object().get("id"_utf16_fly_string));

    if (id_property_value_6.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "id");

    String id_value_6;
    if (!false || !id_property_value_6.is_null()) {
        id_value_6 = TRY(WebIDL::to_usv_string(vm, id_property_value_6));
    }

    federated_value_6.id = id_value_6;

    options.federated = federated_value_6;

    }

    auto password_property_value_7 = JS::js_undefined();
    if (arg0.is_object())
        password_property_value_7 = TRY(arg0.as_object().get("password"_utf16_fly_string));

    if (!password_property_value_7.is_undefined()) {

    auto password_property_value_7_to_dictionary = [&vm, &realm](JS::Value password_property_value_7) -> JS::ThrowCompletionOr<PasswordCredentialData> {
        // This might be unused.
        (void)realm;

    if (!password_property_value_7.is_nullish() && !password_property_value_7.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PasswordCredentialData");

    PasswordCredentialData dictionary_union_type {};

    auto icon_url_property_value_0 = JS::js_undefined();
    if (password_property_value_7.is_object())
        icon_url_property_value_0 = TRY(password_property_value_7.as_object().get("iconURL"_utf16_fly_string));

    if (!icon_url_property_value_0.is_undefined()) {

    Optional<String> icon_url_value_0;

    if (!icon_url_property_value_0.is_undefined()) {
        if (!false || !icon_url_property_value_0.is_null())
            icon_url_value_0 = TRY(WebIDL::to_usv_string(vm, icon_url_property_value_0));
    }

    dictionary_union_type.icon_url = icon_url_value_0;

    }

    auto name_property_value_1 = JS::js_undefined();
    if (password_property_value_7.is_object())
        name_property_value_1 = TRY(password_property_value_7.as_object().get("name"_utf16_fly_string));

    if (!name_property_value_1.is_undefined()) {

    Optional<String> name_value_1;

    if (!name_property_value_1.is_undefined()) {
        if (!false || !name_property_value_1.is_null())
            name_value_1 = TRY(WebIDL::to_usv_string(vm, name_property_value_1));
    }

    dictionary_union_type.name = name_value_1;

    }

    auto password_property_value_2 = JS::js_undefined();
    if (password_property_value_7.is_object())
        password_property_value_2 = TRY(password_property_value_7.as_object().get("password"_utf16_fly_string));

    if (password_property_value_2.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "password");

    String password_value_2;
    if (!false || !password_property_value_2.is_null()) {
        password_value_2 = TRY(WebIDL::to_usv_string(vm, password_property_value_2));
    }

    dictionary_union_type.password = password_value_2;

    auto id_property_value_3 = JS::js_undefined();
    if (password_property_value_7.is_object())
        id_property_value_3 = TRY(password_property_value_7.as_object().get("id"_utf16_fly_string));

    if (id_property_value_3.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "id");

    String id_value_3;
    if (!false || !id_property_value_3.is_null()) {
        id_value_3 = TRY(WebIDL::to_usv_string(vm, id_property_value_3));
    }

    dictionary_union_type.id = id_value_3;

        return dictionary_union_type;
    };

    auto password_property_value_7_to_variant = [&vm, &realm, &password_property_value_7_to_dictionary](JS::Value password_property_value_7) -> JS::ThrowCompletionOr<Variant<PasswordCredentialData, GC::Root<HTMLFormElement>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (password_property_value_7.is_nullish())
            return Variant<PasswordCredentialData, GC::Root<HTMLFormElement>> { TRY(password_property_value_7_to_dictionary(password_property_value_7)) };

        if (password_property_value_7.is_object()) {
            [[maybe_unused]] auto& password_property_value_7_object = password_property_value_7.as_object();

            if (is<PlatformObject>(password_property_value_7_object)) {

                if (auto* password_property_value_7_result = as_if<HTMLFormElement>(password_property_value_7_object))
                    return GC::make_root(*password_property_value_7_result);

            }

        return Variant<PasswordCredentialData, GC::Root<HTMLFormElement>> { TRY(password_property_value_7_to_dictionary(password_property_value_7)) };

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Optional<Variant<PasswordCredentialData, GC::Root<HTMLFormElement>>> password_value_7;
    if (!password_property_value_7.is_undefined())
        password_value_7 = TRY(password_property_value_7_to_variant(password_property_value_7));

    options.password = password_value_7;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create(options); }));

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

JS_DEFINE_NATIVE_FUNCTION(CredentialsContainerPrototype::prevent_silent_access)
{
    WebIDL::log_trace(vm, "CredentialsContainerPrototype::prevent_silent_access");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->prevent_silent_access(); }));

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
