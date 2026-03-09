
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
#include <LibWeb/Bindings/MessageEventPrototype.h>
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

#if __has_include(<LibWeb/Bindings/EventPrototype.h>)
#    include <LibWeb/Bindings/EventPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/MessageEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/MessagePort.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

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

GC_DEFINE_ALLOCATOR(MessageEventPrototype);

MessageEventPrototype::MessageEventPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

MessageEventPrototype::~MessageEventPrototype()
{
}

void MessageEventPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventPrototype>(realm, "Event"_fly_string));


    auto native_data_getter = JS::NativeFunction::create(realm, data_getter, 0, "data"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_data_setter;

    define_direct_accessor("data"_utf16_fly_string, native_data_getter, native_data_setter, default_attributes);

    auto native_origin_getter = JS::NativeFunction::create(realm, origin_getter, 0, "origin"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_origin_setter;

    define_direct_accessor("origin"_utf16_fly_string, native_origin_getter, native_origin_setter, default_attributes);

    auto native_last_event_id_getter = JS::NativeFunction::create(realm, last_event_id_getter, 0, "lastEventId"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_last_event_id_setter;

    define_direct_accessor("lastEventId"_utf16_fly_string, native_last_event_id_getter, native_last_event_id_setter, default_attributes);

    auto native_source_getter = JS::NativeFunction::create(realm, source_getter, 0, "source"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_source_setter;

    define_direct_accessor("source"_utf16_fly_string, native_source_getter, native_source_setter, default_attributes);

    auto native_ports_getter = JS::NativeFunction::create(realm, ports_getter, 0, "ports"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_ports_setter;

    define_direct_accessor("ports"_utf16_fly_string, native_ports_getter, native_ports_setter, default_attributes);

    define_native_function(realm, "initMessageEvent"_utf16_fly_string, init_message_event, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "MessageEvent"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void MessageEventPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::MessageEvent*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::MessageEvent>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "MessageEvent");
    return static_cast<HTML::MessageEvent*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(MessageEventPrototype::data_getter)
{
    WebIDL::log_trace(vm, "MessageEventPrototype::data_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->data(); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(MessageEventPrototype::origin_getter)
{
    WebIDL::log_trace(vm, "MessageEventPrototype::origin_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->origin(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(MessageEventPrototype::last_event_id_getter)
{
    WebIDL::log_trace(vm, "MessageEventPrototype::last_event_id_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->last_event_id(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(MessageEventPrototype::source_getter)
{
    WebIDL::log_trace(vm, "MessageEventPrototype::source_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->source(); }));

    return retval.visit(

        [&vm, &realm]([[maybe_unused]] GC::Root<WindowProxy> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<WindowProxy&>(*visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<MessagePort> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<MessagePort&>(*visited_union_value0);

        },

        [](Empty) -> JS::Value {
            return JS::js_null();
        }

    );

}

JS_DEFINE_NATIVE_FUNCTION(MessageEventPrototype::ports_getter)
{
    WebIDL::log_trace(vm, "MessageEventPrototype::ports_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ports(); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(MessageEventPrototype::init_message_event)
{
    WebIDL::log_trace(vm, "MessageEventPrototype::init_message_event");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "initMessageEvent");

    auto arg0 = vm.argument(0);

    String type;
    if (!false || !arg0.is_null()) {
        type = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    bool bubbles;

    if (!arg1.is_undefined())

    bubbles = arg1.to_boolean();

    else
        bubbles = static_cast<bool>(false);

    auto arg2 = vm.argument(2);

    bool cancelable;

    if (!arg2.is_undefined())

    cancelable = arg2.to_boolean();

    else
        cancelable = static_cast<bool>(false);

    auto arg3 = vm.argument(3);

    JS::Value data = JS::js_undefined();
    if (!arg3.is_undefined())
        data = arg3;

    else
        data = JS::js_null();

    auto arg4 = vm.argument(4);

    String origin;

    if (!arg4.is_undefined()) {
        if (!false || !arg4.is_null())
            origin = TRY(WebIDL::to_usv_string(vm, arg4));
    } else {
        origin = ""_string;
    }

    auto arg5 = vm.argument(5);

    String last_event_id;

    if (!arg5.is_undefined()) {
        if (!false || !arg5.is_null())
            last_event_id = TRY(WebIDL::to_string(vm, arg5));
    } else {
        last_event_id = ""_string;
    }

    auto arg6 = vm.argument(6);

    auto arg6_to_variant = [&vm, &realm](JS::Value arg6) -> JS::ThrowCompletionOr<Variant<GC::Root<WindowProxy>, GC::Root<MessagePort>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg6.is_object()) {
            [[maybe_unused]] auto& arg6_object = arg6.as_object();

            if (is<PlatformObject>(arg6_object)) {

                if (auto* arg6_result = as_if<WindowProxy>(arg6_object))
                    return GC::make_root(*arg6_result);

                if (auto* arg6_result = as_if<MessagePort>(arg6_object))
                    return GC::make_root(*arg6_result);

            }

            if (auto* arg6_result = as_if<WindowProxy>(arg6_object))
                return GC::make_root(*arg6_result);

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Optional<Variant<GC::Root<WindowProxy>, GC::Root<MessagePort>>> source;
    if (!arg6.is_nullish())
        source = TRY(arg6_to_variant(arg6));

    auto arg7 = vm.argument(7);

    GC::RootVector<GC::Root<MessagePort>> ports { vm.heap() };

    if (!arg7.is_undefined()) {

    if (!arg7.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg7);

    auto arg7_iterator_method0 = TRY(arg7.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg7_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg7);

    auto arg7_iterator1 = TRY(JS::get_iterator_from_method(vm, arg7, *arg7_iterator_method0));

    GC::RootVector<GC::Root<MessagePort>> ports_non_optional { vm.heap() };

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg7_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object() || !is<MessagePort>(next_item1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "MessagePort");

    auto& sequence_item1 = static_cast<MessagePort&>(next_item1.as_object());

    ports_non_optional.append(sequence_item1);
    }

        ports = move(ports_non_optional);
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->init_message_event(type, bubbles, cancelable, data, origin, last_event_id, source, ports); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
