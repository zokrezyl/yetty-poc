
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
#include <LibWeb/Bindings/PointerEventConstructor.h>
#include <LibWeb/Bindings/PointerEventPrototype.h>
#include <LibWeb/Bindings/ExceptionOrUtils.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/HTML/WindowProxy.h>
#include <LibWeb/WebIDL/AbstractOperations.h>
#include <LibWeb/WebIDL/Buffers.h>
#include <LibWeb/WebIDL/CallbackType.h>
#include <LibWeb/WebIDL/OverloadResolution.h>
#include <LibWeb/WebIDL/Tracing.h>
#include <LibWeb/WebIDL/Types.h>

#if __has_include(<LibWeb/Bindings/MouseEventPrototype.h>)
#    include <LibWeb/Bindings/MouseEventPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/UIEvents/PointerEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/UIEvents/MouseEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/UIEvents/EventModifier.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/UIEvents/UIEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Window.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CookieStore/CookieStore.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/MediaQueryList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/Screen.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/VisualViewport.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Document.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CustomElements/CustomElementRegistry.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/History.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Navigation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Navigator.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/RequestIdleCallback/IdleRequest.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Speech/SpeechSynthesis.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorkerGlobalScope.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/ScreenOrientation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/DocumentTimeline.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/FontFaceSet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StyleSheetList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/CDATASection.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Comment.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DOMImplementation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DocumentFragment.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DocumentType.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Element.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/HTMLCollection.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Node.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/NodeFilter.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/NodeIterator.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/NodeList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/ProcessingInstruction.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Range.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Text.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/TreeWalker.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLAllCollection.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLHeadElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLScriptElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Location.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Selection/Selection.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedHTML.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ViewTransition/ViewTransition.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XPath/XPathResult.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XPath/XPathExpression.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Request.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Response.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HighResolutionTime/Performance.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ImageBitmap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBFactory.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/CacheStorage.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedTypePolicyFactory.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/RequestIdleCallback/IdleDeadline.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Speech/SpeechSynthesisUtterance.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Speech/SpeechSynthesisVoice.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/AnimationTimeline.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/FontFace.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleSheet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/CharacterData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/XMLDocument.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/Animatable.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StylePropertyMapReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Attr.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DOMTokenList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/NamedNodeMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/ShadowRoot.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRectList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLSlotElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedTypePolicy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbstractRange.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/DOMStringMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ElementInternals.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Scripting/Fetching.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedScriptURL.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/StorageAPI/StorageManager.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Crypto/SubtleCrypto.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Headers.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/NavigationTiming/PerformanceNavigation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/NavigationTiming/PerformanceTiming.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/PerformanceTimeline/PerformanceEntry.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/UserTiming/PerformanceMark.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/UserTiming/PerformanceMeasure.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ImageData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CanvasRenderingContext2D.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSNumericValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRule.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRuleList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/MediaList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StyleSheet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/Animation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/KeyframeEffect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRectReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedScript.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleProperties.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StylePropertyMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CustomElements/CustomStateSet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOMURL/URLSearchParams.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XHR/FormData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorker.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Crypto/CryptoKey.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLCanvasElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Canvas/CanvasDrawPath.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Canvas/CanvasTextDrawingStyles.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLImageElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLVideoElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGImageElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSUnitValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/AnimationEffect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleDeclaration.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/QueuingStrategy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamBYOBReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamDefaultReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/File.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLRenderingContext.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGL2RenderingContext.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Path2D.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CanvasGradient.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CanvasPattern.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPointReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TextMetrics.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrix.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLMediaElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGGraphicsElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStreamDefaultWriter.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLRenderingContextBase.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrixReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPoint.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/AudioTrackList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/MediaError.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TextTrack.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TextTrackList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TimeRanges.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/VideoTrackList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGAnimatedString.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/AudioTrack.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/VideoTrack.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGSVGElement.h>

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

GC_DEFINE_ALLOCATOR(PointerEventConstructor);

