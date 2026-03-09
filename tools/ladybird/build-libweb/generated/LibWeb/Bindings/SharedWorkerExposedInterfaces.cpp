
#include <LibJS/Runtime/Object.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/Bindings/SharedWorkerExposedInterfaces.h>
#include <LibWeb/HTML/Scripting/Environments.h>
#include <LibWeb/HTML/UniversalGlobalScope.h>
#include <LibWeb/Bindings/SecurityPolicyViolationEventConstructor.h>
#include <LibWeb/Bindings/SecurityPolicyViolationEventPrototype.h>
#include <LibWeb/Bindings/CompressionStreamConstructor.h>
#include <LibWeb/Bindings/CompressionStreamPrototype.h>
#include <LibWeb/Bindings/DecompressionStreamConstructor.h>
#include <LibWeb/Bindings/DecompressionStreamPrototype.h>
#include <LibWeb/Bindings/CryptoConstructor.h>
#include <LibWeb/Bindings/CryptoPrototype.h>
#include <LibWeb/Bindings/CryptoKeyConstructor.h>
#include <LibWeb/Bindings/CryptoKeyPrototype.h>
#include <LibWeb/Bindings/SubtleCryptoConstructor.h>
#include <LibWeb/Bindings/SubtleCryptoPrototype.h>
#include <LibWeb/Bindings/CSSImageValueConstructor.h>
#include <LibWeb/Bindings/CSSImageValuePrototype.h>
#include <LibWeb/Bindings/CSSKeywordValueConstructor.h>
#include <LibWeb/Bindings/CSSKeywordValuePrototype.h>
#include <LibWeb/Bindings/CSSMathClampConstructor.h>
#include <LibWeb/Bindings/CSSMathClampPrototype.h>
#include <LibWeb/Bindings/CSSMathInvertConstructor.h>
#include <LibWeb/Bindings/CSSMathInvertPrototype.h>
#include <LibWeb/Bindings/CSSMathMaxConstructor.h>
#include <LibWeb/Bindings/CSSMathMaxPrototype.h>
#include <LibWeb/Bindings/CSSMathMinConstructor.h>
#include <LibWeb/Bindings/CSSMathMinPrototype.h>
#include <LibWeb/Bindings/CSSMathNegateConstructor.h>
#include <LibWeb/Bindings/CSSMathNegatePrototype.h>
#include <LibWeb/Bindings/CSSMathProductConstructor.h>
#include <LibWeb/Bindings/CSSMathProductPrototype.h>
#include <LibWeb/Bindings/CSSMathSumConstructor.h>
#include <LibWeb/Bindings/CSSMathSumPrototype.h>
#include <LibWeb/Bindings/CSSMathValueConstructor.h>
#include <LibWeb/Bindings/CSSMathValuePrototype.h>
#include <LibWeb/Bindings/CSSMatrixComponentConstructor.h>
#include <LibWeb/Bindings/CSSMatrixComponentPrototype.h>
#include <LibWeb/Bindings/CSSNumericArrayConstructor.h>
#include <LibWeb/Bindings/CSSNumericArrayPrototype.h>
#include <LibWeb/Bindings/CSSNumericValueConstructor.h>
#include <LibWeb/Bindings/CSSNumericValuePrototype.h>
#include <LibWeb/Bindings/CSSPerspectiveConstructor.h>
#include <LibWeb/Bindings/CSSPerspectivePrototype.h>
#include <LibWeb/Bindings/CSSRotateConstructor.h>
#include <LibWeb/Bindings/CSSRotatePrototype.h>
#include <LibWeb/Bindings/CSSScaleConstructor.h>
#include <LibWeb/Bindings/CSSScalePrototype.h>
#include <LibWeb/Bindings/CSSSkewConstructor.h>
#include <LibWeb/Bindings/CSSSkewPrototype.h>
#include <LibWeb/Bindings/CSSSkewXConstructor.h>
#include <LibWeb/Bindings/CSSSkewXPrototype.h>
#include <LibWeb/Bindings/CSSSkewYConstructor.h>
#include <LibWeb/Bindings/CSSSkewYPrototype.h>
#include <LibWeb/Bindings/CSSStyleValueConstructor.h>
#include <LibWeb/Bindings/CSSStyleValuePrototype.h>
#include <LibWeb/Bindings/CSSTransformComponentConstructor.h>
#include <LibWeb/Bindings/CSSTransformComponentPrototype.h>
#include <LibWeb/Bindings/CSSTransformValueConstructor.h>
#include <LibWeb/Bindings/CSSTransformValuePrototype.h>
#include <LibWeb/Bindings/CSSTranslateConstructor.h>
#include <LibWeb/Bindings/CSSTranslatePrototype.h>
#include <LibWeb/Bindings/CSSUnitValueConstructor.h>
#include <LibWeb/Bindings/CSSUnitValuePrototype.h>
#include <LibWeb/Bindings/CSSUnparsedValueConstructor.h>
#include <LibWeb/Bindings/CSSUnparsedValuePrototype.h>
#include <LibWeb/Bindings/CSSVariableReferenceValueConstructor.h>
#include <LibWeb/Bindings/CSSVariableReferenceValuePrototype.h>
#include <LibWeb/Bindings/FontFaceConstructor.h>
#include <LibWeb/Bindings/FontFacePrototype.h>
#include <LibWeb/Bindings/FontFaceSetConstructor.h>
#include <LibWeb/Bindings/FontFaceSetPrototype.h>
#include <LibWeb/Bindings/FontFaceSetLoadEventConstructor.h>
#include <LibWeb/Bindings/FontFaceSetLoadEventPrototype.h>
#include <LibWeb/Bindings/StylePropertyMapReadOnlyConstructor.h>
#include <LibWeb/Bindings/StylePropertyMapReadOnlyPrototype.h>
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
#include <LibWeb/Bindings/HeadersConstructor.h>
#include <LibWeb/Bindings/HeadersPrototype.h>
#include <LibWeb/Bindings/RequestConstructor.h>
#include <LibWeb/Bindings/RequestPrototype.h>
#include <LibWeb/Bindings/ResponseConstructor.h>
#include <LibWeb/Bindings/ResponsePrototype.h>
#include <LibWeb/Bindings/BlobConstructor.h>
#include <LibWeb/Bindings/BlobPrototype.h>
#include <LibWeb/Bindings/FileConstructor.h>
#include <LibWeb/Bindings/FilePrototype.h>
#include <LibWeb/Bindings/FileListConstructor.h>
#include <LibWeb/Bindings/FileListPrototype.h>
#include <LibWeb/Bindings/FileReaderConstructor.h>
#include <LibWeb/Bindings/FileReaderPrototype.h>
#include <LibWeb/Bindings/FileReaderSyncConstructor.h>
#include <LibWeb/Bindings/FileReaderSyncPrototype.h>
#include <LibWeb/Bindings/DOMMatrixConstructor.h>
#include <LibWeb/Bindings/DOMMatrixPrototype.h>
#include <LibWeb/Bindings/DOMMatrixReadOnlyConstructor.h>
#include <LibWeb/Bindings/DOMMatrixReadOnlyPrototype.h>
#include <LibWeb/Bindings/DOMPointConstructor.h>
#include <LibWeb/Bindings/DOMPointPrototype.h>
#include <LibWeb/Bindings/DOMPointReadOnlyConstructor.h>
#include <LibWeb/Bindings/DOMPointReadOnlyPrototype.h>
#include <LibWeb/Bindings/DOMQuadConstructor.h>
#include <LibWeb/Bindings/DOMQuadPrototype.h>
#include <LibWeb/Bindings/DOMRectConstructor.h>
#include <LibWeb/Bindings/DOMRectPrototype.h>
#include <LibWeb/Bindings/DOMRectReadOnlyConstructor.h>
#include <LibWeb/Bindings/DOMRectReadOnlyPrototype.h>
#include <LibWeb/Bindings/BroadcastChannelConstructor.h>
#include <LibWeb/Bindings/BroadcastChannelPrototype.h>
#include <LibWeb/Bindings/CanvasGradientConstructor.h>
#include <LibWeb/Bindings/CanvasGradientPrototype.h>
#include <LibWeb/Bindings/CanvasPatternConstructor.h>
#include <LibWeb/Bindings/CanvasPatternPrototype.h>
#include <LibWeb/Bindings/CloseEventConstructor.h>
#include <LibWeb/Bindings/CloseEventPrototype.h>
#include <LibWeb/Bindings/DOMStringListConstructor.h>
#include <LibWeb/Bindings/DOMStringListPrototype.h>
#include <LibWeb/Bindings/ErrorEventConstructor.h>
#include <LibWeb/Bindings/ErrorEventPrototype.h>
#include <LibWeb/Bindings/EventSourceConstructor.h>
#include <LibWeb/Bindings/EventSourcePrototype.h>
#include <LibWeb/Bindings/ImageBitmapConstructor.h>
#include <LibWeb/Bindings/ImageBitmapPrototype.h>
#include <LibWeb/Bindings/ImageDataConstructor.h>
#include <LibWeb/Bindings/ImageDataPrototype.h>
#include <LibWeb/Bindings/MessageChannelConstructor.h>
#include <LibWeb/Bindings/MessageChannelPrototype.h>
#include <LibWeb/Bindings/MessageEventConstructor.h>
#include <LibWeb/Bindings/MessageEventPrototype.h>
#include <LibWeb/Bindings/MessagePortConstructor.h>
#include <LibWeb/Bindings/MessagePortPrototype.h>
#include <LibWeb/Bindings/OffscreenCanvasConstructor.h>
#include <LibWeb/Bindings/OffscreenCanvasPrototype.h>
#include <LibWeb/Bindings/OffscreenCanvasRenderingContext2DConstructor.h>
#include <LibWeb/Bindings/OffscreenCanvasRenderingContext2DPrototype.h>
#include <LibWeb/Bindings/Path2DConstructor.h>
#include <LibWeb/Bindings/Path2DPrototype.h>
#include <LibWeb/Bindings/PromiseRejectionEventConstructor.h>
#include <LibWeb/Bindings/PromiseRejectionEventPrototype.h>
#include <LibWeb/Bindings/SharedWorkerGlobalScopeConstructor.h>
#include <LibWeb/Bindings/SharedWorkerGlobalScopePrototype.h>
#include <LibWeb/Bindings/TextMetricsConstructor.h>
#include <LibWeb/Bindings/TextMetricsPrototype.h>
#include <LibWeb/Bindings/WorkerConstructor.h>
#include <LibWeb/Bindings/WorkerPrototype.h>
#include <LibWeb/Bindings/WorkerGlobalScopeConstructor.h>
#include <LibWeb/Bindings/WorkerGlobalScopePrototype.h>
#include <LibWeb/Bindings/WorkerLocationConstructor.h>
#include <LibWeb/Bindings/WorkerLocationPrototype.h>
#include <LibWeb/Bindings/WorkerNavigatorConstructor.h>
#include <LibWeb/Bindings/WorkerNavigatorPrototype.h>
#include <LibWeb/Bindings/PerformanceConstructor.h>
#include <LibWeb/Bindings/PerformancePrototype.h>
#include <LibWeb/Bindings/IDBCursorConstructor.h>
#include <LibWeb/Bindings/IDBCursorPrototype.h>
#include <LibWeb/Bindings/IDBCursorWithValueConstructor.h>
#include <LibWeb/Bindings/IDBCursorWithValuePrototype.h>
#include <LibWeb/Bindings/IDBDatabaseConstructor.h>
#include <LibWeb/Bindings/IDBDatabasePrototype.h>
#include <LibWeb/Bindings/IDBFactoryConstructor.h>
#include <LibWeb/Bindings/IDBFactoryPrototype.h>
#include <LibWeb/Bindings/IDBIndexConstructor.h>
#include <LibWeb/Bindings/IDBIndexPrototype.h>
#include <LibWeb/Bindings/IDBKeyRangeConstructor.h>
#include <LibWeb/Bindings/IDBKeyRangePrototype.h>
#include <LibWeb/Bindings/IDBObjectStoreConstructor.h>
#include <LibWeb/Bindings/IDBObjectStorePrototype.h>
#include <LibWeb/Bindings/IDBOpenDBRequestConstructor.h>
#include <LibWeb/Bindings/IDBOpenDBRequestPrototype.h>
#include <LibWeb/Bindings/IDBRecordConstructor.h>
#include <LibWeb/Bindings/IDBRecordPrototype.h>
#include <LibWeb/Bindings/IDBRequestConstructor.h>
#include <LibWeb/Bindings/IDBRequestPrototype.h>
#include <LibWeb/Bindings/IDBTransactionConstructor.h>
#include <LibWeb/Bindings/IDBTransactionPrototype.h>
#include <LibWeb/Bindings/IDBVersionChangeEventConstructor.h>
#include <LibWeb/Bindings/IDBVersionChangeEventPrototype.h>
#include <LibWeb/Bindings/MediaCapabilitiesConstructor.h>
#include <LibWeb/Bindings/MediaCapabilitiesPrototype.h>
#include <LibWeb/Bindings/NotificationConstructor.h>
#include <LibWeb/Bindings/NotificationPrototype.h>
#include <LibWeb/Bindings/PerformanceEntryConstructor.h>
#include <LibWeb/Bindings/PerformanceEntryPrototype.h>
#include <LibWeb/Bindings/PerformanceObserverConstructor.h>
#include <LibWeb/Bindings/PerformanceObserverPrototype.h>
#include <LibWeb/Bindings/PerformanceObserverEntryListConstructor.h>
#include <LibWeb/Bindings/PerformanceObserverEntryListPrototype.h>
#include <LibWeb/Bindings/PerformanceResourceTimingConstructor.h>
#include <LibWeb/Bindings/PerformanceResourceTimingPrototype.h>
#include <LibWeb/Bindings/CacheStorageConstructor.h>
#include <LibWeb/Bindings/CacheStoragePrototype.h>
#include <LibWeb/Bindings/ServiceWorkerConstructor.h>
#include <LibWeb/Bindings/ServiceWorkerPrototype.h>
#include <LibWeb/Bindings/ServiceWorkerContainerConstructor.h>
#include <LibWeb/Bindings/ServiceWorkerContainerPrototype.h>
#include <LibWeb/Bindings/ServiceWorkerRegistrationConstructor.h>
#include <LibWeb/Bindings/ServiceWorkerRegistrationPrototype.h>
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
#include <LibWeb/Bindings/TrustedHTMLConstructor.h>
#include <LibWeb/Bindings/TrustedHTMLPrototype.h>
#include <LibWeb/Bindings/TrustedScriptConstructor.h>
#include <LibWeb/Bindings/TrustedScriptPrototype.h>
#include <LibWeb/Bindings/TrustedScriptURLConstructor.h>
#include <LibWeb/Bindings/TrustedScriptURLPrototype.h>
#include <LibWeb/Bindings/TrustedTypePolicyConstructor.h>
#include <LibWeb/Bindings/TrustedTypePolicyPrototype.h>
#include <LibWeb/Bindings/TrustedTypePolicyFactoryConstructor.h>
#include <LibWeb/Bindings/TrustedTypePolicyFactoryPrototype.h>
#include <LibWeb/Bindings/StorageManagerConstructor.h>
#include <LibWeb/Bindings/StorageManagerPrototype.h>
#include <LibWeb/Bindings/URLPatternConstructor.h>
#include <LibWeb/Bindings/URLPatternPrototype.h>
#include <LibWeb/Bindings/PerformanceMarkConstructor.h>
#include <LibWeb/Bindings/PerformanceMarkPrototype.h>
#include <LibWeb/Bindings/PerformanceMeasureConstructor.h>
#include <LibWeb/Bindings/PerformanceMeasurePrototype.h>
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
#include <LibWeb/Bindings/ANGLEInstancedArraysConstructor.h>
#include <LibWeb/Bindings/ANGLEInstancedArraysPrototype.h>
#include <LibWeb/Bindings/EXTBlendMinMaxConstructor.h>
#include <LibWeb/Bindings/EXTBlendMinMaxPrototype.h>
#include <LibWeb/Bindings/EXTColorBufferFloatConstructor.h>
#include <LibWeb/Bindings/EXTColorBufferFloatPrototype.h>
#include <LibWeb/Bindings/EXTRenderSnormConstructor.h>
#include <LibWeb/Bindings/EXTRenderSnormPrototype.h>
#include <LibWeb/Bindings/EXTTextureFilterAnisotropicConstructor.h>
#include <LibWeb/Bindings/EXTTextureFilterAnisotropicPrototype.h>
#include <LibWeb/Bindings/EXTTextureNorm16Constructor.h>
#include <LibWeb/Bindings/EXTTextureNorm16Prototype.h>
#include <LibWeb/Bindings/OESElementIndexUintConstructor.h>
#include <LibWeb/Bindings/OESElementIndexUintPrototype.h>
#include <LibWeb/Bindings/OESStandardDerivativesConstructor.h>
#include <LibWeb/Bindings/OESStandardDerivativesPrototype.h>
#include <LibWeb/Bindings/OESVertexArrayObjectConstructor.h>
#include <LibWeb/Bindings/OESVertexArrayObjectPrototype.h>
#include <LibWeb/Bindings/WebGLCompressedTextureS3tcConstructor.h>
#include <LibWeb/Bindings/WebGLCompressedTextureS3tcPrototype.h>
#include <LibWeb/Bindings/WebGLCompressedTextureS3tcSrgbConstructor.h>
#include <LibWeb/Bindings/WebGLCompressedTextureS3tcSrgbPrototype.h>
#include <LibWeb/Bindings/WebGLDrawBuffersConstructor.h>
#include <LibWeb/Bindings/WebGLDrawBuffersPrototype.h>
#include <LibWeb/Bindings/WebGLVertexArrayObjectOESConstructor.h>
#include <LibWeb/Bindings/WebGLVertexArrayObjectOESPrototype.h>
#include <LibWeb/Bindings/WebGL2RenderingContextConstructor.h>
#include <LibWeb/Bindings/WebGL2RenderingContextPrototype.h>
#include <LibWeb/Bindings/WebGLActiveInfoConstructor.h>
#include <LibWeb/Bindings/WebGLActiveInfoPrototype.h>
#include <LibWeb/Bindings/WebGLBufferConstructor.h>
#include <LibWeb/Bindings/WebGLBufferPrototype.h>
#include <LibWeb/Bindings/WebGLContextEventConstructor.h>
#include <LibWeb/Bindings/WebGLContextEventPrototype.h>
#include <LibWeb/Bindings/WebGLFramebufferConstructor.h>
#include <LibWeb/Bindings/WebGLFramebufferPrototype.h>
#include <LibWeb/Bindings/WebGLObjectConstructor.h>
#include <LibWeb/Bindings/WebGLObjectPrototype.h>
#include <LibWeb/Bindings/WebGLProgramConstructor.h>
#include <LibWeb/Bindings/WebGLProgramPrototype.h>
#include <LibWeb/Bindings/WebGLQueryConstructor.h>
#include <LibWeb/Bindings/WebGLQueryPrototype.h>
#include <LibWeb/Bindings/WebGLRenderbufferConstructor.h>
#include <LibWeb/Bindings/WebGLRenderbufferPrototype.h>
#include <LibWeb/Bindings/WebGLRenderingContextConstructor.h>
#include <LibWeb/Bindings/WebGLRenderingContextPrototype.h>
#include <LibWeb/Bindings/WebGLSamplerConstructor.h>
#include <LibWeb/Bindings/WebGLSamplerPrototype.h>
#include <LibWeb/Bindings/WebGLShaderConstructor.h>
#include <LibWeb/Bindings/WebGLShaderPrototype.h>
#include <LibWeb/Bindings/WebGLShaderPrecisionFormatConstructor.h>
#include <LibWeb/Bindings/WebGLShaderPrecisionFormatPrototype.h>
#include <LibWeb/Bindings/WebGLSyncConstructor.h>
#include <LibWeb/Bindings/WebGLSyncPrototype.h>
#include <LibWeb/Bindings/WebGLTextureConstructor.h>
#include <LibWeb/Bindings/WebGLTexturePrototype.h>
#include <LibWeb/Bindings/WebGLTransformFeedbackConstructor.h>
#include <LibWeb/Bindings/WebGLTransformFeedbackPrototype.h>
#include <LibWeb/Bindings/WebGLUniformLocationConstructor.h>
#include <LibWeb/Bindings/WebGLUniformLocationPrototype.h>
#include <LibWeb/Bindings/WebGLVertexArrayObjectConstructor.h>
#include <LibWeb/Bindings/WebGLVertexArrayObjectPrototype.h>
#include <LibWeb/Bindings/DOMExceptionConstructor.h>
#include <LibWeb/Bindings/DOMExceptionPrototype.h>
#include <LibWeb/Bindings/QuotaExceededErrorConstructor.h>
#include <LibWeb/Bindings/QuotaExceededErrorPrototype.h>
#include <LibWeb/Bindings/WebSocketConstructor.h>
#include <LibWeb/Bindings/WebSocketPrototype.h>
#include <LibWeb/Bindings/FormDataConstructor.h>
#include <LibWeb/Bindings/FormDataPrototype.h>
#include <LibWeb/Bindings/ProgressEventConstructor.h>
#include <LibWeb/Bindings/ProgressEventPrototype.h>
#include <LibWeb/Bindings/XMLHttpRequestConstructor.h>
#include <LibWeb/Bindings/XMLHttpRequestPrototype.h>
#include <LibWeb/Bindings/XMLHttpRequestEventTargetConstructor.h>
#include <LibWeb/Bindings/XMLHttpRequestEventTargetPrototype.h>
#include <LibWeb/Bindings/XMLHttpRequestUploadConstructor.h>
#include <LibWeb/Bindings/XMLHttpRequestUploadPrototype.h>

