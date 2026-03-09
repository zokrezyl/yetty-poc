
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
#include <LibWeb/Bindings/SubtleCryptoPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Crypto/SubtleCrypto.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Crypto/CryptoKey.h>

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

GC_DEFINE_ALLOCATOR(SubtleCryptoPrototype);

SubtleCryptoPrototype::SubtleCryptoPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

SubtleCryptoPrototype::~SubtleCryptoPrototype()
{
}

void SubtleCryptoPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    define_native_function(realm, "encrypt"_utf16_fly_string, encrypt, 3, default_attributes);

    define_native_function(realm, "decrypt"_utf16_fly_string, decrypt, 3, default_attributes);

    define_native_function(realm, "sign"_utf16_fly_string, sign, 3, default_attributes);

    define_native_function(realm, "verify"_utf16_fly_string, verify, 4, default_attributes);

    define_native_function(realm, "digest"_utf16_fly_string, digest, 2, default_attributes);

    define_native_function(realm, "generateKey"_utf16_fly_string, generate_key, 3, default_attributes);

    define_native_function(realm, "deriveKey"_utf16_fly_string, derive_key, 5, default_attributes);

    define_native_function(realm, "deriveBits"_utf16_fly_string, derive_bits, 2, default_attributes);

    define_native_function(realm, "importKey"_utf16_fly_string, import_key, 5, default_attributes);

    define_native_function(realm, "exportKey"_utf16_fly_string, export_key, 2, default_attributes);

    define_native_function(realm, "wrapKey"_utf16_fly_string, wrap_key, 4, default_attributes);

    define_native_function(realm, "unwrapKey"_utf16_fly_string, unwrap_key, 7, default_attributes);

    define_native_function(realm, "encapsulateKey"_utf16_fly_string, encapsulate_key, 5, default_attributes);

    define_native_function(realm, "encapsulateBits"_utf16_fly_string, encapsulate_bits, 2, default_attributes);

    define_native_function(realm, "decapsulateKey"_utf16_fly_string, decapsulate_key, 6, default_attributes);

    define_native_function(realm, "decapsulateBits"_utf16_fly_string, decapsulate_bits, 3, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "SubtleCrypto"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void SubtleCryptoPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Crypto::SubtleCrypto*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Crypto::SubtleCrypto>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SubtleCrypto");
    return static_cast<Crypto::SubtleCrypto*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(SubtleCryptoPrototype::encrypt)
{
    WebIDL::log_trace(vm, "SubtleCryptoPrototype::encrypt");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "encrypt", "3");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<JS::ArrayBuffer>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::DataView>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::TypedArrayBase>(arg0_object))
                return GC::make_root(arg0_object);

        return arg0_object;

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<GC::Root<JS::Object>, String> algorithm = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<CryptoKey>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CryptoKey");

    auto& key = static_cast<CryptoKey&>(arg1.as_object());

    auto arg2 = vm.argument(2);

    GC::Root<WebIDL::BufferSource> data;

    if (!arg2.is_object() || !(is<JS::TypedArrayBase>(arg2.as_object()) || is<JS::ArrayBuffer>(arg2.as_object()) || is<JS::DataView>(arg2.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BufferSource");

    data = GC::make_root(realm.create<WebIDL::BufferSource>(arg2.as_object()));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->encrypt(algorithm, key, data); }));

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

JS_DEFINE_NATIVE_FUNCTION(SubtleCryptoPrototype::decrypt)
{
    WebIDL::log_trace(vm, "SubtleCryptoPrototype::decrypt");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "decrypt", "3");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<JS::ArrayBuffer>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::DataView>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::TypedArrayBase>(arg0_object))
                return GC::make_root(arg0_object);

        return arg0_object;

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<GC::Root<JS::Object>, String> algorithm = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<CryptoKey>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CryptoKey");

    auto& key = static_cast<CryptoKey&>(arg1.as_object());

    auto arg2 = vm.argument(2);

    GC::Root<WebIDL::BufferSource> data;

    if (!arg2.is_object() || !(is<JS::TypedArrayBase>(arg2.as_object()) || is<JS::ArrayBuffer>(arg2.as_object()) || is<JS::DataView>(arg2.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BufferSource");

    data = GC::make_root(realm.create<WebIDL::BufferSource>(arg2.as_object()));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->decrypt(algorithm, key, data); }));

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

JS_DEFINE_NATIVE_FUNCTION(SubtleCryptoPrototype::sign)
{
    WebIDL::log_trace(vm, "SubtleCryptoPrototype::sign");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "sign", "3");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<JS::ArrayBuffer>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::DataView>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::TypedArrayBase>(arg0_object))
                return GC::make_root(arg0_object);

        return arg0_object;

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<GC::Root<JS::Object>, String> algorithm = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<CryptoKey>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CryptoKey");

    auto& key = static_cast<CryptoKey&>(arg1.as_object());

    auto arg2 = vm.argument(2);

    GC::Root<WebIDL::BufferSource> data;

    if (!arg2.is_object() || !(is<JS::TypedArrayBase>(arg2.as_object()) || is<JS::ArrayBuffer>(arg2.as_object()) || is<JS::DataView>(arg2.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BufferSource");

    data = GC::make_root(realm.create<WebIDL::BufferSource>(arg2.as_object()));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->sign(algorithm, key, data); }));

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

