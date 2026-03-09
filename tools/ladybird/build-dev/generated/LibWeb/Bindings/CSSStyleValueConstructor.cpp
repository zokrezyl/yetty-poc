
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
#include <LibWeb/Bindings/CSSStyleValueConstructor.h>
#include <LibWeb/Bindings/CSSStyleValuePrototype.h>
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

GC_DEFINE_ALLOCATOR(CSSStyleValueConstructor);

CSSStyleValueConstructor::CSSStyleValueConstructor(JS::Realm& realm)
    : NativeFunction("CSSStyleValue"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

CSSStyleValueConstructor::~CSSStyleValueConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> CSSStyleValueConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "CSSStyleValue");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> CSSStyleValueConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "CSSStyleValueConstructor::construct");

    return vm().throw_completion<JS::TypeError>(JS::ErrorType::NotAConstructor, "CSSStyleValue");
}


void CSSStyleValueConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    define_direct_property(vm.names.length, JS::Value(0), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "CSSStyleValue"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<CSSStyleValuePrototype>(realm, "CSSStyleValue"_fly_string), 0);


    define_native_function(realm, "parse"_utf16_fly_string, parse, 2, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "parseAll"_utf16_fly_string, parse_all, 2, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

}

JS_DEFINE_NATIVE_FUNCTION(CSSStyleValueConstructor::parse)
{
    WebIDL::log_trace(vm, "CSSStyleValueConstructor::parse");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "parse", "2");

    auto arg0 = vm.argument(0);

    String property;
    if (!false || !arg0.is_null()) {
        property = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    String css_text;
    if (!false || !arg1.is_null()) {
        css_text = TRY(WebIDL::to_usv_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::CSSStyleValue::parse(vm, property, css_text); }));

    return &const_cast<CSSStyleValue&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSStyleValueConstructor::parse_all)
{
    WebIDL::log_trace(vm, "CSSStyleValueConstructor::parse_all");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "parseAll", "2");

    auto arg0 = vm.argument(0);

    String property;
    if (!false || !arg0.is_null()) {
        property = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    String css_text;
    if (!false || !arg1.is_null()) {
        css_text = TRY(WebIDL::to_usv_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::CSSStyleValue::parse_all(vm, property, css_text); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

} // namespace Web::Bindings
