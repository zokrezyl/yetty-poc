
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
#include <LibWeb/Bindings/SVGTransformListPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGTransformList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGTransform.h>

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

GC_DEFINE_ALLOCATOR(SVGTransformListPrototype);

SVGTransformListPrototype::SVGTransformListPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

SVGTransformListPrototype::~SVGTransformListPrototype()
{
}

void SVGTransformListPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_length_getter = JS::NativeFunction::create(realm, length_getter, 0, "length"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_length_setter;

    define_direct_accessor("length"_utf16_fly_string, native_length_getter, native_length_setter, default_attributes);

    auto native_number_of_items_getter = JS::NativeFunction::create(realm, number_of_items_getter, 0, "numberOfItems"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_number_of_items_setter;

    define_direct_accessor("numberOfItems"_utf16_fly_string, native_number_of_items_getter, native_number_of_items_setter, default_attributes);

        define_direct_property("createSVGTransformFromMatrix"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("consolidate"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_native_function(realm, "clear"_utf16_fly_string, clear, 0, default_attributes);

    define_native_function(realm, "initialize"_utf16_fly_string, initialize_, 1, default_attributes);

    define_native_function(realm, "getItem"_utf16_fly_string, get_item, 1, default_attributes);

    define_native_function(realm, "insertItemBefore"_utf16_fly_string, insert_item_before, 2, default_attributes);

    define_native_function(realm, "replaceItem"_utf16_fly_string, replace_item, 2, default_attributes);

    define_native_function(realm, "removeItem"_utf16_fly_string, remove_item, 1, default_attributes);

    define_native_function(realm, "appendItem"_utf16_fly_string, append_item, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_iterator(), realm.intrinsics().array_prototype()->get_without_side_effects(vm.names.values), JS::Attribute::Configurable | JS::Attribute::Writable);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "SVGTransformList"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void SVGTransformListPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<SVG::SVGTransformList*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<SVG::SVGTransformList>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SVGTransformList");
    return static_cast<SVG::SVGTransformList*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(SVGTransformListPrototype::length_getter)
{
    WebIDL::log_trace(vm, "SVGTransformListPrototype::length_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->length(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(SVGTransformListPrototype::number_of_items_getter)
{
    WebIDL::log_trace(vm, "SVGTransformListPrototype::number_of_items_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->number_of_items(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(SVGTransformListPrototype::clear)
{
    WebIDL::log_trace(vm, "SVGTransformListPrototype::clear");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(SVGTransformListPrototype::initialize_)
{
    WebIDL::log_trace(vm, "SVGTransformListPrototype::initialize_");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "initialize");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<SVGTransform>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SVGTransform");

    auto& new_item = static_cast<SVGTransform&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->initialize_(new_item); }));

    return &const_cast<SVGTransform&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGTransformListPrototype::get_item)
{
    WebIDL::log_trace(vm, "SVGTransformListPrototype::get_item");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getItem");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_item(index); }));

    return &const_cast<SVGTransform&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGTransformListPrototype::insert_item_before)
{
    WebIDL::log_trace(vm, "SVGTransformListPrototype::insert_item_before");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "insertItemBefore", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<SVGTransform>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SVGTransform");

    auto& new_item = static_cast<SVGTransform&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->insert_item_before(new_item, index); }));

    return &const_cast<SVGTransform&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGTransformListPrototype::replace_item)
{
    WebIDL::log_trace(vm, "SVGTransformListPrototype::replace_item");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "replaceItem", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<SVGTransform>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SVGTransform");

    auto& new_item = static_cast<SVGTransform&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->replace_item(new_item, index); }));

    return &const_cast<SVGTransform&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGTransformListPrototype::remove_item)
{
    WebIDL::log_trace(vm, "SVGTransformListPrototype::remove_item");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "removeItem");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->remove_item(index); }));

    return &const_cast<SVGTransform&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(SVGTransformListPrototype::append_item)
{
    WebIDL::log_trace(vm, "SVGTransformListPrototype::append_item");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "appendItem");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<SVGTransform>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "SVGTransform");

    auto& new_item = static_cast<SVGTransform&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->append_item(new_item); }));

    return &const_cast<SVGTransform&>(*retval);

}

} // namespace Web::Bindings
