
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
#include <LibWeb/Bindings/DocumentPrototype.h>
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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Document.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Animations/AnimationTimeline.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/FontFace.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Window.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/TrustedTypes/TrustedTypePolicy.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/AnimationEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbstractRange.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/DOMStringMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/ElementInternals.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Request.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Scripting/Fetching.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSNumericValue.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRule.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSRuleList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/MediaList.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StyleSheet.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSStyleProperties.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/StylePropertyMap.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/CustomElements/CustomStateSet.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/Fetch/Headers.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/CSS/CSSUnitValue.h>

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

GC_DEFINE_ALLOCATOR(DocumentPrototype);

DocumentPrototype::DocumentPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

DocumentPrototype::~DocumentPrototype()
{
}

void DocumentPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<NodePrototype>(realm, "Node"_fly_string));


    auto unscopable_object = JS::Object::create(realm, nullptr);

    auto native_domain_getter = JS::NativeFunction::create(realm, domain_getter, 0, "domain"_utf16_fly_string, &realm, "get"sv);

    auto native_domain_setter = JS::NativeFunction::create(realm, domain_setter, 1, "domain"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("domain"_utf16_fly_string, native_domain_getter, native_domain_setter, default_attributes);

    auto native_implementation_getter = JS::NativeFunction::create(realm, implementation_getter, 0, "implementation"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_implementation_setter;

    define_direct_accessor("implementation"_utf16_fly_string, native_implementation_getter, native_implementation_setter, default_attributes);

    auto native_url_getter = JS::NativeFunction::create(realm, url_getter, 0, "URL"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_url_setter;

    define_direct_accessor("URL"_utf16_fly_string, native_url_getter, native_url_setter, default_attributes);

    auto native_document_uri_getter = JS::NativeFunction::create(realm, document_uri_getter, 0, "documentURI"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_document_uri_setter;

    define_direct_accessor("documentURI"_utf16_fly_string, native_document_uri_getter, native_document_uri_setter, default_attributes);

    auto native_character_set_getter = JS::NativeFunction::create(realm, character_set_getter, 0, "characterSet"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_character_set_setter;

    define_direct_accessor("characterSet"_utf16_fly_string, native_character_set_getter, native_character_set_setter, default_attributes);

    auto native_charset_getter = JS::NativeFunction::create(realm, charset_getter, 0, "charset"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_charset_setter;

    define_direct_accessor("charset"_utf16_fly_string, native_charset_getter, native_charset_setter, default_attributes);

    auto native_input_encoding_getter = JS::NativeFunction::create(realm, input_encoding_getter, 0, "inputEncoding"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_input_encoding_setter;

    define_direct_accessor("inputEncoding"_utf16_fly_string, native_input_encoding_getter, native_input_encoding_setter, default_attributes);

    auto native_content_type_getter = JS::NativeFunction::create(realm, content_type_getter, 0, "contentType"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_content_type_setter;

    define_direct_accessor("contentType"_utf16_fly_string, native_content_type_getter, native_content_type_setter, default_attributes);

    auto native_default_view_getter = JS::NativeFunction::create(realm, default_view_getter, 0, "defaultView"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_default_view_setter;

    define_direct_accessor("defaultView"_utf16_fly_string, native_default_view_getter, native_default_view_setter, default_attributes);

    auto native_cookie_getter = JS::NativeFunction::create(realm, cookie_getter, 0, "cookie"_utf16_fly_string, &realm, "get"sv);

    auto native_cookie_setter = JS::NativeFunction::create(realm, cookie_setter, 1, "cookie"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("cookie"_utf16_fly_string, native_cookie_getter, native_cookie_setter, default_attributes);

    auto native_fg_color_getter = JS::NativeFunction::create(realm, fg_color_getter, 0, "fgColor"_utf16_fly_string, &realm, "get"sv);

    auto native_fg_color_setter = JS::NativeFunction::create(realm, fg_color_setter, 1, "fgColor"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("fgColor"_utf16_fly_string, native_fg_color_getter, native_fg_color_setter, default_attributes);

    auto native_link_color_getter = JS::NativeFunction::create(realm, link_color_getter, 0, "linkColor"_utf16_fly_string, &realm, "get"sv);

    auto native_link_color_setter = JS::NativeFunction::create(realm, link_color_setter, 1, "linkColor"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("linkColor"_utf16_fly_string, native_link_color_getter, native_link_color_setter, default_attributes);

    auto native_vlink_color_getter = JS::NativeFunction::create(realm, vlink_color_getter, 0, "vlinkColor"_utf16_fly_string, &realm, "get"sv);

    auto native_vlink_color_setter = JS::NativeFunction::create(realm, vlink_color_setter, 1, "vlinkColor"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("vlinkColor"_utf16_fly_string, native_vlink_color_getter, native_vlink_color_setter, default_attributes);

    auto native_alink_color_getter = JS::NativeFunction::create(realm, alink_color_getter, 0, "alinkColor"_utf16_fly_string, &realm, "get"sv);

    auto native_alink_color_setter = JS::NativeFunction::create(realm, alink_color_setter, 1, "alinkColor"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("alinkColor"_utf16_fly_string, native_alink_color_getter, native_alink_color_setter, default_attributes);

    auto native_bg_color_getter = JS::NativeFunction::create(realm, bg_color_getter, 0, "bgColor"_utf16_fly_string, &realm, "get"sv);

    auto native_bg_color_setter = JS::NativeFunction::create(realm, bg_color_setter, 1, "bgColor"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("bgColor"_utf16_fly_string, native_bg_color_getter, native_bg_color_setter, default_attributes);

    auto native_referrer_getter = JS::NativeFunction::create(realm, referrer_getter, 0, "referrer"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_referrer_setter;

    define_direct_accessor("referrer"_utf16_fly_string, native_referrer_getter, native_referrer_setter, default_attributes);

    auto native_applets_getter = JS::NativeFunction::create(realm, applets_getter, 0, "applets"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_applets_setter;

    define_direct_accessor("applets"_utf16_fly_string, native_applets_getter, native_applets_setter, default_attributes);

    auto native_anchors_getter = JS::NativeFunction::create(realm, anchors_getter, 0, "anchors"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_anchors_setter;

    define_direct_accessor("anchors"_utf16_fly_string, native_anchors_getter, native_anchors_setter, default_attributes);

    auto native_images_getter = JS::NativeFunction::create(realm, images_getter, 0, "images"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_images_setter;

    define_direct_accessor("images"_utf16_fly_string, native_images_getter, native_images_setter, default_attributes);

    auto native_embeds_getter = JS::NativeFunction::create(realm, embeds_getter, 0, "embeds"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_embeds_setter;

    define_direct_accessor("embeds"_utf16_fly_string, native_embeds_getter, native_embeds_setter, default_attributes);

    auto native_plugins_getter = JS::NativeFunction::create(realm, plugins_getter, 0, "plugins"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_plugins_setter;

    define_direct_accessor("plugins"_utf16_fly_string, native_plugins_getter, native_plugins_setter, default_attributes);

    auto native_links_getter = JS::NativeFunction::create(realm, links_getter, 0, "links"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_links_setter;

    define_direct_accessor("links"_utf16_fly_string, native_links_getter, native_links_setter, default_attributes);

    auto native_forms_getter = JS::NativeFunction::create(realm, forms_getter, 0, "forms"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_forms_setter;

    define_direct_accessor("forms"_utf16_fly_string, native_forms_getter, native_forms_setter, default_attributes);

    auto native_scripts_getter = JS::NativeFunction::create(realm, scripts_getter, 0, "scripts"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_scripts_setter;

    define_direct_accessor("scripts"_utf16_fly_string, native_scripts_getter, native_scripts_setter, default_attributes);

    auto native_all_getter = JS::NativeFunction::create(realm, all_getter, 0, "all"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_all_setter;

    define_direct_accessor("all"_utf16_fly_string, native_all_getter, native_all_setter, default_attributes);

    auto native_compat_mode_getter = JS::NativeFunction::create(realm, compat_mode_getter, 0, "compatMode"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_compat_mode_setter;

    define_direct_accessor("compatMode"_utf16_fly_string, native_compat_mode_getter, native_compat_mode_setter, default_attributes);

    auto native_doctype_getter = JS::NativeFunction::create(realm, doctype_getter, 0, "doctype"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_doctype_setter;

    define_direct_accessor("doctype"_utf16_fly_string, native_doctype_getter, native_doctype_setter, default_attributes);

    auto native_document_element_getter = JS::NativeFunction::create(realm, document_element_getter, 0, "documentElement"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_document_element_setter;

    define_direct_accessor("documentElement"_utf16_fly_string, native_document_element_getter, native_document_element_setter, default_attributes);

    auto native_dir_getter = JS::NativeFunction::create(realm, dir_getter, 0, "dir"_utf16_fly_string, &realm, "get"sv);

    auto native_dir_setter = JS::NativeFunction::create(realm, dir_setter, 1, "dir"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("dir"_utf16_fly_string, native_dir_getter, native_dir_setter, default_attributes);

    auto native_body_getter = JS::NativeFunction::create(realm, body_getter, 0, "body"_utf16_fly_string, &realm, "get"sv);

    auto native_body_setter = JS::NativeFunction::create(realm, body_setter, 1, "body"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("body"_utf16_fly_string, native_body_getter, native_body_setter, default_attributes);

    auto native_head_getter = JS::NativeFunction::create(realm, head_getter, 0, "head"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_head_setter;

    define_direct_accessor("head"_utf16_fly_string, native_head_getter, native_head_setter, default_attributes);

    auto native_current_script_getter = JS::NativeFunction::create(realm, current_script_getter, 0, "currentScript"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_current_script_setter;

    define_direct_accessor("currentScript"_utf16_fly_string, native_current_script_getter, native_current_script_setter, default_attributes);

    auto native_last_modified_getter = JS::NativeFunction::create(realm, last_modified_getter, 0, "lastModified"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_last_modified_setter;

    define_direct_accessor("lastModified"_utf16_fly_string, native_last_modified_getter, native_last_modified_setter, default_attributes);

    auto native_ready_state_getter = JS::NativeFunction::create(realm, ready_state_getter, 0, "readyState"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_ready_state_setter;

    define_direct_accessor("readyState"_utf16_fly_string, native_ready_state_getter, native_ready_state_setter, default_attributes);

    auto native_title_getter = JS::NativeFunction::create(realm, title_getter, 0, "title"_utf16_fly_string, &realm, "get"sv);

    auto native_title_setter = JS::NativeFunction::create(realm, title_setter, 1, "title"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("title"_utf16_fly_string, native_title_getter, native_title_setter, default_attributes);

    auto native_hidden_getter = JS::NativeFunction::create(realm, hidden_getter, 0, "hidden"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_hidden_setter;

    define_direct_accessor("hidden"_utf16_fly_string, native_hidden_getter, native_hidden_setter, default_attributes);

    auto native_visibility_state_getter = JS::NativeFunction::create(realm, visibility_state_getter, 0, "visibilityState"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_visibility_state_setter;

    define_direct_accessor("visibilityState"_utf16_fly_string, native_visibility_state_getter, native_visibility_state_setter, default_attributes);

    auto native_design_mode_getter = JS::NativeFunction::create(realm, design_mode_getter, 0, "designMode"_utf16_fly_string, &realm, "get"sv);

    auto native_design_mode_setter = JS::NativeFunction::create(realm, design_mode_setter, 1, "designMode"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("designMode"_utf16_fly_string, native_design_mode_getter, native_design_mode_setter, default_attributes);

    auto native_timeline_getter = JS::NativeFunction::create(realm, timeline_getter, 0, "timeline"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_timeline_setter;

    define_direct_accessor("timeline"_utf16_fly_string, native_timeline_getter, native_timeline_setter, default_attributes);

    auto native_scrolling_element_getter = JS::NativeFunction::create(realm, scrolling_element_getter, 0, "scrollingElement"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_scrolling_element_setter;

    define_direct_accessor("scrollingElement"_utf16_fly_string, native_scrolling_element_getter, native_scrolling_element_setter, default_attributes);

    auto native_onreadystatechange_getter = JS::NativeFunction::create(realm, onreadystatechange_getter, 0, "onreadystatechange"_utf16_fly_string, &realm, "get"sv);

    auto native_onreadystatechange_setter = JS::NativeFunction::create(realm, onreadystatechange_setter, 1, "onreadystatechange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onreadystatechange"_utf16_fly_string, native_onreadystatechange_getter, native_onreadystatechange_setter, default_attributes);

    auto native_onvisibilitychange_getter = JS::NativeFunction::create(realm, onvisibilitychange_getter, 0, "onvisibilitychange"_utf16_fly_string, &realm, "get"sv);

    auto native_onvisibilitychange_setter = JS::NativeFunction::create(realm, onvisibilitychange_setter, 1, "onvisibilitychange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onvisibilitychange"_utf16_fly_string, native_onvisibilitychange_getter, native_onvisibilitychange_setter, default_attributes);

    auto native_root_element_getter = JS::NativeFunction::create(realm, root_element_getter, 0, "rootElement"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_root_element_setter;

    define_direct_accessor("rootElement"_utf16_fly_string, native_root_element_getter, native_root_element_setter, default_attributes);

    auto native_fullscreen_enabled_getter = JS::NativeFunction::create(realm, fullscreen_enabled_getter, 0, "fullscreenEnabled"_utf16_fly_string, &realm, "get"sv);

    auto native_fullscreen_enabled_setter = JS::NativeFunction::create(realm, fullscreen_enabled_setter, 1, "fullscreenEnabled"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("fullscreenEnabled"_utf16_fly_string, native_fullscreen_enabled_getter, native_fullscreen_enabled_setter, default_attributes);

    auto native_fullscreen_getter = JS::NativeFunction::create(realm, fullscreen_getter, 0, "fullscreen"_utf16_fly_string, &realm, "get"sv);

    auto native_fullscreen_setter = JS::NativeFunction::create(realm, fullscreen_setter, 1, "fullscreen"_utf16_fly_string, &realm, "set"sv);

    MUST(unscopable_object->create_data_property("fullscreen"_utf16_fly_string, JS::Value(true)));

    define_direct_accessor("fullscreen"_utf16_fly_string, native_fullscreen_getter, native_fullscreen_setter, default_attributes);

    auto native_onfullscreenchange_getter = JS::NativeFunction::create(realm, onfullscreenchange_getter, 0, "onfullscreenchange"_utf16_fly_string, &realm, "get"sv);

    auto native_onfullscreenchange_setter = JS::NativeFunction::create(realm, onfullscreenchange_setter, 1, "onfullscreenchange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onfullscreenchange"_utf16_fly_string, native_onfullscreenchange_getter, native_onfullscreenchange_setter, default_attributes);

    auto native_onfullscreenerror_getter = JS::NativeFunction::create(realm, onfullscreenerror_getter, 0, "onfullscreenerror"_utf16_fly_string, &realm, "get"sv);

    auto native_onfullscreenerror_setter = JS::NativeFunction::create(realm, onfullscreenerror_setter, 1, "onfullscreenerror"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onfullscreenerror"_utf16_fly_string, native_onfullscreenerror_getter, native_onfullscreenerror_setter, default_attributes);

    auto native_active_element_getter = JS::NativeFunction::create(realm, active_element_getter, 0, "activeElement"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_active_element_setter;

    define_direct_accessor("activeElement"_utf16_fly_string, native_active_element_getter, native_active_element_setter, default_attributes);

    auto native_style_sheets_getter = JS::NativeFunction::create(realm, style_sheets_getter, 0, "styleSheets"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_style_sheets_setter;

    define_direct_accessor("styleSheets"_utf16_fly_string, native_style_sheets_getter, native_style_sheets_setter, default_attributes);

    auto native_adopted_style_sheets_getter = JS::NativeFunction::create(realm, adopted_style_sheets_getter, 0, "adoptedStyleSheets"_utf16_fly_string, &realm, "get"sv);

    auto native_adopted_style_sheets_setter = JS::NativeFunction::create(realm, adopted_style_sheets_setter, 1, "adoptedStyleSheets"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("adoptedStyleSheets"_utf16_fly_string, native_adopted_style_sheets_getter, native_adopted_style_sheets_setter, default_attributes);

    auto native_fullscreen_element_getter = JS::NativeFunction::create(realm, fullscreen_element_getter, 0, "fullscreenElement"_utf16_fly_string, &realm, "get"sv);

    auto native_fullscreen_element_setter = JS::NativeFunction::create(realm, fullscreen_element_setter, 1, "fullscreenElement"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("fullscreenElement"_utf16_fly_string, native_fullscreen_element_getter, native_fullscreen_element_setter, default_attributes);

    auto native_onabort_getter = JS::NativeFunction::create(realm, onabort_getter, 0, "onabort"_utf16_fly_string, &realm, "get"sv);

    auto native_onabort_setter = JS::NativeFunction::create(realm, onabort_setter, 1, "onabort"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onabort"_utf16_fly_string, native_onabort_getter, native_onabort_setter, default_attributes);

    auto native_onauxclick_getter = JS::NativeFunction::create(realm, onauxclick_getter, 0, "onauxclick"_utf16_fly_string, &realm, "get"sv);

    auto native_onauxclick_setter = JS::NativeFunction::create(realm, onauxclick_setter, 1, "onauxclick"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onauxclick"_utf16_fly_string, native_onauxclick_getter, native_onauxclick_setter, default_attributes);

    auto native_onbeforeinput_getter = JS::NativeFunction::create(realm, onbeforeinput_getter, 0, "onbeforeinput"_utf16_fly_string, &realm, "get"sv);

    auto native_onbeforeinput_setter = JS::NativeFunction::create(realm, onbeforeinput_setter, 1, "onbeforeinput"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onbeforeinput"_utf16_fly_string, native_onbeforeinput_getter, native_onbeforeinput_setter, default_attributes);

    auto native_onbeforematch_getter = JS::NativeFunction::create(realm, onbeforematch_getter, 0, "onbeforematch"_utf16_fly_string, &realm, "get"sv);

    auto native_onbeforematch_setter = JS::NativeFunction::create(realm, onbeforematch_setter, 1, "onbeforematch"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onbeforematch"_utf16_fly_string, native_onbeforematch_getter, native_onbeforematch_setter, default_attributes);

    auto native_onbeforetoggle_getter = JS::NativeFunction::create(realm, onbeforetoggle_getter, 0, "onbeforetoggle"_utf16_fly_string, &realm, "get"sv);

    auto native_onbeforetoggle_setter = JS::NativeFunction::create(realm, onbeforetoggle_setter, 1, "onbeforetoggle"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onbeforetoggle"_utf16_fly_string, native_onbeforetoggle_getter, native_onbeforetoggle_setter, default_attributes);

    auto native_onblur_getter = JS::NativeFunction::create(realm, onblur_getter, 0, "onblur"_utf16_fly_string, &realm, "get"sv);

    auto native_onblur_setter = JS::NativeFunction::create(realm, onblur_setter, 1, "onblur"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onblur"_utf16_fly_string, native_onblur_getter, native_onblur_setter, default_attributes);

    auto native_oncancel_getter = JS::NativeFunction::create(realm, oncancel_getter, 0, "oncancel"_utf16_fly_string, &realm, "get"sv);

    auto native_oncancel_setter = JS::NativeFunction::create(realm, oncancel_setter, 1, "oncancel"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("oncancel"_utf16_fly_string, native_oncancel_getter, native_oncancel_setter, default_attributes);

    auto native_oncanplay_getter = JS::NativeFunction::create(realm, oncanplay_getter, 0, "oncanplay"_utf16_fly_string, &realm, "get"sv);

    auto native_oncanplay_setter = JS::NativeFunction::create(realm, oncanplay_setter, 1, "oncanplay"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("oncanplay"_utf16_fly_string, native_oncanplay_getter, native_oncanplay_setter, default_attributes);

    auto native_oncanplaythrough_getter = JS::NativeFunction::create(realm, oncanplaythrough_getter, 0, "oncanplaythrough"_utf16_fly_string, &realm, "get"sv);

    auto native_oncanplaythrough_setter = JS::NativeFunction::create(realm, oncanplaythrough_setter, 1, "oncanplaythrough"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("oncanplaythrough"_utf16_fly_string, native_oncanplaythrough_getter, native_oncanplaythrough_setter, default_attributes);

    auto native_onchange_getter = JS::NativeFunction::create(realm, onchange_getter, 0, "onchange"_utf16_fly_string, &realm, "get"sv);

    auto native_onchange_setter = JS::NativeFunction::create(realm, onchange_setter, 1, "onchange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onchange"_utf16_fly_string, native_onchange_getter, native_onchange_setter, default_attributes);

    auto native_onclick_getter = JS::NativeFunction::create(realm, onclick_getter, 0, "onclick"_utf16_fly_string, &realm, "get"sv);

    auto native_onclick_setter = JS::NativeFunction::create(realm, onclick_setter, 1, "onclick"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onclick"_utf16_fly_string, native_onclick_getter, native_onclick_setter, default_attributes);

    auto native_onclose_getter = JS::NativeFunction::create(realm, onclose_getter, 0, "onclose"_utf16_fly_string, &realm, "get"sv);

    auto native_onclose_setter = JS::NativeFunction::create(realm, onclose_setter, 1, "onclose"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onclose"_utf16_fly_string, native_onclose_getter, native_onclose_setter, default_attributes);

    auto native_oncommand_getter = JS::NativeFunction::create(realm, oncommand_getter, 0, "oncommand"_utf16_fly_string, &realm, "get"sv);

    auto native_oncommand_setter = JS::NativeFunction::create(realm, oncommand_setter, 1, "oncommand"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("oncommand"_utf16_fly_string, native_oncommand_getter, native_oncommand_setter, default_attributes);

    auto native_oncontextlost_getter = JS::NativeFunction::create(realm, oncontextlost_getter, 0, "oncontextlost"_utf16_fly_string, &realm, "get"sv);

    auto native_oncontextlost_setter = JS::NativeFunction::create(realm, oncontextlost_setter, 1, "oncontextlost"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("oncontextlost"_utf16_fly_string, native_oncontextlost_getter, native_oncontextlost_setter, default_attributes);

    auto native_oncontextmenu_getter = JS::NativeFunction::create(realm, oncontextmenu_getter, 0, "oncontextmenu"_utf16_fly_string, &realm, "get"sv);

    auto native_oncontextmenu_setter = JS::NativeFunction::create(realm, oncontextmenu_setter, 1, "oncontextmenu"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("oncontextmenu"_utf16_fly_string, native_oncontextmenu_getter, native_oncontextmenu_setter, default_attributes);

    auto native_oncontextrestored_getter = JS::NativeFunction::create(realm, oncontextrestored_getter, 0, "oncontextrestored"_utf16_fly_string, &realm, "get"sv);

    auto native_oncontextrestored_setter = JS::NativeFunction::create(realm, oncontextrestored_setter, 1, "oncontextrestored"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("oncontextrestored"_utf16_fly_string, native_oncontextrestored_getter, native_oncontextrestored_setter, default_attributes);

    auto native_oncopy_getter = JS::NativeFunction::create(realm, oncopy_getter, 0, "oncopy"_utf16_fly_string, &realm, "get"sv);

    auto native_oncopy_setter = JS::NativeFunction::create(realm, oncopy_setter, 1, "oncopy"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("oncopy"_utf16_fly_string, native_oncopy_getter, native_oncopy_setter, default_attributes);

    auto native_oncuechange_getter = JS::NativeFunction::create(realm, oncuechange_getter, 0, "oncuechange"_utf16_fly_string, &realm, "get"sv);

    auto native_oncuechange_setter = JS::NativeFunction::create(realm, oncuechange_setter, 1, "oncuechange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("oncuechange"_utf16_fly_string, native_oncuechange_getter, native_oncuechange_setter, default_attributes);

    auto native_oncut_getter = JS::NativeFunction::create(realm, oncut_getter, 0, "oncut"_utf16_fly_string, &realm, "get"sv);

    auto native_oncut_setter = JS::NativeFunction::create(realm, oncut_setter, 1, "oncut"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("oncut"_utf16_fly_string, native_oncut_getter, native_oncut_setter, default_attributes);

    auto native_ondblclick_getter = JS::NativeFunction::create(realm, ondblclick_getter, 0, "ondblclick"_utf16_fly_string, &realm, "get"sv);

    auto native_ondblclick_setter = JS::NativeFunction::create(realm, ondblclick_setter, 1, "ondblclick"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ondblclick"_utf16_fly_string, native_ondblclick_getter, native_ondblclick_setter, default_attributes);

    auto native_ondrag_getter = JS::NativeFunction::create(realm, ondrag_getter, 0, "ondrag"_utf16_fly_string, &realm, "get"sv);

    auto native_ondrag_setter = JS::NativeFunction::create(realm, ondrag_setter, 1, "ondrag"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ondrag"_utf16_fly_string, native_ondrag_getter, native_ondrag_setter, default_attributes);

    auto native_ondragend_getter = JS::NativeFunction::create(realm, ondragend_getter, 0, "ondragend"_utf16_fly_string, &realm, "get"sv);

    auto native_ondragend_setter = JS::NativeFunction::create(realm, ondragend_setter, 1, "ondragend"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ondragend"_utf16_fly_string, native_ondragend_getter, native_ondragend_setter, default_attributes);

    auto native_ondragenter_getter = JS::NativeFunction::create(realm, ondragenter_getter, 0, "ondragenter"_utf16_fly_string, &realm, "get"sv);

    auto native_ondragenter_setter = JS::NativeFunction::create(realm, ondragenter_setter, 1, "ondragenter"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ondragenter"_utf16_fly_string, native_ondragenter_getter, native_ondragenter_setter, default_attributes);

    auto native_ondragleave_getter = JS::NativeFunction::create(realm, ondragleave_getter, 0, "ondragleave"_utf16_fly_string, &realm, "get"sv);

    auto native_ondragleave_setter = JS::NativeFunction::create(realm, ondragleave_setter, 1, "ondragleave"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ondragleave"_utf16_fly_string, native_ondragleave_getter, native_ondragleave_setter, default_attributes);

    auto native_ondragover_getter = JS::NativeFunction::create(realm, ondragover_getter, 0, "ondragover"_utf16_fly_string, &realm, "get"sv);

    auto native_ondragover_setter = JS::NativeFunction::create(realm, ondragover_setter, 1, "ondragover"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ondragover"_utf16_fly_string, native_ondragover_getter, native_ondragover_setter, default_attributes);

    auto native_ondragstart_getter = JS::NativeFunction::create(realm, ondragstart_getter, 0, "ondragstart"_utf16_fly_string, &realm, "get"sv);

    auto native_ondragstart_setter = JS::NativeFunction::create(realm, ondragstart_setter, 1, "ondragstart"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ondragstart"_utf16_fly_string, native_ondragstart_getter, native_ondragstart_setter, default_attributes);

    auto native_ondrop_getter = JS::NativeFunction::create(realm, ondrop_getter, 0, "ondrop"_utf16_fly_string, &realm, "get"sv);

    auto native_ondrop_setter = JS::NativeFunction::create(realm, ondrop_setter, 1, "ondrop"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ondrop"_utf16_fly_string, native_ondrop_getter, native_ondrop_setter, default_attributes);

    auto native_ondurationchange_getter = JS::NativeFunction::create(realm, ondurationchange_getter, 0, "ondurationchange"_utf16_fly_string, &realm, "get"sv);

    auto native_ondurationchange_setter = JS::NativeFunction::create(realm, ondurationchange_setter, 1, "ondurationchange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ondurationchange"_utf16_fly_string, native_ondurationchange_getter, native_ondurationchange_setter, default_attributes);

    auto native_onemptied_getter = JS::NativeFunction::create(realm, onemptied_getter, 0, "onemptied"_utf16_fly_string, &realm, "get"sv);

    auto native_onemptied_setter = JS::NativeFunction::create(realm, onemptied_setter, 1, "onemptied"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onemptied"_utf16_fly_string, native_onemptied_getter, native_onemptied_setter, default_attributes);

    auto native_onended_getter = JS::NativeFunction::create(realm, onended_getter, 0, "onended"_utf16_fly_string, &realm, "get"sv);

    auto native_onended_setter = JS::NativeFunction::create(realm, onended_setter, 1, "onended"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onended"_utf16_fly_string, native_onended_getter, native_onended_setter, default_attributes);

    auto native_onerror_getter = JS::NativeFunction::create(realm, onerror_getter, 0, "onerror"_utf16_fly_string, &realm, "get"sv);

    auto native_onerror_setter = JS::NativeFunction::create(realm, onerror_setter, 1, "onerror"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onerror"_utf16_fly_string, native_onerror_getter, native_onerror_setter, default_attributes);

    auto native_onfocus_getter = JS::NativeFunction::create(realm, onfocus_getter, 0, "onfocus"_utf16_fly_string, &realm, "get"sv);

    auto native_onfocus_setter = JS::NativeFunction::create(realm, onfocus_setter, 1, "onfocus"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onfocus"_utf16_fly_string, native_onfocus_getter, native_onfocus_setter, default_attributes);

    auto native_onfocusin_getter = JS::NativeFunction::create(realm, onfocusin_getter, 0, "onfocusin"_utf16_fly_string, &realm, "get"sv);

    auto native_onfocusin_setter = JS::NativeFunction::create(realm, onfocusin_setter, 1, "onfocusin"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onfocusin"_utf16_fly_string, native_onfocusin_getter, native_onfocusin_setter, default_attributes);

    auto native_onfocusout_getter = JS::NativeFunction::create(realm, onfocusout_getter, 0, "onfocusout"_utf16_fly_string, &realm, "get"sv);

    auto native_onfocusout_setter = JS::NativeFunction::create(realm, onfocusout_setter, 1, "onfocusout"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onfocusout"_utf16_fly_string, native_onfocusout_getter, native_onfocusout_setter, default_attributes);

    auto native_onformdata_getter = JS::NativeFunction::create(realm, onformdata_getter, 0, "onformdata"_utf16_fly_string, &realm, "get"sv);

    auto native_onformdata_setter = JS::NativeFunction::create(realm, onformdata_setter, 1, "onformdata"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onformdata"_utf16_fly_string, native_onformdata_getter, native_onformdata_setter, default_attributes);

    auto native_oninput_getter = JS::NativeFunction::create(realm, oninput_getter, 0, "oninput"_utf16_fly_string, &realm, "get"sv);

    auto native_oninput_setter = JS::NativeFunction::create(realm, oninput_setter, 1, "oninput"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("oninput"_utf16_fly_string, native_oninput_getter, native_oninput_setter, default_attributes);

    auto native_oninvalid_getter = JS::NativeFunction::create(realm, oninvalid_getter, 0, "oninvalid"_utf16_fly_string, &realm, "get"sv);

    auto native_oninvalid_setter = JS::NativeFunction::create(realm, oninvalid_setter, 1, "oninvalid"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("oninvalid"_utf16_fly_string, native_oninvalid_getter, native_oninvalid_setter, default_attributes);

    auto native_onkeydown_getter = JS::NativeFunction::create(realm, onkeydown_getter, 0, "onkeydown"_utf16_fly_string, &realm, "get"sv);

    auto native_onkeydown_setter = JS::NativeFunction::create(realm, onkeydown_setter, 1, "onkeydown"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onkeydown"_utf16_fly_string, native_onkeydown_getter, native_onkeydown_setter, default_attributes);

    auto native_onkeypress_getter = JS::NativeFunction::create(realm, onkeypress_getter, 0, "onkeypress"_utf16_fly_string, &realm, "get"sv);

    auto native_onkeypress_setter = JS::NativeFunction::create(realm, onkeypress_setter, 1, "onkeypress"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onkeypress"_utf16_fly_string, native_onkeypress_getter, native_onkeypress_setter, default_attributes);

    auto native_onkeyup_getter = JS::NativeFunction::create(realm, onkeyup_getter, 0, "onkeyup"_utf16_fly_string, &realm, "get"sv);

    auto native_onkeyup_setter = JS::NativeFunction::create(realm, onkeyup_setter, 1, "onkeyup"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onkeyup"_utf16_fly_string, native_onkeyup_getter, native_onkeyup_setter, default_attributes);

    auto native_onload_getter = JS::NativeFunction::create(realm, onload_getter, 0, "onload"_utf16_fly_string, &realm, "get"sv);

    auto native_onload_setter = JS::NativeFunction::create(realm, onload_setter, 1, "onload"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onload"_utf16_fly_string, native_onload_getter, native_onload_setter, default_attributes);

    auto native_onloadeddata_getter = JS::NativeFunction::create(realm, onloadeddata_getter, 0, "onloadeddata"_utf16_fly_string, &realm, "get"sv);

    auto native_onloadeddata_setter = JS::NativeFunction::create(realm, onloadeddata_setter, 1, "onloadeddata"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onloadeddata"_utf16_fly_string, native_onloadeddata_getter, native_onloadeddata_setter, default_attributes);

    auto native_onloadedmetadata_getter = JS::NativeFunction::create(realm, onloadedmetadata_getter, 0, "onloadedmetadata"_utf16_fly_string, &realm, "get"sv);

    auto native_onloadedmetadata_setter = JS::NativeFunction::create(realm, onloadedmetadata_setter, 1, "onloadedmetadata"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onloadedmetadata"_utf16_fly_string, native_onloadedmetadata_getter, native_onloadedmetadata_setter, default_attributes);

    auto native_onloadstart_getter = JS::NativeFunction::create(realm, onloadstart_getter, 0, "onloadstart"_utf16_fly_string, &realm, "get"sv);

    auto native_onloadstart_setter = JS::NativeFunction::create(realm, onloadstart_setter, 1, "onloadstart"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onloadstart"_utf16_fly_string, native_onloadstart_getter, native_onloadstart_setter, default_attributes);

    auto native_onmousedown_getter = JS::NativeFunction::create(realm, onmousedown_getter, 0, "onmousedown"_utf16_fly_string, &realm, "get"sv);

    auto native_onmousedown_setter = JS::NativeFunction::create(realm, onmousedown_setter, 1, "onmousedown"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onmousedown"_utf16_fly_string, native_onmousedown_getter, native_onmousedown_setter, default_attributes);

    auto native_onmouseenter_getter = JS::NativeFunction::create(realm, onmouseenter_getter, 0, "onmouseenter"_utf16_fly_string, &realm, "get"sv);

    auto native_onmouseenter_setter = JS::NativeFunction::create(realm, onmouseenter_setter, 1, "onmouseenter"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onmouseenter"_utf16_fly_string, native_onmouseenter_getter, native_onmouseenter_setter, default_attributes);

    auto native_onmouseleave_getter = JS::NativeFunction::create(realm, onmouseleave_getter, 0, "onmouseleave"_utf16_fly_string, &realm, "get"sv);

    auto native_onmouseleave_setter = JS::NativeFunction::create(realm, onmouseleave_setter, 1, "onmouseleave"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onmouseleave"_utf16_fly_string, native_onmouseleave_getter, native_onmouseleave_setter, default_attributes);

    auto native_onmousemove_getter = JS::NativeFunction::create(realm, onmousemove_getter, 0, "onmousemove"_utf16_fly_string, &realm, "get"sv);

    auto native_onmousemove_setter = JS::NativeFunction::create(realm, onmousemove_setter, 1, "onmousemove"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onmousemove"_utf16_fly_string, native_onmousemove_getter, native_onmousemove_setter, default_attributes);

    auto native_onmouseout_getter = JS::NativeFunction::create(realm, onmouseout_getter, 0, "onmouseout"_utf16_fly_string, &realm, "get"sv);

    auto native_onmouseout_setter = JS::NativeFunction::create(realm, onmouseout_setter, 1, "onmouseout"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onmouseout"_utf16_fly_string, native_onmouseout_getter, native_onmouseout_setter, default_attributes);

    auto native_onmouseover_getter = JS::NativeFunction::create(realm, onmouseover_getter, 0, "onmouseover"_utf16_fly_string, &realm, "get"sv);

    auto native_onmouseover_setter = JS::NativeFunction::create(realm, onmouseover_setter, 1, "onmouseover"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onmouseover"_utf16_fly_string, native_onmouseover_getter, native_onmouseover_setter, default_attributes);

    auto native_onmouseup_getter = JS::NativeFunction::create(realm, onmouseup_getter, 0, "onmouseup"_utf16_fly_string, &realm, "get"sv);

    auto native_onmouseup_setter = JS::NativeFunction::create(realm, onmouseup_setter, 1, "onmouseup"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onmouseup"_utf16_fly_string, native_onmouseup_getter, native_onmouseup_setter, default_attributes);

    auto native_onpaste_getter = JS::NativeFunction::create(realm, onpaste_getter, 0, "onpaste"_utf16_fly_string, &realm, "get"sv);

    auto native_onpaste_setter = JS::NativeFunction::create(realm, onpaste_setter, 1, "onpaste"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onpaste"_utf16_fly_string, native_onpaste_getter, native_onpaste_setter, default_attributes);

    auto native_onpause_getter = JS::NativeFunction::create(realm, onpause_getter, 0, "onpause"_utf16_fly_string, &realm, "get"sv);

    auto native_onpause_setter = JS::NativeFunction::create(realm, onpause_setter, 1, "onpause"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onpause"_utf16_fly_string, native_onpause_getter, native_onpause_setter, default_attributes);

    auto native_onplay_getter = JS::NativeFunction::create(realm, onplay_getter, 0, "onplay"_utf16_fly_string, &realm, "get"sv);

    auto native_onplay_setter = JS::NativeFunction::create(realm, onplay_setter, 1, "onplay"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onplay"_utf16_fly_string, native_onplay_getter, native_onplay_setter, default_attributes);

    auto native_onplaying_getter = JS::NativeFunction::create(realm, onplaying_getter, 0, "onplaying"_utf16_fly_string, &realm, "get"sv);

    auto native_onplaying_setter = JS::NativeFunction::create(realm, onplaying_setter, 1, "onplaying"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onplaying"_utf16_fly_string, native_onplaying_getter, native_onplaying_setter, default_attributes);

    auto native_onprogress_getter = JS::NativeFunction::create(realm, onprogress_getter, 0, "onprogress"_utf16_fly_string, &realm, "get"sv);

    auto native_onprogress_setter = JS::NativeFunction::create(realm, onprogress_setter, 1, "onprogress"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onprogress"_utf16_fly_string, native_onprogress_getter, native_onprogress_setter, default_attributes);

    auto native_onratechange_getter = JS::NativeFunction::create(realm, onratechange_getter, 0, "onratechange"_utf16_fly_string, &realm, "get"sv);

    auto native_onratechange_setter = JS::NativeFunction::create(realm, onratechange_setter, 1, "onratechange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onratechange"_utf16_fly_string, native_onratechange_getter, native_onratechange_setter, default_attributes);

    auto native_onreset_getter = JS::NativeFunction::create(realm, onreset_getter, 0, "onreset"_utf16_fly_string, &realm, "get"sv);

    auto native_onreset_setter = JS::NativeFunction::create(realm, onreset_setter, 1, "onreset"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onreset"_utf16_fly_string, native_onreset_getter, native_onreset_setter, default_attributes);

    auto native_onresize_getter = JS::NativeFunction::create(realm, onresize_getter, 0, "onresize"_utf16_fly_string, &realm, "get"sv);

    auto native_onresize_setter = JS::NativeFunction::create(realm, onresize_setter, 1, "onresize"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onresize"_utf16_fly_string, native_onresize_getter, native_onresize_setter, default_attributes);

    auto native_onscroll_getter = JS::NativeFunction::create(realm, onscroll_getter, 0, "onscroll"_utf16_fly_string, &realm, "get"sv);

    auto native_onscroll_setter = JS::NativeFunction::create(realm, onscroll_setter, 1, "onscroll"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onscroll"_utf16_fly_string, native_onscroll_getter, native_onscroll_setter, default_attributes);

    auto native_onscrollend_getter = JS::NativeFunction::create(realm, onscrollend_getter, 0, "onscrollend"_utf16_fly_string, &realm, "get"sv);

    auto native_onscrollend_setter = JS::NativeFunction::create(realm, onscrollend_setter, 1, "onscrollend"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onscrollend"_utf16_fly_string, native_onscrollend_getter, native_onscrollend_setter, default_attributes);

    auto native_onsecuritypolicyviolation_getter = JS::NativeFunction::create(realm, onsecuritypolicyviolation_getter, 0, "onsecuritypolicyviolation"_utf16_fly_string, &realm, "get"sv);

    auto native_onsecuritypolicyviolation_setter = JS::NativeFunction::create(realm, onsecuritypolicyviolation_setter, 1, "onsecuritypolicyviolation"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onsecuritypolicyviolation"_utf16_fly_string, native_onsecuritypolicyviolation_getter, native_onsecuritypolicyviolation_setter, default_attributes);

    auto native_onseeked_getter = JS::NativeFunction::create(realm, onseeked_getter, 0, "onseeked"_utf16_fly_string, &realm, "get"sv);

    auto native_onseeked_setter = JS::NativeFunction::create(realm, onseeked_setter, 1, "onseeked"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onseeked"_utf16_fly_string, native_onseeked_getter, native_onseeked_setter, default_attributes);

    auto native_onseeking_getter = JS::NativeFunction::create(realm, onseeking_getter, 0, "onseeking"_utf16_fly_string, &realm, "get"sv);

    auto native_onseeking_setter = JS::NativeFunction::create(realm, onseeking_setter, 1, "onseeking"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onseeking"_utf16_fly_string, native_onseeking_getter, native_onseeking_setter, default_attributes);

    auto native_onselect_getter = JS::NativeFunction::create(realm, onselect_getter, 0, "onselect"_utf16_fly_string, &realm, "get"sv);

    auto native_onselect_setter = JS::NativeFunction::create(realm, onselect_setter, 1, "onselect"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onselect"_utf16_fly_string, native_onselect_getter, native_onselect_setter, default_attributes);

    auto native_onselectionchange_getter = JS::NativeFunction::create(realm, onselectionchange_getter, 0, "onselectionchange"_utf16_fly_string, &realm, "get"sv);

    auto native_onselectionchange_setter = JS::NativeFunction::create(realm, onselectionchange_setter, 1, "onselectionchange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onselectionchange"_utf16_fly_string, native_onselectionchange_getter, native_onselectionchange_setter, default_attributes);

    auto native_onslotchange_getter = JS::NativeFunction::create(realm, onslotchange_getter, 0, "onslotchange"_utf16_fly_string, &realm, "get"sv);

    auto native_onslotchange_setter = JS::NativeFunction::create(realm, onslotchange_setter, 1, "onslotchange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onslotchange"_utf16_fly_string, native_onslotchange_getter, native_onslotchange_setter, default_attributes);

    auto native_onstalled_getter = JS::NativeFunction::create(realm, onstalled_getter, 0, "onstalled"_utf16_fly_string, &realm, "get"sv);

    auto native_onstalled_setter = JS::NativeFunction::create(realm, onstalled_setter, 1, "onstalled"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onstalled"_utf16_fly_string, native_onstalled_getter, native_onstalled_setter, default_attributes);

    auto native_onsubmit_getter = JS::NativeFunction::create(realm, onsubmit_getter, 0, "onsubmit"_utf16_fly_string, &realm, "get"sv);

    auto native_onsubmit_setter = JS::NativeFunction::create(realm, onsubmit_setter, 1, "onsubmit"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onsubmit"_utf16_fly_string, native_onsubmit_getter, native_onsubmit_setter, default_attributes);

    auto native_onsuspend_getter = JS::NativeFunction::create(realm, onsuspend_getter, 0, "onsuspend"_utf16_fly_string, &realm, "get"sv);

    auto native_onsuspend_setter = JS::NativeFunction::create(realm, onsuspend_setter, 1, "onsuspend"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onsuspend"_utf16_fly_string, native_onsuspend_getter, native_onsuspend_setter, default_attributes);

    auto native_ontimeupdate_getter = JS::NativeFunction::create(realm, ontimeupdate_getter, 0, "ontimeupdate"_utf16_fly_string, &realm, "get"sv);

    auto native_ontimeupdate_setter = JS::NativeFunction::create(realm, ontimeupdate_setter, 1, "ontimeupdate"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ontimeupdate"_utf16_fly_string, native_ontimeupdate_getter, native_ontimeupdate_setter, default_attributes);

    auto native_ontoggle_getter = JS::NativeFunction::create(realm, ontoggle_getter, 0, "ontoggle"_utf16_fly_string, &realm, "get"sv);

    auto native_ontoggle_setter = JS::NativeFunction::create(realm, ontoggle_setter, 1, "ontoggle"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ontoggle"_utf16_fly_string, native_ontoggle_getter, native_ontoggle_setter, default_attributes);

    auto native_onvolumechange_getter = JS::NativeFunction::create(realm, onvolumechange_getter, 0, "onvolumechange"_utf16_fly_string, &realm, "get"sv);

    auto native_onvolumechange_setter = JS::NativeFunction::create(realm, onvolumechange_setter, 1, "onvolumechange"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onvolumechange"_utf16_fly_string, native_onvolumechange_getter, native_onvolumechange_setter, default_attributes);

    auto native_onwaiting_getter = JS::NativeFunction::create(realm, onwaiting_getter, 0, "onwaiting"_utf16_fly_string, &realm, "get"sv);

    auto native_onwaiting_setter = JS::NativeFunction::create(realm, onwaiting_setter, 1, "onwaiting"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onwaiting"_utf16_fly_string, native_onwaiting_getter, native_onwaiting_setter, default_attributes);

    auto native_onwebkitanimationend_getter = JS::NativeFunction::create(realm, onwebkitanimationend_getter, 0, "onwebkitanimationend"_utf16_fly_string, &realm, "get"sv);

    auto native_onwebkitanimationend_setter = JS::NativeFunction::create(realm, onwebkitanimationend_setter, 1, "onwebkitanimationend"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onwebkitanimationend"_utf16_fly_string, native_onwebkitanimationend_getter, native_onwebkitanimationend_setter, default_attributes);

    auto native_onwebkitanimationiteration_getter = JS::NativeFunction::create(realm, onwebkitanimationiteration_getter, 0, "onwebkitanimationiteration"_utf16_fly_string, &realm, "get"sv);

    auto native_onwebkitanimationiteration_setter = JS::NativeFunction::create(realm, onwebkitanimationiteration_setter, 1, "onwebkitanimationiteration"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onwebkitanimationiteration"_utf16_fly_string, native_onwebkitanimationiteration_getter, native_onwebkitanimationiteration_setter, default_attributes);

    auto native_onwebkitanimationstart_getter = JS::NativeFunction::create(realm, onwebkitanimationstart_getter, 0, "onwebkitanimationstart"_utf16_fly_string, &realm, "get"sv);

    auto native_onwebkitanimationstart_setter = JS::NativeFunction::create(realm, onwebkitanimationstart_setter, 1, "onwebkitanimationstart"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onwebkitanimationstart"_utf16_fly_string, native_onwebkitanimationstart_getter, native_onwebkitanimationstart_setter, default_attributes);

    auto native_onwebkittransitionend_getter = JS::NativeFunction::create(realm, onwebkittransitionend_getter, 0, "onwebkittransitionend"_utf16_fly_string, &realm, "get"sv);

    auto native_onwebkittransitionend_setter = JS::NativeFunction::create(realm, onwebkittransitionend_setter, 1, "onwebkittransitionend"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onwebkittransitionend"_utf16_fly_string, native_onwebkittransitionend_getter, native_onwebkittransitionend_setter, default_attributes);

    auto native_onwheel_getter = JS::NativeFunction::create(realm, onwheel_getter, 0, "onwheel"_utf16_fly_string, &realm, "get"sv);

    auto native_onwheel_setter = JS::NativeFunction::create(realm, onwheel_setter, 1, "onwheel"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onwheel"_utf16_fly_string, native_onwheel_getter, native_onwheel_setter, default_attributes);

    auto native_onanimationcancel_getter = JS::NativeFunction::create(realm, onanimationcancel_getter, 0, "onanimationcancel"_utf16_fly_string, &realm, "get"sv);

    auto native_onanimationcancel_setter = JS::NativeFunction::create(realm, onanimationcancel_setter, 1, "onanimationcancel"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onanimationcancel"_utf16_fly_string, native_onanimationcancel_getter, native_onanimationcancel_setter, default_attributes);

    auto native_onanimationend_getter = JS::NativeFunction::create(realm, onanimationend_getter, 0, "onanimationend"_utf16_fly_string, &realm, "get"sv);

    auto native_onanimationend_setter = JS::NativeFunction::create(realm, onanimationend_setter, 1, "onanimationend"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onanimationend"_utf16_fly_string, native_onanimationend_getter, native_onanimationend_setter, default_attributes);

    auto native_onanimationiteration_getter = JS::NativeFunction::create(realm, onanimationiteration_getter, 0, "onanimationiteration"_utf16_fly_string, &realm, "get"sv);

    auto native_onanimationiteration_setter = JS::NativeFunction::create(realm, onanimationiteration_setter, 1, "onanimationiteration"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onanimationiteration"_utf16_fly_string, native_onanimationiteration_getter, native_onanimationiteration_setter, default_attributes);

    auto native_onanimationstart_getter = JS::NativeFunction::create(realm, onanimationstart_getter, 0, "onanimationstart"_utf16_fly_string, &realm, "get"sv);

    auto native_onanimationstart_setter = JS::NativeFunction::create(realm, onanimationstart_setter, 1, "onanimationstart"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onanimationstart"_utf16_fly_string, native_onanimationstart_getter, native_onanimationstart_setter, default_attributes);

    auto native_onpointerover_getter = JS::NativeFunction::create(realm, onpointerover_getter, 0, "onpointerover"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointerover_setter = JS::NativeFunction::create(realm, onpointerover_setter, 1, "onpointerover"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onpointerover"_utf16_fly_string, native_onpointerover_getter, native_onpointerover_setter, default_attributes);

    auto native_onpointerenter_getter = JS::NativeFunction::create(realm, onpointerenter_getter, 0, "onpointerenter"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointerenter_setter = JS::NativeFunction::create(realm, onpointerenter_setter, 1, "onpointerenter"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onpointerenter"_utf16_fly_string, native_onpointerenter_getter, native_onpointerenter_setter, default_attributes);

    auto native_onpointerdown_getter = JS::NativeFunction::create(realm, onpointerdown_getter, 0, "onpointerdown"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointerdown_setter = JS::NativeFunction::create(realm, onpointerdown_setter, 1, "onpointerdown"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onpointerdown"_utf16_fly_string, native_onpointerdown_getter, native_onpointerdown_setter, default_attributes);

    auto native_onpointermove_getter = JS::NativeFunction::create(realm, onpointermove_getter, 0, "onpointermove"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointermove_setter = JS::NativeFunction::create(realm, onpointermove_setter, 1, "onpointermove"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onpointermove"_utf16_fly_string, native_onpointermove_getter, native_onpointermove_setter, default_attributes);

    if (HTML::is_secure_context(Bindings::principal_host_defined_environment_settings_object(realm))) {
    auto native_onpointerrawupdate_getter = JS::NativeFunction::create(realm, onpointerrawupdate_getter, 0, "onpointerrawupdate"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointerrawupdate_setter = JS::NativeFunction::create(realm, onpointerrawupdate_setter, 1, "onpointerrawupdate"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onpointerrawupdate"_utf16_fly_string, native_onpointerrawupdate_getter, native_onpointerrawupdate_setter, default_attributes);

    }
    auto native_onpointerup_getter = JS::NativeFunction::create(realm, onpointerup_getter, 0, "onpointerup"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointerup_setter = JS::NativeFunction::create(realm, onpointerup_setter, 1, "onpointerup"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onpointerup"_utf16_fly_string, native_onpointerup_getter, native_onpointerup_setter, default_attributes);

    auto native_onpointercancel_getter = JS::NativeFunction::create(realm, onpointercancel_getter, 0, "onpointercancel"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointercancel_setter = JS::NativeFunction::create(realm, onpointercancel_setter, 1, "onpointercancel"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onpointercancel"_utf16_fly_string, native_onpointercancel_getter, native_onpointercancel_setter, default_attributes);

    auto native_onpointerout_getter = JS::NativeFunction::create(realm, onpointerout_getter, 0, "onpointerout"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointerout_setter = JS::NativeFunction::create(realm, onpointerout_setter, 1, "onpointerout"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onpointerout"_utf16_fly_string, native_onpointerout_getter, native_onpointerout_setter, default_attributes);

    auto native_onpointerleave_getter = JS::NativeFunction::create(realm, onpointerleave_getter, 0, "onpointerleave"_utf16_fly_string, &realm, "get"sv);

    auto native_onpointerleave_setter = JS::NativeFunction::create(realm, onpointerleave_setter, 1, "onpointerleave"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onpointerleave"_utf16_fly_string, native_onpointerleave_getter, native_onpointerleave_setter, default_attributes);

    auto native_ongotpointercapture_getter = JS::NativeFunction::create(realm, ongotpointercapture_getter, 0, "ongotpointercapture"_utf16_fly_string, &realm, "get"sv);

    auto native_ongotpointercapture_setter = JS::NativeFunction::create(realm, ongotpointercapture_setter, 1, "ongotpointercapture"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("ongotpointercapture"_utf16_fly_string, native_ongotpointercapture_getter, native_ongotpointercapture_setter, default_attributes);

    auto native_onlostpointercapture_getter = JS::NativeFunction::create(realm, onlostpointercapture_getter, 0, "onlostpointercapture"_utf16_fly_string, &realm, "get"sv);

    auto native_onlostpointercapture_setter = JS::NativeFunction::create(realm, onlostpointercapture_setter, 1, "onlostpointercapture"_utf16_fly_string, &realm, "set"sv);

    define_direct_accessor("onlostpointercapture"_utf16_fly_string, native_onlostpointercapture_getter, native_onlostpointercapture_setter, default_attributes);

    auto native_fonts_getter = JS::NativeFunction::create(realm, fonts_getter, 0, "fonts"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_fonts_setter;

    define_direct_accessor("fonts"_utf16_fly_string, native_fonts_getter, native_fonts_setter, default_attributes);

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

    define_native_function(realm, "hasFocus"_utf16_fly_string, has_focus, 0, default_attributes);

    define_native_function(realm, "open"_utf16_fly_string, open, 0, default_attributes);

    define_native_function(realm, "close"_utf16_fly_string, close, 0, default_attributes);

    define_native_function(realm, "write"_utf16_fly_string, write, 0, default_attributes);

    define_native_function(realm, "writeln"_utf16_fly_string, writeln, 0, default_attributes);

    define_native_function(realm, "getElementById"_utf16_fly_string, get_element_by_id, 1, default_attributes);

    define_native_function(realm, "getElementsByName"_utf16_fly_string, get_elements_by_name, 1, default_attributes);

    define_native_function(realm, "getElementsByTagName"_utf16_fly_string, get_elements_by_tag_name, 1, default_attributes);

    define_native_function(realm, "getElementsByTagNameNS"_utf16_fly_string, get_elements_by_tag_name_ns, 2, default_attributes);

    define_native_function(realm, "getElementsByClassName"_utf16_fly_string, get_elements_by_class_name, 1, default_attributes);

    define_native_function(realm, "clear"_utf16_fly_string, clear, 0, default_attributes);

    define_native_function(realm, "captureEvents"_utf16_fly_string, capture_events, 0, default_attributes);

    define_native_function(realm, "releaseEvents"_utf16_fly_string, release_events, 0, default_attributes);

    define_native_function(realm, "createElement"_utf16_fly_string, create_element, 1, default_attributes);

    define_native_function(realm, "createElementNS"_utf16_fly_string, create_element_ns, 2, default_attributes);

    define_native_function(realm, "createDocumentFragment"_utf16_fly_string, create_document_fragment, 0, default_attributes);

    define_native_function(realm, "createTextNode"_utf16_fly_string, create_text_node, 1, default_attributes);

    define_native_function(realm, "createCDATASection"_utf16_fly_string, create_cdata_section, 1, default_attributes);

    define_native_function(realm, "createComment"_utf16_fly_string, create_comment, 1, default_attributes);

    define_native_function(realm, "createProcessingInstruction"_utf16_fly_string, create_processing_instruction, 2, default_attributes);

    define_native_function(realm, "createAttribute"_utf16_fly_string, create_attribute, 1, default_attributes);

    define_native_function(realm, "createAttributeNS"_utf16_fly_string, create_attribute_ns, 2, default_attributes);

    define_native_function(realm, "createRange"_utf16_fly_string, create_range, 0, default_attributes);

    define_native_function(realm, "createEvent"_utf16_fly_string, create_event, 1, default_attributes);

    define_native_function(realm, "importNode"_utf16_fly_string, import_node, 1, default_attributes);

    define_native_function(realm, "adoptNode"_utf16_fly_string, adopt_node, 1, default_attributes);

    define_native_function(realm, "createNodeIterator"_utf16_fly_string, create_node_iterator, 1, default_attributes);

    define_native_function(realm, "createTreeWalker"_utf16_fly_string, create_tree_walker, 1, default_attributes);

    define_native_function(realm, "getSelection"_utf16_fly_string, get_selection, 0, default_attributes);

    define_native_function(realm, "elementFromPoint"_utf16_fly_string, element_from_point, 2, default_attributes);

    define_native_function(realm, "elementsFromPoint"_utf16_fly_string, elements_from_point, 2, default_attributes);

    define_native_function(realm, "execCommand"_utf16_fly_string, exec_command, 1, default_attributes);

    define_native_function(realm, "queryCommandEnabled"_utf16_fly_string, query_command_enabled, 1, default_attributes);

    define_native_function(realm, "queryCommandIndeterm"_utf16_fly_string, query_command_indeterm, 1, default_attributes);

    define_native_function(realm, "queryCommandState"_utf16_fly_string, query_command_state, 1, default_attributes);

    define_native_function(realm, "queryCommandSupported"_utf16_fly_string, query_command_supported, 1, default_attributes);

    define_native_function(realm, "queryCommandValue"_utf16_fly_string, query_command_value, 1, default_attributes);

    define_native_function(realm, "createExpression"_utf16_fly_string, create_expression, 1, default_attributes);

    define_native_function(realm, "evaluate"_utf16_fly_string, evaluate, 2, default_attributes);

    define_native_function(realm, "createNSResolver"_utf16_fly_string, create_ns_resolver, 1, default_attributes);

    define_native_function(realm, "startViewTransition"_utf16_fly_string, start_view_transition, 0, default_attributes);

    define_native_function(realm, "exitPointerLock"_utf16_fly_string, exit_pointer_lock, 0, default_attributes);

    define_native_function(realm, "exitFullscreen"_utf16_fly_string, exit_fullscreen, 0, default_attributes);

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

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "Document"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void DocumentPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

    auto unscopable_object = JS::Object::create(realm, nullptr);

    auto native_location_getter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Document"_utf16_fly_string, "location"_utf16_fly_string, location_getter, UnforgeableKey::Type::Getter);

    auto native_location_setter = host_defined_intrinsics(realm).ensure_web_unforgeable_function("Document"_utf16_fly_string, "location"_utf16_fly_string, location_setter, UnforgeableKey::Type::Setter);

    object.define_direct_accessor("location"_utf16_fly_string, native_location_getter, native_location_setter, default_attributes);

    object.define_direct_property(vm.well_known_symbol_unscopables(), unscopable_object, JS::Attribute::Configurable);

}

[[maybe_unused]] static JS::ThrowCompletionOr<DOM::Document*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<DOM::Document>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Document");
    return static_cast<DOM::Document*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::location_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::location_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->location(); }));

    if (retval) {

    return retval;

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::location_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::location_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::domain_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::domain_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->domain(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::domain_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::domain_setter");
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
        cpp_value = TRY(WebIDL::to_usv_string(vm, value));
    }

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_domain(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::implementation_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::implementation_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->implementation(); }));

    return &const_cast<DOMImplementation&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::url_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::url_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->url_string(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::document_uri_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::document_uri_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->document_uri(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::character_set_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::character_set_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->character_set(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::charset_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::charset_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->charset(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::input_encoding_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::input_encoding_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->input_encoding(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::content_type_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::content_type_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->content_type(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::default_view_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::default_view_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->default_view(); }));

    if (retval) {

    return &const_cast<WindowProxy&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::cookie_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::cookie_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->cookie(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::cookie_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::cookie_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_cookie(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::fg_color_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::fg_color_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->fg_color(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    auto retval = retval_or_exception.release_value();

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::fg_color_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::fg_color_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_fg_color(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::link_color_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::link_color_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->link_color(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    auto retval = retval_or_exception.release_value();

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::link_color_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::link_color_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_link_color(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::vlink_color_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::vlink_color_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->vlink_color(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    auto retval = retval_or_exception.release_value();

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::vlink_color_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::vlink_color_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_vlink_color(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::alink_color_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::alink_color_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->alink_color(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    auto retval = retval_or_exception.release_value();

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::alink_color_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::alink_color_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_alink_color(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::bg_color_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::bg_color_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->bg_color(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    auto retval = retval_or_exception.release_value();

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::bg_color_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::bg_color_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_bg_color(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::referrer_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::referrer_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->referrer(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::applets_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::applets_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->applets(); }));

    return &const_cast<HTMLCollection&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::anchors_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::anchors_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->anchors(); }));

    return &const_cast<HTMLCollection&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::images_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::images_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->images(); }));

    return &const_cast<HTMLCollection&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::embeds_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::embeds_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->embeds(); }));

    return &const_cast<HTMLCollection&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::plugins_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::plugins_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->plugins(); }));

    return &const_cast<HTMLCollection&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::links_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::links_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->links(); }));

    return &const_cast<HTMLCollection&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::forms_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::forms_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->forms(); }));

    return &const_cast<HTMLCollection&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::scripts_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::scripts_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scripts(); }));

    return &const_cast<HTMLCollection&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::all_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::all_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->all(); }));

    return &const_cast<HTMLAllCollection&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::compat_mode_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::compat_mode_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->compat_mode(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::doctype_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::doctype_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->doctype(); }));

    if (retval) {

    return &const_cast<DocumentType&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::document_element_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::document_element_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->document_element(); }));

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::dir_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::dir_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->dir(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    auto retval = retval_or_exception.release_value();

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::dir_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::dir_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_dir(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::body_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::body_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->body(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    auto retval = retval_or_exception.release_value();

    if (retval) {

    return &const_cast<HTMLElement&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::body_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::body_setter");
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

    GC::Ptr<HTMLElement> cpp_value;

    if (!value.is_nullish()) {
        if (!value.is_object() || !is<HTMLElement>(value.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "HTMLElement");

        cpp_value = &static_cast<HTMLElement&>(value.as_object());
    }

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_body(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::head_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::head_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->head(); }));

    if (retval) {

    return &const_cast<HTMLHeadElement&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::current_script_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::current_script_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->current_script(); }));

    if (retval) {

    return &const_cast<HTMLScriptElement&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::last_modified_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::last_modified_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->last_modified(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ready_state_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ready_state_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ready_state(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::title_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::title_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->title(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    auto retval = retval_or_exception.release_value();

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::title_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::title_setter");
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

    Utf16String cpp_value;
    if (!false || !value.is_null()) {
        cpp_value = TRY(WebIDL::to_utf16_string(vm, value));
    }

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_title(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::hidden_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::hidden_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->hidden(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::visibility_state_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::visibility_state_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->visibility_state(); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::design_mode_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::design_mode_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->design_mode(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    auto retval = retval_or_exception.release_value();

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::design_mode_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::design_mode_setter");
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

    auto maybe_exception = throw_dom_exception_if_needed(vm, [&] { return impl->set_design_mode(cpp_value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (maybe_exception.is_error())
        return maybe_exception.release_error();

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::timeline_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::timeline_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->timeline(); }));

    return &const_cast<DocumentTimeline&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::scrolling_element_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::scrolling_element_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->scrolling_element(); }));

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onreadystatechange_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onreadystatechange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onreadystatechange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onreadystatechange_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onreadystatechange_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onreadystatechange(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onvisibilitychange_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onvisibilitychange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onvisibilitychange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onvisibilitychange_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onvisibilitychange_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_onvisibilitychange(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::root_element_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::root_element_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->root_element(); }));

    if (retval) {

    return &const_cast<SVGSVGElement&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::fullscreen_enabled_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::fullscreen_enabled_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->fullscreen_enabled(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::fullscreen_enabled_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::fullscreen_enabled_setter");
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

    (void)impl;
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::fullscreen_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::fullscreen_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->fullscreen(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::fullscreen_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::fullscreen_setter");
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

    (void)impl;
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onfullscreenchange_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onfullscreenchange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onfullscreenchange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onfullscreenchange_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onfullscreenchange_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onfullscreenerror_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onfullscreenerror_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onfullscreenerror(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onfullscreenerror_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onfullscreenerror_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::active_element_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::active_element_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->active_element(); }));

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::style_sheets_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::style_sheets_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->style_sheets_for_bindings(); }));

    return &const_cast<StyleSheetList&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::adopted_style_sheets_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::adopted_style_sheets_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->adopted_style_sheets(); }));

    return retval;

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::adopted_style_sheets_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::adopted_style_sheets_setter");
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

    TRY(throw_dom_exception_if_needed(vm, [&] { return impl->set_adopted_style_sheets(cpp_value); }));

    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::fullscreen_element_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::fullscreen_element_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->fullscreen_element_for_bindings(); }));

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::fullscreen_element_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::fullscreen_element_setter");
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

    (void)impl;
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onabort_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onabort_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onabort(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onabort_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onabort_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onauxclick_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onauxclick_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onauxclick(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onauxclick_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onauxclick_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onbeforeinput_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onbeforeinput_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onbeforeinput(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onbeforeinput_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onbeforeinput_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onbeforematch_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onbeforematch_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onbeforematch(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onbeforematch_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onbeforematch_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onbeforetoggle_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onbeforetoggle_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onbeforetoggle(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onbeforetoggle_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onbeforetoggle_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onblur_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onblur_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onblur(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onblur_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onblur_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncancel_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncancel_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncancel(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncancel_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncancel_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncanplay_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncanplay_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncanplay(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncanplay_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncanplay_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncanplaythrough_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncanplaythrough_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncanplaythrough(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncanplaythrough_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncanplaythrough_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onchange_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onchange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onchange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onchange_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onchange_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onclick_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onclick_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onclick(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onclick_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onclick_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onclose_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onclose_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onclose(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onclose_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onclose_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncommand_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncommand_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncommand(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncommand_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncommand_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncontextlost_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncontextlost_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncontextlost(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncontextlost_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncontextlost_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncontextmenu_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncontextmenu_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncontextmenu(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncontextmenu_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncontextmenu_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncontextrestored_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncontextrestored_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncontextrestored(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncontextrestored_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncontextrestored_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncopy_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncopy_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncopy(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncopy_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncopy_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncuechange_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncuechange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncuechange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncuechange_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncuechange_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncut_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncut_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oncut(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oncut_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oncut_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondblclick_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondblclick_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondblclick(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondblclick_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondblclick_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondrag_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondrag_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondrag(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondrag_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondrag_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondragend_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondragend_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondragend(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondragend_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondragend_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondragenter_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondragenter_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondragenter(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondragenter_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondragenter_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondragleave_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondragleave_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondragleave(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondragleave_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondragleave_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondragover_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondragover_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondragover(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondragover_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondragover_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondragstart_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondragstart_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondragstart(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondragstart_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondragstart_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondrop_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondrop_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondrop(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondrop_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondrop_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondurationchange_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondurationchange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ondurationchange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ondurationchange_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ondurationchange_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onemptied_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onemptied_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onemptied(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onemptied_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onemptied_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onended_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onended_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onended(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onended_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onended_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onerror_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onerror_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onerror(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onerror_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onerror_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onfocus_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onfocus_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onfocus(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onfocus_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onfocus_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onfocusin_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onfocusin_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onfocusin(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onfocusin_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onfocusin_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onfocusout_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onfocusout_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onfocusout(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onfocusout_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onfocusout_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onformdata_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onformdata_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onformdata(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onformdata_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onformdata_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oninput_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oninput_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oninput(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oninput_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oninput_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oninvalid_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oninvalid_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->oninvalid(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::oninvalid_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::oninvalid_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onkeydown_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onkeydown_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onkeydown(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onkeydown_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onkeydown_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onkeypress_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onkeypress_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onkeypress(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onkeypress_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onkeypress_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onkeyup_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onkeyup_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onkeyup(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onkeyup_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onkeyup_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onload_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onload_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onload(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onload_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onload_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onloadeddata_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onloadeddata_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onloadeddata(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onloadeddata_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onloadeddata_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onloadedmetadata_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onloadedmetadata_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onloadedmetadata(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onloadedmetadata_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onloadedmetadata_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onloadstart_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onloadstart_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onloadstart(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onloadstart_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onloadstart_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onmousedown_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onmousedown_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmousedown(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onmousedown_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onmousedown_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onmouseenter_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onmouseenter_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmouseenter(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onmouseenter_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onmouseenter_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onmouseleave_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onmouseleave_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmouseleave(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onmouseleave_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onmouseleave_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onmousemove_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onmousemove_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmousemove(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onmousemove_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onmousemove_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onmouseout_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onmouseout_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmouseout(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onmouseout_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onmouseout_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onmouseover_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onmouseover_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmouseover(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onmouseover_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onmouseover_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onmouseup_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onmouseup_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onmouseup(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onmouseup_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onmouseup_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpaste_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpaste_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpaste(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpaste_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpaste_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpause_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpause_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpause(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpause_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpause_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onplay_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onplay_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onplay(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onplay_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onplay_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onplaying_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onplaying_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onplaying(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onplaying_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onplaying_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onprogress_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onprogress_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onprogress(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onprogress_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onprogress_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onratechange_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onratechange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onratechange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onratechange_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onratechange_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onreset_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onreset_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onreset(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onreset_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onreset_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onresize_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onresize_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onresize(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onresize_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onresize_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onscroll_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onscroll_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onscroll(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onscroll_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onscroll_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onscrollend_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onscrollend_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onscrollend(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onscrollend_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onscrollend_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onsecuritypolicyviolation_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onsecuritypolicyviolation_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onsecuritypolicyviolation(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onsecuritypolicyviolation_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onsecuritypolicyviolation_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onseeked_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onseeked_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onseeked(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onseeked_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onseeked_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onseeking_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onseeking_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onseeking(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onseeking_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onseeking_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onselect_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onselect_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onselect(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onselect_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onselect_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onselectionchange_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onselectionchange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onselectionchange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onselectionchange_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onselectionchange_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onslotchange_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onslotchange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onslotchange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onslotchange_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onslotchange_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onstalled_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onstalled_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onstalled(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onstalled_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onstalled_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onsubmit_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onsubmit_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onsubmit(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onsubmit_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onsubmit_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onsuspend_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onsuspend_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onsuspend(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onsuspend_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onsuspend_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ontimeupdate_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ontimeupdate_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ontimeupdate(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ontimeupdate_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ontimeupdate_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ontoggle_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ontoggle_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ontoggle(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ontoggle_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ontoggle_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onvolumechange_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onvolumechange_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onvolumechange(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onvolumechange_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onvolumechange_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onwaiting_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onwaiting_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onwaiting(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onwaiting_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onwaiting_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onwebkitanimationend_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onwebkitanimationend_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onwebkitanimationend(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onwebkitanimationend_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onwebkitanimationend_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onwebkitanimationiteration_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onwebkitanimationiteration_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onwebkitanimationiteration(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onwebkitanimationiteration_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onwebkitanimationiteration_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onwebkitanimationstart_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onwebkitanimationstart_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onwebkitanimationstart(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onwebkitanimationstart_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onwebkitanimationstart_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onwebkittransitionend_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onwebkittransitionend_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onwebkittransitionend(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onwebkittransitionend_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onwebkittransitionend_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onwheel_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onwheel_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onwheel(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onwheel_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onwheel_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onanimationcancel_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onanimationcancel_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onanimationcancel(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onanimationcancel_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onanimationcancel_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onanimationend_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onanimationend_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onanimationend(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onanimationend_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onanimationend_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onanimationiteration_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onanimationiteration_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onanimationiteration(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onanimationiteration_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onanimationiteration_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onanimationstart_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onanimationstart_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onanimationstart(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onanimationstart_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onanimationstart_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointerover_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointerover_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointerover(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointerover_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointerover_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointerenter_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointerenter_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointerenter(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointerenter_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointerenter_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointerdown_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointerdown_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointerdown(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointerdown_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointerdown_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointermove_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointermove_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointermove(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointermove_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointermove_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointerrawupdate_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointerrawupdate_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointerrawupdate(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointerrawupdate_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointerrawupdate_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointerup_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointerup_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointerup(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointerup_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointerup_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointercancel_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointercancel_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointercancel(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointercancel_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointercancel_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointerout_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointerout_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointerout(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointerout_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointerout_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointerleave_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointerleave_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onpointerleave(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onpointerleave_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onpointerleave_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ongotpointercapture_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ongotpointercapture_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ongotpointercapture(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::ongotpointercapture_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::ongotpointercapture_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onlostpointercapture_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onlostpointercapture_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->onlostpointercapture(); }));

  if (!retval) {
      return JS::js_null();
  } else {
      return retval->callback;
  }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::onlostpointercapture_setter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::onlostpointercapture_setter");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::fonts_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::fonts_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->fonts(); }));

    return &const_cast<FontFaceSet&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::children_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::children_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->children(); }));

    return &const_cast<HTMLCollection&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::first_element_child_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::first_element_child_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->first_element_child(); }));

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::last_element_child_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::last_element_child_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->last_element_child(); }));

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::child_element_count_getter)
{
    WebIDL::log_trace(vm, "DocumentPrototype::child_element_count_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->child_element_count(); }));

    return JS::Value(static_cast<WebIDL::UnsignedLong>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::has_focus)
{
    WebIDL::log_trace(vm, "DocumentPrototype::has_focus");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->has_focus_for_bindings(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::open0)
{
    WebIDL::log_trace(vm, "DocumentPrototype::open0");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    Optional<String> unused1;

    if (!arg0.is_undefined()) {
        if (!false || !arg0.is_null())
            unused1 = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    Optional<String> unused2;

    if (!arg1.is_undefined()) {
        if (!false || !arg1.is_null())
            unused2 = TRY(WebIDL::to_string(vm, arg1));
    }

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->open(unused1, unused2); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return &const_cast<Document&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::open1)
{
    WebIDL::log_trace(vm, "DocumentPrototype::open1");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    String url;
    if (!false || !arg0.is_null()) {
        url = TRY(WebIDL::to_usv_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    String name;
    if (!false || !arg1.is_null()) {
        name = TRY(WebIDL::to_string(vm, arg1));
    }

    auto arg2 = vm.argument(2);

    String features;
    if (!false || !arg2.is_null()) {
        features = TRY(WebIDL::to_string(vm, arg2));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->open(url, name, features); }));

    if (retval) {

    return &const_cast<WindowProxy&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::close)
{
    WebIDL::log_trace(vm, "DocumentPrototype::close");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->close(); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::write)
{
    WebIDL::log_trace(vm, "DocumentPrototype::write");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

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

        Vector<Variant<GC::Root<TrustedHTML>, Utf16String>> text;

        if (vm.argument_count() > 0) {
            text.ensure_capacity(vm.argument_count() - 0);

            for (size_t i = 0; i < vm.argument_count(); ++i) {
                auto result = TRY(arg0_to_variant(vm.argument(i)));
                text.unchecked_append(move(result));
            }
        }
    
    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->write(move(text)); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::writeln)
{
    WebIDL::log_trace(vm, "DocumentPrototype::writeln");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

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

        Vector<Variant<GC::Root<TrustedHTML>, Utf16String>> text;

        if (vm.argument_count() > 0) {
            text.ensure_capacity(vm.argument_count() - 0);

            for (size_t i = 0; i < vm.argument_count(); ++i) {
                auto result = TRY(arg0_to_variant(vm.argument(i)));
                text.unchecked_append(move(result));
            }
        }
    
    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->writeln(move(text)); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::get_element_by_id)
{
    WebIDL::log_trace(vm, "DocumentPrototype::get_element_by_id");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getElementById");

    auto arg0 = vm.argument(0);

    String id;
    if (!false || !arg0.is_null()) {
        id = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_element_by_id(id); }));

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::get_elements_by_name)
{
    WebIDL::log_trace(vm, "DocumentPrototype::get_elements_by_name");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getElementsByName");

    auto arg0 = vm.argument(0);

    FlyString name;
    if (!false || !arg0.is_null()) {
        name = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_elements_by_name(name); }));

    return &const_cast<NodeList&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::get_elements_by_tag_name)
{
    WebIDL::log_trace(vm, "DocumentPrototype::get_elements_by_tag_name");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::get_elements_by_tag_name_ns)
{
    WebIDL::log_trace(vm, "DocumentPrototype::get_elements_by_tag_name_ns");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::get_elements_by_class_name)
{
    WebIDL::log_trace(vm, "DocumentPrototype::get_elements_by_class_name");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::clear)
{
    WebIDL::log_trace(vm, "DocumentPrototype::clear");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->clear(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::capture_events)
{
    WebIDL::log_trace(vm, "DocumentPrototype::capture_events");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->capture_events(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::release_events)
{
    WebIDL::log_trace(vm, "DocumentPrototype::release_events");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->release_events(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::create_element)
{
    WebIDL::log_trace(vm, "DocumentPrototype::create_element");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "createElement");

    auto arg0 = vm.argument(0);

    String tag_name;
    if (!false || !arg0.is_null()) {
        tag_name = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    auto arg1_to_dictionary = [&vm, &realm](JS::Value arg1) -> JS::ThrowCompletionOr<ElementCreationOptions> {
        // This might be unused.
        (void)realm;

    if (!arg1.is_nullish() && !arg1.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ElementCreationOptions");

    ElementCreationOptions dictionary_union_type {};

    auto is_property_value_0 = JS::js_undefined();
    if (arg1.is_object())
        is_property_value_0 = TRY(arg1.as_object().get("is"_utf16_fly_string));

    if (!is_property_value_0.is_undefined()) {

    Optional<String> is_value_0;

    if (!is_property_value_0.is_undefined()) {
        if (!false || !is_property_value_0.is_null())
            is_value_0 = TRY(WebIDL::to_string(vm, is_property_value_0));
    }

    dictionary_union_type.is = is_value_0;

    }

        return dictionary_union_type;
    };

    auto arg1_to_variant = [&vm, &realm, &arg1_to_dictionary](JS::Value arg1) -> JS::ThrowCompletionOr<Variant<String, ElementCreationOptions>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg1.is_nullish())
            return Variant<String, ElementCreationOptions> { TRY(arg1_to_dictionary(arg1)) };

        if (arg1.is_object()) {
            [[maybe_unused]] auto& arg1_object = arg1.as_object();

        return Variant<String, ElementCreationOptions> { TRY(arg1_to_dictionary(arg1)) };

        }

    String arg1_string;
    if (!false || !arg1.is_null()) {
        arg1_string = TRY(WebIDL::to_string(vm, arg1));
    }

        return { arg1_string };

    };

    Variant<String, ElementCreationOptions> options = arg1.is_undefined() ? TRY(arg1_to_dictionary(arg1)) : TRY(arg1_to_variant(arg1));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->create_element(tag_name, options); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return &const_cast<Element&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::create_element_ns)
{
    WebIDL::log_trace(vm, "DocumentPrototype::create_element_ns");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "createElementNS", "2");

    auto arg0 = vm.argument(0);

    Optional<FlyString> namespace_;
    if (!arg0.is_nullish())
        namespace_ = TRY(WebIDL::to_string(vm, arg0));

    auto arg1 = vm.argument(1);

    String qualified_name;
    if (!false || !arg1.is_null()) {
        qualified_name = TRY(WebIDL::to_string(vm, arg1));
    }

    auto arg2 = vm.argument(2);

    auto arg2_to_dictionary = [&vm, &realm](JS::Value arg2) -> JS::ThrowCompletionOr<ElementCreationOptions> {
        // This might be unused.
        (void)realm;

    if (!arg2.is_nullish() && !arg2.is_object())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "ElementCreationOptions");

    ElementCreationOptions dictionary_union_type {};

    auto is_property_value_1 = JS::js_undefined();
    if (arg2.is_object())
        is_property_value_1 = TRY(arg2.as_object().get("is"_utf16_fly_string));

    if (!is_property_value_1.is_undefined()) {

    Optional<String> is_value_1;

    if (!is_property_value_1.is_undefined()) {
        if (!false || !is_property_value_1.is_null())
            is_value_1 = TRY(WebIDL::to_string(vm, is_property_value_1));
    }

    dictionary_union_type.is = is_value_1;

    }

        return dictionary_union_type;
    };

    auto arg2_to_variant = [&vm, &realm, &arg2_to_dictionary](JS::Value arg2) -> JS::ThrowCompletionOr<Variant<String, ElementCreationOptions>> {
        // These might be unused.
        (void)vm;
        (void)realm;

        if (arg2.is_nullish())
            return Variant<String, ElementCreationOptions> { TRY(arg2_to_dictionary(arg2)) };

        if (arg2.is_object()) {
            [[maybe_unused]] auto& arg2_object = arg2.as_object();

        return Variant<String, ElementCreationOptions> { TRY(arg2_to_dictionary(arg2)) };

        }

    String arg2_string;
    if (!false || !arg2.is_null()) {
        arg2_string = TRY(WebIDL::to_string(vm, arg2));
    }

        return { arg2_string };

    };

    Variant<String, ElementCreationOptions> options = arg2.is_undefined() ? TRY(arg2_to_dictionary(arg2)) : TRY(arg2_to_variant(arg2));

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->create_element_ns(namespace_, qualified_name, options); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return &const_cast<Element&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::create_document_fragment)
{
    WebIDL::log_trace(vm, "DocumentPrototype::create_document_fragment");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_document_fragment(); }));

    return &const_cast<DocumentFragment&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::create_text_node)
{
    WebIDL::log_trace(vm, "DocumentPrototype::create_text_node");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "createTextNode");

    auto arg0 = vm.argument(0);

    Utf16String data;
    if (!false || !arg0.is_null()) {
        data = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_text_node(data); }));

    return &const_cast<Text&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::create_cdata_section)
{
    WebIDL::log_trace(vm, "DocumentPrototype::create_cdata_section");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "createCDATASection");

    auto arg0 = vm.argument(0);

    Utf16String data;
    if (!false || !arg0.is_null()) {
        data = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_cdata_section(data); }));

    return &const_cast<CDATASection&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::create_comment)
{
    WebIDL::log_trace(vm, "DocumentPrototype::create_comment");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "createComment");

    auto arg0 = vm.argument(0);

    Utf16String data;
    if (!false || !arg0.is_null()) {
        data = TRY(WebIDL::to_utf16_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_comment(data); }));

    return &const_cast<Comment&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::create_processing_instruction)
{
    WebIDL::log_trace(vm, "DocumentPrototype::create_processing_instruction");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "createProcessingInstruction", "2");

    auto arg0 = vm.argument(0);

    String target;
    if (!false || !arg0.is_null()) {
        target = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    Utf16String data;
    if (!false || !arg1.is_null()) {
        data = TRY(WebIDL::to_utf16_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_processing_instruction(target, data); }));

    return &const_cast<ProcessingInstruction&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::create_attribute)
{
    WebIDL::log_trace(vm, "DocumentPrototype::create_attribute");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "createAttribute");

    auto arg0 = vm.argument(0);

    String local_name;
    if (!false || !arg0.is_null()) {
        local_name = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_attribute(local_name); }));

    return &const_cast<Attr&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::create_attribute_ns)
{
    WebIDL::log_trace(vm, "DocumentPrototype::create_attribute_ns");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "createAttributeNS", "2");

    auto arg0 = vm.argument(0);

    Optional<FlyString> namespace_;
    if (!arg0.is_nullish())
        namespace_ = TRY(WebIDL::to_string(vm, arg0));

    auto arg1 = vm.argument(1);

    String qualified_name;
    if (!false || !arg1.is_null()) {
        qualified_name = TRY(WebIDL::to_string(vm, arg1));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_attribute_ns(namespace_, qualified_name); }));

    return &const_cast<Attr&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::create_range)
{
    WebIDL::log_trace(vm, "DocumentPrototype::create_range");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_range(); }));

    return &const_cast<Range&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::create_event)
{
    WebIDL::log_trace(vm, "DocumentPrototype::create_event");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "createEvent");

    auto arg0 = vm.argument(0);

    String interface;
    if (!false || !arg0.is_null()) {
        interface = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_event(interface); }));

    return &const_cast<Event&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::import_node)
{
    WebIDL::log_trace(vm, "DocumentPrototype::import_node");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "importNode");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Node>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

    auto& node = static_cast<Node&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    bool deep;

    if (!arg1.is_undefined())

    deep = arg1.to_boolean();

    else
        deep = static_cast<bool>(false);

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->import_node(node, deep); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return &const_cast<Node&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::adopt_node)
{
    WebIDL::log_trace(vm, "DocumentPrototype::adopt_node");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "adoptNode");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Node>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

    auto& node = static_cast<Node&>(arg0.as_object());

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->adopt_node_binding(node); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return &const_cast<Node&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::create_node_iterator)
{
    WebIDL::log_trace(vm, "DocumentPrototype::create_node_iterator");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "createNodeIterator");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Node>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

    auto& root = static_cast<Node&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong what_to_show;

    if (!arg1.is_undefined())

    what_to_show = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        what_to_show = static_cast<WebIDL::UnsignedLong>(0xFFFFFFFF);

    auto arg2 = vm.argument(2);

    NodeFilter* filter = nullptr;
    if (!arg2.is_nullish()) {
        if (!arg2.is_object())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg2);

        auto callback_type = vm.heap().allocate<WebIDL::CallbackType>(arg2.as_object(), HTML::incumbent_realm());
        filter = TRY(throw_dom_exception_if_needed(vm, [&] { return NodeFilter::create(realm, *callback_type); }));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_node_iterator(root, what_to_show, filter); }));

    return &const_cast<NodeIterator&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::create_tree_walker)
{
    WebIDL::log_trace(vm, "DocumentPrototype::create_tree_walker");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "createTreeWalker");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Node>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

    auto& root = static_cast<Node&>(arg0.as_object());

    auto arg1 = vm.argument(1);

    WebIDL::UnsignedLong what_to_show;

    if (!arg1.is_undefined())

    what_to_show = TRY(WebIDL::convert_to_int<WebIDL::UnsignedLong>(vm, arg1, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        what_to_show = static_cast<WebIDL::UnsignedLong>(0xFFFFFFFF);

    auto arg2 = vm.argument(2);

    NodeFilter* filter = nullptr;
    if (!arg2.is_nullish()) {
        if (!arg2.is_object())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg2);

        auto callback_type = vm.heap().allocate<WebIDL::CallbackType>(arg2.as_object(), HTML::incumbent_realm());
        filter = TRY(throw_dom_exception_if_needed(vm, [&] { return NodeFilter::create(realm, *callback_type); }));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_tree_walker(root, what_to_show, filter); }));

    return &const_cast<TreeWalker&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::get_selection)
{
    WebIDL::log_trace(vm, "DocumentPrototype::get_selection");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_selection(); }));

    if (retval) {

    return &const_cast<Selection::Selection&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::element_from_point)
{
    WebIDL::log_trace(vm, "DocumentPrototype::element_from_point");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "elementFromPoint", "2");

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
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->element_from_point(x, y); }));

    if (retval) {

    return &const_cast<Element&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::elements_from_point)
{
    WebIDL::log_trace(vm, "DocumentPrototype::elements_from_point");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "elementsFromPoint", "2");

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
    
    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->elements_from_point(x, y); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::exec_command)
{
    WebIDL::log_trace(vm, "DocumentPrototype::exec_command");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "execCommand");

    auto arg0 = vm.argument(0);

    String command_id;
    if (!false || !arg0.is_null()) {
        command_id = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    bool show_ui;

    if (!arg1.is_undefined())

    show_ui = arg1.to_boolean();

    else
        show_ui = static_cast<bool>(false);

    auto arg2 = vm.argument(2);

    Utf16String value;

    if (!arg2.is_undefined()) {
        if (!false || !arg2.is_null())
            value = TRY(WebIDL::to_utf16_string(vm, arg2));
    } else {
        value = ""_utf16;
    }

    auto& reactions_stack = HTML::relevant_similar_origin_window_agent(*impl).custom_element_reactions_stack;
    reactions_stack.element_queue_stack.append({});

    auto retval_or_exception = throw_dom_exception_if_needed(vm, [&] { return impl->exec_command(command_id, show_ui, value); });

    auto queue = reactions_stack.element_queue_stack.take_last();
    Bindings::invoke_custom_element_reactions(queue);

    if (retval_or_exception.is_error())
        return retval_or_exception.release_error();

    [[maybe_unused]] auto retval = retval_or_exception.release_value();

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::query_command_enabled)
{
    WebIDL::log_trace(vm, "DocumentPrototype::query_command_enabled");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "queryCommandEnabled");

    auto arg0 = vm.argument(0);

    String command_id;
    if (!false || !arg0.is_null()) {
        command_id = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->query_command_enabled(command_id); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::query_command_indeterm)
{
    WebIDL::log_trace(vm, "DocumentPrototype::query_command_indeterm");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "queryCommandIndeterm");

    auto arg0 = vm.argument(0);

    String command_id;
    if (!false || !arg0.is_null()) {
        command_id = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->query_command_indeterm(command_id); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::query_command_state)
{
    WebIDL::log_trace(vm, "DocumentPrototype::query_command_state");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "queryCommandState");

    auto arg0 = vm.argument(0);

    String command_id;
    if (!false || !arg0.is_null()) {
        command_id = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->query_command_state(command_id); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::query_command_supported)
{
    WebIDL::log_trace(vm, "DocumentPrototype::query_command_supported");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "queryCommandSupported");

    auto arg0 = vm.argument(0);

    String command_id;
    if (!false || !arg0.is_null()) {
        command_id = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->query_command_supported(command_id); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::query_command_value)
{
    WebIDL::log_trace(vm, "DocumentPrototype::query_command_value");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "queryCommandValue");

    auto arg0 = vm.argument(0);

    String command_id;
    if (!false || !arg0.is_null()) {
        command_id = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->query_command_value(command_id); }));

    return JS::PrimitiveString::create(vm, retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::create_expression)
{
    WebIDL::log_trace(vm, "DocumentPrototype::create_expression");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "createExpression");

    auto arg0 = vm.argument(0);

    String expression;
    if (!false || !arg0.is_null()) {
        expression = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    XPathNSResolver* resolver = nullptr;
    if (!arg1.is_nullish()) {
        if (!arg1.is_object())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg1);

        auto callback_type = vm.heap().allocate<WebIDL::CallbackType>(arg1.as_object(), HTML::incumbent_realm());
        resolver = TRY(throw_dom_exception_if_needed(vm, [&] { return XPathNSResolver::create(realm, *callback_type); }));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_expression(expression, resolver); }));

    return &const_cast<XPathExpression&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::evaluate)
{
    WebIDL::log_trace(vm, "DocumentPrototype::evaluate");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 2)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountMany, "evaluate", "2");

    auto arg0 = vm.argument(0);

    String expression;
    if (!false || !arg0.is_null()) {
        expression = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    if (!arg1.is_object() || !is<Node>(arg1.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

    auto& context_node = static_cast<Node&>(arg1.as_object());

    auto arg2 = vm.argument(2);

    XPathNSResolver* resolver = nullptr;
    if (!arg2.is_nullish()) {
        if (!arg2.is_object())
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObject, arg2);

        auto callback_type = vm.heap().allocate<WebIDL::CallbackType>(arg2.as_object(), HTML::incumbent_realm());
        resolver = TRY(throw_dom_exception_if_needed(vm, [&] { return XPathNSResolver::create(realm, *callback_type); }));
    }

    auto arg3 = vm.argument(3);

    WebIDL::UnsignedShort type;

    if (!arg3.is_undefined())

    type = TRY(WebIDL::convert_to_int<WebIDL::UnsignedShort>(vm, arg3, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        type = static_cast<WebIDL::UnsignedShort>(0);

    auto arg4 = vm.argument(4);

    GC::Ptr<XPathResult> result;

    if (!arg4.is_nullish()) {
        if (!arg4.is_object() || !is<XPathResult>(arg4.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "XPathResult");

        result = &static_cast<XPathResult&>(arg4.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->evaluate(expression, context_node, resolver, type, result); }));

    return &const_cast<XPathResult&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::create_ns_resolver)
{
    WebIDL::log_trace(vm, "DocumentPrototype::create_ns_resolver");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "createNSResolver");

    auto arg0 = vm.argument(0);

    if (!arg0.is_object() || !is<Node>(arg0.as_object()))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "Node");

    auto& node_resolver = static_cast<Node&>(arg0.as_object());

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->create_ns_resolver(node_resolver); }));

    return &const_cast<Node&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::start_view_transition)
{
    WebIDL::log_trace(vm, "DocumentPrototype::start_view_transition");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    auto arg0 = vm.argument(0);

    if (!arg0.is_function()
&& !arg0.is_undefined())
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAFunction, arg0);

    GC::Ptr<WebIDL::CallbackType> update_callback;
    if (arg0.is_object())
        update_callback = vm.heap().allocate<WebIDL::CallbackType>(arg0.as_object(), HTML::incumbent_realm(), WebIDL::OperationReturnsPromise::No);

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->start_view_transition(update_callback); }));

    return &const_cast<ViewTransition::ViewTransition&>(*retval);

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::exit_pointer_lock)
{
    WebIDL::log_trace(vm, "DocumentPrototype::exit_pointer_lock");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->exit_pointer_lock(); }));

    return JS::js_undefined();

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::exit_fullscreen)
{
    WebIDL::log_trace(vm, "DocumentPrototype::exit_fullscreen");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto steps = [&realm, &vm]() -> JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> {
        (void)realm;

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->exit_fullscreen(); }));

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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::get_animations)
{
    WebIDL::log_trace(vm, "DocumentPrototype::get_animations");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_animations(); }));

    auto new_array0_0 = MUST(JS::Array::create(realm, 0));

    for (size_t i0 = 0; i0 < retval.size(); ++i0) {
        auto& element0 = retval.at(i0);

        auto* wrapped_element0 = &(*element0);

        auto property_index0 = JS::PropertyKey { i0 };
        MUST(new_array0_0->create_data_property(property_index0, wrapped_element0));
    }

    return new_array0_0;

}

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::prepend)
{
    WebIDL::log_trace(vm, "DocumentPrototype::prepend");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::append)
{
    WebIDL::log_trace(vm, "DocumentPrototype::append");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::replace_children)
{
    WebIDL::log_trace(vm, "DocumentPrototype::replace_children");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::move_before)
{
    WebIDL::log_trace(vm, "DocumentPrototype::move_before");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::query_selector)
{
    WebIDL::log_trace(vm, "DocumentPrototype::query_selector");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::query_selector_all)
{
    WebIDL::log_trace(vm, "DocumentPrototype::query_selector_all");
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

JS_DEFINE_NATIVE_FUNCTION(DocumentPrototype::open)
{
    WebIDL::log_trace(vm, "DocumentPrototype::open");

    Optional<int> chosen_overload_callable_id;
    Optional<IDL::EffectiveOverloadSet> effective_overload_set;
    switch (min(3, vm.argument_count())) {

    case 0:
        chosen_overload_callable_id = 0;
        break;


    case 1:
        chosen_overload_callable_id = 0;
        break;


    case 2:
        chosen_overload_callable_id = 0;
        break;


    case 3:
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

        return open0(vm);

    case 1:

        return open1(vm);

    default:
        VERIFY_NOT_REACHED();
    }
}

} // namespace Web::Bindings
