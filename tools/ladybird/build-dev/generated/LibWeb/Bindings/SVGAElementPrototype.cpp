
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
#include <LibWeb/Bindings/SVGAElementPrototype.h>
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

#if __has_include(<LibWeb/Bindings/SVGGraphicsElementPrototype.h>)
#    include <LibWeb/Bindings/SVGGraphicsElementPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGAElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DOMTokenList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Request.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Headers.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGAnimatedString.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XHR/FormData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOMURL/URLSearchParams.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/QueuingStrategy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamBYOBReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamDefaultReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/File.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStreamDefaultWriter.h>

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

GC_DEFINE_ALLOCATOR(SVGAElementPrototype);

SVGAElementPrototype::SVGAElementPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

SVGAElementPrototype::~SVGAElementPrototype()
{
}

void SVGAElementPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<SVGGraphicsElementPrototype>(realm, "SVGGraphicsElement"_fly_string));


    auto native_target_getter = JS::NativeFunction::create(realm, target_getter, 0, "target"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_target_setter;

    define_direct_accessor("target"_utf16_fly_string, native_target_getter, native_target_setter, default_attributes);

    auto native_download_getter = JS::NativeFunction::create(realm, download_getter, 0, "download"_utf16_fly_string, &realm, "get"sv);

    auto native_download_setter = JS::NativeFunction::create(realm, download_setter, 1, "download"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("download"_utf16_fly_string, native_download_getter, native_download_setter, default_attributes);

    auto native_ping_getter = JS::NativeFunction::create(realm, ping_getter, 0, "ping"_utf16_fly_string, &realm, "get"sv);

    auto native_ping_setter = JS::NativeFunction::create(realm, ping_setter, 1, "ping"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ping"_utf16_fly_string, native_ping_getter, native_ping_setter, default_attributes);

    auto native_rel_getter = JS::NativeFunction::create(realm, rel_getter, 0, "rel"_utf16_fly_string, &realm, "get"sv);

    auto native_rel_setter = JS::NativeFunction::create(realm, rel_setter, 1, "rel"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("rel"_utf16_fly_string, native_rel_getter, native_rel_setter, default_attributes);

    auto native_rel_list_getter = JS::NativeFunction::create(realm, rel_list_getter, 0, "relList"_utf16_fly_string, &realm, "get"sv);

    auto native_rel_list_setter = JS::NativeFunction::create(realm, rel_list_setter, 1, "relList"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("relList"_utf16_fly_string, native_rel_list_getter, native_rel_list_setter, default_attributes);

    auto native_hreflang_getter = JS::NativeFunction::create(realm, hreflang_getter, 0, "hreflang"_utf16_fly_string, &realm, "get"sv);

    auto native_hreflang_setter = JS::NativeFunction::create(realm, hreflang_setter, 1, "hreflang"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("hreflang"_utf16_fly_string, native_hreflang_getter, native_hreflang_setter, default_attributes);

    auto native_type_getter = JS::NativeFunction::create(realm, type_getter, 0, "type"_utf16_fly_string, &realm, "get"sv);

    auto native_type_setter = JS::NativeFunction::create(realm, type_setter, 1, "type"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("type"_utf16_fly_string, native_type_getter, native_type_setter, default_attributes);

    auto native_referrer_policy_getter = JS::NativeFunction::create(realm, referrer_policy_getter, 0, "referrerPolicy"_utf16_fly_string, &realm, "get"sv);

    auto native_referrer_policy_setter = JS::NativeFunction::create(realm, referrer_policy_setter, 1, "referrerPolicy"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("referrerPolicy"_utf16_fly_string, native_referrer_policy_getter, native_referrer_policy_setter, default_attributes);

    auto native_href_getter = JS::NativeFunction::create(realm, href_getter, 0, "href"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_href_setter;

    define_direct_accessor("href"_utf16_fly_string, native_href_getter, native_href_setter, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "SVGAElement"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void SVGAElementPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<SVG::SVGAElement*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<SVG::SVGAElement>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SVGAElement");
    return static_cast<SVG::SVGAElement*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(SVGAElementPrototype::target_getter)
{
    WebIDL::log_trace(vm, "SVGAElementPrototype::target_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->target(); }));

    return &const_cast<SVGAnimatedString&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGAElementPrototype::download_getter)
{
    WebIDL::log_trace(vm, "SVGAElementPrototype::download_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto contentAttributeValue = impl->attribute("download"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGAElementPrototype::download_setter)
{
    WebIDL::log_trace(vm, "SVGAElementPrototype::download_setter");
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

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    impl->set_attribute_value("download"_fly_string, cpp_value);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(SVGAElementPrototype::ping_getter)
{
    WebIDL::log_trace(vm, "SVGAElementPrototype::ping_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto content_attribute_value = impl->attribute("ping"_fly_string);

    String retval;
    if (content_attribute_value.has_value())
        retval = MUST(Infra::convert_to_scalar_value_string(*content_attribute_value));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGAElementPrototype::ping_setter)
{
    WebIDL::log_trace(vm, "SVGAElementPrototype::ping_setter");
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

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_usv_string(vm, value));
    }

    impl->set_attribute_value("ping"_fly_string, cpp_value);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(SVGAElementPrototype::rel_getter)
{
    WebIDL::log_trace(vm, "SVGAElementPrototype::rel_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto contentAttributeValue = impl->attribute("rel"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGAElementPrototype::rel_setter)
{
    WebIDL::log_trace(vm, "SVGAElementPrototype::rel_setter");
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

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    impl->set_attribute_value("rel"_fly_string, cpp_value);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(SVGAElementPrototype::rel_list_getter)
{
    WebIDL::log_trace(vm, "SVGAElementPrototype::rel_list_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->rel_list(); }));

    return &const_cast<DOMTokenList&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGAElementPrototype::rel_list_setter)
{
    WebIDL::log_trace(vm, "SVGAElementPrototype::rel_list_setter");
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

    // 1. Let Q be ? Get(jsValue, id).
    auto receiver_value = TRY(impl->get("relList"_utf16_fly_string));

    // 2. If Q is not an Object, then throw a TypeError.
    if (!receiver_value.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, receiver_value);
    auto& receiver = receiver_value.as_object();

    // 3. Let forwardId be the identifier argument of the [PutForwards] extended attribute.
    auto forward_id = "value"_utf16_fly_string;

    // 4. Perform ? Set(Q, forwardId, V, false).
    TRY(receiver.set(JS::PropertyKey { forward_id, JS::PropertyKey::StringMayBeNumber::No }, value, JS::Object::ShouldThrowExceptions::No));

    // 5. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(SVGAElementPrototype::hreflang_getter)
{
    WebIDL::log_trace(vm, "SVGAElementPrototype::hreflang_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto contentAttributeValue = impl->attribute("hreflang"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGAElementPrototype::hreflang_setter)
{
    WebIDL::log_trace(vm, "SVGAElementPrototype::hreflang_setter");
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

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    impl->set_attribute_value("hreflang"_fly_string, cpp_value);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(SVGAElementPrototype::type_getter)
{
    WebIDL::log_trace(vm, "SVGAElementPrototype::type_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto contentAttributeValue = impl->attribute("type"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGAElementPrototype::type_setter)
{
    WebIDL::log_trace(vm, "SVGAElementPrototype::type_setter");
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

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    impl->set_attribute_value("type"_fly_string, cpp_value);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(SVGAElementPrototype::referrer_policy_getter)
{
    WebIDL::log_trace(vm, "SVGAElementPrototype::referrer_policy_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto contentAttributeValue = impl->attribute("referrerpolicy"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    auto did_set_to_missing_value = false;
    if (!contentAttributeValue.has_value()) {
        retval = ""_string;
        did_set_to_missing_value = true;
    }

    Array valid_values { ""_string, "no-referrer"_string, "no-referrer-when-downgrade"_string, "same-origin"_string, "origin"_string, "strict-origin"_string, "origin-when-cross-origin"_string, "strict-origin-when-cross-origin"_string, "unsafe-url"_string };

    auto has_keyword = false;
    for (auto const& value : valid_values) {
        if (value.equals_ignoring_ascii_case(retval)) {
            has_keyword = true;
            retval = value;
            break;
        }
    }

    if (!has_keyword && !did_set_to_missing_value)
        retval = ""_string;
    
    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGAElementPrototype::referrer_policy_setter)
{
    WebIDL::log_trace(vm, "SVGAElementPrototype::referrer_policy_setter");
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

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    impl->set_attribute_value("referrerpolicy"_fly_string, cpp_value);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(SVGAElementPrototype::href_getter)
{
    WebIDL::log_trace(vm, "SVGAElementPrototype::href_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->href(); }));

    return &const_cast<SVGAnimatedString&>(*retval);

}

} // namespace Web::Bindings