PointerEventConstructor::PointerEventConstructor(JS::Realm& realm)
    : NativeFunction("PointerEvent"_utf16_fly_string, realm.intrinsics().function_prototype())
{
}

PointerEventConstructor::~PointerEventConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> PointerEventConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(JS::ErrorType::ConstructorWithoutNew, "PointerEvent");
}


JS::ThrowCompletionOr<GC::Ref<JS::Object>> PointerEventConstructor::construct([[maybe_unused]] FunctionObject& new_target)
{
    WebIDL::log_trace(vm(), "PointerEventConstructor::construct");

    auto& vm = this->vm();
    auto& realm = *vm.current_realm();

    // To internally create a new object implementing the interface PointerEvent:

    // 3.2. Let prototype be ? Get(newTarget, "prototype").
    auto prototype = TRY(new_target.get(vm.names.prototype));

    // 3.3. If Type(prototype) is not Object, then:
    if (!prototype.is_object()) {
        // 1. Let targetRealm be ? GetFunctionRealm(newTarget).
        auto* target_realm = TRY(JS::get_function_realm(vm, new_target));

        // 2. Set prototype to the interface prototype object for interface in targetRealm.
        VERIFY(target_realm);
        prototype = &Bindings::ensure_web_prototype<PointerEventPrototype>(*target_realm, "PointerEvent"_fly_string);
    }

    // 4. Let instance be MakeBasicObject( « [[Prototype]], [[Extensible]], [[Realm]], [[PrimaryInterface]] »).
    // 5. Set instance.[[Realm]] to realm.
    // 6. Set instance.[[PrimaryInterface]] to interface.

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "PointerEvent");

    auto arg0 = vm.argument(0);

    String type;
    if (!false || !arg0.is_null()) {
        type = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PointerEventInit");

    PointerEventInit event_init_dict {};

    auto altitude_angle_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        altitude_angle_property_value_0 = TRY(arg1.as_object().get("altitudeAngle"_utf16_fly_string));

    if (!altitude_angle_property_value_0.is_undefined()) {

    Optional<double> altitude_angle_value_0;

    if (!altitude_angle_property_value_0.is_undefined())
        altitude_angle_value_0 = TRY(altitude_angle_property_value_0.to_double(vm));


    if (altitude_angle_value_0.has_value() && (isinf(*altitude_angle_value_0) || isnan(*altitude_angle_value_0))) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "altitudeAngle");
    }
    
    event_init_dict.altitude_angle = altitude_angle_value_0;

    }

    auto azimuth_angle_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        azimuth_angle_property_value_1 = TRY(arg1.as_object().get("azimuthAngle"_utf16_fly_string));

    if (!azimuth_angle_property_value_1.is_undefined()) {

    Optional<double> azimuth_angle_value_1;

    if (!azimuth_angle_property_value_1.is_undefined())
        azimuth_angle_value_1 = TRY(azimuth_angle_property_value_1.to_double(vm));


    if (azimuth_angle_value_1.has_value() && (isinf(*azimuth_angle_value_1) || isnan(*azimuth_angle_value_1))) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "azimuthAngle");
    }
    
    event_init_dict.azimuth_angle = azimuth_angle_value_1;

    }

    auto coalesced_events_property_value_2 = JS::js_undefined();
    if (arg1.is_object())
        coalesced_events_property_value_2 = TRY(arg1.as_object().get("coalescedEvents"_utf16_fly_string));

    GC::RootVector<GC::Root<PointerEvent>> coalesced_events_value_2 { vm.heap() };

    if (!coalesced_events_property_value_2.is_undefined()) {

    if (!coalesced_events_property_value_2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, coalesced_events_property_value_2);

    auto coalesced_events_property_value_2_iterator_method0 = TRY(coalesced_events_property_value_2.get_method(vm, vm.well_known_symbol_iterator()));
    if (!coalesced_events_property_value_2_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, coalesced_events_property_value_2);

    auto coalesced_events_property_value_2_iterator1 = TRY(JS::get_iterator_from_method(vm, coalesced_events_property_value_2, *coalesced_events_property_value_2_iterator_method0));

    GC::RootVector<GC::Root<PointerEvent>> coalesced_events_value_2_non_optional { vm.heap() };

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, coalesced_events_property_value_2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object() || !is<PointerEvent>(next_item1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PointerEvent");

    auto& sequence_item1 = static_cast<PointerEvent&>(next_item1.as_object());

    coalesced_events_value_2_non_optional.append(sequence_item1);
    }

        coalesced_events_value_2 = move(coalesced_events_value_2_non_optional);
    }

    event_init_dict.coalesced_events = coalesced_events_value_2;

    auto height_property_value_3 = JS::js_undefined();
    if (arg1.is_object())
        height_property_value_3 = TRY(arg1.as_object().get("height"_utf16_fly_string));

    double height_value_3;

    if (!height_property_value_3.is_undefined())
        height_value_3 = TRY(height_property_value_3.to_double(vm));

    else
        height_value_3 = 1;

    if (isinf(height_value_3) || isnan(height_value_3)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "height");
    }
    
    event_init_dict.height = height_value_3;

    auto is_primary_property_value_4 = JS::js_undefined();
    if (arg1.is_object())
        is_primary_property_value_4 = TRY(arg1.as_object().get("isPrimary"_utf16_fly_string));

    bool is_primary_value_4;

    if (!is_primary_property_value_4.is_undefined())

    is_primary_value_4 = is_primary_property_value_4.to_boolean();

    else
        is_primary_value_4 = static_cast<bool>(false);

    event_init_dict.is_primary = is_primary_value_4;

    auto persistent_device_id_property_value_5 = JS::js_undefined();
    if (arg1.is_object())
        persistent_device_id_property_value_5 = TRY(arg1.as_object().get("persistentDeviceId"_utf16_fly_string));

    WebIDL::Long persistent_device_id_value_5;

    if (!persistent_device_id_property_value_5.is_undefined())

    persistent_device_id_value_5 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, persistent_device_id_property_value_5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        persistent_device_id_value_5 = static_cast<WebIDL::Long>(0);

    event_init_dict.persistent_device_id = persistent_device_id_value_5;

    auto pointer_id_property_value_6 = JS::js_undefined();
    if (arg1.is_object())
        pointer_id_property_value_6 = TRY(arg1.as_object().get("pointerId"_utf16_fly_string));

    WebIDL::Long pointer_id_value_6;

    if (!pointer_id_property_value_6.is_undefined())

    pointer_id_value_6 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, pointer_id_property_value_6, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        pointer_id_value_6 = static_cast<WebIDL::Long>(0);

    event_init_dict.pointer_id = pointer_id_value_6;

    auto pointer_type_property_value_7 = JS::js_undefined();
    if (arg1.is_object())
        pointer_type_property_value_7 = TRY(arg1.as_object().get("pointerType"_utf16_fly_string));

    String pointer_type_value_7;

    if (!pointer_type_property_value_7.is_undefined()) {
        if (!false || !pointer_type_property_value_7.is_null())
            pointer_type_value_7 = TRY(WebIDL::to_string(vm, pointer_type_property_value_7));
    } else {
        pointer_type_value_7 = ""_string;
    }

    event_init_dict.pointer_type = pointer_type_value_7;

    auto predicted_events_property_value_8 = JS::js_undefined();
    if (arg1.is_object())
        predicted_events_property_value_8 = TRY(arg1.as_object().get("predictedEvents"_utf16_fly_string));

    GC::RootVector<GC::Root<PointerEvent>> predicted_events_value_8 { vm.heap() };

    if (!predicted_events_property_value_8.is_undefined()) {

    if (!predicted_events_property_value_8.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, predicted_events_property_value_8);

    auto predicted_events_property_value_8_iterator_method0 = TRY(predicted_events_property_value_8.get_method(vm, vm.well_known_symbol_iterator()));
    if (!predicted_events_property_value_8_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, predicted_events_property_value_8);

    auto predicted_events_property_value_8_iterator1 = TRY(JS::get_iterator_from_method(vm, predicted_events_property_value_8, *predicted_events_property_value_8_iterator_method0));

    GC::RootVector<GC::Root<PointerEvent>> predicted_events_value_8_non_optional { vm.heap() };

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, predicted_events_property_value_8_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object() || !is<PointerEvent>(next_item1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PointerEvent");

    auto& sequence_item1 = static_cast<PointerEvent&>(next_item1.as_object());

    predicted_events_value_8_non_optional.append(sequence_item1);
    }

        predicted_events_value_8 = move(predicted_events_value_8_non_optional);
    }

    event_init_dict.predicted_events = predicted_events_value_8;

    auto pressure_property_value_9 = JS::js_undefined();
    if (arg1.is_object())
        pressure_property_value_9 = TRY(arg1.as_object().get("pressure"_utf16_fly_string));

    float pressure_value_9;

    if (!pressure_property_value_9.is_undefined())
        pressure_value_9 = TRY(pressure_property_value_9.to_double(vm));

    else
        pressure_value_9 = 0;

    if (isinf(pressure_value_9) || isnan(pressure_value_9)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "pressure");
    }
    
    event_init_dict.pressure = pressure_value_9;

    auto tangential_pressure_property_value_10 = JS::js_undefined();
    if (arg1.is_object())
        tangential_pressure_property_value_10 = TRY(arg1.as_object().get("tangentialPressure"_utf16_fly_string));

    float tangential_pressure_value_10;

    if (!tangential_pressure_property_value_10.is_undefined())
        tangential_pressure_value_10 = TRY(tangential_pressure_property_value_10.to_double(vm));

    else
        tangential_pressure_value_10 = 0;

    if (isinf(tangential_pressure_value_10) || isnan(tangential_pressure_value_10)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "tangentialPressure");
    }
    
    event_init_dict.tangential_pressure = tangential_pressure_value_10;

    auto tilt_x_property_value_11 = JS::js_undefined();
    if (arg1.is_object())
        tilt_x_property_value_11 = TRY(arg1.as_object().get("tiltX"_utf16_fly_string));

    if (!tilt_x_property_value_11.is_undefined()) {

    Optional<WebIDL::Long> tilt_x_value_11;

    if (!tilt_x_property_value_11.is_undefined())

    tilt_x_value_11 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, tilt_x_property_value_11, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    event_init_dict.tilt_x = tilt_x_value_11;

    }

    auto tilt_y_property_value_12 = JS::js_undefined();
    if (arg1.is_object())
        tilt_y_property_value_12 = TRY(arg1.as_object().get("tiltY"_utf16_fly_string));

    if (!tilt_y_property_value_12.is_undefined()) {

    Optional<WebIDL::Long> tilt_y_value_12;

    if (!tilt_y_property_value_12.is_undefined())

    tilt_y_value_12 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, tilt_y_property_value_12, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    event_init_dict.tilt_y = tilt_y_value_12;

    }

    auto twist_property_value_13 = JS::js_undefined();
    if (arg1.is_object())
        twist_property_value_13 = TRY(arg1.as_object().get("twist"_utf16_fly_string));

    WebIDL::Long twist_value_13;

    if (!twist_property_value_13.is_undefined())

    twist_value_13 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, twist_property_value_13, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        twist_value_13 = static_cast<WebIDL::Long>(0);

    event_init_dict.twist = twist_value_13;

    auto width_property_value_14 = JS::js_undefined();
    if (arg1.is_object())
        width_property_value_14 = TRY(arg1.as_object().get("width"_utf16_fly_string));

    double width_value_14;

    if (!width_property_value_14.is_undefined())
        width_value_14 = TRY(width_property_value_14.to_double(vm));

    else
        width_value_14 = 1;

    if (isinf(width_value_14) || isnan(width_value_14)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "width");
    }
    
    event_init_dict.width = width_value_14;

    auto button_property_value_15 = JS::js_undefined();
    if (arg1.is_object())
        button_property_value_15 = TRY(arg1.as_object().get("button"_utf16_fly_string));

    WebIDL::Short button_value_15;

    if (!button_property_value_15.is_undefined())

    button_value_15 = TRY(WebIDL::convert_to_int<WebIDL::Short>(vm, button_property_value_15, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        button_value_15 = static_cast<WebIDL::Short>(0);

    event_init_dict.button = button_value_15;

    auto buttons_property_value_16 = JS::js_undefined();
    if (arg1.is_object())
        buttons_property_value_16 = TRY(arg1.as_object().get("buttons"_utf16_fly_string));

    WebIDL::UnsignedShort buttons_value_16;

    if (!buttons_property_value_16.is_undefined())

    buttons_value_16 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, buttons_property_value_16, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        buttons_value_16 = static_cast<WebIDL::UnsignedShort>(0);

    event_init_dict.buttons = buttons_value_16;

    auto client_x_property_value_17 = JS::js_undefined();
    if (arg1.is_object())
        client_x_property_value_17 = TRY(arg1.as_object().get("clientX"_utf16_fly_string));

    double client_x_value_17;

    if (!client_x_property_value_17.is_undefined())
        client_x_value_17 = TRY(client_x_property_value_17.to_double(vm));

    else
        client_x_value_17 = 0;

    if (isinf(client_x_value_17) || isnan(client_x_value_17)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "clientX");
    }
    
    event_init_dict.client_x = client_x_value_17;

    auto client_y_property_value_18 = JS::js_undefined();
    if (arg1.is_object())
        client_y_property_value_18 = TRY(arg1.as_object().get("clientY"_utf16_fly_string));

    double client_y_value_18;

    if (!client_y_property_value_18.is_undefined())
        client_y_value_18 = TRY(client_y_property_value_18.to_double(vm));

    else
        client_y_value_18 = 0;

    if (isinf(client_y_value_18) || isnan(client_y_value_18)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "clientY");
    }
    
    event_init_dict.client_y = client_y_value_18;

    auto movement_x_property_value_19 = JS::js_undefined();
    if (arg1.is_object())
        movement_x_property_value_19 = TRY(arg1.as_object().get("movementX"_utf16_fly_string));

    double movement_x_value_19;

    if (!movement_x_property_value_19.is_undefined())
        movement_x_value_19 = TRY(movement_x_property_value_19.to_double(vm));

    else
        movement_x_value_19 = 0;

    if (isinf(movement_x_value_19) || isnan(movement_x_value_19)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "movementX");
    }
    
    event_init_dict.movement_x = movement_x_value_19;

    auto movement_y_property_value_20 = JS::js_undefined();
    if (arg1.is_object())
        movement_y_property_value_20 = TRY(arg1.as_object().get("movementY"_utf16_fly_string));

    double movement_y_value_20;

    if (!movement_y_property_value_20.is_undefined())
        movement_y_value_20 = TRY(movement_y_property_value_20.to_double(vm));

    else
        movement_y_value_20 = 0;

    if (isinf(movement_y_value_20) || isnan(movement_y_value_20)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "movementY");
    }
    
    event_init_dict.movement_y = movement_y_value_20;

    auto related_target_property_value_21 = JS::js_undefined();
    if (arg1.is_object())
        related_target_property_value_21 = TRY(arg1.as_object().get("relatedTarget"_utf16_fly_string));

    GC::Ptr<EventTarget> related_target_value_21;

    if (!related_target_property_value_21.is_nullish()) {
        if (!related_target_property_value_21.is_object() || !is<EventTarget>(related_target_property_value_21.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "EventTarget");

        related_target_value_21 = &static_cast<EventTarget&>(related_target_property_value_21.as_object());
    }

    event_init_dict.related_target = related_target_value_21;

    auto screen_x_property_value_22 = JS::js_undefined();
    if (arg1.is_object())
        screen_x_property_value_22 = TRY(arg1.as_object().get("screenX"_utf16_fly_string));

    double screen_x_value_22;

    if (!screen_x_property_value_22.is_undefined())
        screen_x_value_22 = TRY(screen_x_property_value_22.to_double(vm));

    else
        screen_x_value_22 = 0;

    if (isinf(screen_x_value_22) || isnan(screen_x_value_22)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "screenX");
    }
    
    event_init_dict.screen_x = screen_x_value_22;

    auto screen_y_property_value_23 = JS::js_undefined();
    if (arg1.is_object())
        screen_y_property_value_23 = TRY(arg1.as_object().get("screenY"_utf16_fly_string));

    double screen_y_value_23;

    if (!screen_y_property_value_23.is_undefined())
        screen_y_value_23 = TRY(screen_y_property_value_23.to_double(vm));

    else
        screen_y_value_23 = 0;

    if (isinf(screen_y_value_23) || isnan(screen_y_value_23)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "screenY");
    }
    
    event_init_dict.screen_y = screen_y_value_23;

    auto alt_key_property_value_24 = JS::js_undefined();
    if (arg1.is_object())
        alt_key_property_value_24 = TRY(arg1.as_object().get("altKey"_utf16_fly_string));

    bool alt_key_value_24;

    if (!alt_key_property_value_24.is_undefined())

    alt_key_value_24 = alt_key_property_value_24.to_boolean();

    else
        alt_key_value_24 = static_cast<bool>(false);

    event_init_dict.alt_key = alt_key_value_24;

    auto ctrl_key_property_value_25 = JS::js_undefined();
    if (arg1.is_object())
        ctrl_key_property_value_25 = TRY(arg1.as_object().get("ctrlKey"_utf16_fly_string));

    bool ctrl_key_value_25;

    if (!ctrl_key_property_value_25.is_undefined())

    ctrl_key_value_25 = ctrl_key_property_value_25.to_boolean();

    else
        ctrl_key_value_25 = static_cast<bool>(false);

    event_init_dict.ctrl_key = ctrl_key_value_25;

    auto meta_key_property_value_26 = JS::js_undefined();
    if (arg1.is_object())
        meta_key_property_value_26 = TRY(arg1.as_object().get("metaKey"_utf16_fly_string));

    bool meta_key_value_26;

    if (!meta_key_property_value_26.is_undefined())

    meta_key_value_26 = meta_key_property_value_26.to_boolean();

    else
        meta_key_value_26 = static_cast<bool>(false);

    event_init_dict.meta_key = meta_key_value_26;

    auto modifier_alt_graph_property_value_27 = JS::js_undefined();
    if (arg1.is_object())
        modifier_alt_graph_property_value_27 = TRY(arg1.as_object().get("modifierAltGraph"_utf16_fly_string));

    bool modifier_alt_graph_value_27;

    if (!modifier_alt_graph_property_value_27.is_undefined())

    modifier_alt_graph_value_27 = modifier_alt_graph_property_value_27.to_boolean();

    else
        modifier_alt_graph_value_27 = static_cast<bool>(false);

    event_init_dict.modifier_alt_graph = modifier_alt_graph_value_27;

    auto modifier_caps_lock_property_value_28 = JS::js_undefined();
    if (arg1.is_object())
        modifier_caps_lock_property_value_28 = TRY(arg1.as_object().get("modifierCapsLock"_utf16_fly_string));

    bool modifier_caps_lock_value_28;

    if (!modifier_caps_lock_property_value_28.is_undefined())

    modifier_caps_lock_value_28 = modifier_caps_lock_property_value_28.to_boolean();

    else
        modifier_caps_lock_value_28 = static_cast<bool>(false);

    event_init_dict.modifier_caps_lock = modifier_caps_lock_value_28;

    auto modifier_fn_property_value_29 = JS::js_undefined();
    if (arg1.is_object())
        modifier_fn_property_value_29 = TRY(arg1.as_object().get("modifierFn"_utf16_fly_string));

    bool modifier_fn_value_29;

    if (!modifier_fn_property_value_29.is_undefined())

    modifier_fn_value_29 = modifier_fn_property_value_29.to_boolean();

    else
        modifier_fn_value_29 = static_cast<bool>(false);

    event_init_dict.modifier_fn = modifier_fn_value_29;

    auto modifier_fn_lock_property_value_30 = JS::js_undefined();
    if (arg1.is_object())
        modifier_fn_lock_property_value_30 = TRY(arg1.as_object().get("modifierFnLock"_utf16_fly_string));

    bool modifier_fn_lock_value_30;

    if (!modifier_fn_lock_property_value_30.is_undefined())

    modifier_fn_lock_value_30 = modifier_fn_lock_property_value_30.to_boolean();

    else
        modifier_fn_lock_value_30 = static_cast<bool>(false);

    event_init_dict.modifier_fn_lock = modifier_fn_lock_value_30;

    auto modifier_hyper_property_value_31 = JS::js_undefined();
    if (arg1.is_object())
        modifier_hyper_property_value_31 = TRY(arg1.as_object().get("modifierHyper"_utf16_fly_string));

    bool modifier_hyper_value_31;

    if (!modifier_hyper_property_value_31.is_undefined())

    modifier_hyper_value_31 = modifier_hyper_property_value_31.to_boolean();

    else
        modifier_hyper_value_31 = static_cast<bool>(false);

    event_init_dict.modifier_hyper = modifier_hyper_value_31;

    auto modifier_num_lock_property_value_32 = JS::js_undefined();
    if (arg1.is_object())
        modifier_num_lock_property_value_32 = TRY(arg1.as_object().get("modifierNumLock"_utf16_fly_string));

    bool modifier_num_lock_value_32;

    if (!modifier_num_lock_property_value_32.is_undefined())

    modifier_num_lock_value_32 = modifier_num_lock_property_value_32.to_boolean();

    else
        modifier_num_lock_value_32 = static_cast<bool>(false);

    event_init_dict.modifier_num_lock = modifier_num_lock_value_32;

    auto modifier_scroll_lock_property_value_33 = JS::js_undefined();
    if (arg1.is_object())
        modifier_scroll_lock_property_value_33 = TRY(arg1.as_object().get("modifierScrollLock"_utf16_fly_string));

    bool modifier_scroll_lock_value_33;

    if (!modifier_scroll_lock_property_value_33.is_undefined())

    modifier_scroll_lock_value_33 = modifier_scroll_lock_property_value_33.to_boolean();

    else
        modifier_scroll_lock_value_33 = static_cast<bool>(false);

    event_init_dict.modifier_scroll_lock = modifier_scroll_lock_value_33;

    auto modifier_super_property_value_34 = JS::js_undefined();
    if (arg1.is_object())
        modifier_super_property_value_34 = TRY(arg1.as_object().get("modifierSuper"_utf16_fly_string));

    bool modifier_super_value_34;

    if (!modifier_super_property_value_34.is_undefined())

    modifier_super_value_34 = modifier_super_property_value_34.to_boolean();

    else
        modifier_super_value_34 = static_cast<bool>(false);

    event_init_dict.modifier_super = modifier_super_value_34;

    auto modifier_symbol_property_value_35 = JS::js_undefined();
    if (arg1.is_object())
        modifier_symbol_property_value_35 = TRY(arg1.as_object().get("modifierSymbol"_utf16_fly_string));

    bool modifier_symbol_value_35;

    if (!modifier_symbol_property_value_35.is_undefined())

    modifier_symbol_value_35 = modifier_symbol_property_value_35.to_boolean();

    else
        modifier_symbol_value_35 = static_cast<bool>(false);

    event_init_dict.modifier_symbol = modifier_symbol_value_35;

    auto modifier_symbol_lock_property_value_36 = JS::js_undefined();
    if (arg1.is_object())
        modifier_symbol_lock_property_value_36 = TRY(arg1.as_object().get("modifierSymbolLock"_utf16_fly_string));

    bool modifier_symbol_lock_value_36;

    if (!modifier_symbol_lock_property_value_36.is_undefined())

    modifier_symbol_lock_value_36 = modifier_symbol_lock_property_value_36.to_boolean();

    else
        modifier_symbol_lock_value_36 = static_cast<bool>(false);

    event_init_dict.modifier_symbol_lock = modifier_symbol_lock_value_36;

    auto shift_key_property_value_37 = JS::js_undefined();
    if (arg1.is_object())
        shift_key_property_value_37 = TRY(arg1.as_object().get("shiftKey"_utf16_fly_string));

    bool shift_key_value_37;

    if (!shift_key_property_value_37.is_undefined())

    shift_key_value_37 = shift_key_property_value_37.to_boolean();

    else
        shift_key_value_37 = static_cast<bool>(false);

    event_init_dict.shift_key = shift_key_value_37;

    auto detail_property_value_38 = JS::js_undefined();
    if (arg1.is_object())
        detail_property_value_38 = TRY(arg1.as_object().get("detail"_utf16_fly_string));

    WebIDL::Long detail_value_38;

    if (!detail_property_value_38.is_undefined())

    detail_value_38 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, detail_property_value_38, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        detail_value_38 = static_cast<WebIDL::Long>(0);

    event_init_dict.detail = detail_value_38;

    auto view_property_value_39 = JS::js_undefined();
    if (arg1.is_object())
        view_property_value_39 = TRY(arg1.as_object().get("view"_utf16_fly_string));

    GC::Ptr<WindowProxy> view_value_39;

    if (!view_property_value_39.is_nullish()) {
        if (!view_property_value_39.is_object() || !is<WindowProxy>(view_property_value_39.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WindowProxy");

        view_value_39 = &static_cast<WindowProxy&>(view_property_value_39.as_object());
    }

    event_init_dict.view = view_value_39;

    auto bubbles_property_value_40 = JS::js_undefined();
    if (arg1.is_object())
        bubbles_property_value_40 = TRY(arg1.as_object().get("bubbles"_utf16_fly_string));

    bool bubbles_value_40;

    if (!bubbles_property_value_40.is_undefined())

    bubbles_value_40 = bubbles_property_value_40.to_boolean();

    else
        bubbles_value_40 = static_cast<bool>(false);

    event_init_dict.bubbles = bubbles_value_40;

    auto cancelable_property_value_41 = JS::js_undefined();
    if (arg1.is_object())
        cancelable_property_value_41 = TRY(arg1.as_object().get("cancelable"_utf16_fly_string));

    bool cancelable_value_41;

    if (!cancelable_property_value_41.is_undefined())

    cancelable_value_41 = cancelable_property_value_41.to_boolean();

    else
        cancelable_value_41 = static_cast<bool>(false);

    event_init_dict.cancelable = cancelable_value_41;

    auto composed_property_value_42 = JS::js_undefined();
    if (arg1.is_object())
        composed_property_value_42 = TRY(arg1.as_object().get("composed"_utf16_fly_string));

    bool composed_value_42;

    if (!composed_property_value_42.is_undefined())

    composed_value_42 = composed_property_value_42.to_boolean();

    else
        composed_value_42 = static_cast<bool>(false);

    event_init_dict.composed = composed_value_42;

    auto impl = TRY(throw_dom_exception_if_needed(vm, [&] { return UIEvents::PointerEvent::construct_impl(realm, type, event_init_dict); }));

    // 7. Set instance.[[Prototype]] to prototype.
    VERIFY(prototype.is_object());
    impl->set_prototype(&prototype.as_object());

    // FIXME: Steps 8...11. of the "internally create a new object implementing the interface PointerEvent" algorithm
    // (https://webidl.spec.whatwg.org/#js-platform-objects) are currently not handled, or are handled within UIEvents::PointerEvent::construct_impl().
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


void PointerEventConstructor::initialize(JS::Realm& realm)
{
    auto& vm = this->vm();
    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    Base::initialize(realm);

    set_prototype(&ensure_web_constructor<MouseEventPrototype>(realm, "MouseEvent"_fly_string));

    define_direct_property(vm.names.length, JS::Value(1), JS::Attribute::Configurable);
    define_direct_property(vm.names.name, JS::PrimitiveString::create(vm, "PointerEvent"_string), JS::Attribute::Configurable);
    define_direct_property(vm.names.prototype, &ensure_web_prototype<PointerEventPrototype>(realm, "PointerEvent"_fly_string), 0);


}

} // namespace Web::Bindings
