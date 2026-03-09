
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
#include <LibWeb/Bindings/CustomStateSetPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CustomElements/CustomStateSet.h>

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

GC_DEFINE_ALLOCATOR(CustomStateSetPrototype);

CustomStateSetPrototype::CustomStateSetPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

CustomStateSetPrototype::~CustomStateSetPrototype()
{
}

void CustomStateSetPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    define_native_accessor(realm, vm.names.size, get_size, nullptr, JS::Attribute::Enumerable | JS::Attribute::Configurable);
    define_native_function(realm, vm.names.entries, entries, 0, default_attributes);
    // NOTE: Keys intentionally returns values for setlike
    define_native_function(realm, vm.names.keys, values, 0, default_attributes);
    define_native_function(realm, vm.names.values, values, 0, default_attributes);
    define_direct_property(vm.well_known_symbol_iterator(), get_without_side_effects(vm.names.values), JS::Attribute::Configurable | JS::Attribute::Writable);
    define_native_function(realm, vm.names.forEach, for_each, 1, default_attributes);
    define_native_function(realm, vm.names.has, has, 1, default_attributes);

    define_native_function(realm, vm.names.add, add, 1, default_attributes);

    define_native_function(realm, vm.names.delete_, delete_, 1, default_attributes);

    define_native_function(realm, vm.names.clear, clear, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "CustomStateSet"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void CustomStateSetPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<CustomStateSet*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<CustomStateSet>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CustomStateSet");
    return static_cast<CustomStateSet*>(this_object);
}

// https://webidl.spec.whatwg.org/#js-set-size
JS_DEFINE_NATIVE_FUNCTION(CustomStateSetPrototype::get_size)
{
    WebIDL::log_trace(vm, "CustomStateSetPrototype::size");
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Set> set = impl->set_entries();

    return set->set_size();
}

// https://webidl.spec.whatwg.org/#js-set-entries
JS_DEFINE_NATIVE_FUNCTION(CustomStateSetPrototype::entries)
{
    WebIDL::log_trace(vm, "CustomStateSetPrototype::entries");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Set> set = impl->set_entries();

    return TRY(throw_dom_exception_if_needed(vm, [&] { return JS::SetIterator::create(realm, *set, Object::PropertyKind::KeyAndValue); }));
}

// https://webidl.spec.whatwg.org/#js-set-values
JS_DEFINE_NATIVE_FUNCTION(CustomStateSetPrototype::values)
{
    WebIDL::log_trace(vm, "CustomStateSetPrototype::values");
    auto& realm = *vm.current_realm();
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Set> set = impl->set_entries();

    return TRY(throw_dom_exception_if_needed(vm, [&] { return JS::SetIterator::create(realm, *set, Object::PropertyKind::Value); }));
}

// https://webidl.spec.whatwg.org/#js-set-forEach
JS_DEFINE_NATIVE_FUNCTION(CustomStateSetPrototype::for_each)
{
    WebIDL::log_trace(vm, "CustomStateSetPrototype::for_each");
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Set> set = impl->set_entries();

    auto callback = vm.argument(0);
    if (!callback.is_function())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, callback);

    for (auto& entry : *set) {
        auto value = entry.key;
        TRY(JS::call(vm, callback.as_function(), vm.argument(1), value, value, impl));
    }

    return JS::js_undefined();
}

// https://webidl.spec.whatwg.org/#js-set-has
JS_DEFINE_NATIVE_FUNCTION(CustomStateSetPrototype::has)
{
    WebIDL::log_trace(vm, "CustomStateSetPrototype::has");
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Set> set = impl->set_entries();

    auto value_arg = vm.argument(0);
    
    if (!value_arg.is_string()) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "String");
    }


    // FIXME: If value is -0, set value to +0.
    // What? Which interfaces have a number as their set type?

    return set->set_has(value_arg);
}

// https://webidl.spec.whatwg.org/#js-set-add
JS_DEFINE_NATIVE_FUNCTION(CustomStateSetPrototype::add)
{
    WebIDL::log_trace(vm, "CustomStateSetPrototype::add");
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Set> set = impl->set_entries();

    auto value_arg = vm.argument(0);
    
    if (!value_arg.is_string()) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "String");
    }


    // FIXME: If value is -0, set value to +0.
    // What? Which interfaces have a number as their set type?

    set->set_add(value_arg);
    impl->on_set_modified_from_js({});

    return impl;
}

// https://webidl.spec.whatwg.org/#js-set-delete
JS_DEFINE_NATIVE_FUNCTION(CustomStateSetPrototype::delete_)
{
    WebIDL::log_trace(vm, "CustomStateSetPrototype::delete_");
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Set> set = impl->set_entries();

    auto value_arg = vm.argument(0);
    
    if (!value_arg.is_string()) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "String");
    }


    // FIXME: If value is -0, set value to +0.
    // What? Which interfaces have a number as their set type?

    auto result = set->set_remove(value_arg);
    impl->on_set_modified_from_js({});
    return result;
}

// https://webidl.spec.whatwg.org/#js-set-clear
JS_DEFINE_NATIVE_FUNCTION(CustomStateSetPrototype::clear)
{
    WebIDL::log_trace(vm, "CustomStateSetPrototype::clear");
    auto* impl = TRY(impl_from(vm));

    GC::Ref<JS::Set> set = impl->set_entries();

    set->set_clear();
    impl->on_set_modified_from_js({});

    return JS::js_undefined();
}

} // namespace Web::Bindings
