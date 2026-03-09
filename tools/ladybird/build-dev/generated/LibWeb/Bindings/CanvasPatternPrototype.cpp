
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
#include <LibWeb/Bindings/CanvasPatternPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CanvasPattern.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrix.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrixReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPoint.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPointReadOnly.h>

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

GC_DEFINE_ALLOCATOR(CanvasPatternPrototype);

CanvasPatternPrototype::CanvasPatternPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

CanvasPatternPrototype::~CanvasPatternPrototype()
{
}

void CanvasPatternPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    define_native_function(realm, "setTransform"_utf16_fly_string, set_transform, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "CanvasPattern"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void CanvasPatternPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::CanvasPattern*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::CanvasPattern>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CanvasPattern");
    return static_cast<HTML::CanvasPattern*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(CanvasPatternPrototype::set_transform)
{
    WebIDL::log_trace(vm, "CanvasPatternPrototype::set_transform");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMMatrix2DInit");

    DOMMatrix2DInit transform {};

    auto a_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        a_property_value_0 = TRY(arg0.as_object().get("a"_utf16_fly_string));

    if (!a_property_value_0.is_undefined()) {

    Optional<double> a_value_0;

    if (!a_property_value_0.is_undefined())
        a_value_0 = TRY(a_property_value_0.to_double(vm));


    transform.a = a_value_0;

    }

    auto b_property_value_1 = JS::js_undefined();
    if (arg0.is_object())
        b_property_value_1 = TRY(arg0.as_object().get("b"_utf16_fly_string));

    if (!b_property_value_1.is_undefined()) {

    Optional<double> b_value_1;

    if (!b_property_value_1.is_undefined())
        b_value_1 = TRY(b_property_value_1.to_double(vm));


    transform.b = b_value_1;

    }

    auto c_property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        c_property_value_2 = TRY(arg0.as_object().get("c"_utf16_fly_string));

    if (!c_property_value_2.is_undefined()) {

    Optional<double> c_value_2;

    if (!c_property_value_2.is_undefined())
        c_value_2 = TRY(c_property_value_2.to_double(vm));


    transform.c = c_value_2;

    }

    auto d_property_value_3 = JS::js_undefined();
    if (arg0.is_object())
        d_property_value_3 = TRY(arg0.as_object().get("d"_utf16_fly_string));

    if (!d_property_value_3.is_undefined()) {

    Optional<double> d_value_3;

    if (!d_property_value_3.is_undefined())
        d_value_3 = TRY(d_property_value_3.to_double(vm));


    transform.d = d_value_3;

    }

    auto e_property_value_4 = JS::js_undefined();
    if (arg0.is_object())
        e_property_value_4 = TRY(arg0.as_object().get("e"_utf16_fly_string));

    if (!e_property_value_4.is_undefined()) {

    Optional<double> e_value_4;

    if (!e_property_value_4.is_undefined())
        e_value_4 = TRY(e_property_value_4.to_double(vm));


    transform.e = e_value_4;

    }

    auto f_property_value_5 = JS::js_undefined();
    if (arg0.is_object())
        f_property_value_5 = TRY(arg0.as_object().get("f"_utf16_fly_string));

    if (!f_property_value_5.is_undefined()) {

    Optional<double> f_value_5;

    if (!f_property_value_5.is_undefined())
        f_value_5 = TRY(f_property_value_5.to_double(vm));


    transform.f = f_value_5;

    }

    auto m11_property_value_6 = JS::js_undefined();
    if (arg0.is_object())
        m11_property_value_6 = TRY(arg0.as_object().get("m11"_utf16_fly_string));

    if (!m11_property_value_6.is_undefined()) {

    Optional<double> m11_value_6;

    if (!m11_property_value_6.is_undefined())
        m11_value_6 = TRY(m11_property_value_6.to_double(vm));


    transform.m11 = m11_value_6;

    }

    auto m12_property_value_7 = JS::js_undefined();
    if (arg0.is_object())
        m12_property_value_7 = TRY(arg0.as_object().get("m12"_utf16_fly_string));

    if (!m12_property_value_7.is_undefined()) {

    Optional<double> m12_value_7;

    if (!m12_property_value_7.is_undefined())
        m12_value_7 = TRY(m12_property_value_7.to_double(vm));


    transform.m12 = m12_value_7;

    }

    auto m21_property_value_8 = JS::js_undefined();
    if (arg0.is_object())
        m21_property_value_8 = TRY(arg0.as_object().get("m21"_utf16_fly_string));

    if (!m21_property_value_8.is_undefined()) {

    Optional<double> m21_value_8;

    if (!m21_property_value_8.is_undefined())
        m21_value_8 = TRY(m21_property_value_8.to_double(vm));


    transform.m21 = m21_value_8;

    }

    auto m22_property_value_9 = JS::js_undefined();
    if (arg0.is_object())
        m22_property_value_9 = TRY(arg0.as_object().get("m22"_utf16_fly_string));

    if (!m22_property_value_9.is_undefined()) {

    Optional<double> m22_value_9;

    if (!m22_property_value_9.is_undefined())
        m22_value_9 = TRY(m22_property_value_9.to_double(vm));


    transform.m22 = m22_value_9;

    }

    auto m41_property_value_10 = JS::js_undefined();
    if (arg0.is_object())
        m41_property_value_10 = TRY(arg0.as_object().get("m41"_utf16_fly_string));

    if (!m41_property_value_10.is_undefined()) {

    Optional<double> m41_value_10;

    if (!m41_property_value_10.is_undefined())
        m41_value_10 = TRY(m41_property_value_10.to_double(vm));


    transform.m41 = m41_value_10;

    }

    auto m42_property_value_11 = JS::js_undefined();
    if (arg0.is_object())
        m42_property_value_11 = TRY(arg0.as_object().get("m42"_utf16_fly_string));

    if (!m42_property_value_11.is_undefined()) {

    Optional<double> m42_value_11;

    if (!m42_property_value_11.is_undefined())
        m42_value_11 = TRY(m42_property_value_11.to_double(vm));


    transform.m42 = m42_value_11;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_transform(transform); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
