
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
#include <LibWeb/Bindings/SecurityPolicyViolationEventPrototype.h>
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

GC_DEFINE_ALLOCATOR(SecurityPolicyViolationEventPrototype);

SecurityPolicyViolationEventPrototype::SecurityPolicyViolationEventPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

SecurityPolicyViolationEventPrototype::~SecurityPolicyViolationEventPrototype()
{
}

void SecurityPolicyViolationEventPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventPrototype>(realm, "Event"_fly_string));


    auto native_document_uri_getter = JS::NativeFunction::create(realm, document_uri_getter, 0, "documentURI"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_document_uri_setter;

    define_direct_accessor("documentURI"_utf16_fly_string, native_document_uri_getter, native_document_uri_setter, default_attributes);

    auto native_referrer_getter = JS::NativeFunction::create(realm, referrer_getter, 0, "referrer"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_referrer_setter;

    define_direct_accessor("referrer"_utf16_fly_string, native_referrer_getter, native_referrer_setter, default_attributes);

    auto native_blocked_uri_getter = JS::NativeFunction::create(realm, blocked_uri_getter, 0, "blockedURI"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_blocked_uri_setter;

    define_direct_accessor("blockedURI"_utf16_fly_string, native_blocked_uri_getter, native_blocked_uri_setter, default_attributes);

    auto native_effective_directive_getter = JS::NativeFunction::create(realm, effective_directive_getter, 0, "effectiveDirective"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_effective_directive_setter;

    define_direct_accessor("effectiveDirective"_utf16_fly_string, native_effective_directive_getter, native_effective_directive_setter, default_attributes);

    auto native_violated_directive_getter = JS::NativeFunction::create(realm, violated_directive_getter, 0, "violatedDirective"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_violated_directive_setter;

    define_direct_accessor("violatedDirective"_utf16_fly_string, native_violated_directive_getter, native_violated_directive_setter, default_attributes);

    auto native_original_policy_getter = JS::NativeFunction::create(realm, original_policy_getter, 0, "originalPolicy"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_original_policy_setter;

    define_direct_accessor("originalPolicy"_utf16_fly_string, native_original_policy_getter, native_original_policy_setter, default_attributes);

    auto native_source_file_getter = JS::NativeFunction::create(realm, source_file_getter, 0, "sourceFile"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_source_file_setter;

    define_direct_accessor("sourceFile"_utf16_fly_string, native_source_file_getter, native_source_file_setter, default_attributes);

    auto native_sample_getter = JS::NativeFunction::create(realm, sample_getter, 0, "sample"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_sample_setter;

    define_direct_accessor("sample"_utf16_fly_string, native_sample_getter, native_sample_setter, default_attributes);

    auto native_disposition_getter = JS::NativeFunction::create(realm, disposition_getter, 0, "disposition"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_disposition_setter;

    define_direct_accessor("disposition"_utf16_fly_string, native_disposition_getter, native_disposition_setter, default_attributes);

    auto native_status_code_getter = JS::NativeFunction::create(realm, status_code_getter, 0, "statusCode"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_status_code_setter;

    define_direct_accessor("statusCode"_utf16_fly_string, native_status_code_getter, native_status_code_setter, default_attributes);

    auto native_line_number_getter = JS::NativeFunction::create(realm, line_number_getter, 0, "lineNumber"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_line_number_setter;

    define_direct_accessor("lineNumber"_utf16_fly_string, native_line_number_getter, native_line_number_setter, default_attributes);

    auto native_column_number_getter = JS::NativeFunction::create(realm, column_number_getter, 0, "columnNumber"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_column_number_setter;

    define_direct_accessor("columnNumber"_utf16_fly_string, native_column_number_getter, native_column_number_setter, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "SecurityPolicyViolationEvent"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void SecurityPolicyViolationEventPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<ContentSecurityPolicy::SecurityPolicyViolationEvent*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<ContentSecurityPolicy::SecurityPolicyViolationEvent>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SecurityPolicyViolationEvent");
    return static_cast<ContentSecurityPolicy::SecurityPolicyViolationEvent*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(SecurityPolicyViolationEventPrototype::document_uri_getter)
{
    WebIDL::log_trace(vm, "SecurityPolicyViolationEventPrototype::document_uri_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->document_uri(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(SecurityPolicyViolationEventPrototype::referrer_getter)
{
    WebIDL::log_trace(vm, "SecurityPolicyViolationEventPrototype::referrer_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->referrer(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(SecurityPolicyViolationEventPrototype::blocked_uri_getter)
{
    WebIDL::log_trace(vm, "SecurityPolicyViolationEventPrototype::blocked_uri_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->blocked_uri(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(SecurityPolicyViolationEventPrototype::effective_directive_getter)
{
    WebIDL::log_trace(vm, "SecurityPolicyViolationEventPrototype::effective_directive_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->effective_directive(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(SecurityPolicyViolationEventPrototype::violated_directive_getter)
{
    WebIDL::log_trace(vm, "SecurityPolicyViolationEventPrototype::violated_directive_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->violated_directive(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(SecurityPolicyViolationEventPrototype::original_policy_getter)
{
    WebIDL::log_trace(vm, "SecurityPolicyViolationEventPrototype::original_policy_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->original_policy(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(SecurityPolicyViolationEventPrototype::source_file_getter)
{
    WebIDL::log_trace(vm, "SecurityPolicyViolationEventPrototype::source_file_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->source_file(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(SecurityPolicyViolationEventPrototype::sample_getter)
{
    WebIDL::log_trace(vm, "SecurityPolicyViolationEventPrototype::sample_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->sample(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(SecurityPolicyViolationEventPrototype::disposition_getter)
{
    WebIDL::log_trace(vm, "SecurityPolicyViolationEventPrototype::disposition_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->disposition(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(SecurityPolicyViolationEventPrototype::status_code_getter)
{
    WebIDL::log_trace(vm, "SecurityPolicyViolationEventPrototype::status_code_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->status_code(); }));

    return JS::Value(static_cast<WebIDL::UnsignedShort>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(SecurityPolicyViolationEventPrototype::line_number_getter)
{
    WebIDL::log_trace(vm, "SecurityPolicyViolationEventPrototype::line_number_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->line_number(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(SecurityPolicyViolationEventPrototype::column_number_getter)
{
    WebIDL::log_trace(vm, "SecurityPolicyViolationEventPrototype::column_number_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->column_number(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

} // namespace Web::Bindings
