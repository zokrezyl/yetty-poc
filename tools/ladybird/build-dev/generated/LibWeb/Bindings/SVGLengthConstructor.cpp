
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
#include <LibWeb/Bindings/SVGLengthConstructor.h>
#include <LibWeb/Bindings/SVGLengthPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGLength.h>

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

GC_DEFINE_ALLOCATOR(SVGLengthConstructor);

SVGLengthConstructor::SVGLengthConstructor(JS::Realm& realm)
    : NativeFunction("SVGLength"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

SVGLengthConstructor::~SVGLengthConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> SVGLengthConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "SVGLength");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> SVGLengthConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "SVGLengthConstructor::construct");

    return vm().throw_completion<JS::TypeError>(JS::ErrorType::NotAConstructor, "SVGLength");
}


void SVGLengthConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    define_direct_property(vm.names.length, JS::Value(0), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "SVGLength"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<SVGLengthPrototype>(realm, "SVGLength"_fly_string), 0);


    auto constant_SVG_LENGTHTYPE_UNKNOWN_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0));

    define_direct_property("SVG_LENGTHTYPE_UNKNOWN"_utf16_fly_string, constant_SVG_LENGTHTYPE_UNKNOWN_value, JS::Attribute::Enumerable);

    auto constant_SVG_LENGTHTYPE_NUMBER_value = JS::Value(static_cast<WebIDL::UnsignedShort>(1));

    define_direct_property("SVG_LENGTHTYPE_NUMBER"_utf16_fly_string, constant_SVG_LENGTHTYPE_NUMBER_value, JS::Attribute::Enumerable);

    auto constant_SVG_LENGTHTYPE_PERCENTAGE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(2));

    define_direct_property("SVG_LENGTHTYPE_PERCENTAGE"_utf16_fly_string, constant_SVG_LENGTHTYPE_PERCENTAGE_value, JS::Attribute::Enumerable);

    auto constant_SVG_LENGTHTYPE_EMS_value = JS::Value(static_cast<WebIDL::UnsignedShort>(3));

    define_direct_property("SVG_LENGTHTYPE_EMS"_utf16_fly_string, constant_SVG_LENGTHTYPE_EMS_value, JS::Attribute::Enumerable);

    auto constant_SVG_LENGTHTYPE_EXS_value = JS::Value(static_cast<WebIDL::UnsignedShort>(4));

    define_direct_property("SVG_LENGTHTYPE_EXS"_utf16_fly_string, constant_SVG_LENGTHTYPE_EXS_value, JS::Attribute::Enumerable);

    auto constant_SVG_LENGTHTYPE_PX_value = JS::Value(static_cast<WebIDL::UnsignedShort>(5));

    define_direct_property("SVG_LENGTHTYPE_PX"_utf16_fly_string, constant_SVG_LENGTHTYPE_PX_value, JS::Attribute::Enumerable);

    auto constant_SVG_LENGTHTYPE_CM_value = JS::Value(static_cast<WebIDL::UnsignedShort>(6));

    define_direct_property("SVG_LENGTHTYPE_CM"_utf16_fly_string, constant_SVG_LENGTHTYPE_CM_value, JS::Attribute::Enumerable);

    auto constant_SVG_LENGTHTYPE_MM_value = JS::Value(static_cast<WebIDL::UnsignedShort>(7));

    define_direct_property("SVG_LENGTHTYPE_MM"_utf16_fly_string, constant_SVG_LENGTHTYPE_MM_value, JS::Attribute::Enumerable);

    auto constant_SVG_LENGTHTYPE_IN_value = JS::Value(static_cast<WebIDL::UnsignedShort>(8));

    define_direct_property("SVG_LENGTHTYPE_IN"_utf16_fly_string, constant_SVG_LENGTHTYPE_IN_value, JS::Attribute::Enumerable);

    auto constant_SVG_LENGTHTYPE_PT_value = JS::Value(static_cast<WebIDL::UnsignedShort>(9));

    define_direct_property("SVG_LENGTHTYPE_PT"_utf16_fly_string, constant_SVG_LENGTHTYPE_PT_value, JS::Attribute::Enumerable);

    auto constant_SVG_LENGTHTYPE_PC_value = JS::Value(static_cast<WebIDL::UnsignedShort>(10));

    define_direct_property("SVG_LENGTHTYPE_PC"_utf16_fly_string, constant_SVG_LENGTHTYPE_PC_value, JS::Attribute::Enumerable);

}

} // namespace Web::Bindings
