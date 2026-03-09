
#include <AK/Function.h>
#include <LibIDL/Types.h>
#include <LibJS/Runtime/Array.h>
#include <LibJS/Runtime/DataView.h>
#include <LibJS/Runtime/Error.h>
#include <LibJS/Runtime/FunctionObject.h>
#include <LibJS/Runtime/GlobalObject.h>
#include <LibJS/Runtime/Iterator.h>
#include <LibJS/Runtime/TypedArray.h>
#include <LibJS/Runtime/Value.h>
#include <LibJS/Runtime/ValueInlines.h>
#include <LibURL/Origin.h>
#include <LibWeb/Bindings/WindowGlobalMixin.h>
#include <LibWeb/Bindings/WindowPrototype.h>
#include <LibWeb/Bindings/ExceptionOrUtils.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/Bindings/PrincipalHostDefined.h>
#include <LibWeb/DOM/Element.h>
#include <LibWeb/DOM/Event.h>
#include <LibWeb/DOM/IDLEventListener.h>
#include <LibWeb/DOM/NodeFilter.h>
#include <LibWeb/DOM/Range.h>
#include <LibWeb/HTML/Scripting/Environments.h>
#include <LibWeb/HTML/Window.h>
#include <LibWeb/HTML/WindowProxy.h>
#include <LibWeb/WebIDL/AbstractOperations.h>
#include <LibWeb/WebIDL/OverloadResolution.h>
#include <LibWeb/WebIDL/Tracing.h>
#include <LibWeb/WebIDL/Types.h>


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Window.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CookieStore/CookieStore.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/MediaQueryList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/Screen.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/VisualViewport.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Document.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CustomElements/CustomElementRegistry.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/History.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Navigation.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Navigator.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/RequestIdleCallback/IdleRequest.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Speech/SpeechSynthesis.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

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

WindowGlobalMixin::WindowGlobalMixin() = default;
WindowGlobalMixin::~WindowGlobalMixin() = default;

