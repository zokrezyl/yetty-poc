
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
#include <LibWeb/Bindings/WebGLCompressedTextureS3tcConstructor.h>
#include <LibWeb/Bindings/WebGLCompressedTextureS3tcPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/Extensions/WebGLCompressedTextureS3tc.h>

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

GC_DEFINE_ALLOCATOR(WebGLCompressedTextureS3tcConstructor);

WebGLCompressedTextureS3tcConstructor::WebGLCompressedTextureS3tcConstructor(JS::Realm& realm)
    : NativeFunction("WebGLCompressedTextureS3tc"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

WebGLCompressedTextureS3tcConstructor::~WebGLCompressedTextureS3tcConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> WebGLCompressedTextureS3tcConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "WebGLCompressedTextureS3tc");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> WebGLCompressedTextureS3tcConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "WebGLCompressedTextureS3tcConstructor::construct");

    return vm().throw_completion<JS::TypeError>(JS::ErrorType::NotAConstructor, "WebGLCompressedTextureS3tc");
}


void WebGLCompressedTextureS3tcConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    define_direct_property(vm.names.length, JS::Value(0), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "WebGLCompressedTextureS3tc"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<WebGLCompressedTextureS3tcPrototype>(realm, "WebGLCompressedTextureS3tc"_fly_string), 0);


    auto constant_COMPRESSED_RGB_S3TC_DXT1_EXT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x83F0));

    define_direct_property("COMPRESSED_RGB_S3TC_DXT1_EXT"_utf16_fly_string, constant_COMPRESSED_RGB_S3TC_DXT1_EXT_value, JS::Attribute::Enumerable);

    auto constant_COMPRESSED_RGBA_S3TC_DXT1_EXT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x83F1));

    define_direct_property("COMPRESSED_RGBA_S3TC_DXT1_EXT"_utf16_fly_string, constant_COMPRESSED_RGBA_S3TC_DXT1_EXT_value, JS::Attribute::Enumerable);

    auto constant_COMPRESSED_RGBA_S3TC_DXT3_EXT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x83F2));

    define_direct_property("COMPRESSED_RGBA_S3TC_DXT3_EXT"_utf16_fly_string, constant_COMPRESSED_RGBA_S3TC_DXT3_EXT_value, JS::Attribute::Enumerable);

    auto constant_COMPRESSED_RGBA_S3TC_DXT5_EXT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x83F3));

    define_direct_property("COMPRESSED_RGBA_S3TC_DXT5_EXT"_utf16_fly_string, constant_COMPRESSED_RGBA_S3TC_DXT5_EXT_value, JS::Attribute::Enumerable);

}

} // namespace Web::Bindings