JS_DEFINE_NATIVE_FUNCTION(SubtleCryptoPrototype::verify)
{
    WebIDL::log_trace(vm, "SubtleCryptoPrototype::verify");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "verify", "4");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<JS::ArrayBuffer>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::DataView>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::TypedArrayBase>(arg0_object))
                return GC::make_root(arg0_object);

        return arg0_object;

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<GC::Root<JS::Object>, String> algorithm = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<CryptoKey>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CryptoKey");

    auto& key = static_cast<CryptoKey&>(arg1.as_object());

    auto arg2 = vm.argument(2);

    GC::Root<WebIDL::BufferSource> signature;

    if (!arg2.is_object() || !(is<JS::TypedArrayBase>(arg2.as_object()) || is<JS::ArrayBuffer>(arg2.as_object()) || is<JS::DataView>(arg2.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BufferSource");

    signature = GC::make_root(realm.create<WebIDL::BufferSource>(arg2.as_object()));

    auto arg3 = vm.argument(3);

    GC::Root<WebIDL::BufferSource> data;

    if (!arg3.is_object() || !(is<JS::TypedArrayBase>(arg3.as_object()) || is<JS::ArrayBuffer>(arg3.as_object()) || is<JS::DataView>(arg3.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BufferSource");

    data = GC::make_root(realm.create<WebIDL::BufferSource>(arg3.as_object()));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->verify(algorithm, key, signature, data); }));

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

JS_DEFINE_NATIVE_FUNCTION(SubtleCryptoPrototype::digest)
{
    WebIDL::log_trace(vm, "SubtleCryptoPrototype::digest");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "digest", "2");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<JS::ArrayBuffer>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::DataView>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::TypedArrayBase>(arg0_object))
                return GC::make_root(arg0_object);

        return arg0_object;

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<GC::Root<JS::Object>, String> algorithm = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    GC::Root<WebIDL::BufferSource> data;

    if (!arg1.is_object() || !(is<JS::TypedArrayBase>(arg1.as_object()) || is<JS::ArrayBuffer>(arg1.as_object()) || is<JS::DataView>(arg1.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BufferSource");

    data = GC::make_root(realm.create<WebIDL::BufferSource>(arg1.as_object()));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->digest(algorithm, data); }));

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

JS_DEFINE_NATIVE_FUNCTION(SubtleCryptoPrototype::generate_key)
{
    WebIDL::log_trace(vm, "SubtleCryptoPrototype::generate_key");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "generateKey", "3");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<JS::ArrayBuffer>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::DataView>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::TypedArrayBase>(arg0_object))
                return GC::make_root(arg0_object);

        return arg0_object;

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<GC::Root<JS::Object>, String> algorithm = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    bool extractable;

    extractable = arg1.to_boolean();

    auto arg2 = vm.argument(2);

    if (!arg2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg2);

    auto arg2_iterator_method0 = TRY(arg2.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg2_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg2);

    auto arg2_iterator1 = TRY(JS::get_iterator_from_method(vm, arg2, *arg2_iterator_method0));

    Vector<KeyUsage> key_usages;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    KeyUsage sequence_item1 { KeyUsage::Encrypt };

    auto next_item1_string = TRY(next_item1.to_string(vm));

    if (next_item1_string == "encrypt"sv)
        sequence_item1 = KeyUsage::Encrypt;

    else if (next_item1_string == "decrypt"sv)
        sequence_item1 = KeyUsage::Decrypt;

    else if (next_item1_string == "sign"sv)
        sequence_item1 = KeyUsage::Sign;

    else if (next_item1_string == "verify"sv)
        sequence_item1 = KeyUsage::Verify;

    else if (next_item1_string == "deriveKey"sv)
        sequence_item1 = KeyUsage::Derivekey;

    else if (next_item1_string == "deriveBits"sv)
        sequence_item1 = KeyUsage::Derivebits;

    else if (next_item1_string == "wrapKey"sv)
        sequence_item1 = KeyUsage::Wrapkey;

    else if (next_item1_string == "unwrapKey"sv)
        sequence_item1 = KeyUsage::Unwrapkey;

    else if (next_item1_string == "encapsulateKey"sv)
        sequence_item1 = KeyUsage::Encapsulatekey;

    else if (next_item1_string == "encapsulateBits"sv)
        sequence_item1 = KeyUsage::Encapsulatebits;

    else if (next_item1_string == "decapsulateKey"sv)
        sequence_item1 = KeyUsage::Decapsulatekey;

    else if (next_item1_string == "decapsulateBits"sv)
        sequence_item1 = KeyUsage::Decapsulatebits;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, next_item1_string, "KeyUsage");

    key_usages.append(sequence_item1);
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->generate_key(algorithm, extractable, key_usages); }));

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

JS_DEFINE_NATIVE_FUNCTION(SubtleCryptoPrototype::derive_key)
{
    WebIDL::log_trace(vm, "SubtleCryptoPrototype::derive_key");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "deriveKey", "5");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<JS::ArrayBuffer>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::DataView>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::TypedArrayBase>(arg0_object))
                return GC::make_root(arg0_object);

        return arg0_object;

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<GC::Root<JS::Object>, String> algorithm = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<CryptoKey>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CryptoKey");

    auto& base_key = static_cast<CryptoKey&>(arg1.as_object());

    auto arg2 = vm.argument(2);

    auto arg2_to_variant = [&vm, &realm](JS::Value arg2) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg2.is_object()) {
            [[maybe_unused]] auto& arg2_object = arg2.as_object();

            if (is<JS::ArrayBuffer>(arg2_object))
                return GC::make_root(arg2_object);

            if (is<JS::DataView>(arg2_object))
                return GC::make_root(arg2_object);

            if (is<JS::TypedArrayBase>(arg2_object))
                return GC::make_root(arg2_object);

        return arg2_object;

        }

    String arg2_string;
    if (!false || !arg2.is_null()) {
        arg2_string = TRY(WebIDL::to_string(vm, arg2));
    }

        return { arg2_string };

    };

    Variant<GC::Root<JS::Object>, String> derived_key_type = TRY(arg2_to_variant(arg2));

    auto arg3 = vm.argument(3);

    bool extractable;

    extractable = arg3.to_boolean();

    auto arg4 = vm.argument(4);

    if (!arg4.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg4);

    auto arg4_iterator_method0 = TRY(arg4.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg4_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg4);

    auto arg4_iterator1 = TRY(JS::get_iterator_from_method(vm, arg4, *arg4_iterator_method0));

    Vector<KeyUsage> key_usages;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg4_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    KeyUsage sequence_item1 { KeyUsage::Encrypt };

    auto next_item1_string = TRY(next_item1.to_string(vm));

    if (next_item1_string == "encrypt"sv)
        sequence_item1 = KeyUsage::Encrypt;

    else if (next_item1_string == "decrypt"sv)
        sequence_item1 = KeyUsage::Decrypt;

    else if (next_item1_string == "sign"sv)
        sequence_item1 = KeyUsage::Sign;

    else if (next_item1_string == "verify"sv)
        sequence_item1 = KeyUsage::Verify;

    else if (next_item1_string == "deriveKey"sv)
        sequence_item1 = KeyUsage::Derivekey;

    else if (next_item1_string == "deriveBits"sv)
        sequence_item1 = KeyUsage::Derivebits;

    else if (next_item1_string == "wrapKey"sv)
        sequence_item1 = KeyUsage::Wrapkey;

    else if (next_item1_string == "unwrapKey"sv)
        sequence_item1 = KeyUsage::Unwrapkey;

    else if (next_item1_string == "encapsulateKey"sv)
        sequence_item1 = KeyUsage::Encapsulatekey;

    else if (next_item1_string == "encapsulateBits"sv)
        sequence_item1 = KeyUsage::Encapsulatebits;

    else if (next_item1_string == "decapsulateKey"sv)
        sequence_item1 = KeyUsage::Decapsulatekey;

    else if (next_item1_string == "decapsulateBits"sv)
        sequence_item1 = KeyUsage::Decapsulatebits;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, next_item1_string, "KeyUsage");

    key_usages.append(sequence_item1);
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->derive_key(algorithm, base_key, derived_key_type, extractable, key_usages); }));

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

