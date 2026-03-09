
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
#include <LibWeb/Bindings/NotificationPrototype.h>
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

#if __has_include(<LibWeb/Bindings/EventTargetPrototype.h>)
#    include <LibWeb/Bindings/EventTargetPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/NotificationsAPI/Notification.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

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

GC_DEFINE_ALLOCATOR(NotificationPrototype);

NotificationPrototype::NotificationPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

NotificationPrototype::~NotificationPrototype()
{
}

void NotificationPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    define_direct_property("onclick"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_direct_property("onshow"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_direct_property("onerror"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_direct_property("onclose"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    auto native_title_getter = JS::NativeFunction::create(realm, title_getter, 0, "title"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_title_setter;

    define_direct_accessor("title"_utf16_fly_string, native_title_getter, native_title_setter, default_attributes);

    auto native_dir_getter = JS::NativeFunction::create(realm, dir_getter, 0, "dir"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_dir_setter;

    define_direct_accessor("dir"_utf16_fly_string, native_dir_getter, native_dir_setter, default_attributes);

    auto native_lang_getter = JS::NativeFunction::create(realm, lang_getter, 0, "lang"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_lang_setter;

    define_direct_accessor("lang"_utf16_fly_string, native_lang_getter, native_lang_setter, default_attributes);

    auto native_body_getter = JS::NativeFunction::create(realm, body_getter, 0, "body"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_body_setter;

    define_direct_accessor("body"_utf16_fly_string, native_body_getter, native_body_setter, default_attributes);

    auto native_navigate_getter = JS::NativeFunction::create(realm, navigate_getter, 0, "navigate"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_navigate_setter;

    define_direct_accessor("navigate"_utf16_fly_string, native_navigate_getter, native_navigate_setter, default_attributes);

    auto native_tag_getter = JS::NativeFunction::create(realm, tag_getter, 0, "tag"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_tag_setter;

    define_direct_accessor("tag"_utf16_fly_string, native_tag_getter, native_tag_setter, default_attributes);

    auto native_image_getter = JS::NativeFunction::create(realm, image_getter, 0, "image"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_image_setter;

    define_direct_accessor("image"_utf16_fly_string, native_image_getter, native_image_setter, default_attributes);

    auto native_icon_getter = JS::NativeFunction::create(realm, icon_getter, 0, "icon"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_icon_setter;

    define_direct_accessor("icon"_utf16_fly_string, native_icon_getter, native_icon_setter, default_attributes);

    auto native_badge_getter = JS::NativeFunction::create(realm, badge_getter, 0, "badge"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_badge_setter;

    define_direct_accessor("badge"_utf16_fly_string, native_badge_getter, native_badge_setter, default_attributes);

    auto native_timestamp_getter = JS::NativeFunction::create(realm, timestamp_getter, 0, "timestamp"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_timestamp_setter;

    define_direct_accessor("timestamp"_utf16_fly_string, native_timestamp_getter, native_timestamp_setter, default_attributes);

    auto native_renotify_getter = JS::NativeFunction::create(realm, renotify_getter, 0, "renotify"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_renotify_setter;

    define_direct_accessor("renotify"_utf16_fly_string, native_renotify_getter, native_renotify_setter, default_attributes);

    auto native_silent_getter = JS::NativeFunction::create(realm, silent_getter, 0, "silent"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_silent_setter;

    define_direct_accessor("silent"_utf16_fly_string, native_silent_getter, native_silent_setter, default_attributes);

    auto native_require_interaction_getter = JS::NativeFunction::create(realm, require_interaction_getter, 0, "requireInteraction"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_require_interaction_setter;

    define_direct_accessor("requireInteraction"_utf16_fly_string, native_require_interaction_getter, native_require_interaction_setter, default_attributes);

    auto native_data_getter = JS::NativeFunction::create(realm, data_getter, 0, "data"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_data_setter;

    define_direct_accessor("data"_utf16_fly_string, native_data_getter, native_data_setter, default_attributes);

    auto native_actions_getter = JS::NativeFunction::create(realm, actions_getter, 0, "actions"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_actions_setter;

    define_direct_accessor("actions"_utf16_fly_string, native_actions_getter, native_actions_setter, default_attributes);

        define_direct_property("close"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Notification"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void NotificationPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Notification*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Notification>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Notification");
    return static_cast<Notification*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(NotificationPrototype::title_getter)
{
    WebIDL::log_trace(vm, "NotificationPrototype::title_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->title(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NotificationPrototype::dir_getter)
{
    WebIDL::log_trace(vm, "NotificationPrototype::dir_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dir(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(NotificationPrototype::lang_getter)
{
    WebIDL::log_trace(vm, "NotificationPrototype::lang_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->lang(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NotificationPrototype::body_getter)
{
    WebIDL::log_trace(vm, "NotificationPrototype::body_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->body(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NotificationPrototype::navigate_getter)
{
    WebIDL::log_trace(vm, "NotificationPrototype::navigate_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->navigate(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NotificationPrototype::tag_getter)
{
    WebIDL::log_trace(vm, "NotificationPrototype::tag_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tag(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NotificationPrototype::image_getter)
{
    WebIDL::log_trace(vm, "NotificationPrototype::image_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->image(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NotificationPrototype::icon_getter)
{
    WebIDL::log_trace(vm, "NotificationPrototype::icon_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->icon(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NotificationPrototype::badge_getter)
{
    WebIDL::log_trace(vm, "NotificationPrototype::badge_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->badge(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NotificationPrototype::timestamp_getter)
{
    WebIDL::log_trace(vm, "NotificationPrototype::timestamp_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->timestamp(); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(NotificationPrototype::renotify_getter)
{
    WebIDL::log_trace(vm, "NotificationPrototype::renotify_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->renotify(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NotificationPrototype::silent_getter)
{
    WebIDL::log_trace(vm, "NotificationPrototype::silent_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->silent(); }));

    if (retval.has_value()) {

    return JS::Value(retval.release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(NotificationPrototype::require_interaction_getter)
{
    WebIDL::log_trace(vm, "NotificationPrototype::require_interaction_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->require_interaction(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NotificationPrototype::data_getter)
{
    WebIDL::log_trace(vm, "NotificationPrototype::data_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->data(); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(NotificationPrototype::actions_getter)
{
    WebIDL::log_trace(vm, "NotificationPrototype::actions_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->actions(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);
JS::Value wrapped_element0;

    {
        auto dictionary_object1 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_action1_value;

    wrapped_action1_value = JS::PrimitiveString::create(vm, element0.action);

        MUST(dictionary_object1->create_data_property("action"_utf16_fly_string, wrapped_action1_value));

        Optional<JS::Value> wrapped_icon1_value;

    if (element0.icon.has_value()) {

    wrapped_icon1_value = JS::PrimitiveString::create(vm, const_cast<decltype(element0.icon)&>(element0.icon).release_value());

    }

        if (wrapped_icon1_value.has_value())
            MUST(dictionary_object1->create_data_property("icon"_utf16_fly_string, wrapped_icon1_value.release_value()));

        Optional<JS::Value> wrapped_navigate1_value;

    if (element0.navigate.has_value()) {

    wrapped_navigate1_value = JS::PrimitiveString::create(vm, const_cast<decltype(element0.navigate)&>(element0.navigate).release_value());

    }

        if (wrapped_navigate1_value.has_value())
            MUST(dictionary_object1->create_data_property("navigate"_utf16_fly_string, wrapped_navigate1_value.release_value()));

        JS::Value wrapped_title1_value;

    wrapped_title1_value = JS::PrimitiveString::create(vm, element0.title);

        MUST(dictionary_object1->create_data_property("title"_utf16_fly_string, wrapped_title1_value));

        wrapped_element0 = dictionary_object1;
    }

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    TRY(new_array0_0->set_integrity_level(IntegrityLevel::Frozen));

    return new_array0_0;

}

} // namespace Web::Bindings
