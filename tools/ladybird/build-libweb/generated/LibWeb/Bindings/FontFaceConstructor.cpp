
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
#include <LibWeb/Bindings/FontFaceConstructor.h>
#include <LibWeb/Bindings/FontFacePrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/FontFace.h>

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

GC_DEFINE_ALLOCATOR(FontFaceConstructor);

FontFaceConstructor::FontFaceConstructor(JS::Realm& realm)
    : NativeFunction("FontFace"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

FontFaceConstructor::~FontFaceConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> FontFaceConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "FontFace");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> FontFaceConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "FontFaceConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface FontFace:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<FontFacePrototype>(*target_realm, "FontFace"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "FontFace", "2");

    auto arg0 = vm.argument(0);

    String family;
    if (!false || !arg0.is_null()) {
        family = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<String, GC::Root<WebIDL::BufferSource>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (is<JS::ArrayBuffer>(arg1_object) || is<JS::DataView>(arg1_object) || is<JS::TypedArrayBase>(arg1_object)) {
                GC::Ref<WebIDL::BufferSource> source_object = realm.create<WebIDL::BufferSource>(arg1_object);
                return GC::make_root(source_object);
            }

        }

    String arg1_string;
    if (!false || !arg1.is_null()) {
        arg1_string = TRY(WebIDL::to_string(vm, arg1));
    }

        return { arg1_string };

    };

    Variant<String, GC::Root<WebIDL::BufferSource>> source = TRY(arg1_to_variant(arg1));

    auto arg2 = vm.argument(2);

    if (!arg2.is_nullish() && !arg2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "FontFaceDescriptors");

    FontFaceDescriptors descriptors {};

    auto ascent_override_property_value_0 = JS::js_undefined();
    if (arg2.is_object())
        ascent_override_property_value_0 = TRY(arg2.as_object().get("ascentOverride"_utf16_fly_string));

    String ascent_override_value_0;

    if (!ascent_override_property_value_0.is_undefined()) {
        if (!false || !ascent_override_property_value_0.is_null())
            ascent_override_value_0 = TRY(WebIDL::to_string(vm, ascent_override_property_value_0));
    } else {
        ascent_override_value_0 = "normal"_string;
    }

    descriptors.ascent_override = ascent_override_value_0;

    auto descent_override_property_value_1 = JS::js_undefined();
    if (arg2.is_object())
        descent_override_property_value_1 = TRY(arg2.as_object().get("descentOverride"_utf16_fly_string));

    String descent_override_value_1;

    if (!descent_override_property_value_1.is_undefined()) {
        if (!false || !descent_override_property_value_1.is_null())
            descent_override_value_1 = TRY(WebIDL::to_string(vm, descent_override_property_value_1));
    } else {
        descent_override_value_1 = "normal"_string;
    }

    descriptors.descent_override = descent_override_value_1;

    auto display_property_value_2 = JS::js_undefined();
    if (arg2.is_object())
        display_property_value_2 = TRY(arg2.as_object().get("display"_utf16_fly_string));

    String display_value_2;

    if (!display_property_value_2.is_undefined()) {
        if (!false || !display_property_value_2.is_null())
            display_value_2 = TRY(WebIDL::to_string(vm, display_property_value_2));
    } else {
        display_value_2 = "auto"_string;
    }

    descriptors.display = display_value_2;

    auto feature_settings_property_value_3 = JS::js_undefined();
    if (arg2.is_object())
        feature_settings_property_value_3 = TRY(arg2.as_object().get("featureSettings"_utf16_fly_string));

    String feature_settings_value_3;

    if (!feature_settings_property_value_3.is_undefined()) {
        if (!false || !feature_settings_property_value_3.is_null())
            feature_settings_value_3 = TRY(WebIDL::to_string(vm, feature_settings_property_value_3));
    } else {
        feature_settings_value_3 = "normal"_string;
    }

    descriptors.feature_settings = feature_settings_value_3;

    auto line_gap_override_property_value_4 = JS::js_undefined();
    if (arg2.is_object())
        line_gap_override_property_value_4 = TRY(arg2.as_object().get("lineGapOverride"_utf16_fly_string));

    String line_gap_override_value_4;

    if (!line_gap_override_property_value_4.is_undefined()) {
        if (!false || !line_gap_override_property_value_4.is_null())
            line_gap_override_value_4 = TRY(WebIDL::to_string(vm, line_gap_override_property_value_4));
    } else {
        line_gap_override_value_4 = "normal"_string;
    }

    descriptors.line_gap_override = line_gap_override_value_4;

    auto stretch_property_value_5 = JS::js_undefined();
    if (arg2.is_object())
        stretch_property_value_5 = TRY(arg2.as_object().get("stretch"_utf16_fly_string));

    String stretch_value_5;

    if (!stretch_property_value_5.is_undefined()) {
        if (!false || !stretch_property_value_5.is_null())
            stretch_value_5 = TRY(WebIDL::to_string(vm, stretch_property_value_5));
    } else {
        stretch_value_5 = "normal"_string;
    }

    descriptors.stretch = stretch_value_5;

    auto style_property_value_6 = JS::js_undefined();
    if (arg2.is_object())
        style_property_value_6 = TRY(arg2.as_object().get("style"_utf16_fly_string));

    String style_value_6;

    if (!style_property_value_6.is_undefined()) {
        if (!false || !style_property_value_6.is_null())
            style_value_6 = TRY(WebIDL::to_string(vm, style_property_value_6));
    } else {
        style_value_6 = "normal"_string;
    }

    descriptors.style = style_value_6;

    auto unicode_range_property_value_7 = JS::js_undefined();
    if (arg2.is_object())
        unicode_range_property_value_7 = TRY(arg2.as_object().get("unicodeRange"_utf16_fly_string));

    String unicode_range_value_7;

    if (!unicode_range_property_value_7.is_undefined()) {
        if (!false || !unicode_range_property_value_7.is_null())
            unicode_range_value_7 = TRY(WebIDL::to_string(vm, unicode_range_property_value_7));
    } else {
        unicode_range_value_7 = "U+0-10FFFF"_string;
    }

    descriptors.unicode_range = unicode_range_value_7;

    auto variation_settings_property_value_8 = JS::js_undefined();
    if (arg2.is_object())
        variation_settings_property_value_8 = TRY(arg2.as_object().get("variationSettings"_utf16_fly_string));

    String variation_settings_value_8;

    if (!variation_settings_property_value_8.is_undefined()) {
        if (!false || !variation_settings_property_value_8.is_null())
            variation_settings_value_8 = TRY(WebIDL::to_string(vm, variation_settings_property_value_8));
    } else {
        variation_settings_value_8 = "normal"_string;
    }

    descriptors.variation_settings = variation_settings_value_8;

    auto weight_property_value_9 = JS::js_undefined();
    if (arg2.is_object())
        weight_property_value_9 = TRY(arg2.as_object().get("weight"_utf16_fly_string));

    String weight_value_9;

    if (!weight_property_value_9.is_undefined()) {
        if (!false || !weight_property_value_9.is_null())
            weight_value_9 = TRY(WebIDL::to_string(vm, weight_property_value_9));
    } else {
        weight_value_9 = "normal"_string;
    }

    descriptors.weight = weight_value_9;

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return CSS::FontFace::construct_impl(realm, family, source, descriptors); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface FontFace" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within CSS::FontFace::construct_impl().
    //  8. Let interfaces be the inclusive inherited interfaces of interface.
    //  9. For every interface ancestor interface in interfaces:
    //    9.1. Let unforgeables be the value of the [[Unforgeables]] slot of the interface object of ancestor interface in realm.
    //    9.2. Let keys be ! unforgeables.[[OwnPropertyKeys]]().
    //    9.3. For each element key of keys:
    //      9.3.1. Let descriptor be ! unforgeables.[[GetOwnProperty]](key).
    //      9.3.2. Perform ! DefinePropertyOrThrow(instance, key, descriptor).
    //  10. If interface is declared with the [Global] extended attribute, then:
    //    10.1. Define the regular operations of interface on instance, given realm.
    //    10.2. Define the regular attributes of interface on instance, given realm.
    //    10.3. Define the iteration methods of interface on instance given realm.
    //    10.4. Define the asynchronous iteration methods of interface on instance given realm.
    //    10.5. Define the global property references on instance, given realm.
    //    10.6. Set instance.[[SetPrototypeOf]] as defined in § 3.8.1 [[SetPrototypeOf]].
    //  11. Otherwise, if interfaces contains an interface which supports indexed properties, named properties, or both:
    //    11.1. Set instance.[[GetOwnProperty]] as defined in § 3.9.1 [[GetOwnProperty]].
    //    11.2. Set instance.[[Set]] as defined in § 3.9.2 [[Set]].
    //    11.3. Set instance.[[DefineOwnProperty]] as defined in § 3.9.3 [[DefineOwnProperty]].
    //    11.4. Set instance.[[Delete]] as defined in § 3.9.4 [[Delete]].
    //    11.5. Set instance.[[PreventExtensions]] as defined in § 3.9.5 [[PreventExtensions]].
    //    11.6. Set instance.[[OwnPropertyKeys]] as defined in § 3.9.6 [[OwnPropertyKeys]].

    return *impl;
}


void FontFaceConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    define_direct_property(vm.names.length, JS::Value(2), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "FontFace"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<FontFacePrototype>(realm, "FontFace"_fly_string), 0);


}

} // namespace Web::Bindings
