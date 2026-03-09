
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
#include <LibWeb/Bindings/CookieStorePrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CookieStore/CookieStore.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Window.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorkerGlobalScope.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

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

GC_DEFINE_ALLOCATOR(CookieStorePrototype);

CookieStorePrototype::CookieStorePrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

CookieStorePrototype::~CookieStorePrototype()
{
}

void CookieStorePrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    define_native_function(realm, "get"_utf16_fly_string, get, 0, default_attributes);

    define_native_function(realm, "getAll"_utf16_fly_string, get_all, 0, default_attributes);

    define_native_function(realm, "set"_utf16_fly_string, set, 1, default_attributes);

    define_native_function(realm, "delete"_utf16_fly_string, delete_, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "CookieStore"_string), JS::Attribute::Configurable);

    if (is<HTML::Window>(realm.global_object())) {

    auto native_onchange_getter = JS::NativeFunction::create(realm, onchange_getter, 0, "onchange"_utf16_fly_string, &realm, "get"sv);

    auto native_onchange_setter = JS::NativeFunction::create(realm, onchange_setter, 1, "onchange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onchange"_utf16_fly_string, native_onchange_getter, native_onchange_setter, default_attributes);

    }

    Base::initialize(realm);

}

void CookieStorePrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<CookieStore::CookieStore*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<CookieStore::CookieStore>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CookieStore");
    return static_cast<CookieStore::CookieStore*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(CookieStorePrototype::onchange_getter)
{
    WebIDL::log_trace(vm, "CookieStorePrototype::onchange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onchange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(CookieStorePrototype::onchange_setter)
{
    WebIDL::log_trace(vm, "CookieStorePrototype::onchange_setter");
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

    GC::Ptr<WebIDL::CallbackType> cpp_value;
    if (value.is_object())
        cpp_value = vm.heap().allocate<WebIDL::CallbackType>(value.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onchange(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(CookieStorePrototype::get0)
{
    WebIDL::log_trace(vm, "CookieStorePrototype::get0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    String name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get(name); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If op has a return type that is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    // NOTE: We know that this is a Promise return type statically by the IDL.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

JS_DEFINE_NATIVE_FUNCTION(CookieStorePrototype::get1)
{
    WebIDL::log_trace(vm, "CookieStorePrototype::get1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CookieStoreGetOptions");

    CookieStoreGetOptions options {};

    auto name_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        name_property_value_0 = TRY(arg0.as_object().get("name"_utf16_fly_string));

    if (!name_property_value_0.is_undefined()) {

    Optional<String> name_value_0;

    if (!name_property_value_0.is_undefined()) {
        if (!false || !name_property_value_0.is_null())
            name_value_0 = TRY(WebIDL::to_usv_string(vm, name_property_value_0));
    }

    options.name = name_value_0;

    }

    auto url_property_value_1 = JS::js_undefined();
    if (arg0.is_object())
        url_property_value_1 = TRY(arg0.as_object().get("url"_utf16_fly_string));

    if (!url_property_value_1.is_undefined()) {

    Optional<String> url_value_1;

    if (!url_property_value_1.is_undefined()) {
        if (!false || !url_property_value_1.is_null())
            url_value_1 = TRY(WebIDL::to_usv_string(vm, url_property_value_1));
    }

    options.url = url_value_1;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get(options); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If op has a return type that is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    // NOTE: We know that this is a Promise return type statically by the IDL.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

JS_DEFINE_NATIVE_FUNCTION(CookieStorePrototype::get_all0)
{
    WebIDL::log_trace(vm, "CookieStorePrototype::get_all0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    String name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_all(name); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If op has a return type that is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    // NOTE: We know that this is a Promise return type statically by the IDL.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

JS_DEFINE_NATIVE_FUNCTION(CookieStorePrototype::get_all1)
{
    WebIDL::log_trace(vm, "CookieStorePrototype::get_all1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CookieStoreGetOptions");

    CookieStoreGetOptions options {};

    auto name_property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        name_property_value_2 = TRY(arg0.as_object().get("name"_utf16_fly_string));

    if (!name_property_value_2.is_undefined()) {

    Optional<String> name_value_2;

    if (!name_property_value_2.is_undefined()) {
        if (!false || !name_property_value_2.is_null())
            name_value_2 = TRY(WebIDL::to_usv_string(vm, name_property_value_2));
    }

    options.name = name_value_2;

    }

    auto url_property_value_3 = JS::js_undefined();
    if (arg0.is_object())
        url_property_value_3 = TRY(arg0.as_object().get("url"_utf16_fly_string));

    if (!url_property_value_3.is_undefined()) {

    Optional<String> url_value_3;

    if (!url_property_value_3.is_undefined()) {
        if (!false || !url_property_value_3.is_null())
            url_value_3 = TRY(WebIDL::to_usv_string(vm, url_property_value_3));
    }

    options.url = url_value_3;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_all(options); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If op has a return type that is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    // NOTE: We know that this is a Promise return type statically by the IDL.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

JS_DEFINE_NATIVE_FUNCTION(CookieStorePrototype::set0)
{
    WebIDL::log_trace(vm, "CookieStorePrototype::set0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    String name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    String value;
    if (!false || !arg1.is_null()) {
        value = TRY(WebIDL::to_usv_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set(name, value); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If op has a return type that is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    // NOTE: We know that this is a Promise return type statically by the IDL.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

JS_DEFINE_NATIVE_FUNCTION(CookieStorePrototype::set1)
{
    WebIDL::log_trace(vm, "CookieStorePrototype::set1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CookieInit");

    CookieInit options {};

    auto domain_property_value_4 = JS::js_undefined();
    if (arg0.is_object())
        domain_property_value_4 = TRY(arg0.as_object().get("domain"_utf16_fly_string));

    Optional<String> domain_value_4;

    if (!domain_property_value_4.is_undefined()) {
        if (!domain_property_value_4.is_null())
            domain_value_4 = TRY(WebIDL::to_usv_string(vm, domain_property_value_4));
    }

    options.domain = domain_value_4;

    auto expires_property_value_5 = JS::js_undefined();
    if (arg0.is_object())
        expires_property_value_5 = TRY(arg0.as_object().get("expires"_utf16_fly_string));

    Optional<double> expires_value_5;

    if (!expires_property_value_5.is_undefined())
        expires_value_5 = TRY(expires_property_value_5.to_double(vm));


    if (expires_value_5.has_value() && (isinf(*expires_value_5) || isnan(*expires_value_5))) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "expires");
    }
    
    options.expires = expires_value_5;

    auto name_property_value_6 = JS::js_undefined();
    if (arg0.is_object())
        name_property_value_6 = TRY(arg0.as_object().get("name"_utf16_fly_string));

    if (name_property_value_6.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "name");

    String name_value_6;
    if (!false || !name_property_value_6.is_null()) {
        name_value_6 = TRY(WebIDL::to_usv_string(vm, name_property_value_6));
    }

    options.name = name_value_6;

    auto partitioned_property_value_7 = JS::js_undefined();
    if (arg0.is_object())
        partitioned_property_value_7 = TRY(arg0.as_object().get("partitioned"_utf16_fly_string));

    bool partitioned_value_7;

    if (!partitioned_property_value_7.is_undefined())

    partitioned_value_7 = partitioned_property_value_7.to_boolean();

    else
        partitioned_value_7 = static_cast<bool>(false);

    options.partitioned = partitioned_value_7;

    auto path_property_value_8 = JS::js_undefined();
    if (arg0.is_object())
        path_property_value_8 = TRY(arg0.as_object().get("path"_utf16_fly_string));

    String path_value_8;

    if (!path_property_value_8.is_undefined()) {
        if (!false || !path_property_value_8.is_null())
            path_value_8 = TRY(WebIDL::to_usv_string(vm, path_property_value_8));
    } else {
        path_value_8 = "/"_string;
    }

    options.path = path_value_8;

    auto same_site_property_value_9 = JS::js_undefined();
    if (arg0.is_object())
        same_site_property_value_9 = TRY(arg0.as_object().get("sameSite"_utf16_fly_string));

    CookieSameSite same_site_value_9 { CookieSameSite::Strict };

    if (!same_site_property_value_9.is_undefined()) {

    auto same_site_property_value_9_string = TRY(same_site_property_value_9.to_string(vm));

    if (same_site_property_value_9_string == "strict"sv)
        same_site_value_9 = CookieSameSite::Strict;

    else if (same_site_property_value_9_string == "lax"sv)
        same_site_value_9 = CookieSameSite::Lax;

    else if (same_site_property_value_9_string == "none"sv)
        same_site_value_9 = CookieSameSite::None;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, same_site_property_value_9_string, "CookieSameSite");

    }

    options.same_site = same_site_value_9;

    auto value_property_value_10 = JS::js_undefined();
    if (arg0.is_object())
        value_property_value_10 = TRY(arg0.as_object().get("value"_utf16_fly_string));

    if (value_property_value_10.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "value");

    String value_value_10;
    if (!false || !value_property_value_10.is_null()) {
        value_value_10 = TRY(WebIDL::to_usv_string(vm, value_property_value_10));
    }

    options.value = value_value_10;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set(options); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If op has a return type that is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    // NOTE: We know that this is a Promise return type statically by the IDL.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

JS_DEFINE_NATIVE_FUNCTION(CookieStorePrototype::delete_0)
{
    WebIDL::log_trace(vm, "CookieStorePrototype::delete_0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    String name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_(name); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If op has a return type that is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    // NOTE: We know that this is a Promise return type statically by the IDL.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

JS_DEFINE_NATIVE_FUNCTION(CookieStorePrototype::delete_1)
{
    WebIDL::log_trace(vm, "CookieStorePrototype::delete_1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CookieStoreDeleteOptions");

    CookieStoreDeleteOptions options {};

    auto domain_property_value_11 = JS::js_undefined();
    if (arg0.is_object())
        domain_property_value_11 = TRY(arg0.as_object().get("domain"_utf16_fly_string));

    Optional<String> domain_value_11;

    if (!domain_property_value_11.is_undefined()) {
        if (!domain_property_value_11.is_null())
            domain_value_11 = TRY(WebIDL::to_usv_string(vm, domain_property_value_11));
    }

    options.domain = domain_value_11;

    auto name_property_value_12 = JS::js_undefined();
    if (arg0.is_object())
        name_property_value_12 = TRY(arg0.as_object().get("name"_utf16_fly_string));

    if (name_property_value_12.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "name");

    String name_value_12;
    if (!false || !name_property_value_12.is_null()) {
        name_value_12 = TRY(WebIDL::to_usv_string(vm, name_property_value_12));
    }

    options.name = name_value_12;

    auto partitioned_property_value_13 = JS::js_undefined();
    if (arg0.is_object())
        partitioned_property_value_13 = TRY(arg0.as_object().get("partitioned"_utf16_fly_string));

    bool partitioned_value_13;

    if (!partitioned_property_value_13.is_undefined())

    partitioned_value_13 = partitioned_property_value_13.to_boolean();

    else
        partitioned_value_13 = static_cast<bool>(false);

    options.partitioned = partitioned_value_13;

    auto path_property_value_14 = JS::js_undefined();
    if (arg0.is_object())
        path_property_value_14 = TRY(arg0.as_object().get("path"_utf16_fly_string));

    String path_value_14;

    if (!path_property_value_14.is_undefined()) {
        if (!false || !path_property_value_14.is_null())
            path_value_14 = TRY(WebIDL::to_usv_string(vm, path_property_value_14));
    } else {
        path_value_14 = "/"_string;
    }

    options.path = path_value_14;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_(options); }));

        return retval;
    };

    auto maybe_retval = steps();

    // And then, if an exception E was thrown:
    // 1. If op has a return type that is a promise type, then return ! Call(%Promise.reject%, %Promise%, «E»).
    // 2. Otherwise, end these steps and allow the exception to propagate.
    // NOTE: We know that this is a Promise return type statically by the IDL.
    if (maybe_retval.is_throw_completion())
        return WebIDL::create_rejected_promise(realm, maybe_retval.error_value())->promise();

    auto retval = maybe_retval.release_value();

    return GC::Ref { as<JS::Promise>(*retval->promise()) };

}

JS_DEFINE_NATIVE_FUNCTION(CookieStorePrototype::get)
{
    WebIDL::log_trace(vm, "CookieStorePrototype::get");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(1, vm.argument_count())) {

    case 0:
        chosen_overload_callable_id = 1;
        break;


    case 1: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("USVString", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required});
        overloads.empend(1, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("CookieStoreGetOptions", false)}, Vector<IDL::Optionality> { IDL::Optionality::Optional});

        effective_overload_set.emplace(move(overloads), 0);
        break;
    }

    }

    Vector<StringView> dictionary_types {
    "CookieStoreGetOptions"sv,
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return get0(vm);

    case 1:

        return get1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(CookieStorePrototype::get_all)
{
    WebIDL::log_trace(vm, "CookieStorePrototype::get_all");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(1, vm.argument_count())) {

    case 0:
        chosen_overload_callable_id = 1;
        break;


    case 1: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("USVString", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required});
        overloads.empend(1, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("CookieStoreGetOptions", false)}, Vector<IDL::Optionality> { IDL::Optionality::Optional});

        effective_overload_set.emplace(move(overloads), 0);
        break;
    }

    }

    Vector<StringView> dictionary_types {
    "CookieStoreGetOptions"sv,
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return get_all0(vm);

    case 1:

        return get_all1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(CookieStorePrototype::set)
{
    WebIDL::log_trace(vm, "CookieStorePrototype::set");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(2, vm.argument_count())) {

    case 1:
        chosen_overload_callable_id = 1;
        break;


    case 2:
        chosen_overload_callable_id = 0;
        break;


    }

    Vector<StringView> dictionary_types {
    "CookieInit"sv,
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return set0(vm);

    case 1:

        return set1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(CookieStorePrototype::delete_)
{
    WebIDL::log_trace(vm, "CookieStorePrototype::delete_");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(1, vm.argument_count())) {

    case 1: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("USVString", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required});
        overloads.empend(1, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("CookieStoreDeleteOptions", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required});

        effective_overload_set.emplace(move(overloads), 0);
        break;
    }

    }

    Vector<StringView> dictionary_types {
    "CookieStoreDeleteOptions"sv,
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return delete_0(vm);

    case 1:

        return delete_1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS::Value cookie_list_item_to_value(JS::Realm&, CookieListItem const&);
JS::Value cookie_list_item_to_value(JS::Realm& realm, CookieListItem const& dictionary)
{
    auto& vm = realm.vm();
    CookieListItem copy = dictionary;

    {
        auto dictionary_object0 = JS::Object::create(realm, realm.intrinsics().object_prototype());

        Optional<JS::Value> wrapped_name0_value;

    if (copy.name.has_value()) {

    wrapped_name0_value = JS::PrimitiveString::create(vm, const_cast<decltype(copy.name)&>(copy.name).release_value());

    }

        if (wrapped_name0_value.has_value())
            MUST(dictionary_object0->create_data_property("name"_utf16_fly_string, wrapped_name0_value.release_value()));

        Optional<JS::Value> wrapped_value0_value;

    if (copy.value.has_value()) {

    wrapped_value0_value = JS::PrimitiveString::create(vm, const_cast<decltype(copy.value)&>(copy.value).release_value());

    }

        if (wrapped_value0_value.has_value())
            MUST(dictionary_object0->create_data_property("value"_utf16_fly_string, wrapped_value0_value.release_value()));

        return dictionary_object0;
    }

}

} // namespace Web::Bindings