JS_DEFINE_NATIVE_FUNCTION(SubtleCryptoPrototype::derive_bits)
{
    WebIDL::log_trace(vm, "SubtleCryptoPrototype::derive_bits");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "deriveBits", "2");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<JS::ArrayBuffer>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::DataView>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::TypedArrayBase>(arg0_object))
                return GC::make_root(arg0_object);

        return arg0_object;

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<GC::Root<JS::Object>, String> algorithm = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<CryptoKey>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CryptoKey");

    auto& base_key = static_cast<CryptoKey&>(arg1.as_object());

    auto arg2 = vm.argument(2);

    Optional<WebIDL::UnsignedLong> length;

    if (!arg2.is_null() && !arg2.is_undefined())

    length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->derive_bits(algorithm, base_key, length); }));

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

JS_DEFINE_NATIVE_FUNCTION(SubtleCryptoPrototype::import_key)
{
    WebIDL::log_trace(vm, "SubtleCryptoPrototype::import_key");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "importKey", "5");

    auto arg0 = vm.argument(0);

    KeyFormat format { KeyFormat::RawPublic };

    auto arg0_string = TRY(arg0.to_string(vm));

    if (arg0_string == "raw-public"sv)
        format = KeyFormat::RawPublic;

    else if (arg0_string == "raw-private"sv)
        format = KeyFormat::RawPrivate;

    else if (arg0_string == "raw-seed"sv)
        format = KeyFormat::RawSeed;

    else if (arg0_string == "raw-secret"sv)
        format = KeyFormat::RawSecret;

    else if (arg0_string == "raw"sv)
        format = KeyFormat::Raw;

    else if (arg0_string == "spki"sv)
        format = KeyFormat::Spki;

    else if (arg0_string == "pkcs8"sv)
        format = KeyFormat::Pkcs8;

    else if (arg0_string == "jwk"sv)
        format = KeyFormat::Jwk;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, arg0_string, "KeyFormat");

    auto arg1 = vm.argument(1);

    auto arg1_to_dictionary = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<JsonWebKey> {
        // This might be unused.
        (void)realm;

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "JsonWebKey");

    JsonWebKey dictionary_union_type {};

    auto alg_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        alg_property_value_0 = TRY(arg1.as_object().get("alg"_utf16_fly_string));

    if (!alg_property_value_0.is_undefined()) {

    Optional<String> alg_value_0;

    if (!alg_property_value_0.is_undefined()) {
        if (!false || !alg_property_value_0.is_null())
            alg_value_0 = TRY(WebIDL::to_string(vm, alg_property_value_0));
    }

    dictionary_union_type.alg = alg_value_0;

    }

    auto crv_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        crv_property_value_1 = TRY(arg1.as_object().get("crv"_utf16_fly_string));

    if (!crv_property_value_1.is_undefined()) {

    Optional<String> crv_value_1;

    if (!crv_property_value_1.is_undefined()) {
        if (!false || !crv_property_value_1.is_null())
            crv_value_1 = TRY(WebIDL::to_string(vm, crv_property_value_1));
    }

    dictionary_union_type.crv = crv_value_1;

    }

    auto d_property_value_2 = JS::js_undefined();
    if (arg1.is_object())
        d_property_value_2 = TRY(arg1.as_object().get("d"_utf16_fly_string));

    if (!d_property_value_2.is_undefined()) {

    Optional<String> d_value_2;

    if (!d_property_value_2.is_undefined()) {
        if (!false || !d_property_value_2.is_null())
            d_value_2 = TRY(WebIDL::to_string(vm, d_property_value_2));
    }

    dictionary_union_type.d = d_value_2;

    }

    auto dp_property_value_3 = JS::js_undefined();
    if (arg1.is_object())
        dp_property_value_3 = TRY(arg1.as_object().get("dp"_utf16_fly_string));

    if (!dp_property_value_3.is_undefined()) {

    Optional<String> dp_value_3;

    if (!dp_property_value_3.is_undefined()) {
        if (!false || !dp_property_value_3.is_null())
            dp_value_3 = TRY(WebIDL::to_string(vm, dp_property_value_3));
    }

    dictionary_union_type.dp = dp_value_3;

    }

    auto dq_property_value_4 = JS::js_undefined();
    if (arg1.is_object())
        dq_property_value_4 = TRY(arg1.as_object().get("dq"_utf16_fly_string));

    if (!dq_property_value_4.is_undefined()) {

    Optional<String> dq_value_4;

    if (!dq_property_value_4.is_undefined()) {
        if (!false || !dq_property_value_4.is_null())
            dq_value_4 = TRY(WebIDL::to_string(vm, dq_property_value_4));
    }

    dictionary_union_type.dq = dq_value_4;

    }

    auto e_property_value_5 = JS::js_undefined();
    if (arg1.is_object())
        e_property_value_5 = TRY(arg1.as_object().get("e"_utf16_fly_string));

    if (!e_property_value_5.is_undefined()) {

    Optional<String> e_value_5;

    if (!e_property_value_5.is_undefined()) {
        if (!false || !e_property_value_5.is_null())
            e_value_5 = TRY(WebIDL::to_string(vm, e_property_value_5));
    }

    dictionary_union_type.e = e_value_5;

    }

    auto ext_property_value_6 = JS::js_undefined();
    if (arg1.is_object())
        ext_property_value_6 = TRY(arg1.as_object().get("ext"_utf16_fly_string));

    if (!ext_property_value_6.is_undefined()) {

    Optional<bool> ext_value_6;

    if (!ext_property_value_6.is_undefined())

    ext_value_6 = ext_property_value_6.to_boolean();

    dictionary_union_type.ext = ext_value_6;

    }

    auto k_property_value_7 = JS::js_undefined();
    if (arg1.is_object())
        k_property_value_7 = TRY(arg1.as_object().get("k"_utf16_fly_string));

    if (!k_property_value_7.is_undefined()) {

    Optional<String> k_value_7;

    if (!k_property_value_7.is_undefined()) {
        if (!false || !k_property_value_7.is_null())
            k_value_7 = TRY(WebIDL::to_string(vm, k_property_value_7));
    }

    dictionary_union_type.k = k_value_7;

    }

    auto key_ops_property_value_8 = JS::js_undefined();
    if (arg1.is_object())
        key_ops_property_value_8 = TRY(arg1.as_object().get("key_ops"_utf16_fly_string));

    if (!key_ops_property_value_8.is_undefined()) {

    Optional<Vector<String>> key_ops_value_8;

    if (!key_ops_property_value_8.is_undefined()) {

    if (!key_ops_property_value_8.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, key_ops_property_value_8);

    auto key_ops_property_value_8_iterator_method0 = TRY(key_ops_property_value_8.get_method(vm, vm.well_known_symbol_iterator()));
    if (!key_ops_property_value_8_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, key_ops_property_value_8);

    auto key_ops_property_value_8_iterator1 = TRY(JS::get_iterator_from_method(vm, key_ops_property_value_8, *key_ops_property_value_8_iterator_method0));

    Vector<String> key_ops_value_8_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, key_ops_property_value_8_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    String sequence_item1;
    if (!false || !next_item1.is_null()) {
        sequence_item1 = TRY(WebIDL::to_string(vm, next_item1));
    }

    key_ops_value_8_non_optional.append(sequence_item1);
    }

        key_ops_value_8 = move(key_ops_value_8_non_optional);
    }

    dictionary_union_type.key_ops = key_ops_value_8;

    }

    auto kty_property_value_9 = JS::js_undefined();
    if (arg1.is_object())
        kty_property_value_9 = TRY(arg1.as_object().get("kty"_utf16_fly_string));

    if (!kty_property_value_9.is_undefined()) {

    Optional<String> kty_value_9;

    if (!kty_property_value_9.is_undefined()) {
        if (!false || !kty_property_value_9.is_null())
            kty_value_9 = TRY(WebIDL::to_string(vm, kty_property_value_9));
    }

    dictionary_union_type.kty = kty_value_9;

    }

    auto n_property_value_10 = JS::js_undefined();
    if (arg1.is_object())
        n_property_value_10 = TRY(arg1.as_object().get("n"_utf16_fly_string));

    if (!n_property_value_10.is_undefined()) {

    Optional<String> n_value_10;

    if (!n_property_value_10.is_undefined()) {
        if (!false || !n_property_value_10.is_null())
            n_value_10 = TRY(WebIDL::to_string(vm, n_property_value_10));
    }

    dictionary_union_type.n = n_value_10;

    }

    auto oth_property_value_11 = JS::js_undefined();
    if (arg1.is_object())
        oth_property_value_11 = TRY(arg1.as_object().get("oth"_utf16_fly_string));

    if (!oth_property_value_11.is_undefined()) {

    Optional<Vector<RsaOtherPrimesInfo>> oth_value_11;

    if (!oth_property_value_11.is_undefined()) {

    if (!oth_property_value_11.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, oth_property_value_11);

    auto oth_property_value_11_iterator_method0 = TRY(oth_property_value_11.get_method(vm, vm.well_known_symbol_iterator()));
    if (!oth_property_value_11_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, oth_property_value_11);

    auto oth_property_value_11_iterator1 = TRY(JS::get_iterator_from_method(vm, oth_property_value_11, *oth_property_value_11_iterator_method0));

    Vector<RsaOtherPrimesInfo> oth_value_11_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, oth_property_value_11_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_nullish() && !next_item1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "RsaOtherPrimesInfo");

    RsaOtherPrimesInfo sequence_item1 {};

    auto d_property_value_11 = JS::js_undefined();
    if (next_item1.is_object())
        d_property_value_11 = TRY(next_item1.as_object().get("d"_utf16_fly_string));

    if (!d_property_value_11.is_undefined()) {

    Optional<String> d_value_11;

    if (!d_property_value_11.is_undefined()) {
        if (!false || !d_property_value_11.is_null())
            d_value_11 = TRY(WebIDL::to_string(vm, d_property_value_11));
    }

    sequence_item1.d = d_value_11;

    }

    auto r_property_value_12 = JS::js_undefined();
    if (next_item1.is_object())
        r_property_value_12 = TRY(next_item1.as_object().get("r"_utf16_fly_string));

    if (!r_property_value_12.is_undefined()) {

    Optional<String> r_value_12;

    if (!r_property_value_12.is_undefined()) {
        if (!false || !r_property_value_12.is_null())
            r_value_12 = TRY(WebIDL::to_string(vm, r_property_value_12));
    }

    sequence_item1.r = r_value_12;

    }

    auto t_property_value_13 = JS::js_undefined();
    if (next_item1.is_object())
        t_property_value_13 = TRY(next_item1.as_object().get("t"_utf16_fly_string));

    if (!t_property_value_13.is_undefined()) {

    Optional<String> t_value_13;

    if (!t_property_value_13.is_undefined()) {
        if (!false || !t_property_value_13.is_null())
            t_value_13 = TRY(WebIDL::to_string(vm, t_property_value_13));
    }

    sequence_item1.t = t_value_13;

    }

    oth_value_11_non_optional.append(sequence_item1);
    }

        oth_value_11 = move(oth_value_11_non_optional);
    }

    dictionary_union_type.oth = oth_value_11;

    }

    auto p_property_value_15 = JS::js_undefined();
    if (arg1.is_object())
        p_property_value_15 = TRY(arg1.as_object().get("p"_utf16_fly_string));

    if (!p_property_value_15.is_undefined()) {

    Optional<String> p_value_15;

    if (!p_property_value_15.is_undefined()) {
        if (!false || !p_property_value_15.is_null())
            p_value_15 = TRY(WebIDL::to_string(vm, p_property_value_15));
    }

    dictionary_union_type.p = p_value_15;

    }

    auto priv_property_value_16 = JS::js_undefined();
    if (arg1.is_object())
        priv_property_value_16 = TRY(arg1.as_object().get("priv"_utf16_fly_string));

    if (!priv_property_value_16.is_undefined()) {

    Optional<String> priv_value_16;

    if (!priv_property_value_16.is_undefined()) {
        if (!false || !priv_property_value_16.is_null())
            priv_value_16 = TRY(WebIDL::to_string(vm, priv_property_value_16));
    }

    dictionary_union_type.priv = priv_value_16;

    }

    auto pub_property_value_17 = JS::js_undefined();
    if (arg1.is_object())
        pub_property_value_17 = TRY(arg1.as_object().get("pub"_utf16_fly_string));

    if (!pub_property_value_17.is_undefined()) {

    Optional<String> pub_value_17;

    if (!pub_property_value_17.is_undefined()) {
        if (!false || !pub_property_value_17.is_null())
            pub_value_17 = TRY(WebIDL::to_string(vm, pub_property_value_17));
    }

    dictionary_union_type.pub = pub_value_17;

    }

    auto q_property_value_18 = JS::js_undefined();
    if (arg1.is_object())
        q_property_value_18 = TRY(arg1.as_object().get("q"_utf16_fly_string));

    if (!q_property_value_18.is_undefined()) {

    Optional<String> q_value_18;

    if (!q_property_value_18.is_undefined()) {
        if (!false || !q_property_value_18.is_null())
            q_value_18 = TRY(WebIDL::to_string(vm, q_property_value_18));
    }

    dictionary_union_type.q = q_value_18;

    }

    auto qi_property_value_19 = JS::js_undefined();
    if (arg1.is_object())
        qi_property_value_19 = TRY(arg1.as_object().get("qi"_utf16_fly_string));

    if (!qi_property_value_19.is_undefined()) {

    Optional<String> qi_value_19;

    if (!qi_property_value_19.is_undefined()) {
        if (!false || !qi_property_value_19.is_null())
            qi_value_19 = TRY(WebIDL::to_string(vm, qi_property_value_19));
    }

    dictionary_union_type.qi = qi_value_19;

    }

    auto use_property_value_20 = JS::js_undefined();
    if (arg1.is_object())
        use_property_value_20 = TRY(arg1.as_object().get("use"_utf16_fly_string));

    if (!use_property_value_20.is_undefined()) {

    Optional<String> use_value_20;

    if (!use_property_value_20.is_undefined()) {
        if (!false || !use_property_value_20.is_null())
            use_value_20 = TRY(WebIDL::to_string(vm, use_property_value_20));
    }

    dictionary_union_type.use = use_value_20;

    }

    auto x_property_value_21 = JS::js_undefined();
    if (arg1.is_object())
        x_property_value_21 = TRY(arg1.as_object().get("x"_utf16_fly_string));

    if (!x_property_value_21.is_undefined()) {

    Optional<String> x_value_21;

    if (!x_property_value_21.is_undefined()) {
        if (!false || !x_property_value_21.is_null())
            x_value_21 = TRY(WebIDL::to_string(vm, x_property_value_21));
    }

    dictionary_union_type.x = x_value_21;

    }

    auto y_property_value_22 = JS::js_undefined();
    if (arg1.is_object())
        y_property_value_22 = TRY(arg1.as_object().get("y"_utf16_fly_string));

    if (!y_property_value_22.is_undefined()) {

    Optional<String> y_value_22;

    if (!y_property_value_22.is_undefined()) {
        if (!false || !y_property_value_22.is_null())
            y_value_22 = TRY(WebIDL::to_string(vm, y_property_value_22));
    }

    dictionary_union_type.y = y_value_22;

    }

        return dictionary_union_type;
    };

    auto arg1_to_variant = [&vm, &realm, &arg1_to_dictionary](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<WebIDL::BufferSource>, JsonWebKey>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_nullish())
            return Variant<GC::Root<WebIDL::BufferSource>, JsonWebKey> { TRY(arg1_to_dictionary(arg1)) };

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (is<JS::ArrayBuffer>(arg1_object) || is<JS::DataView>(arg1_object) || is<JS::TypedArrayBase>(arg1_object)) {
                GC::Ref<WebIDL::BufferSource> source_object = realm.create<WebIDL::BufferSource>(arg1_object);
                return GC::make_root(source_object);
            }

        return Variant<GC::Root<WebIDL::BufferSource>, JsonWebKey> { TRY(arg1_to_dictionary(arg1)) };

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<WebIDL::BufferSource>, JsonWebKey> key_data = TRY(arg1_to_variant(arg1));

    auto arg2 = vm.argument(2);

    auto arg2_to_variant = [&vm, &realm](JS::Value arg2) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg2.is_object()) {
            [[maybe_unused]] auto& arg2_object = arg2.as_object();

            if (is<JS::ArrayBuffer>(arg2_object))
                return GC::make_root(arg2_object);

            if (is<JS::DataView>(arg2_object))
                return GC::make_root(arg2_object);

            if (is<JS::TypedArrayBase>(arg2_object))
                return GC::make_root(arg2_object);

        return arg2_object;

        }

    String arg2_string;
    if (!false || !arg2.is_null()) {
        arg2_string = TRY(WebIDL::to_string(vm, arg2));
    }

        return { arg2_string };

    };

    Variant<GC::Root<JS::Object>, String> algorithm = TRY(arg2_to_variant(arg2));

    auto arg3 = vm.argument(3);

    bool extractable;

    extractable = arg3.to_boolean();

    auto arg4 = vm.argument(4);

    if (!arg4.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg4);

    auto arg4_iterator_method0 = TRY(arg4.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg4_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg4);

    auto arg4_iterator1 = TRY(JS::get_iterator_from_method(vm, arg4, *arg4_iterator_method0));

    Vector<KeyUsage> key_usages;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg4_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    KeyUsage sequence_item1 { KeyUsage::Encrypt };

    auto next_item1_string = TRY(next_item1.to_string(vm));

    if (next_item1_string == "encrypt"sv)
        sequence_item1 = KeyUsage::Encrypt;

    else if (next_item1_string == "decrypt"sv)
        sequence_item1 = KeyUsage::Decrypt;

    else if (next_item1_string == "sign"sv)
        sequence_item1 = KeyUsage::Sign;

    else if (next_item1_string == "verify"sv)
        sequence_item1 = KeyUsage::Verify;

    else if (next_item1_string == "deriveKey"sv)
        sequence_item1 = KeyUsage::Derivekey;

    else if (next_item1_string == "deriveBits"sv)
        sequence_item1 = KeyUsage::Derivebits;

    else if (next_item1_string == "wrapKey"sv)
        sequence_item1 = KeyUsage::Wrapkey;

    else if (next_item1_string == "unwrapKey"sv)
        sequence_item1 = KeyUsage::Unwrapkey;

    else if (next_item1_string == "encapsulateKey"sv)
        sequence_item1 = KeyUsage::Encapsulatekey;

    else if (next_item1_string == "encapsulateBits"sv)
        sequence_item1 = KeyUsage::Encapsulatebits;

    else if (next_item1_string == "decapsulateKey"sv)
        sequence_item1 = KeyUsage::Decapsulatekey;

    else if (next_item1_string == "decapsulateBits"sv)
        sequence_item1 = KeyUsage::Decapsulatebits;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, next_item1_string, "KeyUsage");

    key_usages.append(sequence_item1);
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->import_key(format, key_data, algorithm, extractable, key_usages); }));

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

