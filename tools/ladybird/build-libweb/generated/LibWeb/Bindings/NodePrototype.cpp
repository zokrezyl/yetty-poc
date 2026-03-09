
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
#include <LibWeb/Bindings/NodePrototype.h>
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


#include <LibWeb/Bindings/MainThreadVM.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Node.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Document.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Element.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/DocumentTimeline.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/FontFaceSet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StyleSheetList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/CDATASection.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Comment.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DOMImplementation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DocumentFragment.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DocumentType.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/HTMLCollection.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/Animatable.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StylePropertyMapReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Attr.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DOMTokenList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/NamedNodeMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/ShadowRoot.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRectList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLSlotElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Window.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedTypePolicy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/AnimationTimeline.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/FontFace.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleSheet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/CharacterData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/XMLDocument.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbstractRange.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/DOMStringMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ElementInternals.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Request.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Scripting/Fetching.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/Animation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/KeyframeEffect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRectReadOnly.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSNumericValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRule.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRuleList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/MediaList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StyleSheet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleProperties.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StylePropertyMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CustomElements/CustomStateSet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Headers.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/AnimationEffect.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSUnitValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleDeclaration.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/XHR/FormData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOMURL/URLSearchParams.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/QueuingStrategy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamBYOBReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/ReadableStreamDefaultReader.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStream.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/File.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/ServiceWorker/ServiceWorker.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Crypto/CryptoKey.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLCanvasElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Canvas/CanvasDrawPath.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Canvas/CanvasTextDrawingStyles.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLImageElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLVideoElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/SVG/SVGImageElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Streams/WritableStreamDefaultWriter.h>

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

GC_DEFINE_ALLOCATOR(NodePrototype);

NodePrototype::NodePrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

NodePrototype::~NodePrototype()
{
}

void NodePrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<EventTargetPrototype>(realm, "EventTarget"_fly_string));


    auto native_node_type_getter = JS::NativeFunction::create(realm, node_type_getter, 0, "nodeType"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_node_type_setter;

    define_direct_accessor("nodeType"_utf16_fly_string, native_node_type_getter, native_node_type_setter, default_attributes);

    auto native_node_name_getter = JS::NativeFunction::create(realm, node_name_getter, 0, "nodeName"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_node_name_setter;

    define_direct_accessor("nodeName"_utf16_fly_string, native_node_name_getter, native_node_name_setter, default_attributes);

    auto native_base_uri_getter = JS::NativeFunction::create(realm, base_uri_getter, 0, "baseURI"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_base_uri_setter;

    define_direct_accessor("baseURI"_utf16_fly_string, native_base_uri_getter, native_base_uri_setter, default_attributes);

    auto native_is_connected_getter = JS::NativeFunction::create(realm, is_connected_getter, 0, "isConnected"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_is_connected_setter;

    define_direct_accessor("isConnected"_utf16_fly_string, native_is_connected_getter, native_is_connected_setter, default_attributes);

    auto native_owner_document_getter = JS::NativeFunction::create(realm, owner_document_getter, 0, "ownerDocument"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_owner_document_setter;

    define_direct_accessor("ownerDocument"_utf16_fly_string, native_owner_document_getter, native_owner_document_setter, default_attributes);

    auto native_parent_node_getter = JS::NativeFunction::create(realm, parent_node_getter, 0, "parentNode"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_parent_node_setter;

    define_direct_accessor("parentNode"_utf16_fly_string, native_parent_node_getter, native_parent_node_setter, default_attributes);

    auto native_parent_element_getter = JS::NativeFunction::create(realm, parent_element_getter, 0, "parentElement"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_parent_element_setter;

    define_direct_accessor("parentElement"_utf16_fly_string, native_parent_element_getter, native_parent_element_setter, default_attributes);

    auto native_child_nodes_getter = JS::NativeFunction::create(realm, child_nodes_getter, 0, "childNodes"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_child_nodes_setter;

    define_direct_accessor("childNodes"_utf16_fly_string, native_child_nodes_getter, native_child_nodes_setter, default_attributes);

    auto native_first_child_getter = JS::NativeFunction::create(realm, first_child_getter, 0, "firstChild"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_first_child_setter;

    define_direct_accessor("firstChild"_utf16_fly_string, native_first_child_getter, native_first_child_setter, default_attributes);

    auto native_last_child_getter = JS::NativeFunction::create(realm, last_child_getter, 0, "lastChild"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_last_child_setter;

    define_direct_accessor("lastChild"_utf16_fly_string, native_last_child_getter, native_last_child_setter, default_attributes);

    auto native_previous_sibling_getter = JS::NativeFunction::create(realm, previous_sibling_getter, 0, "previousSibling"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_previous_sibling_setter;

    define_direct_accessor("previousSibling"_utf16_fly_string, native_previous_sibling_getter, native_previous_sibling_setter, default_attributes);

    auto native_next_sibling_getter = JS::NativeFunction::create(realm, next_sibling_getter, 0, "nextSibling"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_next_sibling_setter;

    define_direct_accessor("nextSibling"_utf16_fly_string, native_next_sibling_getter, native_next_sibling_setter, default_attributes);

    auto native_node_value_getter = JS::NativeFunction::create(realm, node_value_getter, 0, "nodeValue"_utf16_fly_string, &realm, "get"sv);

    auto native_node_value_setter = JS::NativeFunction::create(realm, node_value_setter, 1, "nodeValue"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("nodeValue"_utf16_fly_string, native_node_value_getter, native_node_value_setter, default_attributes);

    auto native_text_content_getter = JS::NativeFunction::create(realm, text_content_getter, 0, "textContent"_utf16_fly_string, &realm, "get"sv);

    auto native_text_content_setter = JS::NativeFunction::create(realm, text_content_setter, 1, "textContent"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("textContent"_utf16_fly_string, native_text_content_getter, native_text_content_setter, default_attributes);

    auto constant_ELEMENT_NODE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(1));

    define_direct_property("ELEMENT_NODE"_utf16_fly_string, constant_ELEMENT_NODE_value, JS::Attribute::Enumerable);

    auto constant_ATTRIBUTE_NODE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(2));

    define_direct_property("ATTRIBUTE_NODE"_utf16_fly_string, constant_ATTRIBUTE_NODE_value, JS::Attribute::Enumerable);

    auto constant_TEXT_NODE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(3));

    define_direct_property("TEXT_NODE"_utf16_fly_string, constant_TEXT_NODE_value, JS::Attribute::Enumerable);

    auto constant_CDATA_SECTION_NODE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(4));

    define_direct_property("CDATA_SECTION_NODE"_utf16_fly_string, constant_CDATA_SECTION_NODE_value, JS::Attribute::Enumerable);

    auto constant_ENTITY_REFERENCE_NODE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(5));

    define_direct_property("ENTITY_REFERENCE_NODE"_utf16_fly_string, constant_ENTITY_REFERENCE_NODE_value, JS::Attribute::Enumerable);

    auto constant_ENTITY_NODE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(6));

    define_direct_property("ENTITY_NODE"_utf16_fly_string, constant_ENTITY_NODE_value, JS::Attribute::Enumerable);

    auto constant_PROCESSING_INSTRUCTION_NODE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(7));

    define_direct_property("PROCESSING_INSTRUCTION_NODE"_utf16_fly_string, constant_PROCESSING_INSTRUCTION_NODE_value, JS::Attribute::Enumerable);

    auto constant_COMMENT_NODE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(8));

    define_direct_property("COMMENT_NODE"_utf16_fly_string, constant_COMMENT_NODE_value, JS::Attribute::Enumerable);

    auto constant_DOCUMENT_NODE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(9));

    define_direct_property("DOCUMENT_NODE"_utf16_fly_string, constant_DOCUMENT_NODE_value, JS::Attribute::Enumerable);

    auto constant_DOCUMENT_TYPE_NODE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(10));

    define_direct_property("DOCUMENT_TYPE_NODE"_utf16_fly_string, constant_DOCUMENT_TYPE_NODE_value, JS::Attribute::Enumerable);

    auto constant_DOCUMENT_FRAGMENT_NODE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(11));

    define_direct_property("DOCUMENT_FRAGMENT_NODE"_utf16_fly_string, constant_DOCUMENT_FRAGMENT_NODE_value, JS::Attribute::Enumerable);

    auto constant_NOTATION_NODE_value = JS::Value(static_cast<WebIDL::UnsignedShort>(12));

    define_direct_property("NOTATION_NODE"_utf16_fly_string, constant_NOTATION_NODE_value, JS::Attribute::Enumerable);

    auto constant_DOCUMENT_POSITION_DISCONNECTED_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0x01));

    define_direct_property("DOCUMENT_POSITION_DISCONNECTED"_utf16_fly_string, constant_DOCUMENT_POSITION_DISCONNECTED_value, JS::Attribute::Enumerable);

    auto constant_DOCUMENT_POSITION_PRECEDING_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0x02));

    define_direct_property("DOCUMENT_POSITION_PRECEDING"_utf16_fly_string, constant_DOCUMENT_POSITION_PRECEDING_value, JS::Attribute::Enumerable);

    auto constant_DOCUMENT_POSITION_FOLLOWING_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0x04));

    define_direct_property("DOCUMENT_POSITION_FOLLOWING"_utf16_fly_string, constant_DOCUMENT_POSITION_FOLLOWING_value, JS::Attribute::Enumerable);

    auto constant_DOCUMENT_POSITION_CONTAINS_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0x08));

    define_direct_property("DOCUMENT_POSITION_CONTAINS"_utf16_fly_string, constant_DOCUMENT_POSITION_CONTAINS_value, JS::Attribute::Enumerable);

    auto constant_DOCUMENT_POSITION_CONTAINED_BY_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0x10));

    define_direct_property("DOCUMENT_POSITION_CONTAINED_BY"_utf16_fly_string, constant_DOCUMENT_POSITION_CONTAINED_BY_value, JS::Attribute::Enumerable);

    auto constant_DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC_value = JS::Value(static_cast<WebIDL::UnsignedShort>(0x20));

    define_direct_property("DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC"_utf16_fly_string, constant_DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC_value, JS::Attribute::Enumerable);

    define_native_function(realm, "getRootNode"_utf16_fly_string, get_root_node, 0, default_attributes);

    define_native_function(realm, "hasChildNodes"_utf16_fly_string, has_child_nodes, 0, default_attributes);

    define_native_function(realm, "normalize"_utf16_fly_string, normalize, 0, default_attributes);

    define_native_function(realm, "cloneNode"_utf16_fly_string, clone_node, 0, default_attributes);

    define_native_function(realm, "isEqualNode"_utf16_fly_string, is_equal_node, 1, default_attributes);

    define_native_function(realm, "isSameNode"_utf16_fly_string, is_same_node, 1, default_attributes);

    define_native_function(realm, "compareDocumentPosition"_utf16_fly_string, compare_document_position, 1, default_attributes);

    define_native_function(realm, "contains"_utf16_fly_string, contains, 1, default_attributes);

    define_native_function(realm, "lookupPrefix"_utf16_fly_string, lookup_prefix, 1, default_attributes);

    define_native_function(realm, "lookupNamespaceURI"_utf16_fly_string, lookup_namespace_uri, 1, default_attributes);

    define_native_function(realm, "isDefaultNamespace"_utf16_fly_string, is_default_namespace, 1, default_attributes);

    define_native_function(realm, "insertBefore"_utf16_fly_string, insert_before, 2, default_attributes);

    define_native_function(realm, "appendChild"_utf16_fly_string, append_child, 1, default_attributes);

    define_native_function(realm, "replaceChild"_utf16_fly_string, replace_child, 2, default_attributes);

    define_native_function(realm, "removeChild"_utf16_fly_string, remove_child, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Node"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void NodePrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<DOM::Node*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<DOM::Node>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");
    return static_cast<DOM::Node*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::node_type_getter)
{
    WebIDL::log_trace(vm, "NodePrototype::node_type_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->node_type(); }));

    return JS::Value(static_cast<WebIDL::UnsignedShort>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::node_name_getter)
{
    WebIDL::log_trace(vm, "NodePrototype::node_name_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->node_name(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::base_uri_getter)
{
    WebIDL::log_trace(vm, "NodePrototype::base_uri_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->base_uri(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::is_connected_getter)
{
    WebIDL::log_trace(vm, "NodePrototype::is_connected_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_connected(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::owner_document_getter)
{
    WebIDL::log_trace(vm, "NodePrototype::owner_document_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->owner_document(); }));

    if (retval) {

    return &const_cast<Document&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::parent_node_getter)
{
    WebIDL::log_trace(vm, "NodePrototype::parent_node_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->parent_node(); }));

    if (retval) {

    return &const_cast<Node&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::parent_element_getter)
{
    WebIDL::log_trace(vm, "NodePrototype::parent_element_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->parent_element(); }));

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::child_nodes_getter)
{
    WebIDL::log_trace(vm, "NodePrototype::child_nodes_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->child_nodes(); }));

    return &const_cast<NodeList&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::first_child_getter)
{
    WebIDL::log_trace(vm, "NodePrototype::first_child_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->first_child(); }));

    if (retval) {

    return &const_cast<Node&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::last_child_getter)
{
    WebIDL::log_trace(vm, "NodePrototype::last_child_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->last_child(); }));

    if (retval) {

    return &const_cast<Node&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::previous_sibling_getter)
{
    WebIDL::log_trace(vm, "NodePrototype::previous_sibling_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->previous_sibling(); }));

    if (retval) {

    return &const_cast<Node&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::next_sibling_getter)
{
    WebIDL::log_trace(vm, "NodePrototype::next_sibling_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->next_sibling(); }));

    if (retval) {

    return &const_cast<Node&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::node_value_getter)
{
    WebIDL::log_trace(vm, "NodePrototype::node_value_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->node_value(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    auto retval = retval_or_exception.release_value();

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::node_value_setter)
{
    WebIDL::log_trace(vm, "NodePrototype::node_value_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_node_value(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::text_content_getter)
{
    WebIDL::log_trace(vm, "NodePrototype::text_content_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->text_content(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    auto retval = retval_or_exception.release_value();

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::text_content_setter)
{
    WebIDL::log_trace(vm, "NodePrototype::text_content_setter");
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

    Optional<Utf16String> cpp_value;
    if (!value.is_nullish())
        cpp_value = TRY(WebIDL::to_utf16_string(vm, value));

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_text_content(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::get_root_node)
{
    WebIDL::log_trace(vm, "NodePrototype::get_root_node");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "GetRootNodeOptions");

    GetRootNodeOptions options {};

    auto composed_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        composed_property_value_0 = TRY(arg0.as_object().get("composed"_utf16_fly_string));

    bool composed_value_0;

    if (!composed_property_value_0.is_undefined())

    composed_value_0 = composed_property_value_0.to_boolean();

    else
        composed_value_0 = static_cast<bool>(false);

    options.composed = composed_value_0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_root_node(options); }));

    return &const_cast<Node&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::has_child_nodes)
{
    WebIDL::log_trace(vm, "NodePrototype::has_child_nodes");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->has_child_nodes(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::normalize)
{
    WebIDL::log_trace(vm, "NodePrototype::normalize");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->normalize(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::clone_node)
{
    WebIDL::log_trace(vm, "NodePrototype::clone_node");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    bool deep;

    if (!arg0.is_undefined())

    deep = arg0.to_boolean();

    else
        deep = static_cast<bool>(false);

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->clone_node_binding(deep); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return &const_cast<Node&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::is_equal_node)
{
    WebIDL::log_trace(vm, "NodePrototype::is_equal_node");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "isEqualNode");

    auto arg0 = vm.argument(0);

    GC::Ptr<Node> other_node;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<Node>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

        other_node = &static_cast<Node&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_equal_node(other_node); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::is_same_node)
{
    WebIDL::log_trace(vm, "NodePrototype::is_same_node");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "isSameNode");

    auto arg0 = vm.argument(0);

    GC::Ptr<Node> other_node;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<Node>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

        other_node = &static_cast<Node&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_same_node(other_node); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::compare_document_position)
{
    WebIDL::log_trace(vm, "NodePrototype::compare_document_position");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "compareDocumentPosition");

    auto arg0 = vm.argument(0);

    GC::Ptr<Node> other_node;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<Node>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

        other_node = &static_cast<Node&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->compare_document_position(other_node); }));

    return JS::Value(static_cast<WebIDL::UnsignedShort>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::contains)
{
    WebIDL::log_trace(vm, "NodePrototype::contains");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "contains");

    auto arg0 = vm.argument(0);

    GC::Ptr<Node> other;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<Node>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

        other = &static_cast<Node&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->contains(other); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::lookup_prefix)
{
    WebIDL::log_trace(vm, "NodePrototype::lookup_prefix");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "lookupPrefix");

    auto arg0 = vm.argument(0);

    Optional<String> namespace_;
    if (!arg0.is_nullish())
        namespace_ = TRY(WebIDL::to_string(vm, arg0));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->lookup_prefix(namespace_); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::lookup_namespace_uri)
{
    WebIDL::log_trace(vm, "NodePrototype::lookup_namespace_uri");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "lookupNamespaceURI");

    auto arg0 = vm.argument(0);

    Optional<String> prefix;
    if (!arg0.is_nullish())
        prefix = TRY(WebIDL::to_string(vm, arg0));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->lookup_namespace_uri(prefix); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::is_default_namespace)
{
    WebIDL::log_trace(vm, "NodePrototype::is_default_namespace");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "isDefaultNamespace");

    auto arg0 = vm.argument(0);

    Optional<String> namespace_;
    if (!arg0.is_nullish())
        namespace_ = TRY(WebIDL::to_string(vm, arg0));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_default_namespace(namespace_); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::insert_before)
{
    WebIDL::log_trace(vm, "NodePrototype::insert_before");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "insertBefore", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Node>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

    auto& node = static_cast<Node&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    GC::Ptr<Node> child;

    if (!arg1.is_nullish()) {
        if (!arg1.is_object() || !is<Node>(arg1.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

        child = &static_cast<Node&>(arg1.as_object());
    }

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->pre_insert(node, child); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return &const_cast<Node&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::append_child)
{
    WebIDL::log_trace(vm, "NodePrototype::append_child");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "appendChild");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Node>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

    auto& node = static_cast<Node&>(arg0.as_object());

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->append_child(node); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return &const_cast<Node&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::replace_child)
{
    WebIDL::log_trace(vm, "NodePrototype::replace_child");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "replaceChild", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Node>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

    auto& node = static_cast<Node&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<Node>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

    auto& child = static_cast<Node&>(arg1.as_object());

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->replace_child(node, child); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return &const_cast<Node&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(NodePrototype::remove_child)
{
    WebIDL::log_trace(vm, "NodePrototype::remove_child");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "removeChild");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Node>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

    auto& child = static_cast<Node&>(arg0.as_object());

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->pre_remove(child); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return &const_cast<Node&>(*retval);

}

} // namespace Web::Bindings
