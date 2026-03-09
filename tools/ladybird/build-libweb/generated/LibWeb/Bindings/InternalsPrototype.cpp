
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
#include <LibWeb/Bindings/InternalsPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Internals/Internals.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Internals/InternalAnimationTimeline.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Internals/InternalGamepad.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/DOMStringMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ElementInternals.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Element.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/AnimationTimeline.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSNumericValue.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSUnitValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StyleSheetList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/AnimationEffect.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleSheet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/FontFace.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/XMLDocument.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbstractRange.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRule.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRuleList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/MediaList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StyleSheet.h>

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

GC_DEFINE_ALLOCATOR(InternalsPrototype);

InternalsPrototype::InternalsPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

InternalsPrototype::~InternalsPrototype()
{
}

void InternalsPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(realm.intrinsics().object_prototype());


    auto native_headless_getter = JS::NativeFunction::create(realm, headless_getter, 0, "headless"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_headless_setter;

    define_direct_accessor("headless"_utf16_fly_string, native_headless_getter, native_headless_setter, default_attributes);

    auto constant_MOD_NONE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0));

    define_direct_property("MOD_NONE"_utf16_fly_string, constant_MOD_NONE_value, JS::Attribute::Enumerable);

    auto constant_MOD_ALT_value = JS::Value(static_cast<WebIDL::UnsignedShort>(1));

    define_direct_property("MOD_ALT"_utf16_fly_string, constant_MOD_ALT_value, JS::Attribute::Enumerable);

    auto constant_MOD_CTRL_value = JS::Value(static_cast<WebIDL::UnsignedShort>(2));

    define_direct_property("MOD_CTRL"_utf16_fly_string, constant_MOD_CTRL_value, JS::Attribute::Enumerable);

    auto constant_MOD_SHIFT_value = JS::Value(static_cast<WebIDL::UnsignedShort>(4));

    define_direct_property("MOD_SHIFT"_utf16_fly_string, constant_MOD_SHIFT_value, JS::Attribute::Enumerable);

    auto constant_MOD_SUPER_value = JS::Value(static_cast<WebIDL::UnsignedShort>(8));

    define_direct_property("MOD_SUPER"_utf16_fly_string, constant_MOD_SUPER_value, JS::Attribute::Enumerable);

    auto constant_MOD_KEYPAD_value = JS::Value(static_cast<WebIDL::UnsignedShort>(16));

    define_direct_property("MOD_KEYPAD"_utf16_fly_string, constant_MOD_KEYPAD_value, JS::Attribute::Enumerable);

    auto constant_BUTTON_LEFT_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0));

    define_direct_property("BUTTON_LEFT"_utf16_fly_string, constant_BUTTON_LEFT_value, JS::Attribute::Enumerable);

    auto constant_BUTTON_MIDDLE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(1));

    define_direct_property("BUTTON_MIDDLE"_utf16_fly_string, constant_BUTTON_MIDDLE_value, JS::Attribute::Enumerable);

    auto constant_BUTTON_RIGHT_value = JS::Value(static_cast<WebIDL::UnsignedShort>(2));

    define_direct_property("BUTTON_RIGHT"_utf16_fly_string, constant_BUTTON_RIGHT_value, JS::Attribute::Enumerable);

    define_native_function(realm, "signalTestIsDone"_utf16_fly_string, signal_test_is_done, 1, default_attributes);

    define_native_function(realm, "setTestTimeout"_utf16_fly_string, set_test_timeout, 1, default_attributes);

    define_native_function(realm, "loadReferenceTestMetadata"_utf16_fly_string, load_reference_test_metadata, 0, default_attributes);

    define_native_function(realm, "loadTestVariants"_utf16_fly_string, load_test_variants, 0, default_attributes);

    define_native_function(realm, "setTimeZone"_utf16_fly_string, set_time_zone, 1, default_attributes);

    define_native_function(realm, "gc"_utf16_fly_string, gc, 0, default_attributes);

    define_native_function(realm, "hitTest"_utf16_fly_string, hit_test, 2, default_attributes);

    define_native_function(realm, "sendText"_utf16_fly_string, send_text, 2, default_attributes);

    define_native_function(realm, "sendKey"_utf16_fly_string, send_key, 2, default_attributes);

    define_native_function(realm, "paste"_utf16_fly_string, paste, 2, default_attributes);

    define_native_function(realm, "commitText"_utf16_fly_string, commit_text, 0, default_attributes);

    define_native_function(realm, "mouseDown"_utf16_fly_string, mouse_down, 2, default_attributes);

    define_native_function(realm, "mouseUp"_utf16_fly_string, mouse_up, 2, default_attributes);

    define_native_function(realm, "mouseMove"_utf16_fly_string, mouse_move, 2, default_attributes);

    define_native_function(realm, "click"_utf16_fly_string, click, 2, default_attributes);

    define_native_function(realm, "clickAndHold"_utf16_fly_string, click_and_hold, 2, default_attributes);

    define_native_function(realm, "wheel"_utf16_fly_string, wheel, 4, default_attributes);

    define_native_function(realm, "pinch"_utf16_fly_string, pinch, 3, default_attributes);

    define_native_function(realm, "currentCursor"_utf16_fly_string, current_cursor, 0, default_attributes);

    define_native_function(realm, "dispatchUserActivatedEvent"_utf16_fly_string, dispatch_user_activated_event, 2, default_attributes);

    define_native_function(realm, "spoofCurrentURL"_utf16_fly_string, spoof_current_url, 1, default_attributes);

    define_native_function(realm, "createInternalAnimationTimeline"_utf16_fly_string, create_internal_animation_timeline, 0, default_attributes);

    define_native_function(realm, "simulateDragStart"_utf16_fly_string, simulate_drag_start, 4, default_attributes);

    define_native_function(realm, "simulateDragMove"_utf16_fly_string, simulate_drag_move, 2, default_attributes);

    define_native_function(realm, "simulateDrop"_utf16_fly_string, simulate_drop, 2, default_attributes);

    define_native_function(realm, "expireCookiesWithTimeOffset"_utf16_fly_string, expire_cookies_with_time_offset, 1, default_attributes);

    define_native_function(realm, "setHttpMemoryCacheEnabled"_utf16_fly_string, set_http_memory_cache_enabled, 1, default_attributes);

    define_native_function(realm, "getComputedRole"_utf16_fly_string, get_computed_role, 1, default_attributes);

    define_native_function(realm, "getComputedLabel"_utf16_fly_string, get_computed_label, 1, default_attributes);

    define_native_function(realm, "getComputedAriaLevel"_utf16_fly_string, get_computed_aria_level, 1, default_attributes);

    define_native_function(realm, "getEchoServerPort"_utf16_fly_string, get_echo_server_port, 0, default_attributes);

    define_native_function(realm, "setBrowserZoom"_utf16_fly_string, set_browser_zoom, 1, default_attributes);

    define_native_function(realm, "setDevicePixelRatio"_utf16_fly_string, set_device_pixel_ratio, 1, default_attributes);

    define_native_function(realm, "dumpDisplayList"_utf16_fly_string, dump_display_list, 0, default_attributes);

    define_native_function(realm, "dumpLayoutTree"_utf16_fly_string, dump_layout_tree, 1, default_attributes);

    define_native_function(realm, "dumpPaintableTree"_utf16_fly_string, dump_paintable_tree, 1, default_attributes);

    define_native_function(realm, "dumpStackingContextTree"_utf16_fly_string, dump_stacking_context_tree, 0, default_attributes);

    define_native_function(realm, "dumpGCGraph"_utf16_fly_string, dump_gc_graph, 0, default_attributes);

    define_native_function(realm, "getShadowRoot"_utf16_fly_string, get_shadow_root, 1, default_attributes);

    define_native_function(realm, "handleSDLInputEvents"_utf16_fly_string, handle_sdl_input_events, 0, default_attributes);

    define_native_function(realm, "setEnvironmentsTopLevelURL"_utf16_fly_string, set_environments_top_level_url, 1, default_attributes);

    define_native_function(realm, "connectVirtualGamepad"_utf16_fly_string, connect_virtual_gamepad, 0, default_attributes);

    define_native_function(realm, "setHighlightedNode"_utf16_fly_string, set_highlighted_node, 1, default_attributes);

    define_native_function(realm, "clearElement"_utf16_fly_string, clear_element, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Internals"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void InternalsPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<Internals::Internals*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<Internals::Internals>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Internals");
    return static_cast<Internals::Internals*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::headless_getter)
{
    WebIDL::log_trace(vm, "InternalsPrototype::headless_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->headless(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::signal_test_is_done)
{
    WebIDL::log_trace(vm, "InternalsPrototype::signal_test_is_done");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "signalTestIsDone");

    auto arg0 = vm.argument(0);

    String text;
    if (!false || !arg0.is_null()) {
        text = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->signal_test_is_done(text); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::set_test_timeout)
{
    WebIDL::log_trace(vm, "InternalsPrototype::set_test_timeout");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setTestTimeout");

    auto arg0 = vm.argument(0);

    double milliseconds = TRY(arg0.to_double(vm));

    if (isinf(milliseconds) || isnan(milliseconds)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "milliseconds");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_test_timeout(milliseconds); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::load_reference_test_metadata)
{
    WebIDL::log_trace(vm, "InternalsPrototype::load_reference_test_metadata");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->load_reference_test_metadata(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::load_test_variants)
{
    WebIDL::log_trace(vm, "InternalsPrototype::load_test_variants");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->load_test_variants(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::set_time_zone)
{
    WebIDL::log_trace(vm, "InternalsPrototype::set_time_zone");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setTimeZone");

    auto arg0 = vm.argument(0);

    String time_zone;
    if (!false || !arg0.is_null()) {
        time_zone = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_time_zone(time_zone); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::gc)
{
    WebIDL::log_trace(vm, "InternalsPrototype::gc");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->gc(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::hit_test)
{
    WebIDL::log_trace(vm, "InternalsPrototype::hit_test");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "hitTest", "2");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    if (isinf(x) || isnan(x)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    if (isinf(y) || isnan(y)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->hit_test(x, y); }));

    return JS::Value(const_cast<JS::Object*>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::send_text)
{
    WebIDL::log_trace(vm, "InternalsPrototype::send_text");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "sendText", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<HTMLElement>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "HTMLElement");

    auto& target = static_cast<HTMLElement&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    String text;
    if (!false || !arg1.is_null()) {
        text = TRY(WebIDL::to_string(vm, arg1));
    }

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedShort modifiers;

    if (!arg2.is_undefined())

    modifiers = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        modifiers = static_cast<WebIDL::UnsignedShort>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->send_text(target, text, modifiers); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::send_key)
{
    WebIDL::log_trace(vm, "InternalsPrototype::send_key");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "sendKey", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<HTMLElement>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "HTMLElement");

    auto& target = static_cast<HTMLElement&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    String key_name;
    if (!false || !arg1.is_null()) {
        key_name = TRY(WebIDL::to_string(vm, arg1));
    }

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedShort modifiers;

    if (!arg2.is_undefined())

    modifiers = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        modifiers = static_cast<WebIDL::UnsignedShort>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->send_key(target, key_name, modifiers); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::paste)
{
    WebIDL::log_trace(vm, "InternalsPrototype::paste");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "paste", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<HTMLElement>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "HTMLElement");

    auto& target = static_cast<HTMLElement&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    Utf16String text;
    if (!false || !arg1.is_null()) {
        text = TRY(WebIDL::to_utf16_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->paste(target, text); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::commit_text)
{
    WebIDL::log_trace(vm, "InternalsPrototype::commit_text");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->commit_text(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::mouse_down)
{
    WebIDL::log_trace(vm, "InternalsPrototype::mouse_down");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "mouseDown", "2");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    if (isinf(x) || isnan(x)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    if (isinf(y) || isnan(y)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
    auto arg2 = vm.argument(2);

    WebIDL::UnsignedShort button;

    if (!arg2.is_undefined())

    button = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        button = static_cast<WebIDL::UnsignedShort>(0);

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedShort modifiers;

    if (!arg3.is_undefined())

    modifiers = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        modifiers = static_cast<WebIDL::UnsignedShort>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->mouse_down(x, y, button, modifiers); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::mouse_up)
{
    WebIDL::log_trace(vm, "InternalsPrototype::mouse_up");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "mouseUp", "2");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    if (isinf(x) || isnan(x)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    if (isinf(y) || isnan(y)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
    auto arg2 = vm.argument(2);

    WebIDL::UnsignedShort button;

    if (!arg2.is_undefined())

    button = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        button = static_cast<WebIDL::UnsignedShort>(0);

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedShort modifiers;

    if (!arg3.is_undefined())

    modifiers = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        modifiers = static_cast<WebIDL::UnsignedShort>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->mouse_up(x, y, button, modifiers); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::mouse_move)
{
    WebIDL::log_trace(vm, "InternalsPrototype::mouse_move");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "mouseMove", "2");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    if (isinf(x) || isnan(x)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    if (isinf(y) || isnan(y)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
    auto arg2 = vm.argument(2);

    WebIDL::UnsignedShort modifiers;

    if (!arg2.is_undefined())

    modifiers = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        modifiers = static_cast<WebIDL::UnsignedShort>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->mouse_move(x, y, modifiers); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::click)
{
    WebIDL::log_trace(vm, "InternalsPrototype::click");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "click", "2");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    if (isinf(x) || isnan(x)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    if (isinf(y) || isnan(y)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
    auto arg2 = vm.argument(2);

    WebIDL::UnsignedShort click_count;

    if (!arg2.is_undefined())

    click_count = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        click_count = static_cast<WebIDL::UnsignedShort>(1);

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedShort button;

    if (!arg3.is_undefined())

    button = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        button = static_cast<WebIDL::UnsignedShort>(0);

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedShort modifiers;

    if (!arg4.is_undefined())

    modifiers = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        modifiers = static_cast<WebIDL::UnsignedShort>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->click(x, y, click_count, button, modifiers); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::click_and_hold)
{
    WebIDL::log_trace(vm, "InternalsPrototype::click_and_hold");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "clickAndHold", "2");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    if (isinf(x) || isnan(x)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    if (isinf(y) || isnan(y)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
    auto arg2 = vm.argument(2);

    WebIDL::UnsignedShort click_count;

    if (!arg2.is_undefined())

    click_count = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        click_count = static_cast<WebIDL::UnsignedShort>(1);

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedShort button;

    if (!arg3.is_undefined())

    button = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        button = static_cast<WebIDL::UnsignedShort>(0);

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedShort modifiers;

    if (!arg4.is_undefined())

    modifiers = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        modifiers = static_cast<WebIDL::UnsignedShort>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->click_and_hold(x, y, click_count, button, modifiers); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::wheel)
{
    WebIDL::log_trace(vm, "InternalsPrototype::wheel");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "wheel", "4");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    if (isinf(x) || isnan(x)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    if (isinf(y) || isnan(y)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
    auto arg2 = vm.argument(2);

    double delta_x = TRY(arg2.to_double(vm));

    if (isinf(delta_x) || isnan(delta_x)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "deltaX");
    }
    
    auto arg3 = vm.argument(3);

    double delta_y = TRY(arg3.to_double(vm));

    if (isinf(delta_y) || isnan(delta_y)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "deltaY");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->wheel(x, y, delta_x, delta_y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::pinch)
{
    WebIDL::log_trace(vm, "InternalsPrototype::pinch");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "pinch", "3");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    if (isinf(x) || isnan(x)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    if (isinf(y) || isnan(y)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
    auto arg2 = vm.argument(2);

    double scale_delta = TRY(arg2.to_double(vm));

    if (isinf(scale_delta) || isnan(scale_delta)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "scaleDelta");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->pinch(x, y, scale_delta); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::current_cursor)
{
    WebIDL::log_trace(vm, "InternalsPrototype::current_cursor");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->current_cursor(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::dispatch_user_activated_event)
{
    WebIDL::log_trace(vm, "InternalsPrototype::dispatch_user_activated_event");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "dispatchUserActivatedEvent", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<EventTarget>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "EventTarget");

    auto& target = static_cast<EventTarget&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<Event>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Event");

    auto& event = static_cast<Event&>(arg1.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dispatch_user_activated_event(target, event); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::spoof_current_url)
{
    WebIDL::log_trace(vm, "InternalsPrototype::spoof_current_url");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "spoofCurrentURL");

    auto arg0 = vm.argument(0);

    String url;
    if (!false || !arg0.is_null()) {
        url = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->spoof_current_url(url); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::create_internal_animation_timeline)
{
    WebIDL::log_trace(vm, "InternalsPrototype::create_internal_animation_timeline");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_internal_animation_timeline(); }));

    return &const_cast<InternalAnimationTimeline&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::simulate_drag_start)
{
    WebIDL::log_trace(vm, "InternalsPrototype::simulate_drag_start");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "simulateDragStart", "4");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    if (isinf(x) || isnan(x)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    if (isinf(y) || isnan(y)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
    auto arg2 = vm.argument(2);

    String mime_type;
    if (!false || !arg2.is_null()) {
        mime_type = TRY(WebIDL::to_string(vm, arg2));
    }

    auto arg3 = vm.argument(3);

    String contents;
    if (!false || !arg3.is_null()) {
        contents = TRY(WebIDL::to_string(vm, arg3));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->simulate_drag_start(x, y, mime_type, contents); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::simulate_drag_move)
{
    WebIDL::log_trace(vm, "InternalsPrototype::simulate_drag_move");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "simulateDragMove", "2");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    if (isinf(x) || isnan(x)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    if (isinf(y) || isnan(y)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->simulate_drag_move(x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::simulate_drop)
{
    WebIDL::log_trace(vm, "InternalsPrototype::simulate_drop");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "simulateDrop", "2");

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    if (isinf(x) || isnan(x)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "x");
    }
    
    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    if (isinf(y) || isnan(y)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "y");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->simulate_drop(x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::expire_cookies_with_time_offset)
{
    WebIDL::log_trace(vm, "InternalsPrototype::expire_cookies_with_time_offset");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "expireCookiesWithTimeOffset");

    auto arg0 = vm.argument(0);

    WebIDL::LongLong seconds;

    seconds = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->expire_cookies_with_time_offset(seconds); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::set_http_memory_cache_enabled)
{
    WebIDL::log_trace(vm, "InternalsPrototype::set_http_memory_cache_enabled");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setHttpMemoryCacheEnabled");

    auto arg0 = vm.argument(0);

    bool enabled;

    enabled = arg0.to_boolean();

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_http_memory_cache_enabled(enabled); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::get_computed_role)
{
    WebIDL::log_trace(vm, "InternalsPrototype::get_computed_role");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getComputedRole");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Element>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Element");

    auto& element = static_cast<Element&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_computed_role(element); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::get_computed_label)
{
    WebIDL::log_trace(vm, "InternalsPrototype::get_computed_label");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getComputedLabel");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Element>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Element");

    auto& element = static_cast<Element&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_computed_label(element); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::get_computed_aria_level)
{
    WebIDL::log_trace(vm, "InternalsPrototype::get_computed_aria_level");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getComputedAriaLevel");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Element>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Element");

    auto& element = static_cast<Element&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_computed_aria_level(element); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::get_echo_server_port)
{
    WebIDL::log_trace(vm, "InternalsPrototype::get_echo_server_port");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_echo_server_port(); }));

    return JS::Value(static_cast<WebIDL::UnsignedShort>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::set_browser_zoom)
{
    WebIDL::log_trace(vm, "InternalsPrototype::set_browser_zoom");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setBrowserZoom");

    auto arg0 = vm.argument(0);

    double factor = TRY(arg0.to_double(vm));

    if (isinf(factor) || isnan(factor)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "factor");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_browser_zoom(factor); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::set_device_pixel_ratio)
{
    WebIDL::log_trace(vm, "InternalsPrototype::set_device_pixel_ratio");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setDevicePixelRatio");

    auto arg0 = vm.argument(0);

    double ratio = TRY(arg0.to_double(vm));

    if (isinf(ratio) || isnan(ratio)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "ratio");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_device_pixel_ratio(ratio); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::dump_display_list)
{
    WebIDL::log_trace(vm, "InternalsPrototype::dump_display_list");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dump_display_list(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::dump_layout_tree)
{
    WebIDL::log_trace(vm, "InternalsPrototype::dump_layout_tree");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "dumpLayoutTree");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Node>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

    auto& node = static_cast<Node&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dump_layout_tree(node); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::dump_paintable_tree)
{
    WebIDL::log_trace(vm, "InternalsPrototype::dump_paintable_tree");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "dumpPaintableTree");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Node>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

    auto& node = static_cast<Node&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dump_paintable_tree(node); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::dump_stacking_context_tree)
{
    WebIDL::log_trace(vm, "InternalsPrototype::dump_stacking_context_tree");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dump_stacking_context_tree(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::dump_gc_graph)
{
    WebIDL::log_trace(vm, "InternalsPrototype::dump_gc_graph");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->dump_gc_graph(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::get_shadow_root)
{
    WebIDL::log_trace(vm, "InternalsPrototype::get_shadow_root");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getShadowRoot");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Element>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Element");

    auto& element = static_cast<Element&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_shadow_root(element); }));

    if (retval) {

    return &const_cast<ShadowRoot&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::handle_sdl_input_events)
{
    WebIDL::log_trace(vm, "InternalsPrototype::handle_sdl_input_events");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->handle_sdl_input_events(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::set_environments_top_level_url)
{
    WebIDL::log_trace(vm, "InternalsPrototype::set_environments_top_level_url");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setEnvironmentsTopLevelURL");

    auto arg0 = vm.argument(0);

    String url;
    if (!false || !arg0.is_null()) {
        url = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_environments_top_level_url(url); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::connect_virtual_gamepad)
{
    WebIDL::log_trace(vm, "InternalsPrototype::connect_virtual_gamepad");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->connect_virtual_gamepad(); }));

    return &const_cast<InternalGamepad&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::set_highlighted_node)
{
    WebIDL::log_trace(vm, "InternalsPrototype::set_highlighted_node");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setHighlightedNode");

    auto arg0 = vm.argument(0);

    GC::Ptr<Node> node;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<Node>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

        node = &static_cast<Node&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_highlighted_node(node); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(InternalsPrototype::clear_element)
{
    WebIDL::log_trace(vm, "InternalsPrototype::clear_element");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "clearElement");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<HTMLElement>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "HTMLElement");

    auto& element = static_cast<HTMLElement&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear_element(element); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
