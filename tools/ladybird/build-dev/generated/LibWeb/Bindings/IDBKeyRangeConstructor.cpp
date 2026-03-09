
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
#include <LibWeb/Bindings/IDBKeyRangeConstructor.h>
#include <LibWeb/Bindings/IDBKeyRangePrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBKeyRange.h>

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

GC_DEFINE_ALLOCATOR(IDBKeyRangeConstructor);

IDBKeyRangeConstructor::IDBKeyRangeConstructor(JS::Realm& realm)
    : NativeFunction("IDBKeyRange"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

IDBKeyRangeConstructor::~IDBKeyRangeConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> IDBKeyRangeConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "IDBKeyRange");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> IDBKeyRangeConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "IDBKeyRangeConstructor::construct");

    return vm().throw_completion<JS::TypeError>(JS::ErrorType::NotAConstructor, "IDBKeyRange");
}


void IDBKeyRangeConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    define_direct_property(vm.names.length, JS::Value(0), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "IDBKeyRange"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<IDBKeyRangePrototype>(realm, "IDBKeyRange"_fly_string), 0);


    define_native_function(realm, "only"_utf16_fly_string, only, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "lowerBound"_utf16_fly_string, lower_bound, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "upperBound"_utf16_fly_string, upper_bound, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "bound"_utf16_fly_string, bound, 2, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

}

JS_DEFINE_NATIVE_FUNCTION(IDBKeyRangeConstructor::only)
{
    WebIDL::log_trace(vm, "IDBKeyRangeConstructor::only");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "only");

    auto arg0 = vm.argument(0);

    auto value = arg0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return IDBKeyRange::only(vm, value); }));

    return &const_cast<IDBKeyRange&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBKeyRangeConstructor::lower_bound)
{
    WebIDL::log_trace(vm, "IDBKeyRangeConstructor::lower_bound");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "lowerBound");

    auto arg0 = vm.argument(0);

    auto lower = arg0;

    auto arg1 = vm.argument(1);

    bool open;

    if (!arg1.is_undefined())

    open = arg1.to_boolean();

    else
        open = static_cast<bool>(false);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return IDBKeyRange::lower_bound(vm, lower, open); }));

    return &const_cast<IDBKeyRange&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBKeyRangeConstructor::upper_bound)
{
    WebIDL::log_trace(vm, "IDBKeyRangeConstructor::upper_bound");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "upperBound");

    auto arg0 = vm.argument(0);

    auto upper = arg0;

    auto arg1 = vm.argument(1);

    bool open;

    if (!arg1.is_undefined())

    open = arg1.to_boolean();

    else
        open = static_cast<bool>(false);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return IDBKeyRange::upper_bound(vm, upper, open); }));

    return &const_cast<IDBKeyRange&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(IDBKeyRangeConstructor::bound)
{
    WebIDL::log_trace(vm, "IDBKeyRangeConstructor::bound");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "bound", "2");

    auto arg0 = vm.argument(0);

    auto lower = arg0;

    auto arg1 = vm.argument(1);

    auto upper = arg1;

    auto arg2 = vm.argument(2);

    bool lower_open;

    if (!arg2.is_undefined())

    lower_open = arg2.to_boolean();

    else
        lower_open = static_cast<bool>(false);

    auto arg3 = vm.argument(3);

    bool upper_open;

    if (!arg3.is_undefined())

    upper_open = arg3.to_boolean();

    else
        upper_open = static_cast<bool>(false);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return IDBKeyRange::bound(vm, lower, upper, lower_open, upper_open); }));

    return &const_cast<IDBKeyRange&>(*retval);

}

} // namespace Web::Bindings
