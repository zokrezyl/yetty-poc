
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
#include <LibWeb/Bindings/CSSScalePrototype.h>
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

#if __has_include(<LibWeb/Bindings/CSSTransformComponentPrototype.h>)
#    include <LibWeb/Bindings/CSSTransformComponentPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSScale.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSNumericValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSTransformComponent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSUnitValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrix.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrixReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPoint.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPointReadOnly.h>

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

GC_DEFINE_ALLOCATOR(CSSScalePrototype);

CSSScalePrototype::CSSScalePrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

CSSScalePrototype::~CSSScalePrototype()
{
}

void CSSScalePrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<CSSTransformComponentPrototype>(realm, "CSSTransformComponent"_fly_string));


    auto native_x_getter = JS::NativeFunction::create(realm, x_getter, 0, "x"_utf16_fly_string, &realm, "get"sv);

    auto native_x_setter = JS::NativeFunction::create(realm, x_setter, 1, "x"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("x"_utf16_fly_string, native_x_getter, native_x_setter, default_attributes);

    auto native_y_getter = JS::NativeFunction::create(realm, y_getter, 0, "y"_utf16_fly_string, &realm, "get"sv);

    auto native_y_setter = JS::NativeFunction::create(realm, y_setter, 1, "y"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("y"_utf16_fly_string, native_y_getter, native_y_setter, default_attributes);

    auto native_z_getter = JS::NativeFunction::create(realm, z_getter, 0, "z"_utf16_fly_string, &realm, "get"sv);

    auto native_z_setter = JS::NativeFunction::create(realm, z_setter, 1, "z"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("z"_utf16_fly_string, native_z_getter, native_z_setter, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "CSSScale"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void CSSScalePrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<CSS::CSSScale*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<CSS::CSSScale>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CSSScale");
    return static_cast<CSS::CSSScale*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(CSSScalePrototype::x_getter)
{
    WebIDL::log_trace(vm, "CSSScalePrototype::x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->x(); }));

    return retval.visit(

        [&vm, &realm]([[maybe_unused]] double const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::Value(visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<CSSNumericValue> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<CSSNumericValue&>(*visited_union_value0);

        }

    );

}

JS_DEFINE_NATIVE_FUNCTION(CSSScalePrototype::x_setter)
{
    WebIDL::log_trace(vm, "CSSScalePrototype::x_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    auto value_to_variant = [&vm, &realm](JS::Value value) -> JS::ThrowCompletionOr<Variant<double, GC::Root<CSSNumericValue>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (value.is_object()) {
            [[maybe_unused]] auto& value_object = value.as_object();

            if (is<PlatformObject>(value_object)) {

                if (auto* value_result = as_if<CSSNumericValue>(value_object))
                    return GC::make_root(*value_result);

            }

        }

        if (value.is_number()) {

    double value_number = TRY(value.to_double(vm));

    if (isinf(value_number) || isnan(value_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
            return { value_number };
        }

    double value_number = TRY(value.to_double(vm));

    if (isinf(value_number) || isnan(value_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
        return { value_number };

    };

    Variant<double, GC::Root<CSSNumericValue>> cpp_value = TRY(value_to_variant(value));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_x(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSScalePrototype::y_getter)
{
    WebIDL::log_trace(vm, "CSSScalePrototype::y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->y(); }));

    return retval.visit(

        [&vm, &realm]([[maybe_unused]] double const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::Value(visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<CSSNumericValue> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<CSSNumericValue&>(*visited_union_value0);

        }

    );

}

JS_DEFINE_NATIVE_FUNCTION(CSSScalePrototype::y_setter)
{
    WebIDL::log_trace(vm, "CSSScalePrototype::y_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    auto value_to_variant = [&vm, &realm](JS::Value value) -> JS::ThrowCompletionOr<Variant<double, GC::Root<CSSNumericValue>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (value.is_object()) {
            [[maybe_unused]] auto& value_object = value.as_object();

            if (is<PlatformObject>(value_object)) {

                if (auto* value_result = as_if<CSSNumericValue>(value_object))
                    return GC::make_root(*value_result);

            }

        }

        if (value.is_number()) {

    double value_number = TRY(value.to_double(vm));

    if (isinf(value_number) || isnan(value_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
            return { value_number };
        }

    double value_number = TRY(value.to_double(vm));

    if (isinf(value_number) || isnan(value_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
        return { value_number };

    };

    Variant<double, GC::Root<CSSNumericValue>> cpp_value = TRY(value_to_variant(value));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_y(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSScalePrototype::z_getter)
{
    WebIDL::log_trace(vm, "CSSScalePrototype::z_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->z(); }));

    return retval.visit(

        [&vm, &realm]([[maybe_unused]] double const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::Value(visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<CSSNumericValue> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<CSSNumericValue&>(*visited_union_value0);

        }

    );

}

JS_DEFINE_NATIVE_FUNCTION(CSSScalePrototype::z_setter)
{
    WebIDL::log_trace(vm, "CSSScalePrototype::z_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    auto value_to_variant = [&vm, &realm](JS::Value value) -> JS::ThrowCompletionOr<Variant<double, GC::Root<CSSNumericValue>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (value.is_object()) {
            [[maybe_unused]] auto& value_object = value.as_object();

            if (is<PlatformObject>(value_object)) {

                if (auto* value_result = as_if<CSSNumericValue>(value_object))
                    return GC::make_root(*value_result);

            }

        }

        if (value.is_number()) {

    double value_number = TRY(value.to_double(vm));

    if (isinf(value_number) || isnan(value_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "z");
    }
    
            return { value_number };
        }

    double value_number = TRY(value.to_double(vm));

    if (isinf(value_number) || isnan(value_number)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "z");
    }
    
        return { value_number };

    };

    Variant<double, GC::Root<CSSNumericValue>> cpp_value = TRY(value_to_variant(value));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_z(cpp_value); }));

    return JS::js_undefined();
}

} // namespace Web::Bindings
