
#include <AK/Function.h>
#include <LibIDL/Types.h>
#include <LibJS/Runtime/ArrayBuffer.h>
#include <LibJS/Runtime/DataView.h>
#include <LibJS/Runtime/Error.h>
#include <LibJS/Runtime/PrimitiveString.h>
#include <LibJS/Runtime/PromiseConstructor.h>
#include <LibJS/Runtime/TypedArray.h>
#include <LibJS/Runtime/Value.h>
#include <LibJS/Runtime/ValueInlines.h>
#include <LibWeb/Bindings/CSSNamespace.h>
#include <LibWeb/Bindings/ExceptionOrUtils.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/HTML/Window.h>
#include <LibWeb/HTML/WindowProxy.h>
#include <LibWeb/WebIDL/AbstractOperations.h>
#include <LibWeb/WebIDL/Buffers.h>
#include <LibWeb/WebIDL/OverloadResolution.h>
#include <LibWeb/WebIDL/Promise.h>
#include <LibWeb/WebIDL/Tracing.h>
#include <LibWeb/WebIDL/Types.h>


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSS.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSUnitValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSNumericValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleValue.h>

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

GC_DEFINE_ALLOCATOR(CSSNamespace);

CSSNamespace::CSSNamespace(JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())
{
}

CSSNamespace::~CSSNamespace()
{
}

