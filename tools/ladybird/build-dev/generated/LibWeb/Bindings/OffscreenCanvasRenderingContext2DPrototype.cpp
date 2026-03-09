
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
#include <LibWeb/Bindings/OffscreenCanvasRenderingContext2DPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/OffscreenCanvasRenderingContext2D.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CanvasRenderingContext2D.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Canvas/CanvasDrawPath.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Canvas/CanvasTextDrawingStyles.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLCanvasElement.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLRenderingContext.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGL2RenderingContext.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Request.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Scripting/Fetching.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLMediaElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGGraphicsElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrixReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPoint.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/DOMStringMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ElementInternals.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Element.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLRenderingContextBase.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Headers.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/AudioTrackList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/MediaError.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TextTrack.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TextTrackList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TimeRanges.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/VideoTrackList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRectReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGAnimatedString.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/QueuingStrategy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamBYOBReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamDefaultReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStream.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XHR/FormData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOMURL/URLSearchParams.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/AudioTrack.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/VideoTrack.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGSVGElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStreamDefaultWriter.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleDeclaration.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DocumentFragment.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/Animation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/KeyframeEffect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Document.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/HTMLCollection.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/File.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGAnimatedRect.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRule.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRuleList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/MediaList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StyleSheet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSUnitValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorker.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Crypto/CryptoKey.h>

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

GC_DEFINE_ALLOCATOR(OffscreenCanvasRenderingContext2DPrototype);

OffscreenCanvasRenderingContext2DPrototype::OffscreenCanvasRenderingContext2DPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

OffscreenCanvasRenderingContext2DPrototype::~OffscreenCanvasRenderingContext2DPrototype()
{
}