namespace Web::Bindings {

void add_shared_worker_exposed_interfaces(JS::Object& global)
{
    static constexpr u8 attr = JS::Attribute::Writable | JS::Attribute::Configurable;

    [[maybe_unused]] bool is_secure_context = HTML::is_secure_context(HTML::relevant_principal_settings_object(global));
    [[maybe_unused]] bool expose_experimental_interfaces = HTML::UniversalGlobalScopeMixin::expose_experimental_interfaces();

    global.define_intrinsic_accessor("SecurityPolicyViolationEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SecurityPolicyViolationEventPrototype>(realm, "SecurityPolicyViolationEvent"_fly_string); });
    global.define_intrinsic_accessor("CompressionStream"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CompressionStreamPrototype>(realm, "CompressionStream"_fly_string); });
    global.define_intrinsic_accessor("DecompressionStream"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DecompressionStreamPrototype>(realm, "DecompressionStream"_fly_string); });
    global.define_intrinsic_accessor("Crypto"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CryptoPrototype>(realm, "Crypto"_fly_string); });
    if (is_secure_context) {
    global.define_intrinsic_accessor("CryptoKey"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CryptoKeyPrototype>(realm, "CryptoKey"_fly_string); });
    }
    if (is_secure_context) {
    global.define_intrinsic_accessor("SubtleCrypto"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SubtleCryptoPrototype>(realm, "SubtleCrypto"_fly_string); });
    }
    global.define_intrinsic_accessor("CSSImageValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSImageValuePrototype>(realm, "CSSImageValue"_fly_string); });
    global.define_intrinsic_accessor("CSSKeywordValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSKeywordValuePrototype>(realm, "CSSKeywordValue"_fly_string); });
    global.define_intrinsic_accessor("CSSMathClamp"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMathClampPrototype>(realm, "CSSMathClamp"_fly_string); });
    global.define_intrinsic_accessor("CSSMathInvert"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMathInvertPrototype>(realm, "CSSMathInvert"_fly_string); });
    global.define_intrinsic_accessor("CSSMathMax"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMathMaxPrototype>(realm, "CSSMathMax"_fly_string); });
    global.define_intrinsic_accessor("CSSMathMin"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMathMinPrototype>(realm, "CSSMathMin"_fly_string); });
    global.define_intrinsic_accessor("CSSMathNegate"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMathNegatePrototype>(realm, "CSSMathNegate"_fly_string); });
    global.define_intrinsic_accessor("CSSMathProduct"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMathProductPrototype>(realm, "CSSMathProduct"_fly_string); });
    global.define_intrinsic_accessor("CSSMathSum"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMathSumPrototype>(realm, "CSSMathSum"_fly_string); });
    global.define_intrinsic_accessor("CSSMathValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMathValuePrototype>(realm, "CSSMathValue"_fly_string); });
    global.define_intrinsic_accessor("CSSMatrixComponent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMatrixComponentPrototype>(realm, "CSSMatrixComponent"_fly_string); });
    global.define_intrinsic_accessor("CSSNumericArray"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSNumericArrayPrototype>(realm, "CSSNumericArray"_fly_string); });
    global.define_intrinsic_accessor("CSSNumericValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSNumericValuePrototype>(realm, "CSSNumericValue"_fly_string); });
    global.define_intrinsic_accessor("CSSPerspective"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSPerspectivePrototype>(realm, "CSSPerspective"_fly_string); });
    global.define_intrinsic_accessor("CSSRotate"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSRotatePrototype>(realm, "CSSRotate"_fly_string); });
    global.define_intrinsic_accessor("CSSScale"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSScalePrototype>(realm, "CSSScale"_fly_string); });
    global.define_intrinsic_accessor("CSSSkew"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSSkewPrototype>(realm, "CSSSkew"_fly_string); });
    global.define_intrinsic_accessor("CSSSkewX"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSSkewXPrototype>(realm, "CSSSkewX"_fly_string); });
    global.define_intrinsic_accessor("CSSSkewY"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSSkewYPrototype>(realm, "CSSSkewY"_fly_string); });
    global.define_intrinsic_accessor("CSSStyleValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSStyleValuePrototype>(realm, "CSSStyleValue"_fly_string); });
    global.define_intrinsic_accessor("CSSTransformComponent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSTransformComponentPrototype>(realm, "CSSTransformComponent"_fly_string); });
    global.define_intrinsic_accessor("CSSTransformValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSTransformValuePrototype>(realm, "CSSTransformValue"_fly_string); });
    global.define_intrinsic_accessor("CSSTranslate"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSTranslatePrototype>(realm, "CSSTranslate"_fly_string); });
    global.define_intrinsic_accessor("CSSUnitValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSUnitValuePrototype>(realm, "CSSUnitValue"_fly_string); });
    global.define_intrinsic_accessor("CSSUnparsedValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSUnparsedValuePrototype>(realm, "CSSUnparsedValue"_fly_string); });
    global.define_intrinsic_accessor("CSSVariableReferenceValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSVariableReferenceValuePrototype>(realm, "CSSVariableReferenceValue"_fly_string); });
    global.define_intrinsic_accessor("FontFace"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FontFacePrototype>(realm, "FontFace"_fly_string); });
    global.define_intrinsic_accessor("FontFaceSet"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FontFaceSetPrototype>(realm, "FontFaceSet"_fly_string); });
    global.define_intrinsic_accessor("FontFaceSetLoadEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FontFaceSetLoadEventPrototype>(realm, "FontFaceSetLoadEvent"_fly_string); });
    global.define_intrinsic_accessor("StylePropertyMapReadOnly"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<StylePropertyMapReadOnlyPrototype>(realm, "StylePropertyMapReadOnly"_fly_string); });
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
    global.define_intrinsic_accessor("Headers"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HeadersPrototype>(realm, "Headers"_fly_string); });
    global.define_intrinsic_accessor("Request"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<RequestPrototype>(realm, "Request"_fly_string); });
    global.define_intrinsic_accessor("Response"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ResponsePrototype>(realm, "Response"_fly_string); });
    global.define_intrinsic_accessor("Blob"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<BlobPrototype>(realm, "Blob"_fly_string); });
    global.define_intrinsic_accessor("File"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FilePrototype>(realm, "File"_fly_string); });
    global.define_intrinsic_accessor("FileList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FileListPrototype>(realm, "FileList"_fly_string); });
    global.define_intrinsic_accessor("FileReader"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FileReaderPrototype>(realm, "FileReader"_fly_string); });
    global.define_intrinsic_accessor("FileReaderSync"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FileReaderSyncPrototype>(realm, "FileReaderSync"_fly_string); });
    global.define_intrinsic_accessor("DOMMatrix"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMMatrixPrototype>(realm, "DOMMatrix"_fly_string); });
    global.define_intrinsic_accessor("DOMMatrixReadOnly"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMMatrixReadOnlyPrototype>(realm, "DOMMatrixReadOnly"_fly_string); });
    global.define_intrinsic_accessor("DOMPoint"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMPointPrototype>(realm, "DOMPoint"_fly_string); });
    global.define_intrinsic_accessor("DOMPointReadOnly"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMPointReadOnlyPrototype>(realm, "DOMPointReadOnly"_fly_string); });
    global.define_intrinsic_accessor("DOMQuad"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMQuadPrototype>(realm, "DOMQuad"_fly_string); });
    global.define_intrinsic_accessor("DOMRect"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMRectPrototype>(realm, "DOMRect"_fly_string); });
    global.define_intrinsic_accessor("DOMRectReadOnly"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMRectReadOnlyPrototype>(realm, "DOMRectReadOnly"_fly_string); });
    global.define_intrinsic_accessor("BroadcastChannel"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<BroadcastChannelPrototype>(realm, "BroadcastChannel"_fly_string); });
    global.define_intrinsic_accessor("CanvasGradient"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CanvasGradientPrototype>(realm, "CanvasGradient"_fly_string); });
    global.define_intrinsic_accessor("CanvasPattern"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CanvasPatternPrototype>(realm, "CanvasPattern"_fly_string); });
    global.define_intrinsic_accessor("CloseEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CloseEventPrototype>(realm, "CloseEvent"_fly_string); });
    global.define_intrinsic_accessor("DOMStringList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMStringListPrototype>(realm, "DOMStringList"_fly_string); });
    global.define_intrinsic_accessor("ErrorEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ErrorEventPrototype>(realm, "ErrorEvent"_fly_string); });
    global.define_intrinsic_accessor("EventSource"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<EventSourcePrototype>(realm, "EventSource"_fly_string); });
    global.define_intrinsic_accessor("ImageBitmap"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ImageBitmapPrototype>(realm, "ImageBitmap"_fly_string); });
    global.define_intrinsic_accessor("ImageData"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ImageDataPrototype>(realm, "ImageData"_fly_string); });
    global.define_intrinsic_accessor("MessageChannel"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MessageChannelPrototype>(realm, "MessageChannel"_fly_string); });
    global.define_intrinsic_accessor("MessageEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MessageEventPrototype>(realm, "MessageEvent"_fly_string); });
    global.define_intrinsic_accessor("MessagePort"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MessagePortPrototype>(realm, "MessagePort"_fly_string); });
    if (expose_experimental_interfaces) {
    global.define_intrinsic_accessor("OffscreenCanvas"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<OffscreenCanvasPrototype>(realm, "OffscreenCanvas"_fly_string); });
    }
    if (expose_experimental_interfaces) {
    global.define_intrinsic_accessor("OffscreenCanvasRenderingContext2D"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<OffscreenCanvasRenderingContext2DPrototype>(realm, "OffscreenCanvasRenderingContext2D"_fly_string); });
    }
    global.define_intrinsic_accessor("Path2D"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<Path2DPrototype>(realm, "Path2D"_fly_string); });
    global.define_intrinsic_accessor("PromiseRejectionEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PromiseRejectionEventPrototype>(realm, "PromiseRejectionEvent"_fly_string); });
    global.define_intrinsic_accessor("SharedWorkerGlobalScope"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SharedWorkerGlobalScopePrototype>(realm, "SharedWorkerGlobalScope"_fly_string); });
    global.define_intrinsic_accessor("TextMetrics"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TextMetricsPrototype>(realm, "TextMetrics"_fly_string); });
    global.define_intrinsic_accessor("Worker"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WorkerPrototype>(realm, "Worker"_fly_string); });
    global.define_intrinsic_accessor("WorkerGlobalScope"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WorkerGlobalScopePrototype>(realm, "WorkerGlobalScope"_fly_string); });
    global.define_intrinsic_accessor("WorkerLocation"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WorkerLocationPrototype>(realm, "WorkerLocation"_fly_string); });
    global.define_intrinsic_accessor("WorkerNavigator"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WorkerNavigatorPrototype>(realm, "WorkerNavigator"_fly_string); });
    global.define_intrinsic_accessor("Performance"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PerformancePrototype>(realm, "Performance"_fly_string); });
    global.define_intrinsic_accessor("IDBCursor"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBCursorPrototype>(realm, "IDBCursor"_fly_string); });
    global.define_intrinsic_accessor("IDBCursorWithValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBCursorWithValuePrototype>(realm, "IDBCursorWithValue"_fly_string); });
    global.define_intrinsic_accessor("IDBDatabase"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBDatabasePrototype>(realm, "IDBDatabase"_fly_string); });
    global.define_intrinsic_accessor("IDBFactory"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBFactoryPrototype>(realm, "IDBFactory"_fly_string); });
    global.define_intrinsic_accessor("IDBIndex"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBIndexPrototype>(realm, "IDBIndex"_fly_string); });
    global.define_intrinsic_accessor("IDBKeyRange"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBKeyRangePrototype>(realm, "IDBKeyRange"_fly_string); });
    global.define_intrinsic_accessor("IDBObjectStore"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBObjectStorePrototype>(realm, "IDBObjectStore"_fly_string); });
    global.define_intrinsic_accessor("IDBOpenDBRequest"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBOpenDBRequestPrototype>(realm, "IDBOpenDBRequest"_fly_string); });
    global.define_intrinsic_accessor("IDBRecord"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBRecordPrototype>(realm, "IDBRecord"_fly_string); });
    global.define_intrinsic_accessor("IDBRequest"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBRequestPrototype>(realm, "IDBRequest"_fly_string); });
    global.define_intrinsic_accessor("IDBTransaction"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBTransactionPrototype>(realm, "IDBTransaction"_fly_string); });
    global.define_intrinsic_accessor("IDBVersionChangeEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBVersionChangeEventPrototype>(realm, "IDBVersionChangeEvent"_fly_string); });
    global.define_intrinsic_accessor("MediaCapabilities"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MediaCapabilitiesPrototype>(realm, "MediaCapabilities"_fly_string); });
    global.define_intrinsic_accessor("Notification"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<NotificationPrototype>(realm, "Notification"_fly_string); });
    global.define_intrinsic_accessor("PerformanceEntry"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PerformanceEntryPrototype>(realm, "PerformanceEntry"_fly_string); });
    global.define_intrinsic_accessor("PerformanceObserver"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PerformanceObserverPrototype>(realm, "PerformanceObserver"_fly_string); });
    global.define_intrinsic_accessor("PerformanceObserverEntryList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PerformanceObserverEntryListPrototype>(realm, "PerformanceObserverEntryList"_fly_string); });
    global.define_intrinsic_accessor("PerformanceResourceTiming"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PerformanceResourceTimingPrototype>(realm, "PerformanceResourceTiming"_fly_string); });
    if (is_secure_context) {
    if (expose_experimental_interfaces) {
    global.define_intrinsic_accessor("CacheStorage"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CacheStoragePrototype>(realm, "CacheStorage"_fly_string); });
    }
    }
    if (is_secure_context) {
    global.define_intrinsic_accessor("ServiceWorker"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ServiceWorkerPrototype>(realm, "ServiceWorker"_fly_string); });
    }
    if (is_secure_context) {
    global.define_intrinsic_accessor("ServiceWorkerContainer"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ServiceWorkerContainerPrototype>(realm, "ServiceWorkerContainer"_fly_string); });
    }
    if (is_secure_context) {
    global.define_intrinsic_accessor("ServiceWorkerRegistration"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ServiceWorkerRegistrationPrototype>(realm, "ServiceWorkerRegistration"_fly_string); });
    }
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
    global.define_intrinsic_accessor("TrustedHTML"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TrustedHTMLPrototype>(realm, "TrustedHTML"_fly_string); });
    global.define_intrinsic_accessor("TrustedScript"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TrustedScriptPrototype>(realm, "TrustedScript"_fly_string); });
    global.define_intrinsic_accessor("TrustedScriptURL"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TrustedScriptURLPrototype>(realm, "TrustedScriptURL"_fly_string); });
    global.define_intrinsic_accessor("TrustedTypePolicy"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TrustedTypePolicyPrototype>(realm, "TrustedTypePolicy"_fly_string); });
    global.define_intrinsic_accessor("TrustedTypePolicyFactory"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TrustedTypePolicyFactoryPrototype>(realm, "TrustedTypePolicyFactory"_fly_string); });
    if (is_secure_context) {
    global.define_intrinsic_accessor("StorageManager"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<StorageManagerPrototype>(realm, "StorageManager"_fly_string); });
    }
    global.define_intrinsic_accessor("URLPattern"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<URLPatternPrototype>(realm, "URLPattern"_fly_string); });
    global.define_intrinsic_accessor("PerformanceMark"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PerformanceMarkPrototype>(realm, "PerformanceMark"_fly_string); });
    global.define_intrinsic_accessor("PerformanceMeasure"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PerformanceMeasurePrototype>(realm, "PerformanceMeasure"_fly_string); });
    global.define_intrinsic_accessor("WebAssembly"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_namespace<WebAssemblyNamespace>(realm, "WebAssembly"_fly_string); });
    global.define_intrinsic_accessor("WebGL2RenderingContext"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGL2RenderingContextPrototype>(realm, "WebGL2RenderingContext"_fly_string); });
    global.define_intrinsic_accessor("WebGLActiveInfo"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLActiveInfoPrototype>(realm, "WebGLActiveInfo"_fly_string); });
    global.define_intrinsic_accessor("WebGLBuffer"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLBufferPrototype>(realm, "WebGLBuffer"_fly_string); });
    global.define_intrinsic_accessor("WebGLContextEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLContextEventPrototype>(realm, "WebGLContextEvent"_fly_string); });
    global.define_intrinsic_accessor("WebGLFramebuffer"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLFramebufferPrototype>(realm, "WebGLFramebuffer"_fly_string); });
    global.define_intrinsic_accessor("WebGLObject"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLObjectPrototype>(realm, "WebGLObject"_fly_string); });
    global.define_intrinsic_accessor("WebGLProgram"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLProgramPrototype>(realm, "WebGLProgram"_fly_string); });
    global.define_intrinsic_accessor("WebGLQuery"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLQueryPrototype>(realm, "WebGLQuery"_fly_string); });
    global.define_intrinsic_accessor("WebGLRenderbuffer"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLRenderbufferPrototype>(realm, "WebGLRenderbuffer"_fly_string); });
    global.define_intrinsic_accessor("WebGLRenderingContext"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLRenderingContextPrototype>(realm, "WebGLRenderingContext"_fly_string); });
    global.define_intrinsic_accessor("WebGLSampler"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLSamplerPrototype>(realm, "WebGLSampler"_fly_string); });
    global.define_intrinsic_accessor("WebGLShader"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLShaderPrototype>(realm, "WebGLShader"_fly_string); });
    global.define_intrinsic_accessor("WebGLShaderPrecisionFormat"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLShaderPrecisionFormatPrototype>(realm, "WebGLShaderPrecisionFormat"_fly_string); });
    global.define_intrinsic_accessor("WebGLSync"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLSyncPrototype>(realm, "WebGLSync"_fly_string); });
    global.define_intrinsic_accessor("WebGLTexture"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLTexturePrototype>(realm, "WebGLTexture"_fly_string); });
    global.define_intrinsic_accessor("WebGLTransformFeedback"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLTransformFeedbackPrototype>(realm, "WebGLTransformFeedback"_fly_string); });
    global.define_intrinsic_accessor("WebGLUniformLocation"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLUniformLocationPrototype>(realm, "WebGLUniformLocation"_fly_string); });
    global.define_intrinsic_accessor("WebGLVertexArrayObject"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLVertexArrayObjectPrototype>(realm, "WebGLVertexArrayObject"_fly_string); });
    global.define_intrinsic_accessor("DOMException"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMExceptionPrototype>(realm, "DOMException"_fly_string); });
    global.define_intrinsic_accessor("QuotaExceededError"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<QuotaExceededErrorPrototype>(realm, "QuotaExceededError"_fly_string); });
    global.define_intrinsic_accessor("WebSocket"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebSocketPrototype>(realm, "WebSocket"_fly_string); });
    global.define_intrinsic_accessor("FormData"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FormDataPrototype>(realm, "FormData"_fly_string); });
    global.define_intrinsic_accessor("ProgressEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ProgressEventPrototype>(realm, "ProgressEvent"_fly_string); });
    global.define_intrinsic_accessor("XMLHttpRequest"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<XMLHttpRequestPrototype>(realm, "XMLHttpRequest"_fly_string); });
    global.define_intrinsic_accessor("XMLHttpRequestEventTarget"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<XMLHttpRequestEventTargetPrototype>(realm, "XMLHttpRequestEventTarget"_fly_string); });
    global.define_intrinsic_accessor("XMLHttpRequestUpload"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<XMLHttpRequestUploadPrototype>(realm, "XMLHttpRequestUpload"_fly_string); });
}

}
