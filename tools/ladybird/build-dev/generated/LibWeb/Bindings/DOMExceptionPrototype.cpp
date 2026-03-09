
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
#include <LibWeb/Bindings/DOMExceptionPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebIDL/DOMException.h>

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

GC_DEFINE_ALLOCATOR(DOMExceptionPrototype);

DOMExceptionPrototype::DOMExceptionPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().error_prototype())

{
}

DOMExceptionPrototype::~DOMExceptionPrototype()
{
}

void DOMExceptionPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().error_prototype());

    auto native_name_getter = JS::NativeFunction::create(realm, name_getter, 0, "name"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_name_setter;

    define_direct_accessor("name"_utf16_fly_string, native_name_getter, native_name_setter, default_attributes);

    auto native_message_getter = JS::NativeFunction::create(realm, message_getter, 0, "message"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_message_setter;

    define_direct_accessor("message"_utf16_fly_string, native_message_getter, native_message_setter, default_attributes);

    auto native_code_getter = JS::NativeFunction::create(realm, code_getter, 0, "code"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_code_setter;

    define_direct_accessor("code"_utf16_fly_string, native_code_getter, native_code_setter, default_attributes);

    auto constant_INDEX_SIZE_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(1));

    define_direct_property("INDEX_SIZE_ERR"_utf16_fly_string, constant_INDEX_SIZE_ERR_value, JS::Attribute::Enumerable);

    auto constant_DOMSTRING_SIZE_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(2));

    define_direct_property("DOMSTRING_SIZE_ERR"_utf16_fly_string, constant_DOMSTRING_SIZE_ERR_value, JS::Attribute::Enumerable);

    auto constant_HIERARCHY_REQUEST_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(3));

    define_direct_property("HIERARCHY_REQUEST_ERR"_utf16_fly_string, constant_HIERARCHY_REQUEST_ERR_value, JS::Attribute::Enumerable);

    auto constant_WRONG_DOCUMENT_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(4));

    define_direct_property("WRONG_DOCUMENT_ERR"_utf16_fly_string, constant_WRONG_DOCUMENT_ERR_value, JS::Attribute::Enumerable);

    auto constant_INVALID_CHARACTER_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(5));

    define_direct_property("INVALID_CHARACTER_ERR"_utf16_fly_string, constant_INVALID_CHARACTER_ERR_value, JS::Attribute::Enumerable);

    auto constant_NO_DATA_ALLOWED_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(6));

    define_direct_property("NO_DATA_ALLOWED_ERR"_utf16_fly_string, constant_NO_DATA_ALLOWED_ERR_value, JS::Attribute::Enumerable);

    auto constant_NO_MODIFICATION_ALLOWED_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(7));

    define_direct_property("NO_MODIFICATION_ALLOWED_ERR"_utf16_fly_string, constant_NO_MODIFICATION_ALLOWED_ERR_value, JS::Attribute::Enumerable);

    auto constant_NOT_FOUND_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(8));

    define_direct_property("NOT_FOUND_ERR"_utf16_fly_string, constant_NOT_FOUND_ERR_value, JS::Attribute::Enumerable);

    auto constant_NOT_SUPPORTED_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(9));

    define_direct_property("NOT_SUPPORTED_ERR"_utf16_fly_string, constant_NOT_SUPPORTED_ERR_value, JS::Attribute::Enumerable);

    auto constant_INUSE_ATTRIBUTE_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(10));

    define_direct_property("INUSE_ATTRIBUTE_ERR"_utf16_fly_string, constant_INUSE_ATTRIBUTE_ERR_value, JS::Attribute::Enumerable);

    auto constant_INVALID_STATE_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(11));

    define_direct_property("INVALID_STATE_ERR"_utf16_fly_string, constant_INVALID_STATE_ERR_value, JS::Attribute::Enumerable);

    auto constant_SYNTAX_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(12));

    define_direct_property("SYNTAX_ERR"_utf16_fly_string, constant_SYNTAX_ERR_value, JS::Attribute::Enumerable);

    auto constant_INVALID_MODIFICATION_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(13));

    define_direct_property("INVALID_MODIFICATION_ERR"_utf16_fly_string, constant_INVALID_MODIFICATION_ERR_value, JS::Attribute::Enumerable);

    auto constant_NAMESPACE_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(14));

    define_direct_property("NAMESPACE_ERR"_utf16_fly_string, constant_NAMESPACE_ERR_value, JS::Attribute::Enumerable);

    auto constant_INVALID_ACCESS_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(15));

    define_direct_property("INVALID_ACCESS_ERR"_utf16_fly_string, constant_INVALID_ACCESS_ERR_value, JS::Attribute::Enumerable);

    auto constant_VALIDATION_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(16));

    define_direct_property("VALIDATION_ERR"_utf16_fly_string, constant_VALIDATION_ERR_value, JS::Attribute::Enumerable);

    auto constant_TYPE_MISMATCH_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(17));

    define_direct_property("TYPE_MISMATCH_ERR"_utf16_fly_string, constant_TYPE_MISMATCH_ERR_value, JS::Attribute::Enumerable);

    auto constant_SECURITY_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(18));

    define_direct_property("SECURITY_ERR"_utf16_fly_string, constant_SECURITY_ERR_value, JS::Attribute::Enumerable);

    auto constant_NETWORK_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(19));

    define_direct_property("NETWORK_ERR"_utf16_fly_string, constant_NETWORK_ERR_value, JS::Attribute::Enumerable);

    auto constant_ABORT_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(20));

    define_direct_property("ABORT_ERR"_utf16_fly_string, constant_ABORT_ERR_value, JS::Attribute::Enumerable);

    auto constant_URL_MISMATCH_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(21));

    define_direct_property("URL_MISMATCH_ERR"_utf16_fly_string, constant_URL_MISMATCH_ERR_value, JS::Attribute::Enumerable);

    auto constant_QUOTA_EXCEEDED_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(22));

    define_direct_property("QUOTA_EXCEEDED_ERR"_utf16_fly_string, constant_QUOTA_EXCEEDED_ERR_value, JS::Attribute::Enumerable);

    auto constant_TIMEOUT_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(23));

    define_direct_property("TIMEOUT_ERR"_utf16_fly_string, constant_TIMEOUT_ERR_value, JS::Attribute::Enumerable);

    auto constant_INVALID_NODE_TYPE_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(24));

    define_direct_property("INVALID_NODE_TYPE_ERR"_utf16_fly_string, constant_INVALID_NODE_TYPE_ERR_value, JS::Attribute::Enumerable);

    auto constant_DATA_CLONE_ERR_value = JS::Value(static_cast<WebIDL::UnsignedShort>(25));

    define_direct_property("DATA_CLONE_ERR"_utf16_fly_string, constant_DATA_CLONE_ERR_value, JS::Attribute::Enumerable);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "DOMException"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void DOMExceptionPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<WebIDL::DOMException*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<WebIDL::DOMException>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMException");
    return static_cast<WebIDL::DOMException*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(DOMExceptionPrototype::name_getter)
{
    WebIDL::log_trace(vm, "DOMExceptionPrototype::name_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->name(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMExceptionPrototype::message_getter)
{
    WebIDL::log_trace(vm, "DOMExceptionPrototype::message_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->message(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMExceptionPrototype::code_getter)
{
    WebIDL::log_trace(vm, "DOMExceptionPrototype::code_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->code(); }));

    return JS::Value(static_cast<WebIDL::UnsignedShort>(retval));

}

} // namespace Web::Bindings