void WindowGlobalMixin::initialize(JS::Realm& realm, JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;

    object.set_prototype(&ensure_web_prototype<WindowPrototype>(realm, "Window"_fly_string));

    auto native_self_getter = JS::NativeFunction::create(realm, self_getter, 0, "self"_utf16_fly_string, &realm, "get"sv);

    auto native_self_setter = JS::NativeFunction::create(realm, self_setter, 1, "self"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("self"_utf16_fly_string, native_self_getter, native_self_setter, default_attributes);

    auto native_name_getter = JS::NativeFunction::create(realm, name_getter, 0, "name"_utf16_fly_string, &realm, "get"sv);

    auto native_name_setter = JS::NativeFunction::create(realm, name_setter, 1, "name"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("name"_utf16_fly_string, native_name_getter, native_name_setter, default_attributes);

    auto native_status_getter = JS::NativeFunction::create(realm, status_getter, 0, "status"_utf16_fly_string, &realm, "get"sv);

    auto native_status_setter = JS::NativeFunction::create(realm, status_setter, 1, "status"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("status"_utf16_fly_string, native_status_getter, native_status_setter, default_attributes);

    auto native_closed_getter = JS::NativeFunction::create(realm, closed_getter, 0, "closed"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_closed_setter;

    object.define_direct_accessor("closed"_utf16_fly_string, native_closed_getter, native_closed_setter, default_attributes);

    auto native_history_getter = JS::NativeFunction::create(realm, history_getter, 0, "history"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_history_setter;

    object.define_direct_accessor("history"_utf16_fly_string, native_history_getter, native_history_setter, default_attributes);

    auto native_navigation_getter = JS::NativeFunction::create(realm, navigation_getter, 0, "navigation"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_navigation_setter;

    object.define_direct_accessor("navigation"_utf16_fly_string, native_navigation_getter, native_navigation_setter, default_attributes);

    auto native_custom_elements_getter = JS::NativeFunction::create(realm, custom_elements_getter, 0, "customElements"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_custom_elements_setter;

    object.define_direct_accessor("customElements"_utf16_fly_string, native_custom_elements_getter, native_custom_elements_setter, default_attributes);

    auto native_locationbar_getter = JS::NativeFunction::create(realm, locationbar_getter, 0, "locationbar"_utf16_fly_string, &realm, "get"sv);

    auto native_locationbar_setter = JS::NativeFunction::create(realm, locationbar_setter, 1, "locationbar"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("locationbar"_utf16_fly_string, native_locationbar_getter, native_locationbar_setter, default_attributes);

    auto native_menubar_getter = JS::NativeFunction::create(realm, menubar_getter, 0, "menubar"_utf16_fly_string, &realm, "get"sv);

    auto native_menubar_setter = JS::NativeFunction::create(realm, menubar_setter, 1, "menubar"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("menubar"_utf16_fly_string, native_menubar_getter, native_menubar_setter, default_attributes);

    auto native_personalbar_getter = JS::NativeFunction::create(realm, personalbar_getter, 0, "personalbar"_utf16_fly_string, &realm, "get"sv);

    auto native_personalbar_setter = JS::NativeFunction::create(realm, personalbar_setter, 1, "personalbar"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("personalbar"_utf16_fly_string, native_personalbar_getter, native_personalbar_setter, default_attributes);

    auto native_scrollbars_getter = JS::NativeFunction::create(realm, scrollbars_getter, 0, "scrollbars"_utf16_fly_string, &realm, "get"sv);

    auto native_scrollbars_setter = JS::NativeFunction::create(realm, scrollbars_setter, 1, "scrollbars"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("scrollbars"_utf16_fly_string, native_scrollbars_getter, native_scrollbars_setter, default_attributes);

    auto native_statusbar_getter = JS::NativeFunction::create(realm, statusbar_getter, 0, "statusbar"_utf16_fly_string, &realm, "get"sv);

    auto native_statusbar_setter = JS::NativeFunction::create(realm, statusbar_setter, 1, "statusbar"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("statusbar"_utf16_fly_string, native_statusbar_getter, native_statusbar_setter, default_attributes);

    auto native_toolbar_getter = JS::NativeFunction::create(realm, toolbar_getter, 0, "toolbar"_utf16_fly_string, &realm, "get"sv);

    auto native_toolbar_setter = JS::NativeFunction::create(realm, toolbar_setter, 1, "toolbar"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("toolbar"_utf16_fly_string, native_toolbar_getter, native_toolbar_setter, default_attributes);

    auto native_frames_getter = JS::NativeFunction::create(realm, frames_getter, 0, "frames"_utf16_fly_string, &realm, "get"sv);

    auto native_frames_setter = JS::NativeFunction::create(realm, frames_setter, 1, "frames"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("frames"_utf16_fly_string, native_frames_getter, native_frames_setter, default_attributes);

    auto native_length_getter = JS::NativeFunction::create(realm, length_getter, 0, "length"_utf16_fly_string, &realm, "get"sv);

    auto native_length_setter = JS::NativeFunction::create(realm, length_setter, 1, "length"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("length"_utf16_fly_string, native_length_getter, native_length_setter, default_attributes);

    auto native_opener_getter = JS::NativeFunction::create(realm, opener_getter, 0, "opener"_utf16_fly_string, &realm, "get"sv);

    auto native_opener_setter = JS::NativeFunction::create(realm, opener_setter, 1, "opener"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("opener"_utf16_fly_string, native_opener_getter, native_opener_setter, default_attributes);

    auto native_parent_getter = JS::NativeFunction::create(realm, parent_getter, 0, "parent"_utf16_fly_string, &realm, "get"sv);

    auto native_parent_setter = JS::NativeFunction::create(realm, parent_setter, 1, "parent"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("parent"_utf16_fly_string, native_parent_getter, native_parent_setter, default_attributes);

    auto native_frame_element_getter = JS::NativeFunction::create(realm, frame_element_getter, 0, "frameElement"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_frame_element_setter;

    object.define_direct_accessor("frameElement"_utf16_fly_string, native_frame_element_getter, native_frame_element_setter, default_attributes);

    auto native_navigator_getter = JS::NativeFunction::create(realm, navigator_getter, 0, "navigator"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_navigator_setter;

    object.define_direct_accessor("navigator"_utf16_fly_string, native_navigator_getter, native_navigator_setter, default_attributes);

    auto native_client_information_getter = JS::NativeFunction::create(realm, client_information_getter, 0, "clientInformation"_utf16_fly_string, &realm, "get"sv);

    auto native_client_information_setter = JS::NativeFunction::create(realm, client_information_setter, 1, "clientInformation"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("clientInformation"_utf16_fly_string, native_client_information_getter, native_client_information_setter, default_attributes);

    auto native_event_getter = JS::NativeFunction::create(realm, event_getter, 0, "event"_utf16_fly_string, &realm, "get"sv);

    auto native_event_setter = JS::NativeFunction::create(realm, event_setter, 1, "event"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("event"_utf16_fly_string, native_event_getter, native_event_setter, default_attributes);

    auto native_screen_getter = JS::NativeFunction::create(realm, screen_getter, 0, "screen"_utf16_fly_string, &realm, "get"sv);

    auto native_screen_setter = JS::NativeFunction::create(realm, screen_setter, 1, "screen"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("screen"_utf16_fly_string, native_screen_getter, native_screen_setter, default_attributes);

    auto native_visual_viewport_getter = JS::NativeFunction::create(realm, visual_viewport_getter, 0, "visualViewport"_utf16_fly_string, &realm, "get"sv);

    auto native_visual_viewport_setter = JS::NativeFunction::create(realm, visual_viewport_setter, 1, "visualViewport"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("visualViewport"_utf16_fly_string, native_visual_viewport_getter, native_visual_viewport_setter, default_attributes);

    auto native_inner_width_getter = JS::NativeFunction::create(realm, inner_width_getter, 0, "innerWidth"_utf16_fly_string, &realm, "get"sv);

    auto native_inner_width_setter = JS::NativeFunction::create(realm, inner_width_setter, 1, "innerWidth"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("innerWidth"_utf16_fly_string, native_inner_width_getter, native_inner_width_setter, default_attributes);

    auto native_inner_height_getter = JS::NativeFunction::create(realm, inner_height_getter, 0, "innerHeight"_utf16_fly_string, &realm, "get"sv);

    auto native_inner_height_setter = JS::NativeFunction::create(realm, inner_height_setter, 1, "innerHeight"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("innerHeight"_utf16_fly_string, native_inner_height_getter, native_inner_height_setter, default_attributes);

    auto native_scroll_x_getter = JS::NativeFunction::create(realm, scroll_x_getter, 0, "scrollX"_utf16_fly_string, &realm, "get"sv);

    auto native_scroll_x_setter = JS::NativeFunction::create(realm, scroll_x_setter, 1, "scrollX"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("scrollX"_utf16_fly_string, native_scroll_x_getter, native_scroll_x_setter, default_attributes);

    auto native_page_x_offset_getter = JS::NativeFunction::create(realm, page_x_offset_getter, 0, "pageXOffset"_utf16_fly_string, &realm, "get"sv);

    auto native_page_x_offset_setter = JS::NativeFunction::create(realm, page_x_offset_setter, 1, "pageXOffset"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("pageXOffset"_utf16_fly_string, native_page_x_offset_getter, native_page_x_offset_setter, default_attributes);

    auto native_scroll_y_getter = JS::NativeFunction::create(realm, scroll_y_getter, 0, "scrollY"_utf16_fly_string, &realm, "get"sv);

    auto native_scroll_y_setter = JS::NativeFunction::create(realm, scroll_y_setter, 1, "scrollY"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("scrollY"_utf16_fly_string, native_scroll_y_getter, native_scroll_y_setter, default_attributes);

    auto native_page_y_offset_getter = JS::NativeFunction::create(realm, page_y_offset_getter, 0, "pageYOffset"_utf16_fly_string, &realm, "get"sv);

    auto native_page_y_offset_setter = JS::NativeFunction::create(realm, page_y_offset_setter, 1, "pageYOffset"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("pageYOffset"_utf16_fly_string, native_page_y_offset_getter, native_page_y_offset_setter, default_attributes);

    auto native_screen_x_getter = JS::NativeFunction::create(realm, screen_x_getter, 0, "screenX"_utf16_fly_string, &realm, "get"sv);

    auto native_screen_x_setter = JS::NativeFunction::create(realm, screen_x_setter, 1, "screenX"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("screenX"_utf16_fly_string, native_screen_x_getter, native_screen_x_setter, default_attributes);

    auto native_screen_left_getter = JS::NativeFunction::create(realm, screen_left_getter, 0, "screenLeft"_utf16_fly_string, &realm, "get"sv);

    auto native_screen_left_setter = JS::NativeFunction::create(realm, screen_left_setter, 1, "screenLeft"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("screenLeft"_utf16_fly_string, native_screen_left_getter, native_screen_left_setter, default_attributes);

    auto native_screen_y_getter = JS::NativeFunction::create(realm, screen_y_getter, 0, "screenY"_utf16_fly_string, &realm, "get"sv);

    auto native_screen_y_setter = JS::NativeFunction::create(realm, screen_y_setter, 1, "screenY"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("screenY"_utf16_fly_string, native_screen_y_getter, native_screen_y_setter, default_attributes);

    auto native_screen_top_getter = JS::NativeFunction::create(realm, screen_top_getter, 0, "screenTop"_utf16_fly_string, &realm, "get"sv);

    auto native_screen_top_setter = JS::NativeFunction::create(realm, screen_top_setter, 1, "screenTop"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("screenTop"_utf16_fly_string, native_screen_top_getter, native_screen_top_setter, default_attributes);

    auto native_outer_width_getter = JS::NativeFunction::create(realm, outer_width_getter, 0, "outerWidth"_utf16_fly_string, &realm, "get"sv);

    auto native_outer_width_setter = JS::NativeFunction::create(realm, outer_width_setter, 1, "outerWidth"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("outerWidth"_utf16_fly_string, native_outer_width_getter, native_outer_width_setter, default_attributes);

    auto native_outer_height_getter = JS::NativeFunction::create(realm, outer_height_getter, 0, "outerHeight"_utf16_fly_string, &realm, "get"sv);

    auto native_outer_height_setter = JS::NativeFunction::create(realm, outer_height_setter, 1, "outerHeight"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("outerHeight"_utf16_fly_string, native_outer_height_getter, native_outer_height_setter, default_attributes);

    auto native_device_pixel_ratio_getter = JS::NativeFunction::create(realm, device_pixel_ratio_getter, 0, "devicePixelRatio"_utf16_fly_string, &realm, "get"sv);

    auto native_device_pixel_ratio_setter = JS::NativeFunction::create(realm, device_pixel_ratio_setter, 1, "devicePixelRatio"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("devicePixelRatio"_utf16_fly_string, native_device_pixel_ratio_getter, native_device_pixel_ratio_setter, default_attributes);

    if (HTML::is_secure_context(Bindings::principal_host_defined_environment_settings_object(realm))) {
    auto native_cookie_store_getter = JS::NativeFunction::create(realm, cookie_store_getter, 0, "cookieStore"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_cookie_store_setter;

    object.define_direct_accessor("cookieStore"_utf16_fly_string, native_cookie_store_getter, native_cookie_store_setter, default_attributes);

    }
    auto native_external_getter = JS::NativeFunction::create(realm, external_getter, 0, "external"_utf16_fly_string, &realm, "get"sv);

    auto native_external_setter = JS::NativeFunction::create(realm, external_setter, 1, "external"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("external"_utf16_fly_string, native_external_getter, native_external_setter, default_attributes);

    auto native_speech_synthesis_getter = JS::NativeFunction::create(realm, speech_synthesis_getter, 0, "speechSynthesis"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_speech_synthesis_setter;

    object.define_direct_accessor("speechSynthesis"_utf16_fly_string, native_speech_synthesis_getter, native_speech_synthesis_setter, default_attributes);

    auto native_onafterprint_getter = JS::NativeFunction::create(realm, onafterprint_getter, 0, "onafterprint"_utf16_fly_string, &realm, "get"sv);

    auto native_onafterprint_setter = JS::NativeFunction::create(realm, onafterprint_setter, 1, "onafterprint"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onafterprint"_utf16_fly_string, native_onafterprint_getter, native_onafterprint_setter, default_attributes);

    auto native_onbeforeprint_getter = JS::NativeFunction::create(realm, onbeforeprint_getter, 0, "onbeforeprint"_utf16_fly_string, &realm, "get"sv);

    auto native_onbeforeprint_setter = JS::NativeFunction::create(realm, onbeforeprint_setter, 1, "onbeforeprint"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onbeforeprint"_utf16_fly_string, native_onbeforeprint_getter, native_onbeforeprint_setter, default_attributes);

    auto native_onbeforeunload_getter = JS::NativeFunction::create(realm, onbeforeunload_getter, 0, "onbeforeunload"_utf16_fly_string, &realm, "get"sv);

    auto native_onbeforeunload_setter = JS::NativeFunction::create(realm, onbeforeunload_setter, 1, "onbeforeunload"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onbeforeunload"_utf16_fly_string, native_onbeforeunload_getter, native_onbeforeunload_setter, default_attributes);

    auto native_onhashchange_getter = JS::NativeFunction::create(realm, onhashchange_getter, 0, "onhashchange"_utf16_fly_string, &realm, "get"sv);

    auto native_onhashchange_setter = JS::NativeFunction::create(realm, onhashchange_setter, 1, "onhashchange"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onhashchange"_utf16_fly_string, native_onhashchange_getter, native_onhashchange_setter, default_attributes);

    auto native_onlanguagechange_getter = JS::NativeFunction::create(realm, onlanguagechange_getter, 0, "onlanguagechange"_utf16_fly_string, &realm, "get"sv);

    auto native_onlanguagechange_setter = JS::NativeFunction::create(realm, onlanguagechange_setter, 1, "onlanguagechange"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onlanguagechange"_utf16_fly_string, native_onlanguagechange_getter, native_onlanguagechange_setter, default_attributes);

    auto native_onmessage_getter = JS::NativeFunction::create(realm, onmessage_getter, 0, "onmessage"_utf16_fly_string, &realm, "get"sv);

    auto native_onmessage_setter = JS::NativeFunction::create(realm, onmessage_setter, 1, "onmessage"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onmessage"_utf16_fly_string, native_onmessage_getter, native_onmessage_setter, default_attributes);

    auto native_onmessageerror_getter = JS::NativeFunction::create(realm, onmessageerror_getter, 0, "onmessageerror"_utf16_fly_string, &realm, "get"sv);

    auto native_onmessageerror_setter = JS::NativeFunction::create(realm, onmessageerror_setter, 1, "onmessageerror"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onmessageerror"_utf16_fly_string, native_onmessageerror_getter, native_onmessageerror_setter, default_attributes);

    auto native_onoffline_getter = JS::NativeFunction::create(realm, onoffline_getter, 0, "onoffline"_utf16_fly_string, &realm, "get"sv);

    auto native_onoffline_setter = JS::NativeFunction::create(realm, onoffline_setter, 1, "onoffline"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onoffline"_utf16_fly_string, native_onoffline_getter, native_onoffline_setter, default_attributes);

    auto native_ononline_getter = JS::NativeFunction::create(realm, ononline_getter, 0, "ononline"_utf16_fly_string, &realm, "get"sv);

    auto native_ononline_setter = JS::NativeFunction::create(realm, ononline_setter, 1, "ononline"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("ononline"_utf16_fly_string, native_ononline_getter, native_ononline_setter, default_attributes);

    auto native_onpagehide_getter = JS::NativeFunction::create(realm, onpagehide_getter, 0, "onpagehide"_utf16_fly_string, &realm, "get"sv);

    auto native_onpagehide_setter = JS::NativeFunction::create(realm, onpagehide_setter, 1, "onpagehide"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onpagehide"_utf16_fly_string, native_onpagehide_getter, native_onpagehide_setter, default_attributes);

    auto native_onpagereveal_getter = JS::NativeFunction::create(realm, onpagereveal_getter, 0, "onpagereveal"_utf16_fly_string, &realm, "get"sv);

    auto native_onpagereveal_setter = JS::NativeFunction::create(realm, onpagereveal_setter, 1, "onpagereveal"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onpagereveal"_utf16_fly_string, native_onpagereveal_getter, native_onpagereveal_setter, default_attributes);

    auto native_onpageshow_getter = JS::NativeFunction::create(realm, onpageshow_getter, 0, "onpageshow"_utf16_fly_string, &realm, "get"sv);

    auto native_onpageshow_setter = JS::NativeFunction::create(realm, onpageshow_setter, 1, "onpageshow"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onpageshow"_utf16_fly_string, native_onpageshow_getter, native_onpageshow_setter, default_attributes);

    auto native_onpageswap_getter = JS::NativeFunction::create(realm, onpageswap_getter, 0, "onpageswap"_utf16_fly_string, &realm, "get"sv);

    auto native_onpageswap_setter = JS::NativeFunction::create(realm, onpageswap_setter, 1, "onpageswap"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onpageswap"_utf16_fly_string, native_onpageswap_getter, native_onpageswap_setter, default_attributes);

    auto native_onpopstate_getter = JS::NativeFunction::create(realm, onpopstate_getter, 0, "onpopstate"_utf16_fly_string, &realm, "get"sv);

    auto native_onpopstate_setter = JS::NativeFunction::create(realm, onpopstate_setter, 1, "onpopstate"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onpopstate"_utf16_fly_string, native_onpopstate_getter, native_onpopstate_setter, default_attributes);

    auto native_onrejectionhandled_getter = JS::NativeFunction::create(realm, onrejectionhandled_getter, 0, "onrejectionhandled"_utf16_fly_string, &realm, "get"sv);

    auto native_onrejectionhandled_setter = JS::NativeFunction::create(realm, onrejectionhandled_setter, 1, "onrejectionhandled"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onrejectionhandled"_utf16_fly_string, native_onrejectionhandled_getter, native_onrejectionhandled_setter, default_attributes);

    auto native_onstorage_getter = JS::NativeFunction::create(realm, onstorage_getter, 0, "onstorage"_utf16_fly_string, &realm, "get"sv);

    auto native_onstorage_setter = JS::NativeFunction::create(realm, onstorage_setter, 1, "onstorage"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onstorage"_utf16_fly_string, native_onstorage_getter, native_onstorage_setter, default_attributes);

    auto native_onunhandledrejection_getter = JS::NativeFunction::create(realm, onunhandledrejection_getter, 0, "onunhandledrejection"_utf16_fly_string, &realm, "get"sv);

    auto native_onunhandledrejection_setter = JS::NativeFunction::create(realm, onunhandledrejection_setter, 1, "onunhandledrejection"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onunhandledrejection"_utf16_fly_string, native_onunhandledrejection_getter, native_onunhandledrejection_setter, default_attributes);

    auto native_onunload_getter = JS::NativeFunction::create(realm, onunload_getter, 0, "onunload"_utf16_fly_string, &realm, "get"sv);

    auto native_onunload_setter = JS::NativeFunction::create(realm, onunload_setter, 1, "onunload"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onunload"_utf16_fly_string, native_onunload_getter, native_onunload_setter, default_attributes);

    auto native_ongamepadconnected_getter = JS::NativeFunction::create(realm, ongamepadconnected_getter, 0, "ongamepadconnected"_utf16_fly_string, &realm, "get"sv);

    auto native_ongamepadconnected_setter = JS::NativeFunction::create(realm, ongamepadconnected_setter, 1, "ongamepadconnected"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("ongamepadconnected"_utf16_fly_string, native_ongamepadconnected_getter, native_ongamepadconnected_setter, default_attributes);

    auto native_ongamepaddisconnected_getter = JS::NativeFunction::create(realm, ongamepaddisconnected_getter, 0, "ongamepaddisconnected"_utf16_fly_string, &realm, "get"sv);

    auto native_ongamepaddisconnected_setter = JS::NativeFunction::create(realm, ongamepaddisconnected_setter, 1, "ongamepaddisconnected"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("ongamepaddisconnected"_utf16_fly_string, native_ongamepaddisconnected_getter, native_ongamepaddisconnected_setter, default_attributes);

    auto native_origin_getter = JS::NativeFunction::create(realm, origin_getter, 0, "origin"_utf16_fly_string, &realm, "get"sv);

    auto native_origin_setter = JS::NativeFunction::create(realm, origin_setter, 1, "origin"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("origin"_utf16_fly_string, native_origin_getter, native_origin_setter, default_attributes);

    auto native_is_secure_context_getter = JS::NativeFunction::create(realm, is_secure_context_getter, 0, "isSecureContext"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_is_secure_context_setter;

    object.define_direct_accessor("isSecureContext"_utf16_fly_string, native_is_secure_context_getter, native_is_secure_context_setter, default_attributes);

    auto native_cross_origin_isolated_getter = JS::NativeFunction::create(realm, cross_origin_isolated_getter, 0, "crossOriginIsolated"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_cross_origin_isolated_setter;

    object.define_direct_accessor("crossOriginIsolated"_utf16_fly_string, native_cross_origin_isolated_getter, native_cross_origin_isolated_setter, default_attributes);

    auto native_performance_getter = JS::NativeFunction::create(realm, performance_getter, 0, "performance"_utf16_fly_string, &realm, "get"sv);

    auto native_performance_setter = JS::NativeFunction::create(realm, performance_setter, 1, "performance"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("performance"_utf16_fly_string, native_performance_getter, native_performance_setter, default_attributes);

    auto native_indexed_db_getter = JS::NativeFunction::create(realm, indexed_db_getter, 0, "indexedDB"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_indexed_db_setter;

    object.define_direct_accessor("indexedDB"_utf16_fly_string, native_indexed_db_getter, native_indexed_db_setter, default_attributes);

    auto native_crypto_getter = JS::NativeFunction::create(realm, crypto_getter, 0, "crypto"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_crypto_setter;

    object.define_direct_accessor("crypto"_utf16_fly_string, native_crypto_getter, native_crypto_setter, default_attributes);

    if (HTML::UniversalGlobalScopeMixin::expose_experimental_interfaces()) {
    if (HTML::is_secure_context(Bindings::principal_host_defined_environment_settings_object(realm))) {
    auto native_caches_getter = JS::NativeFunction::create(realm, caches_getter, 0, "caches"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_caches_setter;

    object.define_direct_accessor("caches"_utf16_fly_string, native_caches_getter, native_caches_setter, default_attributes);

    }
    }
    auto native_trusted_types_getter = JS::NativeFunction::create(realm, trusted_types_getter, 0, "trustedTypes"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_trusted_types_setter;

    object.define_direct_accessor("trustedTypes"_utf16_fly_string, native_trusted_types_getter, native_trusted_types_setter, default_attributes);

    auto native_local_storage_getter = JS::NativeFunction::create(realm, local_storage_getter, 0, "localStorage"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_local_storage_setter;

    object.define_direct_accessor("localStorage"_utf16_fly_string, native_local_storage_getter, native_local_storage_setter, default_attributes);

    auto native_onabort_getter = JS::NativeFunction::create(realm, onabort_getter, 0, "onabort"_utf16_fly_string, &realm, "get"sv);

    auto native_onabort_setter = JS::NativeFunction::create(realm, onabort_setter, 1, "onabort"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onabort"_utf16_fly_string, native_onabort_getter, native_onabort_setter, default_attributes);

    auto native_onauxclick_getter = JS::NativeFunction::create(realm, onauxclick_getter, 0, "onauxclick"_utf16_fly_string, &realm, "get"sv);

    auto native_onauxclick_setter = JS::NativeFunction::create(realm, onauxclick_setter, 1, "onauxclick"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onauxclick"_utf16_fly_string, native_onauxclick_getter, native_onauxclick_setter, default_attributes);

    auto native_onbeforeinput_getter = JS::NativeFunction::create(realm, onbeforeinput_getter, 0, "onbeforeinput"_utf16_fly_string, &realm, "get"sv);

    auto native_onbeforeinput_setter = JS::NativeFunction::create(realm, onbeforeinput_setter, 1, "onbeforeinput"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onbeforeinput"_utf16_fly_string, native_onbeforeinput_getter, native_onbeforeinput_setter, default_attributes);

    auto native_onbeforematch_getter = JS::NativeFunction::create(realm, onbeforematch_getter, 0, "onbeforematch"_utf16_fly_string, &realm, "get"sv);

    auto native_onbeforematch_setter = JS::NativeFunction::create(realm, onbeforematch_setter, 1, "onbeforematch"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onbeforematch"_utf16_fly_string, native_onbeforematch_getter, native_onbeforematch_setter, default_attributes);

    auto native_onbeforetoggle_getter = JS::NativeFunction::create(realm, onbeforetoggle_getter, 0, "onbeforetoggle"_utf16_fly_string, &realm, "get"sv);

    auto native_onbeforetoggle_setter = JS::NativeFunction::create(realm, onbeforetoggle_setter, 1, "onbeforetoggle"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onbeforetoggle"_utf16_fly_string, native_onbeforetoggle_getter, native_onbeforetoggle_setter, default_attributes);

    auto native_onblur_getter = JS::NativeFunction::create(realm, onblur_getter, 0, "onblur"_utf16_fly_string, &realm, "get"sv);

    auto native_onblur_setter = JS::NativeFunction::create(realm, onblur_setter, 1, "onblur"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onblur"_utf16_fly_string, native_onblur_getter, native_onblur_setter, default_attributes);

    auto native_oncancel_getter = JS::NativeFunction::create(realm, oncancel_getter, 0, "oncancel"_utf16_fly_string, &realm, "get"sv);

    auto native_oncancel_setter = JS::NativeFunction::create(realm, oncancel_setter, 1, "oncancel"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("oncancel"_utf16_fly_string, native_oncancel_getter, native_oncancel_setter, default_attributes);

    auto native_oncanplay_getter = JS::NativeFunction::create(realm, oncanplay_getter, 0, "oncanplay"_utf16_fly_string, &realm, "get"sv);

    auto native_oncanplay_setter = JS::NativeFunction::create(realm, oncanplay_setter, 1, "oncanplay"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("oncanplay"_utf16_fly_string, native_oncanplay_getter, native_oncanplay_setter, default_attributes);

    auto native_oncanplaythrough_getter = JS::NativeFunction::create(realm, oncanplaythrough_getter, 0, "oncanplaythrough"_utf16_fly_string, &realm, "get"sv);

    auto native_oncanplaythrough_setter = JS::NativeFunction::create(realm, oncanplaythrough_setter, 1, "oncanplaythrough"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("oncanplaythrough"_utf16_fly_string, native_oncanplaythrough_getter, native_oncanplaythrough_setter, default_attributes);

    auto native_onchange_getter = JS::NativeFunction::create(realm, onchange_getter, 0, "onchange"_utf16_fly_string, &realm, "get"sv);

    auto native_onchange_setter = JS::NativeFunction::create(realm, onchange_setter, 1, "onchange"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onchange"_utf16_fly_string, native_onchange_getter, native_onchange_setter, default_attributes);

    auto native_onclick_getter = JS::NativeFunction::create(realm, onclick_getter, 0, "onclick"_utf16_fly_string, &realm, "get"sv);

    auto native_onclick_setter = JS::NativeFunction::create(realm, onclick_setter, 1, "onclick"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onclick"_utf16_fly_string, native_onclick_getter, native_onclick_setter, default_attributes);

    auto native_onclose_getter = JS::NativeFunction::create(realm, onclose_getter, 0, "onclose"_utf16_fly_string, &realm, "get"sv);

    auto native_onclose_setter = JS::NativeFunction::create(realm, onclose_setter, 1, "onclose"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onclose"_utf16_fly_string, native_onclose_getter, native_onclose_setter, default_attributes);

    auto native_oncommand_getter = JS::NativeFunction::create(realm, oncommand_getter, 0, "oncommand"_utf16_fly_string, &realm, "get"sv);

    auto native_oncommand_setter = JS::NativeFunction::create(realm, oncommand_setter, 1, "oncommand"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("oncommand"_utf16_fly_string, native_oncommand_getter, native_oncommand_setter, default_attributes);

    auto native_oncontextlost_getter = JS::NativeFunction::create(realm, oncontextlost_getter, 0, "oncontextlost"_utf16_fly_string, &realm, "get"sv);

    auto native_oncontextlost_setter = JS::NativeFunction::create(realm, oncontextlost_setter, 1, "oncontextlost"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("oncontextlost"_utf16_fly_string, native_oncontextlost_getter, native_oncontextlost_setter, default_attributes);

    auto native_oncontextmenu_getter = JS::NativeFunction::create(realm, oncontextmenu_getter, 0, "oncontextmenu"_utf16_fly_string, &realm, "get"sv);

    auto native_oncontextmenu_setter = JS::NativeFunction::create(realm, oncontextmenu_setter, 1, "oncontextmenu"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("oncontextmenu"_utf16_fly_string, native_oncontextmenu_getter, native_oncontextmenu_setter, default_attributes);

    auto native_oncontextrestored_getter = JS::NativeFunction::create(realm, oncontextrestored_getter, 0, "oncontextrestored"_utf16_fly_string, &realm, "get"sv);

    auto native_oncontextrestored_setter = JS::NativeFunction::create(realm, oncontextrestored_setter, 1, "oncontextrestored"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("oncontextrestored"_utf16_fly_string, native_oncontextrestored_getter, native_oncontextrestored_setter, default_attributes);

    auto native_oncopy_getter = JS::NativeFunction::create(realm, oncopy_getter, 0, "oncopy"_utf16_fly_string, &realm, "get"sv);

    auto native_oncopy_setter = JS::NativeFunction::create(realm, oncopy_setter, 1, "oncopy"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("oncopy"_utf16_fly_string, native_oncopy_getter, native_oncopy_setter, default_attributes);

    auto native_oncuechange_getter = JS::NativeFunction::create(realm, oncuechange_getter, 0, "oncuechange"_utf16_fly_string, &realm, "get"sv);

    auto native_oncuechange_setter = JS::NativeFunction::create(realm, oncuechange_setter, 1, "oncuechange"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("oncuechange"_utf16_fly_string, native_oncuechange_getter, native_oncuechange_setter, default_attributes);

    auto native_oncut_getter = JS::NativeFunction::create(realm, oncut_getter, 0, "oncut"_utf16_fly_string, &realm, "get"sv);

    auto native_oncut_setter = JS::NativeFunction::create(realm, oncut_setter, 1, "oncut"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("oncut"_utf16_fly_string, native_oncut_getter, native_oncut_setter, default_attributes);

    auto native_ondblclick_getter = JS::NativeFunction::create(realm, ondblclick_getter, 0, "ondblclick"_utf16_fly_string, &realm, "get"sv);

    auto native_ondblclick_setter = JS::NativeFunction::create(realm, ondblclick_setter, 1, "ondblclick"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("ondblclick"_utf16_fly_string, native_ondblclick_getter, native_ondblclick_setter, default_attributes);

    auto native_ondrag_getter = JS::NativeFunction::create(realm, ondrag_getter, 0, "ondrag"_utf16_fly_string, &realm, "get"sv);

    auto native_ondrag_setter = JS::NativeFunction::create(realm, ondrag_setter, 1, "ondrag"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("ondrag"_utf16_fly_string, native_ondrag_getter, native_ondrag_setter, default_attributes);

    auto native_ondragend_getter = JS::NativeFunction::create(realm, ondragend_getter, 0, "ondragend"_utf16_fly_string, &realm, "get"sv);

    auto native_ondragend_setter = JS::NativeFunction::create(realm, ondragend_setter, 1, "ondragend"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("ondragend"_utf16_fly_string, native_ondragend_getter, native_ondragend_setter, default_attributes);

    auto native_ondragenter_getter = JS::NativeFunction::create(realm, ondragenter_getter, 0, "ondragenter"_utf16_fly_string, &realm, "get"sv);

    auto native_ondragenter_setter = JS::NativeFunction::create(realm, ondragenter_setter, 1, "ondragenter"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("ondragenter"_utf16_fly_string, native_ondragenter_getter, native_ondragenter_setter, default_attributes);

    auto native_ondragleave_getter = JS::NativeFunction::create(realm, ondragleave_getter, 0, "ondragleave"_utf16_fly_string, &realm, "get"sv);

    auto native_ondragleave_setter = JS::NativeFunction::create(realm, ondragleave_setter, 1, "ondragleave"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("ondragleave"_utf16_fly_string, native_ondragleave_getter, native_ondragleave_setter, default_attributes);

    auto native_ondragover_getter = JS::NativeFunction::create(realm, ondragover_getter, 0, "ondragover"_utf16_fly_string, &realm, "get"sv);

    auto native_ondragover_setter = JS::NativeFunction::create(realm, ondragover_setter, 1, "ondragover"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("ondragover"_utf16_fly_string, native_ondragover_getter, native_ondragover_setter, default_attributes);

    auto native_ondragstart_getter = JS::NativeFunction::create(realm, ondragstart_getter, 0, "ondragstart"_utf16_fly_string, &realm, "get"sv);

    auto native_ondragstart_setter = JS::NativeFunction::create(realm, ondragstart_setter, 1, "ondragstart"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("ondragstart"_utf16_fly_string, native_ondragstart_getter, native_ondragstart_setter, default_attributes);

    auto native_ondrop_getter = JS::NativeFunction::create(realm, ondrop_getter, 0, "ondrop"_utf16_fly_string, &realm, "get"sv);

    auto native_ondrop_setter = JS::NativeFunction::create(realm, ondrop_setter, 1, "ondrop"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("ondrop"_utf16_fly_string, native_ondrop_getter, native_ondrop_setter, default_attributes);

    auto native_ondurationchange_getter = JS::NativeFunction::create(realm, ondurationchange_getter, 0, "ondurationchange"_utf16_fly_string, &realm, "get"sv);

    auto native_ondurationchange_setter = JS::NativeFunction::create(realm, ondurationchange_setter, 1, "ondurationchange"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("ondurationchange"_utf16_fly_string, native_ondurationchange_getter, native_ondurationchange_setter, default_attributes);

    auto native_onemptied_getter = JS::NativeFunction::create(realm, onemptied_getter, 0, "onemptied"_utf16_fly_string, &realm, "get"sv);

    auto native_onemptied_setter = JS::NativeFunction::create(realm, onemptied_setter, 1, "onemptied"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onemptied"_utf16_fly_string, native_onemptied_getter, native_onemptied_setter, default_attributes);

    auto native_onended_getter = JS::NativeFunction::create(realm, onended_getter, 0, "onended"_utf16_fly_string, &realm, "get"sv);

    auto native_onended_setter = JS::NativeFunction::create(realm, onended_setter, 1, "onended"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onended"_utf16_fly_string, native_onended_getter, native_onended_setter, default_attributes);

    auto native_onerror_getter = JS::NativeFunction::create(realm, onerror_getter, 0, "onerror"_utf16_fly_string, &realm, "get"sv);

    auto native_onerror_setter = JS::NativeFunction::create(realm, onerror_setter, 1, "onerror"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onerror"_utf16_fly_string, native_onerror_getter, native_onerror_setter, default_attributes);

    auto native_onfocus_getter = JS::NativeFunction::create(realm, onfocus_getter, 0, "onfocus"_utf16_fly_string, &realm, "get"sv);

    auto native_onfocus_setter = JS::NativeFunction::create(realm, onfocus_setter, 1, "onfocus"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onfocus"_utf16_fly_string, native_onfocus_getter, native_onfocus_setter, default_attributes);

    auto native_onfocusin_getter = JS::NativeFunction::create(realm, onfocusin_getter, 0, "onfocusin"_utf16_fly_string, &realm, "get"sv);

    auto native_onfocusin_setter = JS::NativeFunction::create(realm, onfocusin_setter, 1, "onfocusin"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onfocusin"_utf16_fly_string, native_onfocusin_getter, native_onfocusin_setter, default_attributes);

    auto native_onfocusout_getter = JS::NativeFunction::create(realm, onfocusout_getter, 0, "onfocusout"_utf16_fly_string, &realm, "get"sv);

    auto native_onfocusout_setter = JS::NativeFunction::create(realm, onfocusout_setter, 1, "onfocusout"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onfocusout"_utf16_fly_string, native_onfocusout_getter, native_onfocusout_setter, default_attributes);

    auto native_onformdata_getter = JS::NativeFunction::create(realm, onformdata_getter, 0, "onformdata"_utf16_fly_string, &realm, "get"sv);

    auto native_onformdata_setter = JS::NativeFunction::create(realm, onformdata_setter, 1, "onformdata"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onformdata"_utf16_fly_string, native_onformdata_getter, native_onformdata_setter, default_attributes);

    auto native_oninput_getter = JS::NativeFunction::create(realm, oninput_getter, 0, "oninput"_utf16_fly_string, &realm, "get"sv);

    auto native_oninput_setter = JS::NativeFunction::create(realm, oninput_setter, 1, "oninput"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("oninput"_utf16_fly_string, native_oninput_getter, native_oninput_setter, default_attributes);

    auto native_oninvalid_getter = JS::NativeFunction::create(realm, oninvalid_getter, 0, "oninvalid"_utf16_fly_string, &realm, "get"sv);

    auto native_oninvalid_setter = JS::NativeFunction::create(realm, oninvalid_setter, 1, "oninvalid"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("oninvalid"_utf16_fly_string, native_oninvalid_getter, native_oninvalid_setter, default_attributes);

    auto native_onkeydown_getter = JS::NativeFunction::create(realm, onkeydown_getter, 0, "onkeydown"_utf16_fly_string, &realm, "get"sv);

    auto native_onkeydown_setter = JS::NativeFunction::create(realm, onkeydown_setter, 1, "onkeydown"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onkeydown"_utf16_fly_string, native_onkeydown_getter, native_onkeydown_setter, default_attributes);

    auto native_onkeypress_getter = JS::NativeFunction::create(realm, onkeypress_getter, 0, "onkeypress"_utf16_fly_string, &realm, "get"sv);

    auto native_onkeypress_setter = JS::NativeFunction::create(realm, onkeypress_setter, 1, "onkeypress"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onkeypress"_utf16_fly_string, native_onkeypress_getter, native_onkeypress_setter, default_attributes);

    auto native_onkeyup_getter = JS::NativeFunction::create(realm, onkeyup_getter, 0, "onkeyup"_utf16_fly_string, &realm, "get"sv);

    auto native_onkeyup_setter = JS::NativeFunction::create(realm, onkeyup_setter, 1, "onkeyup"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onkeyup"_utf16_fly_string, native_onkeyup_getter, native_onkeyup_setter, default_attributes);

    auto native_onload_getter = JS::NativeFunction::create(realm, onload_getter, 0, "onload"_utf16_fly_string, &realm, "get"sv);

    auto native_onload_setter = JS::NativeFunction::create(realm, onload_setter, 1, "onload"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onload"_utf16_fly_string, native_onload_getter, native_onload_setter, default_attributes);

    auto native_onloadeddata_getter = JS::NativeFunction::create(realm, onloadeddata_getter, 0, "onloadeddata"_utf16_fly_string, &realm, "get"sv);

    auto native_onloadeddata_setter = JS::NativeFunction::create(realm, onloadeddata_setter, 1, "onloadeddata"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onloadeddata"_utf16_fly_string, native_onloadeddata_getter, native_onloadeddata_setter, default_attributes);

    auto native_onloadedmetadata_getter = JS::NativeFunction::create(realm, onloadedmetadata_getter, 0, "onloadedmetadata"_utf16_fly_string, &realm, "get"sv);

    auto native_onloadedmetadata_setter = JS::NativeFunction::create(realm, onloadedmetadata_setter, 1, "onloadedmetadata"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onloadedmetadata"_utf16_fly_string, native_onloadedmetadata_getter, native_onloadedmetadata_setter, default_attributes);

    auto native_onloadstart_getter = JS::NativeFunction::create(realm, onloadstart_getter, 0, "onloadstart"_utf16_fly_string, &realm, "get"sv);

    auto native_onloadstart_setter = JS::NativeFunction::create(realm, onloadstart_setter, 1, "onloadstart"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onloadstart"_utf16_fly_string, native_onloadstart_getter, native_onloadstart_setter, default_attributes);

    auto native_onmousedown_getter = JS::NativeFunction::create(realm, onmousedown_getter, 0, "onmousedown"_utf16_fly_string, &realm, "get"sv);

    auto native_onmousedown_setter = JS::NativeFunction::create(realm, onmousedown_setter, 1, "onmousedown"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onmousedown"_utf16_fly_string, native_onmousedown_getter, native_onmousedown_setter, default_attributes);

    auto native_onmouseenter_getter = JS::NativeFunction::create(realm, onmouseenter_getter, 0, "onmouseenter"_utf16_fly_string, &realm, "get"sv);

    auto native_onmouseenter_setter = JS::NativeFunction::create(realm, onmouseenter_setter, 1, "onmouseenter"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onmouseenter"_utf16_fly_string, native_onmouseenter_getter, native_onmouseenter_setter, default_attributes);

    auto native_onmouseleave_getter = JS::NativeFunction::create(realm, onmouseleave_getter, 0, "onmouseleave"_utf16_fly_string, &realm, "get"sv);

    auto native_onmouseleave_setter = JS::NativeFunction::create(realm, onmouseleave_setter, 1, "onmouseleave"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onmouseleave"_utf16_fly_string, native_onmouseleave_getter, native_onmouseleave_setter, default_attributes);

    auto native_onmousemove_getter = JS::NativeFunction::create(realm, onmousemove_getter, 0, "onmousemove"_utf16_fly_string, &realm, "get"sv);

    auto native_onmousemove_setter = JS::NativeFunction::create(realm, onmousemove_setter, 1, "onmousemove"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onmousemove"_utf16_fly_string, native_onmousemove_getter, native_onmousemove_setter, default_attributes);

    auto native_onmouseout_getter = JS::NativeFunction::create(realm, onmouseout_getter, 0, "onmouseout"_utf16_fly_string, &realm, "get"sv);

    auto native_onmouseout_setter = JS::NativeFunction::create(realm, onmouseout_setter, 1, "onmouseout"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onmouseout"_utf16_fly_string, native_onmouseout_getter, native_onmouseout_setter, default_attributes);

    auto native_onmouseover_getter = JS::NativeFunction::create(realm, onmouseover_getter, 0, "onmouseover"_utf16_fly_string, &realm, "get"sv);

    auto native_onmouseover_setter = JS::NativeFunction::create(realm, onmouseover_setter, 1, "onmouseover"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onmouseover"_utf16_fly_string, native_onmouseover_getter, native_onmouseover_setter, default_attributes);

    auto native_onmouseup_getter = JS::NativeFunction::create(realm, onmouseup_getter, 0, "onmouseup"_utf16_fly_string, &realm, "get"sv);

    auto native_onmouseup_setter = JS::NativeFunction::create(realm, onmouseup_setter, 1, "onmouseup"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onmouseup"_utf16_fly_string, native_onmouseup_getter, native_onmouseup_setter, default_attributes);

    auto native_onpaste_getter = JS::NativeFunction::create(realm, onpaste_getter, 0, "onpaste"_utf16_fly_string, &realm, "get"sv);

    auto native_onpaste_setter = JS::NativeFunction::create(realm, onpaste_setter, 1, "onpaste"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onpaste"_utf16_fly_string, native_onpaste_getter, native_onpaste_setter, default_attributes);

    auto native_onpause_getter = JS::NativeFunction::create(realm, onpause_getter, 0, "onpause"_utf16_fly_string, &realm, "get"sv);

    auto native_onpause_setter = JS::NativeFunction::create(realm, onpause_setter, 1, "onpause"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onpause"_utf16_fly_string, native_onpause_getter, native_onpause_setter, default_attributes);

    auto native_onplay_getter = JS::NativeFunction::create(realm, onplay_getter, 0, "onplay"_utf16_fly_string, &realm, "get"sv);

    auto native_onplay_setter = JS::NativeFunction::create(realm, onplay_setter, 1, "onplay"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onplay"_utf16_fly_string, native_onplay_getter, native_onplay_setter, default_attributes);

    auto native_onplaying_getter = JS::NativeFunction::create(realm, onplaying_getter, 0, "onplaying"_utf16_fly_string, &realm, "get"sv);

    auto native_onplaying_setter = JS::NativeFunction::create(realm, onplaying_setter, 1, "onplaying"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onplaying"_utf16_fly_string, native_onplaying_getter, native_onplaying_setter, default_attributes);

    auto native_onprogress_getter = JS::NativeFunction::create(realm, onprogress_getter, 0, "onprogress"_utf16_fly_string, &realm, "get"sv);

    auto native_onprogress_setter = JS::NativeFunction::create(realm, onprogress_setter, 1, "onprogress"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onprogress"_utf16_fly_string, native_onprogress_getter, native_onprogress_setter, default_attributes);

    auto native_onratechange_getter = JS::NativeFunction::create(realm, onratechange_getter, 0, "onratechange"_utf16_fly_string, &realm, "get"sv);

    auto native_onratechange_setter = JS::NativeFunction::create(realm, onratechange_setter, 1, "onratechange"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onratechange"_utf16_fly_string, native_onratechange_getter, native_onratechange_setter, default_attributes);

    auto native_onreset_getter = JS::NativeFunction::create(realm, onreset_getter, 0, "onreset"_utf16_fly_string, &realm, "get"sv);

    auto native_onreset_setter = JS::NativeFunction::create(realm, onreset_setter, 1, "onreset"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onreset"_utf16_fly_string, native_onreset_getter, native_onreset_setter, default_attributes);

    auto native_onresize_getter = JS::NativeFunction::create(realm, onresize_getter, 0, "onresize"_utf16_fly_string, &realm, "get"sv);

    auto native_onresize_setter = JS::NativeFunction::create(realm, onresize_setter, 1, "onresize"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onresize"_utf16_fly_string, native_onresize_getter, native_onresize_setter, default_attributes);

    auto native_onscroll_getter = JS::NativeFunction::create(realm, onscroll_getter, 0, "onscroll"_utf16_fly_string, &realm, "get"sv);

    auto native_onscroll_setter = JS::NativeFunction::create(realm, onscroll_setter, 1, "onscroll"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onscroll"_utf16_fly_string, native_onscroll_getter, native_onscroll_setter, default_attributes);

    auto native_onscrollend_getter = JS::NativeFunction::create(realm, onscrollend_getter, 0, "onscrollend"_utf16_fly_string, &realm, "get"sv);

    auto native_onscrollend_setter = JS::NativeFunction::create(realm, onscrollend_setter, 1, "onscrollend"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onscrollend"_utf16_fly_string, native_onscrollend_getter, native_onscrollend_setter, default_attributes);

    auto native_onsecuritypolicyviolation_getter = JS::NativeFunction::create(realm, onsecuritypolicyviolation_getter, 0, "onsecuritypolicyviolation"_utf16_fly_string, &realm, "get"sv);

    auto native_onsecuritypolicyviolation_setter = JS::NativeFunction::create(realm, onsecuritypolicyviolation_setter, 1, "onsecuritypolicyviolation"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onsecuritypolicyviolation"_utf16_fly_string, native_onsecuritypolicyviolation_getter, native_onsecuritypolicyviolation_setter, default_attributes);

    auto native_onseeked_getter = JS::NativeFunction::create(realm, onseeked_getter, 0, "onseeked"_utf16_fly_string, &realm, "get"sv);

    auto native_onseeked_setter = JS::NativeFunction::create(realm, onseeked_setter, 1, "onseeked"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onseeked"_utf16_fly_string, native_onseeked_getter, native_onseeked_setter, default_attributes);

    auto native_onseeking_getter = JS::NativeFunction::create(realm, onseeking_getter, 0, "onseeking"_utf16_fly_string, &realm, "get"sv);

    auto native_onseeking_setter = JS::NativeFunction::create(realm, onseeking_setter, 1, "onseeking"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onseeking"_utf16_fly_string, native_onseeking_getter, native_onseeking_setter, default_attributes);

    auto native_onselect_getter = JS::NativeFunction::create(realm, onselect_getter, 0, "onselect"_utf16_fly_string, &realm, "get"sv);

    auto native_onselect_setter = JS::NativeFunction::create(realm, onselect_setter, 1, "onselect"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onselect"_utf16_fly_string, native_onselect_getter, native_onselect_setter, default_attributes);

    auto native_onselectionchange_getter = JS::NativeFunction::create(realm, onselectionchange_getter, 0, "onselectionchange"_utf16_fly_string, &realm, "get"sv);

    auto native_onselectionchange_setter = JS::NativeFunction::create(realm, onselectionchange_setter, 1, "onselectionchange"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onselectionchange"_utf16_fly_string, native_onselectionchange_getter, native_onselectionchange_setter, default_attributes);

    auto native_onslotchange_getter = JS::NativeFunction::create(realm, onslotchange_getter, 0, "onslotchange"_utf16_fly_string, &realm, "get"sv);

    auto native_onslotchange_setter = JS::NativeFunction::create(realm, onslotchange_setter, 1, "onslotchange"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onslotchange"_utf16_fly_string, native_onslotchange_getter, native_onslotchange_setter, default_attributes);

    auto native_onstalled_getter = JS::NativeFunction::create(realm, onstalled_getter, 0, "onstalled"_utf16_fly_string, &realm, "get"sv);

    auto native_onstalled_setter = JS::NativeFunction::create(realm, onstalled_setter, 1, "onstalled"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onstalled"_utf16_fly_string, native_onstalled_getter, native_onstalled_setter, default_attributes);

    auto native_onsubmit_getter = JS::NativeFunction::create(realm, onsubmit_getter, 0, "onsubmit"_utf16_fly_string, &realm, "get"sv);

    auto native_onsubmit_setter = JS::NativeFunction::create(realm, onsubmit_setter, 1, "onsubmit"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onsubmit"_utf16_fly_string, native_onsubmit_getter, native_onsubmit_setter, default_attributes);

    auto native_onsuspend_getter = JS::NativeFunction::create(realm, onsuspend_getter, 0, "onsuspend"_utf16_fly_string, &realm, "get"sv);

    auto native_onsuspend_setter = JS::NativeFunction::create(realm, onsuspend_setter, 1, "onsuspend"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onsuspend"_utf16_fly_string, native_onsuspend_getter, native_onsuspend_setter, default_attributes);

    auto native_ontimeupdate_getter = JS::NativeFunction::create(realm, ontimeupdate_getter, 0, "ontimeupdate"_utf16_fly_string, &realm, "get"sv);

    auto native_ontimeupdate_setter = JS::NativeFunction::create(realm, ontimeupdate_setter, 1, "ontimeupdate"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("ontimeupdate"_utf16_fly_string, native_ontimeupdate_getter, native_ontimeupdate_setter, default_attributes);

    auto native_ontoggle_getter = JS::NativeFunction::create(realm, ontoggle_getter, 0, "ontoggle"_utf16_fly_string, &realm, "get"sv);

    auto native_ontoggle_setter = JS::NativeFunction::create(realm, ontoggle_setter, 1, "ontoggle"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("ontoggle"_utf16_fly_string, native_ontoggle_getter, native_ontoggle_setter, default_attributes);

    auto native_onvolumechange_getter = JS::NativeFunction::create(realm, onvolumechange_getter, 0, "onvolumechange"_utf16_fly_string, &realm, "get"sv);

    auto native_onvolumechange_setter = JS::NativeFunction::create(realm, onvolumechange_setter, 1, "onvolumechange"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onvolumechange"_utf16_fly_string, native_onvolumechange_getter, native_onvolumechange_setter, default_attributes);

    auto native_onwaiting_getter = JS::NativeFunction::create(realm, onwaiting_getter, 0, "onwaiting"_utf16_fly_string, &realm, "get"sv);

    auto native_onwaiting_setter = JS::NativeFunction::create(realm, onwaiting_setter, 1, "onwaiting"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onwaiting"_utf16_fly_string, native_onwaiting_getter, native_onwaiting_setter, default_attributes);

    auto native_onwebkitanimationend_getter = JS::NativeFunction::create(realm, onwebkitanimationend_getter, 0, "onwebkitanimationend"_utf16_fly_string, &realm, "get"sv);

    auto native_onwebkitanimationend_setter = JS::NativeFunction::create(realm, onwebkitanimationend_setter, 1, "onwebkitanimationend"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onwebkitanimationend"_utf16_fly_string, native_onwebkitanimationend_getter, native_onwebkitanimationend_setter, default_attributes);

    auto native_onwebkitanimationiteration_getter = JS::NativeFunction::create(realm, onwebkitanimationiteration_getter, 0, "onwebkitanimationiteration"_utf16_fly_string, &realm, "get"sv);

    auto native_onwebkitanimationiteration_setter = JS::NativeFunction::create(realm, onwebkitanimationiteration_setter, 1, "onwebkitanimationiteration"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onwebkitanimationiteration"_utf16_fly_string, native_onwebkitanimationiteration_getter, native_onwebkitanimationiteration_setter, default_attributes);

    auto native_onwebkitanimationstart_getter = JS::NativeFunction::create(realm, onwebkitanimationstart_getter, 0, "onwebkitanimationstart"_utf16_fly_string, &realm, "get"sv);

    auto native_onwebkitanimationstart_setter = JS::NativeFunction::create(realm, onwebkitanimationstart_setter, 1, "onwebkitanimationstart"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onwebkitanimationstart"_utf16_fly_string, native_onwebkitanimationstart_getter, native_onwebkitanimationstart_setter, default_attributes);

    auto native_onwebkittransitionend_getter = JS::NativeFunction::create(realm, onwebkittransitionend_getter, 0, "onwebkittransitionend"_utf16_fly_string, &realm, "get"sv);

    auto native_onwebkittransitionend_setter = JS::NativeFunction::create(realm, onwebkittransitionend_setter, 1, "onwebkittransitionend"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onwebkittransitionend"_utf16_fly_string, native_onwebkittransitionend_getter, native_onwebkittransitionend_setter, default_attributes);

    auto native_onwheel_getter = JS::NativeFunction::create(realm, onwheel_getter, 0, "onwheel"_utf16_fly_string, &realm, "get"sv);

    auto native_onwheel_setter = JS::NativeFunction::create(realm, onwheel_setter, 1, "onwheel"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onwheel"_utf16_fly_string, native_onwheel_getter, native_onwheel_setter, default_attributes);

    auto native_onanimationcancel_getter = JS::NativeFunction::create(realm, onanimationcancel_getter, 0, "onanimationcancel"_utf16_fly_string, &realm, "get"sv);

    auto native_onanimationcancel_setter = JS::NativeFunction::create(realm, onanimationcancel_setter, 1, "onanimationcancel"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onanimationcancel"_utf16_fly_string, native_onanimationcancel_getter, native_onanimationcancel_setter, default_attributes);

    auto native_onanimationend_getter = JS::NativeFunction::create(realm, onanimationend_getter, 0, "onanimationend"_utf16_fly_string, &realm, "get"sv);

    auto native_onanimationend_setter = JS::NativeFunction::create(realm, onanimationend_setter, 1, "onanimationend"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onanimationend"_utf16_fly_string, native_onanimationend_getter, native_onanimationend_setter, default_attributes);

    auto native_onanimationiteration_getter = JS::NativeFunction::create(realm, onanimationiteration_getter, 0, "onanimationiteration"_utf16_fly_string, &realm, "get"sv);

    auto native_onanimationiteration_setter = JS::NativeFunction::create(realm, onanimationiteration_setter, 1, "onanimationiteration"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onanimationiteration"_utf16_fly_string, native_onanimationiteration_getter, native_onanimationiteration_setter, default_attributes);

    auto native_onanimationstart_getter = JS::NativeFunction::create(realm, onanimationstart_getter, 0, "onanimationstart"_utf16_fly_string, &realm, "get"sv);

    auto native_onanimationstart_setter = JS::NativeFunction::create(realm, onanimationstart_setter, 1, "onanimationstart"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onanimationstart"_utf16_fly_string, native_onanimationstart_getter, native_onanimationstart_setter, default_attributes);

    auto native_onpointerover_getter = JS::NativeFunction::create(realm, onpointerover_getter, 0, "onpointerover"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointerover_setter = JS::NativeFunction::create(realm, onpointerover_setter, 1, "onpointerover"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onpointerover"_utf16_fly_string, native_onpointerover_getter, native_onpointerover_setter, default_attributes);

    auto native_onpointerenter_getter = JS::NativeFunction::create(realm, onpointerenter_getter, 0, "onpointerenter"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointerenter_setter = JS::NativeFunction::create(realm, onpointerenter_setter, 1, "onpointerenter"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onpointerenter"_utf16_fly_string, native_onpointerenter_getter, native_onpointerenter_setter, default_attributes);

    auto native_onpointerdown_getter = JS::NativeFunction::create(realm, onpointerdown_getter, 0, "onpointerdown"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointerdown_setter = JS::NativeFunction::create(realm, onpointerdown_setter, 1, "onpointerdown"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onpointerdown"_utf16_fly_string, native_onpointerdown_getter, native_onpointerdown_setter, default_attributes);

    auto native_onpointermove_getter = JS::NativeFunction::create(realm, onpointermove_getter, 0, "onpointermove"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointermove_setter = JS::NativeFunction::create(realm, onpointermove_setter, 1, "onpointermove"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onpointermove"_utf16_fly_string, native_onpointermove_getter, native_onpointermove_setter, default_attributes);

    if (HTML::is_secure_context(Bindings::principal_host_defined_environment_settings_object(realm))) {
    auto native_onpointerrawupdate_getter = JS::NativeFunction::create(realm, onpointerrawupdate_getter, 0, "onpointerrawupdate"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointerrawupdate_setter = JS::NativeFunction::create(realm, onpointerrawupdate_setter, 1, "onpointerrawupdate"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onpointerrawupdate"_utf16_fly_string, native_onpointerrawupdate_getter, native_onpointerrawupdate_setter, default_attributes);

    }
    auto native_onpointerup_getter = JS::NativeFunction::create(realm, onpointerup_getter, 0, "onpointerup"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointerup_setter = JS::NativeFunction::create(realm, onpointerup_setter, 1, "onpointerup"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onpointerup"_utf16_fly_string, native_onpointerup_getter, native_onpointerup_setter, default_attributes);

    auto native_onpointercancel_getter = JS::NativeFunction::create(realm, onpointercancel_getter, 0, "onpointercancel"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointercancel_setter = JS::NativeFunction::create(realm, onpointercancel_setter, 1, "onpointercancel"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onpointercancel"_utf16_fly_string, native_onpointercancel_getter, native_onpointercancel_setter, default_attributes);

    auto native_onpointerout_getter = JS::NativeFunction::create(realm, onpointerout_getter, 0, "onpointerout"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointerout_setter = JS::NativeFunction::create(realm, onpointerout_setter, 1, "onpointerout"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onpointerout"_utf16_fly_string, native_onpointerout_getter, native_onpointerout_setter, default_attributes);

    auto native_onpointerleave_getter = JS::NativeFunction::create(realm, onpointerleave_getter, 0, "onpointerleave"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointerleave_setter = JS::NativeFunction::create(realm, onpointerleave_setter, 1, "onpointerleave"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onpointerleave"_utf16_fly_string, native_onpointerleave_getter, native_onpointerleave_setter, default_attributes);

    auto native_ongotpointercapture_getter = JS::NativeFunction::create(realm, ongotpointercapture_getter, 0, "ongotpointercapture"_utf16_fly_string, &realm, "get"sv);

    auto native_ongotpointercapture_setter = JS::NativeFunction::create(realm, ongotpointercapture_setter, 1, "ongotpointercapture"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("ongotpointercapture"_utf16_fly_string, native_ongotpointercapture_getter, native_ongotpointercapture_setter, default_attributes);

    auto native_onlostpointercapture_getter = JS::NativeFunction::create(realm, onlostpointercapture_getter, 0, "onlostpointercapture"_utf16_fly_string, &realm, "get"sv);

    auto native_onlostpointercapture_setter = JS::NativeFunction::create(realm, onlostpointercapture_setter, 1, "onlostpointercapture"_utf16_fly_string, &realm, "set"sv);

    object.define_direct_accessor("onlostpointercapture"_utf16_fly_string, native_onlostpointercapture_getter, native_onlostpointercapture_setter, default_attributes);

    auto native_session_storage_getter = JS::NativeFunction::create(realm, session_storage_getter, 0, "sessionStorage"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_session_storage_setter;

    object.define_direct_accessor("sessionStorage"_utf16_fly_string, native_session_storage_getter, native_session_storage_setter, default_attributes);

    object.define_native_function(realm, "close"_utf16_fly_string, close, 0, default_attributes);

    object.define_native_function(realm, "stop"_utf16_fly_string, stop, 0, default_attributes);

    object.define_native_function(realm, "focus"_utf16_fly_string, focus, 0, default_attributes);

    object.define_native_function(realm, "blur"_utf16_fly_string, blur, 0, default_attributes);

    object.define_native_function(realm, "open"_utf16_fly_string, open, 0, default_attributes);

    object.define_native_function(realm, "alert"_utf16_fly_string, alert, 0, default_attributes);

    object.define_native_function(realm, "confirm"_utf16_fly_string, confirm, 0, default_attributes);

    object.define_native_function(realm, "prompt"_utf16_fly_string, prompt, 0, default_attributes);

    object.define_native_function(realm, "postMessage"_utf16_fly_string, post_message, 1, default_attributes);

    object.define_native_function(realm, "getComputedStyle"_utf16_fly_string, get_computed_style, 1, default_attributes);

    object.define_native_function(realm, "matchMedia"_utf16_fly_string, match_media, 1, default_attributes);

    object.define_native_function(realm, "moveTo"_utf16_fly_string, move_to, 2, default_attributes);

    object.define_native_function(realm, "moveBy"_utf16_fly_string, move_by, 2, default_attributes);

    object.define_native_function(realm, "resizeTo"_utf16_fly_string, resize_to, 2, default_attributes);

    object.define_native_function(realm, "resizeBy"_utf16_fly_string, resize_by, 2, default_attributes);

    object.define_native_function(realm, "scroll"_utf16_fly_string, scroll, 0, default_attributes);

    object.define_native_function(realm, "scrollTo"_utf16_fly_string, scroll_to, 0, default_attributes);

    object.define_native_function(realm, "scrollBy"_utf16_fly_string, scroll_by, 0, default_attributes);

    object.define_native_function(realm, "requestIdleCallback"_utf16_fly_string, request_idle_callback, 1, default_attributes);

    object.define_native_function(realm, "cancelIdleCallback"_utf16_fly_string, cancel_idle_callback, 1, default_attributes);

    object.define_native_function(realm, "getSelection"_utf16_fly_string, get_selection, 0, default_attributes);

    object.define_native_function(realm, "find"_utf16_fly_string, find, 0, default_attributes);

    object.define_native_function(realm, "captureEvents"_utf16_fly_string, capture_events, 0, default_attributes);

    object.define_native_function(realm, "releaseEvents"_utf16_fly_string, release_events, 0, default_attributes);

    object.define_native_function(realm, "requestAnimationFrame"_utf16_fly_string, request_animation_frame, 1, default_attributes);

    object.define_native_function(realm, "cancelAnimationFrame"_utf16_fly_string, cancel_animation_frame, 1, default_attributes);

    object.define_native_function(realm, "reportError"_utf16_fly_string, report_error, 1, default_attributes);

    object.define_native_function(realm, "setTimeout"_utf16_fly_string, set_timeout, 1, default_attributes);

    object.define_native_function(realm, "clearTimeout"_utf16_fly_string, clear_timeout, 0, default_attributes);

    object.define_native_function(realm, "setInterval"_utf16_fly_string, set_interval, 1, default_attributes);

    object.define_native_function(realm, "clearInterval"_utf16_fly_string, clear_interval, 0, default_attributes);

    object.define_native_function(realm, "createImageBitmap"_utf16_fly_string, create_image_bitmap, 1, default_attributes);

    object.define_native_function(realm, "fetch"_utf16_fly_string, fetch, 1, default_attributes);

    object.define_native_function(realm, "btoa"_utf16_fly_string, btoa, 1, default_attributes);

    object.define_native_function(realm, "atob"_utf16_fly_string, atob, 1, default_attributes);

    object.define_native_function(realm, "queueMicrotask"_utf16_fly_string, queue_microtask, 1, default_attributes);

    object.define_native_function(realm, "structuredClone"_utf16_fly_string, structured_clone, 1, default_attributes);

    object.define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Window"_string), JS::Attribute::Configurable);

}

void WindowGlobalMixin::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    auto native_window_getter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Window"_utf16_fly_string, "window"_utf16_fly_string, window_getter, UnforgeableKey::Type::Getter);

    GC::Ptr<JS::NativeFunction> native_window_setter;

    object.define_direct_accessor("window"_utf16_fly_string, native_window_getter, native_window_setter, default_attributes);

    auto native_document_getter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Window"_utf16_fly_string, "document"_utf16_fly_string, document_getter, UnforgeableKey::Type::Getter);

    GC::Ptr<JS::NativeFunction> native_document_setter;

    object.define_direct_accessor("document"_utf16_fly_string, native_document_getter, native_document_setter, default_attributes);

    auto native_location_getter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Window"_utf16_fly_string, "location"_utf16_fly_string, location_getter, UnforgeableKey::Type::Getter);

    auto native_location_setter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Window"_utf16_fly_string, "location"_utf16_fly_string, location_setter, UnforgeableKey::Type::Setter);

    object.define_direct_accessor("location"_utf16_fly_string, native_location_getter, native_location_setter, default_attributes);

    auto native_top_getter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Window"_utf16_fly_string, "top"_utf16_fly_string, top_getter, UnforgeableKey::Type::Getter);

    GC::Ptr<JS::NativeFunction> native_top_setter;

    object.define_direct_accessor("top"_utf16_fly_string, native_top_getter, native_top_setter, default_attributes);

}

[[maybe_unused]] static JS::ThrowCompletionOr<HTML::Window*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (is<HTML::Window>(this_object)) {
        return static_cast<HTML::Window*>(this_object);
    }
    if (is<HTML::WindowProxy>(this_object)) {
        return static_cast<HTML::WindowProxy*>(this_object)->window().ptr();
    }

    if (!is<HTML::Window>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Window");
    return static_cast<HTML::Window*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::window_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::window_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->window(); }));

    return &const_cast<WindowProxy&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::self_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::self_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->self(); }));

    return &const_cast<WindowProxy&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::self_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::self_setter");
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
    TRY(impl->create_data_property_or_throw("self"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::document_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::document_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->document(); }));

    return &const_cast<Document&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::name_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::name_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->name(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::name_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::name_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_name(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::status_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::status_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->status(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::status_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::status_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_status(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::closed_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::closed_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->closed(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::location_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::location_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->location(); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::location_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::location_setter");
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
    auto receiver_value = TRY(impl->get("location"_utf16_fly_string));

    // 2. If Q is not an Object, then throw a TypeError.
    if (!receiver_value.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, receiver_value);
    auto& receiver = receiver_value.as_object();

    // 3. Let forwardId be the identifier argument of the [PutForwards] extended attribute.
    auto forward_id = "href"_utf16_fly_string;

    // 4. Perform ? Set(Q, forwardId, V, false).
    TRY(receiver.set(JS::PropertyKey { forward_id, JS::PropertyKey::StringMayBeNumber::No }, value, JS::Object::ShouldThrowExceptions::No));

    // 5. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::history_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::history_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->history(); }));

    return &const_cast<History&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::navigation_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::navigation_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->navigation(); }));

    return &const_cast<Navigation&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::custom_elements_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::custom_elements_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->custom_elements(); }));

    return &const_cast<CustomElementRegistry&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::locationbar_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::locationbar_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->locationbar(); }));

    return &const_cast<BarProp&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::locationbar_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::locationbar_setter");
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
    TRY(impl->create_data_property_or_throw("locationbar"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::menubar_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::menubar_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->menubar(); }));

    return &const_cast<BarProp&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::menubar_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::menubar_setter");
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
    TRY(impl->create_data_property_or_throw("menubar"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::personalbar_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::personalbar_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->personalbar(); }));

    return &const_cast<BarProp&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::personalbar_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::personalbar_setter");
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
    TRY(impl->create_data_property_or_throw("personalbar"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::scrollbars_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::scrollbars_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scrollbars(); }));

    return &const_cast<BarProp&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::scrollbars_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::scrollbars_setter");
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
    TRY(impl->create_data_property_or_throw("scrollbars"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::statusbar_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::statusbar_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->statusbar(); }));

    return &const_cast<BarProp&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::statusbar_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::statusbar_setter");
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
    TRY(impl->create_data_property_or_throw("statusbar"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::toolbar_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::toolbar_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->toolbar(); }));

    return &const_cast<BarProp&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::toolbar_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::toolbar_setter");
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
    TRY(impl->create_data_property_or_throw("toolbar"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::frames_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::frames_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->frames(); }));

    return &const_cast<WindowProxy&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::frames_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::frames_setter");
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
    TRY(impl->create_data_property_or_throw("frames"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::length_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::length_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->length(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::length_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::length_setter");
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
    TRY(impl->create_data_property_or_throw("length"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::top_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::top_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->top(); }));

    if (retval) {

    return &const_cast<WindowProxy&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::opener_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::opener_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->opener(); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::opener_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::opener_setter");
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

    auto cpp_value = value;

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_opener(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::parent_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::parent_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->parent(); }));

    if (retval) {

    return &const_cast<WindowProxy&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::parent_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::parent_setter");
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
    TRY(impl->create_data_property_or_throw("parent"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::frame_element_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::frame_element_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->frame_element(); }));

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::navigator_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::navigator_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->navigator(); }));

    return &const_cast<Navigator&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::client_information_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::client_information_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->navigator(); }));

    return &const_cast<Navigator&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::client_information_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::client_information_setter");
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
    TRY(impl->create_data_property_or_throw("clientInformation"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::event_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::event_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->event(); }));

    return retval.visit(

        [&vm, &realm]([[maybe_unused]] GC::Root<Event> const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return &const_cast<Event&>(*visited_union_value0);

        },

        [&vm, &realm]([[maybe_unused]] Empty const& visited_union_value0) -> JS::Value {
            // These may be unused.
            (void)vm;
            (void)realm;

    return JS::js_undefined();

        }

    );

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::event_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::event_setter");
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
    TRY(impl->create_data_property_or_throw("event"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::screen_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::screen_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->screen(); }));

    return &const_cast<Screen&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::screen_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::screen_setter");
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
    TRY(impl->create_data_property_or_throw("screen"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::visual_viewport_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::visual_viewport_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->visual_viewport(); }));

    if (retval) {

    return &const_cast<VisualViewport&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::visual_viewport_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::visual_viewport_setter");
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
    TRY(impl->create_data_property_or_throw("visualViewport"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::inner_width_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::inner_width_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->inner_width(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::inner_width_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::inner_width_setter");
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
    TRY(impl->create_data_property_or_throw("innerWidth"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::inner_height_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::inner_height_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->inner_height(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::inner_height_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::inner_height_setter");
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
    TRY(impl->create_data_property_or_throw("innerHeight"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::scroll_x_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::scroll_x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll_x(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::scroll_x_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::scroll_x_setter");
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
    TRY(impl->create_data_property_or_throw("scrollX"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::page_x_offset_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::page_x_offset_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll_x(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::page_x_offset_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::page_x_offset_setter");
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
    TRY(impl->create_data_property_or_throw("pageXOffset"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::scroll_y_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::scroll_y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll_y(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::scroll_y_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::scroll_y_setter");
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
    TRY(impl->create_data_property_or_throw("scrollY"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::page_y_offset_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::page_y_offset_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scroll_y(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::page_y_offset_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::page_y_offset_setter");
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
    TRY(impl->create_data_property_or_throw("pageYOffset"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::screen_x_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::screen_x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->screen_x(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::screen_x_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::screen_x_setter");
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
    TRY(impl->create_data_property_or_throw("screenX"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::screen_left_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::screen_left_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->screen_x(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::screen_left_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::screen_left_setter");
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
    TRY(impl->create_data_property_or_throw("screenLeft"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::screen_y_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::screen_y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->screen_y(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::screen_y_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::screen_y_setter");
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
    TRY(impl->create_data_property_or_throw("screenY"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::screen_top_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::screen_top_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->screen_y(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::screen_top_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::screen_top_setter");
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
    TRY(impl->create_data_property_or_throw("screenTop"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::outer_width_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::outer_width_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->outer_width(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::outer_width_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::outer_width_setter");
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
    TRY(impl->create_data_property_or_throw("outerWidth"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::outer_height_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::outer_height_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->outer_height(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::outer_height_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::outer_height_setter");
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
    TRY(impl->create_data_property_or_throw("outerHeight"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::device_pixel_ratio_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::device_pixel_ratio_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->device_pixel_ratio(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::device_pixel_ratio_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::device_pixel_ratio_setter");
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
    TRY(impl->create_data_property_or_throw("devicePixelRatio"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::cookie_store_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::cookie_store_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cookie_store(); }));

    return &const_cast<CookieStore::CookieStore&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::external_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::external_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->external(); }));

    return &const_cast<External&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::external_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::external_setter");
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
    TRY(impl->create_data_property_or_throw("external"_utf16_fly_string, value));

    // 2. Return undefined.
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::speech_synthesis_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::speech_synthesis_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->speech_synthesis(); }));

    return &const_cast<SpeechSynthesis&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onafterprint_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onafterprint_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onafterprint(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onafterprint_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onafterprint_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onafterprint(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onbeforeprint_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onbeforeprint_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onbeforeprint(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onbeforeprint_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onbeforeprint_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onbeforeprint(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onbeforeunload_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onbeforeunload_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onbeforeunload(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onbeforeunload_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onbeforeunload_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onbeforeunload(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onhashchange_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onhashchange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onhashchange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onhashchange_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onhashchange_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onhashchange(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onlanguagechange_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onlanguagechange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onlanguagechange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onlanguagechange_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onlanguagechange_setter");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmessage_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmessage_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmessage(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmessage_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmessage_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onmessage(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmessageerror_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmessageerror_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmessageerror(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmessageerror_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmessageerror_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onmessageerror(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onoffline_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onoffline_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onoffline(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onoffline_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onoffline_setter");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ononline_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ononline_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ononline(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ononline_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ononline_setter");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpagehide_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpagehide_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpagehide(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpagehide_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpagehide_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onpagehide(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpagereveal_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpagereveal_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpagereveal(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpagereveal_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpagereveal_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onpagereveal(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpageshow_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpageshow_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpageshow(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpageshow_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpageshow_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onpageshow(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpageswap_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpageswap_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpageswap(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpageswap_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpageswap_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onpageswap(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpopstate_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpopstate_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpopstate(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpopstate_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpopstate_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onpopstate(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onrejectionhandled_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onrejectionhandled_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onrejectionhandled(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onrejectionhandled_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onrejectionhandled_setter");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onstorage_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onstorage_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onstorage(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onstorage_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onstorage_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onstorage(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onunhandledrejection_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onunhandledrejection_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onunhandledrejection(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onunhandledrejection_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onunhandledrejection_setter");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onunload_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onunload_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onunload(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onunload_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onunload_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onunload(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ongamepadconnected_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ongamepadconnected_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ongamepadconnected(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ongamepadconnected_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ongamepadconnected_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ongamepadconnected(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ongamepaddisconnected_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ongamepaddisconnected_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ongamepaddisconnected(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ongamepaddisconnected_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ongamepaddisconnected_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ongamepaddisconnected(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::origin_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::origin_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->origin(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::origin_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::origin_setter");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::is_secure_context_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::is_secure_context_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_secure_context(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::cross_origin_isolated_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::cross_origin_isolated_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cross_origin_isolated(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::performance_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::performance_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->performance(); }));

    return &const_cast<Performance&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::performance_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::performance_setter");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::indexed_db_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::indexed_db_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->indexed_db(); }));

    return &const_cast<IDBFactory&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::crypto_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::crypto_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->crypto(); }));

    return &const_cast<Crypto::Crypto&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::caches_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::caches_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->caches(); }));

    return &const_cast<CacheStorage&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::trusted_types_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::trusted_types_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->trusted_types(); }));

    return &const_cast<TrustedTypePolicyFactory&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::local_storage_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::local_storage_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->local_storage(); }));

    return &const_cast<Storage&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onabort_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onabort_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onabort(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onabort_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onabort_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onabort(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onauxclick_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onauxclick_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onauxclick(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onauxclick_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onauxclick_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onauxclick(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onbeforeinput_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onbeforeinput_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onbeforeinput(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onbeforeinput_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onbeforeinput_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onbeforeinput(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onbeforematch_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onbeforematch_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onbeforematch(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onbeforematch_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onbeforematch_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onbeforematch(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onbeforetoggle_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onbeforetoggle_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onbeforetoggle(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onbeforetoggle_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onbeforetoggle_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onbeforetoggle(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onblur_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onblur_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onblur(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onblur_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onblur_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onblur(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncancel_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncancel_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncancel(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncancel_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncancel_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_oncancel(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncanplay_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncanplay_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncanplay(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncanplay_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncanplay_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_oncanplay(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncanplaythrough_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncanplaythrough_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncanplaythrough(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncanplaythrough_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncanplaythrough_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_oncanplaythrough(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onchange_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onchange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onchange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onchange_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onchange_setter");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onclick_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onclick_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onclick(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onclick_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onclick_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onclick(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onclose_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onclose_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onclose(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onclose_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onclose_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onclose(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncommand_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncommand_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncommand(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncommand_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncommand_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_oncommand(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncontextlost_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncontextlost_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncontextlost(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncontextlost_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncontextlost_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_oncontextlost(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncontextmenu_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncontextmenu_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncontextmenu(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncontextmenu_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncontextmenu_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_oncontextmenu(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncontextrestored_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncontextrestored_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncontextrestored(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncontextrestored_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncontextrestored_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_oncontextrestored(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncopy_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncopy_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncopy(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncopy_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncopy_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_oncopy(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncuechange_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncuechange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncuechange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncuechange_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncuechange_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_oncuechange(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncut_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncut_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncut(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oncut_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oncut_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_oncut(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondblclick_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondblclick_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondblclick(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondblclick_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondblclick_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ondblclick(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondrag_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondrag_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondrag(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondrag_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondrag_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ondrag(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondragend_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondragend_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondragend(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondragend_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondragend_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ondragend(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondragenter_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondragenter_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondragenter(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondragenter_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondragenter_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ondragenter(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondragleave_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondragleave_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondragleave(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondragleave_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondragleave_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ondragleave(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondragover_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondragover_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondragover(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondragover_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondragover_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ondragover(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondragstart_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondragstart_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondragstart(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondragstart_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondragstart_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ondragstart(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondrop_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondrop_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondrop(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondrop_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondrop_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ondrop(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondurationchange_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondurationchange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondurationchange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ondurationchange_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ondurationchange_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ondurationchange(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onemptied_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onemptied_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onemptied(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onemptied_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onemptied_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onemptied(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onended_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onended_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onended(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onended_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onended_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onended(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onerror_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onerror_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onerror(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onerror_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onerror_setter");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onfocus_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onfocus_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onfocus(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onfocus_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onfocus_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onfocus(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onfocusin_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onfocusin_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onfocusin(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onfocusin_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onfocusin_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onfocusin(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onfocusout_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onfocusout_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onfocusout(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onfocusout_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onfocusout_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onfocusout(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onformdata_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onformdata_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onformdata(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onformdata_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onformdata_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onformdata(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oninput_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oninput_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oninput(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oninput_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oninput_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_oninput(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oninvalid_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oninvalid_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oninvalid(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::oninvalid_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::oninvalid_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_oninvalid(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onkeydown_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onkeydown_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onkeydown(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onkeydown_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onkeydown_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onkeydown(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onkeypress_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onkeypress_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onkeypress(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onkeypress_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onkeypress_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onkeypress(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onkeyup_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onkeyup_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onkeyup(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onkeyup_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onkeyup_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onkeyup(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onload_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onload_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onload(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onload_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onload_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onload(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onloadeddata_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onloadeddata_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onloadeddata(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onloadeddata_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onloadeddata_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onloadeddata(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onloadedmetadata_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onloadedmetadata_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onloadedmetadata(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onloadedmetadata_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onloadedmetadata_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onloadedmetadata(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onloadstart_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onloadstart_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onloadstart(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onloadstart_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onloadstart_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onloadstart(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmousedown_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmousedown_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmousedown(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmousedown_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmousedown_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onmousedown(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmouseenter_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmouseenter_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmouseenter(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmouseenter_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmouseenter_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onmouseenter(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmouseleave_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmouseleave_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmouseleave(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmouseleave_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmouseleave_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onmouseleave(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmousemove_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmousemove_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmousemove(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmousemove_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmousemove_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onmousemove(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmouseout_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmouseout_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmouseout(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmouseout_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmouseout_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onmouseout(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmouseover_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmouseover_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmouseover(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmouseover_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmouseover_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onmouseover(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmouseup_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmouseup_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmouseup(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onmouseup_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onmouseup_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onmouseup(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpaste_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpaste_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpaste(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpaste_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpaste_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onpaste(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpause_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpause_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpause(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpause_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpause_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onpause(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onplay_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onplay_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onplay(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onplay_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onplay_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onplay(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onplaying_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onplaying_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onplaying(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onplaying_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onplaying_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onplaying(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onprogress_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onprogress_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onprogress(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onprogress_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onprogress_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onprogress(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onratechange_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onratechange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onratechange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onratechange_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onratechange_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onratechange(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onreset_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onreset_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onreset(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onreset_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onreset_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onreset(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onresize_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onresize_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onresize(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onresize_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onresize_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onresize(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onscroll_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onscroll_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onscroll(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onscroll_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onscroll_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onscroll(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onscrollend_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onscrollend_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onscrollend(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onscrollend_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onscrollend_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onscrollend(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onsecuritypolicyviolation_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onsecuritypolicyviolation_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onsecuritypolicyviolation(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onsecuritypolicyviolation_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onsecuritypolicyviolation_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onsecuritypolicyviolation(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onseeked_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onseeked_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onseeked(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onseeked_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onseeked_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onseeked(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onseeking_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onseeking_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onseeking(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onseeking_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onseeking_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onseeking(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onselect_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onselect_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onselect(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onselect_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onselect_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onselect(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onselectionchange_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onselectionchange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onselectionchange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onselectionchange_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onselectionchange_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onselectionchange(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onslotchange_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onslotchange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onslotchange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onslotchange_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onslotchange_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onslotchange(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onstalled_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onstalled_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onstalled(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onstalled_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onstalled_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onstalled(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onsubmit_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onsubmit_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onsubmit(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onsubmit_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onsubmit_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onsubmit(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onsuspend_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onsuspend_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onsuspend(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onsuspend_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onsuspend_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onsuspend(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ontimeupdate_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ontimeupdate_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ontimeupdate(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ontimeupdate_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ontimeupdate_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ontimeupdate(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ontoggle_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ontoggle_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ontoggle(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ontoggle_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ontoggle_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ontoggle(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onvolumechange_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onvolumechange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onvolumechange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onvolumechange_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onvolumechange_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onvolumechange(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onwaiting_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onwaiting_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onwaiting(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onwaiting_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onwaiting_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onwaiting(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onwebkitanimationend_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onwebkitanimationend_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onwebkitanimationend(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onwebkitanimationend_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onwebkitanimationend_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onwebkitanimationend(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onwebkitanimationiteration_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onwebkitanimationiteration_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onwebkitanimationiteration(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onwebkitanimationiteration_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onwebkitanimationiteration_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onwebkitanimationiteration(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onwebkitanimationstart_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onwebkitanimationstart_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onwebkitanimationstart(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onwebkitanimationstart_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onwebkitanimationstart_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onwebkitanimationstart(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onwebkittransitionend_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onwebkittransitionend_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onwebkittransitionend(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onwebkittransitionend_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onwebkittransitionend_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onwebkittransitionend(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onwheel_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onwheel_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onwheel(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onwheel_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onwheel_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onwheel(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onanimationcancel_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onanimationcancel_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onanimationcancel(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onanimationcancel_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onanimationcancel_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onanimationcancel(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onanimationend_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onanimationend_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onanimationend(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onanimationend_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onanimationend_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onanimationend(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onanimationiteration_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onanimationiteration_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onanimationiteration(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onanimationiteration_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onanimationiteration_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onanimationiteration(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onanimationstart_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onanimationstart_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onanimationstart(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onanimationstart_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onanimationstart_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onanimationstart(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointerover_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointerover_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointerover(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointerover_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointerover_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onpointerover(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointerenter_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointerenter_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointerenter(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointerenter_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointerenter_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onpointerenter(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointerdown_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointerdown_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointerdown(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointerdown_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointerdown_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onpointerdown(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointermove_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointermove_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointermove(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointermove_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointermove_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onpointermove(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointerrawupdate_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointerrawupdate_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointerrawupdate(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointerrawupdate_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointerrawupdate_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onpointerrawupdate(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointerup_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointerup_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointerup(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointerup_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointerup_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onpointerup(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointercancel_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointercancel_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointercancel(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointercancel_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointercancel_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onpointercancel(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointerout_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointerout_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointerout(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointerout_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointerout_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onpointerout(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointerleave_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointerleave_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointerleave(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onpointerleave_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onpointerleave_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onpointerleave(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ongotpointercapture_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ongotpointercapture_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ongotpointercapture(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::ongotpointercapture_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::ongotpointercapture_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_ongotpointercapture(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onlostpointercapture_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onlostpointercapture_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onlostpointercapture(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::onlostpointercapture_setter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::onlostpointercapture_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onlostpointercapture(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::session_storage_getter)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::session_storage_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->session_storage(); }));

    return &const_cast<Storage&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::close)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::close");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->close(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::stop)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::stop");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->stop(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::focus)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::focus");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->focus(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::blur)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::blur");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->blur(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::open)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::open");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    String url;

    if (!arg0.is_undefined()) {
        if (!false || !arg0.is_null())
            url = TRY(WebIDL::to_usv_string(vm, arg0));
    } else {
        url = ""_string;
    }

    auto arg1 = vm.argument(1);

    String target;

    if (!arg1.is_undefined()) {
        if (!false || !arg1.is_null())
            target = TRY(WebIDL::to_string(vm, arg1));
    } else {
        target = "_blank"_string;
    }

    auto arg2 = vm.argument(2);

    String features;

    if (!arg2.is_undefined()) {
        if (!true || !arg2.is_null())
            features = TRY(WebIDL::to_string(vm, arg2));
    } else {
        features = ""_string;
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->open(url, target, features); }));

    if (retval) {

    return &const_cast<WindowProxy&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::alert0)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::alert0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->alert(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::alert1)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::alert1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    String message;
    if (!false || !arg0.is_null()) {
        message = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->alert(message); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::confirm)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::confirm");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    String message;

    if (!arg0.is_undefined()) {
        if (!false || !arg0.is_null())
            message = TRY(WebIDL::to_string(vm, arg0));
    } else {
        message = ""_string;
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->confirm(message); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::prompt)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::prompt");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    String message;

    if (!arg0.is_undefined()) {
        if (!false || !arg0.is_null())
            message = TRY(WebIDL::to_string(vm, arg0));
    } else {
        message = ""_string;
    }

    auto arg1 = vm.argument(1);

    String default_;

    if (!arg1.is_undefined()) {
        if (!false || !arg1.is_null())
            default_ = TRY(WebIDL::to_string(vm, arg1));
    } else {
        default_ = ""_string;
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->prompt(message, default_); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::post_message0)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::post_message0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    auto message = arg0;

    auto arg1 = vm.argument(1);

    String target_origin;
    if (!false || !arg1.is_null()) {
        target_origin = TRY(WebIDL::to_usv_string(vm, arg1));
    }

    auto arg2 = vm.argument(2);

    Vector<GC::Root<JS::Object>> transfer;

    if (!arg2.is_undefined()) {

    if (!arg2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg2);

    auto arg2_iterator_method0 = TRY(arg2.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg2_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg2);

    auto arg2_iterator1 = TRY(JS::get_iterator_from_method(vm, arg2, *arg2_iterator_method0));

    Vector<GC::Root<JS::Object>> transfer_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, next_item1);
    auto sequence_item1 = GC::make_root(next_item1.as_object());

    transfer_non_optional.append(sequence_item1);
    }

        transfer = move(transfer_non_optional);
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->post_message(message, target_origin, transfer); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::post_message1)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::post_message1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    auto message = arg0;

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WindowPostMessageOptions");

    WindowPostMessageOptions options {};

    auto target_origin_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        target_origin_property_value_0 = TRY(arg1.as_object().get("targetOrigin"_utf16_fly_string));

    String target_origin_value_0;

    if (!target_origin_property_value_0.is_undefined()) {
        if (!false || !target_origin_property_value_0.is_null())
            target_origin_value_0 = TRY(WebIDL::to_usv_string(vm, target_origin_property_value_0));
    } else {
        target_origin_value_0 = "/"_string;
    }

    options.target_origin = target_origin_value_0;

    auto transfer_property_value_1 = JS::js_undefined();
    if (arg1.is_object())
        transfer_property_value_1 = TRY(arg1.as_object().get("transfer"_utf16_fly_string));

    Vector<GC::Root<JS::Object>> transfer_value_1;

    if (!transfer_property_value_1.is_undefined()) {

    if (!transfer_property_value_1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, transfer_property_value_1);

    auto transfer_property_value_1_iterator_method0 = TRY(transfer_property_value_1.get_method(vm, vm.well_known_symbol_iterator()));
    if (!transfer_property_value_1_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, transfer_property_value_1);

    auto transfer_property_value_1_iterator1 = TRY(JS::get_iterator_from_method(vm, transfer_property_value_1, *transfer_property_value_1_iterator_method0));

    Vector<GC::Root<JS::Object>> transfer_value_1_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, transfer_property_value_1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, next_item1);
    auto sequence_item1 = GC::make_root(next_item1.as_object());

    transfer_value_1_non_optional.append(sequence_item1);
    }

        transfer_value_1 = move(transfer_value_1_non_optional);
    }

    options.transfer = transfer_value_1;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->post_message(message, options); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::get_computed_style)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::get_computed_style");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getComputedStyle");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Element>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Element");

    auto& elt = static_cast<Element&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    Optional<String> pseudo_elt;

    if (!arg1.is_undefined()) {
        if (!arg1.is_null())
            pseudo_elt = TRY(WebIDL::to_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_computed_style(elt, pseudo_elt); }));

    return &const_cast<CSSStyleProperties&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::match_media)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::match_media");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "matchMedia");

    auto arg0 = vm.argument(0);

    String query;
    if (!false || !arg0.is_null()) {
        query = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->match_media(query); }));

    return &const_cast<MediaQueryList&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::move_to)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::move_to");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "moveTo", "2");

    auto arg0 = vm.argument(0);

    WebIDL::Long x;

    x = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long y;

    y = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->move_to(x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::move_by)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::move_by");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "moveBy", "2");

    auto arg0 = vm.argument(0);

    WebIDL::Long x;

    x = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long y;

    y = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->move_by(x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::resize_to)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::resize_to");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "resizeTo", "2");

    auto arg0 = vm.argument(0);

    WebIDL::Long x;

    x = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long y;

    y = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->resize_to(x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::resize_by)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::resize_by");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "resizeBy", "2");

    auto arg0 = vm.argument(0);

    WebIDL::Long x;

    x = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long y;

    y = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->resize_by(x, y); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::scroll0)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::scroll0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ScrollToOptions");

    ScrollToOptions options {};

    auto left_property_value_2 = JS::js_undefined();
    if (arg0.is_object())
        left_property_value_2 = TRY(arg0.as_object().get("left"_utf16_fly_string));

    if (!left_property_value_2.is_undefined()) {

    Optional<double> left_value_2;

    if (!left_property_value_2.is_undefined())
        left_value_2 = TRY(left_property_value_2.to_double(vm));


    options.left = left_value_2;

    }

    auto top_property_value_3 = JS::js_undefined();
    if (arg0.is_object())
        top_property_value_3 = TRY(arg0.as_object().get("top"_utf16_fly_string));

    if (!top_property_value_3.is_undefined()) {

    Optional<double> top_value_3;

    if (!top_property_value_3.is_undefined())
        top_value_3 = TRY(top_property_value_3.to_double(vm));


    options.top = top_value_3;

    }

    auto behavior_property_value_4 = JS::js_undefined();
    if (arg0.is_object())
        behavior_property_value_4 = TRY(arg0.as_object().get("behavior"_utf16_fly_string));

    ScrollBehavior behavior_value_4 { ScrollBehavior::Auto };

    if (!behavior_property_value_4.is_undefined()) {

    auto behavior_property_value_4_string = TRY(behavior_property_value_4.to_string(vm));

    if (behavior_property_value_4_string == "auto"sv)
        behavior_value_4 = ScrollBehavior::Auto;

    else if (behavior_property_value_4_string == "instant"sv)
        behavior_value_4 = ScrollBehavior::Instant;

    else if (behavior_property_value_4_string == "smooth"sv)
        behavior_value_4 = ScrollBehavior::Smooth;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, behavior_property_value_4_string, "ScrollBehavior");

    }

    options.behavior = behavior_value_4;

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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::scroll1)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::scroll1");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::scroll_to0)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::scroll_to0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ScrollToOptions");

    ScrollToOptions options {};

    auto left_property_value_5 = JS::js_undefined();
    if (arg0.is_object())
        left_property_value_5 = TRY(arg0.as_object().get("left"_utf16_fly_string));

    if (!left_property_value_5.is_undefined()) {

    Optional<double> left_value_5;

    if (!left_property_value_5.is_undefined())
        left_value_5 = TRY(left_property_value_5.to_double(vm));


    options.left = left_value_5;

    }

    auto top_property_value_6 = JS::js_undefined();
    if (arg0.is_object())
        top_property_value_6 = TRY(arg0.as_object().get("top"_utf16_fly_string));

    if (!top_property_value_6.is_undefined()) {

    Optional<double> top_value_6;

    if (!top_property_value_6.is_undefined())
        top_value_6 = TRY(top_property_value_6.to_double(vm));


    options.top = top_value_6;

    }

    auto behavior_property_value_7 = JS::js_undefined();
    if (arg0.is_object())
        behavior_property_value_7 = TRY(arg0.as_object().get("behavior"_utf16_fly_string));

    ScrollBehavior behavior_value_7 { ScrollBehavior::Auto };

    if (!behavior_property_value_7.is_undefined()) {

    auto behavior_property_value_7_string = TRY(behavior_property_value_7.to_string(vm));

    if (behavior_property_value_7_string == "auto"sv)
        behavior_value_7 = ScrollBehavior::Auto;

    else if (behavior_property_value_7_string == "instant"sv)
        behavior_value_7 = ScrollBehavior::Instant;

    else if (behavior_property_value_7_string == "smooth"sv)
        behavior_value_7 = ScrollBehavior::Smooth;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, behavior_property_value_7_string, "ScrollBehavior");

    }

    options.behavior = behavior_value_7;

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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::scroll_to1)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::scroll_to1");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::scroll_by0)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::scroll_by0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_nullish() && !arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ScrollToOptions");

    ScrollToOptions options {};

    auto left_property_value_8 = JS::js_undefined();
    if (arg0.is_object())
        left_property_value_8 = TRY(arg0.as_object().get("left"_utf16_fly_string));

    if (!left_property_value_8.is_undefined()) {

    Optional<double> left_value_8;

    if (!left_property_value_8.is_undefined())
        left_value_8 = TRY(left_property_value_8.to_double(vm));


    options.left = left_value_8;

    }

    auto top_property_value_9 = JS::js_undefined();
    if (arg0.is_object())
        top_property_value_9 = TRY(arg0.as_object().get("top"_utf16_fly_string));

    if (!top_property_value_9.is_undefined()) {

    Optional<double> top_value_9;

    if (!top_property_value_9.is_undefined())
        top_value_9 = TRY(top_property_value_9.to_double(vm));


    options.top = top_value_9;

    }

    auto behavior_property_value_10 = JS::js_undefined();
    if (arg0.is_object())
        behavior_property_value_10 = TRY(arg0.as_object().get("behavior"_utf16_fly_string));

    ScrollBehavior behavior_value_10 { ScrollBehavior::Auto };

    if (!behavior_property_value_10.is_undefined()) {

    auto behavior_property_value_10_string = TRY(behavior_property_value_10.to_string(vm));

    if (behavior_property_value_10_string == "auto"sv)
        behavior_value_10 = ScrollBehavior::Auto;

    else if (behavior_property_value_10_string == "instant"sv)
        behavior_value_10 = ScrollBehavior::Instant;

    else if (behavior_property_value_10_string == "smooth"sv)
        behavior_value_10 = ScrollBehavior::Smooth;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, behavior_property_value_10_string, "ScrollBehavior");

    }

    options.behavior = behavior_value_10;

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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::scroll_by1)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::scroll_by1");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::request_idle_callback)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::request_idle_callback");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "requestIdleCallback");

    auto arg0 = vm.argument(0);

    if (!arg0.is_function()
)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, arg0);

    auto callback = vm.heap().allocate<WebIDL::CallbackType>(arg0.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    auto arg1 = vm.argument(1);

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "IdleRequestOptions");

    IdleRequestOptions options {};

    auto timeout_property_value_11 = JS::js_undefined();
    if (arg1.is_object())
        timeout_property_value_11 = TRY(arg1.as_object().get("timeout"_utf16_fly_string));

    if (!timeout_property_value_11.is_undefined()) {

    Optional<WebIDL::UnsignedLong> timeout_value_11;

    if (!timeout_property_value_11.is_undefined())

    timeout_value_11 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, timeout_property_value_11, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    options.timeout = timeout_value_11;

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->request_idle_callback(callback, options); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::cancel_idle_callback)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::cancel_idle_callback");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "cancelIdleCallback");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong handle;

    handle = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cancel_idle_callback(handle); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::get_selection)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::get_selection");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_selection(); }));

    if (retval) {

    return &const_cast<Selection::Selection&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::find)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::find");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    String string;

    if (!arg0.is_undefined()) {
        if (!false || !arg0.is_null())
            string = TRY(WebIDL::to_string(vm, arg0));
    } else {
        string = ""_string;
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->find(string); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::capture_events)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::capture_events");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->capture_events(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::release_events)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::release_events");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->release_events(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::request_animation_frame)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::request_animation_frame");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "requestAnimationFrame");

    auto arg0 = vm.argument(0);

    if (!arg0.is_function()
)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, arg0);

    auto callback = vm.heap().allocate<WebIDL::CallbackType>(arg0.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->request_animation_frame(callback); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::cancel_animation_frame)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::cancel_animation_frame");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "cancelAnimationFrame");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong handle;

    handle = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cancel_animation_frame(handle); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::report_error)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::report_error");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "reportError");

    auto arg0 = vm.argument(0);

    auto e = arg0;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->report_error(e); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::set_timeout)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::set_timeout");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::clear_timeout)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::clear_timeout");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::set_interval)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::set_interval");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::clear_interval)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::clear_interval");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::create_image_bitmap0)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::create_image_bitmap0");
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

    auto resize_height_property_value_12 = JS::js_undefined();
    if (arg1.is_object())
        resize_height_property_value_12 = TRY(arg1.as_object().get("resizeHeight"_utf16_fly_string));

    if (!resize_height_property_value_12.is_undefined()) {

    Optional<WebIDL::UnsignedLong> resize_height_value_12;

    if (!resize_height_property_value_12.is_undefined())

    resize_height_value_12 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, resize_height_property_value_12, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    options.resize_height = resize_height_value_12;

    }

    auto resize_quality_property_value_13 = JS::js_undefined();
    if (arg1.is_object())
        resize_quality_property_value_13 = TRY(arg1.as_object().get("resizeQuality"_utf16_fly_string));

    ResizeQuality resize_quality_value_13 { ResizeQuality::Low };

    if (!resize_quality_property_value_13.is_undefined()) {

    auto resize_quality_property_value_13_string = TRY(resize_quality_property_value_13.to_string(vm));

    if (resize_quality_property_value_13_string == "pixelated"sv)
        resize_quality_value_13 = ResizeQuality::Pixelated;

    else if (resize_quality_property_value_13_string == "low"sv)
        resize_quality_value_13 = ResizeQuality::Low;

    else if (resize_quality_property_value_13_string == "medium"sv)
        resize_quality_value_13 = ResizeQuality::Medium;

    else if (resize_quality_property_value_13_string == "high"sv)
        resize_quality_value_13 = ResizeQuality::High;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, resize_quality_property_value_13_string, "ResizeQuality");

    }

    options.resize_quality = resize_quality_value_13;

    auto resize_width_property_value_14 = JS::js_undefined();
    if (arg1.is_object())
        resize_width_property_value_14 = TRY(arg1.as_object().get("resizeWidth"_utf16_fly_string));

    if (!resize_width_property_value_14.is_undefined()) {

    Optional<WebIDL::UnsignedLong> resize_width_value_14;

    if (!resize_width_property_value_14.is_undefined())

    resize_width_value_14 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, resize_width_property_value_14, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    options.resize_width = resize_width_value_14;

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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::create_image_bitmap1)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::create_image_bitmap1");
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

    auto resize_height_property_value_15 = JS::js_undefined();
    if (arg5.is_object())
        resize_height_property_value_15 = TRY(arg5.as_object().get("resizeHeight"_utf16_fly_string));

    if (!resize_height_property_value_15.is_undefined()) {

    Optional<WebIDL::UnsignedLong> resize_height_value_15;

    if (!resize_height_property_value_15.is_undefined())

    resize_height_value_15 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, resize_height_property_value_15, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    options.resize_height = resize_height_value_15;

    }

    auto resize_quality_property_value_16 = JS::js_undefined();
    if (arg5.is_object())
        resize_quality_property_value_16 = TRY(arg5.as_object().get("resizeQuality"_utf16_fly_string));

    ResizeQuality resize_quality_value_16 { ResizeQuality::Low };

    if (!resize_quality_property_value_16.is_undefined()) {

    auto resize_quality_property_value_16_string = TRY(resize_quality_property_value_16.to_string(vm));

    if (resize_quality_property_value_16_string == "pixelated"sv)
        resize_quality_value_16 = ResizeQuality::Pixelated;

    else if (resize_quality_property_value_16_string == "low"sv)
        resize_quality_value_16 = ResizeQuality::Low;

    else if (resize_quality_property_value_16_string == "medium"sv)
        resize_quality_value_16 = ResizeQuality::Medium;

    else if (resize_quality_property_value_16_string == "high"sv)
        resize_quality_value_16 = ResizeQuality::High;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, resize_quality_property_value_16_string, "ResizeQuality");

    }

    options.resize_quality = resize_quality_value_16;

    auto resize_width_property_value_17 = JS::js_undefined();
    if (arg5.is_object())
        resize_width_property_value_17 = TRY(arg5.as_object().get("resizeWidth"_utf16_fly_string));

    if (!resize_width_property_value_17.is_undefined()) {

    Optional<WebIDL::UnsignedLong> resize_width_value_17;

    if (!resize_width_property_value_17.is_undefined())

    resize_width_value_17 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, resize_width_property_value_17, WebIDL::EnforceRange::Yes, WebIDL::Clamp::No));

    options.resize_width = resize_width_value_17;

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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::fetch)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::fetch");
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

    auto body_property_value_18 = JS::js_undefined();
    if (arg1.is_object())
        body_property_value_18 = TRY(arg1.as_object().get("body"_utf16_fly_string));

    if (!body_property_value_18.is_undefined()) {

    auto body_property_value_18_to_variant = [&vm, &realm](JS::Value body_property_value_18) -> JS::ThrowCompletionOr<Variant<GC::Root<ReadableStream>, GC::Root<Blob>, GC::Root<WebIDL::BufferSource>, GC::Root<FormData>, GC::Root<URLSearchParams>, String>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (body_property_value_18.is_object()) {
            [[maybe_unused]] auto& body_property_value_18_object = body_property_value_18.as_object();

            if (is<PlatformObject>(body_property_value_18_object)) {

                if (auto* body_property_value_18_result = as_if<ReadableStream>(body_property_value_18_object))
                    return GC::make_root(*body_property_value_18_result);

                if (auto* body_property_value_18_result = as_if<Blob>(body_property_value_18_object))
                    return GC::make_root(*body_property_value_18_result);

                if (auto* body_property_value_18_result = as_if<FormData>(body_property_value_18_object))
                    return GC::make_root(*body_property_value_18_result);

                if (auto* body_property_value_18_result = as_if<URLSearchParams>(body_property_value_18_object))
                    return GC::make_root(*body_property_value_18_result);

            }

            if (is<JS::ArrayBuffer>(body_property_value_18_object) || is<JS::DataView>(body_property_value_18_object) || is<JS::TypedArrayBase>(body_property_value_18_object)) {
                GC::Ref<WebIDL::BufferSource> source_object = realm.create<WebIDL::BufferSource>(body_property_value_18_object);
                return GC::make_root(source_object);
            }

        }

    String body_property_value_18_string;
    if (!false || !body_property_value_18.is_null()) {
        body_property_value_18_string = TRY(WebIDL::to_usv_string(vm, body_property_value_18));
    }

        return { body_property_value_18_string };

    };

    Optional<Variant<GC::Root<ReadableStream>, GC::Root<Blob>, GC::Root<WebIDL::BufferSource>, GC::Root<FormData>, GC::Root<URLSearchParams>, String>> body_value_18;
    if (!body_property_value_18.is_nullish())
        body_value_18 = TRY(body_property_value_18_to_variant(body_property_value_18));

    init.body = body_value_18;

    }

    auto cache_property_value_19 = JS::js_undefined();
    if (arg1.is_object())
        cache_property_value_19 = TRY(arg1.as_object().get("cache"_utf16_fly_string));

    if (!cache_property_value_19.is_undefined()) {

    RequestCache cache_value_19 { RequestCache::Default };

    if (!cache_property_value_19.is_undefined()) {

    auto cache_property_value_19_string = TRY(cache_property_value_19.to_string(vm));

    if (cache_property_value_19_string == "default"sv)
        cache_value_19 = RequestCache::Default;

    else if (cache_property_value_19_string == "no-store"sv)
        cache_value_19 = RequestCache::NoStore;

    else if (cache_property_value_19_string == "reload"sv)
        cache_value_19 = RequestCache::Reload;

    else if (cache_property_value_19_string == "no-cache"sv)
        cache_value_19 = RequestCache::NoCache;

    else if (cache_property_value_19_string == "force-cache"sv)
        cache_value_19 = RequestCache::ForceCache;

    else if (cache_property_value_19_string == "only-if-cached"sv)
        cache_value_19 = RequestCache::OnlyIfCached;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, cache_property_value_19_string, "RequestCache");

    }

    init.cache = cache_value_19;

    }

    auto credentials_property_value_20 = JS::js_undefined();
    if (arg1.is_object())
        credentials_property_value_20 = TRY(arg1.as_object().get("credentials"_utf16_fly_string));

    if (!credentials_property_value_20.is_undefined()) {

    RequestCredentials credentials_value_20 { RequestCredentials::Omit };

    if (!credentials_property_value_20.is_undefined()) {

    auto credentials_property_value_20_string = TRY(credentials_property_value_20.to_string(vm));

    if (credentials_property_value_20_string == "omit"sv)
        credentials_value_20 = RequestCredentials::Omit;

    else if (credentials_property_value_20_string == "same-origin"sv)
        credentials_value_20 = RequestCredentials::SameOrigin;

    else if (credentials_property_value_20_string == "include"sv)
        credentials_value_20 = RequestCredentials::Include;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, credentials_property_value_20_string, "RequestCredentials");

    }

    init.credentials = credentials_value_20;

    }

    auto duplex_property_value_21 = JS::js_undefined();
    if (arg1.is_object())
        duplex_property_value_21 = TRY(arg1.as_object().get("duplex"_utf16_fly_string));

    if (!duplex_property_value_21.is_undefined()) {

    RequestDuplex duplex_value_21 { RequestDuplex::Half };

    if (!duplex_property_value_21.is_undefined()) {

    auto duplex_property_value_21_string = TRY(duplex_property_value_21.to_string(vm));

    if (duplex_property_value_21_string == "half"sv)
        duplex_value_21 = RequestDuplex::Half;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, duplex_property_value_21_string, "RequestDuplex");

    }

    init.duplex = duplex_value_21;

    }

    auto headers_property_value_22 = JS::js_undefined();
    if (arg1.is_object())
        headers_property_value_22 = TRY(arg1.as_object().get("headers"_utf16_fly_string));

    if (!headers_property_value_22.is_undefined()) {

    auto headers_property_value_22_to_variant = [&vm, &realm](JS::Value headers_property_value_22) -> JS::ThrowCompletionOr<Variant<Vector<Vector<String>>, OrderedHashMap<String, String>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (headers_property_value_22.is_object()) {
            [[maybe_unused]] auto& headers_property_value_22_object = headers_property_value_22.as_object();

        auto method = TRY(headers_property_value_22.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto headers_property_value_22_iterator1 = TRY(JS::get_iterator_from_method(vm, headers_property_value_22, *method));

    Vector<Vector<String>> headers_value_22;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, headers_property_value_22_iterator1));
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

    headers_value_22.append(sequence_item1);
    }


            return headers_value_22;
        }

    OrderedHashMap<String, String> record_union_type;

    auto record_keys1 = TRY(headers_property_value_22_object.internal_own_property_keys());

    for (auto& key1 : record_keys1) {
        auto property_key1 = MUST(JS::PropertyKey::from_value(vm, key1));

        auto descriptor1 = TRY(headers_property_value_22_object.internal_get_own_property(property_key1));

        if (!descriptor1.has_value() || !descriptor1->enumerable.has_value() || !descriptor1->enumerable.value())
            continue;

    String typed_key1;
    if (!false || !key1.is_null()) {
        typed_key1 = TRY(WebIDL::to_byte_string(vm, key1));
    }

        auto value1 = TRY(headers_property_value_22_object.get(property_key1));

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

    Optional<Variant<Vector<Vector<String>>, OrderedHashMap<String, String>>> headers_value_22;
    if (!headers_property_value_22.is_undefined())
        headers_value_22 = TRY(headers_property_value_22_to_variant(headers_property_value_22));

    init.headers = headers_value_22;

    }

    auto integrity_property_value_23 = JS::js_undefined();
    if (arg1.is_object())
        integrity_property_value_23 = TRY(arg1.as_object().get("integrity"_utf16_fly_string));

    if (!integrity_property_value_23.is_undefined()) {

    Optional<String> integrity_value_23;

    if (!integrity_property_value_23.is_undefined()) {
        if (!false || !integrity_property_value_23.is_null())
            integrity_value_23 = TRY(WebIDL::to_string(vm, integrity_property_value_23));
    }

    init.integrity = integrity_value_23;

    }

    auto keepalive_property_value_24 = JS::js_undefined();
    if (arg1.is_object())
        keepalive_property_value_24 = TRY(arg1.as_object().get("keepalive"_utf16_fly_string));

    if (!keepalive_property_value_24.is_undefined()) {

    Optional<bool> keepalive_value_24;

    if (!keepalive_property_value_24.is_undefined())

    keepalive_value_24 = keepalive_property_value_24.to_boolean();

    init.keepalive = keepalive_value_24;

    }

    auto method_property_value_25 = JS::js_undefined();
    if (arg1.is_object())
        method_property_value_25 = TRY(arg1.as_object().get("method"_utf16_fly_string));

    if (!method_property_value_25.is_undefined()) {

    Optional<String> method_value_25;

    if (!method_property_value_25.is_undefined()) {
        if (!false || !method_property_value_25.is_null())
            method_value_25 = TRY(WebIDL::to_byte_string(vm, method_property_value_25));
    }

    init.method = method_value_25;

    }

    auto mode_property_value_26 = JS::js_undefined();
    if (arg1.is_object())
        mode_property_value_26 = TRY(arg1.as_object().get("mode"_utf16_fly_string));

    if (!mode_property_value_26.is_undefined()) {

    RequestMode mode_value_26 { RequestMode::Navigate };

    if (!mode_property_value_26.is_undefined()) {

    auto mode_property_value_26_string = TRY(mode_property_value_26.to_string(vm));

    if (mode_property_value_26_string == "navigate"sv)
        mode_value_26 = RequestMode::Navigate;

    else if (mode_property_value_26_string == "same-origin"sv)
        mode_value_26 = RequestMode::SameOrigin;

    else if (mode_property_value_26_string == "no-cors"sv)
        mode_value_26 = RequestMode::NoCors;

    else if (mode_property_value_26_string == "cors"sv)
        mode_value_26 = RequestMode::Cors;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, mode_property_value_26_string, "RequestMode");

    }

    init.mode = mode_value_26;

    }

    auto priority_property_value_27 = JS::js_undefined();
    if (arg1.is_object())
        priority_property_value_27 = TRY(arg1.as_object().get("priority"_utf16_fly_string));

    if (!priority_property_value_27.is_undefined()) {

    RequestPriority priority_value_27 { RequestPriority::High };

    if (!priority_property_value_27.is_undefined()) {

    auto priority_property_value_27_string = TRY(priority_property_value_27.to_string(vm));

    if (priority_property_value_27_string == "high"sv)
        priority_value_27 = RequestPriority::High;

    else if (priority_property_value_27_string == "low"sv)
        priority_value_27 = RequestPriority::Low;

    else if (priority_property_value_27_string == "auto"sv)
        priority_value_27 = RequestPriority::Auto;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, priority_property_value_27_string, "RequestPriority");

    }

    init.priority = priority_value_27;

    }

    auto redirect_property_value_28 = JS::js_undefined();
    if (arg1.is_object())
        redirect_property_value_28 = TRY(arg1.as_object().get("redirect"_utf16_fly_string));

    if (!redirect_property_value_28.is_undefined()) {

    RequestRedirect redirect_value_28 { RequestRedirect::Follow };

    if (!redirect_property_value_28.is_undefined()) {

    auto redirect_property_value_28_string = TRY(redirect_property_value_28.to_string(vm));

    if (redirect_property_value_28_string == "follow"sv)
        redirect_value_28 = RequestRedirect::Follow;

    else if (redirect_property_value_28_string == "error"sv)
        redirect_value_28 = RequestRedirect::Error;

    else if (redirect_property_value_28_string == "manual"sv)
        redirect_value_28 = RequestRedirect::Manual;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, redirect_property_value_28_string, "RequestRedirect");

    }

    init.redirect = redirect_value_28;

    }

    auto referrer_property_value_29 = JS::js_undefined();
    if (arg1.is_object())
        referrer_property_value_29 = TRY(arg1.as_object().get("referrer"_utf16_fly_string));

    if (!referrer_property_value_29.is_undefined()) {

    Optional<String> referrer_value_29;

    if (!referrer_property_value_29.is_undefined()) {
        if (!false || !referrer_property_value_29.is_null())
            referrer_value_29 = TRY(WebIDL::to_usv_string(vm, referrer_property_value_29));
    }

    init.referrer = referrer_value_29;

    }

    auto referrer_policy_property_value_30 = JS::js_undefined();
    if (arg1.is_object())
        referrer_policy_property_value_30 = TRY(arg1.as_object().get("referrerPolicy"_utf16_fly_string));

    if (!referrer_policy_property_value_30.is_undefined()) {

    ReferrerPolicy referrer_policy_value_30 { ReferrerPolicy::NoReferrer };

    if (!referrer_policy_property_value_30.is_undefined()) {

    auto referrer_policy_property_value_30_string = TRY(referrer_policy_property_value_30.to_string(vm));

    if (referrer_policy_property_value_30_string == ""sv)
        referrer_policy_value_30 = ReferrerPolicy::Empty;

    else if (referrer_policy_property_value_30_string == "no-referrer"sv)
        referrer_policy_value_30 = ReferrerPolicy::NoReferrer;

    else if (referrer_policy_property_value_30_string == "no-referrer-when-downgrade"sv)
        referrer_policy_value_30 = ReferrerPolicy::NoReferrerWhenDowngrade;

    else if (referrer_policy_property_value_30_string == "same-origin"sv)
        referrer_policy_value_30 = ReferrerPolicy::SameOrigin;

    else if (referrer_policy_property_value_30_string == "origin"sv)
        referrer_policy_value_30 = ReferrerPolicy::Origin;

    else if (referrer_policy_property_value_30_string == "strict-origin"sv)
        referrer_policy_value_30 = ReferrerPolicy::StrictOrigin;

    else if (referrer_policy_property_value_30_string == "origin-when-cross-origin"sv)
        referrer_policy_value_30 = ReferrerPolicy::OriginWhenCrossOrigin;

    else if (referrer_policy_property_value_30_string == "strict-origin-when-cross-origin"sv)
        referrer_policy_value_30 = ReferrerPolicy::StrictOriginWhenCrossOrigin;

    else if (referrer_policy_property_value_30_string == "unsafe-url"sv)
        referrer_policy_value_30 = ReferrerPolicy::UnsafeUrl;

    else
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::InvalidEnumerationValue, referrer_policy_property_value_30_string, "ReferrerPolicy");

    }

    init.referrer_policy = referrer_policy_value_30;

    }

    auto signal_property_value_31 = JS::js_undefined();
    if (arg1.is_object())
        signal_property_value_31 = TRY(arg1.as_object().get("signal"_utf16_fly_string));

    if (!signal_property_value_31.is_undefined()) {

    GC::Ptr<AbortSignal> signal_value_31;

    if (!signal_property_value_31.is_nullish()) {
        if (!signal_property_value_31.is_object() || !is<AbortSignal>(signal_property_value_31.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "AbortSignal");

        signal_value_31 = &static_cast<AbortSignal&>(signal_property_value_31.as_object());
    }

    init.signal = signal_value_31;

    }

    auto window_property_value_32 = JS::js_undefined();
    if (arg1.is_object())
        window_property_value_32 = TRY(arg1.as_object().get("window"_utf16_fly_string));

    if (!window_property_value_32.is_undefined()) {

    JS::Value window_value_32 = JS::js_undefined();
    if (!window_property_value_32.is_undefined())
        window_value_32 = window_property_value_32;

    init.window = window_value_32;

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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::btoa)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::btoa");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::atob)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::atob");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::queue_microtask)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::queue_microtask");
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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::structured_clone)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::structured_clone");
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

    auto transfer_property_value_33 = JS::js_undefined();
    if (arg1.is_object())
        transfer_property_value_33 = TRY(arg1.as_object().get("transfer"_utf16_fly_string));

    Vector<GC::Root<JS::Object>> transfer_value_33;

    if (!transfer_property_value_33.is_undefined()) {

    if (!transfer_property_value_33.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, transfer_property_value_33);

    auto transfer_property_value_33_iterator_method0 = TRY(transfer_property_value_33.get_method(vm, vm.well_known_symbol_iterator()));
    if (!transfer_property_value_33_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, transfer_property_value_33);

    auto transfer_property_value_33_iterator1 = TRY(JS::get_iterator_from_method(vm, transfer_property_value_33, *transfer_property_value_33_iterator_method0));

    Vector<GC::Root<JS::Object>> transfer_value_33_non_optional;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, transfer_property_value_33_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    if (!next_item1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, next_item1);
    auto sequence_item1 = GC::make_root(next_item1.as_object());

    transfer_value_33_non_optional.append(sequence_item1);
    }

        transfer_value_33 = move(transfer_value_33_non_optional);
    }

    options.transfer = transfer_value_33;

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->structured_clone(value, options); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::alert)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::alert");

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

        return alert0(vm);

    case 1:

        return alert1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::post_message)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::post_message");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(3, vm.argument_count())) {

    case 1:
        chosen_overload_callable_id = 1;
        break;


    case 2: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("any", false), make_ref_counted<IDL::Type>("USVString", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Required});
        overloads.empend(1, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("any", false), make_ref_counted<IDL::Type>("WindowPostMessageOptions", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Optional});

        effective_overload_set.emplace(move(overloads), 1);
        break;
    }

    case 3:
        chosen_overload_callable_id = 0;
        break;


    }

    Vector<StringView> dictionary_types {
    "WindowPostMessageOptions"sv,
};


    if (!chosen_overload_callable_id.has_value()) {
        if (!effective_overload_set.has_value())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::OverloadResolutionFailed);
        chosen_overload_callable_id = TRY(WebIDL::resolve_overload(vm, effective_overload_set.value(), dictionary_types)).callable_id;
    }

    switch (chosen_overload_callable_id.value()) {

    case 0:

        return post_message0(vm);

    case 1:

        return post_message1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::scroll)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::scroll");

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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::scroll_to)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::scroll_to");

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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::scroll_by)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::scroll_by");

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

JS_DEFINE_NATIVE_FUNCTION(WindowGlobalMixin::create_image_bitmap)
{
    WebIDL::log_trace(vm, "WindowGlobalMixin::create_image_bitmap");

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
    