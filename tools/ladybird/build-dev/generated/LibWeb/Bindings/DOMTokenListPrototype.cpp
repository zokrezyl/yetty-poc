
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
#include <LibWeb/Bindings/DOMTokenListPrototype.h>
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


#include <LibWeb/Bindings/MainThreadVM.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DOMTokenList.h>

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

GC_DEFINE_ALLOCATOR(DOMTokenListPrototype);

DOMTokenListPrototype::DOMTokenListPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

DOMTokenListPrototype::~DOMTokenListPrototype()
{
}

void DOMTokenListPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_length_getter = JS::NativeFunction::create(realm, length_getter, 0, "length"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_length_setter;

    define_direct_accessor("length"_utf16_fly_string, native_length_getter, native_length_setter, default_attributes);

    auto native_value_getter = JS::NativeFunction::create(realm, value_getter, 0, "value"_utf16_fly_string, &realm, "get"sv);

    auto native_value_setter = JS::NativeFunction::create(realm, value_setter, 1, "value"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("value"_utf16_fly_string, native_value_getter, native_value_setter, default_attributes);

    define_native_function(realm, "item"_utf16_fly_string, item, 1, default_attributes);

    define_native_function(realm, "contains"_utf16_fly_string, contains, 1, default_attributes);

    define_native_function(realm, "add"_utf16_fly_string, add, 0, default_attributes);

    define_native_function(realm, "remove"_utf16_fly_string, remove, 0, default_attributes);

    define_native_function(realm, "toggle"_utf16_fly_string, toggle, 1, default_attributes);

    define_native_function(realm, "replace"_utf16_fly_string, replace, 2, default_attributes);

    define_native_function(realm, "supports"_utf16_fly_string, supports, 1, default_attributes);

    define_native_function(realm, "toString"_utf16_fly_string, to_string, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_iterator(), realm.intrinsics().array_prototype()->get_without_side_effects(vm.names.values), JS::Attribute::Configurable | JS::Attribute::Writable);

    define_direct_property(vm.names.entries, realm.intrinsics().array_prototype()->get_without_side_effects(vm.names.entries), default_attributes);
    define_direct_property(vm.names.keys, realm.intrinsics().array_prototype()->get_without_side_effects(vm.names.keys), default_attributes);
    define_direct_property(vm.names.values, realm.intrinsics().array_prototype()->get_without_side_effects(vm.names.values), default_attributes);
    define_direct_property(vm.names.forEach, realm.intrinsics().array_prototype()->get_without_side_effects(vm.names.forEach), default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "DOMTokenList"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void DOMTokenListPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<DOM::DOMTokenList*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<DOM::DOMTokenList>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMTokenList");
    return static_cast<DOM::DOMTokenList*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(DOMTokenListPrototype::length_getter)
{
    WebIDL::log_trace(vm, "DOMTokenListPrototype::length_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->length(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(DOMTokenListPrototype::value_getter)
{
    WebIDL::log_trace(vm, "DOMTokenListPrototype::value_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->value(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    auto retval = retval_or_exception.release_value();

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMTokenListPrototype::value_setter)
{
    WebIDL::log_trace(vm, "DOMTokenListPrototype::value_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_value(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DOMTokenListPrototype::item)
{
    WebIDL::log_trace(vm, "DOMTokenListPrototype::item");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "item");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->item(index); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DOMTokenListPrototype::contains)
{
    WebIDL::log_trace(vm, "DOMTokenListPrototype::contains");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "contains");

    auto arg0 = vm.argument(0);

    String token;
    if (!false || !arg0.is_null()) {
        token = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->contains(token); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMTokenListPrototype::add)
{
    WebIDL::log_trace(vm, "DOMTokenListPrototype::add");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    Vector<String> tokens;

    if (vm.argument_count() > 0) {
        tokens.ensure_capacity(vm.argument_count() - 0);

        for (size_t i = 0; i < vm.argument_count(); ++i) {
            auto to_string_result = TRY(WebIDL::to_string(vm, vm.argument(i)));
            tokens.unchecked_append(move(to_string_result));
        }
    }

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->add(move(tokens)); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(DOMTokenListPrototype::remove)
{
    WebIDL::log_trace(vm, "DOMTokenListPrototype::remove");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    Vector<String> tokens;

    if (vm.argument_count() > 0) {
        tokens.ensure_capacity(vm.argument_count() - 0);

        for (size_t i = 0; i < vm.argument_count(); ++i) {
            auto to_string_result = TRY(WebIDL::to_string(vm, vm.argument(i)));
            tokens.unchecked_append(move(to_string_result));
        }
    }

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->remove(move(tokens)); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(DOMTokenListPrototype::toggle)
{
    WebIDL::log_trace(vm, "DOMTokenListPrototype::toggle");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "toggle");

    auto arg0 = vm.argument(0);

    String token;
    if (!false || !arg0.is_null()) {
        token = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    Optional<bool> force;

    if (!arg1.is_undefined())

    force = arg1.to_boolean();

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->toggle(token, force); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMTokenListPrototype::replace)
{
    WebIDL::log_trace(vm, "DOMTokenListPrototype::replace");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "replace", "2");

    auto arg0 = vm.argument(0);

    String token;
    if (!false || !arg0.is_null()) {
        token = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    String new_token;
    if (!false || !arg1.is_null()) {
        new_token = TRY(WebIDL::to_string(vm, arg1));
    }

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->replace(token, new_token); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMTokenListPrototype::supports)
{
    WebIDL::log_trace(vm, "DOMTokenListPrototype::supports");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "supports");

    auto arg0 = vm.argument(0);

    String token;
    if (!false || !arg0.is_null()) {
        token = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->supports(token); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMTokenListPrototype::to_string)
{
    WebIDL::log_trace(vm, "DOMTokenListPrototype::to_string");
    [[maybe_unused]] auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));


    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->value(); }));


    return JS::PrimitiveString::create(vm, move(retval));
}

} // namespace Web::Bindings
