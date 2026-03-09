
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
#include <LibWeb/Bindings/SecurityPolicyViolationEventConstructor.h>
#include <LibWeb/Bindings/SecurityPolicyViolationEventPrototype.h>
#include <LibWeb/Bindings/ExceptionOrUtils.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/HTML/WindowProxy.h>
#include <LibWeb/WebIDL/AbstractOperations.h>
#include <LibWeb/WebIDL/Buffers.h>
#include <LibWeb/WebIDL/CallbackType.h>
#include <LibWeb/WebIDL/OverloadResolution.h>
#include <LibWeb/WebIDL/Tracing.h>
#include <LibWeb/WebIDL/Types.h>

#if __has_include(<LibWeb/Bindings/EventPrototype.h>)
#    include <LibWeb/Bindings/EventPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ContentSecurityPolicy/SecurityPolicyViolationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

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

GC_DEFINE_ALLOCATOR(SecurityPolicyViolationEventConstructor);

SecurityPolicyViolationEventConstructor::SecurityPolicyViolationEventConstructor(JS::Realm& realm)
    : NativeFunction("SecurityPolicyViolationEvent"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

SecurityPolicyViolationEventConstructor::~SecurityPolicyViolationEventConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> SecurityPolicyViolationEventConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "SecurityPolicyViolationEvent");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> SecurityPolicyViolationEventConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "SecurityPolicyViolationEventConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface SecurityPolicyViolationEvent:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<SecurityPolicyViolationEventPrototype>(*target_realm, "SecurityPolicyViolationEvent"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "SecurityPolicyViolationEvent");

    auto arg0 = vm.argument(0);

    String type;
    if (!false || !arg0.is_null()) {
        type = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SecurityPolicyViolationEventInit");

    SecurityPolicyViolationEventInit event_init_dict {};

    auto blocked_uri_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        blocked_uri_property_value_0 = TRY(arg1.as_object().get("blockedURI"_utf16_fly_string));

    String blocked_uri_value_0;

    if (!blocked_uri_property_value_0.is_undefined()) {
        if (!false || !blocked_uri_property_value_0.is_null())
            blocked_uri_value_0 = TRY(WebIDL::to_usv_string(vm, blocked_uri_property_value_0));
    } else {
        blocked_uri_value_0 = ""_string;
    }

    event_init_dict.blocked_uri = blocked_uri_value_0;

    auto column_number_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        column_number_property_value_1 = TRY(arg1.as_object().get("columnNumber"_utf16_fly_string));

    WebIDL::UnsignedLong column_number_value_1;

    if (!column_number_property_value_1.is_undefined())

    column_number_value_1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, column_number_property_value_1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        column_number_value_1 = static_cast<WebIDL::UnsignedLong>(0);

    event_init_dict.column_number = column_number_value_1;

    auto disposition_property_value_2 = JS::js_undefined();
    if (arg1.is_object())
        disposition_property_value_2 = TRY(arg1.as_object().get("disposition"_utf16_fly_string));

    SecurityPolicyViolationEventDisposition disposition_value_2 { SecurityPolicyViolationEventDisposition::Enforce };

    if (!disposition_property_value_2.is_undefined()) {

    auto disposition_property_value_2_string = TRY(disposition_property_value_2.to_string(vm));

    if (disposition_property_value_2_string == "enforce"sv)
        disposition_value_2 = SecurityPolicyViolationEventDisposition::Enforce;

    else if (disposition_property_value_2_string == "report"sv)
        disposition_value_2 = SecurityPolicyViolationEventDisposition::Report;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, disposition_property_value_2_string, "SecurityPolicyViolationEventDisposition");

    }

    event_init_dict.disposition = disposition_value_2;

    auto document_uri_property_value_3 = JS::js_undefined();
    if (arg1.is_object())
        document_uri_property_value_3 = TRY(arg1.as_object().get("documentURI"_utf16_fly_string));

    String document_uri_value_3;

    if (!document_uri_property_value_3.is_undefined()) {
        if (!false || !document_uri_property_value_3.is_null())
            document_uri_value_3 = TRY(WebIDL::to_usv_string(vm, document_uri_property_value_3));
    } else {
        document_uri_value_3 = ""_string;
    }

    event_init_dict.document_uri = document_uri_value_3;

    auto effective_directive_property_value_4 = JS::js_undefined();
    if (arg1.is_object())
        effective_directive_property_value_4 = TRY(arg1.as_object().get("effectiveDirective"_utf16_fly_string));

    String effective_directive_value_4;

    if (!effective_directive_property_value_4.is_undefined()) {
        if (!false || !effective_directive_property_value_4.is_null())
            effective_directive_value_4 = TRY(WebIDL::to_string(vm, effective_directive_property_value_4));
    } else {
        effective_directive_value_4 = ""_string;
    }

    event_init_dict.effective_directive = effective_directive_value_4;

    auto line_number_property_value_5 = JS::js_undefined();
    if (arg1.is_object())
        line_number_property_value_5 = TRY(arg1.as_object().get("lineNumber"_utf16_fly_string));

    WebIDL::UnsignedLong line_number_value_5;

    if (!line_number_property_value_5.is_undefined())

    line_number_value_5 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, line_number_property_value_5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        line_number_value_5 = static_cast<WebIDL::UnsignedLong>(0);

    event_init_dict.line_number = line_number_value_5;

    auto original_policy_property_value_6 = JS::js_undefined();
    if (arg1.is_object())
        original_policy_property_value_6 = TRY(arg1.as_object().get("originalPolicy"_utf16_fly_string));

    String original_policy_value_6;

    if (!original_policy_property_value_6.is_undefined()) {
        if (!false || !original_policy_property_value_6.is_null())
            original_policy_value_6 = TRY(WebIDL::to_string(vm, original_policy_property_value_6));
    } else {
        original_policy_value_6 = ""_string;
    }

    event_init_dict.original_policy = original_policy_value_6;

    auto referrer_property_value_7 = JS::js_undefined();
    if (arg1.is_object())
        referrer_property_value_7 = TRY(arg1.as_object().get("referrer"_utf16_fly_string));

    String referrer_value_7;

    if (!referrer_property_value_7.is_undefined()) {
        if (!false || !referrer_property_value_7.is_null())
            referrer_value_7 = TRY(WebIDL::to_usv_string(vm, referrer_property_value_7));
    } else {
        referrer_value_7 = ""_string;
    }

    event_init_dict.referrer = referrer_value_7;

    auto sample_property_value_8 = JS::js_undefined();
    if (arg1.is_object())
        sample_property_value_8 = TRY(arg1.as_object().get("sample"_utf16_fly_string));

    String sample_value_8;

    if (!sample_property_value_8.is_undefined()) {
        if (!false || !sample_property_value_8.is_null())
            sample_value_8 = TRY(WebIDL::to_string(vm, sample_property_value_8));
    } else {
        sample_value_8 = ""_string;
    }

    event_init_dict.sample = sample_value_8;

    auto source_file_property_value_9 = JS::js_undefined();
    if (arg1.is_object())
        source_file_property_value_9 = TRY(arg1.as_object().get("sourceFile"_utf16_fly_string));

    String source_file_value_9;

    if (!source_file_property_value_9.is_undefined()) {
        if (!false || !source_file_property_value_9.is_null())
            source_file_value_9 = TRY(WebIDL::to_usv_string(vm, source_file_property_value_9));
    } else {
        source_file_value_9 = ""_string;
    }

    event_init_dict.source_file = source_file_value_9;

    auto status_code_property_value_10 = JS::js_undefined();
    if (arg1.is_object())
        status_code_property_value_10 = TRY(arg1.as_object().get("statusCode"_utf16_fly_string));

    WebIDL::UnsignedShort status_code_value_10;

    if (!status_code_property_value_10.is_undefined())

    status_code_value_10 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, status_code_property_value_10, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        status_code_value_10 = static_cast<WebIDL::UnsignedShort>(0);

    event_init_dict.status_code = status_code_value_10;

    auto violated_directive_property_value_11 = JS::js_undefined();
    if (arg1.is_object())
        violated_directive_property_value_11 = TRY(arg1.as_object().get("violatedDirective"_utf16_fly_string));

    String violated_directive_value_11;

    if (!violated_directive_property_value_11.is_undefined()) {
        if (!false || !violated_directive_property_value_11.is_null())
            violated_directive_value_11 = TRY(WebIDL::to_string(vm, violated_directive_property_value_11));
    } else {
        violated_directive_value_11 = ""_string;
    }

    event_init_dict.violated_directive = violated_directive_value_11;

    auto bubbles_property_value_12 = JS::js_undefined();
    if (arg1.is_object())
        bubbles_property_value_12 = TRY(arg1.as_object().get("bubbles"_utf16_fly_string));

    bool bubbles_value_12;

    if (!bubbles_property_value_12.is_undefined())

    bubbles_value_12 = bubbles_property_value_12.to_boolean();

    else
        bubbles_value_12 = static_cast<bool>(false);

    event_init_dict.bubbles = bubbles_value_12;

    auto cancelable_property_value_13 = JS::js_undefined();
    if (arg1.is_object())
        cancelable_property_value_13 = TRY(arg1.as_object().get("cancelable"_utf16_fly_string));

    bool cancelable_value_13;

    if (!cancelable_property_value_13.is_undefined())

    cancelable_value_13 = cancelable_property_value_13.to_boolean();

    else
        cancelable_value_13 = static_cast<bool>(false);

    event_init_dict.cancelable = cancelable_value_13;

    auto composed_property_value_14 = JS::js_undefined();
    if (arg1.is_object())
        composed_property_value_14 = TRY(arg1.as_object().get("composed"_utf16_fly_string));

    bool composed_value_14;

    if (!composed_property_value_14.is_undefined())

    composed_value_14 = composed_property_value_14.to_boolean();

    else
        composed_value_14 = static_cast<bool>(false);

    event_init_dict.composed = composed_value_14;

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return ContentSecurityPolicy::SecurityPolicyViolationEvent::construct_impl(realm, type, event_init_dict); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface SecurityPolicyViolationEvent" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within ContentSecurityPolicy::SecurityPolicyViolationEvent::construct_impl().
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


void SecurityPolicyViolationEventConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    set_prototype(&ensure_web_constructor<EventPrototype>(realm, "Event"_fly_string));

    define_direct_property(vm.names.length, JS::Value(1), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "SecurityPolicyViolationEvent"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<SecurityPolicyViolationEventPrototype>(realm, "SecurityPolicyViolationEvent"_fly_string), 0);


}

} // namespace Web::Bindings