JS_DEFINE_NATIVE_FUNCTION(SubtleCryptoPrototype::export_key)
{
    WebIDL::log_trace(vm, "SubtleCryptoPrototype::export_key");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "exportKey", "2");

    auto arg0 = vm.argument(0);

    KeyFormat format { KeyFormat::RawPublic };

    auto arg0_string = TRY(arg0.to_string(vm));

    if (arg0_string == "raw-public"sv)
        format = KeyFormat::RawPublic;

    else if (arg0_string == "raw-private"sv)
        format = KeyFormat::RawPrivate;

    else if (arg0_string == "raw-seed"sv)
        format = KeyFormat::RawSeed;

    else if (arg0_string == "raw-secret"sv)
        format = KeyFormat::RawSecret;

    else if (arg0_string == "raw"sv)
        format = KeyFormat::Raw;

    else if (arg0_string == "spki"sv)
        format = KeyFormat::Spki;

    else if (arg0_string == "pkcs8"sv)
        format = KeyFormat::Pkcs8;

    else if (arg0_string == "jwk"sv)
        format = KeyFormat::Jwk;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, arg0_string, "KeyFormat");

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<CryptoKey>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CryptoKey");

    auto& key = static_cast<CryptoKey&>(arg1.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->export_key(format, key); }));

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

