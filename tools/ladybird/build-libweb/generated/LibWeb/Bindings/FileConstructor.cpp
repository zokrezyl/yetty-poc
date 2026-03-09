
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
#include <LibWeb/Bindings/FileConstructor.h>
#include <LibWeb/Bindings/FilePrototype.h>
#include <LibWeb/Bindings/ExceptionOrUtils.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/HTML/WindowProxy.h>
#include <LibWeb/WebIDL/AbstractOperations.h>
#include <LibWeb/WebIDL/Buffers.h>
#include <LibWeb/WebIDL/CallbackType.h>
#include <LibWeb/WebIDL/OverloadResolution.h>
#include <LibWeb/WebIDL/Tracing.h>
#include <LibWeb/WebIDL/Types.h>

#if __has_include(<LibWeb/Bindings/BlobPrototype.h>)
#    include <LibWeb/Bindings/BlobPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/File.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

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

GC_DEFINE_ALLOCATOR(FileConstructor);

FileConstructor::FileConstructor(JS::Realm& realm)
    : NativeFunction("File"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

FileConstructor::~FileConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> FileConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "File");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> FileConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "FileConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface File:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<FilePrototype>(*target_realm, "File"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "File", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg0);

    auto arg0_iterator_method0 = TRY(arg0.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg0_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg0);

    auto arg0_iterator1 = TRY(JS::get_iterator_from_method(vm, arg0, *arg0_iterator_method0));

    Vector<Variant<GC::Root<WebIDL::BufferSource>, GC::Root<Blob>, String>> file_bits;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg0_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    auto next_item1_to_variant = [&vm, &realm](JS::Value next_item1) -> JS::ThrowCompletionOr<Variant<GC::Root<WebIDL::BufferSource>, GC::Root<Blob>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (next_item1.is_object()) {
            [[maybe_unused]] auto& next_item1_object = next_item1.as_object();

            if (is<PlatformObject>(next_item1_object)) {

                if (auto* next_item1_result = as_if<Blob>(next_item1_object))
                    return GC::make_root(*next_item1_result);

            }

            if (is<JS::ArrayBuffer>(next_item1_object) || is<JS::DataView>(next_item1_object) || is<JS::TypedArrayBase>(next_item1_object)) {
                GC::Ref<WebIDL::BufferSource> source_object = realm.create<WebIDL::BufferSource>(next_item1_object);
                return GC::make_root(source_object);
            }

        }

    String next_item1_string;
    if (!false || !next_item1.is_null()) {
        next_item1_string = TRY(WebIDL::to_usv_string(vm, next_item1));
    }

        return { next_item1_string };

    };

    Variant<GC::Root<WebIDL::BufferSource>, GC::Root<Blob>, String> sequence_item1 = TRY(next_item1_to_variant(next_item1));

    file_bits.append(sequence_item1);
    }

    auto arg1 = vm.argument(1);

    String file_name;
    if (!false || !arg1.is_null()) {
        file_name = TRY(WebIDL::to_usv_string(vm, arg1));
    }

    auto arg2 = vm.argument(2);

    if (!arg2.is_nullish() && !arg2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "FilePropertyBag");

    FilePropertyBag options {};

    auto last_modified_property_value_0 = JS::js_undefined();
    if (arg2.is_object())
        last_modified_property_value_0 = TRY(arg2.as_object().get("lastModified"_utf16_fly_string));

    if (!last_modified_property_value_0.is_undefined()) {

    Optional<WebIDL::LongLong> last_modified_value_0;

    if (!last_modified_property_value_0.is_undefined())

    last_modified_value_0 = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, last_modified_property_value_0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    options.last_modified = last_modified_value_0;

    }

    auto endings_property_value_1 = JS::js_undefined();
    if (arg2.is_object())
        endings_property_value_1 = TRY(arg2.as_object().get("endings"_utf16_fly_string));

    EndingType endings_value_1 { EndingType::Transparent };

    if (!endings_property_value_1.is_undefined()) {

    auto endings_property_value_1_string = TRY(endings_property_value_1.to_string(vm));

    if (endings_property_value_1_string == "transparent"sv)
        endings_value_1 = EndingType::Transparent;

    else if (endings_property_value_1_string == "native"sv)
        endings_value_1 = EndingType::Native;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, endings_property_value_1_string, "EndingType");

    }

    options.endings = endings_value_1;

    auto type_property_value_2 = JS::js_undefined();
    if (arg2.is_object())
        type_property_value_2 = TRY(arg2.as_object().get("type"_utf16_fly_string));

    String type_value_2;

    if (!type_property_value_2.is_undefined()) {
        if (!false || !type_property_value_2.is_null())
            type_value_2 = TRY(WebIDL::to_string(vm, type_property_value_2));
    } else {
        type_value_2 = ""_string;
    }

    options.type = type_value_2;

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return FileAPI::File::construct_impl(realm, file_bits, file_name, options); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface File" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within FileAPI::File::construct_impl().
    //  8. Let interfaces be the inclusive inherited interfaces of interface.
    //  9. For every interface ancestor interface in interfaces:
    //    9.1. Let unforgeables be the value of the [[Unforgeables]] slot of the interface object of ancestor interface in realm.
    //    9.2. Let keys be ! unforgeables.[[OwnPropertyKeys]]().
    //    9.3. For each element key of keys:
    //      9.3.1. Let descriptor be ! unforgeables.[[GetOwnProperty]](key).
    //      9.3.2. Perform ! DefinePropertyOrThrow(instance, key, descriptor).
    //  10. If interface is declared with the [Global] extended attribute, then:
    //    10.1. Define the regular operations of interface on instance, given realm.
    //    10.2. Define the regular attributes of interface on instance, given realm.
    //    10.3. Define the iteration methods of interface on instance given realm.
    //    10.4. Define the asynchronous iteration methods of interface on instance given realm.
    //    10.5. Define the global property references on instance, given realm.
    //    10.6. Set instance.[[SetPrototypeOf]] as defined in § 3.8.1 [[SetPrototypeOf]].
    //  11. Otherwise, if interfaces contains an interface which supports indexed properties, named properties, or both:
    //    11.1. Set instance.[[GetOwnProperty]] as defined in § 3.9.1 [[GetOwnProperty]].
    //    11.2. Set instance.[[Set]] as defined in § 3.9.2 [[Set]].
    //    11.3. Set instance.[[DefineOwnProperty]] as defined in § 3.9.3 [[DefineOwnProperty]].
    //    11.4. Set instance.[[Delete]] as defined in § 3.9.4 [[Delete]].
    //    11.5. Set instance.[[PreventExtensions]] as defined in § 3.9.5 [[PreventExtensions]].
    //    11.6. Set instance.[[OwnPropertyKeys]] as defined in § 3.9.6 [[OwnPropertyKeys]].

    return *impl;
}


void FileConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    set_prototype(&ensure_web_constructor<BlobPrototype>(realm, "Blob"_fly_string));

    define_direct_property(vm.names.length, JS::Value(2), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "File"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<FilePrototype>(realm, "File"_fly_string), 0);


}

} // namespace Web::Bindings
