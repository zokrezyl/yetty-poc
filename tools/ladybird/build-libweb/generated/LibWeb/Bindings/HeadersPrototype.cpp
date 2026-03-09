
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
#include <LibWeb/Bindings/HeadersPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Headers.h>

#   include <LibWeb/Fetch/HeadersIterator.h>

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

GC_DEFINE_ALLOCATOR(HeadersPrototype);

HeadersPrototype::HeadersPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

HeadersPrototype::~HeadersPrototype()
{
}

void HeadersPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    define_native_function(realm, "append"_utf16_fly_string, append, 2, default_attributes);

    define_native_function(realm, "delete"_utf16_fly_string, delete_, 1, default_attributes);

    define_native_function(realm, "get"_utf16_fly_string, get, 1, default_attributes);

    define_native_function(realm, "getSetCookie"_utf16_fly_string, get_set_cookie, 0, default_attributes);

    define_native_function(realm, "has"_utf16_fly_string, has, 1, default_attributes);

    define_native_function(realm, "set"_utf16_fly_string, set, 2, default_attributes);

    define_native_function(realm, vm.names.entries, entries, 0, default_attributes);
    define_native_function(realm, vm.names.forEach, for_each, 1, default_attributes);
    define_native_function(realm, vm.names.keys, keys, 0, default_attributes);
    define_native_function(realm, vm.names.values, values, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_iterator(), get_without_side_effects(vm.names.entries), JS::Attribute::Configurable | JS::Attribute::Writable);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Headers"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void HeadersPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Fetch::Headers*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Fetch::Headers>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Headers");
    return static_cast<Fetch::Headers*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(HeadersPrototype::append)
{
    WebIDL::log_trace(vm, "HeadersPrototype::append");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "append", "2");

    auto arg0 = vm.argument(0);

    String name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_byte_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    String value;
    if (!false || !arg1.is_null()) {
        value = TRY(WebIDL::to_byte_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->append(name, value); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(HeadersPrototype::delete_)
{
    WebIDL::log_trace(vm, "HeadersPrototype::delete_");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "delete");

    auto arg0 = vm.argument(0);

    String name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_byte_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_(name); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(HeadersPrototype::get)
{
    WebIDL::log_trace(vm, "HeadersPrototype::get");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "get");

    auto arg0 = vm.argument(0);

    String name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_byte_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get(name); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(HeadersPrototype::get_set_cookie)
{
    WebIDL::log_trace(vm, "HeadersPrototype::get_set_cookie");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_set_cookie(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);
JS::Value wrapped_element0;

    wrapped_element0 = JS::PrimitiveString::create(vm, element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(HeadersPrototype::has)
{
    WebIDL::log_trace(vm, "HeadersPrototype::has");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "has");

    auto arg0 = vm.argument(0);

    String name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_byte_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->has(name); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(HeadersPrototype::set)
{
    WebIDL::log_trace(vm, "HeadersPrototype::set");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "set", "2");

    auto arg0 = vm.argument(0);

    String name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_byte_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    String value;
    if (!false || !arg1.is_null()) {
        value = TRY(WebIDL::to_byte_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set(name, value); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(HeadersPrototype::entries)
{
    WebIDL::log_trace(vm, "HeadersPrototype::entries");
    auto* impl = TRY(impl_from(vm));

    return TRY(throw_dom_exception_if_needed(vm, [&] { return HeadersIterator::create(*impl, Object::PropertyKind::KeyAndValue); }));
}

JS_DEFINE_NATIVE_FUNCTION(HeadersPrototype::for_each)
{
    WebIDL::log_trace(vm, "HeadersPrototype::for_each");
    [[maybe_unused]] auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    auto callback = vm.argument(0);
    if (!callback.is_function())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, callback);

    auto this_value = vm.this_value();
    TRY(impl->for_each([&](auto key, auto value) -> JS::ThrowCompletionOr<void> {

    JS::Value wrapped_key;

    wrapped_key =  JS::PrimitiveString::create(vm, key);

    JS::Value wrapped_value;

    wrapped_value =  JS::PrimitiveString::create(vm, value);

        TRY(JS::call(vm, callback.as_function(), vm.argument(1), wrapped_value, wrapped_key, this_value));
        return {};
    }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HeadersPrototype::keys)
{
    WebIDL::log_trace(vm, "HeadersPrototype::keys");
    auto* impl = TRY(impl_from(vm));

    return TRY(throw_dom_exception_if_needed(vm, [&] { return HeadersIterator::create(*impl, Object::PropertyKind::Key);  }));
}

JS_DEFINE_NATIVE_FUNCTION(HeadersPrototype::values)
{
    WebIDL::log_trace(vm, "HeadersPrototype::values");
    auto* impl = TRY(impl_from(vm));

    return TRY(throw_dom_exception_if_needed(vm, [&] { return HeadersIterator::create(*impl, Object::PropertyKind::Value); }));
}

} // namespace Web::Bindings