JS_DEFINE_NATIVE_FUNCTION(SubtleCryptoPrototype::wrap_key)
{
    WebIDL::log_trace(vm, "SubtleCryptoPrototype::wrap_key");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "wrapKey", "4");

    auto arg0 = vm.argument(0);

    KeyFormat format { KeyFormat::RawPublic };

    auto arg0_string = TRY(arg0.to_string(vm));

    if (arg0_string == "raw-public"sv)
        format = KeyFormat::RawPublic;

    else if (arg0_string == "raw-private"sv)
        format = KeyFormat::RawPrivate;

    else if (arg0_string == "raw-seed"sv)
        format = KeyFormat::RawSeed;

    else if (arg0_string == "raw-secret"sv)
        format = KeyFormat::RawSecret;

    else if (arg0_string == "raw"sv)
        format = KeyFormat::Raw;

    else if (arg0_string == "spki"sv)
        format = KeyFormat::Spki;

    else if (arg0_string == "pkcs8"sv)
        format = KeyFormat::Pkcs8;

    else if (arg0_string == "jwk"sv)
        format = KeyFormat::Jwk;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, arg0_string, "KeyFormat");

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<CryptoKey>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CryptoKey");

    auto& key = static_cast<CryptoKey&>(arg1.as_object());

    auto arg2 = vm.argument(2);

    if (!arg2.is_object() || !is<CryptoKey>(arg2.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CryptoKey");

    auto& wrapping_key = static_cast<CryptoKey&>(arg2.as_object());

    auto arg3 = vm.argument(3);

    auto arg3_to_variant = [&vm, &realm](JS::Value arg3) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg3.is_object()) {
            [[maybe_unused]] auto& arg3_object = arg3.as_object();

            if (is<JS::ArrayBuffer>(arg3_object))
                return GC::make_root(arg3_object);

            if (is<JS::DataView>(arg3_object))
                return GC::make_root(arg3_object);

            if (is<JS::TypedArrayBase>(arg3_object))
                return GC::make_root(arg3_object);

        return arg3_object;

        }

    String arg3_string;
    if (!false || !arg3.is_null()) {
        arg3_string = TRY(WebIDL::to_string(vm, arg3));
    }

        return { arg3_string };

    };

    Variant<GC::Root<JS::Object>, String> wrap_algorithm = TRY(arg3_to_variant(arg3));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->wrap_key(format, key, wrapping_key, wrap_algorithm); }));

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

