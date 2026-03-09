
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
#include <LibWeb/Bindings/MouseEventPrototype.h>
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

#if __has_include(<LibWeb/Bindings/UIEventPrototype.h>)
#    include <LibWeb/Bindings/UIEventPrototype.h>
#endif


#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/UIEvents/MouseEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/UIEvents/EventModifier.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/UIEvents/UIEvent.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/Event.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/HTML/Window.h>

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/EventTarget.h>

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

#include </home/misi/work/my/yetty/tmp/ladybird/Libraries/LibWeb/DOM/AbortSignal.h>

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

GC_DEFINE_ALLOCATOR(MouseEventPrototype);

MouseEventPrototype::MouseEventPrototype([[maybe_unused]] JS::Realm& realm)
    : Object(realm, nullptr)

{
}

MouseEventPrototype::~MouseEventPrototype()
{
}

void MouseEventPrototype::initialize(JS::Realm& realm)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable | JS::Attribute::Configurable | JS::Attribute::Writable;


    set_prototype(&ensure_web_prototype<UIEventPrototype>(realm, "UIEvent"_fly_string));


    auto native_screen_x_getter = JS::NativeFunction::create(realm, screen_x_getter, 0, "screenX"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_screen_x_setter;

    define_direct_accessor("screenX"_utf16_fly_string, native_screen_x_getter, native_screen_x_setter, default_attributes);

    auto native_screen_y_getter = JS::NativeFunction::create(realm, screen_y_getter, 0, "screenY"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_screen_y_setter;

    define_direct_accessor("screenY"_utf16_fly_string, native_screen_y_getter, native_screen_y_setter, default_attributes);

    auto native_page_x_getter = JS::NativeFunction::create(realm, page_x_getter, 0, "pageX"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_page_x_setter;

    define_direct_accessor("pageX"_utf16_fly_string, native_page_x_getter, native_page_x_setter, default_attributes);

    auto native_page_y_getter = JS::NativeFunction::create(realm, page_y_getter, 0, "pageY"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_page_y_setter;

    define_direct_accessor("pageY"_utf16_fly_string, native_page_y_getter, native_page_y_setter, default_attributes);

    auto native_client_x_getter = JS::NativeFunction::create(realm, client_x_getter, 0, "clientX"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_client_x_setter;

    define_direct_accessor("clientX"_utf16_fly_string, native_client_x_getter, native_client_x_setter, default_attributes);

    auto native_client_y_getter = JS::NativeFunction::create(realm, client_y_getter, 0, "clientY"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_client_y_setter;

    define_direct_accessor("clientY"_utf16_fly_string, native_client_y_getter, native_client_y_setter, default_attributes);

    auto native_x_getter = JS::NativeFunction::create(realm, x_getter, 0, "x"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_x_setter;

    define_direct_accessor("x"_utf16_fly_string, native_x_getter, native_x_setter, default_attributes);

    auto native_y_getter = JS::NativeFunction::create(realm, y_getter, 0, "y"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_y_setter;

    define_direct_accessor("y"_utf16_fly_string, native_y_getter, native_y_setter, default_attributes);

    auto native_offset_x_getter = JS::NativeFunction::create(realm, offset_x_getter, 0, "offsetX"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_offset_x_setter;

    define_direct_accessor("offsetX"_utf16_fly_string, native_offset_x_getter, native_offset_x_setter, default_attributes);

    auto native_offset_y_getter = JS::NativeFunction::create(realm, offset_y_getter, 0, "offsetY"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_offset_y_setter;

    define_direct_accessor("offsetY"_utf16_fly_string, native_offset_y_getter, native_offset_y_setter, default_attributes);

    auto native_ctrl_key_getter = JS::NativeFunction::create(realm, ctrl_key_getter, 0, "ctrlKey"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_ctrl_key_setter;

    define_direct_accessor("ctrlKey"_utf16_fly_string, native_ctrl_key_getter, native_ctrl_key_setter, default_attributes);

    auto native_shift_key_getter = JS::NativeFunction::create(realm, shift_key_getter, 0, "shiftKey"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_shift_key_setter;

    define_direct_accessor("shiftKey"_utf16_fly_string, native_shift_key_getter, native_shift_key_setter, default_attributes);

    auto native_alt_key_getter = JS::NativeFunction::create(realm, alt_key_getter, 0, "altKey"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_alt_key_setter;

    define_direct_accessor("altKey"_utf16_fly_string, native_alt_key_getter, native_alt_key_setter, default_attributes);

    auto native_meta_key_getter = JS::NativeFunction::create(realm, meta_key_getter, 0, "metaKey"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_meta_key_setter;

    define_direct_accessor("metaKey"_utf16_fly_string, native_meta_key_getter, native_meta_key_setter, default_attributes);

    auto native_movement_x_getter = JS::NativeFunction::create(realm, movement_x_getter, 0, "movementX"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_movement_x_setter;

    define_direct_accessor("movementX"_utf16_fly_string, native_movement_x_getter, native_movement_x_setter, default_attributes);

    auto native_movement_y_getter = JS::NativeFunction::create(realm, movement_y_getter, 0, "movementY"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_movement_y_setter;

    define_direct_accessor("movementY"_utf16_fly_string, native_movement_y_getter, native_movement_y_setter, default_attributes);

    auto native_button_getter = JS::NativeFunction::create(realm, button_getter, 0, "button"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_button_setter;

    define_direct_accessor("button"_utf16_fly_string, native_button_getter, native_button_setter, default_attributes);

    auto native_buttons_getter = JS::NativeFunction::create(realm, buttons_getter, 0, "buttons"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_buttons_setter;

    define_direct_accessor("buttons"_utf16_fly_string, native_buttons_getter, native_buttons_setter, default_attributes);

    auto native_related_target_getter = JS::NativeFunction::create(realm, related_target_getter, 0, "relatedTarget"_utf16_fly_string, &realm, "get"sv);

    GC::Ptr<JS::NativeFunction> native_related_target_setter;

    define_direct_accessor("relatedTarget"_utf16_fly_string, native_related_target_getter, native_related_target_setter, default_attributes);

    define_native_function(realm, "getModifierState"_utf16_fly_string, get_modifier_state, 1, default_attributes);

    define_native_function(realm, "initMouseEvent"_utf16_fly_string, init_mouse_event, 1, default_attributes);

    define_direct_property(vm.well_known_symbol_to_string_tag(), JS::PrimitiveString::create(vm, "MouseEvent"_string), JS::Attribute::Configurable);

    Base::initialize(realm);

}

void MouseEventPrototype::define_unforgeable_attributes(JS::Realm& realm, [[maybe_unused]] JS::Object& object)
{


    [[maybe_unused]] auto& vm = realm.vm();


    [[maybe_unused]] u8 default_attributes = JS::Attribute::Enumerable;

}

[[maybe_unused]] static JS::ThrowCompletionOr<UIEvents::MouseEvent*> impl_from(JS::VM& vm)
{
    auto this_value = vm.this_value();
    JS::Object* this_object = nullptr;
    if (this_value.is_nullish())
        this_object = &vm.current_realm()->global_object();
    else
        this_object = TRY(this_value.to_object(vm));

    if (!is<UIEvents::MouseEvent>(this_object))
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "MouseEvent");
    return static_cast<UIEvents::MouseEvent*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::screen_x_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::screen_x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->screen_x(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::screen_y_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::screen_y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->screen_y(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::page_x_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::page_x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->page_x(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::page_y_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::page_y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->page_y(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::client_x_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::client_x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->client_x(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::client_y_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::client_y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->client_y(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::x_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->x(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::y_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->y(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::offset_x_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::offset_x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->offset_x(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::offset_y_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::offset_y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->offset_y(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::ctrl_key_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::ctrl_key_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->ctrl_key(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::shift_key_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::shift_key_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->shift_key(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::alt_key_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::alt_key_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->alt_key(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::meta_key_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::meta_key_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->meta_key(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::movement_x_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::movement_x_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->movement_x(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::movement_y_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::movement_y_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->movement_y(); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::button_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::button_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->button(); }));

    return JS::Value(static_cast<WebIDL::Short>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::buttons_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::buttons_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->buttons(); }));

    return JS::Value(static_cast<WebIDL::UnsignedShort>(retval));

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::related_target_getter)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::related_target_getter");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    [[maybe_unused]] auto* impl = TRY(impl_from(vm));

    auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->related_target(); }));

    if (retval) {

    return &const_cast<EventTarget&>(*retval);

    } else {
        return JS::js_null();
    }

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::get_modifier_state)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::get_modifier_state");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "getModifierState");

    auto arg0 = vm.argument(0);

    String key_arg;
    if (!false || !arg0.is_null()) {
        key_arg = TRY(WebIDL::to_string(vm, arg0));
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->get_modifier_state(key_arg); }));

    return JS::Value(retval);

}

JS_DEFINE_NATIVE_FUNCTION(MouseEventPrototype::init_mouse_event)
{
    WebIDL::log_trace(vm, "MouseEventPrototype::init_mouse_event");
    [[maybe_unused]] auto& realm = *vm.current_realm();

    auto* impl = TRY(impl_from(vm));

    if (vm.argument_count() < 1)
        return vm.throw_completion<JS::TypeError>(JS::ErrorType::BadArgCountOne, "initMouseEvent");

    auto arg0 = vm.argument(0);

    String type_arg;
    if (!false || !arg0.is_null()) {
        type_arg = TRY(WebIDL::to_string(vm, arg0));
    }

    auto arg1 = vm.argument(1);

    bool bubbles_arg;

    if (!arg1.is_undefined())

    bubbles_arg = arg1.to_boolean();

    else
        bubbles_arg = static_cast<bool>(false);

    auto arg2 = vm.argument(2);

    bool cancelable_arg;

    if (!arg2.is_undefined())

    cancelable_arg = arg2.to_boolean();

    else
        cancelable_arg = static_cast<bool>(false);

    auto arg3 = vm.argument(3);

    GC::Ptr<WindowProxy> view_arg;

    if (!arg3.is_nullish()) {
        if (!arg3.is_object() || !is<WindowProxy>(arg3.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "WindowProxy");

        view_arg = &static_cast<WindowProxy&>(arg3.as_object());
    }

    auto arg4 = vm.argument(4);

    WebIDL::Long detail_arg;

    if (!arg4.is_undefined())

    detail_arg = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg4, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        detail_arg = static_cast<WebIDL::Long>(0);

    auto arg5 = vm.argument(5);

    WebIDL::Long screen_x_arg;

    if (!arg5.is_undefined())

    screen_x_arg = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg5, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        screen_x_arg = static_cast<WebIDL::Long>(0);

    auto arg6 = vm.argument(6);

    WebIDL::Long screen_y_arg;

    if (!arg6.is_undefined())

    screen_y_arg = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg6, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        screen_y_arg = static_cast<WebIDL::Long>(0);

    auto arg7 = vm.argument(7);

    WebIDL::Long client_x_arg;

    if (!arg7.is_undefined())

    client_x_arg = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg7, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        client_x_arg = static_cast<WebIDL::Long>(0);

    auto arg8 = vm.argument(8);

    WebIDL::Long client_y_arg;

    if (!arg8.is_undefined())

    client_y_arg = TRY(WebIDL::convert_to_int<WebIDL::Long>(vm, arg8, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        client_y_arg = static_cast<WebIDL::Long>(0);

    auto arg9 = vm.argument(9);

    bool ctrl_key_arg;

    if (!arg9.is_undefined())

    ctrl_key_arg = arg9.to_boolean();

    else
        ctrl_key_arg = static_cast<bool>(false);

    auto arg10 = vm.argument(10);

    bool alt_key_arg;

    if (!arg10.is_undefined())

    alt_key_arg = arg10.to_boolean();

    else
        alt_key_arg = static_cast<bool>(false);

    auto arg11 = vm.argument(11);

    bool shift_key_arg;

    if (!arg11.is_undefined())

    shift_key_arg = arg11.to_boolean();

    else
        shift_key_arg = static_cast<bool>(false);

    auto arg12 = vm.argument(12);

    bool meta_key_arg;

    if (!arg12.is_undefined())

    meta_key_arg = arg12.to_boolean();

    else
        meta_key_arg = static_cast<bool>(false);

    auto arg13 = vm.argument(13);

    WebIDL::Short button_arg;

    if (!arg13.is_undefined())

    button_arg = TRY(WebIDL::convert_to_int<WebIDL::Short>(vm, arg13, WebIDL::EnforceRange::No, WebIDL::Clamp::No));

    else
        button_arg = static_cast<WebIDL::Short>(0);

    auto arg14 = vm.argument(14);

    GC::Ptr<EventTarget> related_target_arg;

    if (!arg14.is_nullish()) {
        if (!arg14.is_object() || !is<EventTarget>(arg14.as_object()))
            return vm.throw_completion<JS::TypeError>(JS::ErrorType::NotAnObjectOfType, "EventTarget");

        related_target_arg = &static_cast<EventTarget&>(arg14.as_object());
    }

    [[maybe_unused]] auto retval = TRY(throw_dom_exception_if_needed(vm, [&] { return impl->init_mouse_event(type_arg, bubbles_arg, cancelable_arg, view_arg, detail_arg, screen_x_arg, screen_y_arg, client_x_arg, client_y_arg, ctrl_key_arg, alt_key_arg, shift_key_arg, meta_key_arg, button_arg, related_target_arg); }));

    return JS::js_undefined();

}

} // namespace Web::Bindings
