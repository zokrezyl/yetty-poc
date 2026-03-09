
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
#include <LibWeb/Bindings/WebGLDrawBuffersPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/Extensions/WebGLDrawBuffers.h>

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

GC_DEFINE_ALLOCATOR(WebGLDrawBuffersPrototype);

WebGLDrawBuffersPrototype::WebGLDrawBuffersPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

WebGLDrawBuffersPrototype::~WebGLDrawBuffersPrototype()
{
}

void WebGLDrawBuffersPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto constant_COLOR_ATTACHMENT0_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE0));

    define_direct_property("COLOR_ATTACHMENT0_WEBGL"_utf16_fly_string, constant_COLOR_ATTACHMENT0_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT1_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE1));

    define_direct_property("COLOR_ATTACHMENT1_WEBGL"_utf16_fly_string, constant_COLOR_ATTACHMENT1_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT2_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE2));

    define_direct_property("COLOR_ATTACHMENT2_WEBGL"_utf16_fly_string, constant_COLOR_ATTACHMENT2_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT3_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE3));

    define_direct_property("COLOR_ATTACHMENT3_WEBGL"_utf16_fly_string, constant_COLOR_ATTACHMENT3_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT4_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE4));

    define_direct_property("COLOR_ATTACHMENT4_WEBGL"_utf16_fly_string, constant_COLOR_ATTACHMENT4_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT5_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE5));

    define_direct_property("COLOR_ATTACHMENT5_WEBGL"_utf16_fly_string, constant_COLOR_ATTACHMENT5_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT6_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE6));

    define_direct_property("COLOR_ATTACHMENT6_WEBGL"_utf16_fly_string, constant_COLOR_ATTACHMENT6_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT7_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE7));

    define_direct_property("COLOR_ATTACHMENT7_WEBGL"_utf16_fly_string, constant_COLOR_ATTACHMENT7_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT8_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE8));

    define_direct_property("COLOR_ATTACHMENT8_WEBGL"_utf16_fly_string, constant_COLOR_ATTACHMENT8_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT9_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE9));

    define_direct_property("COLOR_ATTACHMENT9_WEBGL"_utf16_fly_string, constant_COLOR_ATTACHMENT9_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT10_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CEA));

    define_direct_property("COLOR_ATTACHMENT10_WEBGL"_utf16_fly_string, constant_COLOR_ATTACHMENT10_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT11_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CEB));

    define_direct_property("COLOR_ATTACHMENT11_WEBGL"_utf16_fly_string, constant_COLOR_ATTACHMENT11_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT12_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CEC));

    define_direct_property("COLOR_ATTACHMENT12_WEBGL"_utf16_fly_string, constant_COLOR_ATTACHMENT12_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT13_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CED));

    define_direct_property("COLOR_ATTACHMENT13_WEBGL"_utf16_fly_string, constant_COLOR_ATTACHMENT13_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT14_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CEE));

    define_direct_property("COLOR_ATTACHMENT14_WEBGL"_utf16_fly_string, constant_COLOR_ATTACHMENT14_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT15_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CEF));

    define_direct_property("COLOR_ATTACHMENT15_WEBGL"_utf16_fly_string, constant_COLOR_ATTACHMENT15_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER0_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8825));

    define_direct_property("DRAW_BUFFER0_WEBGL"_utf16_fly_string, constant_DRAW_BUFFER0_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER1_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8826));

    define_direct_property("DRAW_BUFFER1_WEBGL"_utf16_fly_string, constant_DRAW_BUFFER1_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER2_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8827));

    define_direct_property("DRAW_BUFFER2_WEBGL"_utf16_fly_string, constant_DRAW_BUFFER2_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER3_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8828));

    define_direct_property("DRAW_BUFFER3_WEBGL"_utf16_fly_string, constant_DRAW_BUFFER3_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER4_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8829));

    define_direct_property("DRAW_BUFFER4_WEBGL"_utf16_fly_string, constant_DRAW_BUFFER4_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER5_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882A));

    define_direct_property("DRAW_BUFFER5_WEBGL"_utf16_fly_string, constant_DRAW_BUFFER5_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER6_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882B));

    define_direct_property("DRAW_BUFFER6_WEBGL"_utf16_fly_string, constant_DRAW_BUFFER6_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER7_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882C));

    define_direct_property("DRAW_BUFFER7_WEBGL"_utf16_fly_string, constant_DRAW_BUFFER7_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER8_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882D));

    define_direct_property("DRAW_BUFFER8_WEBGL"_utf16_fly_string, constant_DRAW_BUFFER8_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER9_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882E));

    define_direct_property("DRAW_BUFFER9_WEBGL"_utf16_fly_string, constant_DRAW_BUFFER9_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER10_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882F));

    define_direct_property("DRAW_BUFFER10_WEBGL"_utf16_fly_string, constant_DRAW_BUFFER10_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER11_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8830));

    define_direct_property("DRAW_BUFFER11_WEBGL"_utf16_fly_string, constant_DRAW_BUFFER11_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER12_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8831));

    define_direct_property("DRAW_BUFFER12_WEBGL"_utf16_fly_string, constant_DRAW_BUFFER12_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER13_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8832));

    define_direct_property("DRAW_BUFFER13_WEBGL"_utf16_fly_string, constant_DRAW_BUFFER13_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER14_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8833));

    define_direct_property("DRAW_BUFFER14_WEBGL"_utf16_fly_string, constant_DRAW_BUFFER14_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER15_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8834));

    define_direct_property("DRAW_BUFFER15_WEBGL"_utf16_fly_string, constant_DRAW_BUFFER15_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_MAX_COLOR_ATTACHMENTS_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CDF));

    define_direct_property("MAX_COLOR_ATTACHMENTS_WEBGL"_utf16_fly_string, constant_MAX_COLOR_ATTACHMENTS_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_MAX_DRAW_BUFFERS_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8824));

    define_direct_property("MAX_DRAW_BUFFERS_WEBGL"_utf16_fly_string, constant_MAX_DRAW_BUFFERS_WEBGL_value, JS::Attribute::Enumerable);

    define_native_function(realm, "drawBuffersWEBGL"_utf16_fly_string, draw_buffers_webgl, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "WebGLDrawBuffers"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void WebGLDrawBuffersPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<WebGLDrawBuffers*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<WebGLDrawBuffers>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLDrawBuffers");
    return static_cast<WebGLDrawBuffers*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(WebGLDrawBuffersPrototype::draw_buffers_webgl)
{
    WebIDL::log_trace(vm, "WebGLDrawBuffersPrototype::draw_buffers_webgl");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "drawBuffersWEBGL");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg0);

    auto arg0_iterator_method0 = TRY(arg0.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg0_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg0);

    auto arg0_iterator1 = TRY(JS::get_iterator_from_method(vm, arg0, *arg0_iterator_method0));

    Vector<WebIDL::UnsignedLong> buffers;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg0_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::UnsignedLong sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    buffers.append(sequence_item1);
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->draw_buffers_webgl(buffers); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
