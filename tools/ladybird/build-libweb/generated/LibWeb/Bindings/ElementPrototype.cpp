
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
#include <LibWeb/Bindings/ElementPrototype.h>
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

#if __has_include(<LibWeb/Bindings/NodePrototype.h>)
#    include <LibWeb/Bindings/NodePrototype.h>
#endif


#include <LibWeb/Bindings/MainThreadVM.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Element.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/Animatable.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StylePropertyMapReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Attr.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DOMTokenList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/NamedNodeMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Node.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/NodeList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/ShadowRoot.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRectList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLSlotElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Window.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedHTML.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedTypePolicy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/Animation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/KeyframeEffect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Document.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/HTMLCollection.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DocumentFragment.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Geometry/DOMRectReadOnly.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Text.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLElement.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/AnimationEffect.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/AnimationTimeline.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/DocumentTimeline.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/FontFaceSet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StyleSheetList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/CDATASection.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Comment.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DOMImplementation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/DocumentType.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/CharacterData.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/DOMStringMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ElementInternals.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSNumericValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/FontFace.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleSheet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/XMLDocument.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbstractRange.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Scripting/Fetching.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleProperties.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StylePropertyMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CustomElements/CustomStateSet.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSUnitValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRule.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRuleList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/MediaList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StyleSheet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleDeclaration.h>

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

GC_DEFINE_ALLOCATOR(ElementPrototype);

ElementPrototype::ElementPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

ElementPrototype::~ElementPrototype()
{
}

void ElementPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<NodePrototype>(realm, "Node"_fly_string));


    auto unscopable_object = JS::Object::create(realm, nullptr);

    auto native_namespace_uri_getter = JS::NativeFunction::create(realm, namespace_uri_getter, 0, "namespaceURI"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_namespace_uri_setter;

    define_direct_accessor("namespaceURI"_utf16_fly_string, native_namespace_uri_getter, native_namespace_uri_setter, default_attributes);

    auto native_prefix_getter = JS::NativeFunction::create(realm, prefix_getter, 0, "prefix"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_prefix_setter;

    define_direct_accessor("prefix"_utf16_fly_string, native_prefix_getter, native_prefix_setter, default_attributes);

    auto native_local_name_getter = JS::NativeFunction::create(realm, local_name_getter, 0, "localName"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_local_name_setter;

    define_direct_accessor("localName"_utf16_fly_string, native_local_name_getter, native_local_name_setter, default_attributes);

    auto native_tag_name_getter = JS::NativeFunction::create(realm, tag_name_getter, 0, "tagName"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_tag_name_setter;

    define_direct_accessor("tagName"_utf16_fly_string, native_tag_name_getter, native_tag_name_setter, default_attributes);

    auto native_id_getter = JS::NativeFunction::create(realm, id_getter, 0, "id"_utf16_fly_string, &realm, "get"sv);

    auto native_id_setter = JS::NativeFunction::create(realm, id_setter, 1, "id"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("id"_utf16_fly_string, native_id_getter, native_id_setter, default_attributes);

    auto native_class_name_getter = JS::NativeFunction::create(realm, class_name_getter, 0, "className"_utf16_fly_string, &realm, "get"sv);

    auto native_class_name_setter = JS::NativeFunction::create(realm, class_name_setter, 1, "className"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("className"_utf16_fly_string, native_class_name_getter, native_class_name_setter, default_attributes);

    auto native_class_list_getter = JS::NativeFunction::create(realm, class_list_getter, 0, "classList"_utf16_fly_string, &realm, "get"sv);

    auto native_class_list_setter = JS::NativeFunction::create(realm, class_list_setter, 1, "classList"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("classList"_utf16_fly_string, native_class_list_getter, native_class_list_setter, default_attributes);

    auto native_slot_getter = JS::NativeFunction::create(realm, slot_getter, 0, "slot"_utf16_fly_string, &realm, "get"sv);

    auto native_slot_setter = JS::NativeFunction::create(realm, slot_setter, 1, "slot"_utf16_fly_string, &realm, "set"sv);

    MUST(unscopable_object->create_data_property("slot"_utf16_fly_string, JS::Value(true)));

    define_direct_accessor("slot"_utf16_fly_string, native_slot_getter, native_slot_setter, default_attributes);

    auto native_attributes_getter = JS::NativeFunction::create(realm, attributes_getter, 0, "attributes"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_attributes_setter;

    define_direct_accessor("attributes"_utf16_fly_string, native_attributes_getter, native_attributes_setter, default_attributes);

    auto native_shadow_root_getter = JS::NativeFunction::create(realm, shadow_root_getter, 0, "shadowRoot"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_shadow_root_setter;

    define_direct_accessor("shadowRoot"_utf16_fly_string, native_shadow_root_getter, native_shadow_root_setter, default_attributes);

    auto native_next_element_sibling_getter = JS::NativeFunction::create(realm, next_element_sibling_getter, 0, "nextElementSibling"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_next_element_sibling_setter;

    define_direct_accessor("nextElementSibling"_utf16_fly_string, native_next_element_sibling_getter, native_next_element_sibling_setter, default_attributes);

    auto native_previous_element_sibling_getter = JS::NativeFunction::create(realm, previous_element_sibling_getter, 0, "previousElementSibling"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_previous_element_sibling_setter;

    define_direct_accessor("previousElementSibling"_utf16_fly_string, native_previous_element_sibling_getter, native_previous_element_sibling_setter, default_attributes);

    auto native_scroll_top_getter = JS::NativeFunction::create(realm, scroll_top_getter, 0, "scrollTop"_utf16_fly_string, &realm, "get"sv);

    auto native_scroll_top_setter = JS::NativeFunction::create(realm, scroll_top_setter, 1, "scrollTop"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("scrollTop"_utf16_fly_string, native_scroll_top_getter, native_scroll_top_setter, default_attributes);

    auto native_scroll_left_getter = JS::NativeFunction::create(realm, scroll_left_getter, 0, "scrollLeft"_utf16_fly_string, &realm, "get"sv);

    auto native_scroll_left_setter = JS::NativeFunction::create(realm, scroll_left_setter, 1, "scrollLeft"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("scrollLeft"_utf16_fly_string, native_scroll_left_getter, native_scroll_left_setter, default_attributes);

    auto native_scroll_width_getter = JS::NativeFunction::create(realm, scroll_width_getter, 0, "scrollWidth"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_scroll_width_setter;

    define_direct_accessor("scrollWidth"_utf16_fly_string, native_scroll_width_getter, native_scroll_width_setter, default_attributes);

    auto native_scroll_height_getter = JS::NativeFunction::create(realm, scroll_height_getter, 0, "scrollHeight"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_scroll_height_setter;

    define_direct_accessor("scrollHeight"_utf16_fly_string, native_scroll_height_getter, native_scroll_height_setter, default_attributes);

    auto native_client_top_getter = JS::NativeFunction::create(realm, client_top_getter, 0, "clientTop"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_client_top_setter;

    define_direct_accessor("clientTop"_utf16_fly_string, native_client_top_getter, native_client_top_setter, default_attributes);

    auto native_client_left_getter = JS::NativeFunction::create(realm, client_left_getter, 0, "clientLeft"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_client_left_setter;

    define_direct_accessor("clientLeft"_utf16_fly_string, native_client_left_getter, native_client_left_setter, default_attributes);

    auto native_client_width_getter = JS::NativeFunction::create(realm, client_width_getter, 0, "clientWidth"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_client_width_setter;

    define_direct_accessor("clientWidth"_utf16_fly_string, native_client_width_getter, native_client_width_setter, default_attributes);

    auto native_client_height_getter = JS::NativeFunction::create(realm, client_height_getter, 0, "clientHeight"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_client_height_setter;

    define_direct_accessor("clientHeight"_utf16_fly_string, native_client_height_getter, native_client_height_setter, default_attributes);

    auto native_current_css_zoom_getter = JS::NativeFunction::create(realm, current_css_zoom_getter, 0, "currentCSSZoom"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_current_css_zoom_setter;

    define_direct_accessor("currentCSSZoom"_utf16_fly_string, native_current_css_zoom_getter, native_current_css_zoom_setter, default_attributes);

    auto native_inner_html_getter = JS::NativeFunction::create(realm, inner_html_getter, 0, "innerHTML"_utf16_fly_string, &realm, "get"sv);

    auto native_inner_html_setter = JS::NativeFunction::create(realm, inner_html_setter, 1, "innerHTML"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("innerHTML"_utf16_fly_string, native_inner_html_getter, native_inner_html_setter, default_attributes);

    auto native_outer_html_getter = JS::NativeFunction::create(realm, outer_html_getter, 0, "outerHTML"_utf16_fly_string, &realm, "get"sv);

    auto native_outer_html_setter = JS::NativeFunction::create(realm, outer_html_setter, 1, "outerHTML"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("outerHTML"_utf16_fly_string, native_outer_html_getter, native_outer_html_setter, default_attributes);

    auto native_part_getter = JS::NativeFunction::create(realm, part_getter, 0, "part"_utf16_fly_string, &realm, "get"sv);

    auto native_part_setter = JS::NativeFunction::create(realm, part_setter, 1, "part"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("part"_utf16_fly_string, native_part_getter, native_part_setter, default_attributes);

    auto native_onfullscreenchange_getter = JS::NativeFunction::create(realm, onfullscreenchange_getter, 0, "onfullscreenchange"_utf16_fly_string, &realm, "get"sv);

    auto native_onfullscreenchange_setter = JS::NativeFunction::create(realm, onfullscreenchange_setter, 1, "onfullscreenchange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onfullscreenchange"_utf16_fly_string, native_onfullscreenchange_getter, native_onfullscreenchange_setter, default_attributes);

    auto native_onfullscreenerror_getter = JS::NativeFunction::create(realm, onfullscreenerror_getter, 0, "onfullscreenerror"_utf16_fly_string, &realm, "get"sv);

    auto native_onfullscreenerror_setter = JS::NativeFunction::create(realm, onfullscreenerror_setter, 1, "onfullscreenerror"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onfullscreenerror"_utf16_fly_string, native_onfullscreenerror_getter, native_onfullscreenerror_setter, default_attributes);

    auto native_assigned_slot_getter = JS::NativeFunction::create(realm, assigned_slot_getter, 0, "assignedSlot"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_assigned_slot_setter;

    define_direct_accessor("assignedSlot"_utf16_fly_string, native_assigned_slot_getter, native_assigned_slot_setter, default_attributes);

    auto native_role_getter = JS::NativeFunction::create(realm, role_getter, 0, "role"_utf16_fly_string, &realm, "get"sv);

    auto native_role_setter = JS::NativeFunction::create(realm, role_setter, 1, "role"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("role"_utf16_fly_string, native_role_getter, native_role_setter, default_attributes);

    auto native_aria_active_descendant_element_getter = JS::NativeFunction::create(realm, aria_active_descendant_element_getter, 0, "ariaActiveDescendantElement"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_active_descendant_element_setter = JS::NativeFunction::create(realm, aria_active_descendant_element_setter, 1, "ariaActiveDescendantElement"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaActiveDescendantElement"_utf16_fly_string, native_aria_active_descendant_element_getter, native_aria_active_descendant_element_setter, default_attributes);

    auto native_aria_atomic_getter = JS::NativeFunction::create(realm, aria_atomic_getter, 0, "ariaAtomic"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_atomic_setter = JS::NativeFunction::create(realm, aria_atomic_setter, 1, "ariaAtomic"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaAtomic"_utf16_fly_string, native_aria_atomic_getter, native_aria_atomic_setter, default_attributes);

    auto native_aria_auto_complete_getter = JS::NativeFunction::create(realm, aria_auto_complete_getter, 0, "ariaAutoComplete"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_auto_complete_setter = JS::NativeFunction::create(realm, aria_auto_complete_setter, 1, "ariaAutoComplete"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaAutoComplete"_utf16_fly_string, native_aria_auto_complete_getter, native_aria_auto_complete_setter, default_attributes);

    auto native_aria_braille_label_getter = JS::NativeFunction::create(realm, aria_braille_label_getter, 0, "ariaBrailleLabel"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_braille_label_setter = JS::NativeFunction::create(realm, aria_braille_label_setter, 1, "ariaBrailleLabel"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaBrailleLabel"_utf16_fly_string, native_aria_braille_label_getter, native_aria_braille_label_setter, default_attributes);

    auto native_aria_braille_role_description_getter = JS::NativeFunction::create(realm, aria_braille_role_description_getter, 0, "ariaBrailleRoleDescription"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_braille_role_description_setter = JS::NativeFunction::create(realm, aria_braille_role_description_setter, 1, "ariaBrailleRoleDescription"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaBrailleRoleDescription"_utf16_fly_string, native_aria_braille_role_description_getter, native_aria_braille_role_description_setter, default_attributes);

    auto native_aria_busy_getter = JS::NativeFunction::create(realm, aria_busy_getter, 0, "ariaBusy"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_busy_setter = JS::NativeFunction::create(realm, aria_busy_setter, 1, "ariaBusy"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaBusy"_utf16_fly_string, native_aria_busy_getter, native_aria_busy_setter, default_attributes);

    auto native_aria_checked_getter = JS::NativeFunction::create(realm, aria_checked_getter, 0, "ariaChecked"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_checked_setter = JS::NativeFunction::create(realm, aria_checked_setter, 1, "ariaChecked"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaChecked"_utf16_fly_string, native_aria_checked_getter, native_aria_checked_setter, default_attributes);

    auto native_aria_col_count_getter = JS::NativeFunction::create(realm, aria_col_count_getter, 0, "ariaColCount"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_col_count_setter = JS::NativeFunction::create(realm, aria_col_count_setter, 1, "ariaColCount"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaColCount"_utf16_fly_string, native_aria_col_count_getter, native_aria_col_count_setter, default_attributes);

    auto native_aria_col_index_getter = JS::NativeFunction::create(realm, aria_col_index_getter, 0, "ariaColIndex"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_col_index_setter = JS::NativeFunction::create(realm, aria_col_index_setter, 1, "ariaColIndex"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaColIndex"_utf16_fly_string, native_aria_col_index_getter, native_aria_col_index_setter, default_attributes);

    auto native_aria_col_index_text_getter = JS::NativeFunction::create(realm, aria_col_index_text_getter, 0, "ariaColIndexText"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_col_index_text_setter = JS::NativeFunction::create(realm, aria_col_index_text_setter, 1, "ariaColIndexText"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaColIndexText"_utf16_fly_string, native_aria_col_index_text_getter, native_aria_col_index_text_setter, default_attributes);

    auto native_aria_col_span_getter = JS::NativeFunction::create(realm, aria_col_span_getter, 0, "ariaColSpan"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_col_span_setter = JS::NativeFunction::create(realm, aria_col_span_setter, 1, "ariaColSpan"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaColSpan"_utf16_fly_string, native_aria_col_span_getter, native_aria_col_span_setter, default_attributes);

    auto native_aria_controls_elements_getter = JS::NativeFunction::create(realm, aria_controls_elements_getter, 0, "ariaControlsElements"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_controls_elements_setter = JS::NativeFunction::create(realm, aria_controls_elements_setter, 1, "ariaControlsElements"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaControlsElements"_utf16_fly_string, native_aria_controls_elements_getter, native_aria_controls_elements_setter, default_attributes);

    auto native_aria_current_getter = JS::NativeFunction::create(realm, aria_current_getter, 0, "ariaCurrent"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_current_setter = JS::NativeFunction::create(realm, aria_current_setter, 1, "ariaCurrent"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaCurrent"_utf16_fly_string, native_aria_current_getter, native_aria_current_setter, default_attributes);

    auto native_aria_described_by_elements_getter = JS::NativeFunction::create(realm, aria_described_by_elements_getter, 0, "ariaDescribedByElements"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_described_by_elements_setter = JS::NativeFunction::create(realm, aria_described_by_elements_setter, 1, "ariaDescribedByElements"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaDescribedByElements"_utf16_fly_string, native_aria_described_by_elements_getter, native_aria_described_by_elements_setter, default_attributes);

    auto native_aria_description_getter = JS::NativeFunction::create(realm, aria_description_getter, 0, "ariaDescription"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_description_setter = JS::NativeFunction::create(realm, aria_description_setter, 1, "ariaDescription"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaDescription"_utf16_fly_string, native_aria_description_getter, native_aria_description_setter, default_attributes);

    auto native_aria_details_elements_getter = JS::NativeFunction::create(realm, aria_details_elements_getter, 0, "ariaDetailsElements"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_details_elements_setter = JS::NativeFunction::create(realm, aria_details_elements_setter, 1, "ariaDetailsElements"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaDetailsElements"_utf16_fly_string, native_aria_details_elements_getter, native_aria_details_elements_setter, default_attributes);

    auto native_aria_disabled_getter = JS::NativeFunction::create(realm, aria_disabled_getter, 0, "ariaDisabled"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_disabled_setter = JS::NativeFunction::create(realm, aria_disabled_setter, 1, "ariaDisabled"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaDisabled"_utf16_fly_string, native_aria_disabled_getter, native_aria_disabled_setter, default_attributes);

    auto native_aria_error_message_elements_getter = JS::NativeFunction::create(realm, aria_error_message_elements_getter, 0, "ariaErrorMessageElements"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_error_message_elements_setter = JS::NativeFunction::create(realm, aria_error_message_elements_setter, 1, "ariaErrorMessageElements"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaErrorMessageElements"_utf16_fly_string, native_aria_error_message_elements_getter, native_aria_error_message_elements_setter, default_attributes);

    auto native_aria_expanded_getter = JS::NativeFunction::create(realm, aria_expanded_getter, 0, "ariaExpanded"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_expanded_setter = JS::NativeFunction::create(realm, aria_expanded_setter, 1, "ariaExpanded"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaExpanded"_utf16_fly_string, native_aria_expanded_getter, native_aria_expanded_setter, default_attributes);

    auto native_aria_flow_to_elements_getter = JS::NativeFunction::create(realm, aria_flow_to_elements_getter, 0, "ariaFlowToElements"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_flow_to_elements_setter = JS::NativeFunction::create(realm, aria_flow_to_elements_setter, 1, "ariaFlowToElements"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaFlowToElements"_utf16_fly_string, native_aria_flow_to_elements_getter, native_aria_flow_to_elements_setter, default_attributes);

    auto native_aria_has_popup_getter = JS::NativeFunction::create(realm, aria_has_popup_getter, 0, "ariaHasPopup"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_has_popup_setter = JS::NativeFunction::create(realm, aria_has_popup_setter, 1, "ariaHasPopup"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaHasPopup"_utf16_fly_string, native_aria_has_popup_getter, native_aria_has_popup_setter, default_attributes);

    auto native_aria_hidden_getter = JS::NativeFunction::create(realm, aria_hidden_getter, 0, "ariaHidden"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_hidden_setter = JS::NativeFunction::create(realm, aria_hidden_setter, 1, "ariaHidden"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaHidden"_utf16_fly_string, native_aria_hidden_getter, native_aria_hidden_setter, default_attributes);

    auto native_aria_invalid_getter = JS::NativeFunction::create(realm, aria_invalid_getter, 0, "ariaInvalid"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_invalid_setter = JS::NativeFunction::create(realm, aria_invalid_setter, 1, "ariaInvalid"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaInvalid"_utf16_fly_string, native_aria_invalid_getter, native_aria_invalid_setter, default_attributes);

    auto native_aria_key_shortcuts_getter = JS::NativeFunction::create(realm, aria_key_shortcuts_getter, 0, "ariaKeyShortcuts"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_key_shortcuts_setter = JS::NativeFunction::create(realm, aria_key_shortcuts_setter, 1, "ariaKeyShortcuts"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaKeyShortcuts"_utf16_fly_string, native_aria_key_shortcuts_getter, native_aria_key_shortcuts_setter, default_attributes);

    auto native_aria_label_getter = JS::NativeFunction::create(realm, aria_label_getter, 0, "ariaLabel"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_label_setter = JS::NativeFunction::create(realm, aria_label_setter, 1, "ariaLabel"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaLabel"_utf16_fly_string, native_aria_label_getter, native_aria_label_setter, default_attributes);

    auto native_aria_labelled_by_elements_getter = JS::NativeFunction::create(realm, aria_labelled_by_elements_getter, 0, "ariaLabelledByElements"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_labelled_by_elements_setter = JS::NativeFunction::create(realm, aria_labelled_by_elements_setter, 1, "ariaLabelledByElements"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaLabelledByElements"_utf16_fly_string, native_aria_labelled_by_elements_getter, native_aria_labelled_by_elements_setter, default_attributes);

    auto native_aria_level_getter = JS::NativeFunction::create(realm, aria_level_getter, 0, "ariaLevel"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_level_setter = JS::NativeFunction::create(realm, aria_level_setter, 1, "ariaLevel"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaLevel"_utf16_fly_string, native_aria_level_getter, native_aria_level_setter, default_attributes);

    auto native_aria_live_getter = JS::NativeFunction::create(realm, aria_live_getter, 0, "ariaLive"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_live_setter = JS::NativeFunction::create(realm, aria_live_setter, 1, "ariaLive"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaLive"_utf16_fly_string, native_aria_live_getter, native_aria_live_setter, default_attributes);

    auto native_aria_modal_getter = JS::NativeFunction::create(realm, aria_modal_getter, 0, "ariaModal"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_modal_setter = JS::NativeFunction::create(realm, aria_modal_setter, 1, "ariaModal"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaModal"_utf16_fly_string, native_aria_modal_getter, native_aria_modal_setter, default_attributes);

    auto native_aria_multi_line_getter = JS::NativeFunction::create(realm, aria_multi_line_getter, 0, "ariaMultiLine"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_multi_line_setter = JS::NativeFunction::create(realm, aria_multi_line_setter, 1, "ariaMultiLine"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaMultiLine"_utf16_fly_string, native_aria_multi_line_getter, native_aria_multi_line_setter, default_attributes);

    auto native_aria_multi_selectable_getter = JS::NativeFunction::create(realm, aria_multi_selectable_getter, 0, "ariaMultiSelectable"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_multi_selectable_setter = JS::NativeFunction::create(realm, aria_multi_selectable_setter, 1, "ariaMultiSelectable"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaMultiSelectable"_utf16_fly_string, native_aria_multi_selectable_getter, native_aria_multi_selectable_setter, default_attributes);

    auto native_aria_orientation_getter = JS::NativeFunction::create(realm, aria_orientation_getter, 0, "ariaOrientation"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_orientation_setter = JS::NativeFunction::create(realm, aria_orientation_setter, 1, "ariaOrientation"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaOrientation"_utf16_fly_string, native_aria_orientation_getter, native_aria_orientation_setter, default_attributes);

    auto native_aria_owns_elements_getter = JS::NativeFunction::create(realm, aria_owns_elements_getter, 0, "ariaOwnsElements"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_owns_elements_setter = JS::NativeFunction::create(realm, aria_owns_elements_setter, 1, "ariaOwnsElements"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaOwnsElements"_utf16_fly_string, native_aria_owns_elements_getter, native_aria_owns_elements_setter, default_attributes);

    auto native_aria_placeholder_getter = JS::NativeFunction::create(realm, aria_placeholder_getter, 0, "ariaPlaceholder"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_placeholder_setter = JS::NativeFunction::create(realm, aria_placeholder_setter, 1, "ariaPlaceholder"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaPlaceholder"_utf16_fly_string, native_aria_placeholder_getter, native_aria_placeholder_setter, default_attributes);

    auto native_aria_pos_in_set_getter = JS::NativeFunction::create(realm, aria_pos_in_set_getter, 0, "ariaPosInSet"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_pos_in_set_setter = JS::NativeFunction::create(realm, aria_pos_in_set_setter, 1, "ariaPosInSet"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaPosInSet"_utf16_fly_string, native_aria_pos_in_set_getter, native_aria_pos_in_set_setter, default_attributes);

    auto native_aria_pressed_getter = JS::NativeFunction::create(realm, aria_pressed_getter, 0, "ariaPressed"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_pressed_setter = JS::NativeFunction::create(realm, aria_pressed_setter, 1, "ariaPressed"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaPressed"_utf16_fly_string, native_aria_pressed_getter, native_aria_pressed_setter, default_attributes);

    auto native_aria_read_only_getter = JS::NativeFunction::create(realm, aria_read_only_getter, 0, "ariaReadOnly"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_read_only_setter = JS::NativeFunction::create(realm, aria_read_only_setter, 1, "ariaReadOnly"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaReadOnly"_utf16_fly_string, native_aria_read_only_getter, native_aria_read_only_setter, default_attributes);

    auto native_aria_relevant_getter = JS::NativeFunction::create(realm, aria_relevant_getter, 0, "ariaRelevant"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_relevant_setter = JS::NativeFunction::create(realm, aria_relevant_setter, 1, "ariaRelevant"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaRelevant"_utf16_fly_string, native_aria_relevant_getter, native_aria_relevant_setter, default_attributes);

    auto native_aria_required_getter = JS::NativeFunction::create(realm, aria_required_getter, 0, "ariaRequired"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_required_setter = JS::NativeFunction::create(realm, aria_required_setter, 1, "ariaRequired"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaRequired"_utf16_fly_string, native_aria_required_getter, native_aria_required_setter, default_attributes);

    auto native_aria_role_description_getter = JS::NativeFunction::create(realm, aria_role_description_getter, 0, "ariaRoleDescription"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_role_description_setter = JS::NativeFunction::create(realm, aria_role_description_setter, 1, "ariaRoleDescription"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaRoleDescription"_utf16_fly_string, native_aria_role_description_getter, native_aria_role_description_setter, default_attributes);

    auto native_aria_row_count_getter = JS::NativeFunction::create(realm, aria_row_count_getter, 0, "ariaRowCount"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_row_count_setter = JS::NativeFunction::create(realm, aria_row_count_setter, 1, "ariaRowCount"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaRowCount"_utf16_fly_string, native_aria_row_count_getter, native_aria_row_count_setter, default_attributes);

    auto native_aria_row_index_getter = JS::NativeFunction::create(realm, aria_row_index_getter, 0, "ariaRowIndex"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_row_index_setter = JS::NativeFunction::create(realm, aria_row_index_setter, 1, "ariaRowIndex"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaRowIndex"_utf16_fly_string, native_aria_row_index_getter, native_aria_row_index_setter, default_attributes);

    auto native_aria_row_index_text_getter = JS::NativeFunction::create(realm, aria_row_index_text_getter, 0, "ariaRowIndexText"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_row_index_text_setter = JS::NativeFunction::create(realm, aria_row_index_text_setter, 1, "ariaRowIndexText"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaRowIndexText"_utf16_fly_string, native_aria_row_index_text_getter, native_aria_row_index_text_setter, default_attributes);

    auto native_aria_row_span_getter = JS::NativeFunction::create(realm, aria_row_span_getter, 0, "ariaRowSpan"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_row_span_setter = JS::NativeFunction::create(realm, aria_row_span_setter, 1, "ariaRowSpan"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaRowSpan"_utf16_fly_string, native_aria_row_span_getter, native_aria_row_span_setter, default_attributes);

    auto native_aria_selected_getter = JS::NativeFunction::create(realm, aria_selected_getter, 0, "ariaSelected"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_selected_setter = JS::NativeFunction::create(realm, aria_selected_setter, 1, "ariaSelected"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaSelected"_utf16_fly_string, native_aria_selected_getter, native_aria_selected_setter, default_attributes);

    auto native_aria_set_size_getter = JS::NativeFunction::create(realm, aria_set_size_getter, 0, "ariaSetSize"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_set_size_setter = JS::NativeFunction::create(realm, aria_set_size_setter, 1, "ariaSetSize"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaSetSize"_utf16_fly_string, native_aria_set_size_getter, native_aria_set_size_setter, default_attributes);

    auto native_aria_sort_getter = JS::NativeFunction::create(realm, aria_sort_getter, 0, "ariaSort"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_sort_setter = JS::NativeFunction::create(realm, aria_sort_setter, 1, "ariaSort"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaSort"_utf16_fly_string, native_aria_sort_getter, native_aria_sort_setter, default_attributes);

    auto native_aria_value_max_getter = JS::NativeFunction::create(realm, aria_value_max_getter, 0, "ariaValueMax"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_value_max_setter = JS::NativeFunction::create(realm, aria_value_max_setter, 1, "ariaValueMax"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaValueMax"_utf16_fly_string, native_aria_value_max_getter, native_aria_value_max_setter, default_attributes);

    auto native_aria_value_min_getter = JS::NativeFunction::create(realm, aria_value_min_getter, 0, "ariaValueMin"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_value_min_setter = JS::NativeFunction::create(realm, aria_value_min_setter, 1, "ariaValueMin"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaValueMin"_utf16_fly_string, native_aria_value_min_getter, native_aria_value_min_setter, default_attributes);

    auto native_aria_value_now_getter = JS::NativeFunction::create(realm, aria_value_now_getter, 0, "ariaValueNow"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_value_now_setter = JS::NativeFunction::create(realm, aria_value_now_setter, 1, "ariaValueNow"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaValueNow"_utf16_fly_string, native_aria_value_now_getter, native_aria_value_now_setter, default_attributes);

    auto native_aria_value_text_getter = JS::NativeFunction::create(realm, aria_value_text_getter, 0, "ariaValueText"_utf16_fly_string, &realm, "get"sv);

    auto native_aria_value_text_setter = JS::NativeFunction::create(realm, aria_value_text_setter, 1, "ariaValueText"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ariaValueText"_utf16_fly_string, native_aria_value_text_getter, native_aria_value_text_setter, default_attributes);

    auto native_children_getter = JS::NativeFunction::create(realm, children_getter, 0, "children"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_children_setter;

    define_direct_accessor("children"_utf16_fly_string, native_children_getter, native_children_setter, default_attributes);

    auto native_first_element_child_getter = JS::NativeFunction::create(realm, first_element_child_getter, 0, "firstElementChild"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_first_element_child_setter;

    define_direct_accessor("firstElementChild"_utf16_fly_string, native_first_element_child_getter, native_first_element_child_setter, default_attributes);

    auto native_last_element_child_getter = JS::NativeFunction::create(realm, last_element_child_getter, 0, "lastElementChild"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_last_element_child_setter;

    define_direct_accessor("lastElementChild"_utf16_fly_string, native_last_element_child_getter, native_last_element_child_setter, default_attributes);

    auto native_child_element_count_getter = JS::NativeFunction::create(realm, child_element_count_getter, 0, "childElementCount"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_child_element_count_setter;

    define_direct_accessor("childElementCount"_utf16_fly_string, native_child_element_count_getter, native_child_element_count_setter, default_attributes);

    define_native_function(realm, "hasAttributes"_utf16_fly_string, has_attributes, 0, default_attributes);

    define_native_function(realm, "getAttributeNames"_utf16_fly_string, get_attribute_names, 0, default_attributes);

    define_native_function(realm, "getAttribute"_utf16_fly_string, get_attribute, 1, default_attributes);

    define_native_function(realm, "getAttributeNS"_utf16_fly_string, get_attribute_ns, 2, default_attributes);

    define_native_function(realm, "setAttribute"_utf16_fly_string, set_attribute, 2, default_attributes);

    define_native_function(realm, "setAttributeNS"_utf16_fly_string, set_attribute_ns, 3, default_attributes);

    define_native_function(realm, "removeAttribute"_utf16_fly_string, remove_attribute, 1, default_attributes);

    define_native_function(realm, "removeAttributeNS"_utf16_fly_string, remove_attribute_ns, 2, default_attributes);

    define_native_function(realm, "toggleAttribute"_utf16_fly_string, toggle_attribute, 1, default_attributes);

    define_native_function(realm, "hasAttribute"_utf16_fly_string, has_attribute, 1, default_attributes);

    define_native_function(realm, "hasAttributeNS"_utf16_fly_string, has_attribute_ns, 2, default_attributes);

    define_native_function(realm, "getAttributeNode"_utf16_fly_string, get_attribute_node, 1, default_attributes);

    define_native_function(realm, "getAttributeNodeNS"_utf16_fly_string, get_attribute_node_ns, 2, default_attributes);

    define_native_function(realm, "setAttributeNode"_utf16_fly_string, set_attribute_node, 1, default_attributes);

    define_native_function(realm, "setAttributeNodeNS"_utf16_fly_string, set_attribute_node_ns, 1, default_attributes);

    define_native_function(realm, "removeAttributeNode"_utf16_fly_string, remove_attribute_node, 1, default_attributes);

    define_native_function(realm, "attachShadow"_utf16_fly_string, attach_shadow, 1, default_attributes);

    define_native_function(realm, "matches"_utf16_fly_string, matches, 1, default_attributes);

    define_native_function(realm, "closest"_utf16_fly_string, closest, 1, default_attributes);

    define_native_function(realm, "webkitMatchesSelector"_utf16_fly_string, webkit_matches_selector, 1, default_attributes);

    define_native_function(realm, "getElementsByTagName"_utf16_fly_string, get_elements_by_tag_name, 1, default_attributes);

    define_native_function(realm, "getElementsByTagNameNS"_utf16_fly_string, get_elements_by_tag_name_ns, 2, default_attributes);

    define_native_function(realm, "getElementsByClassName"_utf16_fly_string, get_elements_by_class_name, 1, default_attributes);

    define_native_function(realm, "insertAdjacentElement"_utf16_fly_string, insert_adjacent_element, 2, default_attributes);

    define_native_function(realm, "insertAdjacentText"_utf16_fly_string, insert_adjacent_text, 2, default_attributes);

    define_native_function(realm, "getClientRects"_utf16_fly_string, get_client_rects, 0, default_attributes);

    define_native_function(realm, "getBoundingClientRect"_utf16_fly_string, get_bounding_client_rect, 0, default_attributes);

    define_native_function(realm, "checkVisibility"_utf16_fly_string, check_visibility, 0, default_attributes);

    define_native_function(realm, "scrollIntoView"_utf16_fly_string, scroll_into_view, 0, default_attributes);

    define_native_function(realm, "scroll"_utf16_fly_string, scroll, 0, default_attributes);

    define_native_function(realm, "scrollTo"_utf16_fly_string, scroll_to, 0, default_attributes);

    define_native_function(realm, "scrollBy"_utf16_fly_string, scroll_by, 0, default_attributes);

    define_native_function(realm, "setHTMLUnsafe"_utf16_fly_string, set_html_unsafe, 1, default_attributes);

    define_native_function(realm, "getHTML"_utf16_fly_string, get_html, 0, default_attributes);

    define_native_function(realm, "insertAdjacentHTML"_utf16_fly_string, insert_adjacent_html, 2, default_attributes);

    define_native_function(realm, "setPointerCapture"_utf16_fly_string, set_pointer_capture, 1, default_attributes);

    define_native_function(realm, "releasePointerCapture"_utf16_fly_string, release_pointer_capture, 1, default_attributes);

    define_native_function(realm, "hasPointerCapture"_utf16_fly_string, has_pointer_capture, 1, default_attributes);

    define_native_function(realm, "requestPointerLock"_utf16_fly_string, request_pointer_lock, 0, default_attributes);

    define_native_function(realm, "computedStyleMap"_utf16_fly_string, computed_style_map, 0, default_attributes);

    define_native_function(realm, "requestFullscreen"_utf16_fly_string, request_fullscreen, 0, default_attributes);

    MUST(unscopable_object->create_data_property("before"_utf16_fly_string, JS::Value(true)));

    define_native_function(realm, "before"_utf16_fly_string, before, 0, default_attributes);

    MUST(unscopable_object->create_data_property("after"_utf16_fly_string, JS::Value(true)));

    define_native_function(realm, "after"_utf16_fly_string, after, 0, default_attributes);

    MUST(unscopable_object->create_data_property("replaceWith"_utf16_fly_string, JS::Value(true)));

    define_native_function(realm, "replaceWith"_utf16_fly_string, replace_with, 0, default_attributes);

    MUST(unscopable_object->create_data_property("remove"_utf16_fly_string, JS::Value(true)));

    define_native_function(realm, "remove"_utf16_fly_string, remove, 0, default_attributes);

    define_native_function(realm, "animate"_utf16_fly_string, animate, 1, default_attributes);

    define_native_function(realm, "getAnimations"_utf16_fly_string, get_animations, 0, default_attributes);

    MUST(unscopable_object->create_data_property("prepend"_utf16_fly_string, JS::Value(true)));

    define_native_function(realm, "prepend"_utf16_fly_string, prepend, 0, default_attributes);

    MUST(unscopable_object->create_data_property("append"_utf16_fly_string, JS::Value(true)));

    define_native_function(realm, "append"_utf16_fly_string, append, 0, default_attributes);

    MUST(unscopable_object->create_data_property("replaceChildren"_utf16_fly_string, JS::Value(true)));

    define_native_function(realm, "replaceChildren"_utf16_fly_string, replace_children, 0, default_attributes);

    define_native_function(realm, "moveBefore"_utf16_fly_string, move_before, 2, default_attributes);

    define_native_function(realm, "querySelector"_utf16_fly_string, query_selector, 1, default_attributes);

    define_native_function(realm, "querySelectorAll"_utf16_fly_string, query_selector_all, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_unscopables(), unscopable_object, JS::Attribute::Configurable);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Element"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void ElementPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    auto unscopable_object = JS::Object::create(realm, nullptr);

    object.define_direct_property(vm.well_known_symbol_unscopables(), unscopable_object, JS::Attribute::Configurable);

}

[[maybe_unused]] static JS::ThrowCompletionOr<DOM::Element*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<DOM::Element>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Element");
    return static_cast<DOM::Element*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::namespace_uri_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::namespace_uri_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->namespace_uri(); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::prefix_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::prefix_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->prefix(); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::local_name_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::local_name_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->local_name(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::tag_name_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::tag_name_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tag_name(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::id_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::id_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto contentAttributeValue = impl->attribute("id"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::id_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::id_setter");
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

    impl->set_attribute_value("id"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::class_name_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::class_name_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto contentAttributeValue = impl->attribute("class"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::class_name_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::class_name_setter");
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

    impl->set_attribute_value("class"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::class_list_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::class_list_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->class_list(); }));

    return &const_cast<DOMTokenList&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::class_list_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::class_list_setter");
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

    // 1. Let Q be ? Get(jsValue, id).
    auto receiver_value = TRY(impl->get("classList"_utf16_fly_string));

    // 2. If Q is not an Object, then throw a TypeError.
    if (!receiver_value.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, receiver_value);
    auto& receiver = receiver_value.as_object();

    // 3. Let forwardId be the identifier argument of the [PutForwards] extended attribute.
    auto forward_id = "value"_utf16_fly_string;

    // 4. Perform ? Set(Q, forwardId, V, false).
    TRY(receiver.set(JS::PropertyKey { forward_id, JS::PropertyKey::StringMayBeNumber::No }, value, JS::Object::ShouldThrowExceptions::No));

    // 5. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::slot_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::slot_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto contentAttributeValue = impl->attribute("slot"_fly_string);

    auto retval = contentAttributeValue.value_or(String {});

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::slot_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::slot_setter");
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

    impl->set_attribute_value("slot"_fly_string, cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::attributes_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::attributes_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->attributes(); }));

    return &const_cast<NamedNodeMap&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::shadow_root_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::shadow_root_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->shadow_root_for_bindings(); }));

    if (retval) {

    return &const_cast<ShadowRoot&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::next_element_sibling_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::next_element_sibling_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->next_element_sibling(); }));

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::previous_element_sibling_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::previous_element_sibling_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->previous_element_sibling(); }));

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::scroll_top_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::scroll_top_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll_top(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::scroll_top_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::scroll_top_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_scroll_top(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::scroll_left_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::scroll_left_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll_left(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::scroll_left_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::scroll_left_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_scroll_left(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::scroll_width_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::scroll_width_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll_width(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::scroll_height_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::scroll_height_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll_height(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::client_top_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::client_top_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->client_top(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::client_left_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::client_left_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->client_left(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::client_width_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::client_width_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->client_width(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::client_height_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::client_height_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->client_height(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::current_css_zoom_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::current_css_zoom_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->current_css_zoom(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::inner_html_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::inner_html_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->inner_html(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    auto retval = retval_or_exception.release_value();

    return retval.visit(

        [&vm, &realm]([[maybe_unused]] GC::Root<TrustedHTML> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<TrustedHTML&>(*visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] Utf16String const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::PrimitiveString::create(vm, visited_union_value0);

        }

    );

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::inner_html_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::inner_html_setter");
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

    auto value_to_variant = [&vm, &realm](JS::Value value) -> JS::ThrowCompletionOr<Variant<GC::Root<TrustedHTML>, Utf16String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (value.is_object()) {
            [[maybe_unused]] auto& value_object = value.as_object();

            if (is<PlatformObject>(value_object)) {

                if (auto* value_result = as_if<TrustedHTML>(value_object))
                    return GC::make_root(*value_result);

            }

        }

    Utf16String value_string;
    if (!true || !value.is_null()) {
        value_string = TRY(WebIDL::to_utf16_string(vm, value));
    }

        return { value_string };

    };

    Variant<GC::Root<TrustedHTML>, Utf16String> cpp_value = TRY(value_to_variant(value));

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_inner_html(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::outer_html_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::outer_html_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->outer_html(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    auto retval = retval_or_exception.release_value();

    return retval.visit(

        [&vm, &realm]([[maybe_unused]] GC::Root<TrustedHTML> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<TrustedHTML&>(*visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] Utf16String const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::PrimitiveString::create(vm, visited_union_value0);

        }

    );

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::outer_html_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::outer_html_setter");
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

    auto value_to_variant = [&vm, &realm](JS::Value value) -> JS::ThrowCompletionOr<Variant<GC::Root<TrustedHTML>, Utf16String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (value.is_object()) {
            [[maybe_unused]] auto& value_object = value.as_object();

            if (is<PlatformObject>(value_object)) {

                if (auto* value_result = as_if<TrustedHTML>(value_object))
                    return GC::make_root(*value_result);

            }

        }

    Utf16String value_string;
    if (!true || !value.is_null()) {
        value_string = TRY(WebIDL::to_utf16_string(vm, value));
    }

        return { value_string };

    };

    Variant<GC::Root<TrustedHTML>, Utf16String> cpp_value = TRY(value_to_variant(value));

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_outer_html(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::part_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::part_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->part_list(); }));

    return &const_cast<DOMTokenList&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::part_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::part_setter");
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

    // 1. Let Q be ? Get(jsValue, id).
    auto receiver_value = TRY(impl->get("part"_utf16_fly_string));

    // 2. If Q is not an Object, then throw a TypeError.
    if (!receiver_value.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, receiver_value);
    auto& receiver = receiver_value.as_object();

    // 3. Let forwardId be the identifier argument of the [PutForwards] extended attribute.
    auto forward_id = "value"_utf16_fly_string;

    // 4. Perform ? Set(Q, forwardId, V, false).
    TRY(receiver.set(JS::PropertyKey { forward_id, JS::PropertyKey::StringMayBeNumber::No }, value, JS::Object::ShouldThrowExceptions::No));

    // 5. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::onfullscreenchange_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::onfullscreenchange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onfullscreenchange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::onfullscreenchange_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::onfullscreenchange_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onfullscreenchange(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::onfullscreenerror_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::onfullscreenerror_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onfullscreenerror(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::onfullscreenerror_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::onfullscreenerror_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onfullscreenerror(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::assigned_slot_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::assigned_slot_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->assigned_slot(); }));

    if (retval) {

    return &const_cast<HTMLSlotElement&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::role_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::role_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->role(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::role_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::role_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_role(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_active_descendant_element_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_active_descendant_element_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    static auto content_attribute = "aria-activedescendant"_fly_string;

    auto retval = impl->get_the_attribute_associated_element(content_attribute, TRY(throw_dom_exception_if_needed(vm, [&] { return impl->aria_active_descendant_element(); })));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_active_descendant_element_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_active_descendant_element_setter");
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

    GC::Ptr<Element> cpp_value;

    if (!value.is_nullish()) {
        if (!value.is_object() || !is<Element>(value.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Element");

        cpp_value = &static_cast<Element&>(value.as_object());
    }

    static auto content_attribute = "aria-activedescendant"_fly_string;

    if (!cpp_value) {
        impl->set_aria_active_descendant_element({});
        impl->remove_attribute(content_attribute);
        return JS::js_undefined();
    }

    impl->set_attribute_value(content_attribute, String {});

    impl->set_aria_active_descendant_element(*cpp_value);

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_atomic_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_atomic_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_atomic(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_atomic_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_atomic_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_atomic(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_auto_complete_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_auto_complete_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_auto_complete(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_auto_complete_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_auto_complete_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_auto_complete(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_braille_label_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_braille_label_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_braille_label(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_braille_label_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_braille_label_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_braille_label(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_braille_role_description_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_braille_role_description_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_braille_role_description(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_braille_role_description_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_braille_role_description_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_braille_role_description(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_busy_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_busy_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_busy(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_busy_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_busy_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_busy(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_checked_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_checked_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_checked(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_checked_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_checked_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_checked(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_col_count_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_col_count_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_col_count(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_col_count_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_col_count_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_col_count(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_col_index_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_col_index_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_col_index(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_col_index_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_col_index_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_col_index(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_col_index_text_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_col_index_text_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_col_index_text(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_col_index_text_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_col_index_text_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_col_index_text(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_col_span_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_col_span_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_col_span(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_col_span_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_col_span_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_col_span(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_controls_elements_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_controls_elements_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    static auto content_attribute = "aria-controls"_fly_string;

    auto retval = impl->get_the_attribute_associated_elements(content_attribute, TRY(throw_dom_exception_if_needed(vm, [&] { return impl->aria_controls_elements(); })));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    auto cached_aria_controls_elements = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cached_aria_controls_elements(); }));
    if (WebIDL::lists_contain_same_elements(cached_aria_controls_elements, retval))
        return cached_aria_controls_elements;

    auto result = TRY([&]() -> JS::ThrowCompletionOr<JS::Value> {

    if (retval.has_value()) {

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    auto& retval_non_optional = retval.value();
    for (size_t i0 = 0; i0 < retval_non_optional.size(); ++i0) {
        auto& element0 = retval_non_optional.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    TRY(new_array0_0->set_integrity_level(IntegrityLevel::Frozen));

    return new_array0_0;

    } else {
        return JS::js_null();
    }

    }());

    if (result.is_null()) {
        TRY(throw_dom_exception_if_needed(vm, [&] { impl->set_cached_aria_controls_elements({}); }));
    } else {
        auto& array = as<JS::Array>(result.as_object());
        TRY(throw_dom_exception_if_needed(vm, [&] { impl->set_cached_aria_controls_elements(&array); }));
    }

    return result;

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_controls_elements_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_controls_elements_setter");
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

    Optional<GC::RootVector<GC::Root<Element>>> cpp_value;

    if (!value.is_nullish()) {

    if (!value.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, value);

    auto value_iterator_method0 = TRY(value.get_method(vm, vm.well_known_symbol_iterator()));
    if (!value_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, value);

    auto value_iterator1 = TRY(JS::get_iterator_from_method(vm, value, *value_iterator_method0));

    GC::RootVector<GC::Root<Element>> cpp_value_non_optional { vm.heap() };

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, value_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object() || !is<Element>(next_item1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Element");

    auto& sequence_item1 = static_cast<Element&>(next_item1.as_object());

    cpp_value_non_optional.append(sequence_item1);
    }

        cpp_value = move(cpp_value_non_optional);
    }

    static auto content_attribute = "aria-controls"_fly_string;

    if (!cpp_value.has_value()) {
        impl->set_aria_controls_elements({});
        impl->remove_attribute(content_attribute);
        return JS::js_undefined();
    }

    impl->set_attribute_value(content_attribute, String {});

    Vector<GC::Weak<DOM::Element>> elements;
    elements.ensure_capacity(cpp_value->size());

    for (auto const& element : *cpp_value) {
        elements.unchecked_append(*element);
    }

    impl->set_aria_controls_elements(move(elements));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_current_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_current_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_current(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_current_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_current_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_current(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_described_by_elements_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_described_by_elements_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    static auto content_attribute = "aria-describedby"_fly_string;

    auto retval = impl->get_the_attribute_associated_elements(content_attribute, TRY(throw_dom_exception_if_needed(vm, [&] { return impl->aria_described_by_elements(); })));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    auto cached_aria_described_by_elements = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cached_aria_described_by_elements(); }));
    if (WebIDL::lists_contain_same_elements(cached_aria_described_by_elements, retval))
        return cached_aria_described_by_elements;

    auto result = TRY([&]() -> JS::ThrowCompletionOr<JS::Value> {

    if (retval.has_value()) {

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    auto& retval_non_optional = retval.value();
    for (size_t i0 = 0; i0 < retval_non_optional.size(); ++i0) {
        auto& element0 = retval_non_optional.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    TRY(new_array0_0->set_integrity_level(IntegrityLevel::Frozen));

    return new_array0_0;

    } else {
        return JS::js_null();
    }

    }());

    if (result.is_null()) {
        TRY(throw_dom_exception_if_needed(vm, [&] { impl->set_cached_aria_described_by_elements({}); }));
    } else {
        auto& array = as<JS::Array>(result.as_object());
        TRY(throw_dom_exception_if_needed(vm, [&] { impl->set_cached_aria_described_by_elements(&array); }));
    }

    return result;

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_described_by_elements_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_described_by_elements_setter");
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

    Optional<GC::RootVector<GC::Root<Element>>> cpp_value;

    if (!value.is_nullish()) {

    if (!value.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, value);

    auto value_iterator_method0 = TRY(value.get_method(vm, vm.well_known_symbol_iterator()));
    if (!value_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, value);

    auto value_iterator1 = TRY(JS::get_iterator_from_method(vm, value, *value_iterator_method0));

    GC::RootVector<GC::Root<Element>> cpp_value_non_optional { vm.heap() };

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, value_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object() || !is<Element>(next_item1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Element");

    auto& sequence_item1 = static_cast<Element&>(next_item1.as_object());

    cpp_value_non_optional.append(sequence_item1);
    }

        cpp_value = move(cpp_value_non_optional);
    }

    static auto content_attribute = "aria-describedby"_fly_string;

    if (!cpp_value.has_value()) {
        impl->set_aria_described_by_elements({});
        impl->remove_attribute(content_attribute);
        return JS::js_undefined();
    }

    impl->set_attribute_value(content_attribute, String {});

    Vector<GC::Weak<DOM::Element>> elements;
    elements.ensure_capacity(cpp_value->size());

    for (auto const& element : *cpp_value) {
        elements.unchecked_append(*element);
    }

    impl->set_aria_described_by_elements(move(elements));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_description_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_description_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_description(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_description_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_description_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_description(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_details_elements_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_details_elements_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    static auto content_attribute = "aria-details"_fly_string;

    auto retval = impl->get_the_attribute_associated_elements(content_attribute, TRY(throw_dom_exception_if_needed(vm, [&] { return impl->aria_details_elements(); })));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    auto cached_aria_details_elements = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cached_aria_details_elements(); }));
    if (WebIDL::lists_contain_same_elements(cached_aria_details_elements, retval))
        return cached_aria_details_elements;

    auto result = TRY([&]() -> JS::ThrowCompletionOr<JS::Value> {

    if (retval.has_value()) {

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    auto& retval_non_optional = retval.value();
    for (size_t i0 = 0; i0 < retval_non_optional.size(); ++i0) {
        auto& element0 = retval_non_optional.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    TRY(new_array0_0->set_integrity_level(IntegrityLevel::Frozen));

    return new_array0_0;

    } else {
        return JS::js_null();
    }

    }());

    if (result.is_null()) {
        TRY(throw_dom_exception_if_needed(vm, [&] { impl->set_cached_aria_details_elements({}); }));
    } else {
        auto& array = as<JS::Array>(result.as_object());
        TRY(throw_dom_exception_if_needed(vm, [&] { impl->set_cached_aria_details_elements(&array); }));
    }

    return result;

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_details_elements_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_details_elements_setter");
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

    Optional<GC::RootVector<GC::Root<Element>>> cpp_value;

    if (!value.is_nullish()) {

    if (!value.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, value);

    auto value_iterator_method0 = TRY(value.get_method(vm, vm.well_known_symbol_iterator()));
    if (!value_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, value);

    auto value_iterator1 = TRY(JS::get_iterator_from_method(vm, value, *value_iterator_method0));

    GC::RootVector<GC::Root<Element>> cpp_value_non_optional { vm.heap() };

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, value_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object() || !is<Element>(next_item1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Element");

    auto& sequence_item1 = static_cast<Element&>(next_item1.as_object());

    cpp_value_non_optional.append(sequence_item1);
    }

        cpp_value = move(cpp_value_non_optional);
    }

    static auto content_attribute = "aria-details"_fly_string;

    if (!cpp_value.has_value()) {
        impl->set_aria_details_elements({});
        impl->remove_attribute(content_attribute);
        return JS::js_undefined();
    }

    impl->set_attribute_value(content_attribute, String {});

    Vector<GC::Weak<DOM::Element>> elements;
    elements.ensure_capacity(cpp_value->size());

    for (auto const& element : *cpp_value) {
        elements.unchecked_append(*element);
    }

    impl->set_aria_details_elements(move(elements));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_disabled_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_disabled_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_disabled(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_disabled_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_disabled_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_disabled(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_error_message_elements_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_error_message_elements_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    static auto content_attribute = "aria-errormessage"_fly_string;

    auto retval = impl->get_the_attribute_associated_elements(content_attribute, TRY(throw_dom_exception_if_needed(vm, [&] { return impl->aria_error_message_elements(); })));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    auto cached_aria_error_message_elements = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cached_aria_error_message_elements(); }));
    if (WebIDL::lists_contain_same_elements(cached_aria_error_message_elements, retval))
        return cached_aria_error_message_elements;

    auto result = TRY([&]() -> JS::ThrowCompletionOr<JS::Value> {

    if (retval.has_value()) {

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    auto& retval_non_optional = retval.value();
    for (size_t i0 = 0; i0 < retval_non_optional.size(); ++i0) {
        auto& element0 = retval_non_optional.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    TRY(new_array0_0->set_integrity_level(IntegrityLevel::Frozen));

    return new_array0_0;

    } else {
        return JS::js_null();
    }

    }());

    if (result.is_null()) {
        TRY(throw_dom_exception_if_needed(vm, [&] { impl->set_cached_aria_error_message_elements({}); }));
    } else {
        auto& array = as<JS::Array>(result.as_object());
        TRY(throw_dom_exception_if_needed(vm, [&] { impl->set_cached_aria_error_message_elements(&array); }));
    }

    return result;

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_error_message_elements_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_error_message_elements_setter");
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

    Optional<GC::RootVector<GC::Root<Element>>> cpp_value;

    if (!value.is_nullish()) {

    if (!value.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, value);

    auto value_iterator_method0 = TRY(value.get_method(vm, vm.well_known_symbol_iterator()));
    if (!value_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, value);

    auto value_iterator1 = TRY(JS::get_iterator_from_method(vm, value, *value_iterator_method0));

    GC::RootVector<GC::Root<Element>> cpp_value_non_optional { vm.heap() };

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, value_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object() || !is<Element>(next_item1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Element");

    auto& sequence_item1 = static_cast<Element&>(next_item1.as_object());

    cpp_value_non_optional.append(sequence_item1);
    }

        cpp_value = move(cpp_value_non_optional);
    }

    static auto content_attribute = "aria-errormessage"_fly_string;

    if (!cpp_value.has_value()) {
        impl->set_aria_error_message_elements({});
        impl->remove_attribute(content_attribute);
        return JS::js_undefined();
    }

    impl->set_attribute_value(content_attribute, String {});

    Vector<GC::Weak<DOM::Element>> elements;
    elements.ensure_capacity(cpp_value->size());

    for (auto const& element : *cpp_value) {
        elements.unchecked_append(*element);
    }

    impl->set_aria_error_message_elements(move(elements));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_expanded_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_expanded_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_expanded(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_expanded_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_expanded_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_expanded(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_flow_to_elements_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_flow_to_elements_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    static auto content_attribute = "aria-flowto"_fly_string;

    auto retval = impl->get_the_attribute_associated_elements(content_attribute, TRY(throw_dom_exception_if_needed(vm, [&] { return impl->aria_flow_to_elements(); })));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    auto cached_aria_flow_to_elements = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cached_aria_flow_to_elements(); }));
    if (WebIDL::lists_contain_same_elements(cached_aria_flow_to_elements, retval))
        return cached_aria_flow_to_elements;

    auto result = TRY([&]() -> JS::ThrowCompletionOr<JS::Value> {

    if (retval.has_value()) {

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    auto& retval_non_optional = retval.value();
    for (size_t i0 = 0; i0 < retval_non_optional.size(); ++i0) {
        auto& element0 = retval_non_optional.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    TRY(new_array0_0->set_integrity_level(IntegrityLevel::Frozen));

    return new_array0_0;

    } else {
        return JS::js_null();
    }

    }());

    if (result.is_null()) {
        TRY(throw_dom_exception_if_needed(vm, [&] { impl->set_cached_aria_flow_to_elements({}); }));
    } else {
        auto& array = as<JS::Array>(result.as_object());
        TRY(throw_dom_exception_if_needed(vm, [&] { impl->set_cached_aria_flow_to_elements(&array); }));
    }

    return result;

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_flow_to_elements_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_flow_to_elements_setter");
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

    Optional<GC::RootVector<GC::Root<Element>>> cpp_value;

    if (!value.is_nullish()) {

    if (!value.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, value);

    auto value_iterator_method0 = TRY(value.get_method(vm, vm.well_known_symbol_iterator()));
    if (!value_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, value);

    auto value_iterator1 = TRY(JS::get_iterator_from_method(vm, value, *value_iterator_method0));

    GC::RootVector<GC::Root<Element>> cpp_value_non_optional { vm.heap() };

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, value_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object() || !is<Element>(next_item1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Element");

    auto& sequence_item1 = static_cast<Element&>(next_item1.as_object());

    cpp_value_non_optional.append(sequence_item1);
    }

        cpp_value = move(cpp_value_non_optional);
    }

    static auto content_attribute = "aria-flowto"_fly_string;

    if (!cpp_value.has_value()) {
        impl->set_aria_flow_to_elements({});
        impl->remove_attribute(content_attribute);
        return JS::js_undefined();
    }

    impl->set_attribute_value(content_attribute, String {});

    Vector<GC::Weak<DOM::Element>> elements;
    elements.ensure_capacity(cpp_value->size());

    for (auto const& element : *cpp_value) {
        elements.unchecked_append(*element);
    }

    impl->set_aria_flow_to_elements(move(elements));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_has_popup_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_has_popup_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_has_popup(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_has_popup_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_has_popup_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_has_popup(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_hidden_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_hidden_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_hidden(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_hidden_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_hidden_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_hidden(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_invalid_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_invalid_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_invalid(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_invalid_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_invalid_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_invalid(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_key_shortcuts_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_key_shortcuts_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_key_shortcuts(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_key_shortcuts_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_key_shortcuts_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_key_shortcuts(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_label_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_label_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_label(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_label_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_label_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_label(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_labelled_by_elements_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_labelled_by_elements_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    static auto content_attribute = "aria-labelledby"_fly_string;

    auto retval = impl->get_the_attribute_associated_elements(content_attribute, TRY(throw_dom_exception_if_needed(vm, [&] { return impl->aria_labelled_by_elements(); })));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    auto cached_aria_labelled_by_elements = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cached_aria_labelled_by_elements(); }));
    if (WebIDL::lists_contain_same_elements(cached_aria_labelled_by_elements, retval))
        return cached_aria_labelled_by_elements;

    auto result = TRY([&]() -> JS::ThrowCompletionOr<JS::Value> {

    if (retval.has_value()) {

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    auto& retval_non_optional = retval.value();
    for (size_t i0 = 0; i0 < retval_non_optional.size(); ++i0) {
        auto& element0 = retval_non_optional.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    TRY(new_array0_0->set_integrity_level(IntegrityLevel::Frozen));

    return new_array0_0;

    } else {
        return JS::js_null();
    }

    }());

    if (result.is_null()) {
        TRY(throw_dom_exception_if_needed(vm, [&] { impl->set_cached_aria_labelled_by_elements({}); }));
    } else {
        auto& array = as<JS::Array>(result.as_object());
        TRY(throw_dom_exception_if_needed(vm, [&] { impl->set_cached_aria_labelled_by_elements(&array); }));
    }

    return result;

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_labelled_by_elements_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_labelled_by_elements_setter");
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

    Optional<GC::RootVector<GC::Root<Element>>> cpp_value;

    if (!value.is_nullish()) {

    if (!value.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, value);

    auto value_iterator_method0 = TRY(value.get_method(vm, vm.well_known_symbol_iterator()));
    if (!value_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, value);

    auto value_iterator1 = TRY(JS::get_iterator_from_method(vm, value, *value_iterator_method0));

    GC::RootVector<GC::Root<Element>> cpp_value_non_optional { vm.heap() };

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, value_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object() || !is<Element>(next_item1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Element");

    auto& sequence_item1 = static_cast<Element&>(next_item1.as_object());

    cpp_value_non_optional.append(sequence_item1);
    }

        cpp_value = move(cpp_value_non_optional);
    }

    static auto content_attribute = "aria-labelledby"_fly_string;

    if (!cpp_value.has_value()) {
        impl->set_aria_labelled_by_elements({});
        impl->remove_attribute(content_attribute);
        return JS::js_undefined();
    }

    impl->set_attribute_value(content_attribute, String {});

    Vector<GC::Weak<DOM::Element>> elements;
    elements.ensure_capacity(cpp_value->size());

    for (auto const& element : *cpp_value) {
        elements.unchecked_append(*element);
    }

    impl->set_aria_labelled_by_elements(move(elements));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_level_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_level_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_level(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_level_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_level_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_level(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_live_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_live_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_live(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_live_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_live_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_live(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_modal_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_modal_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_modal(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_modal_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_modal_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_modal(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_multi_line_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_multi_line_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_multi_line(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_multi_line_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_multi_line_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_multi_line(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_multi_selectable_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_multi_selectable_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_multi_selectable(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_multi_selectable_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_multi_selectable_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_multi_selectable(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_orientation_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_orientation_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_orientation(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_orientation_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_orientation_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_orientation(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_owns_elements_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_owns_elements_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    static auto content_attribute = "aria-owns"_fly_string;

    auto retval = impl->get_the_attribute_associated_elements(content_attribute, TRY(throw_dom_exception_if_needed(vm, [&] { return impl->aria_owns_elements(); })));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    auto cached_aria_owns_elements = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cached_aria_owns_elements(); }));
    if (WebIDL::lists_contain_same_elements(cached_aria_owns_elements, retval))
        return cached_aria_owns_elements;

    auto result = TRY([&]() -> JS::ThrowCompletionOr<JS::Value> {

    if (retval.has_value()) {

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    auto& retval_non_optional = retval.value();
    for (size_t i0 = 0; i0 < retval_non_optional.size(); ++i0) {
        auto& element0 = retval_non_optional.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    TRY(new_array0_0->set_integrity_level(IntegrityLevel::Frozen));

    return new_array0_0;

    } else {
        return JS::js_null();
    }

    }());

    if (result.is_null()) {
        TRY(throw_dom_exception_if_needed(vm, [&] { impl->set_cached_aria_owns_elements({}); }));
    } else {
        auto& array = as<JS::Array>(result.as_object());
        TRY(throw_dom_exception_if_needed(vm, [&] { impl->set_cached_aria_owns_elements(&array); }));
    }

    return result;

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_owns_elements_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_owns_elements_setter");
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

    Optional<GC::RootVector<GC::Root<Element>>> cpp_value;

    if (!value.is_nullish()) {

    if (!value.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, value);

    auto value_iterator_method0 = TRY(value.get_method(vm, vm.well_known_symbol_iterator()));
    if (!value_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, value);

    auto value_iterator1 = TRY(JS::get_iterator_from_method(vm, value, *value_iterator_method0));

    GC::RootVector<GC::Root<Element>> cpp_value_non_optional { vm.heap() };

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, value_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object() || !is<Element>(next_item1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Element");

    auto& sequence_item1 = static_cast<Element&>(next_item1.as_object());

    cpp_value_non_optional.append(sequence_item1);
    }

        cpp_value = move(cpp_value_non_optional);
    }

    static auto content_attribute = "aria-owns"_fly_string;

    if (!cpp_value.has_value()) {
        impl->set_aria_owns_elements({});
        impl->remove_attribute(content_attribute);
        return JS::js_undefined();
    }

    impl->set_attribute_value(content_attribute, String {});

    Vector<GC::Weak<DOM::Element>> elements;
    elements.ensure_capacity(cpp_value->size());

    for (auto const& element : *cpp_value) {
        elements.unchecked_append(*element);
    }

    impl->set_aria_owns_elements(move(elements));

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_placeholder_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_placeholder_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_placeholder(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_placeholder_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_placeholder_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_placeholder(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_pos_in_set_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_pos_in_set_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_pos_in_set(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_pos_in_set_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_pos_in_set_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_pos_in_set(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_pressed_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_pressed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_pressed(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_pressed_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_pressed_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_pressed(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_read_only_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_read_only_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_read_only(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_read_only_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_read_only_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_read_only(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_relevant_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_relevant_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_relevant(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_relevant_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_relevant_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_relevant(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_required_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_required_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_required(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_required_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_required_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_required(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_role_description_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_role_description_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_role_description(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_role_description_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_role_description_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_role_description(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_row_count_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_row_count_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_row_count(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_row_count_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_row_count_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_row_count(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_row_index_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_row_index_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_row_index(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_row_index_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_row_index_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_row_index(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_row_index_text_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_row_index_text_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_row_index_text(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_row_index_text_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_row_index_text_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_row_index_text(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_row_span_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_row_span_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_row_span(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_row_span_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_row_span_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_row_span(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_selected_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_selected_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_selected(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_selected_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_selected_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_selected(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_set_size_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_set_size_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_set_size(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_set_size_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_set_size_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_set_size(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_sort_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_sort_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_sort(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_sort_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_sort_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_sort(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_value_max_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_value_max_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_value_max(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_value_max_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_value_max_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_value_max(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_value_min_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_value_min_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_value_min(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_value_min_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_value_min_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_value_min(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_value_now_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_value_now_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_value_now(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_value_now_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_value_now_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_value_now(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_value_text_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_value_text_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->aria_value_text(); });

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::aria_value_text_setter)
{
    WebIDL::log_trace(vm, "ElementPrototype::aria_value_text_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_aria_value_text(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::children_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::children_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->children(); }));

    return &const_cast<HTMLCollection&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::first_element_child_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::first_element_child_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->first_element_child(); }));

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::last_element_child_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::last_element_child_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->last_element_child(); }));

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::child_element_count_getter)
{
    WebIDL::log_trace(vm, "ElementPrototype::child_element_count_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->child_element_count(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::has_attributes)
{
    WebIDL::log_trace(vm, "ElementPrototype::has_attributes");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->has_attributes(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::get_attribute_names)
{
    WebIDL::log_trace(vm, "ElementPrototype::get_attribute_names");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_attribute_names(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);
JS::Value wrapped_element0;

    wrapped_element0 = JS::PrimitiveString::create(vm, element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::get_attribute)
{
    WebIDL::log_trace(vm, "ElementPrototype::get_attribute");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getAttribute");

    auto arg0 = vm.argument(0);

    String qualified_name;
    if (!false || !arg0.is_null()) {
        qualified_name = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_attribute(qualified_name); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::get_attribute_ns)
{
    WebIDL::log_trace(vm, "ElementPrototype::get_attribute_ns");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getAttributeNS", "2");

    auto arg0 = vm.argument(0);

    Optional<FlyString> namespace_;
    if (!arg0.is_nullish())
        namespace_ = TRY(WebIDL::to_string(vm, arg0));

    auto arg1 = vm.argument(1);

    FlyString local_name;
    if (!false || !arg1.is_null()) {
        local_name = TRY(WebIDL::to_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_attribute_ns(namespace_, local_name); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::set_attribute)
{
    WebIDL::log_trace(vm, "ElementPrototype::set_attribute");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "setAttribute", "2");

    auto arg0 = vm.argument(0);

    String qualified_name;
    if (!false || !arg0.is_null()) {
        qualified_name = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<TrustedHTML>, GC::Root<TrustedScript>, GC::Root<TrustedScriptURL>, Utf16String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (is<PlatformObject>(arg1_object)) {

                if (auto* arg1_result = as_if<TrustedHTML>(arg1_object))
                    return GC::make_root(*arg1_result);

                if (auto* arg1_result = as_if<TrustedScript>(arg1_object))
                    return GC::make_root(*arg1_result);

                if (auto* arg1_result = as_if<TrustedScriptURL>(arg1_object))
                    return GC::make_root(*arg1_result);

            }

        }

    Utf16String arg1_string;
    if (!false || !arg1.is_null()) {
        arg1_string = TRY(WebIDL::to_utf16_string(vm, arg1));
    }

        return { arg1_string };

    };

    Variant<GC::Root<TrustedHTML>, GC::Root<TrustedScript>, GC::Root<TrustedScriptURL>, Utf16String> value = TRY(arg1_to_variant(arg1));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_attribute_for_bindings(qualified_name, value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::set_attribute_ns)
{
    WebIDL::log_trace(vm, "ElementPrototype::set_attribute_ns");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "setAttributeNS", "3");

    auto arg0 = vm.argument(0);

    Optional<FlyString> namespace_;
    if (!arg0.is_nullish())
        namespace_ = TRY(WebIDL::to_string(vm, arg0));

    auto arg1 = vm.argument(1);

    FlyString qualified_name;
    if (!false || !arg1.is_null()) {
        qualified_name = TRY(WebIDL::to_string(vm, arg1));
    }

    auto arg2 = vm.argument(2);

    auto arg2_to_variant = [&vm, &realm](JS::Value arg2) -> JS::ThrowCompletionOr<Variant<GC::Root<TrustedHTML>, GC::Root<TrustedScript>, GC::Root<TrustedScriptURL>, Utf16String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg2.is_object()) {
            [[maybe_unused]] auto& arg2_object = arg2.as_object();

            if (is<PlatformObject>(arg2_object)) {

                if (auto* arg2_result = as_if<TrustedHTML>(arg2_object))
                    return GC::make_root(*arg2_result);

                if (auto* arg2_result = as_if<TrustedScript>(arg2_object))
                    return GC::make_root(*arg2_result);

                if (auto* arg2_result = as_if<TrustedScriptURL>(arg2_object))
                    return GC::make_root(*arg2_result);

            }

        }

    Utf16String arg2_string;
    if (!false || !arg2.is_null()) {
        arg2_string = TRY(WebIDL::to_utf16_string(vm, arg2));
    }

        return { arg2_string };

    };

    Variant<GC::Root<TrustedHTML>, GC::Root<TrustedScript>, GC::Root<TrustedScriptURL>, Utf16String> value = TRY(arg2_to_variant(arg2));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_attribute_ns_for_bindings(namespace_, qualified_name, value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::remove_attribute)
{
    WebIDL::log_trace(vm, "ElementPrototype::remove_attribute");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "removeAttribute");

    auto arg0 = vm.argument(0);

    FlyString qualified_name;
    if (!false || !arg0.is_null()) {
        qualified_name = TRY(WebIDL::to_string(vm, arg0));
    }

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->remove_attribute(qualified_name); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::remove_attribute_ns)
{
    WebIDL::log_trace(vm, "ElementPrototype::remove_attribute_ns");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "removeAttributeNS", "2");

    auto arg0 = vm.argument(0);

    Optional<FlyString> namespace_;
    if (!arg0.is_nullish())
        namespace_ = TRY(WebIDL::to_string(vm, arg0));

    auto arg1 = vm.argument(1);

    FlyString local_name;
    if (!false || !arg1.is_null()) {
        local_name = TRY(WebIDL::to_string(vm, arg1));
    }

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->remove_attribute_ns(namespace_, local_name); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::toggle_attribute)
{
    WebIDL::log_trace(vm, "ElementPrototype::toggle_attribute");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "toggleAttribute");

    auto arg0 = vm.argument(0);

    String qualified_name;
    if (!false || !arg0.is_null()) {
        qualified_name = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    Optional<bool> force;

    if (!arg1.is_undefined())

    force = arg1.to_boolean();

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->toggle_attribute(qualified_name, force); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::has_attribute)
{
    WebIDL::log_trace(vm, "ElementPrototype::has_attribute");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "hasAttribute");

    auto arg0 = vm.argument(0);

    String qualified_name;
    if (!false || !arg0.is_null()) {
        qualified_name = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->has_attribute(qualified_name); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::has_attribute_ns)
{
    WebIDL::log_trace(vm, "ElementPrototype::has_attribute_ns");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "hasAttributeNS", "2");

    auto arg0 = vm.argument(0);

    Optional<FlyString> namespace_;
    if (!arg0.is_nullish())
        namespace_ = TRY(WebIDL::to_string(vm, arg0));

    auto arg1 = vm.argument(1);

    FlyString local_name;
    if (!false || !arg1.is_null()) {
        local_name = TRY(WebIDL::to_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->has_attribute_ns(namespace_, local_name); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::get_attribute_node)
{
    WebIDL::log_trace(vm, "ElementPrototype::get_attribute_node");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getAttributeNode");

    auto arg0 = vm.argument(0);

    FlyString qualified_name;
    if (!false || !arg0.is_null()) {
        qualified_name = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_attribute_node(qualified_name); }));

    if (retval) {

    return &const_cast<Attr&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::get_attribute_node_ns)
{
    WebIDL::log_trace(vm, "ElementPrototype::get_attribute_node_ns");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getAttributeNodeNS", "2");

    auto arg0 = vm.argument(0);

    Optional<FlyString> namespace_;
    if (!arg0.is_nullish())
        namespace_ = TRY(WebIDL::to_string(vm, arg0));

    auto arg1 = vm.argument(1);

    FlyString local_name;
    if (!false || !arg1.is_null()) {
        local_name = TRY(WebIDL::to_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_attribute_node_ns(namespace_, local_name); }));

    if (retval) {

    return &const_cast<Attr&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::set_attribute_node)
{
    WebIDL::log_trace(vm, "ElementPrototype::set_attribute_node");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setAttributeNode");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Attr>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Attr");

    auto& attr = static_cast<Attr&>(arg0.as_object());

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_attribute_node_for_bindings(attr); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    if (retval) {

    return &const_cast<Attr&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::set_attribute_node_ns)
{
    WebIDL::log_trace(vm, "ElementPrototype::set_attribute_node_ns");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setAttributeNodeNS");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Attr>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Attr");

    auto& attr = static_cast<Attr&>(arg0.as_object());

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_attribute_node_ns_for_bindings(attr); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    if (retval) {

    return &const_cast<Attr&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::remove_attribute_node)
{
    WebIDL::log_trace(vm, "ElementPrototype::remove_attribute_node");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "removeAttributeNode");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Attr>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Attr");

    auto& attr = static_cast<Attr&>(arg0.as_object());

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->remove_attribute_node(attr); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return &const_cast<Attr&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::attach_shadow)
{
    WebIDL::log_trace(vm, "ElementPrototype::attach_shadow");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "attachShadow");

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ShadowRootInit");

    ShadowRootInit init {};

    auto clonable_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        clonable_property_value_0 = TRY(arg0.as_object().get("clonable"_utf16_fly_string));

    bool clonable_value_0;

    if (!clonable_property_value_0.is_undefined())

    clonable_value_0 = clonable_property_value_0.to_boolean();

    else
        clonable_value_0 = static_cast<bool>(false);

    init.clonable = clonable_value_0;

    auto delegates_focus_property_value_1 = JS::js_undefined();
    if (arg0.is_object())
        delegates_focus_property_value_1 = TRY(arg0.as_object().get("delegatesFocus"_utf16_fly_string));

    bool delegates_focus_value_1;

    if (!delegates_focus_property_value_1.is_undefined())

    delegates_focus_value_1 = delegates_focus_property_value_1.to_boolean();

    else
        delegates_focus_value_1 = static_cast<bool>(false);

    init.delegates_focus = delegates_focus_value_1;

    auto mode_property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        mode_property_value_2 = TRY(arg0.as_object().get("mode"_utf16_fly_string));

    if (mode_property_value_2.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::MissingRequiredProperty, "mode");

    ShadowRootMode mode_value_2 { ShadowRootMode::Open };

    auto mode_property_value_2_string = TRY(mode_property_value_2.to_string(vm));

    if (mode_property_value_2_string == "open"sv)
        mode_value_2 = ShadowRootMode::Open;

    else if (mode_property_value_2_string == "closed"sv)
        mode_value_2 = ShadowRootMode::Closed;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, mode_property_value_2_string, "ShadowRootMode");

    init.mode = mode_value_2;

    auto serializable_property_value_3 = JS::js_undefined();
    if (arg0.is_object())
        serializable_property_value_3 = TRY(arg0.as_object().get("serializable"_utf16_fly_string));

    bool serializable_value_3;

    if (!serializable_property_value_3.is_undefined())

    serializable_value_3 = serializable_property_value_3.to_boolean();

    else
        serializable_value_3 = static_cast<bool>(false);

    init.serializable = serializable_value_3;

    auto slot_assignment_property_value_4 = JS::js_undefined();
    if (arg0.is_object())
        slot_assignment_property_value_4 = TRY(arg0.as_object().get("slotAssignment"_utf16_fly_string));

    SlotAssignmentMode slot_assignment_value_4 { SlotAssignmentMode::Named };

    if (!slot_assignment_property_value_4.is_undefined()) {

    auto slot_assignment_property_value_4_string = TRY(slot_assignment_property_value_4.to_string(vm));

    if (slot_assignment_property_value_4_string == "manual"sv)
        slot_assignment_value_4 = SlotAssignmentMode::Manual;

    else if (slot_assignment_property_value_4_string == "named"sv)
        slot_assignment_value_4 = SlotAssignmentMode::Named;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, slot_assignment_property_value_4_string, "SlotAssignmentMode");

    }

    init.slot_assignment = slot_assignment_value_4;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->attach_shadow(init); }));

    return &const_cast<ShadowRoot&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::matches)
{
    WebIDL::log_trace(vm, "ElementPrototype::matches");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "matches");

    auto arg0 = vm.argument(0);

    String selectors;
    if (!false || !arg0.is_null()) {
        selectors = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->matches(selectors); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::closest)
{
    WebIDL::log_trace(vm, "ElementPrototype::closest");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "closest");

    auto arg0 = vm.argument(0);

    String selectors;
    if (!false || !arg0.is_null()) {
        selectors = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->closest(selectors); }));

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::webkit_matches_selector)
{
    WebIDL::log_trace(vm, "ElementPrototype::webkit_matches_selector");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "webkitMatchesSelector");

    auto arg0 = vm.argument(0);

    String selectors;
    if (!false || !arg0.is_null()) {
        selectors = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->matches(selectors); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::get_elements_by_tag_name)
{
    WebIDL::log_trace(vm, "ElementPrototype::get_elements_by_tag_name");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getElementsByTagName");

    auto arg0 = vm.argument(0);

    FlyString tag_name;
    if (!false || !arg0.is_null()) {
        tag_name = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_elements_by_tag_name(tag_name); }));

    return &const_cast<HTMLCollection&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::get_elements_by_tag_name_ns)
{
    WebIDL::log_trace(vm, "ElementPrototype::get_elements_by_tag_name_ns");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getElementsByTagNameNS", "2");

    auto arg0 = vm.argument(0);

    Optional<FlyString> namespace_;
    if (!arg0.is_nullish())
        namespace_ = TRY(WebIDL::to_string(vm, arg0));

    auto arg1 = vm.argument(1);

    FlyString local_name;
    if (!false || !arg1.is_null()) {
        local_name = TRY(WebIDL::to_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_elements_by_tag_name_ns(namespace_, local_name); }));

    return &const_cast<HTMLCollection&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::get_elements_by_class_name)
{
    WebIDL::log_trace(vm, "ElementPrototype::get_elements_by_class_name");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getElementsByClassName");

    auto arg0 = vm.argument(0);

    String class_name;
    if (!false || !arg0.is_null()) {
        class_name = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_elements_by_class_name(class_name); }));

    return &const_cast<HTMLCollection&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::insert_adjacent_element)
{
    WebIDL::log_trace(vm, "ElementPrototype::insert_adjacent_element");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "insertAdjacentElement", "2");

    auto arg0 = vm.argument(0);

    String where;
    if (!false || !arg0.is_null()) {
        where = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<Element>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Element");

    auto& element = static_cast<Element&>(arg1.as_object());

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->insert_adjacent_element(where, element); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::insert_adjacent_text)
{
    WebIDL::log_trace(vm, "ElementPrototype::insert_adjacent_text");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "insertAdjacentText", "2");

    auto arg0 = vm.argument(0);

    String where;
    if (!false || !arg0.is_null()) {
        where = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    Utf16String data;
    if (!false || !arg1.is_null()) {
        data = TRY(WebIDL::to_utf16_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->insert_adjacent_text(where, data); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::get_client_rects)
{
    WebIDL::log_trace(vm, "ElementPrototype::get_client_rects");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_client_rects_for_bindings(); }));

    return &const_cast<DOMRectList&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::get_bounding_client_rect)
{
    WebIDL::log_trace(vm, "ElementPrototype::get_bounding_client_rect");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_bounding_client_rect_for_bindings(); }));

    return &const_cast<DOMRect&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::check_visibility)
{
    WebIDL::log_trace(vm, "ElementPrototype::check_visibility");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "CheckVisibilityOptions");

    CheckVisibilityOptions options {};

    auto check_opacity_property_value_5 = JS::js_undefined();
    if (arg0.is_object())
        check_opacity_property_value_5 = TRY(arg0.as_object().get("checkOpacity"_utf16_fly_string));

    bool check_opacity_value_5;

    if (!check_opacity_property_value_5.is_undefined())

    check_opacity_value_5 = check_opacity_property_value_5.to_boolean();

    else
        check_opacity_value_5 = static_cast<bool>(false);

    options.check_opacity = check_opacity_value_5;

    auto check_visibility_css_property_value_6 = JS::js_undefined();
    if (arg0.is_object())
        check_visibility_css_property_value_6 = TRY(arg0.as_object().get("checkVisibilityCSS"_utf16_fly_string));

    bool check_visibility_css_value_6;

    if (!check_visibility_css_property_value_6.is_undefined())

    check_visibility_css_value_6 = check_visibility_css_property_value_6.to_boolean();

    else
        check_visibility_css_value_6 = static_cast<bool>(false);

    options.check_visibility_css = check_visibility_css_value_6;

    auto content_visibility_auto_property_value_7 = JS::js_undefined();
    if (arg0.is_object())
        content_visibility_auto_property_value_7 = TRY(arg0.as_object().get("contentVisibilityAuto"_utf16_fly_string));

    bool content_visibility_auto_value_7;

    if (!content_visibility_auto_property_value_7.is_undefined())

    content_visibility_auto_value_7 = content_visibility_auto_property_value_7.to_boolean();

    else
        content_visibility_auto_value_7 = static_cast<bool>(false);

    options.content_visibility_auto = content_visibility_auto_value_7;

    auto opacity_property_property_value_8 = JS::js_undefined();
    if (arg0.is_object())
        opacity_property_property_value_8 = TRY(arg0.as_object().get("opacityProperty"_utf16_fly_string));

    bool opacity_property_value_8;

    if (!opacity_property_property_value_8.is_undefined())

    opacity_property_value_8 = opacity_property_property_value_8.to_boolean();

    else
        opacity_property_value_8 = static_cast<bool>(false);

    options.opacity_property = opacity_property_value_8;

    auto visibility_property_property_value_9 = JS::js_undefined();
    if (arg0.is_object())
        visibility_property_property_value_9 = TRY(arg0.as_object().get("visibilityProperty"_utf16_fly_string));

    bool visibility_property_value_9;

    if (!visibility_property_property_value_9.is_undefined())

    visibility_property_value_9 = visibility_property_property_value_9.to_boolean();

    else
        visibility_property_value_9 = static_cast<bool>(false);

    options.visibility_property = visibility_property_value_9;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->check_visibility(options); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::scroll_into_view)
{
    WebIDL::log_trace(vm, "ElementPrototype::scroll_into_view");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    auto arg0_to_dictionary = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<ScrollIntoViewOptions> {
        // This might be unused.
        (void)realm;

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ScrollIntoViewOptions");

    ScrollIntoViewOptions dictionary_union_type {};

    auto block_property_value_0 = JS::js_undefined();
    if (arg0.is_object())
        block_property_value_0 = TRY(arg0.as_object().get("block"_utf16_fly_string));

    ScrollLogicalPosition block_value_0 { ScrollLogicalPosition::Start };

    if (!block_property_value_0.is_undefined()) {

    auto block_property_value_0_string = TRY(block_property_value_0.to_string(vm));

    if (block_property_value_0_string == "start"sv)
        block_value_0 = ScrollLogicalPosition::Start;

    else if (block_property_value_0_string == "center"sv)
        block_value_0 = ScrollLogicalPosition::Center;

    else if (block_property_value_0_string == "end"sv)
        block_value_0 = ScrollLogicalPosition::End;

    else if (block_property_value_0_string == "nearest"sv)
        block_value_0 = ScrollLogicalPosition::Nearest;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, block_property_value_0_string, "ScrollLogicalPosition");

    }

    dictionary_union_type.block = block_value_0;

    auto container_property_value_1 = JS::js_undefined();
    if (arg0.is_object())
        container_property_value_1 = TRY(arg0.as_object().get("container"_utf16_fly_string));

    ScrollIntoViewContainer container_value_1 { ScrollIntoViewContainer::All };

    if (!container_property_value_1.is_undefined()) {

    auto container_property_value_1_string = TRY(container_property_value_1.to_string(vm));

    if (container_property_value_1_string == "all"sv)
        container_value_1 = ScrollIntoViewContainer::All;

    else if (container_property_value_1_string == "nearest"sv)
        container_value_1 = ScrollIntoViewContainer::Nearest;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, container_property_value_1_string, "ScrollIntoViewContainer");

    }

    dictionary_union_type.container = container_value_1;

    auto inline__property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        inline__property_value_2 = TRY(arg0.as_object().get("inline"_utf16_fly_string));

    ScrollLogicalPosition inline__value_2 { ScrollLogicalPosition::Nearest };

    if (!inline__property_value_2.is_undefined()) {

    auto inline__property_value_2_string = TRY(inline__property_value_2.to_string(vm));

    if (inline__property_value_2_string == "start"sv)
        inline__value_2 = ScrollLogicalPosition::Start;

    else if (inline__property_value_2_string == "center"sv)
        inline__value_2 = ScrollLogicalPosition::Center;

    else if (inline__property_value_2_string == "end"sv)
        inline__value_2 = ScrollLogicalPosition::End;

    else if (inline__property_value_2_string == "nearest"sv)
        inline__value_2 = ScrollLogicalPosition::Nearest;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, inline__property_value_2_string, "ScrollLogicalPosition");

    }

    dictionary_union_type.inline_ = inline__value_2;

    auto behavior_property_value_3 = JS::js_undefined();
    if (arg0.is_object())
        behavior_property_value_3 = TRY(arg0.as_object().get("behavior"_utf16_fly_string));

    ScrollBehavior behavior_value_3 { ScrollBehavior::Auto };

    if (!behavior_property_value_3.is_undefined()) {

    auto behavior_property_value_3_string = TRY(behavior_property_value_3.to_string(vm));

    if (behavior_property_value_3_string == "auto"sv)
        behavior_value_3 = ScrollBehavior::Auto;

    else if (behavior_property_value_3_string == "instant"sv)
        behavior_value_3 = ScrollBehavior::Instant;

    else if (behavior_property_value_3_string == "smooth"sv)
        behavior_value_3 = ScrollBehavior::Smooth;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, behavior_property_value_3_string, "ScrollBehavior");

    }

    dictionary_union_type.behavior = behavior_value_3;

        return dictionary_union_type;
    };

    auto arg0_to_variant = [&vm, &realm, &arg0_to_dictionary](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<bool, ScrollIntoViewOptions>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_nullish())
            return Variant<bool, ScrollIntoViewOptions> { TRY(arg0_to_dictionary(arg0)) };

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

        return Variant<bool, ScrollIntoViewOptions> { TRY(arg0_to_dictionary(arg0)) };

        }

        if (arg0.is_boolean())
            return Variant<bool, ScrollIntoViewOptions> { arg0.as_bool() };

        return Variant<bool, ScrollIntoViewOptions> { arg0.to_boolean() };

    };

    Variant<bool, ScrollIntoViewOptions> arg = arg0.is_undefined() ? TRY(arg0_to_dictionary(arg0)) : TRY(arg0_to_variant(arg0));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll_into_view(arg); }));

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::scroll0)
{
    WebIDL::log_trace(vm, "ElementPrototype::scroll0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ScrollToOptions");

    ScrollToOptions options {};

    auto left_property_value_10 = JS::js_undefined();
    if (arg0.is_object())
        left_property_value_10 = TRY(arg0.as_object().get("left"_utf16_fly_string));

    if (!left_property_value_10.is_undefined()) {

    Optional<double> left_value_10;

    if (!left_property_value_10.is_undefined())
        left_value_10 = TRY(left_property_value_10.to_double(vm));


    options.left = left_value_10;

    }

    auto top_property_value_11 = JS::js_undefined();
    if (arg0.is_object())
        top_property_value_11 = TRY(arg0.as_object().get("top"_utf16_fly_string));

    if (!top_property_value_11.is_undefined()) {

    Optional<double> top_value_11;

    if (!top_property_value_11.is_undefined())
        top_value_11 = TRY(top_property_value_11.to_double(vm));


    options.top = top_value_11;

    }

    auto behavior_property_value_12 = JS::js_undefined();
    if (arg0.is_object())
        behavior_property_value_12 = TRY(arg0.as_object().get("behavior"_utf16_fly_string));

    ScrollBehavior behavior_value_12 { ScrollBehavior::Auto };

    if (!behavior_property_value_12.is_undefined()) {

    auto behavior_property_value_12_string = TRY(behavior_property_value_12.to_string(vm));

    if (behavior_property_value_12_string == "auto"sv)
        behavior_value_12 = ScrollBehavior::Auto;

    else if (behavior_property_value_12_string == "instant"sv)
        behavior_value_12 = ScrollBehavior::Instant;

    else if (behavior_property_value_12_string == "smooth"sv)
        behavior_value_12 = ScrollBehavior::Smooth;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, behavior_property_value_12_string, "ScrollBehavior");

    }

    options.behavior = behavior_value_12;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll(options); }));

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::scroll1)
{
    WebIDL::log_trace(vm, "ElementPrototype::scroll1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll(x, y); }));

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::scroll_to0)
{
    WebIDL::log_trace(vm, "ElementPrototype::scroll_to0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ScrollToOptions");

    ScrollToOptions options {};

    auto left_property_value_13 = JS::js_undefined();
    if (arg0.is_object())
        left_property_value_13 = TRY(arg0.as_object().get("left"_utf16_fly_string));

    if (!left_property_value_13.is_undefined()) {

    Optional<double> left_value_13;

    if (!left_property_value_13.is_undefined())
        left_value_13 = TRY(left_property_value_13.to_double(vm));


    options.left = left_value_13;

    }

    auto top_property_value_14 = JS::js_undefined();
    if (arg0.is_object())
        top_property_value_14 = TRY(arg0.as_object().get("top"_utf16_fly_string));

    if (!top_property_value_14.is_undefined()) {

    Optional<double> top_value_14;

    if (!top_property_value_14.is_undefined())
        top_value_14 = TRY(top_property_value_14.to_double(vm));


    options.top = top_value_14;

    }

    auto behavior_property_value_15 = JS::js_undefined();
    if (arg0.is_object())
        behavior_property_value_15 = TRY(arg0.as_object().get("behavior"_utf16_fly_string));

    ScrollBehavior behavior_value_15 { ScrollBehavior::Auto };

    if (!behavior_property_value_15.is_undefined()) {

    auto behavior_property_value_15_string = TRY(behavior_property_value_15.to_string(vm));

    if (behavior_property_value_15_string == "auto"sv)
        behavior_value_15 = ScrollBehavior::Auto;

    else if (behavior_property_value_15_string == "instant"sv)
        behavior_value_15 = ScrollBehavior::Instant;

    else if (behavior_property_value_15_string == "smooth"sv)
        behavior_value_15 = ScrollBehavior::Smooth;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, behavior_property_value_15_string, "ScrollBehavior");

    }

    options.behavior = behavior_value_15;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll(options); }));

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::scroll_to1)
{
    WebIDL::log_trace(vm, "ElementPrototype::scroll_to1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll(x, y); }));

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::scroll_by0)
{
    WebIDL::log_trace(vm, "ElementPrototype::scroll_by0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ScrollToOptions");

    ScrollToOptions options {};

    auto left_property_value_16 = JS::js_undefined();
    if (arg0.is_object())
        left_property_value_16 = TRY(arg0.as_object().get("left"_utf16_fly_string));

    if (!left_property_value_16.is_undefined()) {

    Optional<double> left_value_16;

    if (!left_property_value_16.is_undefined())
        left_value_16 = TRY(left_property_value_16.to_double(vm));


    options.left = left_value_16;

    }

    auto top_property_value_17 = JS::js_undefined();
    if (arg0.is_object())
        top_property_value_17 = TRY(arg0.as_object().get("top"_utf16_fly_string));

    if (!top_property_value_17.is_undefined()) {

    Optional<double> top_value_17;

    if (!top_property_value_17.is_undefined())
        top_value_17 = TRY(top_property_value_17.to_double(vm));


    options.top = top_value_17;

    }

    auto behavior_property_value_18 = JS::js_undefined();
    if (arg0.is_object())
        behavior_property_value_18 = TRY(arg0.as_object().get("behavior"_utf16_fly_string));

    ScrollBehavior behavior_value_18 { ScrollBehavior::Auto };

    if (!behavior_property_value_18.is_undefined()) {

    auto behavior_property_value_18_string = TRY(behavior_property_value_18.to_string(vm));

    if (behavior_property_value_18_string == "auto"sv)
        behavior_value_18 = ScrollBehavior::Auto;

    else if (behavior_property_value_18_string == "instant"sv)
        behavior_value_18 = ScrollBehavior::Instant;

    else if (behavior_property_value_18_string == "smooth"sv)
        behavior_value_18 = ScrollBehavior::Smooth;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, behavior_property_value_18_string, "ScrollBehavior");

    }

    options.behavior = behavior_value_18;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll_by(options); }));

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::scroll_by1)
{
    WebIDL::log_trace(vm, "ElementPrototype::scroll_by1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    double x = TRY(arg0.to_double(vm));

    auto arg1 = vm.argument(1);

    double y = TRY(arg1.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll_by(x, y); }));

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::set_html_unsafe)
{
    WebIDL::log_trace(vm, "ElementPrototype::set_html_unsafe");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setHTMLUnsafe");

    auto arg0 = vm.argument(0);

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<TrustedHTML>, Utf16String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<TrustedHTML>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

    Utf16String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

        return { arg0_string };

    };

    Variant<GC::Root<TrustedHTML>, Utf16String> html = TRY(arg0_to_variant(arg0));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_html_unsafe(html); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::get_html)
{
    WebIDL::log_trace(vm, "ElementPrototype::get_html");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "GetHTMLOptions");

    GetHTMLOptions options {};

    auto serializable_shadow_roots_property_value_19 = JS::js_undefined();
    if (arg0.is_object())
        serializable_shadow_roots_property_value_19 = TRY(arg0.as_object().get("serializableShadowRoots"_utf16_fly_string));

    bool serializable_shadow_roots_value_19;

    if (!serializable_shadow_roots_property_value_19.is_undefined())

    serializable_shadow_roots_value_19 = serializable_shadow_roots_property_value_19.to_boolean();

    else
        serializable_shadow_roots_value_19 = static_cast<bool>(false);

    options.serializable_shadow_roots = serializable_shadow_roots_value_19;

    auto shadow_roots_property_value_20 = JS::js_undefined();
    if (arg0.is_object())
        shadow_roots_property_value_20 = TRY(arg0.as_object().get("shadowRoots"_utf16_fly_string));

    GC::RootVector<GC::Root<ShadowRoot>> shadow_roots_value_20 { vm.heap() };

    if (!shadow_roots_property_value_20.is_undefined()) {

    if (!shadow_roots_property_value_20.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, shadow_roots_property_value_20);

    auto shadow_roots_property_value_20_iterator_method0 = TRY(shadow_roots_property_value_20.get_method(vm, vm.well_known_symbol_iterator()));
    if (!shadow_roots_property_value_20_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, shadow_roots_property_value_20);

    auto shadow_roots_property_value_20_iterator1 = TRY(JS::get_iterator_from_method(vm, shadow_roots_property_value_20, *shadow_roots_property_value_20_iterator_method0));

    GC::RootVector<GC::Root<ShadowRoot>> shadow_roots_value_20_non_optional { vm.heap() };

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, shadow_roots_property_value_20_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object() || !is<ShadowRoot>(next_item1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ShadowRoot");

    auto& sequence_item1 = static_cast<ShadowRoot&>(next_item1.as_object());

    shadow_roots_value_20_non_optional.append(sequence_item1);
    }

        shadow_roots_value_20 = move(shadow_roots_value_20_non_optional);
    }

    options.shadow_roots = shadow_roots_value_20;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_html(options); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::insert_adjacent_html)
{
    WebIDL::log_trace(vm, "ElementPrototype::insert_adjacent_html");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "insertAdjacentHTML", "2");

    auto arg0 = vm.argument(0);

    String position;
    if (!false || !arg0.is_null()) {
        position = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<TrustedHTML>, Utf16String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (is<PlatformObject>(arg1_object)) {

                if (auto* arg1_result = as_if<TrustedHTML>(arg1_object))
                    return GC::make_root(*arg1_result);

            }

        }

    Utf16String arg1_string;
    if (!false || !arg1.is_null()) {
        arg1_string = TRY(WebIDL::to_utf16_string(vm, arg1));
    }

        return { arg1_string };

    };

    Variant<GC::Root<TrustedHTML>, Utf16String> text = TRY(arg1_to_variant(arg1));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->insert_adjacent_html(position, text); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::set_pointer_capture)
{
    WebIDL::log_trace(vm, "ElementPrototype::set_pointer_capture");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "setPointerCapture");

    auto arg0 = vm.argument(0);

    WebIDL::Long pointer_id;

    pointer_id = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_pointer_capture(pointer_id); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::release_pointer_capture)
{
    WebIDL::log_trace(vm, "ElementPrototype::release_pointer_capture");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "releasePointerCapture");

    auto arg0 = vm.argument(0);

    WebIDL::Long pointer_id;

    pointer_id = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->release_pointer_capture(pointer_id); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::has_pointer_capture)
{
    WebIDL::log_trace(vm, "ElementPrototype::has_pointer_capture");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "hasPointerCapture");

    auto arg0 = vm.argument(0);

    WebIDL::Long pointer_id;

    pointer_id = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->has_pointer_capture(pointer_id); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::request_pointer_lock)
{
    WebIDL::log_trace(vm, "ElementPrototype::request_pointer_lock");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "PointerLockOptions");

    PointerLockOptions options {};

    auto unadjusted_movement_property_value_21 = JS::js_undefined();
    if (arg0.is_object())
        unadjusted_movement_property_value_21 = TRY(arg0.as_object().get("unadjustedMovement"_utf16_fly_string));

    bool unadjusted_movement_value_21;

    if (!unadjusted_movement_property_value_21.is_undefined())

    unadjusted_movement_value_21 = unadjusted_movement_property_value_21.to_boolean();

    else
        unadjusted_movement_value_21 = static_cast<bool>(false);

    options.unadjusted_movement = unadjusted_movement_value_21;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->request_pointer_lock(options); }));

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::computed_style_map)
{
    WebIDL::log_trace(vm, "ElementPrototype::computed_style_map");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->computed_style_map(); }));

    return &const_cast<StylePropertyMapReadOnly&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::request_fullscreen)
{
    WebIDL::log_trace(vm, "ElementPrototype::request_fullscreen");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->request_fullscreen(); }));

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

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::before)
{
    WebIDL::log_trace(vm, "ElementPrototype::before");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<Node>, Utf16String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<Node>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

    Utf16String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

        return { arg0_string };

    };

        Vector<Variant<GC::Root<Node>, Utf16String>> nodes;

        if (vm.argument_count() > 0) {
            nodes.ensure_capacity(vm.argument_count() - 0);

            for (size_t i = 0; i < vm.argument_count(); ++i) {
                auto result = TRY(arg0_to_variant(vm.argument(i)));
                nodes.unchecked_append(move(result));
            }
        }
    
    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->before(move(nodes)); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::after)
{
    WebIDL::log_trace(vm, "ElementPrototype::after");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<Node>, Utf16String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<Node>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

    Utf16String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

        return { arg0_string };

    };

        Vector<Variant<GC::Root<Node>, Utf16String>> nodes;

        if (vm.argument_count() > 0) {
            nodes.ensure_capacity(vm.argument_count() - 0);

            for (size_t i = 0; i < vm.argument_count(); ++i) {
                auto result = TRY(arg0_to_variant(vm.argument(i)));
                nodes.unchecked_append(move(result));
            }
        }
    
    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->after(move(nodes)); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::replace_with)
{
    WebIDL::log_trace(vm, "ElementPrototype::replace_with");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<Node>, Utf16String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<Node>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

    Utf16String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

        return { arg0_string };

    };

        Vector<Variant<GC::Root<Node>, Utf16String>> nodes;

        if (vm.argument_count() > 0) {
            nodes.ensure_capacity(vm.argument_count() - 0);

            for (size_t i = 0; i < vm.argument_count(); ++i) {
                auto result = TRY(arg0_to_variant(vm.argument(i)));
                nodes.unchecked_append(move(result));
            }
        }
    
    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->replace_with(move(nodes)); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::remove)
{
    WebIDL::log_trace(vm, "ElementPrototype::remove");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->remove_binding(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::animate)
{
    WebIDL::log_trace(vm, "ElementPrototype::animate");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "animate");

    auto arg0 = vm.argument(0);

    Optional<GC::Root<JS::Object>> keyframes;
    if (!arg0.is_null() && !arg0.is_undefined()) {
        if (!arg0.is_object())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg0);
        keyframes = GC::make_root(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_dictionary = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<KeyframeAnimationOptions> {
        // This might be unused.
        (void)realm;

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "KeyframeAnimationOptions");

    KeyframeAnimationOptions dictionary_union_type {};

    auto id_property_value_4 = JS::js_undefined();
    if (arg1.is_object())
        id_property_value_4 = TRY(arg1.as_object().get("id"_utf16_fly_string));

    String id_value_4;

    if (!id_property_value_4.is_undefined()) {
        if (!false || !id_property_value_4.is_null())
            id_value_4 = TRY(WebIDL::to_string(vm, id_property_value_4));
    } else {
        id_value_4 = ""_string;
    }

    dictionary_union_type.id = id_value_4;

    auto timeline_property_value_5 = JS::js_undefined();
    if (arg1.is_object())
        timeline_property_value_5 = TRY(arg1.as_object().get("timeline"_utf16_fly_string));

    if (!timeline_property_value_5.is_undefined()) {

    GC::Ptr<AnimationTimeline> timeline_value_5;

    if (!timeline_property_value_5.is_nullish()) {
        if (!timeline_property_value_5.is_object() || !is<AnimationTimeline>(timeline_property_value_5.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AnimationTimeline");

        timeline_value_5 = &static_cast<AnimationTimeline&>(timeline_property_value_5.as_object());
    }

    dictionary_union_type.timeline = timeline_value_5;

    }

    auto composite_property_value_6 = JS::js_undefined();
    if (arg1.is_object())
        composite_property_value_6 = TRY(arg1.as_object().get("composite"_utf16_fly_string));

    CompositeOperation composite_value_6 { CompositeOperation::Replace };

    if (!composite_property_value_6.is_undefined()) {

    auto composite_property_value_6_string = TRY(composite_property_value_6.to_string(vm));

    if (composite_property_value_6_string == "replace"sv)
        composite_value_6 = CompositeOperation::Replace;

    else if (composite_property_value_6_string == "add"sv)
        composite_value_6 = CompositeOperation::Add;

    else if (composite_property_value_6_string == "accumulate"sv)
        composite_value_6 = CompositeOperation::Accumulate;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, composite_property_value_6_string, "CompositeOperation");

    }

    dictionary_union_type.composite = composite_value_6;

    auto pseudo_element_property_value_7 = JS::js_undefined();
    if (arg1.is_object())
        pseudo_element_property_value_7 = TRY(arg1.as_object().get("pseudoElement"_utf16_fly_string));

    Optional<String> pseudo_element_value_7;

    if (!pseudo_element_property_value_7.is_undefined()) {
        if (!pseudo_element_property_value_7.is_null())
            pseudo_element_value_7 = TRY(WebIDL::to_string(vm, pseudo_element_property_value_7));
    }

    dictionary_union_type.pseudo_element = pseudo_element_value_7;

    auto delay_property_value_8 = JS::js_undefined();
    if (arg1.is_object())
        delay_property_value_8 = TRY(arg1.as_object().get("delay"_utf16_fly_string));

    double delay_value_8;

    if (!delay_property_value_8.is_undefined())
        delay_value_8 = TRY(delay_property_value_8.to_double(vm));

    else
        delay_value_8 = 0;

    if (isinf(delay_value_8) || isnan(delay_value_8)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "delay");
    }
    
    dictionary_union_type.delay = delay_value_8;

    auto direction_property_value_9 = JS::js_undefined();
    if (arg1.is_object())
        direction_property_value_9 = TRY(arg1.as_object().get("direction"_utf16_fly_string));

    PlaybackDirection direction_value_9 { PlaybackDirection::Normal };

    if (!direction_property_value_9.is_undefined()) {

    auto direction_property_value_9_string = TRY(direction_property_value_9.to_string(vm));

    if (direction_property_value_9_string == "normal"sv)
        direction_value_9 = PlaybackDirection::Normal;

    else if (direction_property_value_9_string == "reverse"sv)
        direction_value_9 = PlaybackDirection::Reverse;

    else if (direction_property_value_9_string == "alternate"sv)
        direction_value_9 = PlaybackDirection::Alternate;

    else if (direction_property_value_9_string == "alternate-reverse"sv)
        direction_value_9 = PlaybackDirection::AlternateReverse;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, direction_property_value_9_string, "PlaybackDirection");

    }

    dictionary_union_type.direction = direction_value_9;

    auto duration_property_value_10 = JS::js_undefined();
    if (arg1.is_object())
        duration_property_value_10 = TRY(arg1.as_object().get("duration"_utf16_fly_string));

    auto duration_property_value_10_to_variant = [&vm, &realm](JS::Value duration_property_value_10) -> JS::ThrowCompletionOr<Variant<double, GC::Root<CSSNumericValue>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (duration_property_value_10.is_object()) {
            [[maybe_unused]] auto& duration_property_value_10_object = duration_property_value_10.as_object();

            if (is<PlatformObject>(duration_property_value_10_object)) {

                if (auto* duration_property_value_10_result = as_if<CSSNumericValue>(duration_property_value_10_object))
                    return GC::make_root(*duration_property_value_10_result);

            }

        }

        if (duration_property_value_10.is_number()) {

    double duration_property_value_10_number = TRY(duration_property_value_10.to_double(vm));

            return { duration_property_value_10_number };
        }

    String duration_property_value_10_string;
    if (!false || !duration_property_value_10.is_null()) {
        duration_property_value_10_string = TRY(WebIDL::to_string(vm, duration_property_value_10));
    }

        return { duration_property_value_10_string };

    };

    Variant<double, GC::Root<CSSNumericValue>, String> duration_value_10 = duration_property_value_10.is_undefined() ? TRY(duration_property_value_10_to_variant(JS::Value(JS::PrimitiveString::create(vm, MUST(String::from_utf8("auto"sv)))))) : TRY(duration_property_value_10_to_variant(duration_property_value_10));

    dictionary_union_type.duration = duration_value_10;

    auto easing_property_value_11 = JS::js_undefined();
    if (arg1.is_object())
        easing_property_value_11 = TRY(arg1.as_object().get("easing"_utf16_fly_string));

    String easing_value_11;

    if (!easing_property_value_11.is_undefined()) {
        if (!false || !easing_property_value_11.is_null())
            easing_value_11 = TRY(WebIDL::to_string(vm, easing_property_value_11));
    } else {
        easing_value_11 = "linear"_string;
    }

    dictionary_union_type.easing = easing_value_11;

    auto end_delay_property_value_12 = JS::js_undefined();
    if (arg1.is_object())
        end_delay_property_value_12 = TRY(arg1.as_object().get("endDelay"_utf16_fly_string));

    double end_delay_value_12;

    if (!end_delay_property_value_12.is_undefined())
        end_delay_value_12 = TRY(end_delay_property_value_12.to_double(vm));

    else
        end_delay_value_12 = 0;

    if (isinf(end_delay_value_12) || isnan(end_delay_value_12)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "endDelay");
    }
    
    dictionary_union_type.end_delay = end_delay_value_12;

    auto fill_property_value_13 = JS::js_undefined();
    if (arg1.is_object())
        fill_property_value_13 = TRY(arg1.as_object().get("fill"_utf16_fly_string));

    FillMode fill_value_13 { FillMode::Auto };

    if (!fill_property_value_13.is_undefined()) {

    auto fill_property_value_13_string = TRY(fill_property_value_13.to_string(vm));

    if (fill_property_value_13_string == "none"sv)
        fill_value_13 = FillMode::None;

    else if (fill_property_value_13_string == "forwards"sv)
        fill_value_13 = FillMode::Forwards;

    else if (fill_property_value_13_string == "backwards"sv)
        fill_value_13 = FillMode::Backwards;

    else if (fill_property_value_13_string == "both"sv)
        fill_value_13 = FillMode::Both;

    else if (fill_property_value_13_string == "auto"sv)
        fill_value_13 = FillMode::Auto;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, fill_property_value_13_string, "FillMode");

    }

    dictionary_union_type.fill = fill_value_13;

    auto iteration_start_property_value_14 = JS::js_undefined();
    if (arg1.is_object())
        iteration_start_property_value_14 = TRY(arg1.as_object().get("iterationStart"_utf16_fly_string));

    double iteration_start_value_14;

    if (!iteration_start_property_value_14.is_undefined())
        iteration_start_value_14 = TRY(iteration_start_property_value_14.to_double(vm));

    else
        iteration_start_value_14 = 0.0;

    if (isinf(iteration_start_value_14) || isnan(iteration_start_value_14)) {
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidRestrictedFloatingPointParameter, "iterationStart");
    }
    
    dictionary_union_type.iteration_start = iteration_start_value_14;

    auto iterations_property_value_15 = JS::js_undefined();
    if (arg1.is_object())
        iterations_property_value_15 = TRY(arg1.as_object().get("iterations"_utf16_fly_string));

    double iterations_value_15;

    if (!iterations_property_value_15.is_undefined())
        iterations_value_15 = TRY(iterations_property_value_15.to_double(vm));

    else
        iterations_value_15 = 1.0;

    dictionary_union_type.iterations = iterations_value_15;

        return dictionary_union_type;
    };

    auto arg1_to_variant = [&vm, &realm, &arg1_to_dictionary](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<double, KeyframeAnimationOptions>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_nullish())
            return Variant<double, KeyframeAnimationOptions> { TRY(arg1_to_dictionary(arg1)) };

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

        return Variant<double, KeyframeAnimationOptions> { TRY(arg1_to_dictionary(arg1)) };

        }

        if (arg1.is_number()) {

    double arg1_number = TRY(arg1.to_double(vm));

            return { arg1_number };
        }

    double arg1_number = TRY(arg1.to_double(vm));

        return { arg1_number };

    };

    Variant<double, KeyframeAnimationOptions> options = arg1.is_undefined() ? TRY(arg1_to_dictionary(arg1)) : TRY(arg1_to_variant(arg1));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->animate(keyframes, options); }));

    return &const_cast<Animation&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::get_animations)
{
    WebIDL::log_trace(vm, "ElementPrototype::get_animations");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "GetAnimationsOptions");

    GetAnimationsOptions options {};

    auto pseudo_element_property_value_22 = JS::js_undefined();
    if (arg0.is_object())
        pseudo_element_property_value_22 = TRY(arg0.as_object().get("pseudoElement"_utf16_fly_string));

    Optional<String> pseudo_element_value_22;

    if (!pseudo_element_property_value_22.is_undefined()) {
        if (!pseudo_element_property_value_22.is_null())
            pseudo_element_value_22 = TRY(WebIDL::to_string(vm, pseudo_element_property_value_22));
    }

    options.pseudo_element = pseudo_element_value_22;

    auto subtree_property_value_23 = JS::js_undefined();
    if (arg0.is_object())
        subtree_property_value_23 = TRY(arg0.as_object().get("subtree"_utf16_fly_string));

    bool subtree_value_23;

    if (!subtree_property_value_23.is_undefined())

    subtree_value_23 = subtree_property_value_23.to_boolean();

    else
        subtree_value_23 = static_cast<bool>(false);

    options.subtree = subtree_value_23;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_animations(options); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::prepend)
{
    WebIDL::log_trace(vm, "ElementPrototype::prepend");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<Node>, Utf16String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<Node>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

    Utf16String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

        return { arg0_string };

    };

        Vector<Variant<GC::Root<Node>, Utf16String>> nodes;

        if (vm.argument_count() > 0) {
            nodes.ensure_capacity(vm.argument_count() - 0);

            for (size_t i = 0; i < vm.argument_count(); ++i) {
                auto result = TRY(arg0_to_variant(vm.argument(i)));
                nodes.unchecked_append(move(result));
            }
        }
    
    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->prepend(move(nodes)); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::append)
{
    WebIDL::log_trace(vm, "ElementPrototype::append");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<Node>, Utf16String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<Node>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

    Utf16String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

        return { arg0_string };

    };

        Vector<Variant<GC::Root<Node>, Utf16String>> nodes;

        if (vm.argument_count() > 0) {
            nodes.ensure_capacity(vm.argument_count() - 0);

            for (size_t i = 0; i < vm.argument_count(); ++i) {
                auto result = TRY(arg0_to_variant(vm.argument(i)));
                nodes.unchecked_append(move(result));
            }
        }
    
    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->append(move(nodes)); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::replace_children)
{
    WebIDL::log_trace(vm, "ElementPrototype::replace_children");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0_to_variant = [&vm, &realm](JS::Value arg0) -> JS::ThrowCompletionOr<Variant<GC::Root<Node>, Utf16String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg0.is_object()) {
            [[maybe_unused]] auto& arg0_object = arg0.as_object();

            if (is<PlatformObject>(arg0_object)) {

                if (auto* arg0_result = as_if<Node>(arg0_object))
                    return GC::make_root(*arg0_result);

            }

        }

    Utf16String arg0_string;
    if (!false || !arg0.is_null()) {
        arg0_string = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

        return { arg0_string };

    };

        Vector<Variant<GC::Root<Node>, Utf16String>> nodes;

        if (vm.argument_count() > 0) {
            nodes.ensure_capacity(vm.argument_count() - 0);

            for (size_t i = 0; i < vm.argument_count(); ++i) {
                auto result = TRY(arg0_to_variant(vm.argument(i)));
                nodes.unchecked_append(move(result));
            }
        }
    
    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->replace_children(move(nodes)); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::move_before)
{
    WebIDL::log_trace(vm, "ElementPrototype::move_before");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "moveBefore", "2");

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

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->move_before(node, child); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::query_selector)
{
    WebIDL::log_trace(vm, "ElementPrototype::query_selector");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "querySelector");

    auto arg0 = vm.argument(0);

    String selectors;
    if (!false || !arg0.is_null()) {
        selectors = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->query_selector(selectors); }));

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::query_selector_all)
{
    WebIDL::log_trace(vm, "ElementPrototype::query_selector_all");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "querySelectorAll");

    auto arg0 = vm.argument(0);

    String selectors;
    if (!false || !arg0.is_null()) {
        selectors = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->query_selector_all(selectors); }));

    return &const_cast<NodeList&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::scroll)
{
    WebIDL::log_trace(vm, "ElementPrototype::scroll");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(2, vm.argument_count())) {

    case 0:
        chosen_overload_callable_id = 0;
        break;


    case 1:
        chosen_overload_callable_id = 0;
        break;


    case 2:
        chosen_overload_callable_id = 1;
        break;


    }

    Vector<StringView> dictionary_types {
    "ScrollToOptions"sv,
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return scroll0(vm);

    case 1:

        return scroll1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::scroll_to)
{
    WebIDL::log_trace(vm, "ElementPrototype::scroll_to");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(2, vm.argument_count())) {

    case 0:
        chosen_overload_callable_id = 0;
        break;


    case 1:
        chosen_overload_callable_id = 0;
        break;


    case 2:
        chosen_overload_callable_id = 1;
        break;


    }

    Vector<StringView> dictionary_types {
    "ScrollToOptions"sv,
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return scroll_to0(vm);

    case 1:

        return scroll_to1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(ElementPrototype::scroll_by)
{
    WebIDL::log_trace(vm, "ElementPrototype::scroll_by");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(2, vm.argument_count())) {

    case 0:
        chosen_overload_callable_id = 0;
        break;


    case 1:
        chosen_overload_callable_id = 0;
        break;


    case 2:
        chosen_overload_callable_id = 1;
        break;


    }

    Vector<StringView> dictionary_types {
    "ScrollToOptions"sv,
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return scroll_by0(vm);

    case 1:

        return scroll_by1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

} // namespace Web::Bindings
