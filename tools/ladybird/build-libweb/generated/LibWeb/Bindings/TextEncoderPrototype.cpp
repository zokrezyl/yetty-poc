
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
#include <LibWeb/Bindings/TextEncoderPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Encoding/TextEncoder.h>

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

GC_DEFINE_ALLOCATOR(TextEncoderPrototype);

TextEncoderPrototype::TextEncoderPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

TextEncoderPrototype::~TextEncoderPrototype()
{
}

void TextEncoderPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_encoding_getter = JS::NativeFunction::create(realm, encoding_getter, 0, "encoding"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_encoding_setter;

    define_direct_accessor("encoding"_utf16_fly_string, native_encoding_getter, native_encoding_setter, default_attributes);

    define_native_function(realm, "encode"_utf16_fly_string, encode, 0, default_attributes);

    define_native_function(realm, "encodeInto"_utf16_fly_string, encode_into, 2, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "TextEncoder"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void TextEncoderPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Encoding::TextEncoder*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Encoding::TextEncoder>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "TextEncoder");
    return static_cast<Encoding::TextEncoder*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(TextEncoderPrototype::encoding_getter)
{
    WebIDL::log_trace(vm, "TextEncoderPrototype::encoding_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->encoding(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(TextEncoderPrototype::encode)
{
    WebIDL::log_trace(vm, "TextEncoderPrototype::encode");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    String input;

    if (!arg0.is_undefined()) {
        if (!false || !arg0.is_null())
            input = TRY(WebIDL::to_usv_string(vm, arg0));
    } else {
        input = ""_string;
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->encode(input); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(TextEncoderPrototype::encode_into)
{
    WebIDL::log_trace(vm, "TextEncoderPrototype::encode_into");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "encodeInto", "2");

    auto arg0 = vm.argument(0);

    String source;
    if (!false || !arg0.is_null()) {
        source = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    GC::Root<WebIDL::BufferSource> destination;

    if (!arg1.is_object() || !(is<JS::TypedArrayBase>(arg1.as_object()) || is<JS::ArrayBuffer>(arg1.as_object()) || is<JS::DataView>(arg1.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Uint8Array");

    destination = GC::make_root(realm.create<WebIDL::BufferSource>(arg1.as_object()));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->encode_into(source, destination); }));

    {
        auto dictionary_object0 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_read0_value;

    wrapped_read0_value = JS::Value(static_cast<double>(retval.read));

        MUST(dictionary_object0->create_data_property("read"_utf16_fly_string, wrapped_read0_value));

        JS::Value wrapped_written0_value;

    wrapped_written0_value = JS::Value(static_cast<double>(retval.written));

        MUST(dictionary_object0->create_data_property("written"_utf16_fly_string, wrapped_written0_value));

        return dictionary_object0;
    }

}

} // namespace Web::Bindings
