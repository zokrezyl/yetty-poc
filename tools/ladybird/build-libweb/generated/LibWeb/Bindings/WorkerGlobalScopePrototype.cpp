
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
#include <LibWeb/Bindings/WorkerGlobalScopePrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/WorkerGlobalScope.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/FontFaceSet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/MessagePort.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Window.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/WorkerLocation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/WorkerNavigator.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/FontFace.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Crypto/Crypto.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Request.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Response.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HighResolutionTime/Performance.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ImageBitmap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBFactory.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/CacheStorage.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedTypePolicyFactory.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/MediaCapabilitiesAPI/MediaCapabilities.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Serial/Serial.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorkerGlobalScope.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/ScreenOrientation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/DocumentTimeline.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorkerContainer.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebXR/XRSystem.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/External.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Storage.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/RequestIdleCallback/IdleDeadline.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Speech/SpeechSynthesisUtterance.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Speech/SpeechSynthesisVoice.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedTypePolicy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Serial/SerialPort.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/StorageAPI/StorageManager.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/AnimationTimeline.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Worker.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorkerRegistration.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedScriptURL.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Crypto/CryptoKey.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XHR/FormData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOMURL/URLSearchParams.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLCanvasElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Canvas/CanvasDrawPath.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Canvas/CanvasTextDrawingStyles.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLImageElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLVideoElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGImageElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedScript.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSNumericValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRule.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRuleList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/MediaList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StyleSheet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/Animation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/KeyframeEffect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRectReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleProperties.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StylePropertyMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CustomElements/CustomStateSet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorker.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSUnitValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/AnimationEffect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleDeclaration.h>

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

GC_DEFINE_ALLOCATOR(WorkerGlobalScopePrototype);

WorkerGlobalScopePrototype::WorkerGlobalScopePrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

WorkerGlobalScopePrototype::~WorkerGlobalScopePrototype()
{
}

void WorkerGlobalScopePrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    auto native_self_getter = JS::NativeFunction::create(realm, self_getter, 0, "self"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_self_setter;

    define_direct_accessor("self"_utf16_fly_string, native_self_getter, native_self_setter, default_attributes);

    auto native_location_getter = JS::NativeFunction::create(realm, location_getter, 0, "location"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_location_setter;

    define_direct_accessor("location"_utf16_fly_string, native_location_getter, native_location_setter, default_attributes);

    auto native_navigator_getter = JS::NativeFunction::create(realm, navigator_getter, 0, "navigator"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_navigator_setter;

    define_direct_accessor("navigator"_utf16_fly_string, native_navigator_getter, native_navigator_setter, default_attributes);

    auto native_onerror_getter = JS::NativeFunction::create(realm, onerror_getter, 0, "onerror"_utf16_fly_string, &realm, "get"sv);

    auto native_onerror_setter = JS::NativeFunction::create(realm, onerror_setter, 1, "onerror"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onerror"_utf16_fly_string, native_onerror_getter, native_onerror_setter, default_attributes);

    auto native_onlanguagechange_getter = JS::NativeFunction::create(realm, onlanguagechange_getter, 0, "onlanguagechange"_utf16_fly_string, &realm, "get"sv);

    auto native_onlanguagechange_setter = JS::NativeFunction::create(realm, onlanguagechange_setter, 1, "onlanguagechange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onlanguagechange"_utf16_fly_string, native_onlanguagechange_getter, native_onlanguagechange_setter, default_attributes);

    auto native_onoffline_getter = JS::NativeFunction::create(realm, onoffline_getter, 0, "onoffline"_utf16_fly_string, &realm, "get"sv);

    auto native_onoffline_setter = JS::NativeFunction::create(realm, onoffline_setter, 1, "onoffline"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onoffline"_utf16_fly_string, native_onoffline_getter, native_onoffline_setter, default_attributes);

    auto native_ononline_getter = JS::NativeFunction::create(realm, ononline_getter, 0, "ononline"_utf16_fly_string, &realm, "get"sv);

    auto native_ononline_setter = JS::NativeFunction::create(realm, ononline_setter, 1, "ononline"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ononline"_utf16_fly_string, native_ononline_getter, native_ononline_setter, default_attributes);

    auto native_onrejectionhandled_getter = JS::NativeFunction::create(realm, onrejectionhandled_getter, 0, "onrejectionhandled"_utf16_fly_string, &realm, "get"sv);

    auto native_onrejectionhandled_setter = JS::NativeFunction::create(realm, onrejectionhandled_setter, 1, "onrejectionhandled"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onrejectionhandled"_utf16_fly_string, native_onrejectionhandled_getter, native_onrejectionhandled_setter, default_attributes);

    auto native_onunhandledrejection_getter = JS::NativeFunction::create(realm, onunhandledrejection_getter, 0, "onunhandledrejection"_utf16_fly_string, &realm, "get"sv);

    auto native_onunhandledrejection_setter = JS::NativeFunction::create(realm, onunhandledrejection_setter, 1, "onunhandledrejection"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onunhandledrejection"_utf16_fly_string, native_onunhandledrejection_getter, native_onunhandledrejection_setter, default_attributes);

    auto native_origin_getter = JS::NativeFunction::create(realm, origin_getter, 0, "origin"_utf16_fly_string, &realm, "get"sv);

    auto native_origin_setter = JS::NativeFunction::create(realm, origin_setter, 1, "origin"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("origin"_utf16_fly_string, native_origin_getter, native_origin_setter, default_attributes);

    auto native_is_secure_context_getter = JS::NativeFunction::create(realm, is_secure_context_getter, 0, "isSecureContext"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_is_secure_context_setter;

    define_direct_accessor("isSecureContext"_utf16_fly_string, native_is_secure_context_getter, native_is_secure_context_setter, default_attributes);

    auto native_cross_origin_isolated_getter = JS::NativeFunction::create(realm, cross_origin_isolated_getter, 0, "crossOriginIsolated"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_cross_origin_isolated_setter;

    define_direct_accessor("crossOriginIsolated"_utf16_fly_string, native_cross_origin_isolated_getter, native_cross_origin_isolated_setter, default_attributes);

    auto native_performance_getter = JS::NativeFunction::create(realm, performance_getter, 0, "performance"_utf16_fly_string, &realm, "get"sv);

    auto native_performance_setter = JS::NativeFunction::create(realm, performance_setter, 1, "performance"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("performance"_utf16_fly_string, native_performance_getter, native_performance_setter, default_attributes);

    auto native_indexed_db_getter = JS::NativeFunction::create(realm, indexed_db_getter, 0, "indexedDB"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_indexed_db_setter;

    define_direct_accessor("indexedDB"_utf16_fly_string, native_indexed_db_getter, native_indexed_db_setter, default_attributes);

    auto native_crypto_getter = JS::NativeFunction::create(realm, crypto_getter, 0, "crypto"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_crypto_setter;

    define_direct_accessor("crypto"_utf16_fly_string, native_crypto_getter, native_crypto_setter, default_attributes);

    if (HTML::UniversalGlobalScopeMixin::expose_experimental_interfaces()) {
    if (HTML::is_secure_context(Bindings::principal_host_defined_environment_settings_object(realm))) {
    auto native_caches_getter = JS::NativeFunction::create(realm, caches_getter, 0, "caches"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_caches_setter;

    define_direct_accessor("caches"_utf16_fly_string, native_caches_getter, native_caches_setter, default_attributes);

    }
    }
    auto native_trusted_types_getter = JS::NativeFunction::create(realm, trusted_types_getter, 0, "trustedTypes"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_trusted_types_setter;

    define_direct_accessor("trustedTypes"_utf16_fly_string, native_trusted_types_getter, native_trusted_types_setter, default_attributes);

    auto native_fonts_getter = JS::NativeFunction::create(realm, fonts_getter, 0, "fonts"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_fonts_setter;

    define_direct_accessor("fonts"_utf16_fly_string, native_fonts_getter, native_fonts_setter, default_attributes);

    define_native_function(realm, "importScripts"_utf16_fly_string, import_scripts, 0, default_attributes);

    define_native_function(realm, "btoa"_utf16_fly_string, btoa, 1, default_attributes);

    define_native_function(realm, "atob"_utf16_fly_string, atob, 1, default_attributes);

    define_native_function(realm, "queueMicrotask"_utf16_fly_string, queue_microtask, 1, default_attributes);

    define_native_function(realm, "structuredClone"_utf16_fly_string, structured_clone, 1, default_attributes);

    define_native_function(realm, "reportError"_utf16_fly_string, report_error, 1, default_attributes);

    define_native_function(realm, "setTimeout"_utf16_fly_string, set_timeout, 1, default_attributes);

    define_native_function(realm, "clearTimeout"_utf16_fly_string, clear_timeout, 0, default_attributes);

    define_native_function(realm, "setInterval"_utf16_fly_string, set_interval, 1, default_attributes);

    define_native_function(realm, "clearInterval"_utf16_fly_string, clear_interval, 0, default_attributes);

    define_native_function(realm, "createImageBitmap"_utf16_fly_string, create_image_bitmap, 1, default_attributes);

    define_native_function(realm, "fetch"_utf16_fly_string, fetch, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "WorkerGlobalScope"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void WorkerGlobalScopePrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::WorkerGlobalScope*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::WorkerGlobalScope>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WorkerGlobalScope");
    return static_cast<HTML::WorkerGlobalScope*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::self_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::self_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->self(); }));

    return &const_cast<WorkerGlobalScope&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::location_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::location_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->location(); }));

    return &const_cast<WorkerLocation&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::navigator_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::navigator_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->navigator(); }));

    return &const_cast<WorkerNavigator&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::onerror_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::onerror_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onerror(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::onerror_setter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::onerror_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onerror(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::onlanguagechange_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::onlanguagechange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onlanguagechange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::onlanguagechange_setter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::onlanguagechange_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onlanguagechange(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::onoffline_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::onoffline_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onoffline(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::onoffline_setter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::onoffline_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onoffline(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::ononline_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::ononline_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ononline(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::ononline_setter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::ononline_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ononline(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::onrejectionhandled_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::onrejectionhandled_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onrejectionhandled(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::onrejectionhandled_setter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::onrejectionhandled_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onrejectionhandled(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::onunhandledrejection_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::onunhandledrejection_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onunhandledrejection(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::onunhandledrejection_setter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::onunhandledrejection_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onunhandledrejection(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::origin_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::origin_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->origin(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::origin_setter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::origin_setter");
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

    // 1. Perform ? CreateDataPropertyOrThrow(jsValue, id, V).
    TRY(impl->create_data_property_or_throw("origin"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::is_secure_context_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::is_secure_context_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_secure_context(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::cross_origin_isolated_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::cross_origin_isolated_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cross_origin_isolated(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::performance_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::performance_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->performance(); }));

    return &const_cast<Performance&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::performance_setter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::performance_setter");
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

    // 1. Perform ? CreateDataPropertyOrThrow(jsValue, id, V).
    TRY(impl->create_data_property_or_throw("performance"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::indexed_db_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::indexed_db_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->indexed_db(); }));

    return &const_cast<IDBFactory&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::crypto_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::crypto_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->crypto(); }));

    return &const_cast<Crypto::Crypto&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::caches_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::caches_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->caches(); }));

    return &const_cast<CacheStorage&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::trusted_types_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::trusted_types_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->trusted_types(); }));

    return &const_cast<TrustedTypePolicyFactory&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::fonts_getter)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::fonts_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->fonts(); }));

    return &const_cast<FontFaceSet&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::import_scripts)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::import_scripts");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    Vector<String> urls;

    if (vm.argument_count() > 0) {
        urls.ensure_capacity(vm.argument_count() - 0);

        for (size_t i = 0; i < vm.argument_count(); ++i) {
            auto to_string_result = TRY(WebIDL::to_usv_string(vm, vm.argument(i)));
            urls.unchecked_append(move(to_string_result));
        }
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->import_scripts(move(urls)); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::btoa)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::btoa");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "btoa");

    auto arg0 = vm.argument(0);

    String data;
    if (!false || !arg0.is_null()) {
        data = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->btoa(data); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::atob)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::atob");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "atob");

    auto arg0 = vm.argument(0);

    String data;
    if (!false || !arg0.is_null()) {
        data = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->atob(data); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::queue_microtask)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::queue_microtask");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "queueMicrotask");

    auto arg0 = vm.argument(0);

    if (!arg0.is_function()
)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, arg0);

    auto callback = vm.heap().allocate<WebIDL::CallbackType>(arg0.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->queue_microtask(callback); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::structured_clone)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::structured_clone");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "structuredClone");

    auto arg0 = vm.argument(0);

    auto value = arg0;

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "StructuredSerializeOptions");

    StructuredSerializeOptions options {};

    auto transfer_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        transfer_property_value_0 = TRY(arg1.as_object().get("transfer"_utf16_fly_string));

    Vector<GC::Root<JS::Object>> transfer_value_0;

    if (!transfer_property_value_0.is_undefined()) {

    if (!transfer_property_value_0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, transfer_property_value_0);

    auto transfer_property_value_0_iterator_method0 = TRY(transfer_property_value_0.get_method(vm, vm.well_known_symbol_iterator()));
    if (!transfer_property_value_0_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, transfer_property_value_0);

    auto transfer_property_value_0_iterator1 = TRY(JS::get_iterator_from_method(vm, transfer_property_value_0, *transfer_property_value_0_iterator_method0));

    Vector<GC::Root<JS::Object>> transfer_value_0_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, transfer_property_value_0_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, next_item1);
    auto sequence_item1 = GC::make_root(next_item1.as_object());

    transfer_value_0_non_optional.append(sequence_item1);
    }

        transfer_value_0 = move(transfer_value_0_non_optional);
    }

    options.transfer = transfer_value_0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->structured_clone(value, options); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::report_error)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::report_error");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "reportError");

    auto arg0 = vm.argument(0);

    auto e = arg0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->report_error(e); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::set_timeout)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::set_timeout");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setTimeout");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<String, GC::Ref<WebIDL::CallbackType>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (arg0_object.is_function())
                return vm.heap().allocate<WebIDL::CallbackType>(arg0.as_function(), HTML::incumbent_realm());

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<String, GC::Ref<WebIDL::CallbackType>> handler = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    WebIDL::Long timeout;

    if (!arg1.is_undefined())

    timeout = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        timeout = static_cast<WebIDL::Long>(0);

    GC::RootVector<JS::Value> arguments { vm.heap() };

    if (vm.argument_count() > 2) {
        arguments.ensure_capacity(vm.argument_count() - 2);

        for (size_t i = 2; i < vm.argument_count(); ++i)
            arguments.unchecked_append(vm.argument(i));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_timeout(handler, timeout, move(arguments)); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::clear_timeout)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::clear_timeout");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::Long id;

    if (!arg0.is_undefined())

    id = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        id = static_cast<WebIDL::Long>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear_timeout(id); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::set_interval)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::set_interval");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setInterval");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<String, GC::Ref<WebIDL::CallbackType>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (arg0_object.is_function())
                return vm.heap().allocate<WebIDL::CallbackType>(arg0.as_function(), HTML::incumbent_realm());

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<String, GC::Ref<WebIDL::CallbackType>> handler = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    WebIDL::Long timeout;

    if (!arg1.is_undefined())

    timeout = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        timeout = static_cast<WebIDL::Long>(0);

    GC::RootVector<JS::Value> arguments { vm.heap() };

    if (vm.argument_count() > 2) {
        arguments.ensure_capacity(vm.argument_count() - 2);

        for (size_t i = 2; i < vm.argument_count(); ++i)
            arguments.unchecked_append(vm.argument(i));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_interval(handler, timeout, move(arguments)); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::clear_interval)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::clear_interval");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::Long id;

    if (!arg0.is_undefined())

    id = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        id = static_cast<WebIDL::Long>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear_interval(id); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::create_image_bitmap0)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::create_image_bitmap0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<HTMLImageElement>, GC::Root<SVGImageElement>, GC::Root<HTMLVideoElement>, GC::Root<HTMLCanvasElement>, GC::Root<ImageBitmap>, GC::Root<OffscreenCanvas>, GC::Root<Blob>, GC::Root<ImageData>>> {
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

                if (auto* arg0_result = as_if<Blob>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<ImageData>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<HTMLImageElement>, GC::Root<SVGImageElement>, GC::Root<HTMLVideoElement>, GC::Root<HTMLCanvasElement>, GC::Root<ImageBitmap>, GC::Root<OffscreenCanvas>, GC::Root<Blob>, GC::Root<ImageData>> image = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ImageBitmapOptions");

    ImageBitmapOptions options {};

    auto resize_height_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        resize_height_property_value_1 = TRY(arg1.as_object().get("resizeHeight"_utf16_fly_string));

    if (!resize_height_property_value_1.is_undefined()) {

    Optional<WebIDL::UnsignedLong> resize_height_value_1;

    if (!resize_height_property_value_1.is_undefined())

    resize_height_value_1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, resize_height_property_value_1, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    options.resize_height = resize_height_value_1;

    }

    auto resize_quality_property_value_2 = JS::js_undefined();
    if (arg1.is_object())
        resize_quality_property_value_2 = TRY(arg1.as_object().get("resizeQuality"_utf16_fly_string));

    ResizeQuality resize_quality_value_2 { ResizeQuality::Low };

    if (!resize_quality_property_value_2.is_undefined()) {

    auto resize_quality_property_value_2_string = TRY(resize_quality_property_value_2.to_string(vm));

    if (resize_quality_property_value_2_string == "pixelated"sv)
        resize_quality_value_2 = ResizeQuality::Pixelated;

    else if (resize_quality_property_value_2_string == "low"sv)
        resize_quality_value_2 = ResizeQuality::Low;

    else if (resize_quality_property_value_2_string == "medium"sv)
        resize_quality_value_2 = ResizeQuality::Medium;

    else if (resize_quality_property_value_2_string == "high"sv)
        resize_quality_value_2 = ResizeQuality::High;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, resize_quality_property_value_2_string, "ResizeQuality");

    }

    options.resize_quality = resize_quality_value_2;

    auto resize_width_property_value_3 = JS::js_undefined();
    if (arg1.is_object())
        resize_width_property_value_3 = TRY(arg1.as_object().get("resizeWidth"_utf16_fly_string));

    if (!resize_width_property_value_3.is_undefined()) {

    Optional<WebIDL::UnsignedLong> resize_width_value_3;

    if (!resize_width_property_value_3.is_undefined())

    resize_width_value_3 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, resize_width_property_value_3, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    options.resize_width = resize_width_value_3;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_image_bitmap(image, options); }));

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

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::create_image_bitmap1)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::create_image_bitmap1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<HTMLImageElement>, GC::Root<SVGImageElement>, GC::Root<HTMLVideoElement>, GC::Root<HTMLCanvasElement>, GC::Root<ImageBitmap>, GC::Root<OffscreenCanvas>, GC::Root<Blob>, GC::Root<ImageData>>> {
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

                if (auto* arg0_result = as_if<Blob>(arg0_object))
                    return GC::make_root(*arg0_result);

                if (auto* arg0_result = as_if<ImageData>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<HTMLImageElement>, GC::Root<SVGImageElement>, GC::Root<HTMLVideoElement>, GC::Root<HTMLCanvasElement>, GC::Root<ImageBitmap>, GC::Root<OffscreenCanvas>, GC::Root<Blob>, GC::Root<ImageData>> image = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    WebIDL::Long sx;

    sx = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long sy;

    sy = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long sw;

    sw = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::Long sh;

    sh = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg5 = vm.argument(5);

    if (!arg5.is_nullish() && !arg5.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ImageBitmapOptions");

    ImageBitmapOptions options {};

    auto resize_height_property_value_4 = JS::js_undefined();
    if (arg5.is_object())
        resize_height_property_value_4 = TRY(arg5.as_object().get("resizeHeight"_utf16_fly_string));

    if (!resize_height_property_value_4.is_undefined()) {

    Optional<WebIDL::UnsignedLong> resize_height_value_4;

    if (!resize_height_property_value_4.is_undefined())

    resize_height_value_4 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, resize_height_property_value_4, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    options.resize_height = resize_height_value_4;

    }

    auto resize_quality_property_value_5 = JS::js_undefined();
    if (arg5.is_object())
        resize_quality_property_value_5 = TRY(arg5.as_object().get("resizeQuality"_utf16_fly_string));

    ResizeQuality resize_quality_value_5 { ResizeQuality::Low };

    if (!resize_quality_property_value_5.is_undefined()) {

    auto resize_quality_property_value_5_string = TRY(resize_quality_property_value_5.to_string(vm));

    if (resize_quality_property_value_5_string == "pixelated"sv)
        resize_quality_value_5 = ResizeQuality::Pixelated;

    else if (resize_quality_property_value_5_string == "low"sv)
        resize_quality_value_5 = ResizeQuality::Low;

    else if (resize_quality_property_value_5_string == "medium"sv)
        resize_quality_value_5 = ResizeQuality::Medium;

    else if (resize_quality_property_value_5_string == "high"sv)
        resize_quality_value_5 = ResizeQuality::High;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, resize_quality_property_value_5_string, "ResizeQuality");

    }

    options.resize_quality = resize_quality_value_5;

    auto resize_width_property_value_6 = JS::js_undefined();
    if (arg5.is_object())
        resize_width_property_value_6 = TRY(arg5.as_object().get("resizeWidth"_utf16_fly_string));

    if (!resize_width_property_value_6.is_undefined()) {

    Optional<WebIDL::UnsignedLong> resize_width_value_6;

    if (!resize_width_property_value_6.is_undefined())

    resize_width_value_6 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, resize_width_property_value_6, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    options.resize_width = resize_width_value_6;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_image_bitmap(image, sx, sy, sw, sh, options); }));

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

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::fetch)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::fetch");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "fetch");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<Request>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<Request>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

    String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_usv_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<GC::Root<Request>, String> input = TRY(arg0_to_variant(arg0));

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "RequestInit");

    RequestInit init {};

    auto body_property_value_7 = JS::js_undefined();
    if (arg1.is_object())
        body_property_value_7 = TRY(arg1.as_object().get("body"_utf16_fly_string));

    if (!body_property_value_7.is_undefined()) {

    auto body_property_value_7_to_variant = [&vm, &realm](JS::Value body_property_value_7) -> JS::ThrowCompletionOr<Variant<GC::Root<ReadableStream>, GC::Root<Blob>, GC::Root<WebIDL::BufferSource>, GC::Root<FormData>, GC::Root<URLSearchParams>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (body_property_value_7.is_object()) {
            [[maybe_unused]] auto& body_property_value_7_object = body_property_value_7.as_object();

            if (is<PlatformObject>(body_property_value_7_object)) {

                if (auto* body_property_value_7_result = as_if<ReadableStream>(body_property_value_7_object))
                    return GC::make_root(*body_property_value_7_result);

                if (auto* body_property_value_7_result = as_if<Blob>(body_property_value_7_object))
                    return GC::make_root(*body_property_value_7_result);

                if (auto* body_property_value_7_result = as_if<FormData>(body_property_value_7_object))
                    return GC::make_root(*body_property_value_7_result);

                if (auto* body_property_value_7_result = as_if<URLSearchParams>(body_property_value_7_object))
                    return GC::make_root(*body_property_value_7_result);

            }

            if (is<JS::ArrayBuffer>(body_property_value_7_object) || is<JS::DataView>(body_property_value_7_object) || is<JS::TypedArrayBase>(body_property_value_7_object)) {
                GC::Ref<WebIDL::BufferSource> source_object = realm.create<WebIDL::BufferSource>(body_property_value_7_object);
                return GC::make_root(source_object);
            }

        }

    String body_property_value_7_string;
    if (!false || !body_property_value_7.is_null()) {
        body_property_value_7_string = TRY(WebIDL::to_usv_string(vm, body_property_value_7));
    }

        return { body_property_value_7_string };

    };

    Optional<Variant<GC::Root<ReadableStream>, GC::Root<Blob>, GC::Root<WebIDL::BufferSource>, GC::Root<FormData>, GC::Root<URLSearchParams>, String>> body_value_7;
    if (!body_property_value_7.is_nullish())
        body_value_7 = TRY(body_property_value_7_to_variant(body_property_value_7));

    init.body = body_value_7;

    }

    auto cache_property_value_8 = JS::js_undefined();
    if (arg1.is_object())
        cache_property_value_8 = TRY(arg1.as_object().get("cache"_utf16_fly_string));

    if (!cache_property_value_8.is_undefined()) {

    RequestCache cache_value_8 { RequestCache::Default };

    if (!cache_property_value_8.is_undefined()) {

    auto cache_property_value_8_string = TRY(cache_property_value_8.to_string(vm));

    if (cache_property_value_8_string == "default"sv)
        cache_value_8 = RequestCache::Default;

    else if (cache_property_value_8_string == "no-store"sv)
        cache_value_8 = RequestCache::NoStore;

    else if (cache_property_value_8_string == "reload"sv)
        cache_value_8 = RequestCache::Reload;

    else if (cache_property_value_8_string == "no-cache"sv)
        cache_value_8 = RequestCache::NoCache;

    else if (cache_property_value_8_string == "force-cache"sv)
        cache_value_8 = RequestCache::ForceCache;

    else if (cache_property_value_8_string == "only-if-cached"sv)
        cache_value_8 = RequestCache::OnlyIfCached;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, cache_property_value_8_string, "RequestCache");

    }

    init.cache = cache_value_8;

    }

    auto credentials_property_value_9 = JS::js_undefined();
    if (arg1.is_object())
        credentials_property_value_9 = TRY(arg1.as_object().get("credentials"_utf16_fly_string));

    if (!credentials_property_value_9.is_undefined()) {

    RequestCredentials credentials_value_9 { RequestCredentials::Omit };

    if (!credentials_property_value_9.is_undefined()) {

    auto credentials_property_value_9_string = TRY(credentials_property_value_9.to_string(vm));

    if (credentials_property_value_9_string == "omit"sv)
        credentials_value_9 = RequestCredentials::Omit;

    else if (credentials_property_value_9_string == "same-origin"sv)
        credentials_value_9 = RequestCredentials::SameOrigin;

    else if (credentials_property_value_9_string == "include"sv)
        credentials_value_9 = RequestCredentials::Include;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, credentials_property_value_9_string, "RequestCredentials");

    }

    init.credentials = credentials_value_9;

    }

    auto duplex_property_value_10 = JS::js_undefined();
    if (arg1.is_object())
        duplex_property_value_10 = TRY(arg1.as_object().get("duplex"_utf16_fly_string));

    if (!duplex_property_value_10.is_undefined()) {

    RequestDuplex duplex_value_10 { RequestDuplex::Half };

    if (!duplex_property_value_10.is_undefined()) {

    auto duplex_property_value_10_string = TRY(duplex_property_value_10.to_string(vm));

    if (duplex_property_value_10_string == "half"sv)
        duplex_value_10 = RequestDuplex::Half;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, duplex_property_value_10_string, "RequestDuplex");

    }

    init.duplex = duplex_value_10;

    }

    auto headers_property_value_11 = JS::js_undefined();
    if (arg1.is_object())
        headers_property_value_11 = TRY(arg1.as_object().get("headers"_utf16_fly_string));

    if (!headers_property_value_11.is_undefined()) {

    auto headers_property_value_11_to_variant = [&vm, &realm](JS::Value headers_property_value_11) -> JS::ThrowCompletionOr<Variant<Vector<Vector<String>>, OrderedHashMap<String, String>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (headers_property_value_11.is_object()) {
            [[maybe_unused]] auto& headers_property_value_11_object = headers_property_value_11.as_object();

        auto method = TRY(headers_property_value_11.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto headers_property_value_11_iterator1 = TRY(JS::get_iterator_from_method(vm, headers_property_value_11, *method));

    Vector<Vector<String>> headers_value_11;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, headers_property_value_11_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, next_item1);

    auto next_item1_iterator_method1 = TRY(next_item1.get_method(vm, vm.well_known_symbol_iterator()));
    if (!next_item1_iterator_method1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, next_item1);

    auto next_item1_iterator2 = TRY(JS::get_iterator_from_method(vm, next_item1, *next_item1_iterator_method1));

    Vector<String> sequence_item1;

    for (;;) {
        auto next2 = TRY(JS::iterator_step(vm, next_item1_iterator2));
        if (!next2.has<JS::IterationResult>())
            break;

        auto next_item2 = TRY(next2.get<JS::IterationResult>().value);

    String sequence_item2;
    if (!false || !next_item2.is_null()) {
        sequence_item2 = TRY(WebIDL::to_byte_string(vm, next_item2));
    }

    sequence_item1.append(sequence_item2);
    }

    headers_value_11.append(sequence_item1);
    }


            return headers_value_11;
        }

    OrderedHashMap<String, String> record_union_type;

    auto record_keys1 = TRY(headers_property_value_11_object.internal_own_property_keys());

    for (auto& key1 : record_keys1) {
        auto property_key1 = MUST(JS::PropertyKey::from_value(vm, key1));

        auto descriptor1 = TRY(headers_property_value_11_object.internal_get_own_property(property_key1));

        if (!descriptor1.has_value() || !descriptor1->enumerable.has_value() || !descriptor1->enumerable.value())
            continue;

    String typed_key1;
    if (!false || !key1.is_null()) {
        typed_key1 = TRY(WebIDL::to_byte_string(vm, key1));
    }

        auto value1 = TRY(headers_property_value_11_object.get(property_key1));

    String typed_value1;
    if (!false || !value1.is_null()) {
        typed_value1 = TRY(WebIDL::to_byte_string(vm, value1));
    }

        record_union_type.set(typed_key1, typed_value1);
    }

        return record_union_type;

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Optional<Variant<Vector<Vector<String>>, OrderedHashMap<String, String>>> headers_value_11;
    if (!headers_property_value_11.is_undefined())
        headers_value_11 = TRY(headers_property_value_11_to_variant(headers_property_value_11));

    init.headers = headers_value_11;

    }

    auto integrity_property_value_12 = JS::js_undefined();
    if (arg1.is_object())
        integrity_property_value_12 = TRY(arg1.as_object().get("integrity"_utf16_fly_string));

    if (!integrity_property_value_12.is_undefined()) {

    Optional<String> integrity_value_12;

    if (!integrity_property_value_12.is_undefined()) {
        if (!false || !integrity_property_value_12.is_null())
            integrity_value_12 = TRY(WebIDL::to_string(vm, integrity_property_value_12));
    }

    init.integrity = integrity_value_12;

    }

    auto keepalive_property_value_13 = JS::js_undefined();
    if (arg1.is_object())
        keepalive_property_value_13 = TRY(arg1.as_object().get("keepalive"_utf16_fly_string));

    if (!keepalive_property_value_13.is_undefined()) {

    Optional<bool> keepalive_value_13;

    if (!keepalive_property_value_13.is_undefined())

    keepalive_value_13 = keepalive_property_value_13.to_boolean();

    init.keepalive = keepalive_value_13;

    }

    auto method_property_value_14 = JS::js_undefined();
    if (arg1.is_object())
        method_property_value_14 = TRY(arg1.as_object().get("method"_utf16_fly_string));

    if (!method_property_value_14.is_undefined()) {

    Optional<String> method_value_14;

    if (!method_property_value_14.is_undefined()) {
        if (!false || !method_property_value_14.is_null())
            method_value_14 = TRY(WebIDL::to_byte_string(vm, method_property_value_14));
    }

    init.method = method_value_14;

    }

    auto mode_property_value_15 = JS::js_undefined();
    if (arg1.is_object())
        mode_property_value_15 = TRY(arg1.as_object().get("mode"_utf16_fly_string));

    if (!mode_property_value_15.is_undefined()) {

    RequestMode mode_value_15 { RequestMode::Navigate };

    if (!mode_property_value_15.is_undefined()) {

    auto mode_property_value_15_string = TRY(mode_property_value_15.to_string(vm));

    if (mode_property_value_15_string == "navigate"sv)
        mode_value_15 = RequestMode::Navigate;

    else if (mode_property_value_15_string == "same-origin"sv)
        mode_value_15 = RequestMode::SameOrigin;

    else if (mode_property_value_15_string == "no-cors"sv)
        mode_value_15 = RequestMode::NoCors;

    else if (mode_property_value_15_string == "cors"sv)
        mode_value_15 = RequestMode::Cors;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, mode_property_value_15_string, "RequestMode");

    }

    init.mode = mode_value_15;

    }

    auto priority_property_value_16 = JS::js_undefined();
    if (arg1.is_object())
        priority_property_value_16 = TRY(arg1.as_object().get("priority"_utf16_fly_string));

    if (!priority_property_value_16.is_undefined()) {

    RequestPriority priority_value_16 { RequestPriority::High };

    if (!priority_property_value_16.is_undefined()) {

    auto priority_property_value_16_string = TRY(priority_property_value_16.to_string(vm));

    if (priority_property_value_16_string == "high"sv)
        priority_value_16 = RequestPriority::High;

    else if (priority_property_value_16_string == "low"sv)
        priority_value_16 = RequestPriority::Low;

    else if (priority_property_value_16_string == "auto"sv)
        priority_value_16 = RequestPriority::Auto;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, priority_property_value_16_string, "RequestPriority");

    }

    init.priority = priority_value_16;

    }

    auto redirect_property_value_17 = JS::js_undefined();
    if (arg1.is_object())
        redirect_property_value_17 = TRY(arg1.as_object().get("redirect"_utf16_fly_string));

    if (!redirect_property_value_17.is_undefined()) {

    RequestRedirect redirect_value_17 { RequestRedirect::Follow };

    if (!redirect_property_value_17.is_undefined()) {

    auto redirect_property_value_17_string = TRY(redirect_property_value_17.to_string(vm));

    if (redirect_property_value_17_string == "follow"sv)
        redirect_value_17 = RequestRedirect::Follow;

    else if (redirect_property_value_17_string == "error"sv)
        redirect_value_17 = RequestRedirect::Error;

    else if (redirect_property_value_17_string == "manual"sv)
        redirect_value_17 = RequestRedirect::Manual;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, redirect_property_value_17_string, "RequestRedirect");

    }

    init.redirect = redirect_value_17;

    }

    auto referrer_property_value_18 = JS::js_undefined();
    if (arg1.is_object())
        referrer_property_value_18 = TRY(arg1.as_object().get("referrer"_utf16_fly_string));

    if (!referrer_property_value_18.is_undefined()) {

    Optional<String> referrer_value_18;

    if (!referrer_property_value_18.is_undefined()) {
        if (!false || !referrer_property_value_18.is_null())
            referrer_value_18 = TRY(WebIDL::to_usv_string(vm, referrer_property_value_18));
    }

    init.referrer = referrer_value_18;

    }

    auto referrer_policy_property_value_19 = JS::js_undefined();
    if (arg1.is_object())
        referrer_policy_property_value_19 = TRY(arg1.as_object().get("referrerPolicy"_utf16_fly_string));

    if (!referrer_policy_property_value_19.is_undefined()) {

    ReferrerPolicy referrer_policy_value_19 { ReferrerPolicy::NoReferrer };

    if (!referrer_policy_property_value_19.is_undefined()) {

    auto referrer_policy_property_value_19_string = TRY(referrer_policy_property_value_19.to_string(vm));

    if (referrer_policy_property_value_19_string == ""sv)
        referrer_policy_value_19 = ReferrerPolicy::Empty;

    else if (referrer_policy_property_value_19_string == "no-referrer"sv)
        referrer_policy_value_19 = ReferrerPolicy::NoReferrer;

    else if (referrer_policy_property_value_19_string == "no-referrer-when-downgrade"sv)
        referrer_policy_value_19 = ReferrerPolicy::NoReferrerWhenDowngrade;

    else if (referrer_policy_property_value_19_string == "same-origin"sv)
        referrer_policy_value_19 = ReferrerPolicy::SameOrigin;

    else if (referrer_policy_property_value_19_string == "origin"sv)
        referrer_policy_value_19 = ReferrerPolicy::Origin;

    else if (referrer_policy_property_value_19_string == "strict-origin"sv)
        referrer_policy_value_19 = ReferrerPolicy::StrictOrigin;

    else if (referrer_policy_property_value_19_string == "origin-when-cross-origin"sv)
        referrer_policy_value_19 = ReferrerPolicy::OriginWhenCrossOrigin;

    else if (referrer_policy_property_value_19_string == "strict-origin-when-cross-origin"sv)
        referrer_policy_value_19 = ReferrerPolicy::StrictOriginWhenCrossOrigin;

    else if (referrer_policy_property_value_19_string == "unsafe-url"sv)
        referrer_policy_value_19 = ReferrerPolicy::UnsafeUrl;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, referrer_policy_property_value_19_string, "ReferrerPolicy");

    }

    init.referrer_policy = referrer_policy_value_19;

    }

    auto signal_property_value_20 = JS::js_undefined();
    if (arg1.is_object())
        signal_property_value_20 = TRY(arg1.as_object().get("signal"_utf16_fly_string));

    if (!signal_property_value_20.is_undefined()) {

    GC::Ptr<AbortSignal> signal_value_20;

    if (!signal_property_value_20.is_nullish()) {
        if (!signal_property_value_20.is_object() || !is<AbortSignal>(signal_property_value_20.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AbortSignal");

        signal_value_20 = &static_cast<AbortSignal&>(signal_property_value_20.as_object());
    }

    init.signal = signal_value_20;

    }

    auto window_property_value_21 = JS::js_undefined();
    if (arg1.is_object())
        window_property_value_21 = TRY(arg1.as_object().get("window"_utf16_fly_string));

    if (!window_property_value_21.is_undefined()) {

    JS::Value window_value_21 = JS::js_undefined();
    if (!window_property_value_21.is_undefined())
        window_value_21 = window_property_value_21;

    init.window = window_value_21;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->fetch(input, init); }));

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

JS_DEFINE_NATIVE_FUNCTION(WorkerGlobalScopePrototype::create_image_bitmap)
{
    WebIDL::log_trace(vm, "WorkerGlobalScopePrototype::create_image_bitmap");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(6, vm.argument_count())) {

    case 1:
        chosen_overload_callable_id = 0;
        break;


    case 2:
        chosen_overload_callable_id = 0;
        break;


    case 5:
        chosen_overload_callable_id = 1;
        break;


    case 6:
        chosen_overload_callable_id = 1;
        break;


    }

    Vector<StringView> dictionary_types {
    "ImageBitmapOptions"sv,
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return create_image_bitmap0(vm);

    case 1:

        return create_image_bitmap1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

} // namespace Web::Bindings
