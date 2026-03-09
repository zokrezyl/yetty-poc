
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
#include <LibWeb/Bindings/WebGLDrawBuffersConstructor.h>
#include <LibWeb/Bindings/WebGLDrawBuffersPrototype.h>
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

GC_DEFINE_ALLOCATOR(WebGLDrawBuffersConstructor);

WebGLDrawBuffersConstructor::WebGLDrawBuffersConstructor(JS::Realm& realm)
    : NativeFunction("WebGLDrawBuffers"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

WebGLDrawBuffersConstructor::~WebGLDrawBuffersConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> WebGLDrawBuffersConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "WebGLDrawBuffers");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> WebGLDrawBuffersConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "WebGLDrawBuffersConstructor::construct");

    return vm().throw_completion<JS::TypeError>(JS::ErrorType::NotAConstructor, "WebGLDrawBuffers");
}


void WebGLDrawBuffersConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    define_direct_property(vm.names.length, JS::Value(0), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "WebGLDrawBuffers"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<WebGLDrawBuffersPrototype>(realm, "WebGLDrawBuffers"_fly_string), 0);


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

}

} // namespace Web::Bindings
