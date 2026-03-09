
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
#include <LibWeb/Bindings/StylePropertyMapPrototype.h>
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

#if __has_include(<LibWeb/Bindings/StylePropertyMapReadOnlyPrototype.h>)
#    include <LibWeb/Bindings/StylePropertyMapReadOnlyPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StylePropertyMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StylePropertyMapReadOnly.h>

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

GC_DEFINE_ALLOCATOR(StylePropertyMapPrototype);

StylePropertyMapPrototype::StylePropertyMapPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

StylePropertyMapPrototype::~StylePropertyMapPrototype()
{
}

void StylePropertyMapPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<StylePropertyMapReadOnlyPrototype>(realm, "StylePropertyMapReadOnly"_fly_string));


    define_native_function(realm, "set"_utf16_fly_string, set, 1, default_attributes);

    define_native_function(realm, "append"_utf16_fly_string, append, 1, default_attributes);

    define_native_function(realm, "delete"_utf16_fly_string, delete_, 1, default_attributes);

    define_native_function(realm, "clear"_utf16_fly_string, clear, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "StylePropertyMap"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void StylePropertyMapPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<CSS::StylePropertyMap*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<CSS::StylePropertyMap>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "StylePropertyMap");
    return static_cast<CSS::StylePropertyMap*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(StylePropertyMapPrototype::set)
{
    WebIDL::log_trace(vm, "StylePropertyMapPrototype::set");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "set");

    auto arg0 = vm.argument(0);

    String property;
    if (!false || !arg0.is_null()) {
        property = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<CSSStyleValue>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (is<PlatformObject>(arg1_object)) {

                if (auto* arg1_result = as_if<CSSStyleValue>(arg1_object))
                    return GC::make_root(*arg1_result);

            }

        }

    String arg1_string;
    if (!false || !arg1.is_null()) {
        arg1_string = TRY(WebIDL::to_usv_string(vm, arg1));
    }

        return { arg1_string };

    };

        Vector<Variant<GC::Root<CSSStyleValue>, String>> values;

        if (vm.argument_count() > 1) {
            values.ensure_capacity(vm.argument_count() - 1);

            for (size_t i = 1; i < vm.argument_count(); ++i) {
                auto result = TRY(arg1_to_variant(vm.argument(i)));
                values.unchecked_append(move(result));
            }
        }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set(property, move(values)); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(StylePropertyMapPrototype::append)
{
    WebIDL::log_trace(vm, "StylePropertyMapPrototype::append");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "append");

    auto arg0 = vm.argument(0);

    String property;
    if (!false || !arg0.is_null()) {
        property = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<CSSStyleValue>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (is<PlatformObject>(arg1_object)) {

                if (auto* arg1_result = as_if<CSSStyleValue>(arg1_object))
                    return GC::make_root(*arg1_result);

            }

        }

    String arg1_string;
    if (!false || !arg1.is_null()) {
        arg1_string = TRY(WebIDL::to_usv_string(vm, arg1));
    }

        return { arg1_string };

    };

        Vector<Variant<GC::Root<CSSStyleValue>, String>> values;

        if (vm.argument_count() > 1) {
            values.ensure_capacity(vm.argument_count() - 1);

            for (size_t i = 1; i < vm.argument_count(); ++i) {
                auto result = TRY(arg1_to_variant(vm.argument(i)));
                values.unchecked_append(move(result));
            }
        }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->append(property, move(values)); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(StylePropertyMapPrototype::delete_)
{
    WebIDL::log_trace(vm, "StylePropertyMapPrototype::delete_");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "delete");

    auto arg0 = vm.argument(0);

    String property;
    if (!false || !arg0.is_null()) {
        property = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_(property); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(StylePropertyMapPrototype::clear)
{
    WebIDL::log_trace(vm, "StylePropertyMapPrototype::clear");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear(); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
