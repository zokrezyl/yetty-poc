
#include <AK/Function.h>
#include <AK/TypeCasts.h>
#include <LibJS/Runtime/Array.h>
#include <LibJS/Runtime/Error.h>
#include <LibJS/Runtime/FunctionObject.h>
#include <LibJS/Runtime/GlobalObject.h>
#include <LibJS/Runtime/TypedArray.h>
#include <LibWeb/Bindings/ReadableStreamAsyncIteratorPrototype.h>
#include <LibWeb/Bindings/ExceptionOrUtils.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/WebIDL/AsyncIterator.h>
#include <LibWeb/WebIDL/Tracing.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/QueuingStrategy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamBYOBReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamDefaultReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStreamDefaultWriter.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#   include <LibWeb/Streams/ReadableStreamAsyncIterator.h>

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

GC_DEFINE_ALLOCATOR(ReadableStreamAsyncIteratorPrototype);

ReadableStreamAsyncIteratorPrototype::ReadableStreamAsyncIteratorPrototype(JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().async_iterator_prototype())
{
}

ReadableStreamAsyncIteratorPrototype::~ReadableStreamAsyncIteratorPrototype()
{
}

void ReadableStreamAsyncIteratorPrototype::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    Base::initialize(realm);
    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "ReadableStream AsyncIterator"_string), JS::Attribute::Configurable);

    define_native_function(realm, vm.names.next, next, 0, JS::default_attributes);
    define_native_function(realm, vm.names.return_, return_, 1, JS::default_attributes);
}

JS_DEFINE_NATIVE_FUNCTION(ReadableStreamAsyncIteratorPrototype::next)
{
    WebIDL::log_trace(vm, "ReadableStreamAsyncIteratorPrototype::next");
    auto& realm = *vm.current_realm();

    return TRY(throw_dom_exception_if_needed(vm, [&] {
        return WebIDL::AsyncIterator::next<Streams::ReadableStreamAsyncIterator>(realm, "ReadableStreamAsyncIterator"sv);
    }));
}

JS_DEFINE_NATIVE_FUNCTION(ReadableStreamAsyncIteratorPrototype::return_)
{
    WebIDL::log_trace(vm, "ReadableStreamAsyncIteratorPrototype::return");
    auto& realm = *vm.current_realm();

    auto value = vm.argument(0);

    return TRY(throw_dom_exception_if_needed(vm, [&] {
        return WebIDL::AsyncIterator::return_<Streams::ReadableStreamAsyncIterator>(realm, "ReadableStreamAsyncIterator"sv, value);
    }));
}

} // namespace Web::Bindings