JS_DEFINE_NATIVE_FUNCTION(SubtleCryptoPrototype::unwrap_key)
{
    WebIDL::log_trace(vm, "SubtleCryptoPrototype::unwrap_key");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 7)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "unwrapKey", "7");

    auto arg0 = vm.argument(0);

    KeyFormat format { KeyFormat::RawPublic };

    auto arg0_string = TRY(arg0.to_string(vm));

    if (arg0_string == "raw-public"sv)
        format = KeyFormat::RawPublic;

    else if (arg0_string == "raw-private"sv)
        format = KeyFormat::RawPrivate;

    else if (arg0_string == "raw-seed"sv)
        format = KeyFormat::RawSeed;

    else if (arg0_string == "raw-secret"sv)
        format = KeyFormat::RawSecret;

    else if (arg0_string == "raw"sv)
        format = KeyFormat::Raw;

    else if (arg0_string == "spki"sv)
        format = KeyFormat::Spki;

    else if (arg0_string == "pkcs8"sv)
        format = KeyFormat::Pkcs8;

    else if (arg0_string == "jwk"sv)
        format = KeyFormat::Jwk;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, arg0_string, "KeyFormat");

    auto arg1 = vm.argument(1);

    GC::Root<WebIDL::BufferSource> wrapped_key;

    if (!arg1.is_object() || !(is<JS::TypedArrayBase>(arg1.as_object()) || is<JS::ArrayBuffer>(arg1.as_object()) || is<JS::DataView>(arg1.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BufferSource");

    wrapped_key = GC::make_root(realm.create<WebIDL::BufferSource>(arg1.as_object()));

    auto arg2 = vm.argument(2);

    if (!arg2.is_object() || !is<CryptoKey>(arg2.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CryptoKey");

    auto& unwrapping_key = static_cast<CryptoKey&>(arg2.as_object());

    auto arg3 = vm.argument(3);

    auto arg3_to_variant = [&vm, &realm](JS::Value arg3) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg3.is_object()) {
            [[maybe_unused]] auto& arg3_object = arg3.as_object();

            if (is<JS::ArrayBuffer>(arg3_object))
                return GC::make_root(arg3_object);

            if (is<JS::DataView>(arg3_object))
                return GC::make_root(arg3_object);

            if (is<JS::TypedArrayBase>(arg3_object))
                return GC::make_root(arg3_object);

        return arg3_object;

        }

    String arg3_string;
    if (!false || !arg3.is_null()) {
        arg3_string = TRY(WebIDL::to_string(vm, arg3));
    }

        return { arg3_string };

    };

    Variant<GC::Root<JS::Object>, String> unwrap_algorithm = TRY(arg3_to_variant(arg3));

    auto arg4 = vm.argument(4);

    auto arg4_to_variant = [&vm, &realm](JS::Value arg4) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg4.is_object()) {
            [[maybe_unused]] auto& arg4_object = arg4.as_object();

            if (is<JS::ArrayBuffer>(arg4_object))
                return GC::make_root(arg4_object);

            if (is<JS::DataView>(arg4_object))
                return GC::make_root(arg4_object);

            if (is<JS::TypedArrayBase>(arg4_object))
                return GC::make_root(arg4_object);

        return arg4_object;

        }

    String arg4_string;
    if (!false || !arg4.is_null()) {
        arg4_string = TRY(WebIDL::to_string(vm, arg4));
    }

        return { arg4_string };

    };

    Variant<GC::Root<JS::Object>, String> unwrapped_key_algorithm = TRY(arg4_to_variant(arg4));

    auto arg5 = vm.argument(5);

    bool extractable;

    extractable = arg5.to_boolean();

    auto arg6 = vm.argument(6);

    if (!arg6.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg6);

    auto arg6_iterator_method0 = TRY(arg6.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg6_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg6);

    auto arg6_iterator1 = TRY(JS::get_iterator_from_method(vm, arg6, *arg6_iterator_method0));

    Vector<KeyUsage> key_usages;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg6_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    KeyUsage sequence_item1 { KeyUsage::Encrypt };

    auto next_item1_string = TRY(next_item1.to_string(vm));

    if (next_item1_string == "encrypt"sv)
        sequence_item1 = KeyUsage::Encrypt;

    else if (next_item1_string == "decrypt"sv)
        sequence_item1 = KeyUsage::Decrypt;

    else if (next_item1_string == "sign"sv)
        sequence_item1 = KeyUsage::Sign;

    else if (next_item1_string == "verify"sv)
        sequence_item1 = KeyUsage::Verify;

    else if (next_item1_string == "deriveKey"sv)
        sequence_item1 = KeyUsage::Derivekey;

    else if (next_item1_string == "deriveBits"sv)
        sequence_item1 = KeyUsage::Derivebits;

    else if (next_item1_string == "wrapKey"sv)
        sequence_item1 = KeyUsage::Wrapkey;

    else if (next_item1_string == "unwrapKey"sv)
        sequence_item1 = KeyUsage::Unwrapkey;

    else if (next_item1_string == "encapsulateKey"sv)
        sequence_item1 = KeyUsage::Encapsulatekey;

    else if (next_item1_string == "encapsulateBits"sv)
        sequence_item1 = KeyUsage::Encapsulatebits;

    else if (next_item1_string == "decapsulateKey"sv)
        sequence_item1 = KeyUsage::Decapsulatekey;

    else if (next_item1_string == "decapsulateBits"sv)
        sequence_item1 = KeyUsage::Decapsulatebits;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, next_item1_string, "KeyUsage");

    key_usages.append(sequence_item1);
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->unwrap_key(format, wrapped_key, unwrapping_key, unwrap_algorithm, unwrapped_key_algorithm, extractable, key_usages); }));

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

