
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
#include <LibWeb/Bindings/CSSNumericValuePrototype.h>
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

#if __has_include(<LibWeb/Bindings/CSSStyleValuePrototype.h>)
#    include <LibWeb/Bindings/CSSStyleValuePrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSNumericValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSUnitValue.h>

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

GC_DEFINE_ALLOCATOR(CSSNumericValuePrototype);

CSSNumericValuePrototype::CSSNumericValuePrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

CSSNumericValuePrototype::~CSSNumericValuePrototype()
{
}

void CSSNumericValuePrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<CSSStyleValuePrototype>(realm, "CSSStyleValue"_fly_string));


    define_native_function(realm, "add"_utf16_fly_string, add, 0, default_attributes);

    define_native_function(realm, "sub"_utf16_fly_string, sub, 0, default_attributes);

    define_native_function(realm, "mul"_utf16_fly_string, mul, 0, default_attributes);

    define_native_function(realm, "div"_utf16_fly_string, div, 0, default_attributes);

    define_native_function(realm, "min"_utf16_fly_string, min, 0, default_attributes);

    define_native_function(realm, "max"_utf16_fly_string, max, 0, default_attributes);

    define_native_function(realm, "equals"_utf16_fly_string, equals, 0, default_attributes);

    define_native_function(realm, "to"_utf16_fly_string, to, 1, default_attributes);

    define_native_function(realm, "type"_utf16_fly_string, type, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "CSSNumericValue"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void CSSNumericValuePrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<CSS::CSSNumericValue*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<CSS::CSSNumericValue>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CSSNumericValue");
    return static_cast<CSS::CSSNumericValue*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(CSSNumericValuePrototype::add)
{
    WebIDL::log_trace(vm, "CSSNumericValuePrototype::add");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<double, GC::Root<CSSNumericValue>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<CSSNumericValue>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

        if (arg0.is_number()) {

    double arg0_number = TRY(arg0.to_double(vm));

    if (isinf(arg0_number) || isnan(arg0_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "values");
    }
    
            return { arg0_number };
        }

    double arg0_number = TRY(arg0.to_double(vm));

    if (isinf(arg0_number) || isnan(arg0_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "values");
    }
    
        return { arg0_number };

    };

        Vector<Variant<double, GC::Root<CSSNumericValue>>> values;

        if (vm.argument_count() > 0) {
            values.ensure_capacity(vm.argument_count() - 0);

            for (size_t i = 0; i < vm.argument_count(); ++i) {
                auto result = TRY(arg0_to_variant(vm.argument(i)));
                values.unchecked_append(move(result));
            }
        }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->add(move(values)); }));

    return &const_cast<CSSNumericValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNumericValuePrototype::sub)
{
    WebIDL::log_trace(vm, "CSSNumericValuePrototype::sub");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<double, GC::Root<CSSNumericValue>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<CSSNumericValue>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

        if (arg0.is_number()) {

    double arg0_number = TRY(arg0.to_double(vm));

    if (isinf(arg0_number) || isnan(arg0_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "values");
    }
    
            return { arg0_number };
        }

    double arg0_number = TRY(arg0.to_double(vm));

    if (isinf(arg0_number) || isnan(arg0_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "values");
    }
    
        return { arg0_number };

    };

        Vector<Variant<double, GC::Root<CSSNumericValue>>> values;

        if (vm.argument_count() > 0) {
            values.ensure_capacity(vm.argument_count() - 0);

            for (size_t i = 0; i < vm.argument_count(); ++i) {
                auto result = TRY(arg0_to_variant(vm.argument(i)));
                values.unchecked_append(move(result));
            }
        }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->sub(move(values)); }));

    return &const_cast<CSSNumericValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNumericValuePrototype::mul)
{
    WebIDL::log_trace(vm, "CSSNumericValuePrototype::mul");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<double, GC::Root<CSSNumericValue>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<CSSNumericValue>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

        if (arg0.is_number()) {

    double arg0_number = TRY(arg0.to_double(vm));

    if (isinf(arg0_number) || isnan(arg0_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "values");
    }
    
            return { arg0_number };
        }

    double arg0_number = TRY(arg0.to_double(vm));

    if (isinf(arg0_number) || isnan(arg0_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "values");
    }
    
        return { arg0_number };

    };

        Vector<Variant<double, GC::Root<CSSNumericValue>>> values;

        if (vm.argument_count() > 0) {
            values.ensure_capacity(vm.argument_count() - 0);

            for (size_t i = 0; i < vm.argument_count(); ++i) {
                auto result = TRY(arg0_to_variant(vm.argument(i)));
                values.unchecked_append(move(result));
            }
        }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->mul(move(values)); }));

    return &const_cast<CSSNumericValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNumericValuePrototype::div)
{
    WebIDL::log_trace(vm, "CSSNumericValuePrototype::div");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<double, GC::Root<CSSNumericValue>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<CSSNumericValue>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

        if (arg0.is_number()) {

    double arg0_number = TRY(arg0.to_double(vm));

    if (isinf(arg0_number) || isnan(arg0_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "values");
    }
    
            return { arg0_number };
        }

    double arg0_number = TRY(arg0.to_double(vm));

    if (isinf(arg0_number) || isnan(arg0_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "values");
    }
    
        return { arg0_number };

    };

        Vector<Variant<double, GC::Root<CSSNumericValue>>> values;

        if (vm.argument_count() > 0) {
            values.ensure_capacity(vm.argument_count() - 0);

            for (size_t i = 0; i < vm.argument_count(); ++i) {
                auto result = TRY(arg0_to_variant(vm.argument(i)));
                values.unchecked_append(move(result));
            }
        }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->div(move(values)); }));

    return &const_cast<CSSNumericValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNumericValuePrototype::min)
{
    WebIDL::log_trace(vm, "CSSNumericValuePrototype::min");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<double, GC::Root<CSSNumericValue>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<CSSNumericValue>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

        if (arg0.is_number()) {

    double arg0_number = TRY(arg0.to_double(vm));

    if (isinf(arg0_number) || isnan(arg0_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "values");
    }
    
            return { arg0_number };
        }

    double arg0_number = TRY(arg0.to_double(vm));

    if (isinf(arg0_number) || isnan(arg0_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "values");
    }
    
        return { arg0_number };

    };

        Vector<Variant<double, GC::Root<CSSNumericValue>>> values;

        if (vm.argument_count() > 0) {
            values.ensure_capacity(vm.argument_count() - 0);

            for (size_t i = 0; i < vm.argument_count(); ++i) {
                auto result = TRY(arg0_to_variant(vm.argument(i)));
                values.unchecked_append(move(result));
            }
        }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->min(move(values)); }));

    return &const_cast<CSSNumericValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNumericValuePrototype::max)
{
    WebIDL::log_trace(vm, "CSSNumericValuePrototype::max");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<double, GC::Root<CSSNumericValue>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<CSSNumericValue>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

        if (arg0.is_number()) {

    double arg0_number = TRY(arg0.to_double(vm));

    if (isinf(arg0_number) || isnan(arg0_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "values");
    }
    
            return { arg0_number };
        }

    double arg0_number = TRY(arg0.to_double(vm));

    if (isinf(arg0_number) || isnan(arg0_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "values");
    }
    
        return { arg0_number };

    };

        Vector<Variant<double, GC::Root<CSSNumericValue>>> values;

        if (vm.argument_count() > 0) {
            values.ensure_capacity(vm.argument_count() - 0);

            for (size_t i = 0; i < vm.argument_count(); ++i) {
                auto result = TRY(arg0_to_variant(vm.argument(i)));
                values.unchecked_append(move(result));
            }
        }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->max(move(values)); }));

    return &const_cast<CSSNumericValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNumericValuePrototype::equals)
{
    WebIDL::log_trace(vm, "CSSNumericValuePrototype::equals");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<double, GC::Root<CSSNumericValue>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<CSSNumericValue>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

        if (arg0.is_number()) {

    double arg0_number = TRY(arg0.to_double(vm));

    if (isinf(arg0_number) || isnan(arg0_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
            return { arg0_number };
        }

    double arg0_number = TRY(arg0.to_double(vm));

    if (isinf(arg0_number) || isnan(arg0_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
        return { arg0_number };

    };

        Vector<Variant<double, GC::Root<CSSNumericValue>>> value;

        if (vm.argument_count() > 0) {
            value.ensure_capacity(vm.argument_count() - 0);

            for (size_t i = 0; i < vm.argument_count(); ++i) {
                auto result = TRY(arg0_to_variant(vm.argument(i)));
                value.unchecked_append(move(result));
            }
        }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->equals_for_bindings(move(value)); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNumericValuePrototype::to)
{
    WebIDL::log_trace(vm, "CSSNumericValuePrototype::to");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "to");

    auto arg0 = vm.argument(0);

    String unit;
    if (!false || !arg0.is_null()) {
        unit = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->to(unit); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNumericValuePrototype::type)
{
    WebIDL::log_trace(vm, "CSSNumericValuePrototype::type");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->type_for_bindings(); }));

    {
        auto dictionary_object0 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        Optional<JS::Value> wrapped_angle0_value;

    if (retval.angle.has_value()) {

    wrapped_angle0_value = JS::Value(static_cast<WebIDL::Long>(retval.angle.release_value()));

    }

        if (wrapped_angle0_value.has_value())
            MUST(dictionary_object0->create_data_property("angle"_utf16_fly_string, wrapped_angle0_value.release_value()));

        Optional<JS::Value> wrapped_flex0_value;

    if (retval.flex.has_value()) {

    wrapped_flex0_value = JS::Value(static_cast<WebIDL::Long>(retval.flex.release_value()));

    }

        if (wrapped_flex0_value.has_value())
            MUST(dictionary_object0->create_data_property("flex"_utf16_fly_string, wrapped_flex0_value.release_value()));

        Optional<JS::Value> wrapped_frequency0_value;

    if (retval.frequency.has_value()) {

    wrapped_frequency0_value = JS::Value(static_cast<WebIDL::Long>(retval.frequency.release_value()));

    }

        if (wrapped_frequency0_value.has_value())
            MUST(dictionary_object0->create_data_property("frequency"_utf16_fly_string, wrapped_frequency0_value.release_value()));

        Optional<JS::Value> wrapped_length0_value;

    if (retval.length.has_value()) {

    wrapped_length0_value = JS::Value(static_cast<WebIDL::Long>(retval.length.release_value()));

    }

        if (wrapped_length0_value.has_value())
            MUST(dictionary_object0->create_data_property("length"_utf16_fly_string, wrapped_length0_value.release_value()));

        Optional<JS::Value> wrapped_percent0_value;

    if (retval.percent.has_value()) {

    wrapped_percent0_value = JS::Value(static_cast<WebIDL::Long>(retval.percent.release_value()));

    }

        if (wrapped_percent0_value.has_value())
            MUST(dictionary_object0->create_data_property("percent"_utf16_fly_string, wrapped_percent0_value.release_value()));

        Optional<JS::Value> wrapped_percent_hint0_value;

    if (retval.percent_hint.has_value()) {

    wrapped_percent_hint0_value = JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval.percent_hint.value()));

    }

        if (wrapped_percent_hint0_value.has_value())
            MUST(dictionary_object0->create_data_property("percentHint"_utf16_fly_string, wrapped_percent_hint0_value.release_value()));

        Optional<JS::Value> wrapped_resolution0_value;

    if (retval.resolution.has_value()) {

    wrapped_resolution0_value = JS::Value(static_cast<WebIDL::Long>(retval.resolution.release_value()));

    }

        if (wrapped_resolution0_value.has_value())
            MUST(dictionary_object0->create_data_property("resolution"_utf16_fly_string, wrapped_resolution0_value.release_value()));

        Optional<JS::Value> wrapped_time0_value;

    if (retval.time.has_value()) {

    wrapped_time0_value = JS::Value(static_cast<WebIDL::Long>(retval.time.release_value()));

    }

        if (wrapped_time0_value.has_value())
            MUST(dictionary_object0->create_data_property("time"_utf16_fly_string, wrapped_time0_value.release_value()));

        return dictionary_object0;
    }

}

} // namespace Web::Bindings
