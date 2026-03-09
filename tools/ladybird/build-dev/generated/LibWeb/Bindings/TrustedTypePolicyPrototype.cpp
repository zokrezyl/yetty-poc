
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
#include <LibWeb/Bindings/TrustedTypePolicyPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedTypePolicy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedHTML.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedScript.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedScriptURL.h>

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

GC_DEFINE_ALLOCATOR(TrustedTypePolicyPrototype);

TrustedTypePolicyPrototype::TrustedTypePolicyPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

TrustedTypePolicyPrototype::~TrustedTypePolicyPrototype()
{
}

void TrustedTypePolicyPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_name_getter = JS::NativeFunction::create(realm, name_getter, 0, "name"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_name_setter;

    define_direct_accessor("name"_utf16_fly_string, native_name_getter, native_name_setter, default_attributes);

    define_native_function(realm, "createHTML"_utf16_fly_string, create_html, 1, default_attributes);

    define_native_function(realm, "createScript"_utf16_fly_string, create_script, 1, default_attributes);

    define_native_function(realm, "createScriptURL"_utf16_fly_string, create_script_url, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "TrustedTypePolicy"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void TrustedTypePolicyPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<TrustedTypePolicy*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<TrustedTypePolicy>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "TrustedTypePolicy");
    return static_cast<TrustedTypePolicy*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(TrustedTypePolicyPrototype::name_getter)
{
    WebIDL::log_trace(vm, "TrustedTypePolicyPrototype::name_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->name(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(TrustedTypePolicyPrototype::create_html)
{
    WebIDL::log_trace(vm, "TrustedTypePolicyPrototype::create_html");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "createHTML");

    auto arg0 = vm.argument(0);

    Utf16String input;
    if (!false || !arg0.is_null()) {
        input = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

    GC::RootVector<JS::Value> arguments { vm.heap() };

    if (vm.argument_count() > 1) {
        arguments.ensure_capacity(vm.argument_count() - 1);

        for (size_t i = 1; i < vm.argument_count(); ++i)
            arguments.unchecked_append(vm.argument(i));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_html(input, move(arguments)); }));

    return &const_cast<TrustedHTML&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(TrustedTypePolicyPrototype::create_script)
{
    WebIDL::log_trace(vm, "TrustedTypePolicyPrototype::create_script");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "createScript");

    auto arg0 = vm.argument(0);

    Utf16String input;
    if (!false || !arg0.is_null()) {
        input = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

    GC::RootVector<JS::Value> arguments { vm.heap() };

    if (vm.argument_count() > 1) {
        arguments.ensure_capacity(vm.argument_count() - 1);

        for (size_t i = 1; i < vm.argument_count(); ++i)
            arguments.unchecked_append(vm.argument(i));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_script(input, move(arguments)); }));

    return &const_cast<TrustedScript&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(TrustedTypePolicyPrototype::create_script_url)
{
    WebIDL::log_trace(vm, "TrustedTypePolicyPrototype::create_script_url");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "createScriptURL");

    auto arg0 = vm.argument(0);

    Utf16String input;
    if (!false || !arg0.is_null()) {
        input = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

    GC::RootVector<JS::Value> arguments { vm.heap() };

    if (vm.argument_count() > 1) {
        arguments.ensure_capacity(vm.argument_count() - 1);

        for (size_t i = 1; i < vm.argument_count(); ++i)
            arguments.unchecked_append(vm.argument(i));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_script_url(input, move(arguments)); }));

    return &const_cast<TrustedScriptURL&>(*retval);

}

} // namespace Web::Bindings
