
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
#include <LibWeb/Bindings/ModuleConstructor.h>
#include <LibWeb/Bindings/ModulePrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebAssembly/Module.h>

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

GC_DEFINE_ALLOCATOR(ModuleConstructor);

ModuleConstructor::ModuleConstructor(JS::Realm& realm)
    : NativeFunction("Module"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

ModuleConstructor::~ModuleConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> ModuleConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "WebAssembly.Module");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> ModuleConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "ModuleConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface Module:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<ModulePrototype>(*target_realm, "WebAssembly.Module"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "Module");

    auto arg0 = vm.argument(0);

    GC::Root<WebIDL::BufferSource> bytes;

    if (!arg0.is_object() || !(is<JS::TypedArrayBase>(arg0.as_object()) || is<JS::ArrayBuffer>(arg0.as_object()) || is<JS::DataView>(arg0.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BufferSource");

    bytes = GC::make_root(realm.create<WebIDL::BufferSource>(arg0.as_object()));

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return Module::construct_impl(realm, bytes); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface Module" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within Module::construct_impl().
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


void ModuleConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    define_direct_property(vm.names.length, JS::Value(1), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "Module"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<ModulePrototype>(realm, "WebAssembly.Module"_fly_string), 0);


    define_native_function(realm, "exports"_utf16_fly_string, exports, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "imports"_utf16_fly_string, imports, 1, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_native_function(realm, "customSections"_utf16_fly_string, custom_sections, 2, JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable);

}

JS_DEFINE_NATIVE_FUNCTION(ModuleConstructor::exports)
{
    WebIDL::log_trace(vm, "ModuleConstructor::exports");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "exports");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Module>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Module");

    auto& module_object = static_cast<Module&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return Module::exports(vm, module_object); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);
JS::Value wrapped_element0;

    {
        auto dictionary_object1 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_kind1_value;

    wrapped_kind1_value = JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(element0.kind));

        MUST(dictionary_object1->create_data_property("kind"_utf16_fly_string, wrapped_kind1_value));

        JS::Value wrapped_name1_value;

    wrapped_name1_value = JS::PrimitiveString::create(vm, element0.name);

        MUST(dictionary_object1->create_data_property("name"_utf16_fly_string, wrapped_name1_value));

        wrapped_element0 = dictionary_object1;
    }

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(ModuleConstructor::imports)
{
    WebIDL::log_trace(vm, "ModuleConstructor::imports");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "imports");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Module>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Module");

    auto& module_object = static_cast<Module&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return Module::imports(vm, module_object); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);
JS::Value wrapped_element0;

    {
        auto dictionary_object1 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_kind1_value;

    wrapped_kind1_value = JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(element0.kind));

        MUST(dictionary_object1->create_data_property("kind"_utf16_fly_string, wrapped_kind1_value));

        JS::Value wrapped_module1_value;

    wrapped_module1_value = JS::PrimitiveString::create(vm, element0.module);

        MUST(dictionary_object1->create_data_property("module"_utf16_fly_string, wrapped_module1_value));

        JS::Value wrapped_name1_value;

    wrapped_name1_value = JS::PrimitiveString::create(vm, element0.name);

        MUST(dictionary_object1->create_data_property("name"_utf16_fly_string, wrapped_name1_value));

        wrapped_element0 = dictionary_object1;
    }

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(ModuleConstructor::custom_sections)
{
    WebIDL::log_trace(vm, "ModuleConstructor::custom_sections");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "customSections", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Module>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Module");

    auto& module_object = static_cast<Module&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    String section_name;
    if (!false || !arg1.is_null()) {
        section_name = TRY(WebIDL::to_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return Module::custom_sections(vm, module_object, section_name); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);
JS::Value wrapped_element0;

    wrapped_element0 = &const_cast<JS::ArrayBuffer&>(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

} // namespace Web::Bindings
