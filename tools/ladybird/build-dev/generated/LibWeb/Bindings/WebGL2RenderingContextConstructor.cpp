
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
#include <LibWeb/Bindings/WebGL2RenderingContextConstructor.h>
#include <LibWeb/Bindings/WebGL2RenderingContextPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGL2RenderingContext.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLCanvasElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLRenderingContextBase.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CanvasRenderingContext2D.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLRenderingContext.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLActiveInfo.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLBuffer.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLFramebuffer.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLObject.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLProgram.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLRenderbuffer.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLShader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLShaderPrecisionFormat.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLTexture.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLUniformLocation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLQuery.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLSampler.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLSync.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLTransformFeedback.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLVertexArrayObject.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Canvas/CanvasDrawPath.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Canvas/CanvasTextDrawingStyles.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/DOMStringMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ElementInternals.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Element.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/QueuingStrategy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamBYOBReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamDefaultReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLImageElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLVideoElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ImageBitmap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGImageElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Path2D.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CanvasGradient.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CanvasPattern.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ImageData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPointReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TextMetrics.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrix.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleProperties.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StylePropertyMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/ShadowRoot.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CustomElements/CustomStateSet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/Animatable.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StylePropertyMapReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Attr.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DOMTokenList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/NamedNodeMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Node.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/NodeList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRectList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLSlotElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Window.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedHTML.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedTypePolicy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStreamDefaultWriter.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Request.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Scripting/Fetching.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLMediaElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGGraphicsElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrixReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPoint.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleDeclaration.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DocumentFragment.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/Animation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/KeyframeEffect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Document.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/HTMLCollection.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRectReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Text.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CookieStore/CookieStore.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/MediaQueryList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/Screen.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/VisualViewport.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CustomElements/CustomElementRegistry.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/History.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Navigation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Navigator.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/RequestIdleCallback/IdleRequest.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Speech/SpeechSynthesis.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedScript.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedScriptURL.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Headers.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/AudioTrackList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/MediaError.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TextTrack.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TextTrackList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TimeRanges.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/VideoTrackList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGAnimatedString.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StyleSheetList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/AnimationEffect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/AnimationTimeline.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/DocumentTimeline.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/FontFaceSet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/CDATASection.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Comment.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DOMImplementation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DocumentType.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/NodeFilter.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/NodeIterator.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/ProcessingInstruction.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Range.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/TreeWalker.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLAllCollection.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLHeadElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLScriptElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Location.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Selection/Selection.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ViewTransition/ViewTransition.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XPath/XPathResult.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XPath/XPathExpression.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/CharacterData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorkerGlobalScope.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/ScreenOrientation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/NavigationHistoryEntry.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/NavigationTransition.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Clipboard/Clipboard.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CredentialManagement/CredentialsContainer.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/EncryptedMediaExtensions/MediaKeySystemAccess.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Gamepad/Gamepad.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geolocation/Geolocation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/MimeTypeArray.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/PluginArray.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/UserActivation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/MediaCapabilitiesAPI/MediaCapabilities.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Serial/Serial.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorkerContainer.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebXR/XRSystem.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/MessagePort.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/External.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Storage.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Crypto/Crypto.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Response.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HighResolutionTime/Performance.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBFactory.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/CacheStorage.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedTypePolicyFactory.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/RequestIdleCallback/IdleDeadline.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Speech/SpeechSynthesisUtterance.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Speech/SpeechSynthesisVoice.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XHR/FormData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOMURL/URLSearchParams.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/AudioTrack.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/VideoTrack.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGSVGElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleSheet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSNumericValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/FontFace.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/XMLDocument.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbstractRange.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/NavigationDestination.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/NavigationType.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Clipboard/ClipboardItem.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CredentialManagement/Credential.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CredentialManagement/FederatedCredential.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CredentialManagement/PasswordCredential.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Gamepad/GamepadButton.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Gamepad/GamepadHapticActuator.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/MimeType.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Plugin.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Serial/SerialPort.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Worker.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorkerRegistration.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/StorageAPI/StorageManager.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Crypto/SubtleCrypto.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/NavigationTiming/PerformanceNavigation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/NavigationTiming/PerformanceTiming.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/PerformanceTimeline/PerformanceEntry.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/UserTiming/PerformanceMark.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/UserTiming/PerformanceMeasure.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/File.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRule.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRuleList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/MediaList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StyleSheet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSUnitValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorker.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Crypto/CryptoKey.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGAnimatedRect.h>

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

GC_DEFINE_ALLOCATOR(WebGL2RenderingContextConstructor);

WebGL2RenderingContextConstructor::WebGL2RenderingContextConstructor(JS::Realm& realm)
    : NativeFunction("WebGL2RenderingContext"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

WebGL2RenderingContextConstructor::~WebGL2RenderingContextConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> WebGL2RenderingContextConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "WebGL2RenderingContext");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> WebGL2RenderingContextConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "WebGL2RenderingContextConstructor::construct");

    return vm().throw_completion<JS::TypeError>(JS::ErrorType::NotAConstructor, "WebGL2RenderingContext");
}


