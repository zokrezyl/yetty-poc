
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
#include <LibWeb/Bindings/WebGL2RenderingContextPrototype.h>
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


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGL2RenderingContext.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLCanvasElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLRenderingContextBase.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/FileAPI/Blob.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CanvasRenderingContext2D.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/HTMLElement.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/WebGL/WebGLRenderingContext.h>

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

GC_DEFINE_ALLOCATOR(WebGL2RenderingContextPrototype);

WebGL2RenderingContextPrototype::WebGL2RenderingContextPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())

{
}

WebGL2RenderingContextPrototype::~WebGL2RenderingContextPrototype()
{
}

void WebGL2RenderingContextPrototype::initialize(JS::Realm& realm)
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
            
        define_direct_property("texImage3D"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("texImage3D"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("texSubImage3D"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("texSubImage3D"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("copyTexSubImage3D"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("compressedTexImage3D"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("compressedTexSubImage3D"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("getFragDataLocation"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("isQuery"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("isSampler"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("getSamplerParameter"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("isSync"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("isTransformFeedback"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("getTransformFeedbackVarying"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("getIndexedParameter"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("texImage2D"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("texSubImage2D"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("compressedTexImage2D"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("compressedTexSubImage2D"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("readPixels"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
        define_direct_property("getFramebufferAttachmentParameter"_utf16_fly_string, JS::js_undefined(), default_attributes | JS::Attribute::Unimplemented);
            
    auto constant_READ_BUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0C02));

    define_direct_property("READ_BUFFER"_utf16_fly_string, constant_READ_BUFFER_value, JS::Attribute::Enumerable);

    auto constant_UNPACK_ROW_LENGTH_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0CF2));

    define_direct_property("UNPACK_ROW_LENGTH"_utf16_fly_string, constant_UNPACK_ROW_LENGTH_value, JS::Attribute::Enumerable);

    auto constant_UNPACK_SKIP_ROWS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0CF3));

    define_direct_property("UNPACK_SKIP_ROWS"_utf16_fly_string, constant_UNPACK_SKIP_ROWS_value, JS::Attribute::Enumerable);

    auto constant_UNPACK_SKIP_PIXELS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0CF4));

    define_direct_property("UNPACK_SKIP_PIXELS"_utf16_fly_string, constant_UNPACK_SKIP_PIXELS_value, JS::Attribute::Enumerable);

    auto constant_PACK_ROW_LENGTH_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0D02));

    define_direct_property("PACK_ROW_LENGTH"_utf16_fly_string, constant_PACK_ROW_LENGTH_value, JS::Attribute::Enumerable);

    auto constant_PACK_SKIP_ROWS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0D03));

    define_direct_property("PACK_SKIP_ROWS"_utf16_fly_string, constant_PACK_SKIP_ROWS_value, JS::Attribute::Enumerable);

    auto constant_PACK_SKIP_PIXELS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x0D04));

    define_direct_property("PACK_SKIP_PIXELS"_utf16_fly_string, constant_PACK_SKIP_PIXELS_value, JS::Attribute::Enumerable);

    auto constant_COLOR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1800));

    define_direct_property("COLOR"_utf16_fly_string, constant_COLOR_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1801));

    define_direct_property("DEPTH"_utf16_fly_string, constant_DEPTH_value, JS::Attribute::Enumerable);

    auto constant_STENCIL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1802));

    define_direct_property("STENCIL"_utf16_fly_string, constant_STENCIL_value, JS::Attribute::Enumerable);

    auto constant_RED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x1903));

    define_direct_property("RED"_utf16_fly_string, constant_RED_value, JS::Attribute::Enumerable);

    auto constant_RGB8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8051));

    define_direct_property("RGB8"_utf16_fly_string, constant_RGB8_value, JS::Attribute::Enumerable);

    auto constant_RGB10_A2_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8059));

    define_direct_property("RGB10_A2"_utf16_fly_string, constant_RGB10_A2_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_BINDING_3D_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x806A));

    define_direct_property("TEXTURE_BINDING_3D"_utf16_fly_string, constant_TEXTURE_BINDING_3D_value, JS::Attribute::Enumerable);

    auto constant_UNPACK_SKIP_IMAGES_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x806D));

    define_direct_property("UNPACK_SKIP_IMAGES"_utf16_fly_string, constant_UNPACK_SKIP_IMAGES_value, JS::Attribute::Enumerable);

    auto constant_UNPACK_IMAGE_HEIGHT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x806E));

    define_direct_property("UNPACK_IMAGE_HEIGHT"_utf16_fly_string, constant_UNPACK_IMAGE_HEIGHT_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_3D_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x806F));

    define_direct_property("TEXTURE_3D"_utf16_fly_string, constant_TEXTURE_3D_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_WRAP_R_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8072));

    define_direct_property("TEXTURE_WRAP_R"_utf16_fly_string, constant_TEXTURE_WRAP_R_value, JS::Attribute::Enumerable);

    auto constant_MAX_3D_TEXTURE_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8073));

    define_direct_property("MAX_3D_TEXTURE_SIZE"_utf16_fly_string, constant_MAX_3D_TEXTURE_SIZE_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_2_10_10_10_REV_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8368));

    define_direct_property("UNSIGNED_INT_2_10_10_10_REV"_utf16_fly_string, constant_UNSIGNED_INT_2_10_10_10_REV_value, JS::Attribute::Enumerable);

    auto constant_MAX_ELEMENTS_VERTICES_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x80E8));

    define_direct_property("MAX_ELEMENTS_VERTICES"_utf16_fly_string, constant_MAX_ELEMENTS_VERTICES_value, JS::Attribute::Enumerable);

    auto constant_MAX_ELEMENTS_INDICES_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x80E9));

    define_direct_property("MAX_ELEMENTS_INDICES"_utf16_fly_string, constant_MAX_ELEMENTS_INDICES_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_MIN_LOD_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x813A));

    define_direct_property("TEXTURE_MIN_LOD"_utf16_fly_string, constant_TEXTURE_MIN_LOD_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_MAX_LOD_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x813B));

    define_direct_property("TEXTURE_MAX_LOD"_utf16_fly_string, constant_TEXTURE_MAX_LOD_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_BASE_LEVEL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x813C));

    define_direct_property("TEXTURE_BASE_LEVEL"_utf16_fly_string, constant_TEXTURE_BASE_LEVEL_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_MAX_LEVEL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x813D));

    define_direct_property("TEXTURE_MAX_LEVEL"_utf16_fly_string, constant_TEXTURE_MAX_LEVEL_value, JS::Attribute::Enumerable);

    auto constant_MIN_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8007));

    define_direct_property("MIN"_utf16_fly_string, constant_MIN_value, JS::Attribute::Enumerable);

    auto constant_MAX_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8008));

    define_direct_property("MAX"_utf16_fly_string, constant_MAX_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_COMPONENT24_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x81A6));

    define_direct_property("DEPTH_COMPONENT24"_utf16_fly_string, constant_DEPTH_COMPONENT24_value, JS::Attribute::Enumerable);

    auto constant_MAX_TEXTURE_LOD_BIAS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84FD));

    define_direct_property("MAX_TEXTURE_LOD_BIAS"_utf16_fly_string, constant_MAX_TEXTURE_LOD_BIAS_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_COMPARE_MODE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x884C));

    define_direct_property("TEXTURE_COMPARE_MODE"_utf16_fly_string, constant_TEXTURE_COMPARE_MODE_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_COMPARE_FUNC_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x884D));

    define_direct_property("TEXTURE_COMPARE_FUNC"_utf16_fly_string, constant_TEXTURE_COMPARE_FUNC_value, JS::Attribute::Enumerable);

    auto constant_CURRENT_QUERY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8865));

    define_direct_property("CURRENT_QUERY"_utf16_fly_string, constant_CURRENT_QUERY_value, JS::Attribute::Enumerable);

    auto constant_QUERY_RESULT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8866));

    define_direct_property("QUERY_RESULT"_utf16_fly_string, constant_QUERY_RESULT_value, JS::Attribute::Enumerable);

    auto constant_QUERY_RESULT_AVAILABLE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8867));

    define_direct_property("QUERY_RESULT_AVAILABLE"_utf16_fly_string, constant_QUERY_RESULT_AVAILABLE_value, JS::Attribute::Enumerable);

    auto constant_STREAM_READ_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88E1));

    define_direct_property("STREAM_READ"_utf16_fly_string, constant_STREAM_READ_value, JS::Attribute::Enumerable);

    auto constant_STREAM_COPY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88E2));

    define_direct_property("STREAM_COPY"_utf16_fly_string, constant_STREAM_COPY_value, JS::Attribute::Enumerable);

    auto constant_STATIC_READ_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88E5));

    define_direct_property("STATIC_READ"_utf16_fly_string, constant_STATIC_READ_value, JS::Attribute::Enumerable);

    auto constant_STATIC_COPY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88E6));

    define_direct_property("STATIC_COPY"_utf16_fly_string, constant_STATIC_COPY_value, JS::Attribute::Enumerable);

    auto constant_DYNAMIC_READ_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88E9));

    define_direct_property("DYNAMIC_READ"_utf16_fly_string, constant_DYNAMIC_READ_value, JS::Attribute::Enumerable);

    auto constant_DYNAMIC_COPY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88EA));

    define_direct_property("DYNAMIC_COPY"_utf16_fly_string, constant_DYNAMIC_COPY_value, JS::Attribute::Enumerable);

    auto constant_MAX_DRAW_BUFFERS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8824));

    define_direct_property("MAX_DRAW_BUFFERS"_utf16_fly_string, constant_MAX_DRAW_BUFFERS_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER0_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8825));

    define_direct_property("DRAW_BUFFER0"_utf16_fly_string, constant_DRAW_BUFFER0_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER1_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8826));

    define_direct_property("DRAW_BUFFER1"_utf16_fly_string, constant_DRAW_BUFFER1_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER2_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8827));

    define_direct_property("DRAW_BUFFER2"_utf16_fly_string, constant_DRAW_BUFFER2_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER3_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8828));

    define_direct_property("DRAW_BUFFER3"_utf16_fly_string, constant_DRAW_BUFFER3_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER4_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8829));

    define_direct_property("DRAW_BUFFER4"_utf16_fly_string, constant_DRAW_BUFFER4_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER5_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882A));

    define_direct_property("DRAW_BUFFER5"_utf16_fly_string, constant_DRAW_BUFFER5_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER6_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882B));

    define_direct_property("DRAW_BUFFER6"_utf16_fly_string, constant_DRAW_BUFFER6_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER7_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882C));

    define_direct_property("DRAW_BUFFER7"_utf16_fly_string, constant_DRAW_BUFFER7_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882D));

    define_direct_property("DRAW_BUFFER8"_utf16_fly_string, constant_DRAW_BUFFER8_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER9_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882E));

    define_direct_property("DRAW_BUFFER9"_utf16_fly_string, constant_DRAW_BUFFER9_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER10_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x882F));

    define_direct_property("DRAW_BUFFER10"_utf16_fly_string, constant_DRAW_BUFFER10_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER11_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8830));

    define_direct_property("DRAW_BUFFER11"_utf16_fly_string, constant_DRAW_BUFFER11_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER12_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8831));

    define_direct_property("DRAW_BUFFER12"_utf16_fly_string, constant_DRAW_BUFFER12_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER13_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8832));

    define_direct_property("DRAW_BUFFER13"_utf16_fly_string, constant_DRAW_BUFFER13_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER14_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8833));

    define_direct_property("DRAW_BUFFER14"_utf16_fly_string, constant_DRAW_BUFFER14_value, JS::Attribute::Enumerable);

    auto constant_DRAW_BUFFER15_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8834));

    define_direct_property("DRAW_BUFFER15"_utf16_fly_string, constant_DRAW_BUFFER15_value, JS::Attribute::Enumerable);

    auto constant_MAX_FRAGMENT_UNIFORM_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B49));

    define_direct_property("MAX_FRAGMENT_UNIFORM_COMPONENTS"_utf16_fly_string, constant_MAX_FRAGMENT_UNIFORM_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_MAX_VERTEX_UNIFORM_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B4A));

    define_direct_property("MAX_VERTEX_UNIFORM_COMPONENTS"_utf16_fly_string, constant_MAX_VERTEX_UNIFORM_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_SAMPLER_3D_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B5F));

    define_direct_property("SAMPLER_3D"_utf16_fly_string, constant_SAMPLER_3D_value, JS::Attribute::Enumerable);

    auto constant_SAMPLER_2D_SHADOW_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B62));

    define_direct_property("SAMPLER_2D_SHADOW"_utf16_fly_string, constant_SAMPLER_2D_SHADOW_value, JS::Attribute::Enumerable);

    auto constant_FRAGMENT_SHADER_DERIVATIVE_HINT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B8B));

    define_direct_property("FRAGMENT_SHADER_DERIVATIVE_HINT"_utf16_fly_string, constant_FRAGMENT_SHADER_DERIVATIVE_HINT_value, JS::Attribute::Enumerable);

    auto constant_PIXEL_PACK_BUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88EB));

    define_direct_property("PIXEL_PACK_BUFFER"_utf16_fly_string, constant_PIXEL_PACK_BUFFER_value, JS::Attribute::Enumerable);

    auto constant_PIXEL_UNPACK_BUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88EC));

    define_direct_property("PIXEL_UNPACK_BUFFER"_utf16_fly_string, constant_PIXEL_UNPACK_BUFFER_value, JS::Attribute::Enumerable);

    auto constant_PIXEL_PACK_BUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88ED));

    define_direct_property("PIXEL_PACK_BUFFER_BINDING"_utf16_fly_string, constant_PIXEL_PACK_BUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_PIXEL_UNPACK_BUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88EF));

    define_direct_property("PIXEL_UNPACK_BUFFER_BINDING"_utf16_fly_string, constant_PIXEL_UNPACK_BUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_MAT2x3_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B65));

    define_direct_property("FLOAT_MAT2x3"_utf16_fly_string, constant_FLOAT_MAT2x3_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_MAT2x4_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B66));

    define_direct_property("FLOAT_MAT2x4"_utf16_fly_string, constant_FLOAT_MAT2x4_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_MAT3x2_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B67));

    define_direct_property("FLOAT_MAT3x2"_utf16_fly_string, constant_FLOAT_MAT3x2_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_MAT3x4_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B68));

    define_direct_property("FLOAT_MAT3x4"_utf16_fly_string, constant_FLOAT_MAT3x4_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_MAT4x2_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B69));

    define_direct_property("FLOAT_MAT4x2"_utf16_fly_string, constant_FLOAT_MAT4x2_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_MAT4x3_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B6A));

    define_direct_property("FLOAT_MAT4x3"_utf16_fly_string, constant_FLOAT_MAT4x3_value, JS::Attribute::Enumerable);

    auto constant_SRGB_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C40));

    define_direct_property("SRGB"_utf16_fly_string, constant_SRGB_value, JS::Attribute::Enumerable);

    auto constant_SRGB8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C41));

    define_direct_property("SRGB8"_utf16_fly_string, constant_SRGB8_value, JS::Attribute::Enumerable);

    auto constant_SRGB8_ALPHA8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C43));

    define_direct_property("SRGB8_ALPHA8"_utf16_fly_string, constant_SRGB8_ALPHA8_value, JS::Attribute::Enumerable);

    auto constant_COMPARE_REF_TO_TEXTURE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x884E));

    define_direct_property("COMPARE_REF_TO_TEXTURE"_utf16_fly_string, constant_COMPARE_REF_TO_TEXTURE_value, JS::Attribute::Enumerable);

    auto constant_RGBA32F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8814));

    define_direct_property("RGBA32F"_utf16_fly_string, constant_RGBA32F_value, JS::Attribute::Enumerable);

    auto constant_RGB32F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8815));

    define_direct_property("RGB32F"_utf16_fly_string, constant_RGB32F_value, JS::Attribute::Enumerable);

    auto constant_RGBA16F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x881A));

    define_direct_property("RGBA16F"_utf16_fly_string, constant_RGBA16F_value, JS::Attribute::Enumerable);

    auto constant_RGB16F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x881B));

    define_direct_property("RGB16F"_utf16_fly_string, constant_RGB16F_value, JS::Attribute::Enumerable);

    auto constant_VERTEX_ATTRIB_ARRAY_INTEGER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88FD));

    define_direct_property("VERTEX_ATTRIB_ARRAY_INTEGER"_utf16_fly_string, constant_VERTEX_ATTRIB_ARRAY_INTEGER_value, JS::Attribute::Enumerable);

    auto constant_MAX_ARRAY_TEXTURE_LAYERS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88FF));

    define_direct_property("MAX_ARRAY_TEXTURE_LAYERS"_utf16_fly_string, constant_MAX_ARRAY_TEXTURE_LAYERS_value, JS::Attribute::Enumerable);

    auto constant_MIN_PROGRAM_TEXEL_OFFSET_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8904));

    define_direct_property("MIN_PROGRAM_TEXEL_OFFSET"_utf16_fly_string, constant_MIN_PROGRAM_TEXEL_OFFSET_value, JS::Attribute::Enumerable);

    auto constant_MAX_PROGRAM_TEXEL_OFFSET_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8905));

    define_direct_property("MAX_PROGRAM_TEXEL_OFFSET"_utf16_fly_string, constant_MAX_PROGRAM_TEXEL_OFFSET_value, JS::Attribute::Enumerable);

    auto constant_MAX_VARYING_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8B4B));

    define_direct_property("MAX_VARYING_COMPONENTS"_utf16_fly_string, constant_MAX_VARYING_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_2D_ARRAY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C1A));

    define_direct_property("TEXTURE_2D_ARRAY"_utf16_fly_string, constant_TEXTURE_2D_ARRAY_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_BINDING_2D_ARRAY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C1D));

    define_direct_property("TEXTURE_BINDING_2D_ARRAY"_utf16_fly_string, constant_TEXTURE_BINDING_2D_ARRAY_value, JS::Attribute::Enumerable);

    auto constant_R11F_G11F_B10F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C3A));

    define_direct_property("R11F_G11F_B10F"_utf16_fly_string, constant_R11F_G11F_B10F_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_10F_11F_11F_REV_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C3B));

    define_direct_property("UNSIGNED_INT_10F_11F_11F_REV"_utf16_fly_string, constant_UNSIGNED_INT_10F_11F_11F_REV_value, JS::Attribute::Enumerable);

    auto constant_RGB9_E5_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C3D));

    define_direct_property("RGB9_E5"_utf16_fly_string, constant_RGB9_E5_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_5_9_9_9_REV_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C3E));

    define_direct_property("UNSIGNED_INT_5_9_9_9_REV"_utf16_fly_string, constant_UNSIGNED_INT_5_9_9_9_REV_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_BUFFER_MODE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C7F));

    define_direct_property("TRANSFORM_FEEDBACK_BUFFER_MODE"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_BUFFER_MODE_value, JS::Attribute::Enumerable);

    auto constant_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C80));

    define_direct_property("MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS"_utf16_fly_string, constant_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_VARYINGS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C83));

    define_direct_property("TRANSFORM_FEEDBACK_VARYINGS"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_VARYINGS_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_BUFFER_START_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C84));

    define_direct_property("TRANSFORM_FEEDBACK_BUFFER_START"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_BUFFER_START_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_BUFFER_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C85));

    define_direct_property("TRANSFORM_FEEDBACK_BUFFER_SIZE"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_BUFFER_SIZE_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C88));

    define_direct_property("TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_value, JS::Attribute::Enumerable);

    auto constant_RASTERIZER_DISCARD_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C89));

    define_direct_property("RASTERIZER_DISCARD"_utf16_fly_string, constant_RASTERIZER_DISCARD_value, JS::Attribute::Enumerable);

    auto constant_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C8A));

    define_direct_property("MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS"_utf16_fly_string, constant_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C8B));

    define_direct_property("MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS"_utf16_fly_string, constant_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_value, JS::Attribute::Enumerable);

    auto constant_INTERLEAVED_ATTRIBS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C8C));

    define_direct_property("INTERLEAVED_ATTRIBS"_utf16_fly_string, constant_INTERLEAVED_ATTRIBS_value, JS::Attribute::Enumerable);

    auto constant_SEPARATE_ATTRIBS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C8D));

    define_direct_property("SEPARATE_ATTRIBS"_utf16_fly_string, constant_SEPARATE_ATTRIBS_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_BUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C8E));

    define_direct_property("TRANSFORM_FEEDBACK_BUFFER"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_BUFFER_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_BUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C8F));

    define_direct_property("TRANSFORM_FEEDBACK_BUFFER_BINDING"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_BUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_RGBA32UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D70));

    define_direct_property("RGBA32UI"_utf16_fly_string, constant_RGBA32UI_value, JS::Attribute::Enumerable);

    auto constant_RGB32UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D71));

    define_direct_property("RGB32UI"_utf16_fly_string, constant_RGB32UI_value, JS::Attribute::Enumerable);

    auto constant_RGBA16UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D76));

    define_direct_property("RGBA16UI"_utf16_fly_string, constant_RGBA16UI_value, JS::Attribute::Enumerable);

    auto constant_RGB16UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D77));

    define_direct_property("RGB16UI"_utf16_fly_string, constant_RGB16UI_value, JS::Attribute::Enumerable);

    auto constant_RGBA8UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D7C));

    define_direct_property("RGBA8UI"_utf16_fly_string, constant_RGBA8UI_value, JS::Attribute::Enumerable);

    auto constant_RGB8UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D7D));

    define_direct_property("RGB8UI"_utf16_fly_string, constant_RGB8UI_value, JS::Attribute::Enumerable);

    auto constant_RGBA32I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D82));

    define_direct_property("RGBA32I"_utf16_fly_string, constant_RGBA32I_value, JS::Attribute::Enumerable);

    auto constant_RGB32I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D83));

    define_direct_property("RGB32I"_utf16_fly_string, constant_RGB32I_value, JS::Attribute::Enumerable);

    auto constant_RGBA16I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D88));

    define_direct_property("RGBA16I"_utf16_fly_string, constant_RGBA16I_value, JS::Attribute::Enumerable);

    auto constant_RGB16I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D89));

    define_direct_property("RGB16I"_utf16_fly_string, constant_RGB16I_value, JS::Attribute::Enumerable);

    auto constant_RGBA8I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D8E));

    define_direct_property("RGBA8I"_utf16_fly_string, constant_RGBA8I_value, JS::Attribute::Enumerable);

    auto constant_RGB8I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D8F));

    define_direct_property("RGB8I"_utf16_fly_string, constant_RGB8I_value, JS::Attribute::Enumerable);

    auto constant_RED_INTEGER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D94));

    define_direct_property("RED_INTEGER"_utf16_fly_string, constant_RED_INTEGER_value, JS::Attribute::Enumerable);

    auto constant_RGB_INTEGER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D98));

    define_direct_property("RGB_INTEGER"_utf16_fly_string, constant_RGB_INTEGER_value, JS::Attribute::Enumerable);

    auto constant_RGBA_INTEGER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D99));

    define_direct_property("RGBA_INTEGER"_utf16_fly_string, constant_RGBA_INTEGER_value, JS::Attribute::Enumerable);

    auto constant_SAMPLER_2D_ARRAY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DC1));

    define_direct_property("SAMPLER_2D_ARRAY"_utf16_fly_string, constant_SAMPLER_2D_ARRAY_value, JS::Attribute::Enumerable);

    auto constant_SAMPLER_2D_ARRAY_SHADOW_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DC4));

    define_direct_property("SAMPLER_2D_ARRAY_SHADOW"_utf16_fly_string, constant_SAMPLER_2D_ARRAY_SHADOW_value, JS::Attribute::Enumerable);

    auto constant_SAMPLER_CUBE_SHADOW_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DC5));

    define_direct_property("SAMPLER_CUBE_SHADOW"_utf16_fly_string, constant_SAMPLER_CUBE_SHADOW_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_VEC2_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DC6));

    define_direct_property("UNSIGNED_INT_VEC2"_utf16_fly_string, constant_UNSIGNED_INT_VEC2_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_VEC3_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DC7));

    define_direct_property("UNSIGNED_INT_VEC3"_utf16_fly_string, constant_UNSIGNED_INT_VEC3_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_VEC4_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DC8));

    define_direct_property("UNSIGNED_INT_VEC4"_utf16_fly_string, constant_UNSIGNED_INT_VEC4_value, JS::Attribute::Enumerable);

    auto constant_INT_SAMPLER_2D_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DCA));

    define_direct_property("INT_SAMPLER_2D"_utf16_fly_string, constant_INT_SAMPLER_2D_value, JS::Attribute::Enumerable);

    auto constant_INT_SAMPLER_3D_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DCB));

    define_direct_property("INT_SAMPLER_3D"_utf16_fly_string, constant_INT_SAMPLER_3D_value, JS::Attribute::Enumerable);

    auto constant_INT_SAMPLER_CUBE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DCC));

    define_direct_property("INT_SAMPLER_CUBE"_utf16_fly_string, constant_INT_SAMPLER_CUBE_value, JS::Attribute::Enumerable);

    auto constant_INT_SAMPLER_2D_ARRAY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DCF));

    define_direct_property("INT_SAMPLER_2D_ARRAY"_utf16_fly_string, constant_INT_SAMPLER_2D_ARRAY_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_SAMPLER_2D_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DD2));

    define_direct_property("UNSIGNED_INT_SAMPLER_2D"_utf16_fly_string, constant_UNSIGNED_INT_SAMPLER_2D_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_SAMPLER_3D_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DD3));

    define_direct_property("UNSIGNED_INT_SAMPLER_3D"_utf16_fly_string, constant_UNSIGNED_INT_SAMPLER_3D_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_SAMPLER_CUBE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DD4));

    define_direct_property("UNSIGNED_INT_SAMPLER_CUBE"_utf16_fly_string, constant_UNSIGNED_INT_SAMPLER_CUBE_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_SAMPLER_2D_ARRAY_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DD7));

    define_direct_property("UNSIGNED_INT_SAMPLER_2D_ARRAY"_utf16_fly_string, constant_UNSIGNED_INT_SAMPLER_2D_ARRAY_value, JS::Attribute::Enumerable);

    auto constant_DEPTH_COMPONENT32F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CAC));

    define_direct_property("DEPTH_COMPONENT32F"_utf16_fly_string, constant_DEPTH_COMPONENT32F_value, JS::Attribute::Enumerable);

    auto constant_DEPTH32F_STENCIL8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CAD));

    define_direct_property("DEPTH32F_STENCIL8"_utf16_fly_string, constant_DEPTH32F_STENCIL8_value, JS::Attribute::Enumerable);

    auto constant_FLOAT_32_UNSIGNED_INT_24_8_REV_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8DAD));

    define_direct_property("FLOAT_32_UNSIGNED_INT_24_8_REV"_utf16_fly_string, constant_FLOAT_32_UNSIGNED_INT_24_8_REV_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8210));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8211));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_RED_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8212));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_RED_SIZE"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_RED_SIZE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8213));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_GREEN_SIZE"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8214));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_BLUE_SIZE"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8215));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8216));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8217));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_DEFAULT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8218));

    define_direct_property("FRAMEBUFFER_DEFAULT"_utf16_fly_string, constant_FRAMEBUFFER_DEFAULT_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_INT_24_8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x84FA));

    define_direct_property("UNSIGNED_INT_24_8"_utf16_fly_string, constant_UNSIGNED_INT_24_8_value, JS::Attribute::Enumerable);

    auto constant_DEPTH24_STENCIL8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88F0));

    define_direct_property("DEPTH24_STENCIL8"_utf16_fly_string, constant_DEPTH24_STENCIL8_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNED_NORMALIZED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C17));

    define_direct_property("UNSIGNED_NORMALIZED"_utf16_fly_string, constant_UNSIGNED_NORMALIZED_value, JS::Attribute::Enumerable);

    auto constant_DRAW_FRAMEBUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CA6));

    define_direct_property("DRAW_FRAMEBUFFER_BINDING"_utf16_fly_string, constant_DRAW_FRAMEBUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_READ_FRAMEBUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CA8));

    define_direct_property("READ_FRAMEBUFFER"_utf16_fly_string, constant_READ_FRAMEBUFFER_value, JS::Attribute::Enumerable);

    auto constant_DRAW_FRAMEBUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CA9));

    define_direct_property("DRAW_FRAMEBUFFER"_utf16_fly_string, constant_DRAW_FRAMEBUFFER_value, JS::Attribute::Enumerable);

    auto constant_READ_FRAMEBUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CAA));

    define_direct_property("READ_FRAMEBUFFER_BINDING"_utf16_fly_string, constant_READ_FRAMEBUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_RENDERBUFFER_SAMPLES_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CAB));

    define_direct_property("RENDERBUFFER_SAMPLES"_utf16_fly_string, constant_RENDERBUFFER_SAMPLES_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CD4));

    define_direct_property("FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER"_utf16_fly_string, constant_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER_value, JS::Attribute::Enumerable);

    auto constant_MAX_COLOR_ATTACHMENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CDF));

    define_direct_property("MAX_COLOR_ATTACHMENTS"_utf16_fly_string, constant_MAX_COLOR_ATTACHMENTS_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT1_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE1));

    define_direct_property("COLOR_ATTACHMENT1"_utf16_fly_string, constant_COLOR_ATTACHMENT1_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT2_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE2));

    define_direct_property("COLOR_ATTACHMENT2"_utf16_fly_string, constant_COLOR_ATTACHMENT2_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT3_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE3));

    define_direct_property("COLOR_ATTACHMENT3"_utf16_fly_string, constant_COLOR_ATTACHMENT3_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT4_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE4));

    define_direct_property("COLOR_ATTACHMENT4"_utf16_fly_string, constant_COLOR_ATTACHMENT4_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT5_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE5));

    define_direct_property("COLOR_ATTACHMENT5"_utf16_fly_string, constant_COLOR_ATTACHMENT5_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT6_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE6));

    define_direct_property("COLOR_ATTACHMENT6"_utf16_fly_string, constant_COLOR_ATTACHMENT6_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT7_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE7));

    define_direct_property("COLOR_ATTACHMENT7"_utf16_fly_string, constant_COLOR_ATTACHMENT7_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE8));

    define_direct_property("COLOR_ATTACHMENT8"_utf16_fly_string, constant_COLOR_ATTACHMENT8_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT9_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CE9));

    define_direct_property("COLOR_ATTACHMENT9"_utf16_fly_string, constant_COLOR_ATTACHMENT9_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT10_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CEA));

    define_direct_property("COLOR_ATTACHMENT10"_utf16_fly_string, constant_COLOR_ATTACHMENT10_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT11_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CEB));

    define_direct_property("COLOR_ATTACHMENT11"_utf16_fly_string, constant_COLOR_ATTACHMENT11_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT12_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CEC));

    define_direct_property("COLOR_ATTACHMENT12"_utf16_fly_string, constant_COLOR_ATTACHMENT12_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT13_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CED));

    define_direct_property("COLOR_ATTACHMENT13"_utf16_fly_string, constant_COLOR_ATTACHMENT13_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT14_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CEE));

    define_direct_property("COLOR_ATTACHMENT14"_utf16_fly_string, constant_COLOR_ATTACHMENT14_value, JS::Attribute::Enumerable);

    auto constant_COLOR_ATTACHMENT15_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8CEF));

    define_direct_property("COLOR_ATTACHMENT15"_utf16_fly_string, constant_COLOR_ATTACHMENT15_value, JS::Attribute::Enumerable);

    auto constant_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D56));

    define_direct_property("FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"_utf16_fly_string, constant_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_value, JS::Attribute::Enumerable);

    auto constant_MAX_SAMPLES_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D57));

    define_direct_property("MAX_SAMPLES"_utf16_fly_string, constant_MAX_SAMPLES_value, JS::Attribute::Enumerable);

    auto constant_HALF_FLOAT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x140B));

    define_direct_property("HALF_FLOAT"_utf16_fly_string, constant_HALF_FLOAT_value, JS::Attribute::Enumerable);

    auto constant_RG_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8227));

    define_direct_property("RG"_utf16_fly_string, constant_RG_value, JS::Attribute::Enumerable);

    auto constant_RG_INTEGER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8228));

    define_direct_property("RG_INTEGER"_utf16_fly_string, constant_RG_INTEGER_value, JS::Attribute::Enumerable);

    auto constant_R8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8229));

    define_direct_property("R8"_utf16_fly_string, constant_R8_value, JS::Attribute::Enumerable);

    auto constant_RG8_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x822B));

    define_direct_property("RG8"_utf16_fly_string, constant_RG8_value, JS::Attribute::Enumerable);

    auto constant_R16F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x822D));

    define_direct_property("R16F"_utf16_fly_string, constant_R16F_value, JS::Attribute::Enumerable);

    auto constant_R32F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x822E));

    define_direct_property("R32F"_utf16_fly_string, constant_R32F_value, JS::Attribute::Enumerable);

    auto constant_RG16F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x822F));

    define_direct_property("RG16F"_utf16_fly_string, constant_RG16F_value, JS::Attribute::Enumerable);

    auto constant_RG32F_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8230));

    define_direct_property("RG32F"_utf16_fly_string, constant_RG32F_value, JS::Attribute::Enumerable);

    auto constant_R8I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8231));

    define_direct_property("R8I"_utf16_fly_string, constant_R8I_value, JS::Attribute::Enumerable);

    auto constant_R8UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8232));

    define_direct_property("R8UI"_utf16_fly_string, constant_R8UI_value, JS::Attribute::Enumerable);

    auto constant_R16I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8233));

    define_direct_property("R16I"_utf16_fly_string, constant_R16I_value, JS::Attribute::Enumerable);

    auto constant_R16UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8234));

    define_direct_property("R16UI"_utf16_fly_string, constant_R16UI_value, JS::Attribute::Enumerable);

    auto constant_R32I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8235));

    define_direct_property("R32I"_utf16_fly_string, constant_R32I_value, JS::Attribute::Enumerable);

    auto constant_R32UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8236));

    define_direct_property("R32UI"_utf16_fly_string, constant_R32UI_value, JS::Attribute::Enumerable);

    auto constant_RG8I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8237));

    define_direct_property("RG8I"_utf16_fly_string, constant_RG8I_value, JS::Attribute::Enumerable);

    auto constant_RG8UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8238));

    define_direct_property("RG8UI"_utf16_fly_string, constant_RG8UI_value, JS::Attribute::Enumerable);

    auto constant_RG16I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8239));

    define_direct_property("RG16I"_utf16_fly_string, constant_RG16I_value, JS::Attribute::Enumerable);

    auto constant_RG16UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x823A));

    define_direct_property("RG16UI"_utf16_fly_string, constant_RG16UI_value, JS::Attribute::Enumerable);

    auto constant_RG32I_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x823B));

    define_direct_property("RG32I"_utf16_fly_string, constant_RG32I_value, JS::Attribute::Enumerable);

    auto constant_RG32UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x823C));

    define_direct_property("RG32UI"_utf16_fly_string, constant_RG32UI_value, JS::Attribute::Enumerable);

    auto constant_VERTEX_ARRAY_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x85B5));

    define_direct_property("VERTEX_ARRAY_BINDING"_utf16_fly_string, constant_VERTEX_ARRAY_BINDING_value, JS::Attribute::Enumerable);

    auto constant_R8_SNORM_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F94));

    define_direct_property("R8_SNORM"_utf16_fly_string, constant_R8_SNORM_value, JS::Attribute::Enumerable);

    auto constant_RG8_SNORM_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F95));

    define_direct_property("RG8_SNORM"_utf16_fly_string, constant_RG8_SNORM_value, JS::Attribute::Enumerable);

    auto constant_RGB8_SNORM_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F96));

    define_direct_property("RGB8_SNORM"_utf16_fly_string, constant_RGB8_SNORM_value, JS::Attribute::Enumerable);

    auto constant_RGBA8_SNORM_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F97));

    define_direct_property("RGBA8_SNORM"_utf16_fly_string, constant_RGBA8_SNORM_value, JS::Attribute::Enumerable);

    auto constant_SIGNED_NORMALIZED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F9C));

    define_direct_property("SIGNED_NORMALIZED"_utf16_fly_string, constant_SIGNED_NORMALIZED_value, JS::Attribute::Enumerable);

    auto constant_COPY_READ_BUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F36));

    define_direct_property("COPY_READ_BUFFER"_utf16_fly_string, constant_COPY_READ_BUFFER_value, JS::Attribute::Enumerable);

    auto constant_COPY_WRITE_BUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F37));

    define_direct_property("COPY_WRITE_BUFFER"_utf16_fly_string, constant_COPY_WRITE_BUFFER_value, JS::Attribute::Enumerable);

    auto constant_COPY_READ_BUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F36));

    define_direct_property("COPY_READ_BUFFER_BINDING"_utf16_fly_string, constant_COPY_READ_BUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_COPY_WRITE_BUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8F37));

    define_direct_property("COPY_WRITE_BUFFER_BINDING"_utf16_fly_string, constant_COPY_WRITE_BUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BUFFER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A11));

    define_direct_property("UNIFORM_BUFFER"_utf16_fly_string, constant_UNIFORM_BUFFER_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BUFFER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A28));

    define_direct_property("UNIFORM_BUFFER_BINDING"_utf16_fly_string, constant_UNIFORM_BUFFER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BUFFER_START_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A29));

    define_direct_property("UNIFORM_BUFFER_START"_utf16_fly_string, constant_UNIFORM_BUFFER_START_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BUFFER_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A2A));

    define_direct_property("UNIFORM_BUFFER_SIZE"_utf16_fly_string, constant_UNIFORM_BUFFER_SIZE_value, JS::Attribute::Enumerable);

    auto constant_MAX_VERTEX_UNIFORM_BLOCKS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A2B));

    define_direct_property("MAX_VERTEX_UNIFORM_BLOCKS"_utf16_fly_string, constant_MAX_VERTEX_UNIFORM_BLOCKS_value, JS::Attribute::Enumerable);

    auto constant_MAX_FRAGMENT_UNIFORM_BLOCKS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A2D));

    define_direct_property("MAX_FRAGMENT_UNIFORM_BLOCKS"_utf16_fly_string, constant_MAX_FRAGMENT_UNIFORM_BLOCKS_value, JS::Attribute::Enumerable);

    auto constant_MAX_COMBINED_UNIFORM_BLOCKS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A2E));

    define_direct_property("MAX_COMBINED_UNIFORM_BLOCKS"_utf16_fly_string, constant_MAX_COMBINED_UNIFORM_BLOCKS_value, JS::Attribute::Enumerable);

    auto constant_MAX_UNIFORM_BUFFER_BINDINGS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A2F));

    define_direct_property("MAX_UNIFORM_BUFFER_BINDINGS"_utf16_fly_string, constant_MAX_UNIFORM_BUFFER_BINDINGS_value, JS::Attribute::Enumerable);

    auto constant_MAX_UNIFORM_BLOCK_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A30));

    define_direct_property("MAX_UNIFORM_BLOCK_SIZE"_utf16_fly_string, constant_MAX_UNIFORM_BLOCK_SIZE_value, JS::Attribute::Enumerable);

    auto constant_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A31));

    define_direct_property("MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS"_utf16_fly_string, constant_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A33));

    define_direct_property("MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS"_utf16_fly_string, constant_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BUFFER_OFFSET_ALIGNMENT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A34));

    define_direct_property("UNIFORM_BUFFER_OFFSET_ALIGNMENT"_utf16_fly_string, constant_UNIFORM_BUFFER_OFFSET_ALIGNMENT_value, JS::Attribute::Enumerable);

    auto constant_ACTIVE_UNIFORM_BLOCKS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A36));

    define_direct_property("ACTIVE_UNIFORM_BLOCKS"_utf16_fly_string, constant_ACTIVE_UNIFORM_BLOCKS_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_TYPE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A37));

    define_direct_property("UNIFORM_TYPE"_utf16_fly_string, constant_UNIFORM_TYPE_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A38));

    define_direct_property("UNIFORM_SIZE"_utf16_fly_string, constant_UNIFORM_SIZE_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BLOCK_INDEX_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A3A));

    define_direct_property("UNIFORM_BLOCK_INDEX"_utf16_fly_string, constant_UNIFORM_BLOCK_INDEX_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_OFFSET_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A3B));

    define_direct_property("UNIFORM_OFFSET"_utf16_fly_string, constant_UNIFORM_OFFSET_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_ARRAY_STRIDE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A3C));

    define_direct_property("UNIFORM_ARRAY_STRIDE"_utf16_fly_string, constant_UNIFORM_ARRAY_STRIDE_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_MATRIX_STRIDE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A3D));

    define_direct_property("UNIFORM_MATRIX_STRIDE"_utf16_fly_string, constant_UNIFORM_MATRIX_STRIDE_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_IS_ROW_MAJOR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A3E));

    define_direct_property("UNIFORM_IS_ROW_MAJOR"_utf16_fly_string, constant_UNIFORM_IS_ROW_MAJOR_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BLOCK_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A3F));

    define_direct_property("UNIFORM_BLOCK_BINDING"_utf16_fly_string, constant_UNIFORM_BLOCK_BINDING_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BLOCK_DATA_SIZE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A40));

    define_direct_property("UNIFORM_BLOCK_DATA_SIZE"_utf16_fly_string, constant_UNIFORM_BLOCK_DATA_SIZE_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BLOCK_ACTIVE_UNIFORMS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A42));

    define_direct_property("UNIFORM_BLOCK_ACTIVE_UNIFORMS"_utf16_fly_string, constant_UNIFORM_BLOCK_ACTIVE_UNIFORMS_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A43));

    define_direct_property("UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES"_utf16_fly_string, constant_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A44));

    define_direct_property("UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER"_utf16_fly_string, constant_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER_value, JS::Attribute::Enumerable);

    auto constant_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8A46));

    define_direct_property("UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER"_utf16_fly_string, constant_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER_value, JS::Attribute::Enumerable);

    auto constant_INVALID_INDEX_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0xFFFFFFFF));

    define_direct_property("INVALID_INDEX"_utf16_fly_string, constant_INVALID_INDEX_value, JS::Attribute::Enumerable);

    auto constant_MAX_VERTEX_OUTPUT_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9122));

    define_direct_property("MAX_VERTEX_OUTPUT_COMPONENTS"_utf16_fly_string, constant_MAX_VERTEX_OUTPUT_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_MAX_FRAGMENT_INPUT_COMPONENTS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9125));

    define_direct_property("MAX_FRAGMENT_INPUT_COMPONENTS"_utf16_fly_string, constant_MAX_FRAGMENT_INPUT_COMPONENTS_value, JS::Attribute::Enumerable);

    auto constant_MAX_SERVER_WAIT_TIMEOUT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9111));

    define_direct_property("MAX_SERVER_WAIT_TIMEOUT"_utf16_fly_string, constant_MAX_SERVER_WAIT_TIMEOUT_value, JS::Attribute::Enumerable);

    auto constant_OBJECT_TYPE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9112));

    define_direct_property("OBJECT_TYPE"_utf16_fly_string, constant_OBJECT_TYPE_value, JS::Attribute::Enumerable);

    auto constant_SYNC_CONDITION_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9113));

    define_direct_property("SYNC_CONDITION"_utf16_fly_string, constant_SYNC_CONDITION_value, JS::Attribute::Enumerable);

    auto constant_SYNC_STATUS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9114));

    define_direct_property("SYNC_STATUS"_utf16_fly_string, constant_SYNC_STATUS_value, JS::Attribute::Enumerable);

    auto constant_SYNC_FLAGS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9115));

    define_direct_property("SYNC_FLAGS"_utf16_fly_string, constant_SYNC_FLAGS_value, JS::Attribute::Enumerable);

    auto constant_SYNC_FENCE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9116));

    define_direct_property("SYNC_FENCE"_utf16_fly_string, constant_SYNC_FENCE_value, JS::Attribute::Enumerable);

    auto constant_SYNC_GPU_COMMANDS_COMPLETE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9117));

    define_direct_property("SYNC_GPU_COMMANDS_COMPLETE"_utf16_fly_string, constant_SYNC_GPU_COMMANDS_COMPLETE_value, JS::Attribute::Enumerable);

    auto constant_UNSIGNALED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9118));

    define_direct_property("UNSIGNALED"_utf16_fly_string, constant_UNSIGNALED_value, JS::Attribute::Enumerable);

    auto constant_SIGNALED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9119));

    define_direct_property("SIGNALED"_utf16_fly_string, constant_SIGNALED_value, JS::Attribute::Enumerable);

    auto constant_ALREADY_SIGNALED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x911A));

    define_direct_property("ALREADY_SIGNALED"_utf16_fly_string, constant_ALREADY_SIGNALED_value, JS::Attribute::Enumerable);

    auto constant_TIMEOUT_EXPIRED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x911B));

    define_direct_property("TIMEOUT_EXPIRED"_utf16_fly_string, constant_TIMEOUT_EXPIRED_value, JS::Attribute::Enumerable);

    auto constant_CONDITION_SATISFIED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x911C));

    define_direct_property("CONDITION_SATISFIED"_utf16_fly_string, constant_CONDITION_SATISFIED_value, JS::Attribute::Enumerable);

    auto constant_WAIT_FAILED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x911D));

    define_direct_property("WAIT_FAILED"_utf16_fly_string, constant_WAIT_FAILED_value, JS::Attribute::Enumerable);

    auto constant_SYNC_FLUSH_COMMANDS_BIT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x00000001));

    define_direct_property("SYNC_FLUSH_COMMANDS_BIT"_utf16_fly_string, constant_SYNC_FLUSH_COMMANDS_BIT_value, JS::Attribute::Enumerable);

    auto constant_VERTEX_ATTRIB_ARRAY_DIVISOR_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x88FE));

    define_direct_property("VERTEX_ATTRIB_ARRAY_DIVISOR"_utf16_fly_string, constant_VERTEX_ATTRIB_ARRAY_DIVISOR_value, JS::Attribute::Enumerable);

    auto constant_ANY_SAMPLES_PASSED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8C2F));

    define_direct_property("ANY_SAMPLES_PASSED"_utf16_fly_string, constant_ANY_SAMPLES_PASSED_value, JS::Attribute::Enumerable);

    auto constant_ANY_SAMPLES_PASSED_CONSERVATIVE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D6A));

    define_direct_property("ANY_SAMPLES_PASSED_CONSERVATIVE"_utf16_fly_string, constant_ANY_SAMPLES_PASSED_CONSERVATIVE_value, JS::Attribute::Enumerable);

    auto constant_SAMPLER_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8919));

    define_direct_property("SAMPLER_BINDING"_utf16_fly_string, constant_SAMPLER_BINDING_value, JS::Attribute::Enumerable);

    auto constant_RGB10_A2UI_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x906F));

    define_direct_property("RGB10_A2UI"_utf16_fly_string, constant_RGB10_A2UI_value, JS::Attribute::Enumerable);

    auto constant_INT_2_10_10_10_REV_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D9F));

    define_direct_property("INT_2_10_10_10_REV"_utf16_fly_string, constant_INT_2_10_10_10_REV_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8E22));

    define_direct_property("TRANSFORM_FEEDBACK"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_PAUSED_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8E23));

    define_direct_property("TRANSFORM_FEEDBACK_PAUSED"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_PAUSED_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_ACTIVE_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8E24));

    define_direct_property("TRANSFORM_FEEDBACK_ACTIVE"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_ACTIVE_value, JS::Attribute::Enumerable);

    auto constant_TRANSFORM_FEEDBACK_BINDING_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8E25));

    define_direct_property("TRANSFORM_FEEDBACK_BINDING"_utf16_fly_string, constant_TRANSFORM_FEEDBACK_BINDING_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_IMMUTABLE_FORMAT_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x912F));

    define_direct_property("TEXTURE_IMMUTABLE_FORMAT"_utf16_fly_string, constant_TEXTURE_IMMUTABLE_FORMAT_value, JS::Attribute::Enumerable);

    auto constant_MAX_ELEMENT_INDEX_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x8D6B));

    define_direct_property("MAX_ELEMENT_INDEX"_utf16_fly_string, constant_MAX_ELEMENT_INDEX_value, JS::Attribute::Enumerable);

    auto constant_TEXTURE_IMMUTABLE_LEVELS_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x82DF));

    define_direct_property("TEXTURE_IMMUTABLE_LEVELS"_utf16_fly_string, constant_TEXTURE_IMMUTABLE_LEVELS_value, JS::Attribute::Enumerable);

    auto constant_TIMEOUT_IGNORED_value = JS::Value(static_cast<double>(-1));

    define_direct_property("TIMEOUT_IGNORED"_utf16_fly_string, constant_TIMEOUT_IGNORED_value, JS::Attribute::Enumerable);

    auto constant_MAX_CLIENT_WAIT_TIMEOUT_WEBGL_value = JS::Value(static_cast<WebIDL::UnsignedLong>(0x9247));

    define_direct_property("MAX_CLIENT_WAIT_TIMEOUT_WEBGL"_utf16_fly_string, constant_MAX_CLIENT_WAIT_TIMEOUT_WEBGL_value, JS::Attribute::Enumerable);

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

    define_native_function(realm, "copyBufferSubData"_utf16_fly_string, copy_buffer_sub_data, 5, default_attributes);

    define_native_function(realm, "getBufferSubData"_utf16_fly_string, get_buffer_sub_data, 3, default_attributes);

    define_native_function(realm, "blitFramebuffer"_utf16_fly_string, blit_framebuffer, 10, default_attributes);

    define_native_function(realm, "framebufferTextureLayer"_utf16_fly_string, framebuffer_texture_layer, 5, default_attributes);

    define_native_function(realm, "invalidateFramebuffer"_utf16_fly_string, invalidate_framebuffer, 2, default_attributes);

    define_native_function(realm, "invalidateSubFramebuffer"_utf16_fly_string, invalidate_sub_framebuffer, 6, default_attributes);

    define_native_function(realm, "readBuffer"_utf16_fly_string, read_buffer, 1, default_attributes);

    define_native_function(realm, "getInternalformatParameter"_utf16_fly_string, get_internalformat_parameter, 3, default_attributes);

    define_native_function(realm, "renderbufferStorageMultisample"_utf16_fly_string, renderbuffer_storage_multisample, 5, default_attributes);

    define_native_function(realm, "texStorage2D"_utf16_fly_string, tex_storage2d, 5, default_attributes);

    define_native_function(realm, "texStorage3D"_utf16_fly_string, tex_storage3d, 6, default_attributes);

    define_native_function(realm, "texImage3D"_utf16_fly_string, tex_image3d, 10, default_attributes);

    define_native_function(realm, "texSubImage3D"_utf16_fly_string, tex_sub_image3d, 11, default_attributes);

    define_native_function(realm, "compressedTexImage3D"_utf16_fly_string, compressed_tex_image3d, 8, default_attributes);

    define_native_function(realm, "compressedTexSubImage3D"_utf16_fly_string, compressed_tex_sub_image3d, 10, default_attributes);

    define_native_function(realm, "uniform1ui"_utf16_fly_string, uniform1ui, 2, default_attributes);

    define_native_function(realm, "uniform2ui"_utf16_fly_string, uniform2ui, 3, default_attributes);

    define_native_function(realm, "uniform3ui"_utf16_fly_string, uniform3ui, 4, default_attributes);

    define_native_function(realm, "uniform4ui"_utf16_fly_string, uniform4ui, 5, default_attributes);

    define_native_function(realm, "uniform1uiv"_utf16_fly_string, uniform1uiv, 2, default_attributes);

    define_native_function(realm, "uniform2uiv"_utf16_fly_string, uniform2uiv, 2, default_attributes);

    define_native_function(realm, "uniform3uiv"_utf16_fly_string, uniform3uiv, 2, default_attributes);

    define_native_function(realm, "uniform4uiv"_utf16_fly_string, uniform4uiv, 2, default_attributes);

    define_native_function(realm, "uniformMatrix3x2fv"_utf16_fly_string, uniform_matrix3x2fv, 3, default_attributes);

    define_native_function(realm, "uniformMatrix4x2fv"_utf16_fly_string, uniform_matrix4x2fv, 3, default_attributes);

    define_native_function(realm, "uniformMatrix2x3fv"_utf16_fly_string, uniform_matrix2x3fv, 3, default_attributes);

    define_native_function(realm, "uniformMatrix4x3fv"_utf16_fly_string, uniform_matrix4x3fv, 3, default_attributes);

    define_native_function(realm, "uniformMatrix2x4fv"_utf16_fly_string, uniform_matrix2x4fv, 3, default_attributes);

    define_native_function(realm, "uniformMatrix3x4fv"_utf16_fly_string, uniform_matrix3x4fv, 3, default_attributes);

    define_native_function(realm, "vertexAttribI4i"_utf16_fly_string, vertex_attrib_i4i, 5, default_attributes);

    define_native_function(realm, "vertexAttribI4iv"_utf16_fly_string, vertex_attrib_i4iv, 2, default_attributes);

    define_native_function(realm, "vertexAttribI4ui"_utf16_fly_string, vertex_attrib_i4ui, 5, default_attributes);

    define_native_function(realm, "vertexAttribI4uiv"_utf16_fly_string, vertex_attrib_i4uiv, 2, default_attributes);

    define_native_function(realm, "vertexAttribIPointer"_utf16_fly_string, vertex_attrib_i_pointer, 5, default_attributes);

    define_native_function(realm, "vertexAttribDivisor"_utf16_fly_string, vertex_attrib_divisor, 2, default_attributes);

    define_native_function(realm, "drawArraysInstanced"_utf16_fly_string, draw_arrays_instanced, 4, default_attributes);

    define_native_function(realm, "drawElementsInstanced"_utf16_fly_string, draw_elements_instanced, 5, default_attributes);

    define_native_function(realm, "drawRangeElements"_utf16_fly_string, draw_range_elements, 6, default_attributes);

    define_native_function(realm, "drawBuffers"_utf16_fly_string, draw_buffers, 1, default_attributes);

    define_native_function(realm, "clearBufferfv"_utf16_fly_string, clear_bufferfv, 3, default_attributes);

    define_native_function(realm, "clearBufferiv"_utf16_fly_string, clear_bufferiv, 3, default_attributes);

    define_native_function(realm, "clearBufferuiv"_utf16_fly_string, clear_bufferuiv, 3, default_attributes);

    define_native_function(realm, "clearBufferfi"_utf16_fly_string, clear_bufferfi, 4, default_attributes);

    define_native_function(realm, "createQuery"_utf16_fly_string, create_query, 0, default_attributes);

    define_native_function(realm, "deleteQuery"_utf16_fly_string, delete_query, 1, default_attributes);

    define_native_function(realm, "beginQuery"_utf16_fly_string, begin_query, 2, default_attributes);

    define_native_function(realm, "endQuery"_utf16_fly_string, end_query, 1, default_attributes);

    define_native_function(realm, "getQuery"_utf16_fly_string, get_query, 2, default_attributes);

    define_native_function(realm, "getQueryParameter"_utf16_fly_string, get_query_parameter, 2, default_attributes);

    define_native_function(realm, "createSampler"_utf16_fly_string, create_sampler, 0, default_attributes);

    define_native_function(realm, "deleteSampler"_utf16_fly_string, delete_sampler, 1, default_attributes);

    define_native_function(realm, "bindSampler"_utf16_fly_string, bind_sampler, 2, default_attributes);

    define_native_function(realm, "samplerParameteri"_utf16_fly_string, sampler_parameteri, 3, default_attributes);

    define_native_function(realm, "samplerParameterf"_utf16_fly_string, sampler_parameterf, 3, default_attributes);

    define_native_function(realm, "fenceSync"_utf16_fly_string, fence_sync, 2, default_attributes);

    define_native_function(realm, "deleteSync"_utf16_fly_string, delete_sync, 1, default_attributes);

    define_native_function(realm, "clientWaitSync"_utf16_fly_string, client_wait_sync, 3, default_attributes);

    define_native_function(realm, "waitSync"_utf16_fly_string, wait_sync, 3, default_attributes);

    define_native_function(realm, "getSyncParameter"_utf16_fly_string, get_sync_parameter, 2, default_attributes);

    define_native_function(realm, "createTransformFeedback"_utf16_fly_string, create_transform_feedback, 0, default_attributes);

    define_native_function(realm, "deleteTransformFeedback"_utf16_fly_string, delete_transform_feedback, 1, default_attributes);

    define_native_function(realm, "bindTransformFeedback"_utf16_fly_string, bind_transform_feedback, 2, default_attributes);

    define_native_function(realm, "beginTransformFeedback"_utf16_fly_string, begin_transform_feedback, 1, default_attributes);

    define_native_function(realm, "endTransformFeedback"_utf16_fly_string, end_transform_feedback, 0, default_attributes);

    define_native_function(realm, "transformFeedbackVaryings"_utf16_fly_string, transform_feedback_varyings, 3, default_attributes);

    define_native_function(realm, "pauseTransformFeedback"_utf16_fly_string, pause_transform_feedback, 0, default_attributes);

    define_native_function(realm, "resumeTransformFeedback"_utf16_fly_string, resume_transform_feedback, 0, default_attributes);

    define_native_function(realm, "bindBufferBase"_utf16_fly_string, bind_buffer_base, 3, default_attributes);

    define_native_function(realm, "bindBufferRange"_utf16_fly_string, bind_buffer_range, 5, default_attributes);

    define_native_function(realm, "getUniformIndices"_utf16_fly_string, get_uniform_indices, 2, default_attributes);

    define_native_function(realm, "getActiveUniforms"_utf16_fly_string, get_active_uniforms, 3, default_attributes);

    define_native_function(realm, "getUniformBlockIndex"_utf16_fly_string, get_uniform_block_index, 2, default_attributes);

    define_native_function(realm, "getActiveUniformBlockParameter"_utf16_fly_string, get_active_uniform_block_parameter, 3, default_attributes);

    define_native_function(realm, "getActiveUniformBlockName"_utf16_fly_string, get_active_uniform_block_name, 2, default_attributes);

    define_native_function(realm, "uniformBlockBinding"_utf16_fly_string, uniform_block_binding, 3, default_attributes);

    define_native_function(realm, "createVertexArray"_utf16_fly_string, create_vertex_array, 0, default_attributes);

    define_native_function(realm, "deleteVertexArray"_utf16_fly_string, delete_vertex_array, 1, default_attributes);

    define_native_function(realm, "isVertexArray"_utf16_fly_string, is_vertex_array, 1, default_attributes);

    define_native_function(realm, "bindVertexArray"_utf16_fly_string, bind_vertex_array, 1, default_attributes);

    define_native_function(realm, "bufferData"_utf16_fly_string, buffer_data, 3, default_attributes);

    define_native_function(realm, "bufferSubData"_utf16_fly_string, buffer_sub_data, 3, default_attributes);

    define_native_function(realm, "texImage2D"_utf16_fly_string, tex_image2d, 6, default_attributes);

    define_native_function(realm, "texSubImage2D"_utf16_fly_string, tex_sub_image2d, 7, default_attributes);

    define_native_function(realm, "compressedTexImage2D"_utf16_fly_string, compressed_tex_image2d, 7, default_attributes);

    define_native_function(realm, "compressedTexSubImage2D"_utf16_fly_string, compressed_tex_sub_image2d, 8, default_attributes);

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

    define_native_function(realm, "readPixels"_utf16_fly_string, read_pixels, 7, default_attributes);

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

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "WebGL2RenderingContext"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void WebGL2RenderingContextPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<WebGL::WebGL2RenderingContext*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<WebGL::WebGL2RenderingContext>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGL2RenderingContext");
    return static_cast<WebGL::WebGL2RenderingContext*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::canvas_getter)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::canvas_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->canvas_for_binding(); }));

    return &const_cast<HTMLCanvasElement&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::drawing_buffer_width_getter)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::drawing_buffer_width_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->drawing_buffer_width(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::drawing_buffer_height_getter)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::drawing_buffer_height_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->drawing_buffer_height(); }));

    return JS::Value(static_cast<WebIDL::Long>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::copy_buffer_sub_data)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::copy_buffer_sub_data");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "copyBufferSubData", "5");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong read_target;

    read_target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong write_target;

    write_target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::LongLong read_offset;

    read_offset = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::LongLong write_offset;

    write_offset = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::LongLong size;

    size = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->copy_buffer_sub_data(read_target, write_target, read_offset, write_offset, size); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_buffer_sub_data)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_buffer_sub_data");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getBufferSubData", "3");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::LongLong src_byte_offset;

    src_byte_offset = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    GC::Root<WebIDL::ArrayBufferView> dst_buffer;

        if (!arg2.is_object() || !(is<JS::TypedArrayBase>(arg2.as_object()) || is<JS::DataView>(arg2.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        dst_buffer = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg2.as_object()));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLongLong dst_offset;

    if (!arg3.is_undefined())

    dst_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        dst_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong length;

    if (!arg4.is_undefined())

    length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_buffer_sub_data(target, src_byte_offset, dst_buffer, dst_offset, length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::blit_framebuffer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::blit_framebuffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 10)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "blitFramebuffer", "10");

    auto arg0 = vm.argument(0);

    WebIDL::Long src_x0;

    src_x0 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long src_y0;

    src_y0 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long src_x1;

    src_x1 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long src_y1;

    src_y1 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::Long dst_x0;

    dst_x0 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg5 = vm.argument(5);

    WebIDL::Long dst_y0;

    dst_y0 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg6 = vm.argument(6);

    WebIDL::Long dst_x1;

    dst_x1 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg6, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg7 = vm.argument(7);

    WebIDL::Long dst_y1;

    dst_y1 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg7, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg8 = vm.argument(8);

    WebIDL::UnsignedLong mask;

    mask = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg8, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg9 = vm.argument(9);

    WebIDL::UnsignedLong filter;

    filter = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg9, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->blit_framebuffer(src_x0, src_y0, src_x1, src_y1, dst_x0, dst_y0, dst_x1, dst_y1, mask, filter); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::framebuffer_texture_layer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::framebuffer_texture_layer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "framebufferTextureLayer", "5");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong attachment;

    attachment = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    GC::Ptr<WebGLTexture> texture;

    if (!arg2.is_nullish()) {
        if (!arg2.is_object() || !is<WebGLTexture>(arg2.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLTexture");

        texture = &static_cast<WebGLTexture&>(arg2.as_object());
    }

    auto arg3 = vm.argument(3);

    WebIDL::Long level;

    level = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::Long layer;

    layer = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->framebuffer_texture_layer(target, attachment, texture, level, layer); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::invalidate_framebuffer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::invalidate_framebuffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "invalidateFramebuffer", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    if (!arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg1);

    auto arg1_iterator_method0 = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg1_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg1);

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *arg1_iterator_method0));

    Vector<WebIDL::UnsignedLong> attachments;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::UnsignedLong sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    attachments.append(sequence_item1);
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->invalidate_framebuffer(target, attachments); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::invalidate_sub_framebuffer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::invalidate_sub_framebuffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 6)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "invalidateSubFramebuffer", "6");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    if (!arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg1);

    auto arg1_iterator_method0 = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg1_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg1);

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *arg1_iterator_method0));

    Vector<WebIDL::UnsignedLong> attachments;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::UnsignedLong sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    attachments.append(sequence_item1);
    }

    auto arg2 = vm.argument(2);

    WebIDL::Long x;

    x = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long y;

    y = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::Long width;

    width = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg5 = vm.argument(5);

    WebIDL::Long height;

    height = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->invalidate_sub_framebuffer(target, attachments, x, y, width, height); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::read_buffer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::read_buffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "readBuffer");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong src;

    src = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->read_buffer(src); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_internalformat_parameter)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_internalformat_parameter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getInternalformatParameter", "3");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong internalformat;

    internalformat = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_internalformat_parameter(target, internalformat, pname); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::renderbuffer_storage_multisample)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::renderbuffer_storage_multisample");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "renderbufferStorageMultisample", "5");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long samples;

    samples = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong internalformat;

    internalformat = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long width;

    width = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::Long height;

    height = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->renderbuffer_storage_multisample(target, samples, internalformat, width, height); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_storage2d)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_storage2d");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "texStorage2D", "5");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long levels;

    levels = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong internalformat;

    internalformat = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long width;

    width = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::Long height;

    height = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tex_storage2d(target, levels, internalformat, width, height); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_storage3d)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_storage3d");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 6)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "texStorage3D", "6");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long levels;

    levels = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

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

    WebIDL::Long depth;

    depth = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tex_storage3d(target, levels, internalformat, width, height, depth); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_image3d0)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_image3d0");
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

    WebIDL::Long depth;

    depth = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg6 = vm.argument(6);

    WebIDL::Long border;

    border = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg6, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg7 = vm.argument(7);

    WebIDL::UnsignedLong format;

    format = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg7, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg8 = vm.argument(8);

    WebIDL::UnsignedLong type;

    type = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg8, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg9 = vm.argument(9);

    GC::Root<WebIDL::ArrayBufferView> src_data;

    if (!arg9.is_null() && !arg9.is_undefined()) {

        if (!arg9.is_object() || !(is<JS::TypedArrayBase>(arg9.as_object()) || is<JS::DataView>(arg9.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        src_data = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg9.as_object()));

    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tex_image3d(target, level, internalformat, width, height, depth, border, format, type, src_data); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_image3d1)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_image3d1");
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

    WebIDL::Long depth;

    depth = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg6 = vm.argument(6);

    WebIDL::Long border;

    border = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg6, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg7 = vm.argument(7);

    WebIDL::UnsignedLong format;

    format = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg7, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg8 = vm.argument(8);

    WebIDL::UnsignedLong type;

    type = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg8, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg9 = vm.argument(9);

    GC::Root<WebIDL::ArrayBufferView> src_data;

        if (!arg9.is_object() || !(is<JS::TypedArrayBase>(arg9.as_object()) || is<JS::DataView>(arg9.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        src_data = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg9.as_object()));

    auto arg10 = vm.argument(10);

    WebIDL::UnsignedLongLong src_offset;

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg10, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tex_image3d(target, level, internalformat, width, height, depth, border, format, type, src_data, src_offset); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_sub_image3d)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_sub_image3d");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 11)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "texSubImage3D", "11");

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

    WebIDL::Long zoffset;

    zoffset = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg5 = vm.argument(5);

    WebIDL::Long width;

    width = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg6 = vm.argument(6);

    WebIDL::Long height;

    height = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg6, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg7 = vm.argument(7);

    WebIDL::Long depth;

    depth = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg7, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg8 = vm.argument(8);

    WebIDL::UnsignedLong format;

    format = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg8, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg9 = vm.argument(9);

    WebIDL::UnsignedLong type;

    type = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg9, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg10 = vm.argument(10);

    GC::Root<WebIDL::ArrayBufferView> src_data;

    if (!arg10.is_null() && !arg10.is_undefined()) {

        if (!arg10.is_object() || !(is<JS::TypedArrayBase>(arg10.as_object()) || is<JS::DataView>(arg10.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        src_data = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg10.as_object()));

    }

    auto arg11 = vm.argument(11);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg11.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg11, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tex_sub_image3d(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, src_data, src_offset); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::compressed_tex_image3d)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::compressed_tex_image3d");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 8)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "compressedTexImage3D", "8");

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

    WebIDL::Long depth;

    depth = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg6 = vm.argument(6);

    WebIDL::Long border;

    border = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg6, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg7 = vm.argument(7);

    GC::Root<WebIDL::ArrayBufferView> src_data;

        if (!arg7.is_object() || !(is<JS::TypedArrayBase>(arg7.as_object()) || is<JS::DataView>(arg7.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        src_data = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg7.as_object()));

    auto arg8 = vm.argument(8);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg8.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg8, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg9 = vm.argument(9);

    WebIDL::UnsignedLong src_length_override;

    if (!arg9.is_undefined())

    src_length_override = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg9, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length_override = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->compressed_tex_image3d(target, level, internalformat, width, height, depth, border, src_data, src_offset, src_length_override); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::compressed_tex_sub_image3d)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::compressed_tex_sub_image3d");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 10)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "compressedTexSubImage3D", "10");

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

    WebIDL::Long zoffset;

    zoffset = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg5 = vm.argument(5);

    WebIDL::Long width;

    width = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg6 = vm.argument(6);

    WebIDL::Long height;

    height = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg6, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg7 = vm.argument(7);

    WebIDL::Long depth;

    depth = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg7, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg8 = vm.argument(8);

    WebIDL::UnsignedLong format;

    format = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg8, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg9 = vm.argument(9);

    GC::Root<WebIDL::ArrayBufferView> src_data;

        if (!arg9.is_object() || !(is<JS::TypedArrayBase>(arg9.as_object()) || is<JS::DataView>(arg9.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        src_data = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg9.as_object()));

    auto arg10 = vm.argument(10);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg10.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg10, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg11 = vm.argument(11);

    WebIDL::UnsignedLong src_length_override;

    if (!arg11.is_undefined())

    src_length_override = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg11, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length_override = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->compressed_tex_sub_image3d(target, level, xoffset, yoffset, zoffset, width, height, depth, format, src_data, src_offset, src_length_override); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform1ui)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform1ui");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform1ui", "2");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong v0;

    v0 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform1ui(location, v0); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform2ui)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform2ui");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform2ui", "3");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong v0;

    v0 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong v1;

    v1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform2ui(location, v0, v1); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform3ui)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform3ui");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform3ui", "4");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong v0;

    v0 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong v1;

    v1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong v2;

    v2 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform3ui(location, v0, v1, v2); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform4ui)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform4ui");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform4ui", "5");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong v0;

    v0 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong v1;

    v1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong v2;

    v2 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong v3;

    v3 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform4ui(location, v0, v1, v2, v3); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform1uiv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform1uiv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform1uiv", "2");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Uint32Array>, Vector<WebIDL::UnsignedLong>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (auto* typed_array = as_if<JS::Uint32Array>(arg1_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<WebIDL::UnsignedLong> data;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::UnsignedLong sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    data.append(sequence_item1);
    }


            return data;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Uint32Array>, Vector<WebIDL::UnsignedLong>> data = TRY(arg1_to_variant(arg1));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg2.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong src_length;

    if (!arg3.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform1uiv(location, data, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform2uiv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform2uiv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform2uiv", "2");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Uint32Array>, Vector<WebIDL::UnsignedLong>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (auto* typed_array = as_if<JS::Uint32Array>(arg1_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<WebIDL::UnsignedLong> data;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::UnsignedLong sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    data.append(sequence_item1);
    }


            return data;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Uint32Array>, Vector<WebIDL::UnsignedLong>> data = TRY(arg1_to_variant(arg1));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg2.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong src_length;

    if (!arg3.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform2uiv(location, data, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform3uiv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform3uiv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform3uiv", "2");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Uint32Array>, Vector<WebIDL::UnsignedLong>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (auto* typed_array = as_if<JS::Uint32Array>(arg1_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<WebIDL::UnsignedLong> data;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::UnsignedLong sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    data.append(sequence_item1);
    }


            return data;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Uint32Array>, Vector<WebIDL::UnsignedLong>> data = TRY(arg1_to_variant(arg1));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg2.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong src_length;

    if (!arg3.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform3uiv(location, data, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform4uiv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform4uiv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniform4uiv", "2");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLUniformLocation> location;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLUniformLocation>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLUniformLocation");

        location = &static_cast<WebGLUniformLocation&>(arg0.as_object());
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Uint32Array>, Vector<WebIDL::UnsignedLong>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (auto* typed_array = as_if<JS::Uint32Array>(arg1_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<WebIDL::UnsignedLong> data;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::UnsignedLong sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    data.append(sequence_item1);
    }


            return data;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Uint32Array>, Vector<WebIDL::UnsignedLong>> data = TRY(arg1_to_variant(arg1));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg2.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong src_length;

    if (!arg3.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform4uiv(location, data, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform_matrix3x2fv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform_matrix3x2fv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniformMatrix3x2fv", "3");

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

    Vector<float> data;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    data.append(sequence_item1);
    }


            return data;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> data = TRY(arg2_to_variant(arg2));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg3.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong src_length;

    if (!arg4.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform_matrix3x2fv(location, transpose, data, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform_matrix4x2fv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform_matrix4x2fv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniformMatrix4x2fv", "3");

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

    Vector<float> data;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    data.append(sequence_item1);
    }


            return data;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> data = TRY(arg2_to_variant(arg2));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg3.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong src_length;

    if (!arg4.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform_matrix4x2fv(location, transpose, data, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform_matrix2x3fv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform_matrix2x3fv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniformMatrix2x3fv", "3");

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

    Vector<float> data;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    data.append(sequence_item1);
    }


            return data;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> data = TRY(arg2_to_variant(arg2));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg3.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong src_length;

    if (!arg4.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform_matrix2x3fv(location, transpose, data, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform_matrix4x3fv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform_matrix4x3fv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniformMatrix4x3fv", "3");

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

    Vector<float> data;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    data.append(sequence_item1);
    }


            return data;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> data = TRY(arg2_to_variant(arg2));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg3.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong src_length;

    if (!arg4.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform_matrix4x3fv(location, transpose, data, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform_matrix2x4fv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform_matrix2x4fv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniformMatrix2x4fv", "3");

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

    Vector<float> data;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    data.append(sequence_item1);
    }


            return data;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> data = TRY(arg2_to_variant(arg2));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg3.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong src_length;

    if (!arg4.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform_matrix2x4fv(location, transpose, data, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform_matrix3x4fv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform_matrix3x4fv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniformMatrix3x4fv", "3");

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

    Vector<float> data;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    data.append(sequence_item1);
    }


            return data;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> data = TRY(arg2_to_variant(arg2));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg3.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong src_length;

    if (!arg4.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform_matrix3x4fv(location, transpose, data, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::vertex_attrib_i4i)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::vertex_attrib_i4i");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "vertexAttribI4i", "5");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

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

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vertex_attrib_i4i(index, x, y, z, w); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::vertex_attrib_i4iv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::vertex_attrib_i4iv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "vertexAttribI4iv", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

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

    Vector<WebIDL::Long> values;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::Long sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    values.append(sequence_item1);
    }


            return values;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Int32Array>, Vector<WebIDL::Long>> values = TRY(arg1_to_variant(arg1));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vertex_attrib_i4iv(index, values); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::vertex_attrib_i4ui)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::vertex_attrib_i4ui");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "vertexAttribI4ui", "5");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong x;

    x = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong y;

    y = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong z;

    z = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong w;

    w = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vertex_attrib_i4ui(index, x, y, z, w); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::vertex_attrib_i4uiv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::vertex_attrib_i4uiv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "vertexAttribI4uiv", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    auto arg1_to_variant = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Uint32Array>, Vector<WebIDL::UnsignedLong>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

            if (auto* typed_array = as_if<JS::Uint32Array>(arg1_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *method));

    Vector<WebIDL::UnsignedLong> values;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::UnsignedLong sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    values.append(sequence_item1);
    }


            return values;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Uint32Array>, Vector<WebIDL::UnsignedLong>> values = TRY(arg1_to_variant(arg1));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vertex_attrib_i4uiv(index, values); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::vertex_attrib_i_pointer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::vertex_attrib_i_pointer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "vertexAttribIPointer", "5");

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

    WebIDL::Long stride;

    stride = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::LongLong offset;

    offset = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vertex_attrib_i_pointer(index, size, type, stride, offset); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::vertex_attrib_divisor)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::vertex_attrib_divisor");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "vertexAttribDivisor", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong divisor;

    divisor = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->vertex_attrib_divisor(index, divisor); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::draw_arrays_instanced)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::draw_arrays_instanced");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "drawArraysInstanced", "4");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong mode;

    mode = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long first;

    first = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long count;

    count = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long instance_count;

    instance_count = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->draw_arrays_instanced(mode, first, count, instance_count); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::draw_elements_instanced)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::draw_elements_instanced");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "drawElementsInstanced", "5");

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

    auto arg4 = vm.argument(4);

    WebIDL::Long instance_count;

    instance_count = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->draw_elements_instanced(mode, count, type, offset, instance_count); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::draw_range_elements)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::draw_range_elements");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 6)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "drawRangeElements", "6");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong mode;

    mode = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong start;

    start = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong end;

    end = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::Long count;

    count = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong type;

    type = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg5 = vm.argument(5);

    WebIDL::LongLong offset;

    offset = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->draw_range_elements(mode, start, end, count, type, offset); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::draw_buffers)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::draw_buffers");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "drawBuffers");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg0);

    auto arg0_iterator_method0 = TRY(arg0.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg0_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg0);

    auto arg0_iterator1 = TRY(JS::get_iterator_from_method(vm, arg0, *arg0_iterator_method0));

    Vector<WebIDL::UnsignedLong> buffers;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg0_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::UnsignedLong sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    buffers.append(sequence_item1);
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->draw_buffers(buffers); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::clear_bufferfv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::clear_bufferfv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "clearBufferfv", "3");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong buffer;

    buffer = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long drawbuffer;

    drawbuffer = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

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

    Vector<float> values;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg2_iterator1));
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

    Variant<GC::Root<JS::Float32Array>, Vector<float>> values = TRY(arg2_to_variant(arg2));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg3.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear_bufferfv(buffer, drawbuffer, values, src_offset); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::clear_bufferiv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::clear_bufferiv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "clearBufferiv", "3");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong buffer;

    buffer = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long drawbuffer;

    drawbuffer = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    auto arg2_to_variant = [&vm, &realm](JS::Value arg2) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Int32Array>, Vector<WebIDL::Long>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg2.is_object()) {
            [[maybe_unused]] auto& arg2_object = arg2.as_object();

            if (auto* typed_array = as_if<JS::Int32Array>(arg2_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg2.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg2_iterator1 = TRY(JS::get_iterator_from_method(vm, arg2, *method));

    Vector<WebIDL::Long> values;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::Long sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    values.append(sequence_item1);
    }


            return values;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Int32Array>, Vector<WebIDL::Long>> values = TRY(arg2_to_variant(arg2));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg3.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear_bufferiv(buffer, drawbuffer, values, src_offset); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::clear_bufferuiv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::clear_bufferuiv");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "clearBufferuiv", "3");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong buffer;

    buffer = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long drawbuffer;

    drawbuffer = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    auto arg2_to_variant = [&vm, &realm](JS::Value arg2) -> JS::ThrowCompletionOr<Variant<GC::Root<JS::Uint32Array>, Vector<WebIDL::UnsignedLong>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg2.is_object()) {
            [[maybe_unused]] auto& arg2_object = arg2.as_object();

            if (auto* typed_array = as_if<JS::Uint32Array>(arg2_object))
                return GC::make_root(*typed_array);

        auto method = TRY(arg2.get_method(vm, vm.well_known_symbol_iterator()));

        if (method) {

    auto arg2_iterator1 = TRY(JS::get_iterator_from_method(vm, arg2, *method));

    Vector<WebIDL::UnsignedLong> values;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::UnsignedLong sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    values.append(sequence_item1);
    }


            return values;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Uint32Array>, Vector<WebIDL::UnsignedLong>> values = TRY(arg2_to_variant(arg2));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg3.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear_bufferuiv(buffer, drawbuffer, values, src_offset); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::clear_bufferfi)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::clear_bufferfi");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 4)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "clearBufferfi", "4");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong buffer;

    buffer = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::Long drawbuffer;

    drawbuffer = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    float depth = TRY(arg2.to_double(vm));

    auto arg3 = vm.argument(3);

    WebIDL::Long stencil;

    stencil = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear_bufferfi(buffer, drawbuffer, depth, stencil); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::create_query)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::create_query");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_query(); }));

    return &const_cast<WebGLQuery&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::delete_query)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::delete_query");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "deleteQuery");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLQuery> query;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLQuery>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLQuery");

        query = &static_cast<WebGLQuery&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_query(query); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::begin_query)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::begin_query");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "beginQuery", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<WebGLQuery>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLQuery");

    auto& query = static_cast<WebGLQuery&>(arg1.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->begin_query(target, query); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::end_query)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::end_query");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "endQuery");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->end_query(target); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_query)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_query");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getQuery", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_query(target, pname); }));

    if (retval) {

    return &const_cast<WebGLQuery&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_query_parameter)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_query_parameter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getQueryParameter", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLQuery>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLQuery");

    auto& query = static_cast<WebGLQuery&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_query_parameter(query, pname); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::create_sampler)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::create_sampler");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_sampler(); }));

    return &const_cast<WebGLSampler&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::delete_sampler)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::delete_sampler");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "deleteSampler");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLSampler> sampler;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLSampler>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLSampler");

        sampler = &static_cast<WebGLSampler&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_sampler(sampler); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::bind_sampler)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::bind_sampler");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "bindSampler", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong unit;

    unit = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    GC::Ptr<WebGLSampler> sampler;

    if (!arg1.is_nullish()) {
        if (!arg1.is_object() || !is<WebGLSampler>(arg1.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLSampler");

        sampler = &static_cast<WebGLSampler&>(arg1.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bind_sampler(unit, sampler); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::sampler_parameteri)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::sampler_parameteri");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "samplerParameteri", "3");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLSampler>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLSampler");

    auto& sampler = static_cast<WebGLSampler&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::Long param;

    param = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->sampler_parameteri(sampler, pname, param); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::sampler_parameterf)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::sampler_parameterf");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "samplerParameterf", "3");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLSampler>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLSampler");

    auto& sampler = static_cast<WebGLSampler&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    float param = TRY(arg2.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->sampler_parameterf(sampler, pname, param); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::fence_sync)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::fence_sync");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "fenceSync", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong condition;

    condition = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong flags;

    flags = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->fence_sync(condition, flags); }));

    if (retval) {

    return &const_cast<WebGLSync&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::delete_sync)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::delete_sync");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "deleteSync");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLSync> sync;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLSync>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLSync");

        sync = &static_cast<WebGLSync&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_sync(sync); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::client_wait_sync)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::client_wait_sync");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "clientWaitSync", "3");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLSync>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLSync");

    auto& sync = static_cast<WebGLSync&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong flags;

    flags = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLongLong timeout;

    timeout = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->client_wait_sync(sync, flags, timeout); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::wait_sync)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::wait_sync");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "waitSync", "3");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLSync>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLSync");

    auto& sync = static_cast<WebGLSync&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong flags;

    flags = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::LongLong timeout;

    timeout = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->wait_sync(sync, flags, timeout); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_sync_parameter)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_sync_parameter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getSyncParameter", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLSync>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLSync");

    auto& sync = static_cast<WebGLSync&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_sync_parameter(sync, pname); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::create_transform_feedback)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::create_transform_feedback");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_transform_feedback(); }));

    return &const_cast<WebGLTransformFeedback&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::delete_transform_feedback)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::delete_transform_feedback");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "deleteTransformFeedback");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLTransformFeedback> tf;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLTransformFeedback>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLTransformFeedback");

        tf = &static_cast<WebGLTransformFeedback&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_transform_feedback(tf); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::bind_transform_feedback)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::bind_transform_feedback");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "bindTransformFeedback", "2");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    GC::Ptr<WebGLTransformFeedback> tf;

    if (!arg1.is_nullish()) {
        if (!arg1.is_object() || !is<WebGLTransformFeedback>(arg1.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLTransformFeedback");

        tf = &static_cast<WebGLTransformFeedback&>(arg1.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bind_transform_feedback(target, tf); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::begin_transform_feedback)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::begin_transform_feedback");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "beginTransformFeedback");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong primitive_mode;

    primitive_mode = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->begin_transform_feedback(primitive_mode); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::end_transform_feedback)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::end_transform_feedback");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->end_transform_feedback(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::transform_feedback_varyings)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::transform_feedback_varyings");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "transformFeedbackVaryings", "3");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    if (!arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg1);

    auto arg1_iterator_method0 = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg1_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg1);

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *arg1_iterator_method0));

    Vector<String> varyings;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    String sequence_item1;
    if (!false || !next_item1.is_null()) {
        sequence_item1 = TRY(WebIDL::to_string(vm, next_item1));
    }

    varyings.append(sequence_item1);
    }

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong buffer_mode;

    buffer_mode = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->transform_feedback_varyings(program, varyings, buffer_mode); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::pause_transform_feedback)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::pause_transform_feedback");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->pause_transform_feedback(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::resume_transform_feedback)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::resume_transform_feedback");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->resume_transform_feedback(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::bind_buffer_base)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::bind_buffer_base");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "bindBufferBase", "3");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    GC::Ptr<WebGLBuffer> buffer;

    if (!arg2.is_nullish()) {
        if (!arg2.is_object() || !is<WebGLBuffer>(arg2.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLBuffer");

        buffer = &static_cast<WebGLBuffer&>(arg2.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bind_buffer_base(target, index, buffer); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::bind_buffer_range)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::bind_buffer_range");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 5)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "bindBufferRange", "5");

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong index;

    index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    GC::Ptr<WebGLBuffer> buffer;

    if (!arg2.is_nullish()) {
        if (!arg2.is_object() || !is<WebGLBuffer>(arg2.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLBuffer");

        buffer = &static_cast<WebGLBuffer&>(arg2.as_object());
    }

    auto arg3 = vm.argument(3);

    WebIDL::LongLong offset;

    offset = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::LongLong size;

    size = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bind_buffer_range(target, index, buffer, offset, size); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_uniform_indices)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_uniform_indices");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getUniformIndices", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    if (!arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg1);

    auto arg1_iterator_method0 = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg1_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg1);

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *arg1_iterator_method0));

    Vector<String> uniform_names;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    String sequence_item1;
    if (!false || !next_item1.is_null()) {
        sequence_item1 = TRY(WebIDL::to_string(vm, next_item1));
    }

    uniform_names.append(sequence_item1);
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_uniform_indices(program, uniform_names); }));

    if (retval.has_value()) {

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    auto& retval_non_optional = retval.value();
    for (size_t i0 = 0; i0 < retval_non_optional.size(); ++i0) {
        auto& element0 = retval_non_optional.at(i0);
JS::Value wrapped_element0;

    wrapped_element0 = JS::Value(static_cast<WebIDL::UnsignedLong>(element0));

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_active_uniforms)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_active_uniforms");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getActiveUniforms", "3");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    if (!arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg1);

    auto arg1_iterator_method0 = TRY(arg1.get_method(vm, vm.well_known_symbol_iterator()));
    if (!arg1_iterator_method0)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotIterable, arg1);

    auto arg1_iterator1 = TRY(JS::get_iterator_from_method(vm, arg1, *arg1_iterator_method0));

    Vector<WebIDL::UnsignedLong> uniform_indices;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg1_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    WebIDL::UnsignedLong sequence_item1;

    sequence_item1 = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, next_item1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    uniform_indices.append(sequence_item1);
    }

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_active_uniforms(program, uniform_indices, pname); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_uniform_block_index)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_uniform_block_index");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getUniformBlockIndex", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    String uniform_block_name;
    if (!false || !arg1.is_null()) {
        uniform_block_name = TRY(WebIDL::to_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_uniform_block_index(program, uniform_block_name); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_active_uniform_block_parameter)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_active_uniform_block_parameter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getActiveUniformBlockParameter", "3");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong uniform_block_index;

    uniform_block_index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong pname;

    pname = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_active_uniform_block_parameter(program, uniform_block_index, pname); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_active_uniform_block_name)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_active_uniform_block_name");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "getActiveUniformBlockName", "2");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong uniform_block_index;

    uniform_block_index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_active_uniform_block_name(program, uniform_block_index); }));

    if (retval.has_value()) {

    return JS::PrimitiveString::create(vm, const_cast<decltype(retval)&>(retval).release_value());

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform_block_binding)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform_block_binding");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 3)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "uniformBlockBinding", "3");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<WebGLProgram>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLProgram");

    auto& program = static_cast<WebGLProgram&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong uniform_block_index;

    uniform_block_index = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong uniform_block_binding;

    uniform_block_binding = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform_block_binding(program, uniform_block_index, uniform_block_binding); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::create_vertex_array)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::create_vertex_array");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_vertex_array(); }));

    return &const_cast<WebGLVertexArrayObject&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::delete_vertex_array)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::delete_vertex_array");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "deleteVertexArray");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLVertexArrayObject> vertex_array;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLVertexArrayObject>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLVertexArrayObject");

        vertex_array = &static_cast<WebGLVertexArrayObject&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->delete_vertex_array(vertex_array); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::is_vertex_array)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::is_vertex_array");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "isVertexArray");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLVertexArrayObject> vertex_array;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLVertexArrayObject>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLVertexArrayObject");

        vertex_array = &static_cast<WebGLVertexArrayObject&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_vertex_array(vertex_array); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::bind_vertex_array)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::bind_vertex_array");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "bindVertexArray");

    auto arg0 = vm.argument(0);

    GC::Ptr<WebGLVertexArrayObject> array;

    if (!arg0.is_nullish()) {
        if (!arg0.is_object() || !is<WebGLVertexArrayObject>(arg0.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WebGLVertexArrayObject");

        array = &static_cast<WebGLVertexArrayObject&>(arg0.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->bind_vertex_array(array); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::buffer_data0)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::buffer_data0");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::buffer_data1)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::buffer_data1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    GC::Root<WebIDL::BufferSource> src_data;

    if (!arg1.is_object() || !(is<JS::TypedArrayBase>(arg1.as_object()) || is<JS::ArrayBuffer>(arg1.as_object()) || is<JS::DataView>(arg1.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BufferSource");

    src_data = GC::make_root(realm.create<WebIDL::BufferSource>(arg1.as_object()));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong usage;

    usage = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->buffer_data(target, src_data, usage); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::buffer_sub_data0)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::buffer_sub_data0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::LongLong dst_byte_offset;

    dst_byte_offset = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    GC::Root<WebIDL::BufferSource> src_data;

    if (!arg2.is_object() || !(is<JS::TypedArrayBase>(arg2.as_object()) || is<JS::ArrayBuffer>(arg2.as_object()) || is<JS::DataView>(arg2.as_object())))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "BufferSource");

    src_data = GC::make_root(realm.create<WebIDL::BufferSource>(arg2.as_object()));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->buffer_sub_data(target, dst_byte_offset, src_data); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::buffer_data2)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::buffer_data2");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    GC::Root<WebIDL::ArrayBufferView> src_data;

        if (!arg1.is_object() || !(is<JS::TypedArrayBase>(arg1.as_object()) || is<JS::DataView>(arg1.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        src_data = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg1.as_object()));

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLong usage;

    usage = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLongLong src_offset;

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong length;

    if (!arg4.is_undefined())

    length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->buffer_data(target, src_data, usage, src_offset, length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::buffer_sub_data1)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::buffer_sub_data1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    WebIDL::UnsignedLong target;

    target = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg0, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg1 = vm.argument(1);

    WebIDL::LongLong dst_byte_offset;

    dst_byte_offset = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg2 = vm.argument(2);

    GC::Root<WebIDL::ArrayBufferView> src_data;

        if (!arg2.is_object() || !(is<JS::TypedArrayBase>(arg2.as_object()) || is<JS::DataView>(arg2.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        src_data = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg2.as_object()));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLongLong src_offset;

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong length;

    if (!arg4.is_undefined())

    length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->buffer_sub_data(target, dst_byte_offset, src_data, src_offset, length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_image2d0)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_image2d0");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_image2d1)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_image2d1");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_sub_image2d0)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_sub_image2d0");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_sub_image2d1)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_sub_image2d1");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_image2d2)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_image2d2");
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

    auto arg8_to_variant = [&vm, &realm](JS::Value arg8) -> JS::ThrowCompletionOr<Variant<GC::Root<ImageBitmap>, GC::Root<ImageData>, GC::Root<HTMLImageElement>, GC::Root<HTMLCanvasElement>, GC::Root<HTMLVideoElement>, GC::Root<OffscreenCanvas>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg8.is_object()) {
            [[maybe_unused]] auto& arg8_object = arg8.as_object();

            if (is<PlatformObject>(arg8_object)) {

                if (auto* arg8_result = as_if<ImageBitmap>(arg8_object))
                    return GC::make_root(*arg8_result);

                if (auto* arg8_result = as_if<ImageData>(arg8_object))
                    return GC::make_root(*arg8_result);

                if (auto* arg8_result = as_if<HTMLImageElement>(arg8_object))
                    return GC::make_root(*arg8_result);

                if (auto* arg8_result = as_if<HTMLCanvasElement>(arg8_object))
                    return GC::make_root(*arg8_result);

                if (auto* arg8_result = as_if<HTMLVideoElement>(arg8_object))
                    return GC::make_root(*arg8_result);

                if (auto* arg8_result = as_if<OffscreenCanvas>(arg8_object))
                    return GC::make_root(*arg8_result);

            }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<ImageBitmap>, GC::Root<ImageData>, GC::Root<HTMLImageElement>, GC::Root<HTMLCanvasElement>, GC::Root<HTMLVideoElement>, GC::Root<OffscreenCanvas>> source = TRY(arg8_to_variant(arg8));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tex_image2d(target, level, internalformat, width, height, border, format, type, source); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_image2d3)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_image2d3");
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

    GC::Root<WebIDL::ArrayBufferView> src_data;

        if (!arg8.is_object() || !(is<JS::TypedArrayBase>(arg8.as_object()) || is<JS::DataView>(arg8.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        src_data = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg8.as_object()));

    auto arg9 = vm.argument(9);

    WebIDL::UnsignedLongLong src_offset;

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg9, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tex_image2d(target, level, internalformat, width, height, border, format, type, src_data, src_offset); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_sub_image2d2)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_sub_image2d2");
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

    auto arg8_to_variant = [&vm, &realm](JS::Value arg8) -> JS::ThrowCompletionOr<Variant<GC::Root<ImageBitmap>, GC::Root<ImageData>, GC::Root<HTMLImageElement>, GC::Root<HTMLCanvasElement>, GC::Root<HTMLVideoElement>, GC::Root<OffscreenCanvas>>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg8.is_object()) {
            [[maybe_unused]] auto& arg8_object = arg8.as_object();

            if (is<PlatformObject>(arg8_object)) {

                if (auto* arg8_result = as_if<ImageBitmap>(arg8_object))
                    return GC::make_root(*arg8_result);

                if (auto* arg8_result = as_if<ImageData>(arg8_object))
                    return GC::make_root(*arg8_result);

                if (auto* arg8_result = as_if<HTMLImageElement>(arg8_object))
                    return GC::make_root(*arg8_result);

                if (auto* arg8_result = as_if<HTMLCanvasElement>(arg8_object))
                    return GC::make_root(*arg8_result);

                if (auto* arg8_result = as_if<HTMLVideoElement>(arg8_object))
                    return GC::make_root(*arg8_result);

                if (auto* arg8_result = as_if<OffscreenCanvas>(arg8_object))
                    return GC::make_root(*arg8_result);

            }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<ImageBitmap>, GC::Root<ImageData>, GC::Root<HTMLImageElement>, GC::Root<HTMLCanvasElement>, GC::Root<HTMLVideoElement>, GC::Root<OffscreenCanvas>> source = TRY(arg8_to_variant(arg8));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tex_sub_image2d(target, level, xoffset, yoffset, width, height, format, type, source); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_sub_image2d3)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_sub_image2d3");
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

    GC::Root<WebIDL::ArrayBufferView> src_data;

        if (!arg8.is_object() || !(is<JS::TypedArrayBase>(arg8.as_object()) || is<JS::DataView>(arg8.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        src_data = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg8.as_object()));

    auto arg9 = vm.argument(9);

    WebIDL::UnsignedLongLong src_offset;

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg9, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->tex_sub_image2d(target, level, xoffset, yoffset, width, height, format, type, src_data, src_offset); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::compressed_tex_image2d)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::compressed_tex_image2d");
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

    GC::Root<WebIDL::ArrayBufferView> src_data;

        if (!arg6.is_object() || !(is<JS::TypedArrayBase>(arg6.as_object()) || is<JS::DataView>(arg6.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        src_data = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg6.as_object()));

    auto arg7 = vm.argument(7);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg7.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg7, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg8 = vm.argument(8);

    WebIDL::UnsignedLong src_length_override;

    if (!arg8.is_undefined())

    src_length_override = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg8, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length_override = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->compressed_tex_image2d(target, level, internalformat, width, height, border, src_data, src_offset, src_length_override); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::compressed_tex_sub_image2d)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::compressed_tex_sub_image2d");
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

    GC::Root<WebIDL::ArrayBufferView> src_data;

        if (!arg7.is_object() || !(is<JS::TypedArrayBase>(arg7.as_object()) || is<JS::DataView>(arg7.as_object())))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ArrayBufferView");

        src_data = GC::make_root(realm.create<WebIDL::ArrayBufferView>(arg7.as_object()));

    auto arg8 = vm.argument(8);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg8.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg8, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg9 = vm.argument(9);

    WebIDL::UnsignedLong src_length_override;

    if (!arg9.is_undefined())

    src_length_override = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg9, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length_override = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->compressed_tex_sub_image2d(target, level, xoffset, yoffset, width, height, format, src_data, src_offset, src_length_override); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform1fv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform1fv");
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

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg2.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong src_length;

    if (!arg3.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform1fv(location, v, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform2fv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform2fv");
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

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg2.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong src_length;

    if (!arg3.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform2fv(location, v, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform3fv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform3fv");
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

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg2.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong src_length;

    if (!arg3.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform3fv(location, v, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform4fv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform4fv");
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

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg2.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong src_length;

    if (!arg3.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform4fv(location, v, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform1iv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform1iv");
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

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg2.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong src_length;

    if (!arg3.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform1iv(location, v, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform2iv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform2iv");
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

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg2.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong src_length;

    if (!arg3.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform2iv(location, v, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform3iv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform3iv");
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

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg2.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong src_length;

    if (!arg3.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform3iv(location, v, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform4iv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform4iv");
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

    auto arg2 = vm.argument(2);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg2.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg2, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLong src_length;

    if (!arg3.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform4iv(location, v, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform_matrix2fv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform_matrix2fv");
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

    Vector<float> data;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    data.append(sequence_item1);
    }


            return data;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> data = TRY(arg2_to_variant(arg2));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg3.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong src_length;

    if (!arg4.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform_matrix2fv(location, transpose, data, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform_matrix3fv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform_matrix3fv");
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

    Vector<float> data;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    data.append(sequence_item1);
    }


            return data;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> data = TRY(arg2_to_variant(arg2));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg3.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong src_length;

    if (!arg4.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform_matrix3fv(location, transpose, data, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform_matrix4fv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform_matrix4fv");
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

    Vector<float> data;

    for (;;) {
        auto next1 = TRY(JS::iterator_step(vm, arg2_iterator1));
        if (!next1.has<JS::IterationResult>())
            break;

        auto next_item1 = TRY(next1.get<JS::IterationResult>().value);

    float sequence_item1 = TRY(next_item1.to_double(vm));

    data.append(sequence_item1);
    }


            return data;
        }

        }

        return vm.throw_completion<JS::TypeError>("No union types matched"sv);

    };

    Variant<GC::Root<JS::Float32Array>, Vector<float>> data = TRY(arg2_to_variant(arg2));

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedLongLong src_offset;

    if (!arg3.is_undefined())

    src_offset = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLongLong>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_offset = static_cast<WebIDL::UnsignedLongLong>(0);

    auto arg4 = vm.argument(4);

    WebIDL::UnsignedLong src_length;

    if (!arg4.is_undefined())

    src_length = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        src_length = static_cast<WebIDL::UnsignedLong>(0);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->uniform_matrix4fv(location, transpose, data, src_offset, src_length); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::read_pixels0)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::read_pixels0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::read_pixels1)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::read_pixels1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

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

    WebIDL::LongLong offset;

    offset = TRY(WebIDL::convert_to_int<WebIDL::LongLong>(vm, arg6, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->read_pixels(x, y, width, height, format, type, offset); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_context_attributes)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_context_attributes");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::is_context_lost)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::is_context_lost");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->is_context_lost(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_supported_extensions)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_supported_extensions");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_extension)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_extension");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::active_texture)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::active_texture");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::attach_shader)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::attach_shader");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::bind_attrib_location)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::bind_attrib_location");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::bind_buffer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::bind_buffer");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::bind_framebuffer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::bind_framebuffer");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::bind_renderbuffer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::bind_renderbuffer");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::bind_texture)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::bind_texture");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::blend_color)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::blend_color");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::blend_equation)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::blend_equation");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::blend_equation_separate)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::blend_equation_separate");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::blend_func)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::blend_func");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::blend_func_separate)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::blend_func_separate");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::check_framebuffer_status)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::check_framebuffer_status");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::clear)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::clear");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::clear_color)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::clear_color");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::clear_depth)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::clear_depth");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "clearDepth");

    auto arg0 = vm.argument(0);

    float depth = TRY(arg0.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear_depth(depth); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::clear_stencil)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::clear_stencil");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::color_mask)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::color_mask");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::compile_shader)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::compile_shader");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::copy_tex_image2d)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::copy_tex_image2d");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::copy_tex_sub_image2d)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::copy_tex_sub_image2d");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::create_buffer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::create_buffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_buffer(); }));

    if (retval) {

    return &const_cast<WebGLBuffer&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::create_framebuffer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::create_framebuffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_framebuffer(); }));

    if (retval) {

    return &const_cast<WebGLFramebuffer&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::create_program)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::create_program");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_program(); }));

    if (retval) {

    return &const_cast<WebGLProgram&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::create_renderbuffer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::create_renderbuffer");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_renderbuffer(); }));

    if (retval) {

    return &const_cast<WebGLRenderbuffer&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::create_shader)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::create_shader");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::create_texture)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::create_texture");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_texture(); }));

    if (retval) {

    return &const_cast<WebGLTexture&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::cull_face)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::cull_face");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::delete_buffer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::delete_buffer");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::delete_framebuffer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::delete_framebuffer");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::delete_program)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::delete_program");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::delete_renderbuffer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::delete_renderbuffer");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::delete_shader)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::delete_shader");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::delete_texture)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::delete_texture");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::depth_func)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::depth_func");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::depth_mask)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::depth_mask");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::depth_range)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::depth_range");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::detach_shader)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::detach_shader");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::disable)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::disable");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::disable_vertex_attrib_array)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::disable_vertex_attrib_array");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::draw_arrays)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::draw_arrays");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::draw_elements)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::draw_elements");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::enable)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::enable");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::enable_vertex_attrib_array)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::enable_vertex_attrib_array");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::finish)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::finish");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->finish(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::flush)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::flush");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->flush(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::framebuffer_renderbuffer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::framebuffer_renderbuffer");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::framebuffer_texture2d)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::framebuffer_texture2d");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::front_face)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::front_face");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::generate_mipmap)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::generate_mipmap");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_active_attrib)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_active_attrib");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_active_uniform)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_active_uniform");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_attached_shaders)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_attached_shaders");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_attrib_location)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_attrib_location");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_buffer_parameter)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_buffer_parameter");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_parameter)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_parameter");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_error)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_error");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_error(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_program_parameter)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_program_parameter");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_program_info_log)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_program_info_log");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_renderbuffer_parameter)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_renderbuffer_parameter");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_shader_parameter)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_shader_parameter");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_shader_precision_format)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_shader_precision_format");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_shader_info_log)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_shader_info_log");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_shader_source)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_shader_source");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_tex_parameter)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_tex_parameter");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_uniform)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_uniform");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_uniform_location)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_uniform_location");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_vertex_attrib)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_vertex_attrib");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::get_vertex_attrib_offset)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::get_vertex_attrib_offset");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::hint)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::hint");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::is_buffer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::is_buffer");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::is_enabled)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::is_enabled");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::is_framebuffer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::is_framebuffer");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::is_program)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::is_program");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::is_renderbuffer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::is_renderbuffer");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::is_shader)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::is_shader");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::is_texture)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::is_texture");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::line_width)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::line_width");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "lineWidth");

    auto arg0 = vm.argument(0);

    float width = TRY(arg0.to_double(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->line_width(width); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::link_program)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::link_program");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::pixel_storei)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::pixel_storei");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::polygon_offset)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::polygon_offset");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::renderbuffer_storage)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::renderbuffer_storage");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::sample_coverage)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::sample_coverage");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::scissor)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::scissor");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::shader_source)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::shader_source");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::stencil_func)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::stencil_func");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::stencil_func_separate)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::stencil_func_separate");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::stencil_mask)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::stencil_mask");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::stencil_mask_separate)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::stencil_mask_separate");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::stencil_op)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::stencil_op");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::stencil_op_separate)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::stencil_op_separate");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_parameterf)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_parameterf");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_parameteri)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_parameteri");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform1f)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform1f");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform2f)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform2f");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform3f)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform3f");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform4f)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform4f");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform1i)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform1i");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform2i)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform2i");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform3i)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform3i");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::uniform4i)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::uniform4i");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::use_program)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::use_program");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::validate_program)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::validate_program");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::vertex_attrib1f)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::vertex_attrib1f");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::vertex_attrib2f)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::vertex_attrib2f");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::vertex_attrib3f)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::vertex_attrib3f");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::vertex_attrib4f)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::vertex_attrib4f");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::vertex_attrib1fv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::vertex_attrib1fv");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::vertex_attrib2fv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::vertex_attrib2fv");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::vertex_attrib3fv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::vertex_attrib3fv");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::vertex_attrib4fv)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::vertex_attrib4fv");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::vertex_attrib_pointer)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::vertex_attrib_pointer");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::viewport)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::viewport");
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

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_image3d)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_image3d");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(11, vm.argument_count())) {

    case 10:
        chosen_overload_callable_id = 0;
        break;


    case 11:
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

        return tex_image3d0(vm);

    case 1:

        return tex_image3d1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::buffer_data)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::buffer_data");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(5, vm.argument_count())) {

    case 3: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("long long", false), make_ref_counted<IDL::Type>("unsigned long", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required});
        overloads.empend(1, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("BufferSource", true), make_ref_counted<IDL::Type>("unsigned long", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required});

        effective_overload_set.emplace(move(overloads), 1);
        break;
    }

    case 4:
        chosen_overload_callable_id = 2;
        break;


    case 5:
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

        return buffer_data0(vm);

    case 1:

        return buffer_data1(vm);

    case 2:

        return buffer_data2(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::buffer_sub_data)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::buffer_sub_data");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(5, vm.argument_count())) {

    case 3:
        chosen_overload_callable_id = 0;
        break;


    case 4:
        chosen_overload_callable_id = 1;
        break;


    case 5:
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

        return buffer_sub_data0(vm);

    case 1:

        return buffer_sub_data1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_image2d)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_image2d");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(10, vm.argument_count())) {

    case 6:
        chosen_overload_callable_id = 1;
        break;


    case 9: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("ArrayBufferView", true)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required});
        overloads.empend(2, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::UnionType>("", false, Vector<NonnullRefPtr<IDL::Type const>> {make_ref_counted<IDL::Type>("ImageBitmap", false), make_ref_counted<IDL::Type>("ImageData", false), make_ref_counted<IDL::Type>("HTMLImageElement", false), make_ref_counted<IDL::Type>("HTMLCanvasElement", false), make_ref_counted<IDL::Type>("HTMLVideoElement", false), make_ref_counted<IDL::Type>("OffscreenCanvas", false)})}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required});

        effective_overload_set.emplace(move(overloads), 8);
        break;
    }

    case 10:
        chosen_overload_callable_id = 3;
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

    case 2:

        return tex_image2d2(vm);

    case 3:

        return tex_image2d3(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::tex_sub_image2d)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::tex_sub_image2d");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(10, vm.argument_count())) {

    case 7:
        chosen_overload_callable_id = 1;
        break;


    case 9: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("ArrayBufferView", true)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required});
        overloads.empend(2, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::UnionType>("", false, Vector<NonnullRefPtr<IDL::Type const>> {make_ref_counted<IDL::Type>("ImageBitmap", false), make_ref_counted<IDL::Type>("ImageData", false), make_ref_counted<IDL::Type>("HTMLImageElement", false), make_ref_counted<IDL::Type>("HTMLCanvasElement", false), make_ref_counted<IDL::Type>("HTMLVideoElement", false), make_ref_counted<IDL::Type>("OffscreenCanvas", false)})}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required});

        effective_overload_set.emplace(move(overloads), 8);
        break;
    }

    case 10:
        chosen_overload_callable_id = 3;
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

    case 2:

        return tex_sub_image2d2(vm);

    case 3:

        return tex_sub_image2d3(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

JS_DEFINE_NATIVE_FUNCTION(WebGL2RenderingContextPrototype::read_pixels)
{
    WebIDL::log_trace(vm, "WebGL2RenderingContextPrototype::read_pixels");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(7, vm.argument_count())) {

    case 7: {
        Vector<IDL::EffectiveOverloadSet::Item> overloads;
        overloads.ensure_capacity(2);

        overloads.empend(0, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("ArrayBufferView", true)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required});
        overloads.empend(1, Vector<NonnullRefPtr<IDL::Type const>> { make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("long", false), make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("unsigned long", false), make_ref_counted<IDL::Type>("long long", false)}, Vector<IDL::Optionality> { IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required, IDL::Optionality::Required});

        effective_overload_set.emplace(move(overloads), 6);
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

        return read_pixels0(vm);

    case 1:

        return read_pixels1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

} // namespace Web::Bindings