JS_DEFINE_NATIVE_FUNCTION(SubtleCryptoPrototype::encapsulate_key)
{
    WebIDL::log_trace(vm, "SubtleCryptoPrototype::encapsulate_key");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "encapsulateKey", "5");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<JS::ArrayBuffer>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::DataView>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::TypedArrayBase>(arg0_object))
                return GC::make_root(arg0_object);

        return arg0_object;

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<GC::Root<JS::Object>, String> encapsulation_algorithm = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<CryptoKey>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CryptoKey");

    auto& encapsulation_key = static_cast<CryptoKey&>(arg1.as_object());

    auto arg2 = vm.argument(2);

    auto arg2_to_variant = [&vm, &realm](JS::Value arg2) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg2.is_object()) {
            [[maybe_unused]] auto& arg2_object = arg2.as_object();

            if (is<JS::ArrayBuffer>(arg2_object))
                return GC::make_root(arg2_object);

            if (is<JS::DataView>(arg2_object))
                return GC::make_root(arg2_object);

            if (is<JS::TypedArrayBase>(arg2_object))
                return GC::make_root(arg2_object);

        return arg2_object;

        }

    String arg2_string;
    if (!false || !arg2.is_null()) {
        arg2_string = TRY(WebIDL::to_string(vm, arg2));
    }

        return { arg2_string };

    };

    Variant<GC::Root<JS::Object>, String> shared_key_algorithm = TRY(arg2_to_variant(arg2));

    auto arg3 = vm.argument(3);

    bool extractable;

    extractable = arg3.to_boolean();

    auto arg4 = vm.argument(4);

    if (!arg4.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg4);

    auto arg4_iterator_method0 = TRY(arg4.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg4_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg4);

    auto arg4_iterator1 = TRY(JS::get_iterator_from_method(vm, arg4, *arg4_iterator_method0));

    Vector<KeyUsage> key_usages;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg4_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    KeyUsage sequence_item1 { KeyUsage::Encrypt };

    auto next_item1_string = TRY(next_item1.to_string(vm));

    if (next_item1_string == "encrypt"sv)
        sequence_item1 = KeyUsage::Encrypt;

    else if (next_item1_string == "decrypt"sv)
        sequence_item1 = KeyUsage::Decrypt;

    else if (next_item1_string == "sign"sv)
        sequence_item1 = KeyUsage::Sign;

    else if (next_item1_string == "verify"sv)
        sequence_item1 = KeyUsage::Verify;

    else if (next_item1_string == "deriveKey"sv)
        sequence_item1 = KeyUsage::Derivekey;

    else if (next_item1_string == "deriveBits"sv)
        sequence_item1 = KeyUsage::Derivebits;

    else if (next_item1_string == "wrapKey"sv)
        sequence_item1 = KeyUsage::Wrapkey;

    else if (next_item1_string == "unwrapKey"sv)
        sequence_item1 = KeyUsage::Unwrapkey;

    else if (next_item1_string == "encapsulateKey"sv)
        sequence_item1 = KeyUsage::Encapsulatekey;

    else if (next_item1_string == "encapsulateBits"sv)
        sequence_item1 = KeyUsage::Encapsulatebits;

    else if (next_item1_string == "decapsulateKey"sv)
        sequence_item1 = KeyUsage::Decapsulatekey;

    else if (next_item1_string == "decapsulateBits"sv)
        sequence_item1 = KeyUsage::Decapsulatebits;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, next_item1_string, "KeyUsage");

    key_usages.append(sequence_item1);
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->encapsulate_key(encapsulation_algorithm, encapsulation_key, shared_key_algorithm, extractable, key_usages); }));

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

