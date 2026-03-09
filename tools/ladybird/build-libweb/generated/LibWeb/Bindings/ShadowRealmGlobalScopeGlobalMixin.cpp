
#include <AK/Function.h>
#include <LibIDL/Types.h>
#include <LibJS/Runtime/Array.h>
#include <LibJS/Runtime/DataView.h>
#include <LibJS/Runtime/Error.h>
#include <LibJS/Runtime/FunctionObject.h>
#include <LibJS/Runtime/GlobalObject.h>
#include <LibJS/Runtime/Iterator.h>
#include <LibJS/Runtime/TypedArray.h>
#include <LibJS/Runtime/Value.h>
#include <LibJS/Runtime/ValueInlines.h>
#include <LibURL/Origin.h>
#include <LibWeb/Bindings/ShadowRealmGlobalScopeGlobalMixin.h>
#include <LibWeb/Bindings/ShadowRealmGlobalScopePrototype.h>
#include <LibWeb/Bindings/ExceptionOrUtils.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/Bindings/PrincipalHostDefined.h>
#include <LibWeb/DOM/Element.h>
#include <LibWeb/DOM/Event.h>
#include <LibWeb/DOM/IDLEventListener.h>
#include <LibWeb/DOM/NodeFilter.h>
#include <LibWeb/DOM/Range.h>
#include <LibWeb/HTML/Scripting/Environments.h>
#include <LibWeb/HTML/Window.h>
#include <LibWeb/HTML/WindowProxy.h>
#include <LibWeb/WebIDL/AbstractOperations.h>
#include <LibWeb/WebIDL/OverloadResolution.h>
#include <LibWeb/WebIDL/Tracing.h>
#include <LibWeb/WebIDL/Types.h>


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ShadowRealmGlobalScope.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/MessagePort.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

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

ShadowRealmGlobalScopeGlobalMixin::ShadowRealmGlobalScopeGlobalMixin() = default;
ShadowRealmGlobalScopeGlobalMixin::~ShadowRealmGlobalScopeGlobalMixin() = default;

void ShadowRealmGlobalScopeGlobalMixin::initialize(JS::Realm& realm, JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;

    object.set_prototype(&ensure_web_prototype<ShadowRealmGlobalScopePrototype>(realm, "ShadowRealmGlobalScope"_fly_string));

    auto native_self_getter = JS::NativeFunction::create(realm, self_getter, 0, "self"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_self_setter;

    object.define_direct_accessor("self"_utf16_fly_string, native_self_getter, native_self_setter, default_attributes);

    object.define_native_function(realm, "btoa"_utf16_fly_string, btoa, 1, default_attributes);

    object.define_native_function(realm, "atob"_utf16_fly_string, atob, 1, default_attributes);

    object.define_native_function(realm, "queueMicrotask"_utf16_fly_string, queue_microtask, 1, default_attributes);

    object.define_native_function(realm, "structuredClone"_utf16_fly_string, structured_clone, 1, default_attributes);

    object.define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "ShadowRealmGlobalScope"_string), JS::Attribute::Configurable);

}

void ShadowRealmGlobalScopeGlobalMixin::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::ShadowRealmGlobalScope*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::ShadowRealmGlobalScope>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ShadowRealmGlobalScope");
    return static_cast<HTML::ShadowRealmGlobalScope*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(ShadowRealmGlobalScopeGlobalMixin::self_getter)
{
    WebIDL::log_trace(vm, "ShadowRealmGlobalScopeGlobalMixin::self_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->self(); }));

    return &const_cast<ShadowRealmGlobalScope&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ShadowRealmGlobalScopeGlobalMixin::btoa)
{
    WebIDL::log_trace(vm, "ShadowRealmGlobalScopeGlobalMixin::btoa");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "btoa");

    auto arg0 = vm.argument(0);

    String data;
    if (!false || !arg0.is_null()) {
        data = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->btoa(data); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(ShadowRealmGlobalScopeGlobalMixin::atob)
{
    WebIDL::log_trace(vm, "ShadowRealmGlobalScopeGlobalMixin::atob");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "atob");

    auto arg0 = vm.argument(0);

    String data;
    if (!false || !arg0.is_null()) {
        data = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->atob(data); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(ShadowRealmGlobalScopeGlobalMixin::queue_microtask)
{
    WebIDL::log_trace(vm, "ShadowRealmGlobalScopeGlobalMixin::queue_microtask");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "queueMicrotask");

    auto arg0 = vm.argument(0);

    if (!arg0.is_function()
)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, arg0);

    auto callback = vm.heap().allocate<WebIDL::CallbackType>(arg0.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->queue_microtask(callback); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ShadowRealmGlobalScopeGlobalMixin::structured_clone)
{
    WebIDL::log_trace(vm, "ShadowRealmGlobalScopeGlobalMixin::structured_clone");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "structuredClone");

    auto arg0 = vm.argument(0);

    auto value = arg0;

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "StructuredSerializeOptions");

    StructuredSerializeOptions options {};

    auto transfer_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        transfer_property_value_0 = TRY(arg1.as_object().get("transfer"_utf16_fly_string));

    Vector<GC::Root<JS::Object>> transfer_value_0;

    if (!transfer_property_value_0.is_undefined()) {

    if (!transfer_property_value_0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, transfer_property_value_0);

    auto transfer_property_value_0_iterator_method0 = TRY(transfer_property_value_0.get_method(vm, vm.well_known_symbol_iterator()));
    if (!transfer_property_value_0_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, transfer_property_value_0);

    auto transfer_property_value_0_iterator1 = TRY(JS::get_iterator_from_method(vm, transfer_property_value_0, *transfer_property_value_0_iterator_method0));

    Vector<GC::Root<JS::Object>> transfer_value_0_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, transfer_property_value_0_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, next_item1);
    auto sequence_item1 = GC::make_root(next_item1.as_object());

    transfer_value_0_non_optional.append(sequence_item1);
    }

        transfer_value_0 = move(transfer_value_0_non_optional);
    }

    options.transfer = transfer_value_0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->structured_clone(value, options); }));

    return retval;

}

} // namespace Web::Bindings
    