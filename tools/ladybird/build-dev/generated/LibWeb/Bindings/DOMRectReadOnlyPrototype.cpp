
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
#include <LibWeb/Bindings/DOMRectReadOnlyPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRectReadOnly.h>

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

GC_DEFINE_ALLOCATOR(DOMRectReadOnlyPrototype);

DOMRectReadOnlyPrototype::DOMRectReadOnlyPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

DOMRectReadOnlyPrototype::~DOMRectReadOnlyPrototype()
{
}

void DOMRectReadOnlyPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_x_getter = JS::NativeFunction::create(realm, x_getter, 0, "x"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_x_setter;

    define_direct_accessor("x"_utf16_fly_string, native_x_getter, native_x_setter, default_attributes);

    auto native_y_getter = JS::NativeFunction::create(realm, y_getter, 0, "y"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_y_setter;

    define_direct_accessor("y"_utf16_fly_string, native_y_getter, native_y_setter, default_attributes);

    auto native_width_getter = JS::NativeFunction::create(realm, width_getter, 0, "width"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_width_setter;

    define_direct_accessor("width"_utf16_fly_string, native_width_getter, native_width_setter, default_attributes);

    auto native_height_getter = JS::NativeFunction::create(realm, height_getter, 0, "height"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_height_setter;

    define_direct_accessor("height"_utf16_fly_string, native_height_getter, native_height_setter, default_attributes);

    auto native_top_getter = JS::NativeFunction::create(realm, top_getter, 0, "top"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_top_setter;

    define_direct_accessor("top"_utf16_fly_string, native_top_getter, native_top_setter, default_attributes);

    auto native_right_getter = JS::NativeFunction::create(realm, right_getter, 0, "right"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_right_setter;

    define_direct_accessor("right"_utf16_fly_string, native_right_getter, native_right_setter, default_attributes);

    auto native_bottom_getter = JS::NativeFunction::create(realm, bottom_getter, 0, "bottom"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_bottom_setter;

    define_direct_accessor("bottom"_utf16_fly_string, native_bottom_getter, native_bottom_setter, default_attributes);

    auto native_left_getter = JS::NativeFunction::create(realm, left_getter, 0, "left"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_left_setter;

    define_direct_accessor("left"_utf16_fly_string, native_left_getter, native_left_setter, default_attributes);

    define_native_function(realm, "toJSON"_utf16_fly_string, to_json, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "DOMRectReadOnly"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void DOMRectReadOnlyPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Geometry::DOMRectReadOnly*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Geometry::DOMRectReadOnly>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMRectReadOnly");
    return static_cast<Geometry::DOMRectReadOnly*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(DOMRectReadOnlyPrototype::x_getter)
{
    WebIDL::log_trace(vm, "DOMRectReadOnlyPrototype::x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->x(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMRectReadOnlyPrototype::y_getter)
{
    WebIDL::log_trace(vm, "DOMRectReadOnlyPrototype::y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->y(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMRectReadOnlyPrototype::width_getter)
{
    WebIDL::log_trace(vm, "DOMRectReadOnlyPrototype::width_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->width(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMRectReadOnlyPrototype::height_getter)
{
    WebIDL::log_trace(vm, "DOMRectReadOnlyPrototype::height_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->height(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMRectReadOnlyPrototype::top_getter)
{
    WebIDL::log_trace(vm, "DOMRectReadOnlyPrototype::top_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->top(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMRectReadOnlyPrototype::right_getter)
{
    WebIDL::log_trace(vm, "DOMRectReadOnlyPrototype::right_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->right(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMRectReadOnlyPrototype::bottom_getter)
{
    WebIDL::log_trace(vm, "DOMRectReadOnlyPrototype::bottom_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bottom(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMRectReadOnlyPrototype::left_getter)
{
    WebIDL::log_trace(vm, "DOMRectReadOnlyPrototype::left_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->left(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DOMRectReadOnlyPrototype::to_json)
{
    WebIDL::log_trace(vm, "DOMRectReadOnlyPrototype::to_json");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

    auto x_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->x(); }));

    JS::Value x_retval_wrapped;

    x_retval_wrapped = JS::Value(x_retval);

    MUST(result->create_data_property("x"_utf16_fly_string, x_retval_wrapped));

    auto y_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->y(); }));

    JS::Value y_retval_wrapped;

    y_retval_wrapped = JS::Value(y_retval);

    MUST(result->create_data_property("y"_utf16_fly_string, y_retval_wrapped));

    auto width_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->width(); }));

    JS::Value width_retval_wrapped;

    width_retval_wrapped = JS::Value(width_retval);

    MUST(result->create_data_property("width"_utf16_fly_string, width_retval_wrapped));

    auto height_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->height(); }));

    JS::Value height_retval_wrapped;

    height_retval_wrapped = JS::Value(height_retval);

    MUST(result->create_data_property("height"_utf16_fly_string, height_retval_wrapped));

    auto top_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->top(); }));

    JS::Value top_retval_wrapped;

    top_retval_wrapped = JS::Value(top_retval);

    MUST(result->create_data_property("top"_utf16_fly_string, top_retval_wrapped));

    auto right_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->right(); }));

    JS::Value right_retval_wrapped;

    right_retval_wrapped = JS::Value(right_retval);

    MUST(result->create_data_property("right"_utf16_fly_string, right_retval_wrapped));

    auto bottom_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bottom(); }));

    JS::Value bottom_retval_wrapped;

    bottom_retval_wrapped = JS::Value(bottom_retval);

    MUST(result->create_data_property("bottom"_utf16_fly_string, bottom_retval_wrapped));

    auto left_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->left(); }));

    JS::Value left_retval_wrapped;

    left_retval_wrapped = JS::Value(left_retval);

    MUST(result->create_data_property("left"_utf16_fly_string, left_retval_wrapped));

    return result;
}

} // namespace Web::Bindings