void OffscreenCanvasRenderingContext2DPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_canvas_getter = JS::NativeFunction::create(realm, canvas_getter, 0, "canvas"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_canvas_setter;

    define_direct_accessor("canvas"_utf16_fly_string, native_canvas_getter, native_canvas_setter, default_attributes);

    auto native_stroke_style_getter = JS::NativeFunction::create(realm, stroke_style_getter, 0, "strokeStyle"_utf16_fly_string, &realm, "get"sv);

    auto native_stroke_style_setter = JS::NativeFunction::create(realm, stroke_style_setter, 1, "strokeStyle"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("strokeStyle"_utf16_fly_string, native_stroke_style_getter, native_stroke_style_setter, default_attributes);

    auto native_fill_style_getter = JS::NativeFunction::create(realm, fill_style_getter, 0, "fillStyle"_utf16_fly_string, &realm, "get"sv);

    auto native_fill_style_setter = JS::NativeFunction::create(realm, fill_style_setter, 1, "fillStyle"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("fillStyle"_utf16_fly_string, native_fill_style_getter, native_fill_style_setter, default_attributes);

    auto native_line_width_getter = JS::NativeFunction::create(realm, line_width_getter, 0, "lineWidth"_utf16_fly_string, &realm, "get"sv);

    auto native_line_width_setter = JS::NativeFunction::create(realm, line_width_setter, 1, "lineWidth"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("lineWidth"_utf16_fly_string, native_line_width_getter, native_line_width_setter, default_attributes);

    auto native_line_cap_getter = JS::NativeFunction::create(realm, line_cap_getter, 0, "lineCap"_utf16_fly_string, &realm, "get"sv);

    auto native_line_cap_setter = JS::NativeFunction::create(realm, line_cap_setter, 1, "lineCap"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("lineCap"_utf16_fly_string, native_line_cap_getter, native_line_cap_setter, default_attributes);

    auto native_line_join_getter = JS::NativeFunction::create(realm, line_join_getter, 0, "lineJoin"_utf16_fly_string, &realm, "get"sv);

    auto native_line_join_setter = JS::NativeFunction::create(realm, line_join_setter, 1, "lineJoin"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("lineJoin"_utf16_fly_string, native_line_join_getter, native_line_join_setter, default_attributes);

    auto native_miter_limit_getter = JS::NativeFunction::create(realm, miter_limit_getter, 0, "miterLimit"_utf16_fly_string, &realm, "get"sv);

    auto native_miter_limit_setter = JS::NativeFunction::create(realm, miter_limit_setter, 1, "miterLimit"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("miterLimit"_utf16_fly_string, native_miter_limit_getter, native_miter_limit_setter, default_attributes);

    auto native_line_dash_offset_getter = JS::NativeFunction::create(realm, line_dash_offset_getter, 0, "lineDashOffset"_utf16_fly_string, &realm, "get"sv);

    auto native_line_dash_offset_setter = JS::NativeFunction::create(realm, line_dash_offset_setter, 1, "lineDashOffset"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("lineDashOffset"_utf16_fly_string, native_line_dash_offset_getter, native_line_dash_offset_setter, default_attributes);

    auto native_global_alpha_getter = JS::NativeFunction::create(realm, global_alpha_getter, 0, "globalAlpha"_utf16_fly_string, &realm, "get"sv);

    auto native_global_alpha_setter = JS::NativeFunction::create(realm, global_alpha_setter, 1, "globalAlpha"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("globalAlpha"_utf16_fly_string, native_global_alpha_getter, native_global_alpha_setter, default_attributes);

    auto native_global_composite_operation_getter = JS::NativeFunction::create(realm, global_composite_operation_getter, 0, "globalCompositeOperation"_utf16_fly_string, &realm, "get"sv);

    auto native_global_composite_operation_setter = JS::NativeFunction::create(realm, global_composite_operation_setter, 1, "globalCompositeOperation"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("globalCompositeOperation"_utf16_fly_string, native_global_composite_operation_getter, native_global_composite_operation_setter, default_attributes);

    auto native_filter_getter = JS::NativeFunction::create(realm, filter_getter, 0, "filter"_utf16_fly_string, &realm, "get"sv);

    auto native_filter_setter = JS::NativeFunction::create(realm, filter_setter, 1, "filter"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("filter"_utf16_fly_string, native_filter_getter, native_filter_setter, default_attributes);

    define_direct_property("lang"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    auto native_font_getter = JS::NativeFunction::create(realm, font_getter, 0, "font"_utf16_fly_string, &realm, "get"sv);

    auto native_font_setter = JS::NativeFunction::create(realm, font_setter, 1, "font"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("font"_utf16_fly_string, native_font_getter, native_font_setter, default_attributes);

    auto native_text_align_getter = JS::NativeFunction::create(realm, text_align_getter, 0, "textAlign"_utf16_fly_string, &realm, "get"sv);

    auto native_text_align_setter = JS::NativeFunction::create(realm, text_align_setter, 1, "textAlign"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("textAlign"_utf16_fly_string, native_text_align_getter, native_text_align_setter, default_attributes);

    auto native_text_baseline_getter = JS::NativeFunction::create(realm, text_baseline_getter, 0, "textBaseline"_utf16_fly_string, &realm, "get"sv);

    auto native_text_baseline_setter = JS::NativeFunction::create(realm, text_baseline_setter, 1, "textBaseline"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("textBaseline"_utf16_fly_string, native_text_baseline_getter, native_text_baseline_setter, default_attributes);

    auto native_direction_getter = JS::NativeFunction::create(realm, direction_getter, 0, "direction"_utf16_fly_string, &realm, "get"sv);

    auto native_direction_setter = JS::NativeFunction::create(realm, direction_setter, 1, "direction"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("direction"_utf16_fly_string, native_direction_getter, native_direction_setter, default_attributes);

    define_direct_property("letterSpacing"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_direct_property("fontKerning"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_direct_property("fontStretch"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_direct_property("fontVariantCaps"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_direct_property("textRendering"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_direct_property("wordSpacing"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    auto native_shadow_offset_x_getter = JS::NativeFunction::create(realm, shadow_offset_x_getter, 0, "shadowOffsetX"_utf16_fly_string, &realm, "get"sv);

    auto native_shadow_offset_x_setter = JS::NativeFunction::create(realm, shadow_offset_x_setter, 1, "shadowOffsetX"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("shadowOffsetX"_utf16_fly_string, native_shadow_offset_x_getter, native_shadow_offset_x_setter, default_attributes);

    auto native_shadow_offset_y_getter = JS::NativeFunction::create(realm, shadow_offset_y_getter, 0, "shadowOffsetY"_utf16_fly_string, &realm, "get"sv);

    auto native_shadow_offset_y_setter = JS::NativeFunction::create(realm, shadow_offset_y_setter, 1, "shadowOffsetY"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("shadowOffsetY"_utf16_fly_string, native_shadow_offset_y_getter, native_shadow_offset_y_setter, default_attributes);

    auto native_shadow_blur_getter = JS::NativeFunction::create(realm, shadow_blur_getter, 0, "shadowBlur"_utf16_fly_string, &realm, "get"sv);

    auto native_shadow_blur_setter = JS::NativeFunction::create(realm, shadow_blur_setter, 1, "shadowBlur"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("shadowBlur"_utf16_fly_string, native_shadow_blur_getter, native_shadow_blur_setter, default_attributes);

    auto native_shadow_color_getter = JS::NativeFunction::create(realm, shadow_color_getter, 0, "shadowColor"_utf16_fly_string, &realm, "get"sv);

    auto native_shadow_color_setter = JS::NativeFunction::create(realm, shadow_color_setter, 1, "shadowColor"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("shadowColor"_utf16_fly_string, native_shadow_color_getter, native_shadow_color_setter, default_attributes);

    auto native_image_smoothing_enabled_getter = JS::NativeFunction::create(realm, image_smoothing_enabled_getter, 0, "imageSmoothingEnabled"_utf16_fly_string, &realm, "get"sv);

    auto native_image_smoothing_enabled_setter = JS::NativeFunction::create(realm, image_smoothing_enabled_setter, 1, "imageSmoothingEnabled"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("imageSmoothingEnabled"_utf16_fly_string, native_image_smoothing_enabled_getter, native_image_smoothing_enabled_setter, default_attributes);

    auto native_image_smoothing_quality_getter = JS::NativeFunction::create(realm, image_smoothing_quality_getter, 0, "imageSmoothingQuality"_utf16_fly_string, &realm, "get"sv);

    auto native_image_smoothing_quality_setter = JS::NativeFunction::create(realm, image_smoothing_quality_setter, 1, "imageSmoothingQuality"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("imageSmoothingQuality"_utf16_fly_string, native_image_smoothing_quality_getter, native_image_smoothing_quality_setter, default_attributes);

        define_direct_property("isPointInStroke"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("isPointInStroke"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_native_function(realm, "createLinearGradient"_utf16_fly_string, create_linear_gradient, 4, default_attributes);

    define_native_function(realm, "createRadialGradient"_utf16_fly_string, create_radial_gradient, 6, default_attributes);

    define_native_function(realm, "createConicGradient"_utf16_fly_string, create_conic_gradient, 3, default_attributes);

    define_native_function(realm, "createPattern"_utf16_fly_string, create_pattern, 2, default_attributes);

    define_native_function(realm, "getContextAttributes"_utf16_fly_string, get_context_attributes, 0, default_attributes);

    define_native_function(realm, "setLineDash"_utf16_fly_string, set_line_dash, 1, default_attributes);

    define_native_function(realm, "getLineDash"_utf16_fly_string, get_line_dash, 0, default_attributes);

    define_native_function(realm, "createImageData"_utf16_fly_string, create_image_data, 1, default_attributes);

    define_native_function(realm, "getImageData"_utf16_fly_string, get_image_data, 4, default_attributes);

    define_native_function(realm, "putImageData"_utf16_fly_string, put_image_data, 3, default_attributes);

    define_native_function(realm, "scale"_utf16_fly_string, scale, 2, default_attributes);

    define_native_function(realm, "rotate"_utf16_fly_string, rotate, 1, default_attributes);

    define_native_function(realm, "translate"_utf16_fly_string, translate, 2, default_attributes);

    define_native_function(realm, "transform"_utf16_fly_string, transform, 6, default_attributes);

    define_native_function(realm, "getTransform"_utf16_fly_string, get_transform, 0, default_attributes);

    define_native_function(realm, "setTransform"_utf16_fly_string, set_transform, 0, default_attributes);

    define_native_function(realm, "resetTransform"_utf16_fly_string, reset_transform, 0, default_attributes);

    define_native_function(realm, "clearRect"_utf16_fly_string, clear_rect, 4, default_attributes);

    define_native_function(realm, "fillRect"_utf16_fly_string, fill_rect, 4, default_attributes);

    define_native_function(realm, "strokeRect"_utf16_fly_string, stroke_rect, 4, default_attributes);

    define_native_function(realm, "drawImage"_utf16_fly_string, draw_image, 3, default_attributes);

    define_native_function(realm, "save"_utf16_fly_string, save, 0, default_attributes);

    define_native_function(realm, "restore"_utf16_fly_string, restore, 0, default_attributes);

    define_native_function(realm, "reset"_utf16_fly_string, reset, 0, default_attributes);

    define_native_function(realm, "isContextLost"_utf16_fly_string, is_context_lost, 0, default_attributes);

    define_native_function(realm, "closePath"_utf16_fly_string, close_path, 0, default_attributes);

    define_native_function(realm, "moveTo"_utf16_fly_string, move_to, 2, default_attributes);

    define_native_function(realm, "lineTo"_utf16_fly_string, line_to, 2, default_attributes);

    define_native_function(realm, "quadraticCurveTo"_utf16_fly_string, quadratic_curve_to, 4, default_attributes);

    define_native_function(realm, "bezierCurveTo"_utf16_fly_string, bezier_curve_to, 6, default_attributes);

    define_native_function(realm, "arcTo"_utf16_fly_string, arc_to, 5, default_attributes);

    define_native_function(realm, "rect"_utf16_fly_string, rect, 4, default_attributes);

    define_native_function(realm, "roundRect"_utf16_fly_string, round_rect, 4, default_attributes);

    define_native_function(realm, "arc"_utf16_fly_string, arc, 5, default_attributes);

    define_native_function(realm, "ellipse"_utf16_fly_string, ellipse, 7, default_attributes);

    define_native_function(realm, "fillText"_utf16_fly_string, fill_text, 3, default_attributes);

    define_native_function(realm, "strokeText"_utf16_fly_string, stroke_text, 3, default_attributes);

    define_native_function(realm, "measureText"_utf16_fly_string, measure_text, 1, default_attributes);

    define_native_function(realm, "beginPath"_utf16_fly_string, begin_path, 0, default_attributes);

    define_native_function(realm, "fill"_utf16_fly_string, fill, 0, default_attributes);

    define_native_function(realm, "stroke"_utf16_fly_string, stroke, 0, default_attributes);

    define_native_function(realm, "clip"_utf16_fly_string, clip, 0, default_attributes);

    define_native_function(realm, "isPointInPath"_utf16_fly_string, is_point_in_path, 2, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "OffscreenCanvasRenderingContext2D"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void OffscreenCanvasRenderingContext2DPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::OffscreenCanvasRenderingContext2D*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::OffscreenCanvasRenderingContext2D>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "OffscreenCanvasRenderingContext2D");
    return static_cast<HTML::OffscreenCanvasRenderingContext2D*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::canvas_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::canvas_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->canvas(); }));

    return &const_cast<OffscreenCanvas&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::stroke_style_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::stroke_style_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->stroke_style(); }));

    return retval.visit(

        [&vm, &realm]([[maybe_unused]] String const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::PrimitiveString::create(vm, visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<CanvasGradient> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<CanvasGradient&>(*visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<CanvasPattern> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<CanvasPattern&>(*visited_union_value0);

        }

    );

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::stroke_style_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::stroke_style_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    auto value_to_variant = [&vm, &realm](JS::Value value) -> JS::ThrowCompletionOr<Variant<String, GC::Root<CanvasGradient>, GC::Root<CanvasPattern>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (value.is_object()) {
            [[maybe_unused]] auto& value_object = value.as_object();

            if (is<PlatformObject>(value_object)) {

                if (auto* value_result = as_if<CanvasGradient>(value_object))
                    return GC::make_root(*value_result);

                if (auto* value_result = as_if<CanvasPattern>(value_object))
                    return GC::make_root(*value_result);

            }

        }

    String value_string;
    if (!false || !value.is_null()) {
        value_string = TRY(WebIDL::to_string(vm, value));
    }

        return { value_string };

    };

    Variant<String, GC::Root<CanvasGradient>, GC::Root<CanvasPattern>> cpp_value = TRY(value_to_variant(value));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_stroke_style(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::fill_style_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::fill_style_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->fill_style(); }));

    return retval.visit(

        [&vm, &realm]([[maybe_unused]] String const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::PrimitiveString::create(vm, visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<CanvasGradient> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<CanvasGradient&>(*visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] GC::Root<CanvasPattern> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<CanvasPattern&>(*visited_union_value0);

        }

    );

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::fill_style_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::fill_style_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    auto value_to_variant = [&vm, &realm](JS::Value value) -> JS::ThrowCompletionOr<Variant<String, GC::Root<CanvasGradient>, GC::Root<CanvasPattern>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (value.is_object()) {
            [[maybe_unused]] auto& value_object = value.as_object();

            if (is<PlatformObject>(value_object)) {

                if (auto* value_result = as_if<CanvasGradient>(value_object))
                    return GC::make_root(*value_result);

                if (auto* value_result = as_if<CanvasPattern>(value_object))
                    return GC::make_root(*value_result);

            }

        }

    String value_string;
    if (!false || !value.is_null()) {
        value_string = TRY(WebIDL::to_string(vm, value));
    }

        return { value_string };

    };

    Variant<String, GC::Root<CanvasGradient>, GC::Root<CanvasPattern>> cpp_value = TRY(value_to_variant(value));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_fill_style(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::line_width_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::line_width_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->line_width(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::line_width_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::line_width_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_line_width(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::line_cap_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::line_cap_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->line_cap(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::line_cap_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::line_cap_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    CanvasLineCap cpp_value { CanvasLineCap::Butt };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == "butt"sv)
        cpp_value = CanvasLineCap::Butt;

    else if (value_string == "round"sv)
        cpp_value = CanvasLineCap::Round;

    else if (value_string == "square"sv)
        cpp_value = CanvasLineCap::Square;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_line_cap(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::line_join_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::line_join_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->line_join(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::line_join_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::line_join_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    CanvasLineJoin cpp_value { CanvasLineJoin::Round };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == "round"sv)
        cpp_value = CanvasLineJoin::Round;

    else if (value_string == "bevel"sv)
        cpp_value = CanvasLineJoin::Bevel;

    else if (value_string == "miter"sv)
        cpp_value = CanvasLineJoin::Miter;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_line_join(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::miter_limit_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::miter_limit_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->miter_limit(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::miter_limit_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::miter_limit_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_miter_limit(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::line_dash_offset_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::line_dash_offset_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->line_dash_offset(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::line_dash_offset_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::line_dash_offset_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_line_dash_offset(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::global_alpha_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::global_alpha_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->global_alpha(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::global_alpha_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::global_alpha_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_global_alpha(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::global_composite_operation_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::global_composite_operation_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->global_composite_operation(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::global_composite_operation_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::global_composite_operation_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_global_composite_operation(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::filter_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::filter_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->filter(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::filter_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::filter_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_filter(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::font_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::font_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->font(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::font_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::font_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_font(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::text_align_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::text_align_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->text_align(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::text_align_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::text_align_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    CanvasTextAlign cpp_value { CanvasTextAlign::Start };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == "start"sv)
        cpp_value = CanvasTextAlign::Start;

    else if (value_string == "end"sv)
        cpp_value = CanvasTextAlign::End;

    else if (value_string == "left"sv)
        cpp_value = CanvasTextAlign::Left;

    else if (value_string == "right"sv)
        cpp_value = CanvasTextAlign::Right;

    else if (value_string == "center"sv)
        cpp_value = CanvasTextAlign::Center;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_text_align(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::text_baseline_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::text_baseline_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->text_baseline(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::text_baseline_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::text_baseline_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    CanvasTextBaseline cpp_value { CanvasTextBaseline::Top };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == "top"sv)
        cpp_value = CanvasTextBaseline::Top;

    else if (value_string == "hanging"sv)
        cpp_value = CanvasTextBaseline::Hanging;

    else if (value_string == "middle"sv)
        cpp_value = CanvasTextBaseline::Middle;

    else if (value_string == "alphabetic"sv)
        cpp_value = CanvasTextBaseline::Alphabetic;

    else if (value_string == "ideographic"sv)
        cpp_value = CanvasTextBaseline::Ideographic;

    else if (value_string == "bottom"sv)
        cpp_value = CanvasTextBaseline::Bottom;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_text_baseline(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::direction_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::direction_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->direction(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::direction_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::direction_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    CanvasDirection cpp_value { CanvasDirection::Ltr };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == "ltr"sv)
        cpp_value = CanvasDirection::Ltr;

    else if (value_string == "rtl"sv)
        cpp_value = CanvasDirection::Rtl;

    else if (value_string == "inherit"sv)
        cpp_value = CanvasDirection::Inherit;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_direction(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::shadow_offset_x_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::shadow_offset_x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->shadow_offset_x(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::shadow_offset_x_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::shadow_offset_x_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_shadow_offset_x(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::shadow_offset_y_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::shadow_offset_y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->shadow_offset_y(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::shadow_offset_y_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::shadow_offset_y_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_shadow_offset_y(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::shadow_blur_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::shadow_blur_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->shadow_blur(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::shadow_blur_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::shadow_blur_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    double cpp_value = TRY(value.to_double(vm));

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_shadow_blur(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::shadow_color_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::shadow_color_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->shadow_color(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::shadow_color_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::shadow_color_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_shadow_color(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::image_smoothing_enabled_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::image_smoothing_enabled_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->image_smoothing_enabled(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::image_smoothing_enabled_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::image_smoothing_enabled_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    bool cpp_value;

    cpp_value = value.to_boolean();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_image_smoothing_enabled(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::image_smoothing_quality_getter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::image_smoothing_quality_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->image_smoothing_quality(); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::image_smoothing_quality_setter)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::image_smoothing_quality_setter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    // 1. Let V be undefined.
    auto value = JS::js_undefined();

    // 2. If any arguments were passed, then set V to the value of the first argument passed.
    if (vm.argument_count() > 0)
        value = vm.argument(0);

    // 3. Let id be attribute’s identifier.
    // 4. Let idlObject be null.
    // 5. If attribute is a regular attribute:

    // FIXME: 1. Let jsValue be the this value, if it is not null or undefined, or realm’s global object otherwise. (This will subsequently cause a TypeError in a few steps, if the global object does not implement target and [LegacyLenientThis] is not specified.)
    auto* impl = TRY(impl_from(vm));

    // FIXME: 2. If jsValue is a platform object, then perform a security check, passing jsValue, id, and "setter".
    // FIXME: 3. Let validThis be true if jsValue implements target, or false otherwise.
    // FIXME: 4. If validThis is false and attribute was not specified with the [LegacyLenientThis] extended attribute, then throw a TypeError.

    ImageSmoothingQuality cpp_value { ImageSmoothingQuality::Low };

    auto value_string = TRY(value.to_string(vm));

    if (value_string == "low"sv)
        cpp_value = ImageSmoothingQuality::Low;

    else if (value_string == "medium"sv)
        cpp_value = ImageSmoothingQuality::Medium;

    else if (value_string == "high"sv)
        cpp_value = ImageSmoothingQuality::High;

    else
        return JS::js_undefined();

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_image_smoothing_quality(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::create_linear_gradient)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::create_linear_gradient");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "createLinearGradient", "4");

    auto arg0 = vm.argument(0);

    double x0 = TRY(arg0.to_double(vm));

    if (isinf(x0) || isnan(x0)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x0");
    }
    
    auto arg1 = vm.argument(1);

    double y0 = TRY(arg1.to_double(vm));

    if (isinf(y0) || isnan(y0)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y0");
    }
    
    auto arg2 = vm.argument(2);

    double x1 = TRY(arg2.to_double(vm));

    if (isinf(x1) || isnan(x1)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x1");
    }
    
    auto arg3 = vm.argument(3);

    double y1 = TRY(arg3.to_double(vm));

    if (isinf(y1) || isnan(y1)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y1");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_linear_gradient(x0, y0, x1, y1); }));

    return &const_cast<CanvasGradient&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::create_radial_gradient)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::create_radial_gradient");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 6)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "createRadialGradient", "6");

    auto arg0 = vm.argument(0);

    double x0 = TRY(arg0.to_double(vm));

    if (isinf(x0) || isnan(x0)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x0");
    }
    
    auto arg1 = vm.argument(1);

    double y0 = TRY(arg1.to_double(vm));

    if (isinf(y0) || isnan(y0)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y0");
    }
    
    auto arg2 = vm.argument(2);

    double r0 = TRY(arg2.to_double(vm));

    if (isinf(r0) || isnan(r0)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "r0");
    }
    
    auto arg3 = vm.argument(3);

    double x1 = TRY(arg3.to_double(vm));

    if (isinf(x1) || isnan(x1)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x1");
    }
    
    auto arg4 = vm.argument(4);

    double y1 = TRY(arg4.to_double(vm));

    if (isinf(y1) || isnan(y1)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y1");
    }
    
    auto arg5 = vm.argument(5);

    double r1 = TRY(arg5.to_double(vm));

    if (isinf(r1) || isnan(r1)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "r1");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_radial_gradient(x0, y0, r0, x1, y1, r1); }));

    return &const_cast<CanvasGradient&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::create_conic_gradient)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::create_conic_gradient");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "createConicGradient", "3");

    auto arg0 = vm.argument(0);

    double start_angle = TRY(arg0.to_double(vm));

    if (isinf(start_angle) || isnan(start_angle)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "startAngle");
    }
    
    auto arg1 = vm.argument(1);

    double x = TRY(arg1.to_double(vm));

    if (isinf(x) || isnan(x)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
    auto arg2 = vm.argument(2);

    double y = TRY(arg2.to_double(vm));

    if (isinf(y) || isnan(y)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_conic_gradient(start_angle, x, y); }));

    return &const_cast<CanvasGradient&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::create_pattern)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::create_pattern");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "createPattern", "2");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<HTMLImageElement>, GC::Root<SVGImageElement>, GC::Root<HTMLVideoElement>, GC::Root<HTMLCanvasElement>, GC::Root<ImageBitmap>, GC::Root<OffscreenCanvas>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<HTMLImageElement>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<SVGImageElement>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<HTMLVideoElement>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<HTMLCanvasElement>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<ImageBitmap>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<OffscreenCanvas>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<HTMLImageElement>, GC::Root<SVGImageElement>, GC::Root<HTMLVideoElement>, GC::Root<HTMLCanvasElement>, GC::Root<ImageBitmap>, GC::Root<OffscreenCanvas>> image = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    String repetition;
    if (!true || !arg1.is_null()) {
        repetition = TRY(WebIDL::to_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_pattern(image, repetition); }));

    if (retval) {

    return &const_cast<CanvasPattern&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::get_context_attributes)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::get_context_attributes");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_context_attributes(); }));

    {
        auto dictionary_object0 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_alpha0_value;

    wrapped_alpha0_value = JS::Value(retval.alpha);

        MUST(dictionary_object0->create_data_property("alpha"_utf16_fly_string, wrapped_alpha0_value));

        JS::Value wrapped_color_space0_value;

    wrapped_color_space0_value = JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval.color_space));

        MUST(dictionary_object0->create_data_property("colorSpace"_utf16_fly_string, wrapped_color_space0_value));

        JS::Value wrapped_color_type0_value;

    wrapped_color_type0_value = JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval.color_type));

        MUST(dictionary_object0->create_data_property("colorType"_utf16_fly_string, wrapped_color_type0_value));

        JS::Value wrapped_desynchronized0_value;

    wrapped_desynchronized0_value = JS::Value(retval.desynchronized);

        MUST(dictionary_object0->create_data_property("desynchronized"_utf16_fly_string, wrapped_desynchronized0_value));

        JS::Value wrapped_will_read_frequently0_value;

    wrapped_will_read_frequently0_value = JS::Value(retval.will_read_frequently);

        MUST(dictionary_object0->create_data_property("willReadFrequently"_utf16_fly_string, wrapped_will_read_frequently0_value));

        return dictionary_object0;
    }

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::set_line_dash)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::set_line_dash");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setLineDash");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg0);

    auto arg0_iterator_method0 = TRY(arg0.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg0_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg0);

    auto arg0_iterator1 = TRY(JS::get_iterator_from_method(vm, arg0, *arg0_iterator_method0));

    Vector<double> segments;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg0_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    double sequence_item1 = TRY(next_item1.to_double(vm));

    segments.append(sequence_item1);
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_line_dash(segments); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::get_line_dash)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::get_line_dash");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_line_dash(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);
JS::Value wrapped_element0;

    wrapped_element0 = JS::Value(element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::create_image_data0)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::create_image_data0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::Long sw;

    sw = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long sh;

    sh = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    if (!arg2.is_nullish() && !arg2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ImageDataSettings");

    ImageDataSettings settings {};

    auto color_space_property_value_0 = JS::js_undefined();
    if (arg2.is_object())
        color_space_property_value_0 = TRY(arg2.as_object().get("colorSpace"_utf16_fly_string));

    if (!color_space_property_value_0.is_undefined()) {

    PredefinedColorSpace color_space_value_0 { PredefinedColorSpace::Srgb };

    if (!color_space_property_value_0.is_undefined()) {

    auto color_space_property_value_0_string = TRY(color_space_property_value_0.to_string(vm));

    if (color_space_property_value_0_string == "srgb"sv)
        color_space_value_0 = PredefinedColorSpace::Srgb;

    else if (color_space_property_value_0_string == "display-p3"sv)
        color_space_value_0 = PredefinedColorSpace::DisplayP3;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, color_space_property_value_0_string, "PredefinedColorSpace");

    }

    settings.color_space = color_space_value_0;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_image_data(sw, sh, settings); }));

    return &const_cast<ImageData&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::create_image_data1)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::create_image_data1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<ImageData>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ImageData");

    auto& image_data = static_cast<ImageData&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_image_data(image_data); }));

    return &const_cast<ImageData&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::get_image_data)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::get_image_data");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getImageData", "4");

    auto arg0 = vm.argument(0);

    WebIDL::Long sx;

    sx = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long sy;

    sy = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long sw;

    sw = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long sh;

    sh = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    if (!arg4.is_nullish() && !arg4.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ImageDataSettings");

    ImageDataSettings settings {};

    auto color_space_property_value_1 = JS::js_undefined();
    if (arg4.is_object())
        color_space_property_value_1 = TRY(arg4.as_object().get("colorSpace"_utf16_fly_string));

    if (!color_space_property_value_1.is_undefined()) {

    PredefinedColorSpace color_space_value_1 { PredefinedColorSpace::Srgb };

    if (!color_space_property_value_1.is_undefined()) {

    auto color_space_property_value_1_string = TRY(color_space_property_value_1.to_string(vm));

    if (color_space_property_value_1_string == "srgb"sv)
        color_space_value_1 = PredefinedColorSpace::Srgb;

    else if (color_space_property_value_1_string == "display-p3"sv)
        color_space_value_1 = PredefinedColorSpace::DisplayP3;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, color_space_property_value_1_string, "PredefinedColorSpace");

    }

    settings.color_space = color_space_value_1;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_image_data(sx, sy, sw, sh, settings); }));

    return &const_cast<ImageData&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::put_image_data0)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::put_image_data0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<ImageData>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ImageData");

    auto& image_data = static_cast<ImageData&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::Long dx;

    dx = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long dy;

    dy = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->put_image_data(image_data, dx, dy); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::put_image_data1)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::put_image_data1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<ImageData>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ImageData");

    auto& image_data = static_cast<ImageData&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::Long dx;

    dx = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long dy;

    dy = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long dirty_x;

    dirty_x = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::Long dirty_y;

    dirty_y = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    auto arg5 = vm.argument(5);

    WebIDL::Long dirty_width;

    dirty_width = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg5, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    auto arg6 = vm.argument(6);

    WebIDL::Long dirty_height;

    dirty_height = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg6, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->put_image_data(image_data, dx, dy, dirty_x, dirty_y, dirty_width, dirty_height); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::scale)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::scale");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "scale", "2");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scale(x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::rotate)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::rotate");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "rotate");

    auto arg0 = vm.argument(0);

    double radians = TRY(arg0.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->rotate(radians); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::translate)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::translate");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "translate", "2");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->translate(x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::transform)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::transform");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 6)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "transform", "6");

    auto arg0 = vm.argument(0);

    double a = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double b = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double c = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double d = TRY(arg3.to_double(vm));

    auto arg4 = vm.argument(4);

    double e = TRY(arg4.to_double(vm));

    auto arg5 = vm.argument(5);

    double f = TRY(arg5.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->transform(a, b, c, d, e, f); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::get_transform)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::get_transform");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_transform(); }));

    return &const_cast<DOMMatrix&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::set_transform0)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::set_transform0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    double a = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double b = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double c = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double d = TRY(arg3.to_double(vm));

    auto arg4 = vm.argument(4);

    double e = TRY(arg4.to_double(vm));

    auto arg5 = vm.argument(5);

    double f = TRY(arg5.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_transform(a, b, c, d, e, f); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::set_transform1)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::set_transform1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMMatrix2DInit");

    DOMMatrix2DInit transform {};

    auto a_property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        a_property_value_2 = TRY(arg0.as_object().get("a"_utf16_fly_string));

    if (!a_property_value_2.is_undefined()) {

    Optional<double> a_value_2;

    if (!a_property_value_2.is_undefined())
        a_value_2 = TRY(a_property_value_2.to_double(vm));


    transform.a = a_value_2;

    }

    auto b_property_value_3 = JS::js_undefined();
    if (arg0.is_object())
        b_property_value_3 = TRY(arg0.as_object().get("b"_utf16_fly_string));

    if (!b_property_value_3.is_undefined()) {

    Optional<double> b_value_3;

    if (!b_property_value_3.is_undefined())
        b_value_3 = TRY(b_property_value_3.to_double(vm));


    transform.b = b_value_3;

    }

    auto c_property_value_4 = JS::js_undefined();
    if (arg0.is_object())
        c_property_value_4 = TRY(arg0.as_object().get("c"_utf16_fly_string));

    if (!c_property_value_4.is_undefined()) {

    Optional<double> c_value_4;

    if (!c_property_value_4.is_undefined())
        c_value_4 = TRY(c_property_value_4.to_double(vm));


    transform.c = c_value_4;

    }

    auto d_property_value_5 = JS::js_undefined();
    if (arg0.is_object())
        d_property_value_5 = TRY(arg0.as_object().get("d"_utf16_fly_string));

    if (!d_property_value_5.is_undefined()) {

    Optional<double> d_value_5;

    if (!d_property_value_5.is_undefined())
        d_value_5 = TRY(d_property_value_5.to_double(vm));


    transform.d = d_value_5;

    }

    auto e_property_value_6 = JS::js_undefined();
    if (arg0.is_object())
        e_property_value_6 = TRY(arg0.as_object().get("e"_utf16_fly_string));

    if (!e_property_value_6.is_undefined()) {

    Optional<double> e_value_6;

    if (!e_property_value_6.is_undefined())
        e_value_6 = TRY(e_property_value_6.to_double(vm));


    transform.e = e_value_6;

    }

    auto f_property_value_7 = JS::js_undefined();
    if (arg0.is_object())
        f_property_value_7 = TRY(arg0.as_object().get("f"_utf16_fly_string));

    if (!f_property_value_7.is_undefined()) {

    Optional<double> f_value_7;

    if (!f_property_value_7.is_undefined())
        f_value_7 = TRY(f_property_value_7.to_double(vm));


    transform.f = f_value_7;

    }

    auto m11_property_value_8 = JS::js_undefined();
    if (arg0.is_object())
        m11_property_value_8 = TRY(arg0.as_object().get("m11"_utf16_fly_string));

    if (!m11_property_value_8.is_undefined()) {

    Optional<double> m11_value_8;

    if (!m11_property_value_8.is_undefined())
        m11_value_8 = TRY(m11_property_value_8.to_double(vm));


    transform.m11 = m11_value_8;

    }

    auto m12_property_value_9 = JS::js_undefined();
    if (arg0.is_object())
        m12_property_value_9 = TRY(arg0.as_object().get("m12"_utf16_fly_string));

    if (!m12_property_value_9.is_undefined()) {

    Optional<double> m12_value_9;

    if (!m12_property_value_9.is_undefined())
        m12_value_9 = TRY(m12_property_value_9.to_double(vm));


    transform.m12 = m12_value_9;

    }

    auto m21_property_value_10 = JS::js_undefined();
    if (arg0.is_object())
        m21_property_value_10 = TRY(arg0.as_object().get("m21"_utf16_fly_string));

    if (!m21_property_value_10.is_undefined()) {

    Optional<double> m21_value_10;

    if (!m21_property_value_10.is_undefined())
        m21_value_10 = TRY(m21_property_value_10.to_double(vm));


    transform.m21 = m21_value_10;

    }

    auto m22_property_value_11 = JS::js_undefined();
    if (arg0.is_object())
        m22_property_value_11 = TRY(arg0.as_object().get("m22"_utf16_fly_string));

    if (!m22_property_value_11.is_undefined()) {

    Optional<double> m22_value_11;

    if (!m22_property_value_11.is_undefined())
        m22_value_11 = TRY(m22_property_value_11.to_double(vm));


    transform.m22 = m22_value_11;

    }

    auto m41_property_value_12 = JS::js_undefined();
    if (arg0.is_object())
        m41_property_value_12 = TRY(arg0.as_object().get("m41"_utf16_fly_string));

    if (!m41_property_value_12.is_undefined()) {

    Optional<double> m41_value_12;

    if (!m41_property_value_12.is_undefined())
        m41_value_12 = TRY(m41_property_value_12.to_double(vm));


    transform.m41 = m41_value_12;

    }

    auto m42_property_value_13 = JS::js_undefined();
    if (arg0.is_object())
        m42_property_value_13 = TRY(arg0.as_object().get("m42"_utf16_fly_string));

    if (!m42_property_value_13.is_undefined()) {

    Optional<double> m42_value_13;

    if (!m42_property_value_13.is_undefined())
        m42_value_13 = TRY(m42_property_value_13.to_double(vm));


    transform.m42 = m42_value_13;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_transform(transform); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::reset_transform)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::reset_transform");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->reset_transform(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::clear_rect)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::clear_rect");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "clearRect", "4");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double w = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double h = TRY(arg3.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear_rect(x, y, w, h); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::fill_rect)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::fill_rect");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "fillRect", "4");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double w = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double h = TRY(arg3.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->fill_rect(x, y, w, h); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::stroke_rect)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::stroke_rect");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "strokeRect", "4");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double w = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double h = TRY(arg3.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->stroke_rect(x, y, w, h); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::draw_image0)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::draw_image0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<HTMLImageElement>, GC::Root<SVGImageElement>, GC::Root<HTMLVideoElement>, GC::Root<HTMLCanvasElement>, GC::Root<ImageBitmap>, GC::Root<OffscreenCanvas>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<HTMLImageElement>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<SVGImageElement>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<HTMLVideoElement>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<HTMLCanvasElement>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<ImageBitmap>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<OffscreenCanvas>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<HTMLImageElement>, GC::Root<SVGImageElement>, GC::Root<HTMLVideoElement>, GC::Root<HTMLCanvasElement>, GC::Root<ImageBitmap>, GC::Root<OffscreenCanvas>> image = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    double dx = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double dy = TRY(arg2.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->draw_image(image, dx, dy); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::draw_image1)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::draw_image1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<HTMLImageElement>, GC::Root<SVGImageElement>, GC::Root<HTMLVideoElement>, GC::Root<HTMLCanvasElement>, GC::Root<ImageBitmap>, GC::Root<OffscreenCanvas>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<HTMLImageElement>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<SVGImageElement>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<HTMLVideoElement>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<HTMLCanvasElement>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<ImageBitmap>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<OffscreenCanvas>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<HTMLImageElement>, GC::Root<SVGImageElement>, GC::Root<HTMLVideoElement>, GC::Root<HTMLCanvasElement>, GC::Root<ImageBitmap>, GC::Root<OffscreenCanvas>> image = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    double dx = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double dy = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double dw = TRY(arg3.to_double(vm));

    auto arg4 = vm.argument(4);

    double dh = TRY(arg4.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->draw_image(image, dx, dy, dw, dh); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::draw_image2)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::draw_image2");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<HTMLImageElement>, GC::Root<SVGImageElement>, GC::Root<HTMLVideoElement>, GC::Root<HTMLCanvasElement>, GC::Root<ImageBitmap>, GC::Root<OffscreenCanvas>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<HTMLImageElement>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<SVGImageElement>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<HTMLVideoElement>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<HTMLCanvasElement>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<ImageBitmap>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<OffscreenCanvas>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<HTMLImageElement>, GC::Root<SVGImageElement>, GC::Root<HTMLVideoElement>, GC::Root<HTMLCanvasElement>, GC::Root<ImageBitmap>, GC::Root<OffscreenCanvas>> image = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    double sx = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double sy = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double sw = TRY(arg3.to_double(vm));

    auto arg4 = vm.argument(4);

    double sh = TRY(arg4.to_double(vm));

    auto arg5 = vm.argument(5);

    double dx = TRY(arg5.to_double(vm));

    auto arg6 = vm.argument(6);

    double dy = TRY(arg6.to_double(vm));

    auto arg7 = vm.argument(7);

    double dw = TRY(arg7.to_double(vm));

    auto arg8 = vm.argument(8);

    double dh = TRY(arg8.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->draw_image(image, sx, sy, sw, sh, dx, dy, dw, dh); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::save)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::save");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->save(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::restore)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::restore");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->restore(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::reset)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::reset");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->reset(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::is_context_lost)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::is_context_lost");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_context_lost(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::close_path)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::close_path");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->close_path(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::move_to)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::move_to");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "moveTo", "2");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->move_to(x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::line_to)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::line_to");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "lineTo", "2");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->line_to(x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::quadratic_curve_to)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::quadratic_curve_to");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "quadraticCurveTo", "4");

    auto arg0 = vm.argument(0);

    double cpx = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double cpy = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double x = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double y = TRY(arg3.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->quadratic_curve_to(cpx, cpy, x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::bezier_curve_to)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::bezier_curve_to");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 6)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "bezierCurveTo", "6");

    auto arg0 = vm.argument(0);

    double cp1x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double cp1y = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double cp2x = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double cp2y = TRY(arg3.to_double(vm));

    auto arg4 = vm.argument(4);

    double x = TRY(arg4.to_double(vm));

    auto arg5 = vm.argument(5);

    double y = TRY(arg5.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bezier_curve_to(cp1x, cp1y, cp2x, cp2y, x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::arc_to)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::arc_to");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "arcTo", "5");

    auto arg0 = vm.argument(0);

    double x1 = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y1 = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double x2 = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double y2 = TRY(arg3.to_double(vm));

    auto arg4 = vm.argument(4);

    double radius = TRY(arg4.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->arc_to(x1, y1, x2, y2, radius); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::rect)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::rect");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "rect", "4");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double w = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double h = TRY(arg3.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->rect(x, y, w, h); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::round_rect)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::round_rect");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "roundRect", "4");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double w = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double h = TRY(arg3.to_double(vm));

    auto arg4 = vm.argument(4);

    auto arg4_to_dictionary = [&vm, &realm](JS::Value arg4) -> JS::ThrowCompletionOr<DOMPointInit> {
        // This might be unused.
        (void)realm;

    if (!arg4.is_nullish() && !arg4.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMPointInit");

    DOMPointInit dictionary_union_type {};

    auto w_property_value_0 = JS::js_undefined();
    if (arg4.is_object())
        w_property_value_0 = TRY(arg4.as_object().get("w"_utf16_fly_string));

    double w_value_0;

    if (!w_property_value_0.is_undefined())
        w_value_0 = TRY(w_property_value_0.to_double(vm));

    else
        w_value_0 = 1;

    dictionary_union_type.w = w_value_0;

    auto x_property_value_1 = JS::js_undefined();
    if (arg4.is_object())
        x_property_value_1 = TRY(arg4.as_object().get("x"_utf16_fly_string));

    double x_value_1;

    if (!x_property_value_1.is_undefined())
        x_value_1 = TRY(x_property_value_1.to_double(vm));

    else
        x_value_1 = 0;

    dictionary_union_type.x = x_value_1;

    auto y_property_value_2 = JS::js_undefined();
    if (arg4.is_object())
        y_property_value_2 = TRY(arg4.as_object().get("y"_utf16_fly_string));

    double y_value_2;

    if (!y_property_value_2.is_undefined())
        y_value_2 = TRY(y_property_value_2.to_double(vm));

    else
        y_value_2 = 0;

    dictionary_union_type.y = y_value_2;

    auto z_property_value_3 = JS::js_undefined();
    if (arg4.is_object())
        z_property_value_3 = TRY(arg4.as_object().get("z"_utf16_fly_string));

    double z_value_3;

    if (!z_property_value_3.is_undefined())
        z_value_3 = TRY(z_property_value_3.to_double(vm));

    else
        z_value_3 = 0;

    dictionary_union_type.z = z_value_3;

        return dictionary_union_type;
    };

    auto arg4_to_variant = [&vm, &realm, &arg4_to_dictionary](JS::Value arg4) -> JS::ThrowCompletionOr<Variant<double, DOMPointInit, Vector<Variant<double, DOMPointInit>>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg4.is_nullish())
            return Variant<double, DOMPointInit, Vector<Variant<double, DOMPointInit>>> { TRY(arg4_to_dictionary(arg4)) };

        if (arg4.is_object()) {
            [[maybe_unused]] auto& arg4_object = arg4.as_object();

        auto method = TRY(arg4.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg4_iterator1 = TRY(JS::get_iterator_from_method(vm, arg4, *method));

    Vector<Variant<double, DOMPointInit>> radii;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg4_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    auto next_item1_to_dictionary = [&vm, &realm](JS::Value next_item1) -> JS::ThrowCompletionOr<DOMPointInit> {
        // This might be unused.
        (void)realm;

    if (!next_item1.is_nullish() && !next_item1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "DOMPointInit");

    DOMPointInit dictionary_union_type {};

    auto w_property_value_4 = JS::js_undefined();
    if (next_item1.is_object())
        w_property_value_4 = TRY(next_item1.as_object().get("w"_utf16_fly_string));

    double w_value_4;

    if (!w_property_value_4.is_undefined())
        w_value_4 = TRY(w_property_value_4.to_double(vm));

    else
        w_value_4 = 1;

    dictionary_union_type.w = w_value_4;

    auto x_property_value_5 = JS::js_undefined();
    if (next_item1.is_object())
        x_property_value_5 = TRY(next_item1.as_object().get("x"_utf16_fly_string));

    double x_value_5;

    if (!x_property_value_5.is_undefined())
        x_value_5 = TRY(x_property_value_5.to_double(vm));

    else
        x_value_5 = 0;

    dictionary_union_type.x = x_value_5;

    auto y_property_value_6 = JS::js_undefined();
    if (next_item1.is_object())
        y_property_value_6 = TRY(next_item1.as_object().get("y"_utf16_fly_string));

    double y_value_6;

    if (!y_property_value_6.is_undefined())
        y_value_6 = TRY(y_property_value_6.to_double(vm));

    else
        y_value_6 = 0;

    dictionary_union_type.y = y_value_6;

    auto z_property_value_7 = JS::js_undefined();
    if (next_item1.is_object())
        z_property_value_7 = TRY(next_item1.as_object().get("z"_utf16_fly_string));

    double z_value_7;

    if (!z_property_value_7.is_undefined())
        z_value_7 = TRY(z_property_value_7.to_double(vm));

    else
        z_value_7 = 0;

    dictionary_union_type.z = z_value_7;

        return dictionary_union_type;
    };

    auto next_item1_to_variant = [&vm, &realm, &next_item1_to_dictionary](JS::Value next_item1) -> JS::ThrowCompletionOr<Variant<double, DOMPointInit>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (next_item1.is_nullish())
            return Variant<double, DOMPointInit> { TRY(next_item1_to_dictionary(next_item1)) };

        if (next_item1.is_object()) {
            [[maybe_unused]] auto& next_item1_object = next_item1.as_object();

        return Variant<double, DOMPointInit> { TRY(next_item1_to_dictionary(next_item1)) };

        }

        if (next_item1.is_number()) {

    double next_item1_number = TRY(next_item1.to_double(vm));

            return { next_item1_number };
        }

    double next_item1_number = TRY(next_item1.to_double(vm));

        return { next_item1_number };

    };

    Variant<double, DOMPointInit> sequence_item1 = TRY(next_item1_to_variant(next_item1));

    radii.append(sequence_item1);
    }


            return radii;
        }

        return Variant<double, DOMPointInit, Vector<Variant<double, DOMPointInit>>> { TRY(arg4_to_dictionary(arg4)) };

        }

        if (arg4.is_number()) {

    double arg4_number = TRY(arg4.to_double(vm));

            return { arg4_number };
        }

    double arg4_number = TRY(arg4.to_double(vm));

        return { arg4_number };

    };

    Variant<double, DOMPointInit, Vector<Variant<double, DOMPointInit>>> radii = arg4.is_undefined() ? 0 : TRY(arg4_to_variant(arg4));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->round_rect(x, y, w, h, radii); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::arc)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::arc");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "arc", "5");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double radius = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double start_angle = TRY(arg3.to_double(vm));

    auto arg4 = vm.argument(4);

    double end_angle = TRY(arg4.to_double(vm));

    auto arg5 = vm.argument(5);

    bool counterclockwise;

    if (!arg5.is_undefined())

    counterclockwise = arg5.to_boolean();

    else
        counterclockwise = static_cast<bool>(false);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->arc(x, y, radius, start_angle, end_angle, counterclockwise); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::ellipse)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::ellipse");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 7)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "ellipse", "7");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double radius_x = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    double radius_y = TRY(arg3.to_double(vm));

    auto arg4 = vm.argument(4);

    double rotation = TRY(arg4.to_double(vm));

    auto arg5 = vm.argument(5);

    double start_angle = TRY(arg5.to_double(vm));

    auto arg6 = vm.argument(6);

    double end_angle = TRY(arg6.to_double(vm));

    auto arg7 = vm.argument(7);

    bool counterclockwise;

    if (!arg7.is_undefined())

    counterclockwise = arg7.to_boolean();

    else
        counterclockwise = static_cast<bool>(false);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ellipse(x, y, radius_x, radius_y, rotation, start_angle, end_angle, counterclockwise); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::fill_text)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::fill_text");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "fillText", "3");

    auto arg0 = vm.argument(0);

    Utf16String text;
    if (!false || !arg0.is_null()) {
        text = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    double x = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double y = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    Optional<double> max_width;

    if (!arg3.is_undefined())
        max_width = TRY(arg3.to_double(vm));


    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->fill_text(text, x, y, max_width); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::stroke_text)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::stroke_text");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "strokeText", "3");

    auto arg0 = vm.argument(0);

    Utf16String text;
    if (!false || !arg0.is_null()) {
        text = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    double x = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double y = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    Optional<double> max_width;

    if (!arg3.is_undefined())
        max_width = TRY(arg3.to_double(vm));


    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->stroke_text(text, x, y, max_width); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::measure_text)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::measure_text");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "measureText");

    auto arg0 = vm.argument(0);

    Utf16String text;
    if (!false || !arg0.is_null()) {
        text = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->measure_text(text); }));

    return &const_cast<TextMetrics&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::begin_path)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::begin_path");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->begin_path(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::fill0)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::fill0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    String fill_rule;

    if (!arg0.is_undefined()) {
        if (!false || !arg0.is_null())
            fill_rule = TRY(WebIDL::to_string(vm, arg0));
    } else {
        fill_rule = "nonzero"_string;
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->fill(fill_rule); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::fill1)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::fill1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Path2D>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Path2D");

    auto& path = static_cast<Path2D&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    String fill_rule;

    if (!arg1.is_undefined()) {
        if (!false || !arg1.is_null())
            fill_rule = TRY(WebIDL::to_string(vm, arg1));
    } else {
        fill_rule = "nonzero"_string;
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->fill(path, fill_rule); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::stroke0)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::stroke0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->stroke(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::stroke1)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::stroke1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Path2D>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Path2D");

    auto& path = static_cast<Path2D&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->stroke(path); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::clip0)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::clip0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    String fill_rule;

    if (!arg0.is_undefined()) {
        if (!false || !arg0.is_null())
            fill_rule = TRY(WebIDL::to_string(vm, arg0));
    } else {
        fill_rule = "nonzero"_string;
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clip(fill_rule); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::clip1)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::clip1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Path2D>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Path2D");

    auto& path = static_cast<Path2D&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    String fill_rule;

    if (!arg1.is_undefined()) {
        if (!false || !arg1.is_null())
            fill_rule = TRY(WebIDL::to_string(vm, arg1));
    } else {
        fill_rule = "nonzero"_string;
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clip(path, fill_rule); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::is_point_in_path0)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::is_point_in_path0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    String fill_rule;

    if (!arg2.is_undefined()) {
        if (!false || !arg2.is_null())
            fill_rule = TRY(WebIDL::to_string(vm, arg2));
    } else {
        fill_rule = "nonzero"_string;
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_point_in_path(x, y, fill_rule); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::is_point_in_path1)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::is_point_in_path1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Path2D>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Path2D");

    auto& path = static_cast<Path2D&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    double x = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    double y = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    String fill_rule;

    if (!arg3.is_undefined()) {
        if (!false || !arg3.is_null())
            fill_rule = TRY(WebIDL::to_string(vm, arg3));
    } else {
        fill_rule = "nonzero"_string;
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_point_in_path(path, x, y, fill_rule); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::create_image_data)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::create_image_data");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(3, vm.argument_count())) {

    case 1:
        chosen_overload_callable_id = 1;
        break;


    case 2:
        chosen_overload_callable_id = 0;
        break;


    case 3:
        chosen_overload_callable_id = 0;
        break;


    }

    Vector<StringView> dictionary_types {
    "ImageDataSettings"sv,
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return create_image_data0(vm);

    case 1:

        return create_image_data1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::put_image_data)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::put_image_data");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(7, vm.argument_count())) {

    case 3:
        chosen_overload_callable_id = 0;
        break;


    case 7:
        chosen_overload_callable_id = 1;
        break;


    }

    Vector<StringView> dictionary_types {
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return put_image_data0(vm);

    case 1:

        return put_image_data1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::set_transform)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::set_transform");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(6, vm.argument_count())) {

    case 0:
        chosen_overload_callable_id = 1;
        break;


    case 1:
        chosen_overload_callable_id = 1;
        break;


    case 6:
        chosen_overload_callable_id = 0;
        break;


    }

    Vector<StringView> dictionary_types {
    "DOMMatrix2DInit"sv,
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return set_transform0(vm);

    case 1:

        return set_transform1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::draw_image)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::draw_image");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(9, vm.argument_count())) {

    case 3:
        chosen_overload_callable_id = 0;
        break;


    case 5:
        chosen_overload_callable_id = 1;
        break;


    case 9:
        chosen_overload_callable_id = 2;
        break;


    }

    Vector<StringView> dictionary_types {
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return draw_image0(vm);

    case 1:

        return draw_image1(vm);

    case 2:

        return draw_image2(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::fill)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::fill");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(2, vm.argument_count())) {

    case 0:
        chosen_overload_callable_id = 0;
        break;


    case 1: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("DOMString", false)}, Vector<IDL::Optionality> { IDL::Optionality::Optional});
        overloads.empend(1, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("Path2D", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required});

        effective_overload_set.emplace(move(overloads), 0);
        break;
    }

    case 2:
        chosen_overload_callable_id = 1;
        break;


    }

    Vector<StringView> dictionary_types {
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return fill0(vm);

    case 1:

        return fill1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::stroke)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::stroke");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(1, vm.argument_count())) {

    case 0:
        chosen_overload_callable_id = 0;
        break;


    case 1:
        chosen_overload_callable_id = 1;
        break;


    }

    Vector<StringView> dictionary_types {
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return stroke0(vm);

    case 1:

        return stroke1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::clip)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::clip");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(2, vm.argument_count())) {

    case 0:
        chosen_overload_callable_id = 0;
        break;


    case 1: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("DOMString", false)}, Vector<IDL::Optionality> { IDL::Optionality::Optional});
        overloads.empend(1, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("Path2D", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required});

        effective_overload_set.emplace(move(overloads), 0);
        break;
    }

    case 2:
        chosen_overload_callable_id = 1;
        break;


    }

    Vector<StringView> dictionary_types {
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return clip0(vm);

    case 1:

        return clip1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(OffscreenCanvasRenderingContext2DPrototype::is_point_in_path)
{
    WebIDL::log_trace(vm, "OffscreenCanvasRenderingContext2DPrototype::is_point_in_path");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(4, vm.argument_count())) {

    case 2:
        chosen_overload_callable_id = 0;
        break;


    case 3: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("unrestricted double", false), make_ref_counted<IDL::Type>("unrestricted double", false), make_ref_counted<IDL::Type>("DOMString", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Optional});
        overloads.empend(1, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("Path2D", false), make_ref_counted<IDL::Type>("unrestricted double", false), make_ref_counted<IDL::Type>("unrestricted double", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required});

        effective_overload_set.emplace(move(overloads), 0);
        break;
    }

    case 4:
        chosen_overload_callable_id = 1;
        break;


    }

    Vector<StringView> dictionary_types {
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return is_point_in_path0(vm);

    case 1:

        return is_point_in_path1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

} // namespace Web::Bindings
