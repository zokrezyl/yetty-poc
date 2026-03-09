
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
#include <LibWeb/Bindings/EXTTextureNorm16Constructor.h>
#include <LibWeb/Bindings/EXTTextureNorm16Prototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/Extensions/EXTTextureNorm16.h>

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

GC_DEFINE_ALLOCATOR(EXTTextureNorm16Constructor);

EXTTextureNorm16Constructor::EXTTextureNorm16Constructor(JS::Realm& realm)
    : NativeFunction("EXTTextureNorm16"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

EXTTextureNorm16Constructor::~EXTTextureNorm16Constructor()
{
}

JS::ThrowCompletionOr<JS::Value> EXTTextureNorm16Constructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "EXTTextureNorm16");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> EXTTextureNorm16Constructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "EXTTextureNorm16Constructor::construct");

    return vm().throw_completion<JS::TypeError>(JS::ErrorType::NotAConstructor, "EXTTextureNorm16");
}


void EXTTextureNorm16Constructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    define_direct_property(vm.names.length, JS::Value(0), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "EXTTextureNorm16"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<EXTTextureNorm16Prototype>(realm, "EXTTextureNorm16"_fly_string), 0);


    auto constant_R16_EXT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x822A));

    define_direct_property("R16_EXT"_utf16_fly_string, constant_R16_EXT_value, JS::Attribute::Enumerable);

    auto constant_RG16_EXT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x822C));

    define_direct_property("RG16_EXT"_utf16_fly_string, constant_RG16_EXT_value, JS::Attribute::Enumerable);

    auto constant_RGB16_EXT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8054));

    define_direct_property("RGB16_EXT"_utf16_fly_string, constant_RGB16_EXT_value, JS::Attribute::Enumerable);

    auto constant_RGBA16_EXT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x805B));

    define_direct_property("RGBA16_EXT"_utf16_fly_string, constant_RGBA16_EXT_value, JS::Attribute::Enumerable);

    auto constant_R16_SNORM_EXT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F98));

    define_direct_property("R16_SNORM_EXT"_utf16_fly_string, constant_R16_SNORM_EXT_value, JS::Attribute::Enumerable);

    auto constant_RG16_SNORM_EXT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F99));

    define_direct_property("RG16_SNORM_EXT"_utf16_fly_string, constant_RG16_SNORM_EXT_value, JS::Attribute::Enumerable);

    auto constant_RGB16_SNORM_EXT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F9A));

    define_direct_property("RGB16_SNORM_EXT"_utf16_fly_string, constant_RGB16_SNORM_EXT_value, JS::Attribute::Enumerable);

    auto constant_RGBA16_SNORM_EXT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F9B));

    define_direct_property("RGBA16_SNORM_EXT"_utf16_fly_string, constant_RGBA16_SNORM_EXT_value, JS::Attribute::Enumerable);

}

} // namespace Web::Bindings
