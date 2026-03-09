
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
#include <LibWeb/Bindings/CSSFontFeatureValuesMapPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSFontFeatureValuesMap.h>

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

GC_DEFINE_ALLOCATOR(CSSFontFeatureValuesMapPrototype);

CSSFontFeatureValuesMapPrototype::CSSFontFeatureValuesMapPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

CSSFontFeatureValuesMapPrototype::~CSSFontFeatureValuesMapPrototype()
{
}

void CSSFontFeatureValuesMapPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    define_native_function(realm, "set"_utf16_fly_string, set, 2, default_attributes);

    define_native_accessor(realm, vm.names.size, get_size, nullptr, JS::Attribute::Enumerable | JS::Attribute::Configurable);
    define_native_function(realm, vm.names.entries, entries, 0, default_attributes);
    define_direct_property(vm.well_known_symbol_iterator(), get_without_side_effects(vm.names.entries), JS::Attribute::Configurable | JS::Attribute::Writable);
    define_native_function(realm, vm.names.keys, keys, 0, default_attributes);
    define_native_function(realm, vm.names.values, values, 0, default_attributes);
    define_native_function(realm, vm.names.forEach, for_each, 1, default_attributes);
    define_native_function(realm, vm.names.get, get, 1, default_attributes);
    define_native_function(realm, vm.names.has, has, 1, default_attributes);
    define_native_function(realm, vm.names.delete_, delete_, 1, default_attributes);
    define_native_function(realm, vm.names.clear, clear, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "CSSFontFeatureValuesMap"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void CSSFontFeatureValuesMapPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<CSS::CSSFontFeatureValuesMap*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<CSS::CSSFontFeatureValuesMap>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CSSFontFeatureValuesMap");
    return static_cast<CSS::CSSFontFeatureValuesMap*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFeatureValuesMapPrototype::set)
{
    WebIDL::log_trace(vm, "CSSFontFeatureValuesMapPrototype::set");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "set", "2");

    auto arg0 = vm.argument(0);

    String feature_value_name;
    if (!false || !arg0.is_null()) {
        feature_value_name = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<WebIDL::UnsignedLong, Vector<WebIDL::UnsignedLong>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<WebIDL::UnsignedLong> values;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::UnsignedLong sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    values.append(sequence_item1);
    }


            return values;
        }

        }

        if (arg1.is_number()) {

    WebIDL::UnsignedLong arg1_number;

    arg1_number = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

            return { arg1_number };
        }

    WebIDL::UnsignedLong arg1_number;

    arg1_number = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

        return { arg1_number };

    };

    Variant<WebIDL::UnsignedLong, Vector<WebIDL::UnsignedLong>> values = TRY(arg1_to_variant(arg1));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set(feature_value_name, values); }));

    return JS::js_undefined();

}

// https://webidl.spec.whatwg.org/#js-map-size
JS_DEFINE_NATIVE_FUNCTION(CSSFontFeatureValuesMapPrototype::get_size)
{
    WebIDL::log_trace(vm, "CSSFontFeatureValuesMapPrototype::size");
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Map> map = impl->map_entries();

    return map->map_size();
}

// https://webidl.spec.whatwg.org/#js-map-entries
JS_DEFINE_NATIVE_FUNCTION(CSSFontFeatureValuesMapPrototype::entries)
{
    WebIDL::log_trace(vm, "CSSFontFeatureValuesMapPrototype::entries");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Map> map = impl->map_entries();

    return TRY(throw_dom_exception_if_needed(vm, [&] { return JS::MapIterator::create(realm, *map, Object::PropertyKind::KeyAndValue); }));
}

// https://webidl.spec.whatwg.org/#js-map-keys
JS_DEFINE_NATIVE_FUNCTION(CSSFontFeatureValuesMapPrototype::keys)
{
    WebIDL::log_trace(vm, "CSSFontFeatureValuesMapPrototype::keys");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Map> map = impl->map_entries();

    return TRY(throw_dom_exception_if_needed(vm, [&] { return JS::MapIterator::create(realm, *map, Object::PropertyKind::Key); }));
}

// https://webidl.spec.whatwg.org/#js-map-values
JS_DEFINE_NATIVE_FUNCTION(CSSFontFeatureValuesMapPrototype::values)
{
    WebIDL::log_trace(vm, "CSSFontFeatureValuesMapPrototype::values");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Map> map = impl->map_entries();

    return TRY(throw_dom_exception_if_needed(vm, [&] { return JS::MapIterator::create(realm, *map, Object::PropertyKind::Value); }));
}

// https://webidl.spec.whatwg.org/#js-map-forEach
JS_DEFINE_NATIVE_FUNCTION(CSSFontFeatureValuesMapPrototype::for_each)
{
    WebIDL::log_trace(vm, "CSSFontFeatureValuesMapPrototype::for_each");
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Map> map = impl->map_entries();

    auto callback = vm.argument(0);
    if (!callback.is_function())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, callback);

    for (auto& entry : *map)
        TRY(JS::call(vm, callback.as_function(), vm.argument(1), entry.key, entry.value, impl));

    return JS::js_undefined();
}

// https://webidl.spec.whatwg.org/#js-map-get
JS_DEFINE_NATIVE_FUNCTION(CSSFontFeatureValuesMapPrototype::get)
{
    WebIDL::log_trace(vm, "CSSFontFeatureValuesMapPrototype::get");
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Map> map = impl->map_entries();

    auto key_arg = vm.argument(0);
    auto key = JS::PrimitiveString::create(vm, TRY(key_arg.to_string(vm)));

    // FIXME: If key is -0, set key to +0.
    // What? Which interfaces have a number as their map key type?

    auto result = map->map_get(key);

    if (!result.has_value())
        return JS::js_undefined();

    return result.release_value();
}

// https://webidl.spec.whatwg.org/#js-map-has
JS_DEFINE_NATIVE_FUNCTION(CSSFontFeatureValuesMapPrototype::has)
{
    WebIDL::log_trace(vm, "CSSFontFeatureValuesMapPrototype::has");
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Map> map = impl->map_entries();

    auto key_arg = vm.argument(0);
    auto key = JS::PrimitiveString::create(vm, TRY(key_arg.to_string(vm)));

    // FIXME: If key is -0, set key to +0.
    // What? Which interfaces have a number as their map key type?

    return map->map_has(key);
}

// https://webidl.spec.whatwg.org/#js-map-delete
JS_DEFINE_NATIVE_FUNCTION(CSSFontFeatureValuesMapPrototype::delete_)
{
    WebIDL::log_trace(vm, "CSSFontFeatureValuesMapPrototype::delete_");
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Map> map = impl->map_entries();

    auto key_arg = vm.argument(0);
    auto key = JS::PrimitiveString::create(vm, TRY(key_arg.to_string(vm)));

    // FIXME: If key is -0, set key to +0.
    // What? Which interfaces have a number as their map key type?

    auto result = map->map_remove(key);
    impl->on_map_modified_from_js({});

    return result;
}

// https://webidl.spec.whatwg.org/#js-map-clear
JS_DEFINE_NATIVE_FUNCTION(CSSFontFeatureValuesMapPrototype::clear)
{
    WebIDL::log_trace(vm, "CSSFontFeatureValuesMapPrototype::clear");
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Map> map = impl->map_entries();

    map->map_clear();
    impl->on_map_modified_from_js({});

    return JS::js_undefined();
}

} // namespace Web::Bindings
