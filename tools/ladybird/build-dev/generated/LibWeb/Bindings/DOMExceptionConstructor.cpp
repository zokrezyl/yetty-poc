
#include <LibIDL/Types.h>
#include <LibGC/Heap.h>
#include <LibJS/Runtime/Array.h>
#include <LibJS/Runtime/ArrayBuffer.h>
#include <LibJS/Runtime/DataView.h>
#include <LibJS/Runtime/GlobalObject.h>
#include <LibJS/Runtime/Iterator.h>
#include <LibJS/Runtime/PromiseConstructor.h>
#include <LibJS/Runtime/ValueInlines.h>
#include <LibJS/Runtime/TypedArray.h>
#include <LibWeb/Bindings/DOMExceptionConstructor.h>
#include <LibWeb/Bindings/DOMExceptionPrototype.h>
#include <LibWeb/Bindings/ExceptionOrUtils.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/HTML/WindowProxy.h>
#include <LibWeb/WebIDL/AbstractOperations.h>
#include <LibWeb/WebIDL/Buffers.h>
#include <LibWeb/WebIDL/CallbackType.h>
#include <LibWeb/WebIDL/OverloadResolution.h>
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

GC_DEFINE_ALLOCATOR(DOMExceptionConstructor);

DOMExceptionConstructor::DOMExceptionConstructor(JS::Realm& realm)
    : NativeFunction("DOMException"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

DOMExceptionConstructor::~DOMExceptionConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> DOMExceptionConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "DOMException");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> DOMExceptionConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "DOMExceptionConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface DOMException:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<DOMExceptionPrototype>(*target_realm, "DOMException"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    auto arg0 = vm.argument(0);

    Utf16String message;

    if (!arg0.is_undefined()) {
        if (!false || !arg0.is_null())
            message = TRY(WebIDL::to_utf16_string(vm, arg0));
    } else {
        message = ""_utf16;
    }

    auto arg1 = vm.argument(1);

    String name;

    if (!arg1.is_undefined()) {
        if (!false || !arg1.is_null())
            name = TRY(WebIDL::to_string(vm, arg1));
    } else {
        name = "Error"_string;
    }

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return WebIDL::DOMException::construct_impl(realm, message, name); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface DOMException" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within WebIDL::DOMException::construct_impl().
    //  8. Let interfaces be the inclusive inherited interfaces of interface.
    //  9. For every interface ancestor interface in interfaces:
    //    9.1. Let unforgeables be the value of the [[Unforgeables]] slot of the interface object of ancestor interface in realm.
    //    9.2. Let keys be ! unforgeables.[[OwnPropertyKeys]]().
    //    9.3. For each element key of keys:
    //      9.3.1. Let descriptor be ! unforgeables.[[GetOwnProperty]](key).
    //      9.3.2. Perform ! DefinePropertyOrThrow(instance, key, descriptor).
    //  10. If interface is declared with the [Global] extended attribute, then:
    //    10.1. Define the regular operations of interface on instance, given realm.
    //    10.2. Define the regular attributes of interface on instance, given realm.
    //    10.3. Define the iteration methods of interface on instance given realm.
    //    10.4. Define the asynchronous iteration methods of interface on instance given realm.
    //    10.5. Define the global property references on instance, given realm.
    //    10.6. Set instance.[[SetPrototypeOf]] as defined in § 3.8.1 [[SetPrototypeOf]].
    //  11. Otherwise, if interfaces contains an interface which supports indexed properties, named properties, or both:
    //    11.1. Set instance.[[GetOwnProperty]] as defined in § 3.9.1 [[GetOwnProperty]].
    //    11.2. Set instance.[[Set]] as defined in § 3.9.2 [[Set]].
    //    11.3. Set instance.[[DefineOwnProperty]] as defined in § 3.9.3 [[DefineOwnProperty]].
    //    11.4. Set instance.[[Delete]] as defined in § 3.9.4 [[Delete]].
    //    11.5. Set instance.[[PreventExtensions]] as defined in § 3.9.5 [[PreventExtensions]].
    //    11.6. Set instance.[[OwnPropertyKeys]] as defined in § 3.9.6 [[OwnPropertyKeys]].

    return *impl;
}


void DOMExceptionConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    define_direct_property(vm.names.length, JS::Value(0), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "DOMException"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<DOMExceptionPrototype>(realm, "DOMException"_fly_string), 0);


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

}

} // namespace Web::Bindings
