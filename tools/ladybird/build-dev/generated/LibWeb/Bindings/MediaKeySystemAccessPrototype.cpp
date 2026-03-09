
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
#include <LibWeb/Bindings/MediaKeySystemAccessPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/EncryptedMediaExtensions/MediaKeySystemAccess.h>

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

GC_DEFINE_ALLOCATOR(MediaKeySystemAccessPrototype);

MediaKeySystemAccessPrototype::MediaKeySystemAccessPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

MediaKeySystemAccessPrototype::~MediaKeySystemAccessPrototype()
{
}

void MediaKeySystemAccessPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_key_system_getter = JS::NativeFunction::create(realm, key_system_getter, 0, "keySystem"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_key_system_setter;

    define_direct_accessor("keySystem"_utf16_fly_string, native_key_system_getter, native_key_system_setter, default_attributes);

        define_direct_property("createMediaKeys"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_native_function(realm, "getConfiguration"_utf16_fly_string, get_configuration, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "MediaKeySystemAccess"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void MediaKeySystemAccessPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<MediaKeySystemAccess*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<MediaKeySystemAccess>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "MediaKeySystemAccess");
    return static_cast<MediaKeySystemAccess*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(MediaKeySystemAccessPrototype::key_system_getter)
{
    WebIDL::log_trace(vm, "MediaKeySystemAccessPrototype::key_system_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->key_system(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(MediaKeySystemAccessPrototype::get_configuration)
{
    WebIDL::log_trace(vm, "MediaKeySystemAccessPrototype::get_configuration");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_configuration(); }));

    {
        auto dictionary_object0 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_audio_capabilities0_value;

    auto new_array1_2 = MUST(JS::Array::create(realm, 0));

    for (size_t i1 = 0; i1 < retval.audio_capabilities.size(); ++i1) {
        auto& element1 = retval.audio_capabilities.at(i1);
JS::Value wrapped_element1;

    {
        auto dictionary_object2 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_content_type2_value;

    wrapped_content_type2_value = JS::PrimitiveString::create(vm, element1.content_type);

        MUST(dictionary_object2->create_data_property("contentType"_utf16_fly_string, wrapped_content_type2_value));

        JS::Value wrapped_encryption_scheme2_value;

    if (element1.encryption_scheme.has_value()) {

    wrapped_encryption_scheme2_value = JS::PrimitiveString::create(vm, const_cast<decltype(element1.encryption_scheme)&>(element1.encryption_scheme).release_value());

    } else {
        wrapped_encryption_scheme2_value = JS::js_null();
    }

        MUST(dictionary_object2->create_data_property("encryptionScheme"_utf16_fly_string, wrapped_encryption_scheme2_value));

        JS::Value wrapped_robustness2_value;

    wrapped_robustness2_value = JS::PrimitiveString::create(vm, element1.robustness);

        MUST(dictionary_object2->create_data_property("robustness"_utf16_fly_string, wrapped_robustness2_value));

        wrapped_element1 = dictionary_object2;
    }

        auto property_index1 = JS::PropertyKey { i1 };
        MUST(new_array1_2->create_data_property(property_index1, wrapped_element1));
    }

    wrapped_audio_capabilities0_value = new_array1_2;

        MUST(dictionary_object0->create_data_property("audioCapabilities"_utf16_fly_string, wrapped_audio_capabilities0_value));

        JS::Value wrapped_distinctive_identifier0_value;

    wrapped_distinctive_identifier0_value = JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval.distinctive_identifier));

        MUST(dictionary_object0->create_data_property("distinctiveIdentifier"_utf16_fly_string, wrapped_distinctive_identifier0_value));

        JS::Value wrapped_init_data_types0_value;

    auto new_array1_4 = MUST(JS::Array::create(realm, 0));

    for (size_t i1 = 0; i1 < retval.init_data_types.size(); ++i1) {
        auto& element1 = retval.init_data_types.at(i1);
JS::Value wrapped_element1;

    wrapped_element1 = JS::PrimitiveString::create(vm, element1);

        auto property_index1 = JS::PropertyKey { i1 };
        MUST(new_array1_4->create_data_property(property_index1, wrapped_element1));
    }

    wrapped_init_data_types0_value = new_array1_4;

        MUST(dictionary_object0->create_data_property("initDataTypes"_utf16_fly_string, wrapped_init_data_types0_value));

        JS::Value wrapped_label0_value;

    wrapped_label0_value = JS::PrimitiveString::create(vm, retval.label);

        MUST(dictionary_object0->create_data_property("label"_utf16_fly_string, wrapped_label0_value));

        JS::Value wrapped_persistent_state0_value;

    wrapped_persistent_state0_value = JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval.persistent_state));

        MUST(dictionary_object0->create_data_property("persistentState"_utf16_fly_string, wrapped_persistent_state0_value));

        Optional<JS::Value> wrapped_session_types0_value;

    if (retval.session_types.has_value()) {

    auto new_array1_7 = MUST(JS::Array::create(realm, 0));

    auto& retval_session_types_non_optional = retval.session_types.value();
    for (size_t i1 = 0; i1 < retval_session_types_non_optional.size(); ++i1) {
        auto& element1 = retval_session_types_non_optional.at(i1);
JS::Value wrapped_element1;

    wrapped_element1 = JS::PrimitiveString::create(vm, element1);

        auto property_index1 = JS::PropertyKey { i1 };
        MUST(new_array1_7->create_data_property(property_index1, wrapped_element1));
    }

    wrapped_session_types0_value = new_array1_7;

    }

        if (wrapped_session_types0_value.has_value())
            MUST(dictionary_object0->create_data_property("sessionTypes"_utf16_fly_string, wrapped_session_types0_value.release_value()));

        JS::Value wrapped_video_capabilities0_value;

    auto new_array1_8 = MUST(JS::Array::create(realm, 0));

    for (size_t i1 = 0; i1 < retval.video_capabilities.size(); ++i1) {
        auto& element1 = retval.video_capabilities.at(i1);
JS::Value wrapped_element1;

    {
        auto dictionary_object2 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_content_type2_value;

    wrapped_content_type2_value = JS::PrimitiveString::create(vm, element1.content_type);

        MUST(dictionary_object2->create_data_property("contentType"_utf16_fly_string, wrapped_content_type2_value));

        JS::Value wrapped_encryption_scheme2_value;

    if (element1.encryption_scheme.has_value()) {

    wrapped_encryption_scheme2_value = JS::PrimitiveString::create(vm, const_cast<decltype(element1.encryption_scheme)&>(element1.encryption_scheme).release_value());

    } else {
        wrapped_encryption_scheme2_value = JS::js_null();
    }

        MUST(dictionary_object2->create_data_property("encryptionScheme"_utf16_fly_string, wrapped_encryption_scheme2_value));

        JS::Value wrapped_robustness2_value;

    wrapped_robustness2_value = JS::PrimitiveString::create(vm, element1.robustness);

        MUST(dictionary_object2->create_data_property("robustness"_utf16_fly_string, wrapped_robustness2_value));

        wrapped_element1 = dictionary_object2;
    }

        auto property_index1 = JS::PropertyKey { i1 };
        MUST(new_array1_8->create_data_property(property_index1, wrapped_element1));
    }

    wrapped_video_capabilities0_value = new_array1_8;

        MUST(dictionary_object0->create_data_property("videoCapabilities"_utf16_fly_string, wrapped_video_capabilities0_value));

        return dictionary_object0;
    }

}

} // namespace Web::Bindings
