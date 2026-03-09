
#include <LibJS/Runtime/Object.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/Bindings/ShadowRealmExposedInterfaces.h>
#include <LibWeb/HTML/Scripting/Environments.h>
#include <LibWeb/HTML/UniversalGlobalScope.h>
#include <LibWeb/Bindings/CompressionStreamConstructor.h>
#include <LibWeb/Bindings/CompressionStreamPrototype.h>
#include <LibWeb/Bindings/DecompressionStreamConstructor.h>
#include <LibWeb/Bindings/DecompressionStreamPrototype.h>
#include <LibWeb/Bindings/AbortControllerConstructor.h>
#include <LibWeb/Bindings/AbortControllerPrototype.h>
#include <LibWeb/Bindings/AbortSignalConstructor.h>
#include <LibWeb/Bindings/AbortSignalPrototype.h>
#include <LibWeb/Bindings/CustomEventConstructor.h>
#include <LibWeb/Bindings/CustomEventPrototype.h>
#include <LibWeb/Bindings/EventConstructor.h>
#include <LibWeb/Bindings/EventPrototype.h>
#include <LibWeb/Bindings/EventTargetConstructor.h>
#include <LibWeb/Bindings/EventTargetPrototype.h>
#include <LibWeb/Bindings/DOMURLConstructor.h>
#include <LibWeb/Bindings/DOMURLPrototype.h>
#include <LibWeb/Bindings/OriginConstructor.h>
#include <LibWeb/Bindings/OriginPrototype.h>
#include <LibWeb/Bindings/URLSearchParamsConstructor.h>
#include <LibWeb/Bindings/URLSearchParamsPrototype.h>
#include <LibWeb/Bindings/TextDecoderConstructor.h>
#include <LibWeb/Bindings/TextDecoderPrototype.h>
#include <LibWeb/Bindings/TextEncoderConstructor.h>
#include <LibWeb/Bindings/TextEncoderPrototype.h>
#include <LibWeb/Bindings/TextEncoderStreamConstructor.h>
#include <LibWeb/Bindings/TextEncoderStreamPrototype.h>
#include <LibWeb/Bindings/CloseEventConstructor.h>
#include <LibWeb/Bindings/CloseEventPrototype.h>
#include <LibWeb/Bindings/PromiseRejectionEventConstructor.h>
#include <LibWeb/Bindings/PromiseRejectionEventPrototype.h>
#include <LibWeb/Bindings/ShadowRealmGlobalScopeConstructor.h>
#include <LibWeb/Bindings/ShadowRealmGlobalScopePrototype.h>
#include <LibWeb/Bindings/ByteLengthQueuingStrategyConstructor.h>
#include <LibWeb/Bindings/ByteLengthQueuingStrategyPrototype.h>
#include <LibWeb/Bindings/CountQueuingStrategyConstructor.h>
#include <LibWeb/Bindings/CountQueuingStrategyPrototype.h>
#include <LibWeb/Bindings/ReadableByteStreamControllerConstructor.h>
#include <LibWeb/Bindings/ReadableByteStreamControllerPrototype.h>
#include <LibWeb/Bindings/ReadableStreamConstructor.h>
#include <LibWeb/Bindings/ReadableStreamPrototype.h>
#include <LibWeb/Bindings/ReadableStreamBYOBReaderConstructor.h>
#include <LibWeb/Bindings/ReadableStreamBYOBReaderPrototype.h>
#include <LibWeb/Bindings/ReadableStreamBYOBRequestConstructor.h>
#include <LibWeb/Bindings/ReadableStreamBYOBRequestPrototype.h>
#include <LibWeb/Bindings/ReadableStreamDefaultControllerConstructor.h>
#include <LibWeb/Bindings/ReadableStreamDefaultControllerPrototype.h>
#include <LibWeb/Bindings/ReadableStreamDefaultReaderConstructor.h>
#include <LibWeb/Bindings/ReadableStreamDefaultReaderPrototype.h>
#include <LibWeb/Bindings/TransformStreamConstructor.h>
#include <LibWeb/Bindings/TransformStreamPrototype.h>
#include <LibWeb/Bindings/TransformStreamDefaultControllerConstructor.h>
#include <LibWeb/Bindings/TransformStreamDefaultControllerPrototype.h>
#include <LibWeb/Bindings/WritableStreamConstructor.h>
#include <LibWeb/Bindings/WritableStreamPrototype.h>
#include <LibWeb/Bindings/WritableStreamDefaultControllerConstructor.h>
#include <LibWeb/Bindings/WritableStreamDefaultControllerPrototype.h>
#include <LibWeb/Bindings/WritableStreamDefaultWriterConstructor.h>
#include <LibWeb/Bindings/WritableStreamDefaultWriterPrototype.h>
#include <LibWeb/Bindings/GlobalConstructor.h>
#include <LibWeb/Bindings/GlobalPrototype.h>
#include <LibWeb/Bindings/InstanceConstructor.h>
#include <LibWeb/Bindings/InstancePrototype.h>
#include <LibWeb/Bindings/MemoryConstructor.h>
#include <LibWeb/Bindings/MemoryPrototype.h>
#include <LibWeb/Bindings/ModuleConstructor.h>
#include <LibWeb/Bindings/ModulePrototype.h>
#include <LibWeb/Bindings/TableConstructor.h>
#include <LibWeb/Bindings/TablePrototype.h>
#include <LibWeb/Bindings/WebAssemblyNamespace.h>
#include <LibWeb/Bindings/DOMExceptionConstructor.h>
#include <LibWeb/Bindings/DOMExceptionPrototype.h>
#include <LibWeb/Bindings/QuotaExceededErrorConstructor.h>
#include <LibWeb/Bindings/QuotaExceededErrorPrototype.h>

