
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
#include <LibWeb/Bindings/PerformanceNavigationPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/NavigationTiming/PerformanceNavigation.h>

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

GC_DEFINE_ALLOCATOR(PerformanceNavigationPrototype);

PerformanceNavigationPrototype::PerformanceNavigationPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

PerformanceNavigationPrototype::~PerformanceNavigationPrototype()
{
}

void PerformanceNavigationPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_type_getter = JS::NativeFunction::create(realm, type_getter, 0, "type"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_type_setter;

    define_direct_accessor("type"_utf16_fly_string, native_type_getter, native_type_setter, default_attributes);

    auto native_redirect_count_getter = JS::NativeFunction::create(realm, redirect_count_getter, 0, "redirectCount"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_redirect_count_setter;

    define_direct_accessor("redirectCount"_utf16_fly_string, native_redirect_count_getter, native_redirect_count_setter, default_attributes);

    auto constant_TYPE_NAVIGATE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0));

    define_direct_property("TYPE_NAVIGATE"_utf16_fly_string, constant_TYPE_NAVIGATE_value, JS::Attribute::Enumerable);

    auto constant_TYPE_RELOAD_value = JS::Value(static_cast<WebIDL::UnsignedShort>(1));

    define_direct_property("TYPE_RELOAD"_utf16_fly_string, constant_TYPE_RELOAD_value, JS::Attribute::Enumerable);

    auto constant_TYPE_BACK_FORWARD_value = JS::Value(static_cast<WebIDL::UnsignedShort>(2));

    define_direct_property("TYPE_BACK_FORWARD"_utf16_fly_string, constant_TYPE_BACK_FORWARD_value, JS::Attribute::Enumerable);

    auto constant_TYPE_RESERVED_value = JS::Value(static_cast<WebIDL::UnsignedShort>(255));

    define_direct_property("TYPE_RESERVED"_utf16_fly_string, constant_TYPE_RESERVED_value, JS::Attribute::Enumerable);

    define_native_function(realm, "toJSON"_utf16_fly_string, to_json, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "PerformanceNavigation"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void PerformanceNavigationPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<NavigationTiming::PerformanceNavigation*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<NavigationTiming::PerformanceNavigation>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PerformanceNavigation");
    return static_cast<NavigationTiming::PerformanceNavigation*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(PerformanceNavigationPrototype::type_getter)
{
    WebIDL::log_trace(vm, "PerformanceNavigationPrototype::type_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->type(); }));

    return JS::Value(static_cast<WebIDL::UnsignedShort>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceNavigationPrototype::redirect_count_getter)
{
    WebIDL::log_trace(vm, "PerformanceNavigationPrototype::redirect_count_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->redirect_count(); }));

    return JS::Value(static_cast<WebIDL::UnsignedShort>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceNavigationPrototype::to_json)
{
    WebIDL::log_trace(vm, "PerformanceNavigationPrototype::to_json");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

    auto type_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->type(); }));

    JS::Value type_retval_wrapped;

    type_retval_wrapped = JS::Value(static_cast<WebIDL::UnsignedShort>(type_retval));

    MUST(result->create_data_property("type"_utf16_fly_string, type_retval_wrapped));

    auto redirect_count_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->redirect_count(); }));

    JS::Value redirect_count_retval_wrapped;

    redirect_count_retval_wrapped = JS::Value(static_cast<WebIDL::UnsignedShort>(redirect_count_retval));

    MUST(result->create_data_property("redirectCount"_utf16_fly_string, redirect_count_retval_wrapped));

    auto constant_TYPE_NAVIGATE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0));

    MUST(result->create_data_property("TYPE_NAVIGATE"_utf16_fly_string, constant_TYPE_NAVIGATE_value));

    auto constant_TYPE_RELOAD_value = JS::Value(static_cast<WebIDL::UnsignedShort>(1));

    MUST(result->create_data_property("TYPE_RELOAD"_utf16_fly_string, constant_TYPE_RELOAD_value));

    auto constant_TYPE_BACK_FORWARD_value = JS::Value(static_cast<WebIDL::UnsignedShort>(2));

    MUST(result->create_data_property("TYPE_BACK_FORWARD"_utf16_fly_string, constant_TYPE_BACK_FORWARD_value));

    auto constant_TYPE_RESERVED_value = JS::Value(static_cast<WebIDL::UnsignedShort>(255));

    MUST(result->create_data_property("TYPE_RESERVED"_utf16_fly_string, constant_TYPE_RESERVED_value));

    return result;
}

} // namespace Web::Bindings
