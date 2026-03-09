
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
#include <LibWeb/Bindings/HTMLMediaElementPrototype.h>
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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLMediaElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/AudioTrackList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/MediaError.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TextTrack.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TextTrackList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/TimeRanges.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/VideoTrackList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Scripting/Fetching.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/AudioTrack.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/DOMStringMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ElementInternals.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Element.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/VideoTrack.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSUnitValue.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGGraphicsElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStreamDefaultWriter.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLRenderingContextBase.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMMatrixReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMPoint.h>

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

GC_DEFINE_ALLOCATOR(HTMLMediaElementPrototype);

HTMLMediaElementPrototype::HTMLMediaElementPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

HTMLMediaElementPrototype::~HTMLMediaElementPrototype()
{
}

void HTMLMediaElementPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<HTMLElementPrototype>(realm, "HTMLElement"_fly_string));


    auto native_error_getter = JS::NativeFunction::create(realm, error_getter, 0, "error"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_error_setter;

    define_direct_accessor("error"_utf16_fly_string, native_error_getter, native_error_setter, default_attributes);

    auto native_src_getter = JS::NativeFunction::create(realm, src_getter, 0, "src"_utf16_fly_string, &realm, "get"sv);

    auto native_src_setter = JS::NativeFunction::create(realm, src_setter, 1, "src"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("src"_utf16_fly_string, native_src_getter, native_src_setter, default_attributes);

    define_direct_property("srcObject"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    auto native_current_src_getter = JS::NativeFunction::create(realm, current_src_getter, 0, "currentSrc"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_current_src_setter;

    define_direct_accessor("currentSrc"_utf16_fly_string, native_current_src_getter, native_current_src_setter, default_attributes);

    auto native_cross_origin_getter = JS::NativeFunction::create(realm, cross_origin_getter, 0, "crossOrigin"_utf16_fly_string, &realm, "get"sv);

    auto native_cross_origin_setter = JS::NativeFunction::create(realm, cross_origin_setter, 1, "crossOrigin"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("crossOrigin"_utf16_fly_string, native_cross_origin_getter, native_cross_origin_setter, default_attributes);

    auto native_network_state_getter = JS::NativeFunction::create(realm, network_state_getter, 0, "networkState"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_network_state_setter;

    define_direct_accessor("networkState"_utf16_fly_string, native_network_state_getter, native_network_state_setter, default_attributes);

    auto native_preload_getter = JS::NativeFunction::create(realm, preload_getter, 0, "preload"_utf16_fly_string, &realm, "get"sv);

    auto native_preload_setter = JS::NativeFunction::create(realm, preload_setter, 1, "preload"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("preload"_utf16_fly_string, native_preload_getter, native_preload_setter, default_attributes);

    auto native_buffered_getter = JS::NativeFunction::create(realm, buffered_getter, 0, "buffered"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_buffered_setter;

    define_direct_accessor("buffered"_utf16_fly_string, native_buffered_getter, native_buffered_setter, default_attributes);

    auto native_ready_state_getter = JS::NativeFunction::create(realm, ready_state_getter, 0, "readyState"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_ready_state_setter;

    define_direct_accessor("readyState"_utf16_fly_string, native_ready_state_getter, native_ready_state_setter, default_attributes);

    auto native_seeking_getter = JS::NativeFunction::create(realm, seeking_getter, 0, "seeking"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_seeking_setter;

    define_direct_accessor("seeking"_utf16_fly_string, native_seeking_getter, native_seeking_setter, default_attributes);

    auto native_current_time_getter = JS::NativeFunction::create(realm, current_time_getter, 0, "currentTime"_utf16_fly_string, &realm, "get"sv);

    auto native_current_time_setter = JS::NativeFunction::create(realm, current_time_setter, 1, "currentTime"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("currentTime"_utf16_fly_string, native_current_time_getter, native_current_time_setter, default_attributes);

    auto native_duration_getter = JS::NativeFunction::create(realm, duration_getter, 0, "duration"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_duration_setter;

    define_direct_accessor("duration"_utf16_fly_string, native_duration_getter, native_duration_setter, default_attributes);

    auto native_paused_getter = JS::NativeFunction::create(realm, paused_getter, 0, "paused"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_paused_setter;

    define_direct_accessor("paused"_utf16_fly_string, native_paused_getter, native_paused_setter, default_attributes);

    auto native_default_playback_rate_getter = JS::NativeFunction::create(realm, default_playback_rate_getter, 0, "defaultPlaybackRate"_utf16_fly_string, &realm, "get"sv);

    auto native_default_playback_rate_setter = JS::NativeFunction::create(realm, default_playback_rate_setter, 1, "defaultPlaybackRate"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("defaultPlaybackRate"_utf16_fly_string, native_default_playback_rate_getter, native_default_playback_rate_setter, default_attributes);

    auto native_playback_rate_getter = JS::NativeFunction::create(realm, playback_rate_getter, 0, "playbackRate"_utf16_fly_string, &realm, "get"sv);

    auto native_playback_rate_setter = JS::NativeFunction::create(realm, playback_rate_setter, 1, "playbackRate"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("playbackRate"_utf16_fly_string, native_playback_rate_getter, native_playback_rate_setter, default_attributes);

    define_direct_property("preservesPitch"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    auto native_played_getter = JS::NativeFunction::create(realm, played_getter, 0, "played"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_played_setter;

    define_direct_accessor("played"_utf16_fly_string, native_played_getter, native_played_setter, default_attributes);

    auto native_seekable_getter = JS::NativeFunction::create(realm, seekable_getter, 0, "seekable"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_seekable_setter;

    define_direct_accessor("seekable"_utf16_fly_string, native_seekable_getter, native_seekable_setter, default_attributes);

    auto native_ended_getter = JS::NativeFunction::create(realm, ended_getter, 0, "ended"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_ended_setter;

    define_direct_accessor("ended"_utf16_fly_string, native_ended_getter, native_ended_setter, default_attributes);

    auto native_autoplay_getter = JS::NativeFunction::create(realm, autoplay_getter, 0, "autoplay"_utf16_fly_string, &realm, "get"sv);

    auto native_autoplay_setter = JS::NativeFunction::create(realm, autoplay_setter, 1, "autoplay"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("autoplay"_utf16_fly_string, native_autoplay_getter, native_autoplay_setter, default_attributes);

    auto native_loop_getter = JS::NativeFunction::create(realm, loop_getter, 0, "loop"_utf16_fly_string, &realm, "get"sv);

    auto native_loop_setter = JS::NativeFunction::create(realm, loop_setter, 1, "loop"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("loop"_utf16_fly_string, native_loop_getter, native_loop_setter, default_attributes);

    auto native_controls_getter = JS::NativeFunction::create(realm, controls_getter, 0, "controls"_utf16_fly_string, &realm, "get"sv);

    auto native_controls_setter = JS::NativeFunction::create(realm, controls_setter, 1, "controls"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("controls"_utf16_fly_string, native_controls_getter, native_controls_setter, default_attributes);

    auto native_volume_getter = JS::NativeFunction::create(realm, volume_getter, 0, "volume"_utf16_fly_string, &realm, "get"sv);

    auto native_volume_setter = JS::NativeFunction::create(realm, volume_setter, 1, "volume"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("volume"_utf16_fly_string, native_volume_getter, native_volume_setter, default_attributes);

    auto native_muted_getter = JS::NativeFunction::create(realm, muted_getter, 0, "muted"_utf16_fly_string, &realm, "get"sv);

    auto native_muted_setter = JS::NativeFunction::create(realm, muted_setter, 1, "muted"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("muted"_utf16_fly_string, native_muted_getter, native_muted_setter, default_attributes);

    auto native_default_muted_getter = JS::NativeFunction::create(realm, default_muted_getter, 0, "defaultMuted"_utf16_fly_string, &realm, "get"sv);

    auto native_default_muted_setter = JS::NativeFunction::create(realm, default_muted_setter, 1, "defaultMuted"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("defaultMuted"_utf16_fly_string, native_default_muted_getter, native_default_muted_setter, default_attributes);

    auto native_audio_tracks_getter = JS::NativeFunction::create(realm, audio_tracks_getter, 0, "audioTracks"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_audio_tracks_setter;

    define_direct_accessor("audioTracks"_utf16_fly_string, native_audio_tracks_getter, native_audio_tracks_setter, default_attributes);

    auto native_video_tracks_getter = JS::NativeFunction::create(realm, video_tracks_getter, 0, "videoTracks"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_video_tracks_setter;

    define_direct_accessor("videoTracks"_utf16_fly_string, native_video_tracks_getter, native_video_tracks_setter, default_attributes);

    auto native_text_tracks_getter = JS::NativeFunction::create(realm, text_tracks_getter, 0, "textTracks"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_text_tracks_setter;

    define_direct_accessor("textTracks"_utf16_fly_string, native_text_tracks_getter, native_text_tracks_setter, default_attributes);

        define_direct_property("getStartDate"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    auto constant_NETWORK_EMPTY_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0));

    define_direct_property("NETWORK_EMPTY"_utf16_fly_string, constant_NETWORK_EMPTY_value, JS::Attribute::Enumerable);

    auto constant_NETWORK_IDLE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(1));

    define_direct_property("NETWORK_IDLE"_utf16_fly_string, constant_NETWORK_IDLE_value, JS::Attribute::Enumerable);

    auto constant_NETWORK_LOADING_value = JS::Value(static_cast<WebIDL::UnsignedShort>(2));

    define_direct_property("NETWORK_LOADING"_utf16_fly_string, constant_NETWORK_LOADING_value, JS::Attribute::Enumerable);

    auto constant_NETWORK_NO_SOURCE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(3));

    define_direct_property("NETWORK_NO_SOURCE"_utf16_fly_string, constant_NETWORK_NO_SOURCE_value, JS::Attribute::Enumerable);

    auto constant_HAVE_NOTHING_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0));

    define_direct_property("HAVE_NOTHING"_utf16_fly_string, constant_HAVE_NOTHING_value, JS::Attribute::Enumerable);

    auto constant_HAVE_METADATA_value = JS::Value(static_cast<WebIDL::UnsignedShort>(1));

    define_direct_property("HAVE_METADATA"_utf16_fly_string, constant_HAVE_METADATA_value, JS::Attribute::Enumerable);

    auto constant_HAVE_CURRENT_DATA_value = JS::Value(static_cast<WebIDL::UnsignedShort>(2));

    define_direct_property("HAVE_CURRENT_DATA"_utf16_fly_string, constant_HAVE_CURRENT_DATA_value, JS::Attribute::Enumerable);

    auto constant_HAVE_FUTURE_DATA_value = JS::Value(static_cast<WebIDL::UnsignedShort>(3));

    define_direct_property("HAVE_FUTURE_DATA"_utf16_fly_string, constant_HAVE_FUTURE_DATA_value, JS::Attribute::Enumerable);

    auto constant_HAVE_ENOUGH_DATA_value = JS::Value(static_cast<WebIDL::UnsignedShort>(4));

    define_direct_property("HAVE_ENOUGH_DATA"_utf16_fly_string, constant_HAVE_ENOUGH_DATA_value, JS::Attribute::Enumerable);

    define_native_function(realm, "load"_utf16_fly_string, load, 0, default_attributes);

    define_native_function(realm, "canPlayType"_utf16_fly_string, can_play_type, 1, default_attributes);

    define_native_function(realm, "fastSeek"_utf16_fly_string, fast_seek, 1, default_attributes);

    define_native_function(realm, "play"_utf16_fly_string, play, 0, default_attributes);

    define_native_function(realm, "pause"_utf16_fly_string, pause, 0, default_attributes);

    define_native_function(realm, "addTextTrack"_utf16_fly_string, add_text_track, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "HTMLMediaElement"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void HTMLMediaElementPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::HTMLMediaElement*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<HTML::HTMLMediaElement>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "HTMLMediaElement");
    return static_cast<HTML::HTMLMediaElement*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::error_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::error_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->error(); }));

    if (retval) {

    return &const_cast<MediaError&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::src_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::src_getter");
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

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::src_setter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::src_setter");
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

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::current_src_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::current_src_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->current_src(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::cross_origin_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::cross_origin_getter");
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

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::cross_origin_setter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::cross_origin_setter");
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

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::network_state_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::network_state_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->network_state(); }));

    return JS::Value(static_cast<WebIDL::UnsignedShort>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::preload_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::preload_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto contentAttributeValue = impl->attribute("preload"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    auto did_set_to_missing_value = false;
    if (!contentAttributeValue.has_value()) {
        retval = "metadata"_string;
        did_set_to_missing_value = true;
    }

    Array valid_values { "auto"_string, "none"_string, "metadata"_string };

    auto has_keyword = false;
    for (auto const& value : valid_values) {
        if (value.equals_ignoring_ascii_case(retval)) {
            has_keyword = true;
            retval = value;
            break;
        }
    }

    if (!has_keyword && !did_set_to_missing_value)
        retval = "metadata"_string;
    
    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::preload_setter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::preload_setter");
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

    impl->set_attribute_value("preload"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::buffered_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::buffered_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->buffered(); }));

    return &const_cast<TimeRanges&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::ready_state_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::ready_state_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ready_state(); }));

    return JS::Value(static_cast<WebIDL::UnsignedShort>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::seeking_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::seeking_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->seeking(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::current_time_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::current_time_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->current_time(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::current_time_setter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::current_time_setter");
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

    if (isinf(cpp_value) || isnan(cpp_value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "currentTime");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_current_time(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::duration_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::duration_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->duration(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::paused_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::paused_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->paused(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::default_playback_rate_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::default_playback_rate_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->default_playback_rate(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::default_playback_rate_setter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::default_playback_rate_setter");
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

    if (isinf(cpp_value) || isnan(cpp_value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "defaultPlaybackRate");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_default_playback_rate(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::playback_rate_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::playback_rate_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->playback_rate(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::playback_rate_setter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::playback_rate_setter");
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

    if (isinf(cpp_value) || isnan(cpp_value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "playbackRate");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_playback_rate(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::played_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::played_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->played(); }));

    return &const_cast<TimeRanges&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::seekable_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::seekable_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->seekable(); }));

    return &const_cast<TimeRanges&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::ended_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::ended_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ended(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::autoplay_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::autoplay_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval = impl->has_attribute("autoplay"_fly_string);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::autoplay_setter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::autoplay_setter");
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
        impl->remove_attribute("autoplay"_fly_string);
    else
        impl->set_attribute_value("autoplay"_fly_string, String {});

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::loop_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::loop_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval = impl->has_attribute("loop"_fly_string);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::loop_setter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::loop_setter");
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
        impl->remove_attribute("loop"_fly_string);
    else
        impl->set_attribute_value("loop"_fly_string, String {});

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::controls_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::controls_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval = impl->has_attribute("controls"_fly_string);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::controls_setter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::controls_setter");
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
        impl->remove_attribute("controls"_fly_string);
    else
        impl->set_attribute_value("controls"_fly_string, String {});

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::volume_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::volume_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->volume(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::volume_setter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::volume_setter");
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

    if (isinf(cpp_value) || isnan(cpp_value)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "volume");
    }
    
    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_volume(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::muted_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::muted_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->muted(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::muted_setter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::muted_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_muted(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::default_muted_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::default_muted_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval = impl->has_attribute("muted"_fly_string);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::default_muted_setter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::default_muted_setter");
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
        impl->remove_attribute("muted"_fly_string);
    else
        impl->set_attribute_value("muted"_fly_string, String {});

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::audio_tracks_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::audio_tracks_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->audio_tracks(); }));

    return &const_cast<AudioTrackList&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::video_tracks_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::video_tracks_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->video_tracks(); }));

    return &const_cast<VideoTrackList&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::text_tracks_getter)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::text_tracks_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->text_tracks(); }));

    return &const_cast<TextTrackList&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::load)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::load");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->load(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::can_play_type)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::can_play_type");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "canPlayType");

    auto arg0 = vm.argument(0);

    String type;
    if (!false || !arg0.is_null()) {
        type = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->can_play_type(type); }));

    return JS::PrimitiveString::create(vm, Bindings::idl_enum_to_string(retval));

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::fast_seek)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::fast_seek");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "fastSeek");

    auto arg0 = vm.argument(0);

    double time = TRY(arg0.to_double(vm));

    if (isinf(time) || isnan(time)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "time");
    }
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->fast_seek(time); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::play)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::play");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->play(); }));

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

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::pause)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::pause");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->pause(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(HTMLMediaElementPrototype::add_text_track)
{
    WebIDL::log_trace(vm, "HTMLMediaElementPrototype::add_text_track");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "addTextTrack");

    auto arg0 = vm.argument(0);

    TextTrackKind kind { TextTrackKind::Subtitles };

    auto arg0_string = TRY(arg0.to_string(vm));

    if (arg0_string == "subtitles"sv)
        kind = TextTrackKind::Subtitles;

    else if (arg0_string == "captions"sv)
        kind = TextTrackKind::Captions;

    else if (arg0_string == "descriptions"sv)
        kind = TextTrackKind::Descriptions;

    else if (arg0_string == "chapters"sv)
        kind = TextTrackKind::Chapters;

    else if (arg0_string == "metadata"sv)
        kind = TextTrackKind::Metadata;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, arg0_string, "TextTrackKind");

    auto arg1 = vm.argument(1);

    String label;

    if (!arg1.is_undefined()) {
        if (!false || !arg1.is_null())
            label = TRY(WebIDL::to_string(vm, arg1));
    } else {
        label = ""_string;
    }

    auto arg2 = vm.argument(2);

    String language;

    if (!arg2.is_undefined()) {
        if (!false || !arg2.is_null())
            language = TRY(WebIDL::to_string(vm, arg2));
    } else {
        language = ""_string;
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->add_text_track(kind, label, language); }));

    return &const_cast<TextTrack&>(*retval);

}

} // namespace Web::Bindings