JS_DEFINE_NATIVE_FUNCTION(SubtleCryptoPrototype::encapsulate_bits)
{
    WebIDL::log_trace(vm, "SubtleCryptoPrototype::encapsulate_bits");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "encapsulateBits", "2");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<JS::ArrayBuffer>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::DataView>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::TypedArrayBase>(arg0_object))
                return GC::make_root(arg0_object);

        return arg0_object;

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<GC::Root<JS::Object>, String> encapsulation_algorithm = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<CryptoKey>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CryptoKey");

    auto& encapsulation_key = static_cast<CryptoKey&>(arg1.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->encapsulate_bits(encapsulation_algorithm, encapsulation_key); }));

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

JS_DEFINE_NATIVE_FUNCTION(SubtleCryptoPrototype::decapsulate_key)
{
    WebIDL::log_trace(vm, "SubtleCryptoPrototype::decapsulate_key");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 6)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "decapsulateKey", "6");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<JS::ArrayBuffer>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::DataView>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::TypedArrayBase>(arg0_object))
                return GC::make_root(arg0_object);

        return arg0_object;

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<GC::Root<JS::Object>, String> decapsulation_algorithm = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<CryptoKey>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CryptoKey");

    auto& decapsulation_key = static_cast<CryptoKey&>(arg1.as_object());

    auto arg2 = vm.argument(2);

    GC::Root<WebIDL::BufferSource> ciphertext;

    if (!arg2.is_object() || !(is<JS::TypedArrayBase>(arg2.as_object()) || is<JS::ArrayBuffer>(arg2.as_object()) || is<JS::DataView>(arg2.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BufferSource");

    ciphertext = GC::make_root(realm.create<WebIDL::BufferSource>(arg2.as_object()));

    auto arg3 = vm.argument(3);

    auto arg3_to_variant = [&vm, &realm](JS::Value arg3) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg3.is_object()) {
            [[maybe_unused]] auto& arg3_object = arg3.as_object();

            if (is<JS::ArrayBuffer>(arg3_object))
                return GC::make_root(arg3_object);

            if (is<JS::DataView>(arg3_object))
                return GC::make_root(arg3_object);

            if (is<JS::TypedArrayBase>(arg3_object))
                return GC::make_root(arg3_object);

        return arg3_object;

        }

    String arg3_string;
    if (!false || !arg3.is_null()) {
        arg3_string = TRY(WebIDL::to_string(vm, arg3));
    }

        return { arg3_string };

    };

    Variant<GC::Root<JS::Object>, String> shared_key_algorithm = TRY(arg3_to_variant(arg3));

    auto arg4 = vm.argument(4);

    bool extractable;

    extractable = arg4.to_boolean();

    auto arg5 = vm.argument(5);

    if (!arg5.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg5);

    auto arg5_iterator_method0 = TRY(arg5.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg5_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg5);

    auto arg5_iterator1 = TRY(JS::get_iterator_from_method(vm, arg5, *arg5_iterator_method0));

    Vector<KeyUsage> key_usages;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg5_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    KeyUsage sequence_item1 { KeyUsage::Encrypt };

    auto next_item1_string = TRY(next_item1.to_string(vm));

    if (next_item1_string == "encrypt"sv)
        sequence_item1 = KeyUsage::Encrypt;

    else if (next_item1_string == "decrypt"sv)
        sequence_item1 = KeyUsage::Decrypt;

    else if (next_item1_string == "sign"sv)
        sequence_item1 = KeyUsage::Sign;

    else if (next_item1_string == "verify"sv)
        sequence_item1 = KeyUsage::Verify;

    else if (next_item1_string == "deriveKey"sv)
        sequence_item1 = KeyUsage::Derivekey;

    else if (next_item1_string == "deriveBits"sv)
        sequence_item1 = KeyUsage::Derivebits;

    else if (next_item1_string == "wrapKey"sv)
        sequence_item1 = KeyUsage::Wrapkey;

    else if (next_item1_string == "unwrapKey"sv)
        sequence_item1 = KeyUsage::Unwrapkey;

    else if (next_item1_string == "encapsulateKey"sv)
        sequence_item1 = KeyUsage::Encapsulatekey;

    else if (next_item1_string == "encapsulateBits"sv)
        sequence_item1 = KeyUsage::Encapsulatebits;

    else if (next_item1_string == "decapsulateKey"sv)
        sequence_item1 = KeyUsage::Decapsulatekey;

    else if (next_item1_string == "decapsulateBits"sv)
        sequence_item1 = KeyUsage::Decapsulatebits;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, next_item1_string, "KeyUsage");

    key_usages.append(sequence_item1);
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->decapsulate_key(decapsulation_algorithm, decapsulation_key, ciphertext, shared_key_algorithm, extractable, key_usages); }));

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

JS_DEFINE_NATIVE_FUNCTION(SubtleCryptoPrototype::decapsulate_bits)
{
    WebIDL::log_trace(vm, "SubtleCryptoPrototype::decapsulate_bits");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "decapsulateBits", "3");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Object>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<JS::ArrayBuffer>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::DataView>(arg0_object))
                return GC::make_root(arg0_object);

            if (is<JS::TypedArrayBase>(arg0_object))
                return GC::make_root(arg0_object);

        return arg0_object;

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<GC::Root<JS::Object>, String> decapsulation_algorithm = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<CryptoKey>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CryptoKey");

    auto& decapsulation_key = static_cast<CryptoKey&>(arg1.as_object());

    auto arg2 = vm.argument(2);

    GC::Root<WebIDL::BufferSource> ciphertext;

    if (!arg2.is_object() || !(is<JS::TypedArrayBase>(arg2.as_object()) || is<JS::ArrayBuffer>(arg2.as_object()) || is<JS::DataView>(arg2.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BufferSource");

    ciphertext = GC::make_root(realm.create<WebIDL::BufferSource>(arg2.as_object()));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->decapsulate_bits(decapsulation_algorithm, decapsulation_key, ciphertext); }));

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
