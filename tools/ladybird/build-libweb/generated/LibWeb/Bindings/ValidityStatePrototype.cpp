
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
#include <LibWeb/Bindings/ValidityStatePrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ValidityState.h>

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

GC_DEFINE_ALLOCATOR(ValidityStatePrototype);

ValidityStatePrototype::ValidityStatePrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

ValidityStatePrototype::~ValidityStatePrototype()
{
}

void ValidityStatePrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_value_missing_getter = JS::NativeFunction::create(realm, value_missing_getter, 0, "valueMissing"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_value_missing_setter;

    define_direct_accessor("valueMissing"_utf16_fly_string, native_value_missing_getter, native_value_missing_setter, default_attributes);

    auto native_type_mismatch_getter = JS::NativeFunction::create(realm, type_mismatch_getter, 0, "typeMismatch"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_type_mismatch_setter;

    define_direct_accessor("typeMismatch"_utf16_fly_string, native_type_mismatch_getter, native_type_mismatch_setter, default_attributes);

    auto native_pattern_mismatch_getter = JS::NativeFunction::create(realm, pattern_mismatch_getter, 0, "patternMismatch"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_pattern_mismatch_setter;

    define_direct_accessor("patternMismatch"_utf16_fly_string, native_pattern_mismatch_getter, native_pattern_mismatch_setter, default_attributes);

    auto native_too_long_getter = JS::NativeFunction::create(realm, too_long_getter, 0, "tooLong"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_too_long_setter;

    define_direct_accessor("tooLong"_utf16_fly_string, native_too_long_getter, native_too_long_setter, default_attributes);

    auto native_too_short_getter = JS::NativeFunction::create(realm, too_short_getter, 0, "tooShort"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_too_short_setter;

    define_direct_accessor("tooShort"_utf16_fly_string, native_too_short_getter, native_too_short_setter, default_attributes);

    auto native_range_underflow_getter = JS::NativeFunction::create(realm, range_underflow_getter, 0, "rangeUnderflow"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_range_underflow_setter;

    define_direct_accessor("rangeUnderflow"_utf16_fly_string, native_range_underflow_getter, native_range_underflow_setter, default_attributes);

    auto native_range_overflow_getter = JS::NativeFunction::create(realm, range_overflow_getter, 0, "rangeOverflow"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_range_overflow_setter;

    define_direct_accessor("rangeOverflow"_utf16_fly_string, native_range_overflow_getter, native_range_overflow_setter, default_attributes);

    auto native_step_mismatch_getter = JS::NativeFunction::create(realm, step_mismatch_getter, 0, "stepMismatch"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_step_mismatch_setter;

    define_direct_accessor("stepMismatch"_utf16_fly_string, native_step_mismatch_getter, native_step_mismatch_setter, default_attributes);

    auto native_bad_input_getter = JS::NativeFunction::create(realm, bad_input_getter, 0, "badInput"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_bad_input_setter;

    define_direct_accessor("badInput"_utf16_fly_string, native_bad_input_getter, native_bad_input_setter, default_attributes);

    auto native_custom_error_getter = JS::NativeFunction::create(realm, custom_error_getter, 0, "customError"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_custom_error_setter;

    define_direct_accessor("customError"_utf16_fly_string, native_custom_error_getter, native_custom_error_setter, default_attributes);

    auto native_valid_getter = JS::NativeFunction::create(realm, valid_getter, 0, "valid"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_valid_setter;

    define_direct_accessor("valid"_utf16_fly_string, native_valid_getter, native_valid_setter, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "ValidityState"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void ValidityStatePrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::ValidityState*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::ValidityState>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ValidityState");
    return static_cast<HTML::ValidityState*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(ValidityStatePrototype::value_missing_getter)
{
    WebIDL::log_trace(vm, "ValidityStatePrototype::value_missing_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->value_missing(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ValidityStatePrototype::type_mismatch_getter)
{
    WebIDL::log_trace(vm, "ValidityStatePrototype::type_mismatch_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->type_mismatch(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ValidityStatePrototype::pattern_mismatch_getter)
{
    WebIDL::log_trace(vm, "ValidityStatePrototype::pattern_mismatch_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->pattern_mismatch(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ValidityStatePrototype::too_long_getter)
{
    WebIDL::log_trace(vm, "ValidityStatePrototype::too_long_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->too_long(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ValidityStatePrototype::too_short_getter)
{
    WebIDL::log_trace(vm, "ValidityStatePrototype::too_short_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->too_short(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ValidityStatePrototype::range_underflow_getter)
{
    WebIDL::log_trace(vm, "ValidityStatePrototype::range_underflow_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->range_underflow(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ValidityStatePrototype::range_overflow_getter)
{
    WebIDL::log_trace(vm, "ValidityStatePrototype::range_overflow_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->range_overflow(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ValidityStatePrototype::step_mismatch_getter)
{
    WebIDL::log_trace(vm, "ValidityStatePrototype::step_mismatch_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->step_mismatch(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ValidityStatePrototype::bad_input_getter)
{
    WebIDL::log_trace(vm, "ValidityStatePrototype::bad_input_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bad_input(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ValidityStatePrototype::custom_error_getter)
{
    WebIDL::log_trace(vm, "ValidityStatePrototype::custom_error_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->custom_error(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ValidityStatePrototype::valid_getter)
{
    WebIDL::log_trace(vm, "ValidityStatePrototype::valid_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->valid(); }));

    return JS::Value(retval);

}

} // namespace Web::Bindings
