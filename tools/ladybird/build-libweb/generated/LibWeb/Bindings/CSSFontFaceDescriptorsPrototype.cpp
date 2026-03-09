
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
#include <LibWeb/Bindings/CSSFontFaceDescriptorsPrototype.h>
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

#if __has_include(<LibWeb/Bindings/CSSStyleDeclarationPrototype.h>)
#    include <LibWeb/Bindings/CSSStyleDeclarationPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSFontFaceDescriptors.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleDeclaration.h>

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

GC_DEFINE_ALLOCATOR(CSSFontFaceDescriptorsPrototype);

CSSFontFaceDescriptorsPrototype::CSSFontFaceDescriptorsPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

CSSFontFaceDescriptorsPrototype::~CSSFontFaceDescriptorsPrototype()
{
}

void CSSFontFaceDescriptorsPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<CSSStyleDeclarationPrototype>(realm, "CSSStyleDeclaration"_fly_string));


    auto native_src_getter = JS::NativeFunction::create(realm, src_getter, 0, "src"_utf16_fly_string, &realm, "get"sv);

    auto native_src_setter = JS::NativeFunction::create(realm, src_setter, 1, "src"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("src"_utf16_fly_string, native_src_getter, native_src_setter, default_attributes);

    auto native_font_family_regular_getter = JS::NativeFunction::create(realm, font_family_regular_getter, 0, "fontFamily"_utf16_fly_string, &realm, "get"sv);

    auto native_font_family_regular_setter = JS::NativeFunction::create(realm, font_family_regular_setter, 1, "fontFamily"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("fontFamily"_utf16_fly_string, native_font_family_regular_getter, native_font_family_regular_setter, default_attributes);

    auto native_font_family_dashed_getter = JS::NativeFunction::create(realm, font_family_dashed_getter, 0, "font-family"_utf16_fly_string, &realm, "get"sv);

    auto native_font_family_dashed_setter = JS::NativeFunction::create(realm, font_family_dashed_setter, 1, "font-family"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("font-family"_utf16_fly_string, native_font_family_dashed_getter, native_font_family_dashed_setter, default_attributes);

    auto native_font_style_regular_getter = JS::NativeFunction::create(realm, font_style_regular_getter, 0, "fontStyle"_utf16_fly_string, &realm, "get"sv);

    auto native_font_style_regular_setter = JS::NativeFunction::create(realm, font_style_regular_setter, 1, "fontStyle"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("fontStyle"_utf16_fly_string, native_font_style_regular_getter, native_font_style_regular_setter, default_attributes);

    auto native_font_style_dashed_getter = JS::NativeFunction::create(realm, font_style_dashed_getter, 0, "font-style"_utf16_fly_string, &realm, "get"sv);

    auto native_font_style_dashed_setter = JS::NativeFunction::create(realm, font_style_dashed_setter, 1, "font-style"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("font-style"_utf16_fly_string, native_font_style_dashed_getter, native_font_style_dashed_setter, default_attributes);

    auto native_font_weight_regular_getter = JS::NativeFunction::create(realm, font_weight_regular_getter, 0, "fontWeight"_utf16_fly_string, &realm, "get"sv);

    auto native_font_weight_regular_setter = JS::NativeFunction::create(realm, font_weight_regular_setter, 1, "fontWeight"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("fontWeight"_utf16_fly_string, native_font_weight_regular_getter, native_font_weight_regular_setter, default_attributes);

    auto native_font_weight_dashed_getter = JS::NativeFunction::create(realm, font_weight_dashed_getter, 0, "font-weight"_utf16_fly_string, &realm, "get"sv);

    auto native_font_weight_dashed_setter = JS::NativeFunction::create(realm, font_weight_dashed_setter, 1, "font-weight"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("font-weight"_utf16_fly_string, native_font_weight_dashed_getter, native_font_weight_dashed_setter, default_attributes);

    auto native_font_stretch_regular_getter = JS::NativeFunction::create(realm, font_stretch_regular_getter, 0, "fontStretch"_utf16_fly_string, &realm, "get"sv);

    auto native_font_stretch_regular_setter = JS::NativeFunction::create(realm, font_stretch_regular_setter, 1, "fontStretch"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("fontStretch"_utf16_fly_string, native_font_stretch_regular_getter, native_font_stretch_regular_setter, default_attributes);

    auto native_font_stretch_dashed_getter = JS::NativeFunction::create(realm, font_stretch_dashed_getter, 0, "font-stretch"_utf16_fly_string, &realm, "get"sv);

    auto native_font_stretch_dashed_setter = JS::NativeFunction::create(realm, font_stretch_dashed_setter, 1, "font-stretch"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("font-stretch"_utf16_fly_string, native_font_stretch_dashed_getter, native_font_stretch_dashed_setter, default_attributes);

    auto native_font_width_regular_getter = JS::NativeFunction::create(realm, font_width_regular_getter, 0, "fontWidth"_utf16_fly_string, &realm, "get"sv);

    auto native_font_width_regular_setter = JS::NativeFunction::create(realm, font_width_regular_setter, 1, "fontWidth"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("fontWidth"_utf16_fly_string, native_font_width_regular_getter, native_font_width_regular_setter, default_attributes);

    auto native_font_width_dashed_getter = JS::NativeFunction::create(realm, font_width_dashed_getter, 0, "font-width"_utf16_fly_string, &realm, "get"sv);

    auto native_font_width_dashed_setter = JS::NativeFunction::create(realm, font_width_dashed_setter, 1, "font-width"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("font-width"_utf16_fly_string, native_font_width_dashed_getter, native_font_width_dashed_setter, default_attributes);

    auto native_unicode_range_regular_getter = JS::NativeFunction::create(realm, unicode_range_regular_getter, 0, "unicodeRange"_utf16_fly_string, &realm, "get"sv);

    auto native_unicode_range_regular_setter = JS::NativeFunction::create(realm, unicode_range_regular_setter, 1, "unicodeRange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("unicodeRange"_utf16_fly_string, native_unicode_range_regular_getter, native_unicode_range_regular_setter, default_attributes);

    auto native_unicode_range_dashed_getter = JS::NativeFunction::create(realm, unicode_range_dashed_getter, 0, "unicode-range"_utf16_fly_string, &realm, "get"sv);

    auto native_unicode_range_dashed_setter = JS::NativeFunction::create(realm, unicode_range_dashed_setter, 1, "unicode-range"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("unicode-range"_utf16_fly_string, native_unicode_range_dashed_getter, native_unicode_range_dashed_setter, default_attributes);

    auto native_font_feature_settings_regular_getter = JS::NativeFunction::create(realm, font_feature_settings_regular_getter, 0, "fontFeatureSettings"_utf16_fly_string, &realm, "get"sv);

    auto native_font_feature_settings_regular_setter = JS::NativeFunction::create(realm, font_feature_settings_regular_setter, 1, "fontFeatureSettings"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("fontFeatureSettings"_utf16_fly_string, native_font_feature_settings_regular_getter, native_font_feature_settings_regular_setter, default_attributes);

    auto native_font_feature_settings_dashed_getter = JS::NativeFunction::create(realm, font_feature_settings_dashed_getter, 0, "font-feature-settings"_utf16_fly_string, &realm, "get"sv);

    auto native_font_feature_settings_dashed_setter = JS::NativeFunction::create(realm, font_feature_settings_dashed_setter, 1, "font-feature-settings"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("font-feature-settings"_utf16_fly_string, native_font_feature_settings_dashed_getter, native_font_feature_settings_dashed_setter, default_attributes);

    auto native_font_variation_settings_regular_getter = JS::NativeFunction::create(realm, font_variation_settings_regular_getter, 0, "fontVariationSettings"_utf16_fly_string, &realm, "get"sv);

    auto native_font_variation_settings_regular_setter = JS::NativeFunction::create(realm, font_variation_settings_regular_setter, 1, "fontVariationSettings"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("fontVariationSettings"_utf16_fly_string, native_font_variation_settings_regular_getter, native_font_variation_settings_regular_setter, default_attributes);

    auto native_font_variation_settings_dashed_getter = JS::NativeFunction::create(realm, font_variation_settings_dashed_getter, 0, "font-variation-settings"_utf16_fly_string, &realm, "get"sv);

    auto native_font_variation_settings_dashed_setter = JS::NativeFunction::create(realm, font_variation_settings_dashed_setter, 1, "font-variation-settings"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("font-variation-settings"_utf16_fly_string, native_font_variation_settings_dashed_getter, native_font_variation_settings_dashed_setter, default_attributes);

    auto native_font_named_instance_regular_getter = JS::NativeFunction::create(realm, font_named_instance_regular_getter, 0, "fontNamedInstance"_utf16_fly_string, &realm, "get"sv);

    auto native_font_named_instance_regular_setter = JS::NativeFunction::create(realm, font_named_instance_regular_setter, 1, "fontNamedInstance"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("fontNamedInstance"_utf16_fly_string, native_font_named_instance_regular_getter, native_font_named_instance_regular_setter, default_attributes);

    auto native_font_named_instance_dashed_getter = JS::NativeFunction::create(realm, font_named_instance_dashed_getter, 0, "font-named-instance"_utf16_fly_string, &realm, "get"sv);

    auto native_font_named_instance_dashed_setter = JS::NativeFunction::create(realm, font_named_instance_dashed_setter, 1, "font-named-instance"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("font-named-instance"_utf16_fly_string, native_font_named_instance_dashed_getter, native_font_named_instance_dashed_setter, default_attributes);

    auto native_font_display_regular_getter = JS::NativeFunction::create(realm, font_display_regular_getter, 0, "fontDisplay"_utf16_fly_string, &realm, "get"sv);

    auto native_font_display_regular_setter = JS::NativeFunction::create(realm, font_display_regular_setter, 1, "fontDisplay"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("fontDisplay"_utf16_fly_string, native_font_display_regular_getter, native_font_display_regular_setter, default_attributes);

    auto native_font_display_dashed_getter = JS::NativeFunction::create(realm, font_display_dashed_getter, 0, "font-display"_utf16_fly_string, &realm, "get"sv);

    auto native_font_display_dashed_setter = JS::NativeFunction::create(realm, font_display_dashed_setter, 1, "font-display"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("font-display"_utf16_fly_string, native_font_display_dashed_getter, native_font_display_dashed_setter, default_attributes);

    auto native_font_language_override_regular_getter = JS::NativeFunction::create(realm, font_language_override_regular_getter, 0, "fontLanguageOverride"_utf16_fly_string, &realm, "get"sv);

    auto native_font_language_override_regular_setter = JS::NativeFunction::create(realm, font_language_override_regular_setter, 1, "fontLanguageOverride"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("fontLanguageOverride"_utf16_fly_string, native_font_language_override_regular_getter, native_font_language_override_regular_setter, default_attributes);

    auto native_font_language_override_dashed_getter = JS::NativeFunction::create(realm, font_language_override_dashed_getter, 0, "font-language-override"_utf16_fly_string, &realm, "get"sv);

    auto native_font_language_override_dashed_setter = JS::NativeFunction::create(realm, font_language_override_dashed_setter, 1, "font-language-override"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("font-language-override"_utf16_fly_string, native_font_language_override_dashed_getter, native_font_language_override_dashed_setter, default_attributes);

    auto native_ascent_override_regular_getter = JS::NativeFunction::create(realm, ascent_override_regular_getter, 0, "ascentOverride"_utf16_fly_string, &realm, "get"sv);

    auto native_ascent_override_regular_setter = JS::NativeFunction::create(realm, ascent_override_regular_setter, 1, "ascentOverride"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ascentOverride"_utf16_fly_string, native_ascent_override_regular_getter, native_ascent_override_regular_setter, default_attributes);

    auto native_ascent_override_dashed_getter = JS::NativeFunction::create(realm, ascent_override_dashed_getter, 0, "ascent-override"_utf16_fly_string, &realm, "get"sv);

    auto native_ascent_override_dashed_setter = JS::NativeFunction::create(realm, ascent_override_dashed_setter, 1, "ascent-override"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ascent-override"_utf16_fly_string, native_ascent_override_dashed_getter, native_ascent_override_dashed_setter, default_attributes);

    auto native_descent_override_regular_getter = JS::NativeFunction::create(realm, descent_override_regular_getter, 0, "descentOverride"_utf16_fly_string, &realm, "get"sv);

    auto native_descent_override_regular_setter = JS::NativeFunction::create(realm, descent_override_regular_setter, 1, "descentOverride"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("descentOverride"_utf16_fly_string, native_descent_override_regular_getter, native_descent_override_regular_setter, default_attributes);

    auto native_descent_override_dashed_getter = JS::NativeFunction::create(realm, descent_override_dashed_getter, 0, "descent-override"_utf16_fly_string, &realm, "get"sv);

    auto native_descent_override_dashed_setter = JS::NativeFunction::create(realm, descent_override_dashed_setter, 1, "descent-override"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("descent-override"_utf16_fly_string, native_descent_override_dashed_getter, native_descent_override_dashed_setter, default_attributes);

    auto native_line_gap_override_regular_getter = JS::NativeFunction::create(realm, line_gap_override_regular_getter, 0, "lineGapOverride"_utf16_fly_string, &realm, "get"sv);

    auto native_line_gap_override_regular_setter = JS::NativeFunction::create(realm, line_gap_override_regular_setter, 1, "lineGapOverride"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("lineGapOverride"_utf16_fly_string, native_line_gap_override_regular_getter, native_line_gap_override_regular_setter, default_attributes);

    auto native_line_gap_override_dashed_getter = JS::NativeFunction::create(realm, line_gap_override_dashed_getter, 0, "line-gap-override"_utf16_fly_string, &realm, "get"sv);

    auto native_line_gap_override_dashed_setter = JS::NativeFunction::create(realm, line_gap_override_dashed_setter, 1, "line-gap-override"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("line-gap-override"_utf16_fly_string, native_line_gap_override_dashed_getter, native_line_gap_override_dashed_setter, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "CSSFontFaceDescriptors"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void CSSFontFaceDescriptorsPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<CSS::CSSFontFaceDescriptors*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<CSS::CSSFontFaceDescriptors>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CSSFontFaceDescriptors");
    return static_cast<CSS::CSSFontFaceDescriptors*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::src_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::src_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->src(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::src_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::src_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_src(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_family_regular_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_family_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_family(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_family_regular_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_family_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_family(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_family_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_family_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_family(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_family_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_family_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_family(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_style_regular_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_style_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_style(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_style_regular_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_style_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_style(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_style_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_style_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_style(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_style_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_style_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_style(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_weight_regular_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_weight_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_weight(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_weight_regular_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_weight_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_weight(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_weight_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_weight_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_weight(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_weight_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_weight_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_weight(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_stretch_regular_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_stretch_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_width(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_stretch_regular_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_stretch_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_width(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_stretch_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_stretch_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_width(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_stretch_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_stretch_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_width(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_width_regular_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_width_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_width(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_width_regular_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_width_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_width(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_width_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_width_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_width(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_width_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_width_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_width(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::unicode_range_regular_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::unicode_range_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->unicode_range(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::unicode_range_regular_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::unicode_range_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_unicode_range(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::unicode_range_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::unicode_range_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->unicode_range(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::unicode_range_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::unicode_range_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_unicode_range(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_feature_settings_regular_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_feature_settings_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_feature_settings(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_feature_settings_regular_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_feature_settings_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_feature_settings(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_feature_settings_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_feature_settings_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_feature_settings(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_feature_settings_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_feature_settings_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_feature_settings(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_variation_settings_regular_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_variation_settings_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_variation_settings(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_variation_settings_regular_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_variation_settings_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_variation_settings(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_variation_settings_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_variation_settings_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_variation_settings(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_variation_settings_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_variation_settings_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_variation_settings(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_named_instance_regular_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_named_instance_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_named_instance(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_named_instance_regular_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_named_instance_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_named_instance(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_named_instance_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_named_instance_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_named_instance(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_named_instance_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_named_instance_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_named_instance(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_display_regular_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_display_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_display(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_display_regular_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_display_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_display(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_display_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_display_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_display(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_display_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_display_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_display(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_language_override_regular_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_language_override_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_language_override(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_language_override_regular_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_language_override_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_language_override(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_language_override_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_language_override_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_language_override(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::font_language_override_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::font_language_override_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font_language_override(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::ascent_override_regular_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::ascent_override_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ascent_override(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::ascent_override_regular_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::ascent_override_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ascent_override(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::ascent_override_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::ascent_override_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ascent_override(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::ascent_override_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::ascent_override_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ascent_override(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::descent_override_regular_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::descent_override_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->descent_override(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::descent_override_regular_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::descent_override_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_descent_override(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::descent_override_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::descent_override_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->descent_override(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::descent_override_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::descent_override_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_descent_override(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::line_gap_override_regular_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::line_gap_override_regular_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->line_gap_override(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::line_gap_override_regular_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::line_gap_override_regular_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_line_gap_override(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::line_gap_override_dashed_getter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::line_gap_override_dashed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->line_gap_override(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(CSSFontFaceDescriptorsPrototype::line_gap_override_dashed_setter)
{
    WebIDL::log_trace(vm, "CSSFontFaceDescriptorsPrototype::line_gap_override_dashed_setter");
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
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_line_gap_override(cpp_value); }));

    return JS::js_undefined();
}

} // namespace Web::Bindings
