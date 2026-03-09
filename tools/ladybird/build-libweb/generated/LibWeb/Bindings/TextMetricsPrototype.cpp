
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
#include <LibWeb/Bindings/TextMetricsPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TextMetrics.h>

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

GC_DEFINE_ALLOCATOR(TextMetricsPrototype);

TextMetricsPrototype::TextMetricsPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

TextMetricsPrototype::~TextMetricsPrototype()
{
}

void TextMetricsPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_width_getter = JS::NativeFunction::create(realm, width_getter, 0, "width"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_width_setter;

    define_direct_accessor("width"_utf16_fly_string, native_width_getter, native_width_setter, default_attributes);

    auto native_actual_bounding_box_left_getter = JS::NativeFunction::create(realm, actual_bounding_box_left_getter, 0, "actualBoundingBoxLeft"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_actual_bounding_box_left_setter;

    define_direct_accessor("actualBoundingBoxLeft"_utf16_fly_string, native_actual_bounding_box_left_getter, native_actual_bounding_box_left_setter, default_attributes);

    auto native_actual_bounding_box_right_getter = JS::NativeFunction::create(realm, actual_bounding_box_right_getter, 0, "actualBoundingBoxRight"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_actual_bounding_box_right_setter;

    define_direct_accessor("actualBoundingBoxRight"_utf16_fly_string, native_actual_bounding_box_right_getter, native_actual_bounding_box_right_setter, default_attributes);

    auto native_font_bounding_box_ascent_getter = JS::NativeFunction::create(realm, font_bounding_box_ascent_getter, 0, "fontBoundingBoxAscent"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_font_bounding_box_ascent_setter;

    define_direct_accessor("fontBoundingBoxAscent"_utf16_fly_string, native_font_bounding_box_ascent_getter, native_font_bounding_box_ascent_setter, default_attributes);

    auto native_font_bounding_box_descent_getter = JS::NativeFunction::create(realm, font_bounding_box_descent_getter, 0, "fontBoundingBoxDescent"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_font_bounding_box_descent_setter;

    define_direct_accessor("fontBoundingBoxDescent"_utf16_fly_string, native_font_bounding_box_descent_getter, native_font_bounding_box_descent_setter, default_attributes);

    auto native_actual_bounding_box_ascent_getter = JS::NativeFunction::create(realm, actual_bounding_box_ascent_getter, 0, "actualBoundingBoxAscent"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_actual_bounding_box_ascent_setter;

    define_direct_accessor("actualBoundingBoxAscent"_utf16_fly_string, native_actual_bounding_box_ascent_getter, native_actual_bounding_box_ascent_setter, default_attributes);

    auto native_actual_bounding_box_descent_getter = JS::NativeFunction::create(realm, actual_bounding_box_descent_getter, 0, "actualBoundingBoxDescent"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_actual_bounding_box_descent_setter;

    define_direct_accessor("actualBoundingBoxDescent"_utf16_fly_string, native_actual_bounding_box_descent_getter, native_actual_bounding_box_descent_setter, default_attributes);

    auto native_em_height_ascent_getter = JS::NativeFunction::create(realm, em_height_ascent_getter, 0, "emHeightAscent"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_em_height_ascent_setter;

    define_direct_accessor("emHeightAscent"_utf16_fly_string, native_em_height_ascent_getter, native_em_height_ascent_setter, default_attributes);

    auto native_em_height_descent_getter = JS::NativeFunction::create(realm, em_height_descent_getter, 0, "emHeightDescent"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_em_height_descent_setter;

    define_direct_accessor("emHeightDescent"_utf16_fly_string, native_em_height_descent_getter, native_em_height_descent_setter, default_attributes);

    auto native_hanging_baseline_getter = JS::NativeFunction::create(realm, hanging_baseline_getter, 0, "hangingBaseline"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_hanging_baseline_setter;

    define_direct_accessor("hangingBaseline"_utf16_fly_string, native_hanging_baseline_getter, native_hanging_baseline_setter, default_attributes);

    auto native_alphabetic_baseline_getter = JS::NativeFunction::create(realm, alphabetic_baseline_getter, 0, "alphabeticBaseline"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_alphabetic_baseline_setter;

    define_direct_accessor("alphabeticBaseline"_utf16_fly_string, native_alphabetic_baseline_getter, native_alphabetic_baseline_setter, default_attributes);

    auto native_ideographic_baseline_getter = JS::NativeFunction::create(realm, ideographic_baseline_getter, 0, "ideographicBaseline"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_ideographic_baseline_setter;

    define_direct_accessor("ideographicBaseline"_utf16_fly_string, native_ideographic_baseline_getter, native_ideographic_baseline_setter, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "TextMetrics"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void TextMetricsPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::TextMetrics*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::TextMetrics>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "TextMetrics");
    return static_cast<HTML::TextMetrics*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(TextMetricsPrototype::width_getter)
{
    WebIDL::log_trace(vm, "TextMetricsPrototype::width_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->width(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(TextMetricsPrototype::actual_bounding_box_left_getter)
{
    WebIDL::log_trace(vm, "TextMetricsPrototype::actual_bounding_box_left_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->actual_bounding_box_left(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(TextMetricsPrototype::actual_bounding_box_right_getter)
{
    WebIDL::log_trace(vm, "TextMetricsPrototype::actual_bounding_box_right_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->actual_bounding_box_right(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(TextMetricsPrototype::font_bounding_box_ascent_getter)
{
    WebIDL::log_trace(vm, "TextMetricsPrototype::font_bounding_box_ascent_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_bounding_box_ascent(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(TextMetricsPrototype::font_bounding_box_descent_getter)
{
    WebIDL::log_trace(vm, "TextMetricsPrototype::font_bounding_box_descent_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font_bounding_box_descent(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(TextMetricsPrototype::actual_bounding_box_ascent_getter)
{
    WebIDL::log_trace(vm, "TextMetricsPrototype::actual_bounding_box_ascent_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->actual_bounding_box_ascent(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(TextMetricsPrototype::actual_bounding_box_descent_getter)
{
    WebIDL::log_trace(vm, "TextMetricsPrototype::actual_bounding_box_descent_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->actual_bounding_box_descent(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(TextMetricsPrototype::em_height_ascent_getter)
{
    WebIDL::log_trace(vm, "TextMetricsPrototype::em_height_ascent_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->em_height_ascent(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(TextMetricsPrototype::em_height_descent_getter)
{
    WebIDL::log_trace(vm, "TextMetricsPrototype::em_height_descent_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->em_height_descent(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(TextMetricsPrototype::hanging_baseline_getter)
{
    WebIDL::log_trace(vm, "TextMetricsPrototype::hanging_baseline_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->hanging_baseline(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(TextMetricsPrototype::alphabetic_baseline_getter)
{
    WebIDL::log_trace(vm, "TextMetricsPrototype::alphabetic_baseline_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->alphabetic_baseline(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(TextMetricsPrototype::ideographic_baseline_getter)
{
    WebIDL::log_trace(vm, "TextMetricsPrototype::ideographic_baseline_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ideographic_baseline(); }));

    return JS::Value(retval);

}

} // namespace Web::Bindings