void CSSNamespace::initialize(JS::Realm& realm)
{
    [[maybe_unused]] auto& vm = this->vm();

    Base::initialize(realm);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "CSS"_string), JS::Attribute::Configurable);


    define_native_function(realm, "escape"_utf16_fly_string, escape, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "supports"_utf16_fly_string, supports, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "registerProperty"_utf16_fly_string, register_property, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "number"_utf16_fly_string, number, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "percent"_utf16_fly_string, percent, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "deg"_utf16_fly_string, deg, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "grad"_utf16_fly_string, grad, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "rad"_utf16_fly_string, rad, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "turn"_utf16_fly_string, turn, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "fr"_utf16_fly_string, fr, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "Hz"_utf16_fly_string, hz, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "kHz"_utf16_fly_string, k_hz, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "cap"_utf16_fly_string, cap, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "ch"_utf16_fly_string, ch, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "cm"_utf16_fly_string, cm, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "dvb"_utf16_fly_string, dvb, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "dvh"_utf16_fly_string, dvh, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "dvi"_utf16_fly_string, dvi, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "dvmax"_utf16_fly_string, dvmax, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "dvmin"_utf16_fly_string, dvmin, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "dvw"_utf16_fly_string, dvw, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "em"_utf16_fly_string, em, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "ex"_utf16_fly_string, ex, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "ic"_utf16_fly_string, ic, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "in"_utf16_fly_string, in, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "lh"_utf16_fly_string, lh, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "lvb"_utf16_fly_string, lvb, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "lvh"_utf16_fly_string, lvh, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "lvi"_utf16_fly_string, lvi, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "lvmax"_utf16_fly_string, lvmax, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "lvmin"_utf16_fly_string, lvmin, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "lvw"_utf16_fly_string, lvw, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "mm"_utf16_fly_string, mm, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "pc"_utf16_fly_string, pc, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "pt"_utf16_fly_string, pt, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "px"_utf16_fly_string, px, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "Q"_utf16_fly_string, q, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "rcap"_utf16_fly_string, rcap, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "rch"_utf16_fly_string, rch, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "rem"_utf16_fly_string, rem, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "rex"_utf16_fly_string, rex, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "ric"_utf16_fly_string, ric, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "rlh"_utf16_fly_string, rlh, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "svb"_utf16_fly_string, svb, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "svh"_utf16_fly_string, svh, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "svi"_utf16_fly_string, svi, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "svmax"_utf16_fly_string, svmax, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "svmin"_utf16_fly_string, svmin, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "svw"_utf16_fly_string, svw, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "vb"_utf16_fly_string, vb, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "vh"_utf16_fly_string, vh, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "vi"_utf16_fly_string, vi, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "vmax"_utf16_fly_string, vmax, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "vmin"_utf16_fly_string, vmin, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "vw"_utf16_fly_string, vw, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "dpcm"_utf16_fly_string, dpcm, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "dpi"_utf16_fly_string, dpi, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "dppx"_utf16_fly_string, dppx, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "x"_utf16_fly_string, x, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "ms"_utf16_fly_string, ms, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "s"_utf16_fly_string, s, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::escape)
{
    WebIDL::log_trace(vm, "CSSNamespace::escape");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "escape");

    auto arg0 = vm.argument(0);

    String ident;
    if (!false || !arg0.is_null()) {
        ident = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::escape(vm, ident); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::supports0)
{
    WebIDL::log_trace(vm, "CSSNamespace::supports0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto arg0 = vm.argument(0);

    String property;
    if (!false || !arg0.is_null()) {
        property = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    String value;
    if (!false || !arg1.is_null()) {
        value = TRY(WebIDL::to_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::supports(vm, property, value); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::supports1)
{
    WebIDL::log_trace(vm, "CSSNamespace::supports1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto arg0 = vm.argument(0);

    String condition_text;
    if (!false || !arg0.is_null()) {
        condition_text = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::supports(vm, condition_text); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::register_property)
{
    WebIDL::log_trace(vm, "CSSNamespace::register_property");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "registerProperty");

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PropertyDefinition");

    PropertyDefinition definition {};

    auto inherits_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        inherits_property_value_0 = TRY(arg0.as_object().get("inherits"_utf16_fly_string));

    if (inherits_property_value_0.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "inherits");

    bool inherits_value_0;

    inherits_value_0 = inherits_property_value_0.to_boolean();

    definition.inherits = inherits_value_0;

    auto initial_value_property_value_1 = JS::js_undefined();
    if (arg0.is_object())
        initial_value_property_value_1 = TRY(arg0.as_object().get("initialValue"_utf16_fly_string));

    if (!initial_value_property_value_1.is_undefined()) {

    Optional<String> initial_value_value_1;

    if (!initial_value_property_value_1.is_undefined()) {
        if (!false || !initial_value_property_value_1.is_null())
            initial_value_value_1 = TRY(WebIDL::to_string(vm, initial_value_property_value_1));
    }

    definition.initial_value = initial_value_value_1;

    }

    auto name_property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        name_property_value_2 = TRY(arg0.as_object().get("name"_utf16_fly_string));

    if (name_property_value_2.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "name");

    String name_value_2;
    if (!false || !name_property_value_2.is_null()) {
        name_value_2 = TRY(WebIDL::to_string(vm, name_property_value_2));
    }

    definition.name = name_value_2;

    auto syntax_property_value_3 = JS::js_undefined();
    if (arg0.is_object())
        syntax_property_value_3 = TRY(arg0.as_object().get("syntax"_utf16_fly_string));

    String syntax_value_3;

    if (!syntax_property_value_3.is_undefined()) {
        if (!false || !syntax_property_value_3.is_null())
            syntax_value_3 = TRY(WebIDL::to_string(vm, syntax_property_value_3));
    } else {
        syntax_value_3 = "*"_string;
    }

    definition.syntax = syntax_value_3;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::register_property(vm, definition); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::number)
{
    WebIDL::log_trace(vm, "CSSNamespace::number");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "number");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::number(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::percent)
{
    WebIDL::log_trace(vm, "CSSNamespace::percent");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "percent");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::percent(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::deg)
{
    WebIDL::log_trace(vm, "CSSNamespace::deg");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "deg");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::deg(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::grad)
{
    WebIDL::log_trace(vm, "CSSNamespace::grad");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "grad");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::grad(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::rad)
{
    WebIDL::log_trace(vm, "CSSNamespace::rad");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "rad");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::rad(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::turn)
{
    WebIDL::log_trace(vm, "CSSNamespace::turn");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "turn");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::turn(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::fr)
{
    WebIDL::log_trace(vm, "CSSNamespace::fr");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "fr");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::fr(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::hz)
{
    WebIDL::log_trace(vm, "CSSNamespace::hz");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "Hz");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::hz(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::k_hz)
{
    WebIDL::log_trace(vm, "CSSNamespace::k_hz");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "kHz");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::khz(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::cap)
{
    WebIDL::log_trace(vm, "CSSNamespace::cap");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "cap");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::cap(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::ch)
{
    WebIDL::log_trace(vm, "CSSNamespace::ch");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "ch");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::ch(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::cm)
{
    WebIDL::log_trace(vm, "CSSNamespace::cm");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "cm");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::cm(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::dvb)
{
    WebIDL::log_trace(vm, "CSSNamespace::dvb");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "dvb");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::dvb(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::dvh)
{
    WebIDL::log_trace(vm, "CSSNamespace::dvh");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "dvh");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::dvh(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::dvi)
{
    WebIDL::log_trace(vm, "CSSNamespace::dvi");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "dvi");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::dvi(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::dvmax)
{
    WebIDL::log_trace(vm, "CSSNamespace::dvmax");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "dvmax");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::dvmax(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::dvmin)
{
    WebIDL::log_trace(vm, "CSSNamespace::dvmin");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "dvmin");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::dvmin(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::dvw)
{
    WebIDL::log_trace(vm, "CSSNamespace::dvw");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "dvw");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::dvw(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::em)
{
    WebIDL::log_trace(vm, "CSSNamespace::em");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "em");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::em(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::ex)
{
    WebIDL::log_trace(vm, "CSSNamespace::ex");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "ex");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::ex(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::ic)
{
    WebIDL::log_trace(vm, "CSSNamespace::ic");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "ic");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::ic(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::in)
{
    WebIDL::log_trace(vm, "CSSNamespace::in");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "in");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::in(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::lh)
{
    WebIDL::log_trace(vm, "CSSNamespace::lh");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "lh");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::lh(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::lvb)
{
    WebIDL::log_trace(vm, "CSSNamespace::lvb");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "lvb");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::lvb(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::lvh)
{
    WebIDL::log_trace(vm, "CSSNamespace::lvh");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "lvh");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::lvh(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::lvi)
{
    WebIDL::log_trace(vm, "CSSNamespace::lvi");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "lvi");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::lvi(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::lvmax)
{
    WebIDL::log_trace(vm, "CSSNamespace::lvmax");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "lvmax");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::lvmax(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::lvmin)
{
    WebIDL::log_trace(vm, "CSSNamespace::lvmin");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "lvmin");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::lvmin(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::lvw)
{
    WebIDL::log_trace(vm, "CSSNamespace::lvw");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "lvw");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::lvw(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::mm)
{
    WebIDL::log_trace(vm, "CSSNamespace::mm");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "mm");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::mm(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::pc)
{
    WebIDL::log_trace(vm, "CSSNamespace::pc");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "pc");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::pc(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::pt)
{
    WebIDL::log_trace(vm, "CSSNamespace::pt");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "pt");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::pt(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::px)
{
    WebIDL::log_trace(vm, "CSSNamespace::px");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "px");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::px(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::q)
{
    WebIDL::log_trace(vm, "CSSNamespace::q");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "Q");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::q(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::rcap)
{
    WebIDL::log_trace(vm, "CSSNamespace::rcap");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "rcap");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::rcap(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::rch)
{
    WebIDL::log_trace(vm, "CSSNamespace::rch");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "rch");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::rch(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::rem)
{
    WebIDL::log_trace(vm, "CSSNamespace::rem");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "rem");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::rem(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::rex)
{
    WebIDL::log_trace(vm, "CSSNamespace::rex");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "rex");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::rex(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::ric)
{
    WebIDL::log_trace(vm, "CSSNamespace::ric");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "ric");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::ric(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::rlh)
{
    WebIDL::log_trace(vm, "CSSNamespace::rlh");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "rlh");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::rlh(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::svb)
{
    WebIDL::log_trace(vm, "CSSNamespace::svb");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "svb");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::svb(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::svh)
{
    WebIDL::log_trace(vm, "CSSNamespace::svh");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "svh");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::svh(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::svi)
{
    WebIDL::log_trace(vm, "CSSNamespace::svi");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "svi");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::svi(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::svmax)
{
    WebIDL::log_trace(vm, "CSSNamespace::svmax");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "svmax");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::svmax(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::svmin)
{
    WebIDL::log_trace(vm, "CSSNamespace::svmin");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "svmin");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::svmin(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::svw)
{
    WebIDL::log_trace(vm, "CSSNamespace::svw");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "svw");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::svw(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::vb)
{
    WebIDL::log_trace(vm, "CSSNamespace::vb");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "vb");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::vb(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::vh)
{
    WebIDL::log_trace(vm, "CSSNamespace::vh");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "vh");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::vh(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::vi)
{
    WebIDL::log_trace(vm, "CSSNamespace::vi");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "vi");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::vi(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::vmax)
{
    WebIDL::log_trace(vm, "CSSNamespace::vmax");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "vmax");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::vmax(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::vmin)
{
    WebIDL::log_trace(vm, "CSSNamespace::vmin");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "vmin");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::vmin(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::vw)
{
    WebIDL::log_trace(vm, "CSSNamespace::vw");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "vw");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::vw(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::dpcm)
{
    WebIDL::log_trace(vm, "CSSNamespace::dpcm");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "dpcm");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::dpcm(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::dpi)
{
    WebIDL::log_trace(vm, "CSSNamespace::dpi");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "dpi");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::dpi(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::dppx)
{
    WebIDL::log_trace(vm, "CSSNamespace::dppx");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "dppx");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::dppx(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::x)
{
    WebIDL::log_trace(vm, "CSSNamespace::x");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "x");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::x(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::ms)
{
    WebIDL::log_trace(vm, "CSSNamespace::ms");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "ms");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::ms(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::s)
{
    WebIDL::log_trace(vm, "CSSNamespace::s");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "s");

    auto arg0 = vm.argument(0);

    double value = TRY(arg0.to_double(vm));

    if (isinf(value) || isnan(value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "value");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::s(vm, value); }));

    return &const_cast<CSSUnitValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSNamespace::supports)
{
    WebIDL::log_trace(vm, "CSSNamespace::supports");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(2, vm.argument_count())) {

    case 1:
        chosen_overload_callable_id = 1;
        break;


    case 2:
        chosen_overload_callable_id = 0;
        break;


    }

    Vector<StringView> dictionary_types {
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return supports0(vm);

    case 1:

        return supports1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

} // namespace Web::Bindings
