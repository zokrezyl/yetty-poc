
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
#include <LibWeb/Bindings/TrustedTypePolicyFactoryPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedTypePolicyFactory.h>

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

GC_DEFINE_ALLOCATOR(TrustedTypePolicyFactoryPrototype);

TrustedTypePolicyFactoryPrototype::TrustedTypePolicyFactoryPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

TrustedTypePolicyFactoryPrototype::~TrustedTypePolicyFactoryPrototype()
{
}

void TrustedTypePolicyFactoryPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_empty_html_getter = JS::NativeFunction::create(realm, empty_html_getter, 0, "emptyHTML"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_empty_html_setter;

    define_direct_accessor("emptyHTML"_utf16_fly_string, native_empty_html_getter, native_empty_html_setter, default_attributes);

    auto native_empty_script_getter = JS::NativeFunction::create(realm, empty_script_getter, 0, "emptyScript"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_empty_script_setter;

    define_direct_accessor("emptyScript"_utf16_fly_string, native_empty_script_getter, native_empty_script_setter, default_attributes);

    auto native_default_policy_getter = JS::NativeFunction::create(realm, default_policy_getter, 0, "defaultPolicy"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_default_policy_setter;

    define_direct_accessor("defaultPolicy"_utf16_fly_string, native_default_policy_getter, native_default_policy_setter, default_attributes);

    define_native_function(realm, "createPolicy"_utf16_fly_string, create_policy, 1, default_attributes);

    define_native_function(realm, "isHTML"_utf16_fly_string, is_html, 1, default_attributes);

    define_native_function(realm, "isScript"_utf16_fly_string, is_script, 1, default_attributes);

    define_native_function(realm, "isScriptURL"_utf16_fly_string, is_script_url, 1, default_attributes);

    define_native_function(realm, "getAttributeType"_utf16_fly_string, get_attribute_type, 2, default_attributes);

    define_native_function(realm, "getPropertyType"_utf16_fly_string, get_property_type, 2, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "TrustedTypePolicyFactory"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void TrustedTypePolicyFactoryPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<TrustedTypePolicyFactory*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<TrustedTypePolicyFactory>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "TrustedTypePolicyFactory");
    return static_cast<TrustedTypePolicyFactory*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(TrustedTypePolicyFactoryPrototype::empty_html_getter)
{
    WebIDL::log_trace(vm, "TrustedTypePolicyFactoryPrototype::empty_html_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->empty_html(); }));

    return &const_cast<TrustedHTML&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(TrustedTypePolicyFactoryPrototype::empty_script_getter)
{
    WebIDL::log_trace(vm, "TrustedTypePolicyFactoryPrototype::empty_script_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->empty_script(); }));

    return &const_cast<TrustedScript&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(TrustedTypePolicyFactoryPrototype::default_policy_getter)
{
    WebIDL::log_trace(vm, "TrustedTypePolicyFactoryPrototype::default_policy_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->default_policy(); }));

    if (retval) {

    return &const_cast<TrustedTypePolicy&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(TrustedTypePolicyFactoryPrototype::create_policy)
{
    WebIDL::log_trace(vm, "TrustedTypePolicyFactoryPrototype::create_policy");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "createPolicy");

    auto arg0 = vm.argument(0);

    Utf16String policy_name;
    if (!false || !arg0.is_null()) {
        policy_name = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "TrustedTypePolicyOptions");

    TrustedTypePolicyOptions policy_options {};

    auto create_html_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        create_html_property_value_0 = TRY(arg1.as_object().get("createHTML"_utf16_fly_string));

    if (!create_html_property_value_0.is_undefined()) {

    if (!create_html_property_value_0.is_function()
&& !create_html_property_value_0.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, create_html_property_value_0);

    GC::Ptr<WebIDL::CallbackType> create_html_value_0;
    if (create_html_property_value_0.is_object())
        create_html_value_0 = vm.heap().allocate<WebIDL::CallbackType>(create_html_property_value_0.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    policy_options.create_html = create_html_value_0;

    }

    auto create_script_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        create_script_property_value_1 = TRY(arg1.as_object().get("createScript"_utf16_fly_string));

    if (!create_script_property_value_1.is_undefined()) {

    if (!create_script_property_value_1.is_function()
&& !create_script_property_value_1.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, create_script_property_value_1);

    GC::Ptr<WebIDL::CallbackType> create_script_value_1;
    if (create_script_property_value_1.is_object())
        create_script_value_1 = vm.heap().allocate<WebIDL::CallbackType>(create_script_property_value_1.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    policy_options.create_script = create_script_value_1;

    }

    auto create_script_url_property_value_2 = JS::js_undefined();
    if (arg1.is_object())
        create_script_url_property_value_2 = TRY(arg1.as_object().get("createScriptURL"_utf16_fly_string));

    if (!create_script_url_property_value_2.is_undefined()) {

    if (!create_script_url_property_value_2.is_function()
&& !create_script_url_property_value_2.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, create_script_url_property_value_2);

    GC::Ptr<WebIDL::CallbackType> create_script_url_value_2;
    if (create_script_url_property_value_2.is_object())
        create_script_url_value_2 = vm.heap().allocate<WebIDL::CallbackType>(create_script_url_property_value_2.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    policy_options.create_script_url = create_script_url_value_2;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_policy(policy_name, policy_options); }));

    return &const_cast<TrustedTypePolicy&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(TrustedTypePolicyFactoryPrototype::is_html)
{
    WebIDL::log_trace(vm, "TrustedTypePolicyFactoryPrototype::is_html");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "isHTML");

    auto arg0 = vm.argument(0);

    auto value = arg0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_html(value); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(TrustedTypePolicyFactoryPrototype::is_script)
{
    WebIDL::log_trace(vm, "TrustedTypePolicyFactoryPrototype::is_script");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "isScript");

    auto arg0 = vm.argument(0);

    auto value = arg0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_script(value); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(TrustedTypePolicyFactoryPrototype::is_script_url)
{
    WebIDL::log_trace(vm, "TrustedTypePolicyFactoryPrototype::is_script_url");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "isScriptURL");

    auto arg0 = vm.argument(0);

    auto value = arg0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_script_url(value); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(TrustedTypePolicyFactoryPrototype::get_attribute_type)
{
    WebIDL::log_trace(vm, "TrustedTypePolicyFactoryPrototype::get_attribute_type");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getAttributeType", "2");

    auto arg0 = vm.argument(0);

    Utf16String tag_name;
    if (!false || !arg0.is_null()) {
        tag_name = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    Utf16String attribute;
    if (!false || !arg1.is_null()) {
        attribute = TRY(WebIDL::to_utf16_string(vm, arg1));
    }

    auto arg2 = vm.argument(2);

    Optional<Utf16String> element_ns;

    if (!arg2.is_undefined()) {
        if (!arg2.is_null())
            element_ns = TRY(WebIDL::to_utf16_string(vm, arg2));
    }

    auto arg3 = vm.argument(3);

    Optional<Utf16String> attr_ns;

    if (!arg3.is_undefined()) {
        if (!arg3.is_null())
            attr_ns = TRY(WebIDL::to_utf16_string(vm, arg3));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_attribute_type(tag_name, attribute, element_ns, attr_ns); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(TrustedTypePolicyFactoryPrototype::get_property_type)
{
    WebIDL::log_trace(vm, "TrustedTypePolicyFactoryPrototype::get_property_type");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getPropertyType", "2");

    auto arg0 = vm.argument(0);

    Utf16String tag_name;
    if (!false || !arg0.is_null()) {
        tag_name = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    Utf16String property;
    if (!false || !arg1.is_null()) {
        property = TRY(WebIDL::to_utf16_string(vm, arg1));
    }

    auto arg2 = vm.argument(2);

    Optional<Utf16String> element_ns;

    if (!arg2.is_undefined()) {
        if (!arg2.is_null())
            element_ns = TRY(WebIDL::to_utf16_string(vm, arg2));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_property_type(tag_name, property, element_ns); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

} // namespace Web::Bindings
