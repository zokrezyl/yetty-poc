
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
#include <LibWeb/Bindings/WebGLRenderingContextPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLRenderingContext.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLCanvasElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLRenderingContextBase.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CanvasRenderingContext2D.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGL2RenderingContext.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLQuery.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLSampler.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLSync.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLTransformFeedback.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLVertexArrayObject.h>

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

GC_DEFINE_ALLOCATOR(WebGLRenderingContextPrototype);

WebGLRenderingContextPrototype::WebGLRenderingContextPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

WebGLRenderingContextPrototype::~WebGLRenderingContextPrototype()
{
}

void WebGLRenderingContextPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_canvas_getter = JS::NativeFunction::create(realm, canvas_getter, 0, "canvas"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_canvas_setter;

    define_direct_accessor("canvas"_utf16_fly_string, native_canvas_getter, native_canvas_setter, default_attributes);

    auto native_drawing_buffer_width_getter = JS::NativeFunction::create(realm, drawing_buffer_width_getter, 0, "drawingBufferWidth"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_drawing_buffer_width_setter;

    define_direct_accessor("drawingBufferWidth"_utf16_fly_string, native_drawing_buffer_width_getter, native_drawing_buffer_width_setter, default_attributes);

    auto native_drawing_buffer_height_getter = JS::NativeFunction::create(realm, drawing_buffer_height_getter, 0, "drawingBufferHeight"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_drawing_buffer_height_setter;

    define_direct_accessor("drawingBufferHeight"_utf16_fly_string, native_drawing_buffer_height_getter, native_drawing_buffer_height_setter, default_attributes);

    define_direct_property("drawingBufferFormat"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_direct_property("drawingBufferColorSpace"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    define_direct_property("unpackColorSpace"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("getFramebufferAttachmentParameter"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
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

    define_native_function(realm, "bufferData"_utf16_fly_string, buffer_data, 3, default_attributes);

    define_native_function(realm, "bufferSubData"_utf16_fly_string, buffer_sub_data, 3, default_attributes);

    define_native_function(realm, "compressedTexImage2D"_utf16_fly_string, compressed_tex_image2d, 7, default_attributes);

    define_native_function(realm, "compressedTexSubImage2D"_utf16_fly_string, compressed_tex_sub_image2d, 8, default_attributes);

    define_native_function(realm, "readPixels"_utf16_fly_string, read_pixels, 7, default_attributes);

    define_native_function(realm, "texImage2D"_utf16_fly_string, tex_image2d, 6, default_attributes);

    define_native_function(realm, "texSubImage2D"_utf16_fly_string, tex_sub_image2d, 7, default_attributes);

    define_native_function(realm, "uniform1fv"_utf16_fly_string, uniform1fv, 2, default_attributes);

    define_native_function(realm, "uniform2fv"_utf16_fly_string, uniform2fv, 2, default_attributes);

    define_native_function(realm, "uniform3fv"_utf16_fly_string, uniform3fv, 2, default_attributes);

    define_native_function(realm, "uniform4fv"_utf16_fly_string, uniform4fv, 2, default_attributes);

    define_native_function(realm, "uniform1iv"_utf16_fly_string, uniform1iv, 2, default_attributes);

    define_native_function(realm, "uniform2iv"_utf16_fly_string, uniform2iv, 2, default_attributes);

    define_native_function(realm, "uniform3iv"_utf16_fly_string, uniform3iv, 2, default_attributes);

    define_native_function(realm, "uniform4iv"_utf16_fly_string, uniform4iv, 2, default_attributes);

    define_native_function(realm, "uniformMatrix2fv"_utf16_fly_string, uniform_matrix2fv, 3, default_attributes);

    define_native_function(realm, "uniformMatrix3fv"_utf16_fly_string, uniform_matrix3fv, 3, default_attributes);

    define_native_function(realm, "uniformMatrix4fv"_utf16_fly_string, uniform_matrix4fv, 3, default_attributes);

    define_native_function(realm, "getContextAttributes"_utf16_fly_string, get_context_attributes, 0, default_attributes);

    define_native_function(realm, "isContextLost"_utf16_fly_string, is_context_lost, 0, default_attributes);

    define_native_function(realm, "getSupportedExtensions"_utf16_fly_string, get_supported_extensions, 0, default_attributes);

    define_native_function(realm, "getExtension"_utf16_fly_string, get_extension, 1, default_attributes);

    define_native_function(realm, "activeTexture"_utf16_fly_string, active_texture, 1, default_attributes);

    define_native_function(realm, "attachShader"_utf16_fly_string, attach_shader, 2, default_attributes);

    define_native_function(realm, "bindAttribLocation"_utf16_fly_string, bind_attrib_location, 3, default_attributes);

    define_native_function(realm, "bindBuffer"_utf16_fly_string, bind_buffer, 2, default_attributes);

    define_native_function(realm, "bindFramebuffer"_utf16_fly_string, bind_framebuffer, 2, default_attributes);

    define_native_function(realm, "bindRenderbuffer"_utf16_fly_string, bind_renderbuffer, 2, default_attributes);

    define_native_function(realm, "bindTexture"_utf16_fly_string, bind_texture, 2, default_attributes);

    define_native_function(realm, "blendColor"_utf16_fly_string, blend_color, 4, default_attributes);

    define_native_function(realm, "blendEquation"_utf16_fly_string, blend_equation, 1, default_attributes);

    define_native_function(realm, "blendEquationSeparate"_utf16_fly_string, blend_equation_separate, 2, default_attributes);

    define_native_function(realm, "blendFunc"_utf16_fly_string, blend_func, 2, default_attributes);

    define_native_function(realm, "blendFuncSeparate"_utf16_fly_string, blend_func_separate, 4, default_attributes);

    define_native_function(realm, "checkFramebufferStatus"_utf16_fly_string, check_framebuffer_status, 1, default_attributes);

    define_native_function(realm, "clear"_utf16_fly_string, clear, 1, default_attributes);

    define_native_function(realm, "clearColor"_utf16_fly_string, clear_color, 4, default_attributes);

    define_native_function(realm, "clearDepth"_utf16_fly_string, clear_depth, 1, default_attributes);

    define_native_function(realm, "clearStencil"_utf16_fly_string, clear_stencil, 1, default_attributes);

    define_native_function(realm, "colorMask"_utf16_fly_string, color_mask, 4, default_attributes);

    define_native_function(realm, "compileShader"_utf16_fly_string, compile_shader, 1, default_attributes);

    define_native_function(realm, "copyTexImage2D"_utf16_fly_string, copy_tex_image2d, 8, default_attributes);

    define_native_function(realm, "copyTexSubImage2D"_utf16_fly_string, copy_tex_sub_image2d, 8, default_attributes);

    define_native_function(realm, "createBuffer"_utf16_fly_string, create_buffer, 0, default_attributes);

    define_native_function(realm, "createFramebuffer"_utf16_fly_string, create_framebuffer, 0, default_attributes);

    define_native_function(realm, "createProgram"_utf16_fly_string, create_program, 0, default_attributes);

    define_native_function(realm, "createRenderbuffer"_utf16_fly_string, create_renderbuffer, 0, default_attributes);

    define_native_function(realm, "createShader"_utf16_fly_string, create_shader, 1, default_attributes);

    define_native_function(realm, "createTexture"_utf16_fly_string, create_texture, 0, default_attributes);

    define_native_function(realm, "cullFace"_utf16_fly_string, cull_face, 1, default_attributes);

    define_native_function(realm, "deleteBuffer"_utf16_fly_string, delete_buffer, 1, default_attributes);

    define_native_function(realm, "deleteFramebuffer"_utf16_fly_string, delete_framebuffer, 1, default_attributes);

    define_native_function(realm, "deleteProgram"_utf16_fly_string, delete_program, 1, default_attributes);

    define_native_function(realm, "deleteRenderbuffer"_utf16_fly_string, delete_renderbuffer, 1, default_attributes);

    define_native_function(realm, "deleteShader"_utf16_fly_string, delete_shader, 1, default_attributes);

    define_native_function(realm, "deleteTexture"_utf16_fly_string, delete_texture, 1, default_attributes);

    define_native_function(realm, "depthFunc"_utf16_fly_string, depth_func, 1, default_attributes);

    define_native_function(realm, "depthMask"_utf16_fly_string, depth_mask, 1, default_attributes);

    define_native_function(realm, "depthRange"_utf16_fly_string, depth_range, 2, default_attributes);

    define_native_function(realm, "detachShader"_utf16_fly_string, detach_shader, 2, default_attributes);

    define_native_function(realm, "disable"_utf16_fly_string, disable, 1, default_attributes);

    define_native_function(realm, "disableVertexAttribArray"_utf16_fly_string, disable_vertex_attrib_array, 1, default_attributes);

    define_native_function(realm, "drawArrays"_utf16_fly_string, draw_arrays, 3, default_attributes);

    define_native_function(realm, "drawElements"_utf16_fly_string, draw_elements, 4, default_attributes);

    define_native_function(realm, "enable"_utf16_fly_string, enable, 1, default_attributes);

    define_native_function(realm, "enableVertexAttribArray"_utf16_fly_string, enable_vertex_attrib_array, 1, default_attributes);

    define_native_function(realm, "finish"_utf16_fly_string, finish, 0, default_attributes);

    define_native_function(realm, "flush"_utf16_fly_string, flush, 0, default_attributes);

    define_native_function(realm, "framebufferRenderbuffer"_utf16_fly_string, framebuffer_renderbuffer, 4, default_attributes);

    define_native_function(realm, "framebufferTexture2D"_utf16_fly_string, framebuffer_texture2d, 5, default_attributes);

    define_native_function(realm, "frontFace"_utf16_fly_string, front_face, 1, default_attributes);

    define_native_function(realm, "generateMipmap"_utf16_fly_string, generate_mipmap, 1, default_attributes);

    define_native_function(realm, "getActiveAttrib"_utf16_fly_string, get_active_attrib, 2, default_attributes);

    define_native_function(realm, "getActiveUniform"_utf16_fly_string, get_active_uniform, 2, default_attributes);

    define_native_function(realm, "getAttachedShaders"_utf16_fly_string, get_attached_shaders, 1, default_attributes);

    define_native_function(realm, "getAttribLocation"_utf16_fly_string, get_attrib_location, 2, default_attributes);

    define_native_function(realm, "getBufferParameter"_utf16_fly_string, get_buffer_parameter, 2, default_attributes);

    define_native_function(realm, "getParameter"_utf16_fly_string, get_parameter, 1, default_attributes);

    define_native_function(realm, "getError"_utf16_fly_string, get_error, 0, default_attributes);

    define_native_function(realm, "getProgramParameter"_utf16_fly_string, get_program_parameter, 2, default_attributes);

    define_native_function(realm, "getProgramInfoLog"_utf16_fly_string, get_program_info_log, 1, default_attributes);

    define_native_function(realm, "getRenderbufferParameter"_utf16_fly_string, get_renderbuffer_parameter, 2, default_attributes);

    define_native_function(realm, "getShaderParameter"_utf16_fly_string, get_shader_parameter, 2, default_attributes);

    define_native_function(realm, "getShaderPrecisionFormat"_utf16_fly_string, get_shader_precision_format, 2, default_attributes);

    define_native_function(realm, "getShaderInfoLog"_utf16_fly_string, get_shader_info_log, 1, default_attributes);

    define_native_function(realm, "getShaderSource"_utf16_fly_string, get_shader_source, 1, default_attributes);

    define_native_function(realm, "getTexParameter"_utf16_fly_string, get_tex_parameter, 2, default_attributes);

    define_native_function(realm, "getUniform"_utf16_fly_string, get_uniform, 2, default_attributes);

    define_native_function(realm, "getUniformLocation"_utf16_fly_string, get_uniform_location, 2, default_attributes);

    define_native_function(realm, "getVertexAttrib"_utf16_fly_string, get_vertex_attrib, 2, default_attributes);

    define_native_function(realm, "getVertexAttribOffset"_utf16_fly_string, get_vertex_attrib_offset, 2, default_attributes);

    define_native_function(realm, "hint"_utf16_fly_string, hint, 2, default_attributes);

    define_native_function(realm, "isBuffer"_utf16_fly_string, is_buffer, 1, default_attributes);

    define_native_function(realm, "isEnabled"_utf16_fly_string, is_enabled, 1, default_attributes);

    define_native_function(realm, "isFramebuffer"_utf16_fly_string, is_framebuffer, 1, default_attributes);

    define_native_function(realm, "isProgram"_utf16_fly_string, is_program, 1, default_attributes);

    define_native_function(realm, "isRenderbuffer"_utf16_fly_string, is_renderbuffer, 1, default_attributes);

    define_native_function(realm, "isShader"_utf16_fly_string, is_shader, 1, default_attributes);

    define_native_function(realm, "isTexture"_utf16_fly_string, is_texture, 1, default_attributes);

    define_native_function(realm, "lineWidth"_utf16_fly_string, line_width, 1, default_attributes);

    define_native_function(realm, "linkProgram"_utf16_fly_string, link_program, 1, default_attributes);

    define_native_function(realm, "pixelStorei"_utf16_fly_string, pixel_storei, 2, default_attributes);

    define_native_function(realm, "polygonOffset"_utf16_fly_string, polygon_offset, 2, default_attributes);

    define_native_function(realm, "renderbufferStorage"_utf16_fly_string, renderbuffer_storage, 4, default_attributes);

    define_native_function(realm, "sampleCoverage"_utf16_fly_string, sample_coverage, 2, default_attributes);

    define_native_function(realm, "scissor"_utf16_fly_string, scissor, 4, default_attributes);

    define_native_function(realm, "shaderSource"_utf16_fly_string, shader_source, 2, default_attributes);

    define_native_function(realm, "stencilFunc"_utf16_fly_string, stencil_func, 3, default_attributes);

    define_native_function(realm, "stencilFuncSeparate"_utf16_fly_string, stencil_func_separate, 4, default_attributes);

    define_native_function(realm, "stencilMask"_utf16_fly_string, stencil_mask, 1, default_attributes);

    define_native_function(realm, "stencilMaskSeparate"_utf16_fly_string, stencil_mask_separate, 2, default_attributes);

    define_native_function(realm, "stencilOp"_utf16_fly_string, stencil_op, 3, default_attributes);

    define_native_function(realm, "stencilOpSeparate"_utf16_fly_string, stencil_op_separate, 4, default_attributes);

    define_native_function(realm, "texParameterf"_utf16_fly_string, tex_parameterf, 3, default_attributes);

    define_native_function(realm, "texParameteri"_utf16_fly_string, tex_parameteri, 3, default_attributes);

    define_native_function(realm, "uniform1f"_utf16_fly_string, uniform1f, 2, default_attributes);

    define_native_function(realm, "uniform2f"_utf16_fly_string, uniform2f, 3, default_attributes);

    define_native_function(realm, "uniform3f"_utf16_fly_string, uniform3f, 4, default_attributes);

    define_native_function(realm, "uniform4f"_utf16_fly_string, uniform4f, 5, default_attributes);

    define_native_function(realm, "uniform1i"_utf16_fly_string, uniform1i, 2, default_attributes);

    define_native_function(realm, "uniform2i"_utf16_fly_string, uniform2i, 3, default_attributes);

    define_native_function(realm, "uniform3i"_utf16_fly_string, uniform3i, 4, default_attributes);

    define_native_function(realm, "uniform4i"_utf16_fly_string, uniform4i, 5, default_attributes);

    define_native_function(realm, "useProgram"_utf16_fly_string, use_program, 1, default_attributes);

    define_native_function(realm, "validateProgram"_utf16_fly_string, validate_program, 1, default_attributes);

    define_native_function(realm, "vertexAttrib1f"_utf16_fly_string, vertex_attrib1f, 2, default_attributes);

    define_native_function(realm, "vertexAttrib2f"_utf16_fly_string, vertex_attrib2f, 3, default_attributes);

    define_native_function(realm, "vertexAttrib3f"_utf16_fly_string, vertex_attrib3f, 4, default_attributes);

    define_native_function(realm, "vertexAttrib4f"_utf16_fly_string, vertex_attrib4f, 5, default_attributes);

    define_native_function(realm, "vertexAttrib1fv"_utf16_fly_string, vertex_attrib1fv, 2, default_attributes);

    define_native_function(realm, "vertexAttrib2fv"_utf16_fly_string, vertex_attrib2fv, 2, default_attributes);

    define_native_function(realm, "vertexAttrib3fv"_utf16_fly_string, vertex_attrib3fv, 2, default_attributes);

    define_native_function(realm, "vertexAttrib4fv"_utf16_fly_string, vertex_attrib4fv, 2, default_attributes);

    define_native_function(realm, "vertexAttribPointer"_utf16_fly_string, vertex_attrib_pointer, 6, default_attributes);

    define_native_function(realm, "viewport"_utf16_fly_string, viewport, 4, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "WebGLRenderingContext"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void WebGLRenderingContextPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<WebGL::WebGLRenderingContext*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<WebGL::WebGLRenderingContext>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLRenderingContext");
    return static_cast<WebGL::WebGLRenderingContext*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::canvas_getter)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::canvas_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->canvas_for_binding(); }));

    return &const_cast<HTMLCanvasElement&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::drawing_buffer_width_getter)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::drawing_buffer_width_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->drawing_buffer_width(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::drawing_buffer_height_getter)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::drawing_buffer_height_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->drawing_buffer_height(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::buffer_data0)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::buffer_data0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::LongLong size;

    size = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong usage;

    usage = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->buffer_data(target, size, usage); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::buffer_data1)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::buffer_data1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    GC::Root<WebIDL::BufferSource> data;

    if (!arg1.is_object() || !(is<JS::TypedArrayBase>(arg1.as_object()) || is<JS::ArrayBuffer>(arg1.as_object()) || is<JS::DataView>(arg1.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BufferSource");

    data = GC::make_root(realm.create<WebIDL::BufferSource>(arg1.as_object()));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong usage;

    usage = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->buffer_data(target, data, usage); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::buffer_sub_data)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::buffer_sub_data");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "bufferSubData", "3");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::LongLong offset;

    offset = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    GC::Root<WebIDL::BufferSource> data;

    if (!arg2.is_object() || !(is<JS::TypedArrayBase>(arg2.as_object()) || is<JS::ArrayBuffer>(arg2.as_object()) || is<JS::DataView>(arg2.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BufferSource");

    data = GC::make_root(realm.create<WebIDL::BufferSource>(arg2.as_object()));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->buffer_sub_data(target, offset, data); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::compressed_tex_image2d)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::compressed_tex_image2d");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 7)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "compressedTexImage2D", "7");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long level;

    level = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong internalformat;

    internalformat = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long width;

    width = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::Long height;

    height = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg5 = vm.argument(5);

    WebIDL::Long border;

    border = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg6 = vm.argument(6);

    GC::Root<WebIDL::ArrayBufferView> data;

        if (!arg6.is_object() || !(is<JS::TypedArrayBase>(arg6.as_object()) || is<JS::DataView>(arg6.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        data = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg6.as_object()));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->compressed_tex_image2d(target, level, internalformat, width, height, border, data); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::compressed_tex_sub_image2d)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::compressed_tex_sub_image2d");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 8)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "compressedTexSubImage2D", "8");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long level;

    level = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long xoffset;

    xoffset = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long yoffset;

    yoffset = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::Long width;

    width = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg5 = vm.argument(5);

    WebIDL::Long height;

    height = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg6 = vm.argument(6);

    WebIDL::UnsignedLong format;

    format = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg6, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg7 = vm.argument(7);

    GC::Root<WebIDL::ArrayBufferView> data;

        if (!arg7.is_object() || !(is<JS::TypedArrayBase>(arg7.as_object()) || is<JS::DataView>(arg7.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        data = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg7.as_object()));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->compressed_tex_sub_image2d(target, level, xoffset, yoffset, width, height, format, data); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::read_pixels)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::read_pixels");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 7)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "readPixels", "7");

    auto arg0 = vm.argument(0);

    WebIDL::Long x;

    x = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long y;

    y = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long width;

    width = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long height;

    height = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong format;

    format = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg5 = vm.argument(5);

    WebIDL::UnsignedLong type;

    type = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg6 = vm.argument(6);

    GC::Root<WebIDL::ArrayBufferView> pixels;

    if (!arg6.is_null() && !arg6.is_undefined()) {

        if (!arg6.is_object() || !(is<JS::TypedArrayBase>(arg6.as_object()) || is<JS::DataView>(arg6.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        pixels = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg6.as_object()));

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->read_pixels(x, y, width, height, format, type, pixels); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::tex_image2d0)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::tex_image2d0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long level;

    level = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long internalformat;

    internalformat = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long width;

    width = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::Long height;

    height = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg5 = vm.argument(5);

    WebIDL::Long border;

    border = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg6 = vm.argument(6);

    WebIDL::UnsignedLong format;

    format = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg6, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg7 = vm.argument(7);

    WebIDL::UnsignedLong type;

    type = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg7, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg8 = vm.argument(8);

    GC::Root<WebIDL::ArrayBufferView> pixels;

    if (!arg8.is_null() && !arg8.is_undefined()) {

        if (!arg8.is_object() || !(is<JS::TypedArrayBase>(arg8.as_object()) || is<JS::DataView>(arg8.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        pixels = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg8.as_object()));

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tex_image2d(target, level, internalformat, width, height, border, format, type, pixels); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::tex_image2d1)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::tex_image2d1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long level;

    level = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long internalformat;

    internalformat = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong format;

    format = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong type;

    type = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg5 = vm.argument(5);

    auto arg5_to_variant = [&vm, &realm](JS::Value arg5) -> JS::ThrowCompletionOr<Variant<GC::Root<ImageBitmap>, GC::Root<ImageData>, GC::Root<HTMLImageElement>, GC::Root<HTMLCanvasElement>, GC::Root<HTMLVideoElement>, GC::Root<OffscreenCanvas>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg5.is_object()) {
            [[maybe_unused]] auto& arg5_object = arg5.as_object();

            if (is<PlatformObject>(arg5_object)) {

                if (auto* arg5_result = as_if<ImageBitmap>(arg5_object))
                    return GC::make_root(*arg5_result);

                if (auto* arg5_result = as_if<ImageData>(arg5_object))
                    return GC::make_root(*arg5_result);

                if (auto* arg5_result = as_if<HTMLImageElement>(arg5_object))
                    return GC::make_root(*arg5_result);

                if (auto* arg5_result = as_if<HTMLCanvasElement>(arg5_object))
                    return GC::make_root(*arg5_result);

                if (auto* arg5_result = as_if<HTMLVideoElement>(arg5_object))
                    return GC::make_root(*arg5_result);

                if (auto* arg5_result = as_if<OffscreenCanvas>(arg5_object))
                    return GC::make_root(*arg5_result);

            }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<ImageBitmap>, GC::Root<ImageData>, GC::Root<HTMLImageElement>, GC::Root<HTMLCanvasElement>, GC::Root<HTMLVideoElement>, GC::Root<OffscreenCanvas>> source = TRY(arg5_to_variant(arg5));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tex_image2d(target, level, internalformat, format, type, source); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::tex_sub_image2d0)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::tex_sub_image2d0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long level;

    level = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long xoffset;

    xoffset = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long yoffset;

    yoffset = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::Long width;

    width = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg5 = vm.argument(5);

    WebIDL::Long height;

    height = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg6 = vm.argument(6);

    WebIDL::UnsignedLong format;

    format = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg6, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg7 = vm.argument(7);

    WebIDL::UnsignedLong type;

    type = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg7, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg8 = vm.argument(8);

    GC::Root<WebIDL::ArrayBufferView> pixels;

    if (!arg8.is_null() && !arg8.is_undefined()) {

        if (!arg8.is_object() || !(is<JS::TypedArrayBase>(arg8.as_object()) || is<JS::DataView>(arg8.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        pixels = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg8.as_object()));

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tex_sub_image2d(target, level, xoffset, yoffset, width, height, format, type, pixels); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::tex_sub_image2d1)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::tex_sub_image2d1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long level;

    level = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long xoffset;

    xoffset = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long yoffset;

    yoffset = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong format;

    format = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg5 = vm.argument(5);

    WebIDL::UnsignedLong type;

    type = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg6 = vm.argument(6);

    auto arg6_to_variant = [&vm, &realm](JS::Value arg6) -> JS::ThrowCompletionOr<Variant<GC::Root<ImageBitmap>, GC::Root<ImageData>, GC::Root<HTMLImageElement>, GC::Root<HTMLCanvasElement>, GC::Root<HTMLVideoElement>, GC::Root<OffscreenCanvas>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg6.is_object()) {
            [[maybe_unused]] auto& arg6_object = arg6.as_object();

            if (is<PlatformObject>(arg6_object)) {

                if (auto* arg6_result = as_if<ImageBitmap>(arg6_object))
                    return GC::make_root(*arg6_result);

                if (auto* arg6_result = as_if<ImageData>(arg6_object))
                    return GC::make_root(*arg6_result);

                if (auto* arg6_result = as_if<HTMLImageElement>(arg6_object))
                    return GC::make_root(*arg6_result);

                if (auto* arg6_result = as_if<HTMLCanvasElement>(arg6_object))
                    return GC::make_root(*arg6_result);

                if (auto* arg6_result = as_if<HTMLVideoElement>(arg6_object))
                    return GC::make_root(*arg6_result);

                if (auto* arg6_result = as_if<OffscreenCanvas>(arg6_object))
                    return GC::make_root(*arg6_result);

            }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<ImageBitmap>, GC::Root<ImageData>, GC::Root<HTMLImageElement>, GC::Root<HTMLCanvasElement>, GC::Root<HTMLVideoElement>, GC::Root<OffscreenCanvas>> source = TRY(arg6_to_variant(arg6));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tex_sub_image2d(target, level, xoffset, yoffset, format, type, source); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform1fv)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform1fv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform1fv", "2");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Float32Array>, Vector<float>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (auto* typed_array = as_if<JS::Float32Array>(arg1_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<float> v;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    v.append(sequence_item1);
    }


            return v;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> v = TRY(arg1_to_variant(arg1));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform1fv(location, v); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform2fv)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform2fv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform2fv", "2");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Float32Array>, Vector<float>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (auto* typed_array = as_if<JS::Float32Array>(arg1_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<float> v;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    v.append(sequence_item1);
    }


            return v;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> v = TRY(arg1_to_variant(arg1));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform2fv(location, v); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform3fv)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform3fv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform3fv", "2");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Float32Array>, Vector<float>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (auto* typed_array = as_if<JS::Float32Array>(arg1_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<float> v;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    v.append(sequence_item1);
    }


            return v;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> v = TRY(arg1_to_variant(arg1));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform3fv(location, v); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform4fv)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform4fv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform4fv", "2");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Float32Array>, Vector<float>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (auto* typed_array = as_if<JS::Float32Array>(arg1_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<float> v;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    v.append(sequence_item1);
    }


            return v;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> v = TRY(arg1_to_variant(arg1));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform4fv(location, v); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform1iv)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform1iv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform1iv", "2");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Int32Array>, Vector<WebIDL::Long>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (auto* typed_array = as_if<JS::Int32Array>(arg1_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<WebIDL::Long> v;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::Long sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    v.append(sequence_item1);
    }


            return v;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Int32Array>, Vector<WebIDL::Long>> v = TRY(arg1_to_variant(arg1));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform1iv(location, v); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform2iv)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform2iv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform2iv", "2");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Int32Array>, Vector<WebIDL::Long>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (auto* typed_array = as_if<JS::Int32Array>(arg1_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<WebIDL::Long> v;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::Long sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    v.append(sequence_item1);
    }


            return v;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Int32Array>, Vector<WebIDL::Long>> v = TRY(arg1_to_variant(arg1));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform2iv(location, v); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform3iv)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform3iv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform3iv", "2");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Int32Array>, Vector<WebIDL::Long>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (auto* typed_array = as_if<JS::Int32Array>(arg1_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<WebIDL::Long> v;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::Long sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    v.append(sequence_item1);
    }


            return v;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Int32Array>, Vector<WebIDL::Long>> v = TRY(arg1_to_variant(arg1));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform3iv(location, v); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform4iv)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform4iv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform4iv", "2");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Int32Array>, Vector<WebIDL::Long>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (auto* typed_array = as_if<JS::Int32Array>(arg1_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<WebIDL::Long> v;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::Long sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    v.append(sequence_item1);
    }


            return v;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Int32Array>, Vector<WebIDL::Long>> v = TRY(arg1_to_variant(arg1));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform4iv(location, v); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform_matrix2fv)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform_matrix2fv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniformMatrix2fv", "3");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    bool transpose;

    transpose = arg1.to_boolean();

    auto arg2 = vm.argument(2);

    auto arg2_to_variant = [&vm, &realm](JS::Value arg2) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Float32Array>, Vector<float>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg2.is_object()) {
            [[maybe_unused]] auto& arg2_object = arg2.as_object();

            if (auto* typed_array = as_if<JS::Float32Array>(arg2_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg2.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg2_iterator1 = TRY(JS::get_iterator_from_method(vm, arg2, *method));

    Vector<float> value;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    value.append(sequence_item1);
    }


            return value;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> value = TRY(arg2_to_variant(arg2));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform_matrix2fv(location, transpose, value); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform_matrix3fv)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform_matrix3fv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniformMatrix3fv", "3");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    bool transpose;

    transpose = arg1.to_boolean();

    auto arg2 = vm.argument(2);

    auto arg2_to_variant = [&vm, &realm](JS::Value arg2) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Float32Array>, Vector<float>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg2.is_object()) {
            [[maybe_unused]] auto& arg2_object = arg2.as_object();

            if (auto* typed_array = as_if<JS::Float32Array>(arg2_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg2.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg2_iterator1 = TRY(JS::get_iterator_from_method(vm, arg2, *method));

    Vector<float> value;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    value.append(sequence_item1);
    }


            return value;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> value = TRY(arg2_to_variant(arg2));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform_matrix3fv(location, transpose, value); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform_matrix4fv)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform_matrix4fv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniformMatrix4fv", "3");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    bool transpose;

    transpose = arg1.to_boolean();

    auto arg2 = vm.argument(2);

    auto arg2_to_variant = [&vm, &realm](JS::Value arg2) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Float32Array>, Vector<float>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg2.is_object()) {
            [[maybe_unused]] auto& arg2_object = arg2.as_object();

            if (auto* typed_array = as_if<JS::Float32Array>(arg2_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg2.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg2_iterator1 = TRY(JS::get_iterator_from_method(vm, arg2, *method));

    Vector<float> value;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    value.append(sequence_item1);
    }


            return value;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> value = TRY(arg2_to_variant(arg2));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform_matrix4fv(location, transpose, value); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_context_attributes)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_context_attributes");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_context_attributes(); }));

    if (retval.has_value()) {

    {
        auto dictionary_object0 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        JS::Value wrapped_alpha0_value;

    wrapped_alpha0_value = JS::Value(retval->alpha);

        MUST(dictionary_object0->create_data_property("alpha"_utf16_fly_string, wrapped_alpha0_value));

        JS::Value wrapped_antialias0_value;

    wrapped_antialias0_value = JS::Value(retval->antialias);

        MUST(dictionary_object0->create_data_property("antialias"_utf16_fly_string, wrapped_antialias0_value));

        JS::Value wrapped_depth0_value;

    wrapped_depth0_value = JS::Value(retval->depth);

        MUST(dictionary_object0->create_data_property("depth"_utf16_fly_string, wrapped_depth0_value));

        JS::Value wrapped_desynchronized0_value;

    wrapped_desynchronized0_value = JS::Value(retval->desynchronized);

        MUST(dictionary_object0->create_data_property("desynchronized"_utf16_fly_string, wrapped_desynchronized0_value));

        JS::Value wrapped_fail_if_major_performance_caveat0_value;

    wrapped_fail_if_major_performance_caveat0_value = JS::Value(retval->fail_if_major_performance_caveat);

        MUST(dictionary_object0->create_data_property("failIfMajorPerformanceCaveat"_utf16_fly_string, wrapped_fail_if_major_performance_caveat0_value));

        JS::Value wrapped_power_preference0_value;

    wrapped_power_preference0_value = JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval->power_preference));

        MUST(dictionary_object0->create_data_property("powerPreference"_utf16_fly_string, wrapped_power_preference0_value));

        JS::Value wrapped_premultiplied_alpha0_value;

    wrapped_premultiplied_alpha0_value = JS::Value(retval->premultiplied_alpha);

        MUST(dictionary_object0->create_data_property("premultipliedAlpha"_utf16_fly_string, wrapped_premultiplied_alpha0_value));

        JS::Value wrapped_preserve_drawing_buffer0_value;

    wrapped_preserve_drawing_buffer0_value = JS::Value(retval->preserve_drawing_buffer);

        MUST(dictionary_object0->create_data_property("preserveDrawingBuffer"_utf16_fly_string, wrapped_preserve_drawing_buffer0_value));

        JS::Value wrapped_stencil0_value;

    wrapped_stencil0_value = JS::Value(retval->stencil);

        MUST(dictionary_object0->create_data_property("stencil"_utf16_fly_string, wrapped_stencil0_value));

        return dictionary_object0;
    }

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::is_context_lost)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::is_context_lost");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_context_lost(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_supported_extensions)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_supported_extensions");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_supported_extensions(); }));

    if (retval.has_value()) {

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    auto& retval_non_optional = retval.value();
    for (size_t i0 = 0; i0 < retval_non_optional.size(); ++i0) {
        auto& element0 = retval_non_optional.at(i0);
JS::Value wrapped_element0;

    wrapped_element0 = JS::PrimitiveString::create(vm, element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_extension)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_extension");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getExtension");

    auto arg0 = vm.argument(0);

    String name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_extension(name); }));

    if (retval) {

    return JS::Value(const_cast<JS::Object*>(retval));

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::active_texture)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::active_texture");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "activeTexture");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong texture;

    texture = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->active_texture(texture); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::attach_shader)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::attach_shader");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "attachShader", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<WebGLShader>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLShader");

    auto& shader = static_cast<WebGLShader&>(arg1.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->attach_shader(program, shader); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::bind_attrib_location)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::bind_attrib_location");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "bindAttribLocation", "3");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    String name;
    if (!false || !arg2.is_null()) {
        name = TRY(WebIDL::to_string(vm, arg2));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bind_attrib_location(program, index, name); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::bind_buffer)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::bind_buffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "bindBuffer", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    GC::Ptr<WebGLBuffer> buffer;

    if (!arg1.is_nullish()) {
        if (!arg1.is_object() || !is<WebGLBuffer>(arg1.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLBuffer");

        buffer = &static_cast<WebGLBuffer&>(arg1.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bind_buffer(target, buffer); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::bind_framebuffer)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::bind_framebuffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "bindFramebuffer", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    GC::Ptr<WebGLFramebuffer> framebuffer;

    if (!arg1.is_nullish()) {
        if (!arg1.is_object() || !is<WebGLFramebuffer>(arg1.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLFramebuffer");

        framebuffer = &static_cast<WebGLFramebuffer&>(arg1.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bind_framebuffer(target, framebuffer); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::bind_renderbuffer)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::bind_renderbuffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "bindRenderbuffer", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    GC::Ptr<WebGLRenderbuffer> renderbuffer;

    if (!arg1.is_nullish()) {
        if (!arg1.is_object() || !is<WebGLRenderbuffer>(arg1.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLRenderbuffer");

        renderbuffer = &static_cast<WebGLRenderbuffer&>(arg1.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bind_renderbuffer(target, renderbuffer); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::bind_texture)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::bind_texture");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "bindTexture", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    GC::Ptr<WebGLTexture> texture;

    if (!arg1.is_nullish()) {
        if (!arg1.is_object() || !is<WebGLTexture>(arg1.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLTexture");

        texture = &static_cast<WebGLTexture&>(arg1.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bind_texture(target, texture); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::blend_color)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::blend_color");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "blendColor", "4");

    auto arg0 = vm.argument(0);

    float red = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    float green = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    float blue = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    float alpha = TRY(arg3.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->blend_color(red, green, blue, alpha); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::blend_equation)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::blend_equation");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "blendEquation");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong mode;

    mode = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->blend_equation(mode); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::blend_equation_separate)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::blend_equation_separate");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "blendEquationSeparate", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong mode_rgb;

    mode_rgb = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong mode_alpha;

    mode_alpha = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->blend_equation_separate(mode_rgb, mode_alpha); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::blend_func)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::blend_func");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "blendFunc", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong sfactor;

    sfactor = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong dfactor;

    dfactor = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->blend_func(sfactor, dfactor); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::blend_func_separate)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::blend_func_separate");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "blendFuncSeparate", "4");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong src_rgb;

    src_rgb = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong dst_rgb;

    dst_rgb = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong src_alpha;

    src_alpha = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong dst_alpha;

    dst_alpha = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->blend_func_separate(src_rgb, dst_rgb, src_alpha, dst_alpha); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::check_framebuffer_status)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::check_framebuffer_status");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "checkFramebufferStatus");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->check_framebuffer_status(target); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::clear)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::clear");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "clear");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong mask;

    mask = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear(mask); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::clear_color)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::clear_color");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "clearColor", "4");

    auto arg0 = vm.argument(0);

    float red = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    float green = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    float blue = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    float alpha = TRY(arg3.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear_color(red, green, blue, alpha); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::clear_depth)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::clear_depth");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "clearDepth");

    auto arg0 = vm.argument(0);

    float depth = TRY(arg0.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear_depth(depth); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::clear_stencil)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::clear_stencil");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "clearStencil");

    auto arg0 = vm.argument(0);

    WebIDL::Long s;

    s = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear_stencil(s); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::color_mask)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::color_mask");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "colorMask", "4");

    auto arg0 = vm.argument(0);

    bool red;

    red = arg0.to_boolean();

    auto arg1 = vm.argument(1);

    bool green;

    green = arg1.to_boolean();

    auto arg2 = vm.argument(2);

    bool blue;

    blue = arg2.to_boolean();

    auto arg3 = vm.argument(3);

    bool alpha;

    alpha = arg3.to_boolean();

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->color_mask(red, green, blue, alpha); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::compile_shader)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::compile_shader");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "compileShader");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLShader>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLShader");

    auto& shader = static_cast<WebGLShader&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->compile_shader(shader); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::copy_tex_image2d)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::copy_tex_image2d");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 8)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "copyTexImage2D", "8");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long level;

    level = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong internalformat;

    internalformat = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long x;

    x = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::Long y;

    y = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg5 = vm.argument(5);

    WebIDL::Long width;

    width = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg6 = vm.argument(6);

    WebIDL::Long height;

    height = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg6, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg7 = vm.argument(7);

    WebIDL::Long border;

    border = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg7, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->copy_tex_image2d(target, level, internalformat, x, y, width, height, border); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::copy_tex_sub_image2d)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::copy_tex_sub_image2d");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 8)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "copyTexSubImage2D", "8");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long level;

    level = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long xoffset;

    xoffset = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long yoffset;

    yoffset = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::Long x;

    x = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg5 = vm.argument(5);

    WebIDL::Long y;

    y = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg6 = vm.argument(6);

    WebIDL::Long width;

    width = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg6, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg7 = vm.argument(7);

    WebIDL::Long height;

    height = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg7, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->copy_tex_sub_image2d(target, level, xoffset, yoffset, x, y, width, height); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::create_buffer)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::create_buffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_buffer(); }));

    if (retval) {

    return &const_cast<WebGLBuffer&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::create_framebuffer)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::create_framebuffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_framebuffer(); }));

    if (retval) {

    return &const_cast<WebGLFramebuffer&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::create_program)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::create_program");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_program(); }));

    if (retval) {

    return &const_cast<WebGLProgram&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::create_renderbuffer)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::create_renderbuffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_renderbuffer(); }));

    if (retval) {

    return &const_cast<WebGLRenderbuffer&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::create_shader)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::create_shader");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "createShader");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong type;

    type = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_shader(type); }));

    if (retval) {

    return &const_cast<WebGLShader&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::create_texture)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::create_texture");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_texture(); }));

    if (retval) {

    return &const_cast<WebGLTexture&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::cull_face)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::cull_face");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "cullFace");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong mode;

    mode = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cull_face(mode); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::delete_buffer)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::delete_buffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "deleteBuffer");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLBuffer> buffer;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLBuffer>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLBuffer");

        buffer = &static_cast<WebGLBuffer&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_buffer(buffer); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::delete_framebuffer)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::delete_framebuffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "deleteFramebuffer");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLFramebuffer> framebuffer;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLFramebuffer>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLFramebuffer");

        framebuffer = &static_cast<WebGLFramebuffer&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_framebuffer(framebuffer); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::delete_program)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::delete_program");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "deleteProgram");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLProgram> program;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

        program = &static_cast<WebGLProgram&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_program(program); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::delete_renderbuffer)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::delete_renderbuffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "deleteRenderbuffer");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLRenderbuffer> renderbuffer;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLRenderbuffer>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLRenderbuffer");

        renderbuffer = &static_cast<WebGLRenderbuffer&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_renderbuffer(renderbuffer); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::delete_shader)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::delete_shader");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "deleteShader");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLShader> shader;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLShader>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLShader");

        shader = &static_cast<WebGLShader&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_shader(shader); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::delete_texture)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::delete_texture");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "deleteTexture");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLTexture> texture;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLTexture>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLTexture");

        texture = &static_cast<WebGLTexture&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_texture(texture); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::depth_func)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::depth_func");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "depthFunc");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong func;

    func = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->depth_func(func); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::depth_mask)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::depth_mask");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "depthMask");

    auto arg0 = vm.argument(0);

    bool flag;

    flag = arg0.to_boolean();

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->depth_mask(flag); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::depth_range)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::depth_range");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "depthRange", "2");

    auto arg0 = vm.argument(0);

    float z_near = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    float z_far = TRY(arg1.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->depth_range(z_near, z_far); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::detach_shader)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::detach_shader");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "detachShader", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<WebGLShader>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLShader");

    auto& shader = static_cast<WebGLShader&>(arg1.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->detach_shader(program, shader); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::disable)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::disable");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "disable");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong cap;

    cap = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->disable(cap); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::disable_vertex_attrib_array)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::disable_vertex_attrib_array");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "disableVertexAttribArray");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->disable_vertex_attrib_array(index); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::draw_arrays)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::draw_arrays");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "drawArrays", "3");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong mode;

    mode = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long first;

    first = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long count;

    count = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->draw_arrays(mode, first, count); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::draw_elements)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::draw_elements");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "drawElements", "4");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong mode;

    mode = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long count;

    count = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong type;

    type = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::LongLong offset;

    offset = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->draw_elements(mode, count, type, offset); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::enable)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::enable");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "enable");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong cap;

    cap = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->enable(cap); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::enable_vertex_attrib_array)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::enable_vertex_attrib_array");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "enableVertexAttribArray");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->enable_vertex_attrib_array(index); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::finish)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::finish");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->finish(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::flush)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::flush");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->flush(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::framebuffer_renderbuffer)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::framebuffer_renderbuffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "framebufferRenderbuffer", "4");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong attachment;

    attachment = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong renderbuffertarget;

    renderbuffertarget = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    GC::Ptr<WebGLRenderbuffer> renderbuffer;

    if (!arg3.is_nullish()) {
        if (!arg3.is_object() || !is<WebGLRenderbuffer>(arg3.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLRenderbuffer");

        renderbuffer = &static_cast<WebGLRenderbuffer&>(arg3.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->framebuffer_renderbuffer(target, attachment, renderbuffertarget, renderbuffer); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::framebuffer_texture2d)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::framebuffer_texture2d");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "framebufferTexture2D", "5");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong attachment;

    attachment = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong textarget;

    textarget = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    GC::Ptr<WebGLTexture> texture;

    if (!arg3.is_nullish()) {
        if (!arg3.is_object() || !is<WebGLTexture>(arg3.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLTexture");

        texture = &static_cast<WebGLTexture&>(arg3.as_object());
    }

    auto arg4 = vm.argument(4);

    WebIDL::Long level;

    level = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->framebuffer_texture2d(target, attachment, textarget, texture, level); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::front_face)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::front_face");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "frontFace");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong mode;

    mode = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->front_face(mode); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::generate_mipmap)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::generate_mipmap");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "generateMipmap");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->generate_mipmap(target); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_active_attrib)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_active_attrib");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getActiveAttrib", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_active_attrib(program, index); }));

    if (retval) {

    return &const_cast<WebGLActiveInfo&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_active_uniform)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_active_uniform");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getActiveUniform", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_active_uniform(program, index); }));

    if (retval) {

    return &const_cast<WebGLActiveInfo&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_attached_shaders)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_attached_shaders");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getAttachedShaders");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_attached_shaders(program); }));

    if (retval.has_value()) {

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    auto& retval_non_optional = retval.value();
    for (size_t i0 = 0; i0 < retval_non_optional.size(); ++i0) {
        auto& element0 = retval_non_optional.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_attrib_location)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_attrib_location");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getAttribLocation", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    String name;
    if (!false || !arg1.is_null()) {
        name = TRY(WebIDL::to_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_attrib_location(program, name); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_buffer_parameter)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_buffer_parameter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getBufferParameter", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_buffer_parameter(target, pname); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_parameter)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_parameter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getParameter");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_parameter(pname); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_error)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_error");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_error(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_program_parameter)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_program_parameter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getProgramParameter", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_program_parameter(program, pname); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_program_info_log)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_program_info_log");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getProgramInfoLog");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_program_info_log(program); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_renderbuffer_parameter)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_renderbuffer_parameter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getRenderbufferParameter", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_renderbuffer_parameter(target, pname); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_shader_parameter)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_shader_parameter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getShaderParameter", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLShader>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLShader");

    auto& shader = static_cast<WebGLShader&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_shader_parameter(shader, pname); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_shader_precision_format)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_shader_precision_format");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getShaderPrecisionFormat", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong shadertype;

    shadertype = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong precisiontype;

    precisiontype = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_shader_precision_format(shadertype, precisiontype); }));

    if (retval) {

    return &const_cast<WebGLShaderPrecisionFormat&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_shader_info_log)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_shader_info_log");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getShaderInfoLog");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLShader>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLShader");

    auto& shader = static_cast<WebGLShader&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_shader_info_log(shader); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_shader_source)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_shader_source");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getShaderSource");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLShader>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLShader");

    auto& shader = static_cast<WebGLShader&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_shader_source(shader); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_tex_parameter)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_tex_parameter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getTexParameter", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_tex_parameter(target, pname); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_uniform)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_uniform");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getUniform", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<WebGLUniformLocation>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

    auto& location = static_cast<WebGLUniformLocation&>(arg1.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_uniform(program, location); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_uniform_location)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_uniform_location");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getUniformLocation", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    String name;
    if (!false || !arg1.is_null()) {
        name = TRY(WebIDL::to_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_uniform_location(program, name); }));

    if (retval) {

    return &const_cast<WebGLUniformLocation&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_vertex_attrib)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_vertex_attrib");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getVertexAttrib", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_vertex_attrib(index, pname); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::get_vertex_attrib_offset)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::get_vertex_attrib_offset");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getVertexAttribOffset", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_vertex_attrib_offset(index, pname); }));

    return JS::Value(static_cast<double>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::hint)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::hint");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "hint", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong mode;

    mode = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->hint(target, mode); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::is_buffer)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::is_buffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "isBuffer");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLBuffer> buffer;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLBuffer>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLBuffer");

        buffer = &static_cast<WebGLBuffer&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_buffer(buffer); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::is_enabled)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::is_enabled");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "isEnabled");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong cap;

    cap = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_enabled(cap); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::is_framebuffer)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::is_framebuffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "isFramebuffer");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLFramebuffer> framebuffer;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLFramebuffer>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLFramebuffer");

        framebuffer = &static_cast<WebGLFramebuffer&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_framebuffer(framebuffer); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::is_program)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::is_program");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "isProgram");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLProgram> program;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

        program = &static_cast<WebGLProgram&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_program(program); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::is_renderbuffer)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::is_renderbuffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "isRenderbuffer");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLRenderbuffer> renderbuffer;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLRenderbuffer>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLRenderbuffer");

        renderbuffer = &static_cast<WebGLRenderbuffer&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_renderbuffer(renderbuffer); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::is_shader)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::is_shader");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "isShader");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLShader> shader;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLShader>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLShader");

        shader = &static_cast<WebGLShader&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_shader(shader); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::is_texture)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::is_texture");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "isTexture");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLTexture> texture;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLTexture>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLTexture");

        texture = &static_cast<WebGLTexture&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_texture(texture); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::line_width)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::line_width");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "lineWidth");

    auto arg0 = vm.argument(0);

    float width = TRY(arg0.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->line_width(width); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::link_program)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::link_program");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "linkProgram");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->link_program(program); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::pixel_storei)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::pixel_storei");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "pixelStorei", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long param;

    param = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->pixel_storei(pname, param); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::polygon_offset)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::polygon_offset");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "polygonOffset", "2");

    auto arg0 = vm.argument(0);

    float factor = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    float units = TRY(arg1.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->polygon_offset(factor, units); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::renderbuffer_storage)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::renderbuffer_storage");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "renderbufferStorage", "4");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong internalformat;

    internalformat = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long width;

    width = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long height;

    height = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->renderbuffer_storage(target, internalformat, width, height); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::sample_coverage)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::sample_coverage");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "sampleCoverage", "2");

    auto arg0 = vm.argument(0);

    float value = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    bool invert;

    invert = arg1.to_boolean();

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->sample_coverage(value, invert); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::scissor)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::scissor");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "scissor", "4");

    auto arg0 = vm.argument(0);

    WebIDL::Long x;

    x = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long y;

    y = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long width;

    width = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long height;

    height = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scissor(x, y, width, height); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::shader_source)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::shader_source");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "shaderSource", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLShader>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLShader");

    auto& shader = static_cast<WebGLShader&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    String source;
    if (!false || !arg1.is_null()) {
        source = TRY(WebIDL::to_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->shader_source(shader, source); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::stencil_func)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::stencil_func");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "stencilFunc", "3");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong func;

    func = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long ref;

    ref = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong mask;

    mask = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->stencil_func(func, ref, mask); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::stencil_func_separate)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::stencil_func_separate");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "stencilFuncSeparate", "4");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong face;

    face = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong func;

    func = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long ref;

    ref = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong mask;

    mask = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->stencil_func_separate(face, func, ref, mask); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::stencil_mask)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::stencil_mask");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "stencilMask");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong mask;

    mask = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->stencil_mask(mask); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::stencil_mask_separate)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::stencil_mask_separate");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "stencilMaskSeparate", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong face;

    face = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong mask;

    mask = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->stencil_mask_separate(face, mask); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::stencil_op)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::stencil_op");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "stencilOp", "3");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong fail;

    fail = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong zfail;

    zfail = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong zpass;

    zpass = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->stencil_op(fail, zfail, zpass); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::stencil_op_separate)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::stencil_op_separate");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "stencilOpSeparate", "4");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong face;

    face = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong fail;

    fail = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong zfail;

    zfail = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong zpass;

    zpass = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->stencil_op_separate(face, fail, zfail, zpass); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::tex_parameterf)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::tex_parameterf");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "texParameterf", "3");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    float param = TRY(arg2.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tex_parameterf(target, pname, param); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::tex_parameteri)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::tex_parameteri");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "texParameteri", "3");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long param;

    param = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tex_parameteri(target, pname, param); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform1f)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform1f");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform1f", "2");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    float x = TRY(arg1.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform1f(location, x); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform2f)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform2f");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform2f", "3");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    float x = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    float y = TRY(arg2.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform2f(location, x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform3f)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform3f");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform3f", "4");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    float x = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    float y = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    float z = TRY(arg3.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform3f(location, x, y, z); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform4f)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform4f");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform4f", "5");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    float x = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    float y = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    float z = TRY(arg3.to_double(vm));

    auto arg4 = vm.argument(4);

    float w = TRY(arg4.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform4f(location, x, y, z, w); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform1i)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform1i");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform1i", "2");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    WebIDL::Long x;

    x = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform1i(location, x); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform2i)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform2i");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform2i", "3");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    WebIDL::Long x;

    x = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long y;

    y = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform2i(location, x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform3i)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform3i");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform3i", "4");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    WebIDL::Long x;

    x = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long y;

    y = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long z;

    z = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform3i(location, x, y, z); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::uniform4i)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::uniform4i");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform4i", "5");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    WebIDL::Long x;

    x = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long y;

    y = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long z;

    z = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::Long w;

    w = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform4i(location, x, y, z, w); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::use_program)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::use_program");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "useProgram");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLProgram> program;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

        program = &static_cast<WebGLProgram&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->use_program(program); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::validate_program)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::validate_program");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "validateProgram");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->validate_program(program); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::vertex_attrib1f)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::vertex_attrib1f");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "vertexAttrib1f", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    float x = TRY(arg1.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vertex_attrib1f(index, x); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::vertex_attrib2f)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::vertex_attrib2f");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "vertexAttrib2f", "3");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    float x = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    float y = TRY(arg2.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vertex_attrib2f(index, x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::vertex_attrib3f)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::vertex_attrib3f");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "vertexAttrib3f", "4");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    float x = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    float y = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    float z = TRY(arg3.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vertex_attrib3f(index, x, y, z); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::vertex_attrib4f)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::vertex_attrib4f");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "vertexAttrib4f", "5");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    float x = TRY(arg1.to_double(vm));

    auto arg2 = vm.argument(2);

    float y = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    float z = TRY(arg3.to_double(vm));

    auto arg4 = vm.argument(4);

    float w = TRY(arg4.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vertex_attrib4f(index, x, y, z, w); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::vertex_attrib1fv)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::vertex_attrib1fv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "vertexAttrib1fv", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Float32Array>, Vector<float>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (auto* typed_array = as_if<JS::Float32Array>(arg1_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<float> values;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    values.append(sequence_item1);
    }


            return values;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> values = TRY(arg1_to_variant(arg1));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vertex_attrib1fv(index, values); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::vertex_attrib2fv)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::vertex_attrib2fv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "vertexAttrib2fv", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Float32Array>, Vector<float>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (auto* typed_array = as_if<JS::Float32Array>(arg1_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<float> values;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    values.append(sequence_item1);
    }


            return values;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> values = TRY(arg1_to_variant(arg1));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vertex_attrib2fv(index, values); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::vertex_attrib3fv)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::vertex_attrib3fv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "vertexAttrib3fv", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Float32Array>, Vector<float>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (auto* typed_array = as_if<JS::Float32Array>(arg1_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<float> values;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    values.append(sequence_item1);
    }


            return values;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> values = TRY(arg1_to_variant(arg1));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vertex_attrib3fv(index, values); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::vertex_attrib4fv)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::vertex_attrib4fv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "vertexAttrib4fv", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Float32Array>, Vector<float>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (auto* typed_array = as_if<JS::Float32Array>(arg1_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<float> values;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    values.append(sequence_item1);
    }


            return values;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> values = TRY(arg1_to_variant(arg1));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vertex_attrib4fv(index, values); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::vertex_attrib_pointer)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::vertex_attrib_pointer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 6)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "vertexAttribPointer", "6");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long size;

    size = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong type;

    type = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    bool normalized;

    normalized = arg3.to_boolean();

    auto arg4 = vm.argument(4);

    WebIDL::Long stride;

    stride = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg5 = vm.argument(5);

    WebIDL::LongLong offset;

    offset = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vertex_attrib_pointer(index, size, type, normalized, stride, offset); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::viewport)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::viewport");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "viewport", "4");

    auto arg0 = vm.argument(0);

    WebIDL::Long x;

    x = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long y;

    y = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long width;

    width = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long height;

    height = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->viewport(x, y, width, height); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::buffer_data)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::buffer_data");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(3, vm.argument_count())) {

    case 3: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("long long", false), make_ref_counted<IDL::Type>("unsigned long", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required});
        overloads.empend(1, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("BufferSource", true), make_ref_counted<IDL::Type>("unsigned long", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required});

        effective_overload_set.emplace(move(overloads), 1);
        break;
    }

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

        return buffer_data0(vm);

    case 1:

        return buffer_data1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::tex_image2d)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::tex_image2d");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(9, vm.argument_count())) {

    case 6:
        chosen_overload_callable_id = 1;
        break;


    case 9:
        chosen_overload_callable_id = 0;
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

        return tex_image2d0(vm);

    case 1:

        return tex_image2d1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(WebGLRenderingContextPrototype::tex_sub_image2d)
{
    WebIDL::log_trace(vm, "WebGLRenderingContextPrototype::tex_sub_image2d");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(9, vm.argument_count())) {

    case 7:
        chosen_overload_callable_id = 1;
        break;


    case 9:
        chosen_overload_callable_id = 0;
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

        return tex_sub_image2d0(vm);

    case 1:

        return tex_sub_image2d1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

} // namespace Web::Bindings