namespace Web::Bindings {

void add_shadow_realm_exposed_interfaces(JS::Object& global)
{
    static constexpr u8 attr = JS::Attribute::Writable | JS::Attribute::Configurable;

    [[maybe_unused]] bool is_secure_context = HTML::is_secure_context(HTML::relevant_principal_settings_object(global));
    [[maybe_unused]] bool expose_experimental_interfaces = HTML::UniversalGlobalScopeMixin::expose_experimental_interfaces();

    global.define_intrinsic_accessor("CompressionStream"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CompressionStreamPrototype>(realm, "CompressionStream"_fly_string); });
    global.define_intrinsic_accessor("DecompressionStream"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DecompressionStreamPrototype>(realm, "DecompressionStream"_fly_string); });
    global.define_intrinsic_accessor("AbortController"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AbortControllerPrototype>(realm, "AbortController"_fly_string); });
    global.define_intrinsic_accessor("AbortSignal"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AbortSignalPrototype>(realm, "AbortSignal"_fly_string); });
    global.define_intrinsic_accessor("CustomEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CustomEventPrototype>(realm, "CustomEvent"_fly_string); });
    global.define_intrinsic_accessor("Event"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<EventPrototype>(realm, "Event"_fly_string); });
    global.define_intrinsic_accessor("EventTarget"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<EventTargetPrototype>(realm, "EventTarget"_fly_string); });
    global.define_intrinsic_accessor("URL"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMURLPrototype>(realm, "URL"_fly_string); });
    global.define_intrinsic_accessor("Origin"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<OriginPrototype>(realm, "Origin"_fly_string); });
    global.define_intrinsic_accessor("URLSearchParams"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<URLSearchParamsPrototype>(realm, "URLSearchParams"_fly_string); });
    global.define_intrinsic_accessor("TextDecoder"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TextDecoderPrototype>(realm, "TextDecoder"_fly_string); });
    global.define_intrinsic_accessor("TextEncoder"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TextEncoderPrototype>(realm, "TextEncoder"_fly_string); });
    global.define_intrinsic_accessor("TextEncoderStream"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TextEncoderStreamPrototype>(realm, "TextEncoderStream"_fly_string); });
    global.define_intrinsic_accessor("CloseEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CloseEventPrototype>(realm, "CloseEvent"_fly_string); });
    global.define_intrinsic_accessor("PromiseRejectionEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PromiseRejectionEventPrototype>(realm, "PromiseRejectionEvent"_fly_string); });
    global.define_intrinsic_accessor("ShadowRealmGlobalScope"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ShadowRealmGlobalScopePrototype>(realm, "ShadowRealmGlobalScope"_fly_string); });
    global.define_intrinsic_accessor("ByteLengthQueuingStrategy"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ByteLengthQueuingStrategyPrototype>(realm, "ByteLengthQueuingStrategy"_fly_string); });
    global.define_intrinsic_accessor("CountQueuingStrategy"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CountQueuingStrategyPrototype>(realm, "CountQueuingStrategy"_fly_string); });
    global.define_intrinsic_accessor("ReadableByteStreamController"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ReadableByteStreamControllerPrototype>(realm, "ReadableByteStreamController"_fly_string); });
    global.define_intrinsic_accessor("ReadableStream"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ReadableStreamPrototype>(realm, "ReadableStream"_fly_string); });
    global.define_intrinsic_accessor("ReadableStreamBYOBReader"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ReadableStreamBYOBReaderPrototype>(realm, "ReadableStreamBYOBReader"_fly_string); });
    global.define_intrinsic_accessor("ReadableStreamBYOBRequest"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ReadableStreamBYOBRequestPrototype>(realm, "ReadableStreamBYOBRequest"_fly_string); });
    global.define_intrinsic_accessor("ReadableStreamDefaultController"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ReadableStreamDefaultControllerPrototype>(realm, "ReadableStreamDefaultController"_fly_string); });
    global.define_intrinsic_accessor("ReadableStreamDefaultReader"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ReadableStreamDefaultReaderPrototype>(realm, "ReadableStreamDefaultReader"_fly_string); });
    global.define_intrinsic_accessor("TransformStream"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TransformStreamPrototype>(realm, "TransformStream"_fly_string); });
    global.define_intrinsic_accessor("TransformStreamDefaultController"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TransformStreamDefaultControllerPrototype>(realm, "TransformStreamDefaultController"_fly_string); });
    global.define_intrinsic_accessor("WritableStream"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WritableStreamPrototype>(realm, "WritableStream"_fly_string); });
    global.define_intrinsic_accessor("WritableStreamDefaultController"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WritableStreamDefaultControllerPrototype>(realm, "WritableStreamDefaultController"_fly_string); });
    global.define_intrinsic_accessor("WritableStreamDefaultWriter"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WritableStreamDefaultWriterPrototype>(realm, "WritableStreamDefaultWriter"_fly_string); });
    global.define_intrinsic_accessor("WebAssembly"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_namespace<WebAssemblyNamespace>(realm, "WebAssembly"_fly_string); });
    global.define_intrinsic_accessor("DOMException"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMExceptionPrototype>(realm, "DOMException"_fly_string); });
    global.define_intrinsic_accessor("QuotaExceededError"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<QuotaExceededErrorPrototype>(realm, "QuotaExceededError"_fly_string); });
}

}