void WebGL2RenderingContextConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    define_direct_property(vm.names.length, JS::Value(0), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "WebGL2RenderingContext"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<WebGL2RenderingContextPrototype>(realm, "WebGL2RenderingContext"_fly_string), 0);


    auto constant_READ_BUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0C02));

    define_direct_property("READ_BUFFER"_utf16_fly_string, constant_READ_BUFFER_value, JS::Attribute::Enumerable);

    auto constant_UNPACK_ROW_LENGTH_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0CF2));

    define_direct_property("UNPACK_ROW_LENGTH"_utf16_fly_string, constant_UNPACK_ROW_LENGTH_value, JS::Attribute::Enumerable);

    auto constant_UNPACK_SKIP_ROWS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0CF3));

    define_direct_property("UNPACK_SKIP_ROWS"_utf16_fly_string, constant_UNPACK_SKIP_ROWS_value, JS::Attribute::Enumerable);

    auto constant_UNPACK_SKIP_PIXELS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0CF4));

    define_direct_property("UNPACK_SKIP_PIXELS"_utf16_fly_string, constant_UNPACK_SKIP_PIXELS_value, JS::Attribute::Enumerable);

    auto constant_PACK_ROW_LENGTH_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0D02));

    define_direct_property("PACK_ROW_LENGTH"_utf16_fly_string, constant_PACK_ROW_LENGTH_value, JS::Attribute::Enumerable);

    auto constant_PACK_SKIP_ROWS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0D03));

    define_direct_property("PACK_SKIP_ROWS"_utf16_fly_string, constant_PACK_SKIP_ROWS_value, JS::Attribute::Enumerable);

    auto constant_PACK_SKIP_PIXELS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0D04));

    define_direct_property("PACK_SKIP_PIXELS"_utf16_fly_string, constant_PACK_SKIP_PIXELS_value, JS::Attribute::Enumerable);

    auto constant_COLOR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1800));

    define_direct_property("COLOR"_utf16_fly_string, constant_COLOR_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1801));

    define_direct_property("DEPTH"_utf16_fly_string, constant_DEPTH_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1802));

    define_direct_property("STENCIL"_utf16_fly_string, constant_STENCIL_value, JS::Attribute::Enumerable);

    auto constant_RED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1903));

    define_direct_property("RED"_utf16_fly_string, constant_RED_value, JS::Attribute::Enumerable);

    auto constant_RGB8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8051));

    define_direct_property("RGB8"_utf16_fly_string, constant_RGB8_value, JS::Attribute::Enumerable);

    auto constant_RGB10_A2_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8059));

    define_direct_property("RGB10_A2"_utf16_fly_string, constant_RGB10_A2_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_BINDING_3D_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x806A));

    define_direct_property("TEXTURE_BINDING_3D"_utf16_fly_string, constant_TEXTURE_BINDING_3D_value, JS::Attribute::Enumerable);

    auto constant_UNPACK_SKIP_IMAGES_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x806D));

    define_direct_property("UNPACK_SKIP_IMAGES"_utf16_fly_string, constant_UNPACK_SKIP_IMAGES_value, JS::Attribute::Enumerable);

    auto constant_UNPACK_IMAGE_HEIGHT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x806E));

    define_direct_property("UNPACK_IMAGE_HEIGHT"_utf16_fly_string, constant_UNPACK_IMAGE_HEIGHT_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_3D_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x806F));

    define_direct_property("TEXTURE_3D"_utf16_fly_string, constant_TEXTURE_3D_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_WRAP_R_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8072));

    define_direct_property("TEXTURE_WRAP_R"_utf16_fly_string, constant_TEXTURE_WRAP_R_value, JS::Attribute::Enumerable);

    auto constant_MAX_3D_TEXTURE_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8073));

    define_direct_property("MAX_3D_TEXTURE_SIZE"_utf16_fly_string, constant_MAX_3D_TEXTURE_SIZE_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_2_10_10_10_REV_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8368));

    define_direct_property("UNSIGNED_INT_2_10_10_10_REV"_utf16_fly_string, constant_UNSIGNED_INT_2_10_10_10_REV_value, JS::Attribute::Enumerable);

    auto constant_MAX_ELEMENTS_VERTICES_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x80E8));

    define_direct_property("MAX_ELEMENTS_VERTICES"_utf16_fly_string, constant_MAX_ELEMENTS_VERTICES_value, JS::Attribute::Enumerable);

    auto constant_MAX_ELEMENTS_INDICES_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x80E9));

    define_direct_property("MAX_ELEMENTS_INDICES"_utf16_fly_string, constant_MAX_ELEMENTS_INDICES_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_MIN_LOD_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x813A));

    define_direct_property("TEXTURE_MIN_LOD"_utf16_fly_string, constant_TEXTURE_MIN_LOD_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_MAX_LOD_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x813B));

    define_direct_property("TEXTURE_MAX_LOD"_utf16_fly_string, constant_TEXTURE_MAX_LOD_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_BASE_LEVEL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x813C));

    define_direct_property("TEXTURE_BASE_LEVEL"_utf16_fly_string, constant_TEXTURE_BASE_LEVEL_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_MAX_LEVEL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x813D));

    define_direct_property("TEXTURE_MAX_LEVEL"_utf16_fly_string, constant_TEXTURE_MAX_LEVEL_value, JS::Attribute::Enumerable);

    auto constant_MIN_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8007));

    define_direct_property("MIN"_utf16_fly_string, constant_MIN_value, JS::Attribute::Enumerable);

    auto constant_MAX_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8008));

    define_direct_property("MAX"_utf16_fly_string, constant_MAX_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_COMPONENT24_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x81A6));

    define_direct_property("DEPTH_COMPONENT24"_utf16_fly_string, constant_DEPTH_COMPONENT24_value, JS::Attribute::Enumerable);

    auto constant_MAX_TEXTURE_LOD_BIAS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84FD));

    define_direct_property("MAX_TEXTURE_LOD_BIAS"_utf16_fly_string, constant_MAX_TEXTURE_LOD_BIAS_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_COMPARE_MODE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x884C));

    define_direct_property("TEXTURE_COMPARE_MODE"_utf16_fly_string, constant_TEXTURE_COMPARE_MODE_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_COMPARE_FUNC_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x884D));

    define_direct_property("TEXTURE_COMPARE_FUNC"_utf16_fly_string, constant_TEXTURE_COMPARE_FUNC_value, JS::Attribute::Enumerable);

    auto constant_CURRENT_QUERY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8865));

    define_direct_property("CURRENT_QUERY"_utf16_fly_string, constant_CURRENT_QUERY_value, JS::Attribute::Enumerable);

    auto constant_QUERY_RESULT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8866));

    define_direct_property("QUERY_RESULT"_utf16_fly_string, constant_QUERY_RESULT_value, JS::Attribute::Enumerable);

    auto constant_QUERY_RESULT_AVAILABLE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8867));

    define_direct_property("QUERY_RESULT_AVAILABLE"_utf16_fly_string, constant_QUERY_RESULT_AVAILABLE_value, JS::Attribute::Enumerable);

    auto constant_STREAM_READ_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88E1));

    define_direct_property("STREAM_READ"_utf16_fly_string, constant_STREAM_READ_value, JS::Attribute::Enumerable);

    auto constant_STREAM_COPY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88E2));

    define_direct_property("STREAM_COPY"_utf16_fly_string, constant_STREAM_COPY_value, JS::Attribute::Enumerable);

    auto constant_STATIC_READ_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88E5));

    define_direct_property("STATIC_READ"_utf16_fly_string, constant_STATIC_READ_value, JS::Attribute::Enumerable);

    auto constant_STATIC_COPY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88E6));

    define_direct_property("STATIC_COPY"_utf16_fly_string, constant_STATIC_COPY_value, JS::Attribute::Enumerable);

    auto constant_DYNAMIC_READ_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88E9));

    define_direct_property("DYNAMIC_READ"_utf16_fly_string, constant_DYNAMIC_READ_value, JS::Attribute::Enumerable);

    auto constant_DYNAMIC_COPY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88EA));

    define_direct_property("DYNAMIC_COPY"_utf16_fly_string, constant_DYNAMIC_COPY_value, JS::Attribute::Enumerable);

    auto constant_MAX_DRAW_BUFFERS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8824));

    define_direct_property("MAX_DRAW_BUFFERS"_utf16_fly_string, constant_MAX_DRAW_BUFFERS_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER0_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8825));

    define_direct_property("DRAW_BUFFER0"_utf16_fly_string, constant_DRAW_BUFFER0_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER1_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8826));

    define_direct_property("DRAW_BUFFER1"_utf16_fly_string, constant_DRAW_BUFFER1_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER2_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8827));

    define_direct_property("DRAW_BUFFER2"_utf16_fly_string, constant_DRAW_BUFFER2_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER3_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8828));

    define_direct_property("DRAW_BUFFER3"_utf16_fly_string, constant_DRAW_BUFFER3_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER4_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8829));

    define_direct_property("DRAW_BUFFER4"_utf16_fly_string, constant_DRAW_BUFFER4_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER5_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882A));

    define_direct_property("DRAW_BUFFER5"_utf16_fly_string, constant_DRAW_BUFFER5_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER6_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882B));

    define_direct_property("DRAW_BUFFER6"_utf16_fly_string, constant_DRAW_BUFFER6_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER7_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882C));

    define_direct_property("DRAW_BUFFER7"_utf16_fly_string, constant_DRAW_BUFFER7_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882D));

    define_direct_property("DRAW_BUFFER8"_utf16_fly_string, constant_DRAW_BUFFER8_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER9_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882E));

    define_direct_property("DRAW_BUFFER9"_utf16_fly_string, constant_DRAW_BUFFER9_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER10_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882F));

    define_direct_property("DRAW_BUFFER10"_utf16_fly_string, constant_DRAW_BUFFER10_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER11_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8830));

    define_direct_property("DRAW_BUFFER11"_utf16_fly_string, constant_DRAW_BUFFER11_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER12_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8831));

    define_direct_property("DRAW_BUFFER12"_utf16_fly_string, constant_DRAW_BUFFER12_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER13_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8832));

    define_direct_property("DRAW_BUFFER13"_utf16_fly_string, constant_DRAW_BUFFER13_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER14_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8833));

    define_direct_property("DRAW_BUFFER14"_utf16_fly_string, constant_DRAW_BUFFER14_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER15_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8834));

    define_direct_property("DRAW_BUFFER15"_utf16_fly_string, constant_DRAW_BUFFER15_value, JS::Attribute::Enumerable);

    auto constant_MAX_FRAGMENT_UNIFORM_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B49));

    define_direct_property("MAX_FRAGMENT_UNIFORM_COMPONENTS"_utf16_fly_string, constant_MAX_FRAGMENT_UNIFORM_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_MAX_VERTEX_UNIFORM_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B4A));

    define_direct_property("MAX_VERTEX_UNIFORM_COMPONENTS"_utf16_fly_string, constant_MAX_VERTEX_UNIFORM_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_SAMPLER_3D_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B5F));

    define_direct_property("SAMPLER_3D"_utf16_fly_string, constant_SAMPLER_3D_value, JS::Attribute::Enumerable);

    auto constant_SAMPLER_2D_SHADOW_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B62));

    define_direct_property("SAMPLER_2D_SHADOW"_utf16_fly_string, constant_SAMPLER_2D_SHADOW_value, JS::Attribute::Enumerable);

    auto constant_FRAGMENT_SHADER_DERIVATIVE_HINT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B8B));

    define_direct_property("FRAGMENT_SHADER_DERIVATIVE_HINT"_utf16_fly_string, constant_FRAGMENT_SHADER_DERIVATIVE_HINT_value, JS::Attribute::Enumerable);

    auto constant_PIXEL_PACK_BUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88EB));

    define_direct_property("PIXEL_PACK_BUFFER"_utf16_fly_string, constant_PIXEL_PACK_BUFFER_value, JS::Attribute::Enumerable);

    auto constant_PIXEL_UNPACK_BUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88EC));

    define_direct_property("PIXEL_UNPACK_BUFFER"_utf16_fly_string, constant_PIXEL_UNPACK_BUFFER_value, JS::Attribute::Enumerable);

    auto constant_PIXEL_PACK_BUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88ED));

    define_direct_property("PIXEL_PACK_BUFFER_BINDING"_utf16_fly_string, constant_PIXEL_PACK_BUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_PIXEL_UNPACK_BUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88EF));

    define_direct_property("PIXEL_UNPACK_BUFFER_BINDING"_utf16_fly_string, constant_PIXEL_UNPACK_BUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_MAT2x3_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B65));

    define_direct_property("FLOAT_MAT2x3"_utf16_fly_string, constant_FLOAT_MAT2x3_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_MAT2x4_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B66));

    define_direct_property("FLOAT_MAT2x4"_utf16_fly_string, constant_FLOAT_MAT2x4_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_MAT3x2_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B67));

    define_direct_property("FLOAT_MAT3x2"_utf16_fly_string, constant_FLOAT_MAT3x2_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_MAT3x4_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B68));

    define_direct_property("FLOAT_MAT3x4"_utf16_fly_string, constant_FLOAT_MAT3x4_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_MAT4x2_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B69));

    define_direct_property("FLOAT_MAT4x2"_utf16_fly_string, constant_FLOAT_MAT4x2_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_MAT4x3_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B6A));

    define_direct_property("FLOAT_MAT4x3"_utf16_fly_string, constant_FLOAT_MAT4x3_value, JS::Attribute::Enumerable);

    auto constant_SRGB_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C40));

    define_direct_property("SRGB"_utf16_fly_string, constant_SRGB_value, JS::Attribute::Enumerable);

    auto constant_SRGB8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C41));

    define_direct_property("SRGB8"_utf16_fly_string, constant_SRGB8_value, JS::Attribute::Enumerable);

    auto constant_SRGB8_ALPHA8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C43));

    define_direct_property("SRGB8_ALPHA8"_utf16_fly_string, constant_SRGB8_ALPHA8_value, JS::Attribute::Enumerable);

    auto constant_COMPARE_REF_TO_TEXTURE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x884E));

    define_direct_property("COMPARE_REF_TO_TEXTURE"_utf16_fly_string, constant_COMPARE_REF_TO_TEXTURE_value, JS::Attribute::Enumerable);

    auto constant_RGBA32F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8814));

    define_direct_property("RGBA32F"_utf16_fly_string, constant_RGBA32F_value, JS::Attribute::Enumerable);

    auto constant_RGB32F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8815));

    define_direct_property("RGB32F"_utf16_fly_string, constant_RGB32F_value, JS::Attribute::Enumerable);

    auto constant_RGBA16F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x881A));

    define_direct_property("RGBA16F"_utf16_fly_string, constant_RGBA16F_value, JS::Attribute::Enumerable);

    auto constant_RGB16F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x881B));

    define_direct_property("RGB16F"_utf16_fly_string, constant_RGB16F_value, JS::Attribute::Enumerable);

    auto constant_VERTEX_ATTRIB_ARRAY_INTEGER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88FD));

    define_direct_property("VERTEX_ATTRIB_ARRAY_INTEGER"_utf16_fly_string, constant_VERTEX_ATTRIB_ARRAY_INTEGER_value, JS::Attribute::Enumerable);

    auto constant_MAX_ARRAY_TEXTURE_LAYERS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88FF));

    define_direct_property("MAX_ARRAY_TEXTURE_LAYERS"_utf16_fly_string, constant_MAX_ARRAY_TEXTURE_LAYERS_value, JS::Attribute::Enumerable);

    auto constant_MIN_PROGRAM_TEXEL_OFFSET_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8904));

    define_direct_property("MIN_PROGRAM_TEXEL_OFFSET"_utf16_fly_string, constant_MIN_PROGRAM_TEXEL_OFFSET_value, JS::Attribute::Enumerable);

    auto constant_MAX_PROGRAM_TEXEL_OFFSET_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8905));

    define_direct_property("MAX_PROGRAM_TEXEL_OFFSET"_utf16_fly_string, constant_MAX_PROGRAM_TEXEL_OFFSET_value, JS::Attribute::Enumerable);

    auto constant_MAX_VARYING_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B4B));

    define_direct_property("MAX_VARYING_COMPONENTS"_utf16_fly_string, constant_MAX_VARYING_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_2D_ARRAY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C1A));

    define_direct_property("TEXTURE_2D_ARRAY"_utf16_fly_string, constant_TEXTURE_2D_ARRAY_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_BINDING_2D_ARRAY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C1D));

    define_direct_property("TEXTURE_BINDING_2D_ARRAY"_utf16_fly_string, constant_TEXTURE_BINDING_2D_ARRAY_value, JS::Attribute::Enumerable);

    auto constant_R11F_G11F_B10F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C3A));

    define_direct_property("R11F_G11F_B10F"_utf16_fly_string, constant_R11F_G11F_B10F_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_10F_11F_11F_REV_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C3B));

    define_direct_property("UNSIGNED_INT_10F_11F_11F_REV"_utf16_fly_string, constant_UNSIGNED_INT_10F_11F_11F_REV_value, JS::Attribute::Enumerable);

    auto constant_RGB9_E5_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C3D));

    define_direct_property("RGB9_E5"_utf16_fly_string, constant_RGB9_E5_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_5_9_9_9_REV_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C3E));

    define_direct_property("UNSIGNED_INT_5_9_9_9_REV"_utf16_fly_string, constant_UNSIGNED_INT_5_9_9_9_REV_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_BUFFER_MODE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C7F));

    define_direct_property("TRANSFORM_FEEDBACK_BUFFER_MODE"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_BUFFER_MODE_value, JS::Attribute::Enumerable);

    auto constant_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C80));

    define_direct_property("MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS"_utf16_fly_string, constant_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_VARYINGS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C83));

    define_direct_property("TRANSFORM_FEEDBACK_VARYINGS"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_VARYINGS_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_BUFFER_START_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C84));

    define_direct_property("TRANSFORM_FEEDBACK_BUFFER_START"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_BUFFER_START_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_BUFFER_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C85));

    define_direct_property("TRANSFORM_FEEDBACK_BUFFER_SIZE"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_BUFFER_SIZE_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C88));

    define_direct_property("TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_value, JS::Attribute::Enumerable);

    auto constant_RASTERIZER_DISCARD_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C89));

    define_direct_property("RASTERIZER_DISCARD"_utf16_fly_string, constant_RASTERIZER_DISCARD_value, JS::Attribute::Enumerable);

    auto constant_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C8A));

    define_direct_property("MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS"_utf16_fly_string, constant_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C8B));

    define_direct_property("MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS"_utf16_fly_string, constant_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_value, JS::Attribute::Enumerable);

    auto constant_INTERLEAVED_ATTRIBS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C8C));

    define_direct_property("INTERLEAVED_ATTRIBS"_utf16_fly_string, constant_INTERLEAVED_ATTRIBS_value, JS::Attribute::Enumerable);

    auto constant_SEPARATE_ATTRIBS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C8D));

    define_direct_property("SEPARATE_ATTRIBS"_utf16_fly_string, constant_SEPARATE_ATTRIBS_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_BUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C8E));

    define_direct_property("TRANSFORM_FEEDBACK_BUFFER"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_BUFFER_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_BUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C8F));

    define_direct_property("TRANSFORM_FEEDBACK_BUFFER_BINDING"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_BUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_RGBA32UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D70));

    define_direct_property("RGBA32UI"_utf16_fly_string, constant_RGBA32UI_value, JS::Attribute::Enumerable);

    auto constant_RGB32UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D71));

    define_direct_property("RGB32UI"_utf16_fly_string, constant_RGB32UI_value, JS::Attribute::Enumerable);

    auto constant_RGBA16UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D76));

    define_direct_property("RGBA16UI"_utf16_fly_string, constant_RGBA16UI_value, JS::Attribute::Enumerable);

    auto constant_RGB16UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D77));

    define_direct_property("RGB16UI"_utf16_fly_string, constant_RGB16UI_value, JS::Attribute::Enumerable);

    auto constant_RGBA8UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D7C));

    define_direct_property("RGBA8UI"_utf16_fly_string, constant_RGBA8UI_value, JS::Attribute::Enumerable);

    auto constant_RGB8UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D7D));

    define_direct_property("RGB8UI"_utf16_fly_string, constant_RGB8UI_value, JS::Attribute::Enumerable);

    auto constant_RGBA32I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D82));

    define_direct_property("RGBA32I"_utf16_fly_string, constant_RGBA32I_value, JS::Attribute::Enumerable);

    auto constant_RGB32I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D83));

    define_direct_property("RGB32I"_utf16_fly_string, constant_RGB32I_value, JS::Attribute::Enumerable);

    auto constant_RGBA16I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D88));

    define_direct_property("RGBA16I"_utf16_fly_string, constant_RGBA16I_value, JS::Attribute::Enumerable);

    auto constant_RGB16I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D89));

    define_direct_property("RGB16I"_utf16_fly_string, constant_RGB16I_value, JS::Attribute::Enumerable);

    auto constant_RGBA8I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D8E));

    define_direct_property("RGBA8I"_utf16_fly_string, constant_RGBA8I_value, JS::Attribute::Enumerable);

    auto constant_RGB8I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D8F));

    define_direct_property("RGB8I"_utf16_fly_string, constant_RGB8I_value, JS::Attribute::Enumerable);

    auto constant_RED_INTEGER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D94));

    define_direct_property("RED_INTEGER"_utf16_fly_string, constant_RED_INTEGER_value, JS::Attribute::Enumerable);

    auto constant_RGB_INTEGER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D98));

    define_direct_property("RGB_INTEGER"_utf16_fly_string, constant_RGB_INTEGER_value, JS::Attribute::Enumerable);

    auto constant_RGBA_INTEGER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D99));

    define_direct_property("RGBA_INTEGER"_utf16_fly_string, constant_RGBA_INTEGER_value, JS::Attribute::Enumerable);

    auto constant_SAMPLER_2D_ARRAY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DC1));

    define_direct_property("SAMPLER_2D_ARRAY"_utf16_fly_string, constant_SAMPLER_2D_ARRAY_value, JS::Attribute::Enumerable);

    auto constant_SAMPLER_2D_ARRAY_SHADOW_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DC4));

    define_direct_property("SAMPLER_2D_ARRAY_SHADOW"_utf16_fly_string, constant_SAMPLER_2D_ARRAY_SHADOW_value, JS::Attribute::Enumerable);

    auto constant_SAMPLER_CUBE_SHADOW_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DC5));

    define_direct_property("SAMPLER_CUBE_SHADOW"_utf16_fly_string, constant_SAMPLER_CUBE_SHADOW_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_VEC2_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DC6));

    define_direct_property("UNSIGNED_INT_VEC2"_utf16_fly_string, constant_UNSIGNED_INT_VEC2_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_VEC3_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DC7));

    define_direct_property("UNSIGNED_INT_VEC3"_utf16_fly_string, constant_UNSIGNED_INT_VEC3_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_VEC4_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DC8));

    define_direct_property("UNSIGNED_INT_VEC4"_utf16_fly_string, constant_UNSIGNED_INT_VEC4_value, JS::Attribute::Enumerable);

    auto constant_INT_SAMPLER_2D_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DCA));

    define_direct_property("INT_SAMPLER_2D"_utf16_fly_string, constant_INT_SAMPLER_2D_value, JS::Attribute::Enumerable);

    auto constant_INT_SAMPLER_3D_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DCB));

    define_direct_property("INT_SAMPLER_3D"_utf16_fly_string, constant_INT_SAMPLER_3D_value, JS::Attribute::Enumerable);

    auto constant_INT_SAMPLER_CUBE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DCC));

    define_direct_property("INT_SAMPLER_CUBE"_utf16_fly_string, constant_INT_SAMPLER_CUBE_value, JS::Attribute::Enumerable);

    auto constant_INT_SAMPLER_2D_ARRAY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DCF));

    define_direct_property("INT_SAMPLER_2D_ARRAY"_utf16_fly_string, constant_INT_SAMPLER_2D_ARRAY_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_SAMPLER_2D_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DD2));

    define_direct_property("UNSIGNED_INT_SAMPLER_2D"_utf16_fly_string, constant_UNSIGNED_INT_SAMPLER_2D_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_SAMPLER_3D_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DD3));

    define_direct_property("UNSIGNED_INT_SAMPLER_3D"_utf16_fly_string, constant_UNSIGNED_INT_SAMPLER_3D_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_SAMPLER_CUBE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DD4));

    define_direct_property("UNSIGNED_INT_SAMPLER_CUBE"_utf16_fly_string, constant_UNSIGNED_INT_SAMPLER_CUBE_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_SAMPLER_2D_ARRAY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DD7));

    define_direct_property("UNSIGNED_INT_SAMPLER_2D_ARRAY"_utf16_fly_string, constant_UNSIGNED_INT_SAMPLER_2D_ARRAY_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_COMPONENT32F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CAC));

    define_direct_property("DEPTH_COMPONENT32F"_utf16_fly_string, constant_DEPTH_COMPONENT32F_value, JS::Attribute::Enumerable);

    auto constant_DEPTH32F_STENCIL8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CAD));

    define_direct_property("DEPTH32F_STENCIL8"_utf16_fly_string, constant_DEPTH32F_STENCIL8_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_32_UNSIGNED_INT_24_8_REV_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DAD));

    define_direct_property("FLOAT_32_UNSIGNED_INT_24_8_REV"_utf16_fly_string, constant_FLOAT_32_UNSIGNED_INT_24_8_REV_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8210));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8211));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_RED_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8212));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_RED_SIZE"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_RED_SIZE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8213));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_GREEN_SIZE"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8214));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_BLUE_SIZE"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8215));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8216));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8217));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_DEFAULT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8218));

    define_direct_property("FRAMEBUFFER_DEFAULT"_utf16_fly_string, constant_FRAMEBUFFER_DEFAULT_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_24_8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84FA));

    define_direct_property("UNSIGNED_INT_24_8"_utf16_fly_string, constant_UNSIGNED_INT_24_8_value, JS::Attribute::Enumerable);

    auto constant_DEPTH24_STENCIL8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88F0));

    define_direct_property("DEPTH24_STENCIL8"_utf16_fly_string, constant_DEPTH24_STENCIL8_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_NORMALIZED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C17));

    define_direct_property("UNSIGNED_NORMALIZED"_utf16_fly_string, constant_UNSIGNED_NORMALIZED_value, JS::Attribute::Enumerable);

    auto constant_DRAW_FRAMEBUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CA6));

    define_direct_property("DRAW_FRAMEBUFFER_BINDING"_utf16_fly_string, constant_DRAW_FRAMEBUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_READ_FRAMEBUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CA8));

    define_direct_property("READ_FRAMEBUFFER"_utf16_fly_string, constant_READ_FRAMEBUFFER_value, JS::Attribute::Enumerable);

    auto constant_DRAW_FRAMEBUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CA9));

    define_direct_property("DRAW_FRAMEBUFFER"_utf16_fly_string, constant_DRAW_FRAMEBUFFER_value, JS::Attribute::Enumerable);

    auto constant_READ_FRAMEBUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CAA));

    define_direct_property("READ_FRAMEBUFFER_BINDING"_utf16_fly_string, constant_READ_FRAMEBUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_RENDERBUFFER_SAMPLES_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CAB));

    define_direct_property("RENDERBUFFER_SAMPLES"_utf16_fly_string, constant_RENDERBUFFER_SAMPLES_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CD4));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER_value, JS::Attribute::Enumerable);

    auto constant_MAX_COLOR_ATTACHMENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CDF));

    define_direct_property("MAX_COLOR_ATTACHMENTS"_utf16_fly_string, constant_MAX_COLOR_ATTACHMENTS_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT1_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE1));

    define_direct_property("COLOR_ATTACHMENT1"_utf16_fly_string, constant_COLOR_ATTACHMENT1_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT2_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE2));

    define_direct_property("COLOR_ATTACHMENT2"_utf16_fly_string, constant_COLOR_ATTACHMENT2_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT3_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE3));

    define_direct_property("COLOR_ATTACHMENT3"_utf16_fly_string, constant_COLOR_ATTACHMENT3_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT4_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE4));

    define_direct_property("COLOR_ATTACHMENT4"_utf16_fly_string, constant_COLOR_ATTACHMENT4_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT5_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE5));

    define_direct_property("COLOR_ATTACHMENT5"_utf16_fly_string, constant_COLOR_ATTACHMENT5_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT6_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE6));

    define_direct_property("COLOR_ATTACHMENT6"_utf16_fly_string, constant_COLOR_ATTACHMENT6_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT7_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE7));

    define_direct_property("COLOR_ATTACHMENT7"_utf16_fly_string, constant_COLOR_ATTACHMENT7_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE8));

    define_direct_property("COLOR_ATTACHMENT8"_utf16_fly_string, constant_COLOR_ATTACHMENT8_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT9_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE9));

    define_direct_property("COLOR_ATTACHMENT9"_utf16_fly_string, constant_COLOR_ATTACHMENT9_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT10_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CEA));

    define_direct_property("COLOR_ATTACHMENT10"_utf16_fly_string, constant_COLOR_ATTACHMENT10_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT11_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CEB));

    define_direct_property("COLOR_ATTACHMENT11"_utf16_fly_string, constant_COLOR_ATTACHMENT11_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT12_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CEC));

    define_direct_property("COLOR_ATTACHMENT12"_utf16_fly_string, constant_COLOR_ATTACHMENT12_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT13_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CED));

    define_direct_property("COLOR_ATTACHMENT13"_utf16_fly_string, constant_COLOR_ATTACHMENT13_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT14_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CEE));

    define_direct_property("COLOR_ATTACHMENT14"_utf16_fly_string, constant_COLOR_ATTACHMENT14_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT15_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CEF));

    define_direct_property("COLOR_ATTACHMENT15"_utf16_fly_string, constant_COLOR_ATTACHMENT15_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D56));

    define_direct_property("FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"_utf16_fly_string, constant_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_value, JS::Attribute::Enumerable);

    auto constant_MAX_SAMPLES_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D57));

    define_direct_property("MAX_SAMPLES"_utf16_fly_string, constant_MAX_SAMPLES_value, JS::Attribute::Enumerable);

    auto constant_HALF_FLOAT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x140B));

    define_direct_property("HALF_FLOAT"_utf16_fly_string, constant_HALF_FLOAT_value, JS::Attribute::Enumerable);

    auto constant_RG_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8227));

    define_direct_property("RG"_utf16_fly_string, constant_RG_value, JS::Attribute::Enumerable);

    auto constant_RG_INTEGER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8228));

    define_direct_property("RG_INTEGER"_utf16_fly_string, constant_RG_INTEGER_value, JS::Attribute::Enumerable);

    auto constant_R8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8229));

    define_direct_property("R8"_utf16_fly_string, constant_R8_value, JS::Attribute::Enumerable);

    auto constant_RG8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x822B));

    define_direct_property("RG8"_utf16_fly_string, constant_RG8_value, JS::Attribute::Enumerable);

    auto constant_R16F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x822D));

    define_direct_property("R16F"_utf16_fly_string, constant_R16F_value, JS::Attribute::Enumerable);

    auto constant_R32F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x822E));

    define_direct_property("R32F"_utf16_fly_string, constant_R32F_value, JS::Attribute::Enumerable);

    auto constant_RG16F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x822F));

    define_direct_property("RG16F"_utf16_fly_string, constant_RG16F_value, JS::Attribute::Enumerable);

    auto constant_RG32F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8230));

    define_direct_property("RG32F"_utf16_fly_string, constant_RG32F_value, JS::Attribute::Enumerable);

    auto constant_R8I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8231));

    define_direct_property("R8I"_utf16_fly_string, constant_R8I_value, JS::Attribute::Enumerable);

    auto constant_R8UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8232));

    define_direct_property("R8UI"_utf16_fly_string, constant_R8UI_value, JS::Attribute::Enumerable);

    auto constant_R16I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8233));

    define_direct_property("R16I"_utf16_fly_string, constant_R16I_value, JS::Attribute::Enumerable);

    auto constant_R16UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8234));

    define_direct_property("R16UI"_utf16_fly_string, constant_R16UI_value, JS::Attribute::Enumerable);

    auto constant_R32I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8235));

    define_direct_property("R32I"_utf16_fly_string, constant_R32I_value, JS::Attribute::Enumerable);

    auto constant_R32UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8236));

    define_direct_property("R32UI"_utf16_fly_string, constant_R32UI_value, JS::Attribute::Enumerable);

    auto constant_RG8I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8237));

    define_direct_property("RG8I"_utf16_fly_string, constant_RG8I_value, JS::Attribute::Enumerable);

    auto constant_RG8UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8238));

    define_direct_property("RG8UI"_utf16_fly_string, constant_RG8UI_value, JS::Attribute::Enumerable);

    auto constant_RG16I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8239));

    define_direct_property("RG16I"_utf16_fly_string, constant_RG16I_value, JS::Attribute::Enumerable);

    auto constant_RG16UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x823A));

    define_direct_property("RG16UI"_utf16_fly_string, constant_RG16UI_value, JS::Attribute::Enumerable);

    auto constant_RG32I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x823B));

    define_direct_property("RG32I"_utf16_fly_string, constant_RG32I_value, JS::Attribute::Enumerable);

    auto constant_RG32UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x823C));

    define_direct_property("RG32UI"_utf16_fly_string, constant_RG32UI_value, JS::Attribute::Enumerable);

    auto constant_VERTEX_ARRAY_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x85B5));

    define_direct_property("VERTEX_ARRAY_BINDING"_utf16_fly_string, constant_VERTEX_ARRAY_BINDING_value, JS::Attribute::Enumerable);

    auto constant_R8_SNORM_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F94));

    define_direct_property("R8_SNORM"_utf16_fly_string, constant_R8_SNORM_value, JS::Attribute::Enumerable);

    auto constant_RG8_SNORM_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F95));

    define_direct_property("RG8_SNORM"_utf16_fly_string, constant_RG8_SNORM_value, JS::Attribute::Enumerable);

    auto constant_RGB8_SNORM_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F96));

    define_direct_property("RGB8_SNORM"_utf16_fly_string, constant_RGB8_SNORM_value, JS::Attribute::Enumerable);

    auto constant_RGBA8_SNORM_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F97));

    define_direct_property("RGBA8_SNORM"_utf16_fly_string, constant_RGBA8_SNORM_value, JS::Attribute::Enumerable);

    auto constant_SIGNED_NORMALIZED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F9C));

    define_direct_property("SIGNED_NORMALIZED"_utf16_fly_string, constant_SIGNED_NORMALIZED_value, JS::Attribute::Enumerable);

    auto constant_COPY_READ_BUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F36));

    define_direct_property("COPY_READ_BUFFER"_utf16_fly_string, constant_COPY_READ_BUFFER_value, JS::Attribute::Enumerable);

    auto constant_COPY_WRITE_BUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F37));

    define_direct_property("COPY_WRITE_BUFFER"_utf16_fly_string, constant_COPY_WRITE_BUFFER_value, JS::Attribute::Enumerable);

    auto constant_COPY_READ_BUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F36));

    define_direct_property("COPY_READ_BUFFER_BINDING"_utf16_fly_string, constant_COPY_READ_BUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_COPY_WRITE_BUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F37));

    define_direct_property("COPY_WRITE_BUFFER_BINDING"_utf16_fly_string, constant_COPY_WRITE_BUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A11));

    define_direct_property("UNIFORM_BUFFER"_utf16_fly_string, constant_UNIFORM_BUFFER_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A28));

    define_direct_property("UNIFORM_BUFFER_BINDING"_utf16_fly_string, constant_UNIFORM_BUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BUFFER_START_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A29));

    define_direct_property("UNIFORM_BUFFER_START"_utf16_fly_string, constant_UNIFORM_BUFFER_START_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BUFFER_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A2A));

    define_direct_property("UNIFORM_BUFFER_SIZE"_utf16_fly_string, constant_UNIFORM_BUFFER_SIZE_value, JS::Attribute::Enumerable);

    auto constant_MAX_VERTEX_UNIFORM_BLOCKS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A2B));

    define_direct_property("MAX_VERTEX_UNIFORM_BLOCKS"_utf16_fly_string, constant_MAX_VERTEX_UNIFORM_BLOCKS_value, JS::Attribute::Enumerable);

    auto constant_MAX_FRAGMENT_UNIFORM_BLOCKS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A2D));

    define_direct_property("MAX_FRAGMENT_UNIFORM_BLOCKS"_utf16_fly_string, constant_MAX_FRAGMENT_UNIFORM_BLOCKS_value, JS::Attribute::Enumerable);

    auto constant_MAX_COMBINED_UNIFORM_BLOCKS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A2E));

    define_direct_property("MAX_COMBINED_UNIFORM_BLOCKS"_utf16_fly_string, constant_MAX_COMBINED_UNIFORM_BLOCKS_value, JS::Attribute::Enumerable);

    auto constant_MAX_UNIFORM_BUFFER_BINDINGS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A2F));

    define_direct_property("MAX_UNIFORM_BUFFER_BINDINGS"_utf16_fly_string, constant_MAX_UNIFORM_BUFFER_BINDINGS_value, JS::Attribute::Enumerable);

    auto constant_MAX_UNIFORM_BLOCK_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A30));

    define_direct_property("MAX_UNIFORM_BLOCK_SIZE"_utf16_fly_string, constant_MAX_UNIFORM_BLOCK_SIZE_value, JS::Attribute::Enumerable);

    auto constant_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A31));

    define_direct_property("MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS"_utf16_fly_string, constant_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A33));

    define_direct_property("MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS"_utf16_fly_string, constant_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BUFFER_OFFSET_ALIGNMENT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A34));

    define_direct_property("UNIFORM_BUFFER_OFFSET_ALIGNMENT"_utf16_fly_string, constant_UNIFORM_BUFFER_OFFSET_ALIGNMENT_value, JS::Attribute::Enumerable);

    auto constant_ACTIVE_UNIFORM_BLOCKS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A36));

    define_direct_property("ACTIVE_UNIFORM_BLOCKS"_utf16_fly_string, constant_ACTIVE_UNIFORM_BLOCKS_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_TYPE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A37));

    define_direct_property("UNIFORM_TYPE"_utf16_fly_string, constant_UNIFORM_TYPE_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A38));

    define_direct_property("UNIFORM_SIZE"_utf16_fly_string, constant_UNIFORM_SIZE_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BLOCK_INDEX_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A3A));

    define_direct_property("UNIFORM_BLOCK_INDEX"_utf16_fly_string, constant_UNIFORM_BLOCK_INDEX_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_OFFSET_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A3B));

    define_direct_property("UNIFORM_OFFSET"_utf16_fly_string, constant_UNIFORM_OFFSET_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_ARRAY_STRIDE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A3C));

    define_direct_property("UNIFORM_ARRAY_STRIDE"_utf16_fly_string, constant_UNIFORM_ARRAY_STRIDE_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_MATRIX_STRIDE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A3D));

    define_direct_property("UNIFORM_MATRIX_STRIDE"_utf16_fly_string, constant_UNIFORM_MATRIX_STRIDE_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_IS_ROW_MAJOR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A3E));

    define_direct_property("UNIFORM_IS_ROW_MAJOR"_utf16_fly_string, constant_UNIFORM_IS_ROW_MAJOR_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BLOCK_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A3F));

    define_direct_property("UNIFORM_BLOCK_BINDING"_utf16_fly_string, constant_UNIFORM_BLOCK_BINDING_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BLOCK_DATA_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A40));

    define_direct_property("UNIFORM_BLOCK_DATA_SIZE"_utf16_fly_string, constant_UNIFORM_BLOCK_DATA_SIZE_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BLOCK_ACTIVE_UNIFORMS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A42));

    define_direct_property("UNIFORM_BLOCK_ACTIVE_UNIFORMS"_utf16_fly_string, constant_UNIFORM_BLOCK_ACTIVE_UNIFORMS_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A43));

    define_direct_property("UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES"_utf16_fly_string, constant_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A44));

    define_direct_property("UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER"_utf16_fly_string, constant_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A46));

    define_direct_property("UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER"_utf16_fly_string, constant_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER_value, JS::Attribute::Enumerable);

    auto constant_INVALID_INDEX_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0xFFFFFFFF));

    define_direct_property("INVALID_INDEX"_utf16_fly_string, constant_INVALID_INDEX_value, JS::Attribute::Enumerable);

    auto constant_MAX_VERTEX_OUTPUT_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9122));

    define_direct_property("MAX_VERTEX_OUTPUT_COMPONENTS"_utf16_fly_string, constant_MAX_VERTEX_OUTPUT_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_MAX_FRAGMENT_INPUT_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9125));

    define_direct_property("MAX_FRAGMENT_INPUT_COMPONENTS"_utf16_fly_string, constant_MAX_FRAGMENT_INPUT_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_MAX_SERVER_WAIT_TIMEOUT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9111));

    define_direct_property("MAX_SERVER_WAIT_TIMEOUT"_utf16_fly_string, constant_MAX_SERVER_WAIT_TIMEOUT_value, JS::Attribute::Enumerable);

    auto constant_OBJECT_TYPE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9112));

    define_direct_property("OBJECT_TYPE"_utf16_fly_string, constant_OBJECT_TYPE_value, JS::Attribute::Enumerable);

    auto constant_SYNC_CONDITION_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9113));

    define_direct_property("SYNC_CONDITION"_utf16_fly_string, constant_SYNC_CONDITION_value, JS::Attribute::Enumerable);

    auto constant_SYNC_STATUS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9114));

    define_direct_property("SYNC_STATUS"_utf16_fly_string, constant_SYNC_STATUS_value, JS::Attribute::Enumerable);

    auto constant_SYNC_FLAGS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9115));

    define_direct_property("SYNC_FLAGS"_utf16_fly_string, constant_SYNC_FLAGS_value, JS::Attribute::Enumerable);

    auto constant_SYNC_FENCE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9116));

    define_direct_property("SYNC_FENCE"_utf16_fly_string, constant_SYNC_FENCE_value, JS::Attribute::Enumerable);

    auto constant_SYNC_GPU_COMMANDS_COMPLETE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9117));

    define_direct_property("SYNC_GPU_COMMANDS_COMPLETE"_utf16_fly_string, constant_SYNC_GPU_COMMANDS_COMPLETE_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNALED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9118));

    define_direct_property("UNSIGNALED"_utf16_fly_string, constant_UNSIGNALED_value, JS::Attribute::Enumerable);

    auto constant_SIGNALED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9119));

    define_direct_property("SIGNALED"_utf16_fly_string, constant_SIGNALED_value, JS::Attribute::Enumerable);

    auto constant_ALREADY_SIGNALED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x911A));

    define_direct_property("ALREADY_SIGNALED"_utf16_fly_string, constant_ALREADY_SIGNALED_value, JS::Attribute::Enumerable);

    auto constant_TIMEOUT_EXPIRED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x911B));

    define_direct_property("TIMEOUT_EXPIRED"_utf16_fly_string, constant_TIMEOUT_EXPIRED_value, JS::Attribute::Enumerable);

    auto constant_CONDITION_SATISFIED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x911C));

    define_direct_property("CONDITION_SATISFIED"_utf16_fly_string, constant_CONDITION_SATISFIED_value, JS::Attribute::Enumerable);

    auto constant_WAIT_FAILED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x911D));

    define_direct_property("WAIT_FAILED"_utf16_fly_string, constant_WAIT_FAILED_value, JS::Attribute::Enumerable);

    auto constant_SYNC_FLUSH_COMMANDS_BIT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x00000001));

    define_direct_property("SYNC_FLUSH_COMMANDS_BIT"_utf16_fly_string, constant_SYNC_FLUSH_COMMANDS_BIT_value, JS::Attribute::Enumerable);

    auto constant_VERTEX_ATTRIB_ARRAY_DIVISOR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88FE));

    define_direct_property("VERTEX_ATTRIB_ARRAY_DIVISOR"_utf16_fly_string, constant_VERTEX_ATTRIB_ARRAY_DIVISOR_value, JS::Attribute::Enumerable);

    auto constant_ANY_SAMPLES_PASSED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C2F));

    define_direct_property("ANY_SAMPLES_PASSED"_utf16_fly_string, constant_ANY_SAMPLES_PASSED_value, JS::Attribute::Enumerable);

    auto constant_ANY_SAMPLES_PASSED_CONSERVATIVE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D6A));

    define_direct_property("ANY_SAMPLES_PASSED_CONSERVATIVE"_utf16_fly_string, constant_ANY_SAMPLES_PASSED_CONSERVATIVE_value, JS::Attribute::Enumerable);

    auto constant_SAMPLER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8919));

    define_direct_property("SAMPLER_BINDING"_utf16_fly_string, constant_SAMPLER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_RGB10_A2UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x906F));

    define_direct_property("RGB10_A2UI"_utf16_fly_string, constant_RGB10_A2UI_value, JS::Attribute::Enumerable);

    auto constant_INT_2_10_10_10_REV_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D9F));

    define_direct_property("INT_2_10_10_10_REV"_utf16_fly_string, constant_INT_2_10_10_10_REV_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8E22));

    define_direct_property("TRANSFORM_FEEDBACK"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_PAUSED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8E23));

    define_direct_property("TRANSFORM_FEEDBACK_PAUSED"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_PAUSED_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_ACTIVE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8E24));

    define_direct_property("TRANSFORM_FEEDBACK_ACTIVE"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_ACTIVE_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8E25));

    define_direct_property("TRANSFORM_FEEDBACK_BINDING"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_BINDING_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_IMMUTABLE_FORMAT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x912F));

    define_direct_property("TEXTURE_IMMUTABLE_FORMAT"_utf16_fly_string, constant_TEXTURE_IMMUTABLE_FORMAT_value, JS::Attribute::Enumerable);

    auto constant_MAX_ELEMENT_INDEX_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D6B));

    define_direct_property("MAX_ELEMENT_INDEX"_utf16_fly_string, constant_MAX_ELEMENT_INDEX_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_IMMUTABLE_LEVELS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x82DF));

    define_direct_property("TEXTURE_IMMUTABLE_LEVELS"_utf16_fly_string, constant_TEXTURE_IMMUTABLE_LEVELS_value, JS::Attribute::Enumerable);

    auto constant_TIMEOUT_IGNORED_value = JS::Value(static_cast<double>(-1));

    define_direct_property("TIMEOUT_IGNORED"_utf16_fly_string, constant_TIMEOUT_IGNORED_value, JS::Attribute::Enumerable);

    auto constant_MAX_CLIENT_WAIT_TIMEOUT_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9247));

    define_direct_property("MAX_CLIENT_WAIT_TIMEOUT_WEBGL"_utf16_fly_string, constant_MAX_CLIENT_WAIT_TIMEOUT_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_BUFFER_BIT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x00000100));

    define_direct_property("DEPTH_BUFFER_BIT"_utf16_fly_string, constant_DEPTH_BUFFER_BIT_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_BUFFER_BIT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x00000400));

    define_direct_property("STENCIL_BUFFER_BIT"_utf16_fly_string, constant_STENCIL_BUFFER_BIT_value, JS::Attribute::Enumerable);

    auto constant_COLOR_BUFFER_BIT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x00004000));

    define_direct_property("COLOR_BUFFER_BIT"_utf16_fly_string, constant_COLOR_BUFFER_BIT_value, JS::Attribute::Enumerable);

    auto constant_POINTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0000));

    define_direct_property("POINTS"_utf16_fly_string, constant_POINTS_value, JS::Attribute::Enumerable);

    auto constant_LINES_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0001));

    define_direct_property("LINES"_utf16_fly_string, constant_LINES_value, JS::Attribute::Enumerable);

    auto constant_LINE_LOOP_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0002));

    define_direct_property("LINE_LOOP"_utf16_fly_string, constant_LINE_LOOP_value, JS::Attribute::Enumerable);

    auto constant_LINE_STRIP_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0003));

    define_direct_property("LINE_STRIP"_utf16_fly_string, constant_LINE_STRIP_value, JS::Attribute::Enumerable);

    auto constant_TRIANGLES_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0004));

    define_direct_property("TRIANGLES"_utf16_fly_string, constant_TRIANGLES_value, JS::Attribute::Enumerable);

    auto constant_TRIANGLE_STRIP_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0005));

    define_direct_property("TRIANGLE_STRIP"_utf16_fly_string, constant_TRIANGLE_STRIP_value, JS::Attribute::Enumerable);

    auto constant_TRIANGLE_FAN_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0006));

    define_direct_property("TRIANGLE_FAN"_utf16_fly_string, constant_TRIANGLE_FAN_value, JS::Attribute::Enumerable);

    auto constant_ZERO_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0));

    define_direct_property("ZERO"_utf16_fly_string, constant_ZERO_value, JS::Attribute::Enumerable);

    auto constant_ONE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(1));

    define_direct_property("ONE"_utf16_fly_string, constant_ONE_value, JS::Attribute::Enumerable);

    auto constant_SRC_COLOR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0300));

    define_direct_property("SRC_COLOR"_utf16_fly_string, constant_SRC_COLOR_value, JS::Attribute::Enumerable);

    auto constant_ONE_MINUS_SRC_COLOR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0301));

    define_direct_property("ONE_MINUS_SRC_COLOR"_utf16_fly_string, constant_ONE_MINUS_SRC_COLOR_value, JS::Attribute::Enumerable);

    auto constant_SRC_ALPHA_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0302));

    define_direct_property("SRC_ALPHA"_utf16_fly_string, constant_SRC_ALPHA_value, JS::Attribute::Enumerable);

    auto constant_ONE_MINUS_SRC_ALPHA_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0303));

    define_direct_property("ONE_MINUS_SRC_ALPHA"_utf16_fly_string, constant_ONE_MINUS_SRC_ALPHA_value, JS::Attribute::Enumerable);

    auto constant_DST_ALPHA_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0304));

    define_direct_property("DST_ALPHA"_utf16_fly_string, constant_DST_ALPHA_value, JS::Attribute::Enumerable);

    auto constant_ONE_MINUS_DST_ALPHA_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0305));

    define_direct_property("ONE_MINUS_DST_ALPHA"_utf16_fly_string, constant_ONE_MINUS_DST_ALPHA_value, JS::Attribute::Enumerable);

    auto constant_DST_COLOR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0306));

    define_direct_property("DST_COLOR"_utf16_fly_string, constant_DST_COLOR_value, JS::Attribute::Enumerable);

    auto constant_ONE_MINUS_DST_COLOR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0307));

    define_direct_property("ONE_MINUS_DST_COLOR"_utf16_fly_string, constant_ONE_MINUS_DST_COLOR_value, JS::Attribute::Enumerable);

    auto constant_SRC_ALPHA_SATURATE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0308));

    define_direct_property("SRC_ALPHA_SATURATE"_utf16_fly_string, constant_SRC_ALPHA_SATURATE_value, JS::Attribute::Enumerable);

    auto constant_FUNC_ADD_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8006));

    define_direct_property("FUNC_ADD"_utf16_fly_string, constant_FUNC_ADD_value, JS::Attribute::Enumerable);

    auto constant_BLEND_EQUATION_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8009));

    define_direct_property("BLEND_EQUATION"_utf16_fly_string, constant_BLEND_EQUATION_value, JS::Attribute::Enumerable);

    auto constant_BLEND_EQUATION_RGB_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8009));

    define_direct_property("BLEND_EQUATION_RGB"_utf16_fly_string, constant_BLEND_EQUATION_RGB_value, JS::Attribute::Enumerable);

    auto constant_BLEND_EQUATION_ALPHA_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x883D));

    define_direct_property("BLEND_EQUATION_ALPHA"_utf16_fly_string, constant_BLEND_EQUATION_ALPHA_value, JS::Attribute::Enumerable);

    auto constant_FUNC_SUBTRACT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x800A));

    define_direct_property("FUNC_SUBTRACT"_utf16_fly_string, constant_FUNC_SUBTRACT_value, JS::Attribute::Enumerable);

    auto constant_FUNC_REVERSE_SUBTRACT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x800B));

    define_direct_property("FUNC_REVERSE_SUBTRACT"_utf16_fly_string, constant_FUNC_REVERSE_SUBTRACT_value, JS::Attribute::Enumerable);

    auto constant_BLEND_DST_RGB_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x80C8));

    define_direct_property("BLEND_DST_RGB"_utf16_fly_string, constant_BLEND_DST_RGB_value, JS::Attribute::Enumerable);

    auto constant_BLEND_SRC_RGB_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x80C9));

    define_direct_property("BLEND_SRC_RGB"_utf16_fly_string, constant_BLEND_SRC_RGB_value, JS::Attribute::Enumerable);

    auto constant_BLEND_DST_ALPHA_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x80CA));

    define_direct_property("BLEND_DST_ALPHA"_utf16_fly_string, constant_BLEND_DST_ALPHA_value, JS::Attribute::Enumerable);

    auto constant_BLEND_SRC_ALPHA_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x80CB));

    define_direct_property("BLEND_SRC_ALPHA"_utf16_fly_string, constant_BLEND_SRC_ALPHA_value, JS::Attribute::Enumerable);

    auto constant_CONSTANT_COLOR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8001));

    define_direct_property("CONSTANT_COLOR"_utf16_fly_string, constant_CONSTANT_COLOR_value, JS::Attribute::Enumerable);

    auto constant_ONE_MINUS_CONSTANT_COLOR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8002));

    define_direct_property("ONE_MINUS_CONSTANT_COLOR"_utf16_fly_string, constant_ONE_MINUS_CONSTANT_COLOR_value, JS::Attribute::Enumerable);

    auto constant_CONSTANT_ALPHA_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8003));

    define_direct_property("CONSTANT_ALPHA"_utf16_fly_string, constant_CONSTANT_ALPHA_value, JS::Attribute::Enumerable);

    auto constant_ONE_MINUS_CONSTANT_ALPHA_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8004));

    define_direct_property("ONE_MINUS_CONSTANT_ALPHA"_utf16_fly_string, constant_ONE_MINUS_CONSTANT_ALPHA_value, JS::Attribute::Enumerable);

    auto constant_BLEND_COLOR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8005));

    define_direct_property("BLEND_COLOR"_utf16_fly_string, constant_BLEND_COLOR_value, JS::Attribute::Enumerable);

    auto constant_ARRAY_BUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8892));

    define_direct_property("ARRAY_BUFFER"_utf16_fly_string, constant_ARRAY_BUFFER_value, JS::Attribute::Enumerable);

    auto constant_ELEMENT_ARRAY_BUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8893));

    define_direct_property("ELEMENT_ARRAY_BUFFER"_utf16_fly_string, constant_ELEMENT_ARRAY_BUFFER_value, JS::Attribute::Enumerable);

    auto constant_ARRAY_BUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8894));

    define_direct_property("ARRAY_BUFFER_BINDING"_utf16_fly_string, constant_ARRAY_BUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_ELEMENT_ARRAY_BUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8895));

    define_direct_property("ELEMENT_ARRAY_BUFFER_BINDING"_utf16_fly_string, constant_ELEMENT_ARRAY_BUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_STREAM_DRAW_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88E0));

    define_direct_property("STREAM_DRAW"_utf16_fly_string, constant_STREAM_DRAW_value, JS::Attribute::Enumerable);

    auto constant_STATIC_DRAW_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88E4));

    define_direct_property("STATIC_DRAW"_utf16_fly_string, constant_STATIC_DRAW_value, JS::Attribute::Enumerable);

    auto constant_DYNAMIC_DRAW_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88E8));

    define_direct_property("DYNAMIC_DRAW"_utf16_fly_string, constant_DYNAMIC_DRAW_value, JS::Attribute::Enumerable);

    auto constant_BUFFER_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8764));

    define_direct_property("BUFFER_SIZE"_utf16_fly_string, constant_BUFFER_SIZE_value, JS::Attribute::Enumerable);

    auto constant_BUFFER_USAGE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8765));

    define_direct_property("BUFFER_USAGE"_utf16_fly_string, constant_BUFFER_USAGE_value, JS::Attribute::Enumerable);

    auto constant_CURRENT_VERTEX_ATTRIB_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8626));

    define_direct_property("CURRENT_VERTEX_ATTRIB"_utf16_fly_string, constant_CURRENT_VERTEX_ATTRIB_value, JS::Attribute::Enumerable);

    auto constant_FRONT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0404));

    define_direct_property("FRONT"_utf16_fly_string, constant_FRONT_value, JS::Attribute::Enumerable);

    auto constant_BACK_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0405));

    define_direct_property("BACK"_utf16_fly_string, constant_BACK_value, JS::Attribute::Enumerable);

    auto constant_FRONT_AND_BACK_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0408));

    define_direct_property("FRONT_AND_BACK"_utf16_fly_string, constant_FRONT_AND_BACK_value, JS::Attribute::Enumerable);

    auto constant_CULL_FACE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B44));

    define_direct_property("CULL_FACE"_utf16_fly_string, constant_CULL_FACE_value, JS::Attribute::Enumerable);

    auto constant_BLEND_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0BE2));

    define_direct_property("BLEND"_utf16_fly_string, constant_BLEND_value, JS::Attribute::Enumerable);

    auto constant_DITHER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0BD0));

    define_direct_property("DITHER"_utf16_fly_string, constant_DITHER_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_TEST_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B90));

    define_direct_property("STENCIL_TEST"_utf16_fly_string, constant_STENCIL_TEST_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_TEST_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B71));

    define_direct_property("DEPTH_TEST"_utf16_fly_string, constant_DEPTH_TEST_value, JS::Attribute::Enumerable);

    auto constant_SCISSOR_TEST_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0C11));

    define_direct_property("SCISSOR_TEST"_utf16_fly_string, constant_SCISSOR_TEST_value, JS::Attribute::Enumerable);

    auto constant_POLYGON_OFFSET_FILL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8037));

    define_direct_property("POLYGON_OFFSET_FILL"_utf16_fly_string, constant_POLYGON_OFFSET_FILL_value, JS::Attribute::Enumerable);

    auto constant_SAMPLE_ALPHA_TO_COVERAGE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x809E));

    define_direct_property("SAMPLE_ALPHA_TO_COVERAGE"_utf16_fly_string, constant_SAMPLE_ALPHA_TO_COVERAGE_value, JS::Attribute::Enumerable);

    auto constant_SAMPLE_COVERAGE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x80A0));

    define_direct_property("SAMPLE_COVERAGE"_utf16_fly_string, constant_SAMPLE_COVERAGE_value, JS::Attribute::Enumerable);

    auto constant_NO_ERROR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0));

    define_direct_property("NO_ERROR"_utf16_fly_string, constant_NO_ERROR_value, JS::Attribute::Enumerable);

    auto constant_INVALID_ENUM_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0500));

    define_direct_property("INVALID_ENUM"_utf16_fly_string, constant_INVALID_ENUM_value, JS::Attribute::Enumerable);

    auto constant_INVALID_VALUE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0501));

    define_direct_property("INVALID_VALUE"_utf16_fly_string, constant_INVALID_VALUE_value, JS::Attribute::Enumerable);

    auto constant_INVALID_OPERATION_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0502));

    define_direct_property("INVALID_OPERATION"_utf16_fly_string, constant_INVALID_OPERATION_value, JS::Attribute::Enumerable);

    auto constant_OUT_OF_MEMORY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0505));

    define_direct_property("OUT_OF_MEMORY"_utf16_fly_string, constant_OUT_OF_MEMORY_value, JS::Attribute::Enumerable);

    auto constant_CW_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0900));

    define_direct_property("CW"_utf16_fly_string, constant_CW_value, JS::Attribute::Enumerable);

    auto constant_CCW_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0901));

    define_direct_property("CCW"_utf16_fly_string, constant_CCW_value, JS::Attribute::Enumerable);

    auto constant_LINE_WIDTH_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B21));

    define_direct_property("LINE_WIDTH"_utf16_fly_string, constant_LINE_WIDTH_value, JS::Attribute::Enumerable);

    auto constant_ALIASED_POINT_SIZE_RANGE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x846D));

    define_direct_property("ALIASED_POINT_SIZE_RANGE"_utf16_fly_string, constant_ALIASED_POINT_SIZE_RANGE_value, JS::Attribute::Enumerable);

    auto constant_ALIASED_LINE_WIDTH_RANGE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x846E));

    define_direct_property("ALIASED_LINE_WIDTH_RANGE"_utf16_fly_string, constant_ALIASED_LINE_WIDTH_RANGE_value, JS::Attribute::Enumerable);

    auto constant_CULL_FACE_MODE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B45));

    define_direct_property("CULL_FACE_MODE"_utf16_fly_string, constant_CULL_FACE_MODE_value, JS::Attribute::Enumerable);

    auto constant_FRONT_FACE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B46));

    define_direct_property("FRONT_FACE"_utf16_fly_string, constant_FRONT_FACE_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_RANGE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B70));

    define_direct_property("DEPTH_RANGE"_utf16_fly_string, constant_DEPTH_RANGE_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_WRITEMASK_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B72));

    define_direct_property("DEPTH_WRITEMASK"_utf16_fly_string, constant_DEPTH_WRITEMASK_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_CLEAR_VALUE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B73));

    define_direct_property("DEPTH_CLEAR_VALUE"_utf16_fly_string, constant_DEPTH_CLEAR_VALUE_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_FUNC_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B74));

    define_direct_property("DEPTH_FUNC"_utf16_fly_string, constant_DEPTH_FUNC_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_CLEAR_VALUE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B91));

    define_direct_property("STENCIL_CLEAR_VALUE"_utf16_fly_string, constant_STENCIL_CLEAR_VALUE_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_FUNC_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B92));

    define_direct_property("STENCIL_FUNC"_utf16_fly_string, constant_STENCIL_FUNC_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_FAIL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B94));

    define_direct_property("STENCIL_FAIL"_utf16_fly_string, constant_STENCIL_FAIL_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_PASS_DEPTH_FAIL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B95));

    define_direct_property("STENCIL_PASS_DEPTH_FAIL"_utf16_fly_string, constant_STENCIL_PASS_DEPTH_FAIL_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_PASS_DEPTH_PASS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B96));

    define_direct_property("STENCIL_PASS_DEPTH_PASS"_utf16_fly_string, constant_STENCIL_PASS_DEPTH_PASS_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_REF_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B97));

    define_direct_property("STENCIL_REF"_utf16_fly_string, constant_STENCIL_REF_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_VALUE_MASK_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B93));

    define_direct_property("STENCIL_VALUE_MASK"_utf16_fly_string, constant_STENCIL_VALUE_MASK_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_WRITEMASK_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0B98));

    define_direct_property("STENCIL_WRITEMASK"_utf16_fly_string, constant_STENCIL_WRITEMASK_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_BACK_FUNC_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8800));

    define_direct_property("STENCIL_BACK_FUNC"_utf16_fly_string, constant_STENCIL_BACK_FUNC_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_BACK_FAIL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8801));

    define_direct_property("STENCIL_BACK_FAIL"_utf16_fly_string, constant_STENCIL_BACK_FAIL_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_BACK_PASS_DEPTH_FAIL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8802));

    define_direct_property("STENCIL_BACK_PASS_DEPTH_FAIL"_utf16_fly_string, constant_STENCIL_BACK_PASS_DEPTH_FAIL_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_BACK_PASS_DEPTH_PASS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8803));

    define_direct_property("STENCIL_BACK_PASS_DEPTH_PASS"_utf16_fly_string, constant_STENCIL_BACK_PASS_DEPTH_PASS_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_BACK_REF_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CA3));

    define_direct_property("STENCIL_BACK_REF"_utf16_fly_string, constant_STENCIL_BACK_REF_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_BACK_VALUE_MASK_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CA4));

    define_direct_property("STENCIL_BACK_VALUE_MASK"_utf16_fly_string, constant_STENCIL_BACK_VALUE_MASK_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_BACK_WRITEMASK_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CA5));

    define_direct_property("STENCIL_BACK_WRITEMASK"_utf16_fly_string, constant_STENCIL_BACK_WRITEMASK_value, JS::Attribute::Enumerable);

    auto constant_VIEWPORT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0BA2));

    define_direct_property("VIEWPORT"_utf16_fly_string, constant_VIEWPORT_value, JS::Attribute::Enumerable);

    auto constant_SCISSOR_BOX_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0C10));

    define_direct_property("SCISSOR_BOX"_utf16_fly_string, constant_SCISSOR_BOX_value, JS::Attribute::Enumerable);

    auto constant_COLOR_CLEAR_VALUE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0C22));

    define_direct_property("COLOR_CLEAR_VALUE"_utf16_fly_string, constant_COLOR_CLEAR_VALUE_value, JS::Attribute::Enumerable);

    auto constant_COLOR_WRITEMASK_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0C23));

    define_direct_property("COLOR_WRITEMASK"_utf16_fly_string, constant_COLOR_WRITEMASK_value, JS::Attribute::Enumerable);

    auto constant_UNPACK_ALIGNMENT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0CF5));

    define_direct_property("UNPACK_ALIGNMENT"_utf16_fly_string, constant_UNPACK_ALIGNMENT_value, JS::Attribute::Enumerable);

    auto constant_PACK_ALIGNMENT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0D05));

    define_direct_property("PACK_ALIGNMENT"_utf16_fly_string, constant_PACK_ALIGNMENT_value, JS::Attribute::Enumerable);

    auto constant_MAX_TEXTURE_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0D33));

    define_direct_property("MAX_TEXTURE_SIZE"_utf16_fly_string, constant_MAX_TEXTURE_SIZE_value, JS::Attribute::Enumerable);

    auto constant_MAX_VIEWPORT_DIMS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0D3A));

    define_direct_property("MAX_VIEWPORT_DIMS"_utf16_fly_string, constant_MAX_VIEWPORT_DIMS_value, JS::Attribute::Enumerable);

    auto constant_SUBPIXEL_BITS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0D50));

    define_direct_property("SUBPIXEL_BITS"_utf16_fly_string, constant_SUBPIXEL_BITS_value, JS::Attribute::Enumerable);

    auto constant_RED_BITS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0D52));

    define_direct_property("RED_BITS"_utf16_fly_string, constant_RED_BITS_value, JS::Attribute::Enumerable);

    auto constant_GREEN_BITS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0D53));

    define_direct_property("GREEN_BITS"_utf16_fly_string, constant_GREEN_BITS_value, JS::Attribute::Enumerable);

    auto constant_BLUE_BITS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0D54));

    define_direct_property("BLUE_BITS"_utf16_fly_string, constant_BLUE_BITS_value, JS::Attribute::Enumerable);

    auto constant_ALPHA_BITS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0D55));

    define_direct_property("ALPHA_BITS"_utf16_fly_string, constant_ALPHA_BITS_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_BITS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0D56));

    define_direct_property("DEPTH_BITS"_utf16_fly_string, constant_DEPTH_BITS_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_BITS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0D57));

    define_direct_property("STENCIL_BITS"_utf16_fly_string, constant_STENCIL_BITS_value, JS::Attribute::Enumerable);

    auto constant_POLYGON_OFFSET_UNITS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x2A00));

    define_direct_property("POLYGON_OFFSET_UNITS"_utf16_fly_string, constant_POLYGON_OFFSET_UNITS_value, JS::Attribute::Enumerable);

    auto constant_POLYGON_OFFSET_FACTOR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8038));

    define_direct_property("POLYGON_OFFSET_FACTOR"_utf16_fly_string, constant_POLYGON_OFFSET_FACTOR_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_BINDING_2D_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8069));

    define_direct_property("TEXTURE_BINDING_2D"_utf16_fly_string, constant_TEXTURE_BINDING_2D_value, JS::Attribute::Enumerable);

    auto constant_SAMPLE_BUFFERS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x80A8));

    define_direct_property("SAMPLE_BUFFERS"_utf16_fly_string, constant_SAMPLE_BUFFERS_value, JS::Attribute::Enumerable);

    auto constant_SAMPLES_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x80A9));

    define_direct_property("SAMPLES"_utf16_fly_string, constant_SAMPLES_value, JS::Attribute::Enumerable);

    auto constant_SAMPLE_COVERAGE_VALUE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x80AA));

    define_direct_property("SAMPLE_COVERAGE_VALUE"_utf16_fly_string, constant_SAMPLE_COVERAGE_VALUE_value, JS::Attribute::Enumerable);

    auto constant_SAMPLE_COVERAGE_INVERT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x80AB));

    define_direct_property("SAMPLE_COVERAGE_INVERT"_utf16_fly_string, constant_SAMPLE_COVERAGE_INVERT_value, JS::Attribute::Enumerable);

    auto constant_COMPRESSED_TEXTURE_FORMATS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x86A3));

    define_direct_property("COMPRESSED_TEXTURE_FORMATS"_utf16_fly_string, constant_COMPRESSED_TEXTURE_FORMATS_value, JS::Attribute::Enumerable);

    auto constant_DONT_CARE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1100));

    define_direct_property("DONT_CARE"_utf16_fly_string, constant_DONT_CARE_value, JS::Attribute::Enumerable);

    auto constant_FASTEST_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1101));

    define_direct_property("FASTEST"_utf16_fly_string, constant_FASTEST_value, JS::Attribute::Enumerable);

    auto constant_NICEST_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1102));

    define_direct_property("NICEST"_utf16_fly_string, constant_NICEST_value, JS::Attribute::Enumerable);

    auto constant_GENERATE_MIPMAP_HINT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8192));

    define_direct_property("GENERATE_MIPMAP_HINT"_utf16_fly_string, constant_GENERATE_MIPMAP_HINT_value, JS::Attribute::Enumerable);

    auto constant_BYTE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1400));

    define_direct_property("BYTE"_utf16_fly_string, constant_BYTE_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_BYTE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1401));

    define_direct_property("UNSIGNED_BYTE"_utf16_fly_string, constant_UNSIGNED_BYTE_value, JS::Attribute::Enumerable);

    auto constant_SHORT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1402));

    define_direct_property("SHORT"_utf16_fly_string, constant_SHORT_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_SHORT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1403));

    define_direct_property("UNSIGNED_SHORT"_utf16_fly_string, constant_UNSIGNED_SHORT_value, JS::Attribute::Enumerable);

    auto constant_INT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1404));

    define_direct_property("INT"_utf16_fly_string, constant_INT_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1405));

    define_direct_property("UNSIGNED_INT"_utf16_fly_string, constant_UNSIGNED_INT_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1406));

    define_direct_property("FLOAT"_utf16_fly_string, constant_FLOAT_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_COMPONENT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1902));

    define_direct_property("DEPTH_COMPONENT"_utf16_fly_string, constant_DEPTH_COMPONENT_value, JS::Attribute::Enumerable);

    auto constant_ALPHA_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1906));

    define_direct_property("ALPHA"_utf16_fly_string, constant_ALPHA_value, JS::Attribute::Enumerable);

    auto constant_RGB_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1907));

    define_direct_property("RGB"_utf16_fly_string, constant_RGB_value, JS::Attribute::Enumerable);

    auto constant_RGBA_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1908));

    define_direct_property("RGBA"_utf16_fly_string, constant_RGBA_value, JS::Attribute::Enumerable);

    auto constant_LUMINANCE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1909));

    define_direct_property("LUMINANCE"_utf16_fly_string, constant_LUMINANCE_value, JS::Attribute::Enumerable);

    auto constant_LUMINANCE_ALPHA_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x190A));

    define_direct_property("LUMINANCE_ALPHA"_utf16_fly_string, constant_LUMINANCE_ALPHA_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_SHORT_4_4_4_4_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8033));

    define_direct_property("UNSIGNED_SHORT_4_4_4_4"_utf16_fly_string, constant_UNSIGNED_SHORT_4_4_4_4_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_SHORT_5_5_5_1_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8034));

    define_direct_property("UNSIGNED_SHORT_5_5_5_1"_utf16_fly_string, constant_UNSIGNED_SHORT_5_5_5_1_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_SHORT_5_6_5_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8363));

    define_direct_property("UNSIGNED_SHORT_5_6_5"_utf16_fly_string, constant_UNSIGNED_SHORT_5_6_5_value, JS::Attribute::Enumerable);

    auto constant_FRAGMENT_SHADER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B30));

    define_direct_property("FRAGMENT_SHADER"_utf16_fly_string, constant_FRAGMENT_SHADER_value, JS::Attribute::Enumerable);

    auto constant_VERTEX_SHADER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B31));

    define_direct_property("VERTEX_SHADER"_utf16_fly_string, constant_VERTEX_SHADER_value, JS::Attribute::Enumerable);

    auto constant_MAX_VERTEX_ATTRIBS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8869));

    define_direct_property("MAX_VERTEX_ATTRIBS"_utf16_fly_string, constant_MAX_VERTEX_ATTRIBS_value, JS::Attribute::Enumerable);

    auto constant_MAX_VERTEX_UNIFORM_VECTORS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DFB));

    define_direct_property("MAX_VERTEX_UNIFORM_VECTORS"_utf16_fly_string, constant_MAX_VERTEX_UNIFORM_VECTORS_value, JS::Attribute::Enumerable);

    auto constant_MAX_VARYING_VECTORS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DFC));

    define_direct_property("MAX_VARYING_VECTORS"_utf16_fly_string, constant_MAX_VARYING_VECTORS_value, JS::Attribute::Enumerable);

    auto constant_MAX_COMBINED_TEXTURE_IMAGE_UNITS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B4D));

    define_direct_property("MAX_COMBINED_TEXTURE_IMAGE_UNITS"_utf16_fly_string, constant_MAX_COMBINED_TEXTURE_IMAGE_UNITS_value, JS::Attribute::Enumerable);

    auto constant_MAX_VERTEX_TEXTURE_IMAGE_UNITS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B4C));

    define_direct_property("MAX_VERTEX_TEXTURE_IMAGE_UNITS"_utf16_fly_string, constant_MAX_VERTEX_TEXTURE_IMAGE_UNITS_value, JS::Attribute::Enumerable);

    auto constant_MAX_TEXTURE_IMAGE_UNITS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8872));

    define_direct_property("MAX_TEXTURE_IMAGE_UNITS"_utf16_fly_string, constant_MAX_TEXTURE_IMAGE_UNITS_value, JS::Attribute::Enumerable);

    auto constant_MAX_FRAGMENT_UNIFORM_VECTORS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DFD));

    define_direct_property("MAX_FRAGMENT_UNIFORM_VECTORS"_utf16_fly_string, constant_MAX_FRAGMENT_UNIFORM_VECTORS_value, JS::Attribute::Enumerable);

    auto constant_SHADER_TYPE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B4F));

    define_direct_property("SHADER_TYPE"_utf16_fly_string, constant_SHADER_TYPE_value, JS::Attribute::Enumerable);

    auto constant_DELETE_STATUS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B80));

    define_direct_property("DELETE_STATUS"_utf16_fly_string, constant_DELETE_STATUS_value, JS::Attribute::Enumerable);

    auto constant_LINK_STATUS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B82));

    define_direct_property("LINK_STATUS"_utf16_fly_string, constant_LINK_STATUS_value, JS::Attribute::Enumerable);

    auto constant_VALIDATE_STATUS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B83));

    define_direct_property("VALIDATE_STATUS"_utf16_fly_string, constant_VALIDATE_STATUS_value, JS::Attribute::Enumerable);

    auto constant_ATTACHED_SHADERS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B85));

    define_direct_property("ATTACHED_SHADERS"_utf16_fly_string, constant_ATTACHED_SHADERS_value, JS::Attribute::Enumerable);

    auto constant_ACTIVE_UNIFORMS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B86));

    define_direct_property("ACTIVE_UNIFORMS"_utf16_fly_string, constant_ACTIVE_UNIFORMS_value, JS::Attribute::Enumerable);

    auto constant_ACTIVE_ATTRIBUTES_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B89));

    define_direct_property("ACTIVE_ATTRIBUTES"_utf16_fly_string, constant_ACTIVE_ATTRIBUTES_value, JS::Attribute::Enumerable);

    auto constant_SHADING_LANGUAGE_VERSION_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B8C));

    define_direct_property("SHADING_LANGUAGE_VERSION"_utf16_fly_string, constant_SHADING_LANGUAGE_VERSION_value, JS::Attribute::Enumerable);

    auto constant_CURRENT_PROGRAM_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B8D));

    define_direct_property("CURRENT_PROGRAM"_utf16_fly_string, constant_CURRENT_PROGRAM_value, JS::Attribute::Enumerable);

    auto constant_NEVER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0200));

    define_direct_property("NEVER"_utf16_fly_string, constant_NEVER_value, JS::Attribute::Enumerable);

    auto constant_LESS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0201));

    define_direct_property("LESS"_utf16_fly_string, constant_LESS_value, JS::Attribute::Enumerable);

    auto constant_EQUAL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0202));

    define_direct_property("EQUAL"_utf16_fly_string, constant_EQUAL_value, JS::Attribute::Enumerable);

    auto constant_LEQUAL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0203));

    define_direct_property("LEQUAL"_utf16_fly_string, constant_LEQUAL_value, JS::Attribute::Enumerable);

    auto constant_GREATER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0204));

    define_direct_property("GREATER"_utf16_fly_string, constant_GREATER_value, JS::Attribute::Enumerable);

    auto constant_NOTEQUAL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0205));

    define_direct_property("NOTEQUAL"_utf16_fly_string, constant_NOTEQUAL_value, JS::Attribute::Enumerable);

    auto constant_GEQUAL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0206));

    define_direct_property("GEQUAL"_utf16_fly_string, constant_GEQUAL_value, JS::Attribute::Enumerable);

    auto constant_ALWAYS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0207));

    define_direct_property("ALWAYS"_utf16_fly_string, constant_ALWAYS_value, JS::Attribute::Enumerable);

    auto constant_KEEP_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1E00));

    define_direct_property("KEEP"_utf16_fly_string, constant_KEEP_value, JS::Attribute::Enumerable);

    auto constant_REPLACE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1E01));

    define_direct_property("REPLACE"_utf16_fly_string, constant_REPLACE_value, JS::Attribute::Enumerable);

    auto constant_INCR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1E02));

    define_direct_property("INCR"_utf16_fly_string, constant_INCR_value, JS::Attribute::Enumerable);

    auto constant_DECR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1E03));

    define_direct_property("DECR"_utf16_fly_string, constant_DECR_value, JS::Attribute::Enumerable);

    auto constant_INVERT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x150A));

    define_direct_property("INVERT"_utf16_fly_string, constant_INVERT_value, JS::Attribute::Enumerable);

    auto constant_INCR_WRAP_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8507));

    define_direct_property("INCR_WRAP"_utf16_fly_string, constant_INCR_WRAP_value, JS::Attribute::Enumerable);

    auto constant_DECR_WRAP_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8508));

    define_direct_property("DECR_WRAP"_utf16_fly_string, constant_DECR_WRAP_value, JS::Attribute::Enumerable);

    auto constant_VENDOR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1F00));

    define_direct_property("VENDOR"_utf16_fly_string, constant_VENDOR_value, JS::Attribute::Enumerable);

    auto constant_RENDERER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1F01));

    define_direct_property("RENDERER"_utf16_fly_string, constant_RENDERER_value, JS::Attribute::Enumerable);

    auto constant_VERSION_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1F02));

    define_direct_property("VERSION"_utf16_fly_string, constant_VERSION_value, JS::Attribute::Enumerable);

    auto constant_NEAREST_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x2600));

    define_direct_property("NEAREST"_utf16_fly_string, constant_NEAREST_value, JS::Attribute::Enumerable);

    auto constant_LINEAR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x2601));

    define_direct_property("LINEAR"_utf16_fly_string, constant_LINEAR_value, JS::Attribute::Enumerable);

    auto constant_NEAREST_MIPMAP_NEAREST_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x2700));

    define_direct_property("NEAREST_MIPMAP_NEAREST"_utf16_fly_string, constant_NEAREST_MIPMAP_NEAREST_value, JS::Attribute::Enumerable);

    auto constant_LINEAR_MIPMAP_NEAREST_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x2701));

    define_direct_property("LINEAR_MIPMAP_NEAREST"_utf16_fly_string, constant_LINEAR_MIPMAP_NEAREST_value, JS::Attribute::Enumerable);

    auto constant_NEAREST_MIPMAP_LINEAR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x2702));

    define_direct_property("NEAREST_MIPMAP_LINEAR"_utf16_fly_string, constant_NEAREST_MIPMAP_LINEAR_value, JS::Attribute::Enumerable);

    auto constant_LINEAR_MIPMAP_LINEAR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x2703));

    define_direct_property("LINEAR_MIPMAP_LINEAR"_utf16_fly_string, constant_LINEAR_MIPMAP_LINEAR_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_MAG_FILTER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x2800));

    define_direct_property("TEXTURE_MAG_FILTER"_utf16_fly_string, constant_TEXTURE_MAG_FILTER_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_MIN_FILTER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x2801));

    define_direct_property("TEXTURE_MIN_FILTER"_utf16_fly_string, constant_TEXTURE_MIN_FILTER_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_WRAP_S_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x2802));

    define_direct_property("TEXTURE_WRAP_S"_utf16_fly_string, constant_TEXTURE_WRAP_S_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_WRAP_T_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x2803));

    define_direct_property("TEXTURE_WRAP_T"_utf16_fly_string, constant_TEXTURE_WRAP_T_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_2D_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0DE1));

    define_direct_property("TEXTURE_2D"_utf16_fly_string, constant_TEXTURE_2D_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1702));

    define_direct_property("TEXTURE"_utf16_fly_string, constant_TEXTURE_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_CUBE_MAP_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8513));

    define_direct_property("TEXTURE_CUBE_MAP"_utf16_fly_string, constant_TEXTURE_CUBE_MAP_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_BINDING_CUBE_MAP_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8514));

    define_direct_property("TEXTURE_BINDING_CUBE_MAP"_utf16_fly_string, constant_TEXTURE_BINDING_CUBE_MAP_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_CUBE_MAP_POSITIVE_X_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8515));

    define_direct_property("TEXTURE_CUBE_MAP_POSITIVE_X"_utf16_fly_string, constant_TEXTURE_CUBE_MAP_POSITIVE_X_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_CUBE_MAP_NEGATIVE_X_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8516));

    define_direct_property("TEXTURE_CUBE_MAP_NEGATIVE_X"_utf16_fly_string, constant_TEXTURE_CUBE_MAP_NEGATIVE_X_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_CUBE_MAP_POSITIVE_Y_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8517));

    define_direct_property("TEXTURE_CUBE_MAP_POSITIVE_Y"_utf16_fly_string, constant_TEXTURE_CUBE_MAP_POSITIVE_Y_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_CUBE_MAP_NEGATIVE_Y_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8518));

    define_direct_property("TEXTURE_CUBE_MAP_NEGATIVE_Y"_utf16_fly_string, constant_TEXTURE_CUBE_MAP_NEGATIVE_Y_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_CUBE_MAP_POSITIVE_Z_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8519));

    define_direct_property("TEXTURE_CUBE_MAP_POSITIVE_Z"_utf16_fly_string, constant_TEXTURE_CUBE_MAP_POSITIVE_Z_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_CUBE_MAP_NEGATIVE_Z_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x851A));

    define_direct_property("TEXTURE_CUBE_MAP_NEGATIVE_Z"_utf16_fly_string, constant_TEXTURE_CUBE_MAP_NEGATIVE_Z_value, JS::Attribute::Enumerable);

    auto constant_MAX_CUBE_MAP_TEXTURE_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x851C));

    define_direct_property("MAX_CUBE_MAP_TEXTURE_SIZE"_utf16_fly_string, constant_MAX_CUBE_MAP_TEXTURE_SIZE_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE0_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84C0));

    define_direct_property("TEXTURE0"_utf16_fly_string, constant_TEXTURE0_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE1_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84C1));

    define_direct_property("TEXTURE1"_utf16_fly_string, constant_TEXTURE1_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE2_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84C2));

    define_direct_property("TEXTURE2"_utf16_fly_string, constant_TEXTURE2_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE3_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84C3));

    define_direct_property("TEXTURE3"_utf16_fly_string, constant_TEXTURE3_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE4_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84C4));

    define_direct_property("TEXTURE4"_utf16_fly_string, constant_TEXTURE4_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE5_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84C5));

    define_direct_property("TEXTURE5"_utf16_fly_string, constant_TEXTURE5_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE6_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84C6));

    define_direct_property("TEXTURE6"_utf16_fly_string, constant_TEXTURE6_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE7_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84C7));

    define_direct_property("TEXTURE7"_utf16_fly_string, constant_TEXTURE7_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84C8));

    define_direct_property("TEXTURE8"_utf16_fly_string, constant_TEXTURE8_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE9_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84C9));

    define_direct_property("TEXTURE9"_utf16_fly_string, constant_TEXTURE9_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE10_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84CA));

    define_direct_property("TEXTURE10"_utf16_fly_string, constant_TEXTURE10_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE11_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84CB));

    define_direct_property("TEXTURE11"_utf16_fly_string, constant_TEXTURE11_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE12_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84CC));

    define_direct_property("TEXTURE12"_utf16_fly_string, constant_TEXTURE12_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE13_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84CD));

    define_direct_property("TEXTURE13"_utf16_fly_string, constant_TEXTURE13_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE14_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84CE));

    define_direct_property("TEXTURE14"_utf16_fly_string, constant_TEXTURE14_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE15_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84CF));

    define_direct_property("TEXTURE15"_utf16_fly_string, constant_TEXTURE15_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE16_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84D0));

    define_direct_property("TEXTURE16"_utf16_fly_string, constant_TEXTURE16_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE17_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84D1));

    define_direct_property("TEXTURE17"_utf16_fly_string, constant_TEXTURE17_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE18_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84D2));

    define_direct_property("TEXTURE18"_utf16_fly_string, constant_TEXTURE18_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE19_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84D3));

    define_direct_property("TEXTURE19"_utf16_fly_string, constant_TEXTURE19_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE20_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84D4));

    define_direct_property("TEXTURE20"_utf16_fly_string, constant_TEXTURE20_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE21_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84D5));

    define_direct_property("TEXTURE21"_utf16_fly_string, constant_TEXTURE21_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE22_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84D6));

    define_direct_property("TEXTURE22"_utf16_fly_string, constant_TEXTURE22_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE23_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84D7));

    define_direct_property("TEXTURE23"_utf16_fly_string, constant_TEXTURE23_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE24_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84D8));

    define_direct_property("TEXTURE24"_utf16_fly_string, constant_TEXTURE24_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE25_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84D9));

    define_direct_property("TEXTURE25"_utf16_fly_string, constant_TEXTURE25_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE26_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84DA));

    define_direct_property("TEXTURE26"_utf16_fly_string, constant_TEXTURE26_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE27_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84DB));

    define_direct_property("TEXTURE27"_utf16_fly_string, constant_TEXTURE27_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE28_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84DC));

    define_direct_property("TEXTURE28"_utf16_fly_string, constant_TEXTURE28_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE29_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84DD));

    define_direct_property("TEXTURE29"_utf16_fly_string, constant_TEXTURE29_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE30_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84DE));

    define_direct_property("TEXTURE30"_utf16_fly_string, constant_TEXTURE30_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE31_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84DF));

    define_direct_property("TEXTURE31"_utf16_fly_string, constant_TEXTURE31_value, JS::Attribute::Enumerable);

    auto constant_ACTIVE_TEXTURE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84E0));

    define_direct_property("ACTIVE_TEXTURE"_utf16_fly_string, constant_ACTIVE_TEXTURE_value, JS::Attribute::Enumerable);

    auto constant_REPEAT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x2901));

    define_direct_property("REPEAT"_utf16_fly_string, constant_REPEAT_value, JS::Attribute::Enumerable);

    auto constant_CLAMP_TO_EDGE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x812F));

    define_direct_property("CLAMP_TO_EDGE"_utf16_fly_string, constant_CLAMP_TO_EDGE_value, JS::Attribute::Enumerable);

    auto constant_MIRRORED_REPEAT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8370));

    define_direct_property("MIRRORED_REPEAT"_utf16_fly_string, constant_MIRRORED_REPEAT_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_VEC2_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B50));

    define_direct_property("FLOAT_VEC2"_utf16_fly_string, constant_FLOAT_VEC2_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_VEC3_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B51));

    define_direct_property("FLOAT_VEC3"_utf16_fly_string, constant_FLOAT_VEC3_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_VEC4_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B52));

    define_direct_property("FLOAT_VEC4"_utf16_fly_string, constant_FLOAT_VEC4_value, JS::Attribute::Enumerable);

    auto constant_INT_VEC2_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B53));

    define_direct_property("INT_VEC2"_utf16_fly_string, constant_INT_VEC2_value, JS::Attribute::Enumerable);

    auto constant_INT_VEC3_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B54));

    define_direct_property("INT_VEC3"_utf16_fly_string, constant_INT_VEC3_value, JS::Attribute::Enumerable);

    auto constant_INT_VEC4_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B55));

    define_direct_property("INT_VEC4"_utf16_fly_string, constant_INT_VEC4_value, JS::Attribute::Enumerable);

    auto constant_BOOL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B56));

    define_direct_property("BOOL"_utf16_fly_string, constant_BOOL_value, JS::Attribute::Enumerable);

    auto constant_BOOL_VEC2_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B57));

    define_direct_property("BOOL_VEC2"_utf16_fly_string, constant_BOOL_VEC2_value, JS::Attribute::Enumerable);

    auto constant_BOOL_VEC3_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B58));

    define_direct_property("BOOL_VEC3"_utf16_fly_string, constant_BOOL_VEC3_value, JS::Attribute::Enumerable);

    auto constant_BOOL_VEC4_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B59));

    define_direct_property("BOOL_VEC4"_utf16_fly_string, constant_BOOL_VEC4_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_MAT2_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B5A));

    define_direct_property("FLOAT_MAT2"_utf16_fly_string, constant_FLOAT_MAT2_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_MAT3_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B5B));

    define_direct_property("FLOAT_MAT3"_utf16_fly_string, constant_FLOAT_MAT3_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_MAT4_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B5C));

    define_direct_property("FLOAT_MAT4"_utf16_fly_string, constant_FLOAT_MAT4_value, JS::Attribute::Enumerable);

    auto constant_SAMPLER_2D_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B5E));

    define_direct_property("SAMPLER_2D"_utf16_fly_string, constant_SAMPLER_2D_value, JS::Attribute::Enumerable);

    auto constant_SAMPLER_CUBE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B60));

    define_direct_property("SAMPLER_CUBE"_utf16_fly_string, constant_SAMPLER_CUBE_value, JS::Attribute::Enumerable);

    auto constant_VERTEX_ATTRIB_ARRAY_ENABLED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8622));

    define_direct_property("VERTEX_ATTRIB_ARRAY_ENABLED"_utf16_fly_string, constant_VERTEX_ATTRIB_ARRAY_ENABLED_value, JS::Attribute::Enumerable);

    auto constant_VERTEX_ATTRIB_ARRAY_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8623));

    define_direct_property("VERTEX_ATTRIB_ARRAY_SIZE"_utf16_fly_string, constant_VERTEX_ATTRIB_ARRAY_SIZE_value, JS::Attribute::Enumerable);

    auto constant_VERTEX_ATTRIB_ARRAY_STRIDE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8624));

    define_direct_property("VERTEX_ATTRIB_ARRAY_STRIDE"_utf16_fly_string, constant_VERTEX_ATTRIB_ARRAY_STRIDE_value, JS::Attribute::Enumerable);

    auto constant_VERTEX_ATTRIB_ARRAY_TYPE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8625));

    define_direct_property("VERTEX_ATTRIB_ARRAY_TYPE"_utf16_fly_string, constant_VERTEX_ATTRIB_ARRAY_TYPE_value, JS::Attribute::Enumerable);

    auto constant_VERTEX_ATTRIB_ARRAY_NORMALIZED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x886A));

    define_direct_property("VERTEX_ATTRIB_ARRAY_NORMALIZED"_utf16_fly_string, constant_VERTEX_ATTRIB_ARRAY_NORMALIZED_value, JS::Attribute::Enumerable);

    auto constant_VERTEX_ATTRIB_ARRAY_POINTER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8645));

    define_direct_property("VERTEX_ATTRIB_ARRAY_POINTER"_utf16_fly_string, constant_VERTEX_ATTRIB_ARRAY_POINTER_value, JS::Attribute::Enumerable);

    auto constant_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x889F));

    define_direct_property("VERTEX_ATTRIB_ARRAY_BUFFER_BINDING"_utf16_fly_string, constant_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_IMPLEMENTATION_COLOR_READ_TYPE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B9A));

    define_direct_property("IMPLEMENTATION_COLOR_READ_TYPE"_utf16_fly_string, constant_IMPLEMENTATION_COLOR_READ_TYPE_value, JS::Attribute::Enumerable);

    auto constant_IMPLEMENTATION_COLOR_READ_FORMAT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B9B));

    define_direct_property("IMPLEMENTATION_COLOR_READ_FORMAT"_utf16_fly_string, constant_IMPLEMENTATION_COLOR_READ_FORMAT_value, JS::Attribute::Enumerable);

    auto constant_COMPILE_STATUS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B81));

    define_direct_property("COMPILE_STATUS"_utf16_fly_string, constant_COMPILE_STATUS_value, JS::Attribute::Enumerable);

    auto constant_LOW_FLOAT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DF0));

    define_direct_property("LOW_FLOAT"_utf16_fly_string, constant_LOW_FLOAT_value, JS::Attribute::Enumerable);

    auto constant_MEDIUM_FLOAT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DF1));

    define_direct_property("MEDIUM_FLOAT"_utf16_fly_string, constant_MEDIUM_FLOAT_value, JS::Attribute::Enumerable);

    auto constant_HIGH_FLOAT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DF2));

    define_direct_property("HIGH_FLOAT"_utf16_fly_string, constant_HIGH_FLOAT_value, JS::Attribute::Enumerable);

    auto constant_LOW_INT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DF3));

    define_direct_property("LOW_INT"_utf16_fly_string, constant_LOW_INT_value, JS::Attribute::Enumerable);

    auto constant_MEDIUM_INT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DF4));

    define_direct_property("MEDIUM_INT"_utf16_fly_string, constant_MEDIUM_INT_value, JS::Attribute::Enumerable);

    auto constant_HIGH_INT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DF5));

    define_direct_property("HIGH_INT"_utf16_fly_string, constant_HIGH_INT_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D40));

    define_direct_property("FRAMEBUFFER"_utf16_fly_string, constant_FRAMEBUFFER_value, JS::Attribute::Enumerable);

    auto constant_RENDERBUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D41));

    define_direct_property("RENDERBUFFER"_utf16_fly_string, constant_RENDERBUFFER_value, JS::Attribute::Enumerable);

    auto constant_RGBA4_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8056));

    define_direct_property("RGBA4"_utf16_fly_string, constant_RGBA4_value, JS::Attribute::Enumerable);

    auto constant_RGB5_A1_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8057));

    define_direct_property("RGB5_A1"_utf16_fly_string, constant_RGB5_A1_value, JS::Attribute::Enumerable);

    auto constant_RGBA8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8058));

    define_direct_property("RGBA8"_utf16_fly_string, constant_RGBA8_value, JS::Attribute::Enumerable);

    auto constant_RGB565_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D62));

    define_direct_property("RGB565"_utf16_fly_string, constant_RGB565_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_COMPONENT16_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x81A5));

    define_direct_property("DEPTH_COMPONENT16"_utf16_fly_string, constant_DEPTH_COMPONENT16_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_INDEX8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D48));

    define_direct_property("STENCIL_INDEX8"_utf16_fly_string, constant_STENCIL_INDEX8_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_STENCIL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84F9));

    define_direct_property("DEPTH_STENCIL"_utf16_fly_string, constant_DEPTH_STENCIL_value, JS::Attribute::Enumerable);

    auto constant_RENDERBUFFER_WIDTH_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D42));

    define_direct_property("RENDERBUFFER_WIDTH"_utf16_fly_string, constant_RENDERBUFFER_WIDTH_value, JS::Attribute::Enumerable);

    auto constant_RENDERBUFFER_HEIGHT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D43));

    define_direct_property("RENDERBUFFER_HEIGHT"_utf16_fly_string, constant_RENDERBUFFER_HEIGHT_value, JS::Attribute::Enumerable);

    auto constant_RENDERBUFFER_INTERNAL_FORMAT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D44));

    define_direct_property("RENDERBUFFER_INTERNAL_FORMAT"_utf16_fly_string, constant_RENDERBUFFER_INTERNAL_FORMAT_value, JS::Attribute::Enumerable);

    auto constant_RENDERBUFFER_RED_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D50));

    define_direct_property("RENDERBUFFER_RED_SIZE"_utf16_fly_string, constant_RENDERBUFFER_RED_SIZE_value, JS::Attribute::Enumerable);

    auto constant_RENDERBUFFER_GREEN_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D51));

    define_direct_property("RENDERBUFFER_GREEN_SIZE"_utf16_fly_string, constant_RENDERBUFFER_GREEN_SIZE_value, JS::Attribute::Enumerable);

    auto constant_RENDERBUFFER_BLUE_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D52));

    define_direct_property("RENDERBUFFER_BLUE_SIZE"_utf16_fly_string, constant_RENDERBUFFER_BLUE_SIZE_value, JS::Attribute::Enumerable);

    auto constant_RENDERBUFFER_ALPHA_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D53));

    define_direct_property("RENDERBUFFER_ALPHA_SIZE"_utf16_fly_string, constant_RENDERBUFFER_ALPHA_SIZE_value, JS::Attribute::Enumerable);

    auto constant_RENDERBUFFER_DEPTH_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D54));

    define_direct_property("RENDERBUFFER_DEPTH_SIZE"_utf16_fly_string, constant_RENDERBUFFER_DEPTH_SIZE_value, JS::Attribute::Enumerable);

    auto constant_RENDERBUFFER_STENCIL_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D55));

    define_direct_property("RENDERBUFFER_STENCIL_SIZE"_utf16_fly_string, constant_RENDERBUFFER_STENCIL_SIZE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CD0));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CD1));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_OBJECT_NAME"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CD2));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CD3));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT0_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE0));

    define_direct_property("COLOR_ATTACHMENT0"_utf16_fly_string, constant_COLOR_ATTACHMENT0_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_ATTACHMENT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D00));

    define_direct_property("DEPTH_ATTACHMENT"_utf16_fly_string, constant_DEPTH_ATTACHMENT_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_ATTACHMENT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D20));

    define_direct_property("STENCIL_ATTACHMENT"_utf16_fly_string, constant_STENCIL_ATTACHMENT_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_STENCIL_ATTACHMENT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x821A));

    define_direct_property("DEPTH_STENCIL_ATTACHMENT"_utf16_fly_string, constant_DEPTH_STENCIL_ATTACHMENT_value, JS::Attribute::Enumerable);

    auto constant_NONE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0));

    define_direct_property("NONE"_utf16_fly_string, constant_NONE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_COMPLETE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CD5));

    define_direct_property("FRAMEBUFFER_COMPLETE"_utf16_fly_string, constant_FRAMEBUFFER_COMPLETE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CD6));

    define_direct_property("FRAMEBUFFER_INCOMPLETE_ATTACHMENT"_utf16_fly_string, constant_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CD7));

    define_direct_property("FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"_utf16_fly_string, constant_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CD9));

    define_direct_property("FRAMEBUFFER_INCOMPLETE_DIMENSIONS"_utf16_fly_string, constant_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_UNSUPPORTED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CDD));

    define_direct_property("FRAMEBUFFER_UNSUPPORTED"_utf16_fly_string, constant_FRAMEBUFFER_UNSUPPORTED_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CA6));

    define_direct_property("FRAMEBUFFER_BINDING"_utf16_fly_string, constant_FRAMEBUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_RENDERBUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CA7));

    define_direct_property("RENDERBUFFER_BINDING"_utf16_fly_string, constant_RENDERBUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_MAX_RENDERBUFFER_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84E8));

    define_direct_property("MAX_RENDERBUFFER_SIZE"_utf16_fly_string, constant_MAX_RENDERBUFFER_SIZE_value, JS::Attribute::Enumerable);

    auto constant_INVALID_FRAMEBUFFER_OPERATION_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0506));

    define_direct_property("INVALID_FRAMEBUFFER_OPERATION"_utf16_fly_string, constant_INVALID_FRAMEBUFFER_OPERATION_value, JS::Attribute::Enumerable);

    auto constant_UNPACK_FLIP_Y_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9240));

    define_direct_property("UNPACK_FLIP_Y_WEBGL"_utf16_fly_string, constant_UNPACK_FLIP_Y_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_UNPACK_PREMULTIPLY_ALPHA_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9241));

    define_direct_property("UNPACK_PREMULTIPLY_ALPHA_WEBGL"_utf16_fly_string, constant_UNPACK_PREMULTIPLY_ALPHA_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_CONTEXT_LOST_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9242));

    define_direct_property("CONTEXT_LOST_WEBGL"_utf16_fly_string, constant_CONTEXT_LOST_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_UNPACK_COLORSPACE_CONVERSION_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9243));

    define_direct_property("UNPACK_COLORSPACE_CONVERSION_WEBGL"_utf16_fly_string, constant_UNPACK_COLORSPACE_CONVERSION_WEBGL_value, JS::Attribute::Enumerable);

    auto constant_BROWSER_DEFAULT_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9244));

    define_direct_property("BROWSER_DEFAULT_WEBGL"_utf16_fly_string, constant_BROWSER_DEFAULT_WEBGL_value, JS::Attribute::Enumerable);

}

} // namespace Web::Bindings
