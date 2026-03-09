
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
#include <LibWeb/Bindings/PerformanceEntryPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/PerformanceTimeline/PerformanceEntry.h>

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

GC_DEFINE_ALLOCATOR(PerformanceEntryPrototype);

PerformanceEntryPrototype::PerformanceEntryPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

PerformanceEntryPrototype::~PerformanceEntryPrototype()
{
}

void PerformanceEntryPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_name_getter = JS::NativeFunction::create(realm, name_getter, 0, "name"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_name_setter;

    define_direct_accessor("name"_utf16_fly_string, native_name_getter, native_name_setter, default_attributes);

    auto native_entry_type_getter = JS::NativeFunction::create(realm, entry_type_getter, 0, "entryType"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_entry_type_setter;

    define_direct_accessor("entryType"_utf16_fly_string, native_entry_type_getter, native_entry_type_setter, default_attributes);

    auto native_start_time_getter = JS::NativeFunction::create(realm, start_time_getter, 0, "startTime"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_start_time_setter;

    define_direct_accessor("startTime"_utf16_fly_string, native_start_time_getter, native_start_time_setter, default_attributes);

    auto native_duration_getter = JS::NativeFunction::create(realm, duration_getter, 0, "duration"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_duration_setter;

    define_direct_accessor("duration"_utf16_fly_string, native_duration_getter, native_duration_setter, default_attributes);

    define_native_function(realm, "toJSON"_utf16_fly_string, to_json, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "PerformanceEntry"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void PerformanceEntryPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<PerformanceEntry*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<PerformanceEntry>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PerformanceEntry");
    return static_cast<PerformanceEntry*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(PerformanceEntryPrototype::name_getter)
{
    WebIDL::log_trace(vm, "PerformanceEntryPrototype::name_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->name(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceEntryPrototype::entry_type_getter)
{
    WebIDL::log_trace(vm, "PerformanceEntryPrototype::entry_type_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->entry_type(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceEntryPrototype::start_time_getter)
{
    WebIDL::log_trace(vm, "PerformanceEntryPrototype::start_time_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->start_time(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceEntryPrototype::duration_getter)
{
    WebIDL::log_trace(vm, "PerformanceEntryPrototype::duration_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->duration(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(PerformanceEntryPrototype::to_json)
{
    WebIDL::log_trace(vm, "PerformanceEntryPrototype::to_json");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    auto result = JS::Object::create(realm, realm.intrinsics().object_prototype());

    auto name_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->name(); }));

    JS::Value name_retval_wrapped;

    name_retval_wrapped = JS::PrimitiveString::create(vm, name_retval);

    MUST(result->create_data_property("name"_utf16_fly_string, name_retval_wrapped));

    auto entry_type_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->entry_type(); }));

    JS::Value entry_type_retval_wrapped;

    entry_type_retval_wrapped = JS::PrimitiveString::create(vm, entry_type_retval);

    MUST(result->create_data_property("entryType"_utf16_fly_string, entry_type_retval_wrapped));

    auto start_time_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->start_time(); }));

    JS::Value start_time_retval_wrapped;

    start_time_retval_wrapped = JS::Value(start_time_retval);

    MUST(result->create_data_property("startTime"_utf16_fly_string, start_time_retval_wrapped));

    auto duration_retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->duration(); }));

    JS::Value duration_retval_wrapped;

    duration_retval_wrapped = JS::Value(duration_retval);

    MUST(result->create_data_property("duration"_utf16_fly_string, duration_retval_wrapped));

    return result;
}

} // namespace Web::Bindings
