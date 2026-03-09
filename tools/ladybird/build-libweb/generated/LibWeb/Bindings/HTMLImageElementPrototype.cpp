
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
#include <LibWeb/Bindings/HTMLImageElementPrototype.h>
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

#if __has_include(<LibWeb/Bindings/HTMLElementPrototype.h>)
#    include <LibWeb/Bindings/HTMLElementPrototype.h>
#endif


#include <LibWeb/Bindings/MainThreadVM.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLImageElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Request.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Scripting/Fetching.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Headers.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/DOMStringMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ElementInternals.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Element.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XHR/FormData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOMURL/URLSearchParams.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/QueuingStrategy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamBYOBReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamDefaultReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/File.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStreamDefaultWriter.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ImageBitmap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/IndexedDB/IDBFactory.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/CacheStorage.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedTypePolicyFactory.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/RequestIdleCallback/IdleDeadline.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Speech/SpeechSynthesisUtterance.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Speech/SpeechSynthesisVoice.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ImageData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CanvasRenderingContext2D.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRule.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRuleList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/MediaList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StyleSheet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSUnitValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorker.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Crypto/CryptoKey.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLCanvasElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Canvas/CanvasDrawPath.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Canvas/CanvasTextDrawingStyles.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLVideoElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGImageElement.h>

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

GC_DEFINE_ALLOCATOR(HTMLImageElementPrototype);

HTMLImageElementPrototype::HTMLImageElementPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

HTMLImageElementPrototype::~HTMLImageElementPrototype()
{
}

void HTMLImageElementPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<HTMLElementPrototype>(realm, "HTMLElement"_fly_string));


    auto native_alt_getter = JS::NativeFunction::create(realm, alt_getter, 0, "alt"_utf16_fly_string, &realm, "get"sv);

    auto native_alt_setter = JS::NativeFunction::create(realm, alt_setter, 1, "alt"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("alt"_utf16_fly_string, native_alt_getter, native_alt_setter, default_attributes);

    auto native_src_getter = JS::NativeFunction::create(realm, src_getter, 0, "src"_utf16_fly_string, &realm, "get"sv);

    auto native_src_setter = JS::NativeFunction::create(realm, src_setter, 1, "src"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("src"_utf16_fly_string, native_src_getter, native_src_setter, default_attributes);

    auto native_srcset_getter = JS::NativeFunction::create(realm, srcset_getter, 0, "srcset"_utf16_fly_string, &realm, "get"sv);

    auto native_srcset_setter = JS::NativeFunction::create(realm, srcset_setter, 1, "srcset"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("srcset"_utf16_fly_string, native_srcset_getter, native_srcset_setter, default_attributes);

    auto native_sizes_getter = JS::NativeFunction::create(realm, sizes_getter, 0, "sizes"_utf16_fly_string, &realm, "get"sv);

    auto native_sizes_setter = JS::NativeFunction::create(realm, sizes_setter, 1, "sizes"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("sizes"_utf16_fly_string, native_sizes_getter, native_sizes_setter, default_attributes);

    auto native_cross_origin_getter = JS::NativeFunction::create(realm, cross_origin_getter, 0, "crossOrigin"_utf16_fly_string, &realm, "get"sv);

    auto native_cross_origin_setter = JS::NativeFunction::create(realm, cross_origin_setter, 1, "crossOrigin"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("crossOrigin"_utf16_fly_string, native_cross_origin_getter, native_cross_origin_setter, default_attributes);

    auto native_use_map_getter = JS::NativeFunction::create(realm, use_map_getter, 0, "useMap"_utf16_fly_string, &realm, "get"sv);

    auto native_use_map_setter = JS::NativeFunction::create(realm, use_map_setter, 1, "useMap"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("useMap"_utf16_fly_string, native_use_map_getter, native_use_map_setter, default_attributes);

    auto native_is_map_getter = JS::NativeFunction::create(realm, is_map_getter, 0, "isMap"_utf16_fly_string, &realm, "get"sv);

    auto native_is_map_setter = JS::NativeFunction::create(realm, is_map_setter, 1, "isMap"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("isMap"_utf16_fly_string, native_is_map_getter, native_is_map_setter, default_attributes);

    auto native_width_getter = JS::NativeFunction::create(realm, width_getter, 0, "width"_utf16_fly_string, &realm, "get"sv);

    auto native_width_setter = JS::NativeFunction::create(realm, width_setter, 1, "width"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("width"_utf16_fly_string, native_width_getter, native_width_setter, default_attributes);

    auto native_height_getter = JS::NativeFunction::create(realm, height_getter, 0, "height"_utf16_fly_string, &realm, "get"sv);

    auto native_height_setter = JS::NativeFunction::create(realm, height_setter, 1, "height"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("height"_utf16_fly_string, native_height_getter, native_height_setter, default_attributes);

    auto native_natural_width_getter = JS::NativeFunction::create(realm, natural_width_getter, 0, "naturalWidth"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_natural_width_setter;

    define_direct_accessor("naturalWidth"_utf16_fly_string, native_natural_width_getter, native_natural_width_setter, default_attributes);

    auto native_natural_height_getter = JS::NativeFunction::create(realm, natural_height_getter, 0, "naturalHeight"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_natural_height_setter;

    define_direct_accessor("naturalHeight"_utf16_fly_string, native_natural_height_getter, native_natural_height_setter, default_attributes);

    auto native_complete_getter = JS::NativeFunction::create(realm, complete_getter, 0, "complete"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_complete_setter;

    define_direct_accessor("complete"_utf16_fly_string, native_complete_getter, native_complete_setter, default_attributes);

    auto native_current_src_getter = JS::NativeFunction::create(realm, current_src_getter, 0, "currentSrc"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_current_src_setter;

    define_direct_accessor("currentSrc"_utf16_fly_string, native_current_src_getter, native_current_src_setter, default_attributes);

    auto native_referrer_policy_getter = JS::NativeFunction::create(realm, referrer_policy_getter, 0, "referrerPolicy"_utf16_fly_string, &realm, "get"sv);

    auto native_referrer_policy_setter = JS::NativeFunction::create(realm, referrer_policy_setter, 1, "referrerPolicy"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("referrerPolicy"_utf16_fly_string, native_referrer_policy_getter, native_referrer_policy_setter, default_attributes);

    auto native_decoding_getter = JS::NativeFunction::create(realm, decoding_getter, 0, "decoding"_utf16_fly_string, &realm, "get"sv);

    auto native_decoding_setter = JS::NativeFunction::create(realm, decoding_setter, 1, "decoding"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("decoding"_utf16_fly_string, native_decoding_getter, native_decoding_setter, default_attributes);

    auto native_loading_getter = JS::NativeFunction::create(realm, loading_getter, 0, "loading"_utf16_fly_string, &realm, "get"sv);

    auto native_loading_setter = JS::NativeFunction::create(realm, loading_setter, 1, "loading"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("loading"_utf16_fly_string, native_loading_getter, native_loading_setter, default_attributes);

    auto native_fetch_priority_getter = JS::NativeFunction::create(realm, fetch_priority_getter, 0, "fetchPriority"_utf16_fly_string, &realm, "get"sv);

    auto native_fetch_priority_setter = JS::NativeFunction::create(realm, fetch_priority_setter, 1, "fetchPriority"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("fetchPriority"_utf16_fly_string, native_fetch_priority_getter, native_fetch_priority_setter, default_attributes);

    auto native_name_getter = JS::NativeFunction::create(realm, name_getter, 0, "name"_utf16_fly_string, &realm, "get"sv);

    auto native_name_setter = JS::NativeFunction::create(realm, name_setter, 1, "name"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("name"_utf16_fly_string, native_name_getter, native_name_setter, default_attributes);

    auto native_lowsrc_getter = JS::NativeFunction::create(realm, lowsrc_getter, 0, "lowsrc"_utf16_fly_string, &realm, "get"sv);

    auto native_lowsrc_setter = JS::NativeFunction::create(realm, lowsrc_setter, 1, "lowsrc"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("lowsrc"_utf16_fly_string, native_lowsrc_getter, native_lowsrc_setter, default_attributes);

    auto native_align_getter = JS::NativeFunction::create(realm, align_getter, 0, "align"_utf16_fly_string, &realm, "get"sv);

    auto native_align_setter = JS::NativeFunction::create(realm, align_setter, 1, "align"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("align"_utf16_fly_string, native_align_getter, native_align_setter, default_attributes);

    auto native_hspace_getter = JS::NativeFunction::create(realm, hspace_getter, 0, "hspace"_utf16_fly_string, &realm, "get"sv);

    auto native_hspace_setter = JS::NativeFunction::create(realm, hspace_setter, 1, "hspace"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("hspace"_utf16_fly_string, native_hspace_getter, native_hspace_setter, default_attributes);

    auto native_vspace_getter = JS::NativeFunction::create(realm, vspace_getter, 0, "vspace"_utf16_fly_string, &realm, "get"sv);

    auto native_vspace_setter = JS::NativeFunction::create(realm, vspace_setter, 1, "vspace"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("vspace"_utf16_fly_string, native_vspace_getter, native_vspace_setter, default_attributes);

    auto native_long_desc_getter = JS::NativeFunction::create(realm, long_desc_getter, 0, "longDesc"_utf16_fly_string, &realm, "get"sv);

    auto native_long_desc_setter = JS::NativeFunction::create(realm, long_desc_setter, 1, "longDesc"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("longDesc"_utf16_fly_string, native_long_desc_getter, native_long_desc_setter, default_attributes);

    auto native_border_getter = JS::NativeFunction::create(realm, border_getter, 0, "border"_utf16_fly_string, &realm, "get"sv);

    auto native_border_setter = JS::NativeFunction::create(realm, border_setter, 1, "border"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("border"_utf16_fly_string, native_border_getter, native_border_setter, default_attributes);

    auto native_x_getter = JS::NativeFunction::create(realm, x_getter, 0, "x"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_x_setter;

    define_direct_accessor("x"_utf16_fly_string, native_x_getter, native_x_setter, default_attributes);

    auto native_y_getter = JS::NativeFunction::create(realm, y_getter, 0, "y"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_y_setter;

    define_direct_accessor("y"_utf16_fly_string, native_y_getter, native_y_setter, default_attributes);

    define_native_function(realm, "decode"_utf16_fly_string, decode, 0, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "HTMLImageElement"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void HTMLImageElementPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::HTMLImageElement*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::HTMLImageElement>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "HTMLImageElement");
    return static_cast<HTML::HTMLImageElement*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::alt_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::alt_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto contentAttributeValue = impl->attribute("alt"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::alt_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::alt_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    impl->set_attribute_value("alt"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::src_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::src_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto content_attribute_value = impl->attribute("src"_fly_string);

    if (!content_attribute_value.has_value())
        return JS::PrimitiveString::create(vm, String {});

    auto url_string = impl->document().encoding_parse_and_serialize_url(*content_attribute_value);
    if (url_string.has_value())
        return JS::PrimitiveString::create(vm, url_string.release_value());

    String retval;
    if (content_attribute_value.has_value())
        retval = MUST(Infra::convert_to_scalar_value_string(*content_attribute_value));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::src_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::src_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_usv_string(vm, value));
    }

    impl->set_attribute_value("src"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::srcset_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::srcset_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto contentAttributeValue = impl->attribute("srcset"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::srcset_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::srcset_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    impl->set_attribute_value("srcset"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::sizes_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::sizes_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto contentAttributeValue = impl->attribute("sizes"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::sizes_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::sizes_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    impl->set_attribute_value("sizes"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::cross_origin_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::cross_origin_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto content_attribute_value = impl->attribute("crossorigin"_fly_string);

    auto retval = impl->attribute("crossorigin"_fly_string);

    Array valid_values { "anonymous"_string, "use-credentials"_string };
    

    if (retval.has_value()) {
        auto found = false;
        for (auto const& value : valid_values) {
            if (value.equals_ignoring_ascii_case(retval.value())) {
                found = true;
                retval = value;
                break;
            }
        }

        if (!found)
            retval = "anonymous"_string;
    }
    
    VERIFY(!retval.has_value() || valid_values.contains_slow(retval.value()));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::cross_origin_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::cross_origin_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    Optional<String> cpp_value;
    if (!value.is_nullish())
        cpp_value = TRY(WebIDL::to_string(vm, value));

    if (!cpp_value.has_value())
        impl->remove_attribute("crossorigin"_fly_string);
    else
        impl->set_attribute_value("crossorigin"_fly_string, cpp_value.value());

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::use_map_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::use_map_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto contentAttributeValue = impl->attribute("usemap"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::use_map_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::use_map_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    impl->set_attribute_value("usemap"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::is_map_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::is_map_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval = impl->has_attribute("ismap"_fly_string);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::is_map_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::is_map_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    bool cpp_value;

    cpp_value = value.to_boolean();

    if (!cpp_value)
        impl->remove_attribute("ismap"_fly_string);
    else
        impl->set_attribute_value("ismap"_fly_string, String {});

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::width_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::width_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->width(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    auto retval = retval_or_exception.release_value();

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::width_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::width_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    WebIDL::UnsignedLong cpp_value;

    cpp_value = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, value, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_width(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::height_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::height_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->height(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    auto retval = retval_or_exception.release_value();

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::height_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::height_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    WebIDL::UnsignedLong cpp_value;

    cpp_value = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, value, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_height(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::natural_width_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::natural_width_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->natural_width(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::natural_height_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::natural_height_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->natural_height(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::complete_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::complete_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->complete(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::current_src_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::current_src_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->current_src(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::referrer_policy_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::referrer_policy_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto contentAttributeValue = impl->attribute("referrerpolicy"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    auto did_set_to_missing_value = false;
    if (!contentAttributeValue.has_value()) {
        retval = ""_string;
        did_set_to_missing_value = true;
    }

    Array valid_values { ""_string, "no-referrer"_string, "no-referrer-when-downgrade"_string, "same-origin"_string, "origin"_string, "strict-origin"_string, "origin-when-cross-origin"_string, "strict-origin-when-cross-origin"_string, "unsafe-url"_string };

    auto has_keyword = false;
    for (auto const& value : valid_values) {
        if (value.equals_ignoring_ascii_case(retval)) {
            has_keyword = true;
            retval = value;
            break;
        }
    }

    if (!has_keyword && !did_set_to_missing_value)
        retval = ""_string;
    
    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::referrer_policy_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::referrer_policy_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    impl->set_attribute_value("referrerpolicy"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::decoding_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::decoding_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto contentAttributeValue = impl->attribute("decoding"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    auto did_set_to_missing_value = false;
    if (!contentAttributeValue.has_value()) {
        retval = "auto"_string;
        did_set_to_missing_value = true;
    }

    Array valid_values { "sync"_string, "async"_string, "auto"_string };

    auto has_keyword = false;
    for (auto const& value : valid_values) {
        if (value.equals_ignoring_ascii_case(retval)) {
            has_keyword = true;
            retval = value;
            break;
        }
    }

    if (!has_keyword && !did_set_to_missing_value)
        retval = "auto"_string;
    
    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::decoding_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::decoding_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    impl->set_attribute_value("decoding"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::loading_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::loading_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto contentAttributeValue = impl->attribute("loading"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    auto did_set_to_missing_value = false;
    if (!contentAttributeValue.has_value()) {
        retval = "eager"_string;
        did_set_to_missing_value = true;
    }

    Array valid_values { "lazy"_string, "eager"_string };

    auto has_keyword = false;
    for (auto const& value : valid_values) {
        if (value.equals_ignoring_ascii_case(retval)) {
            has_keyword = true;
            retval = value;
            break;
        }
    }

    if (!has_keyword && !did_set_to_missing_value)
        retval = "eager"_string;
    
    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::loading_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::loading_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    impl->set_attribute_value("loading"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::fetch_priority_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::fetch_priority_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto contentAttributeValue = impl->attribute("fetchpriority"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    auto did_set_to_missing_value = false;
    if (!contentAttributeValue.has_value()) {
        retval = "auto"_string;
        did_set_to_missing_value = true;
    }

    Array valid_values { "high"_string, "low"_string, "auto"_string };

    auto has_keyword = false;
    for (auto const& value : valid_values) {
        if (value.equals_ignoring_ascii_case(retval)) {
            has_keyword = true;
            retval = value;
            break;
        }
    }

    if (!has_keyword && !did_set_to_missing_value)
        retval = "auto"_string;
    
    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::fetch_priority_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::fetch_priority_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    impl->set_attribute_value("fetchpriority"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::name_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::name_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto contentAttributeValue = impl->attribute("name"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::name_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::name_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    impl->set_attribute_value("name"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::lowsrc_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::lowsrc_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto content_attribute_value = impl->attribute("lowsrc"_fly_string);

    if (!content_attribute_value.has_value())
        return JS::PrimitiveString::create(vm, String {});

    auto url_string = impl->document().encoding_parse_and_serialize_url(*content_attribute_value);
    if (url_string.has_value())
        return JS::PrimitiveString::create(vm, url_string.release_value());

    String retval;
    if (content_attribute_value.has_value())
        retval = MUST(Infra::convert_to_scalar_value_string(*content_attribute_value));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::lowsrc_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::lowsrc_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_usv_string(vm, value));
    }

    impl->set_attribute_value("lowsrc"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::align_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::align_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto contentAttributeValue = impl->attribute("align"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::align_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::align_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    impl->set_attribute_value("align"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::hspace_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::hspace_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    u32 retval = 0;
    auto content_attribute_value = impl->get_attribute("hspace"_fly_string);
    u32 minimum = 0;
    u32 maximum = 2147483647;
    if (content_attribute_value.has_value()) {
        auto parsed_value = Web::HTML::parse_non_negative_integer(*content_attribute_value);
        if (parsed_value.has_value()) {
            if (*parsed_value >= minimum && *parsed_value <= maximum) {
                retval = *parsed_value;
            }
        }
    }

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::hspace_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::hspace_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    WebIDL::UnsignedLong cpp_value;

    cpp_value = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, value, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    u32 minimum = 0;
    u32 new_value = minimum;
    if (cpp_value >= minimum && cpp_value <= 2147483647)
        new_value = cpp_value;
    impl->set_attribute_value("hspace"_fly_string, String::number(new_value));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::vspace_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::vspace_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    u32 retval = 0;
    auto content_attribute_value = impl->get_attribute("vspace"_fly_string);
    u32 minimum = 0;
    u32 maximum = 2147483647;
    if (content_attribute_value.has_value()) {
        auto parsed_value = Web::HTML::parse_non_negative_integer(*content_attribute_value);
        if (parsed_value.has_value()) {
            if (*parsed_value >= minimum && *parsed_value <= maximum) {
                retval = *parsed_value;
            }
        }
    }

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::vspace_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::vspace_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    WebIDL::UnsignedLong cpp_value;

    cpp_value = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, value, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    u32 minimum = 0;
    u32 new_value = minimum;
    if (cpp_value >= minimum && cpp_value <= 2147483647)
        new_value = cpp_value;
    impl->set_attribute_value("vspace"_fly_string, String::number(new_value));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::long_desc_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::long_desc_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto content_attribute_value = impl->attribute("longdesc"_fly_string);

    if (!content_attribute_value.has_value())
        return JS::PrimitiveString::create(vm, String {});

    auto url_string = impl->document().encoding_parse_and_serialize_url(*content_attribute_value);
    if (url_string.has_value())
        return JS::PrimitiveString::create(vm, url_string.release_value());

    String retval;
    if (content_attribute_value.has_value())
        retval = MUST(Infra::convert_to_scalar_value_string(*content_attribute_value));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::long_desc_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::long_desc_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_usv_string(vm, value));
    }

    impl->set_attribute_value("longdesc"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::border_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::border_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto contentAttributeValue = impl->attribute("border"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::border_setter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::border_setter");
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

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    String cpp_value;
    if (!true || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_string(vm, value));
    }

    impl->set_attribute_value("border"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::x_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->x(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::y_getter)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->y(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(HTMLImageElementPrototype::decode)
{
    WebIDL::log_trace(vm, "HTMLImageElementPrototype::decode");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->decode(); }));

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

} // namespace Web::Bindings
