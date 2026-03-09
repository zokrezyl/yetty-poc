
#include <LibGC/DeferGC.h>
#include <LibJS/Runtime/Object.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/Bindings/PrincipalHostDefined.h>
#include <LibWeb/Export.h>
#include <LibWeb/HTML/Window.h>
#include <LibWeb/HTML/DedicatedWorkerGlobalScope.h>
#include <LibWeb/HTML/SharedWorkerGlobalScope.h>
#include <LibWeb/HTML/ShadowRealmGlobalScope.h>
#include <LibWeb/Bindings/AnimationConstructor.h>
#include <LibWeb/Bindings/AnimationPrototype.h>
#include <LibWeb/Bindings/AnimationEffectConstructor.h>
#include <LibWeb/Bindings/AnimationEffectPrototype.h>
#include <LibWeb/Bindings/AnimationPlaybackEventConstructor.h>
#include <LibWeb/Bindings/AnimationPlaybackEventPrototype.h>
#include <LibWeb/Bindings/AnimationTimelineConstructor.h>
#include <LibWeb/Bindings/AnimationTimelinePrototype.h>
#include <LibWeb/Bindings/DocumentTimelineConstructor.h>
#include <LibWeb/Bindings/DocumentTimelinePrototype.h>
#include <LibWeb/Bindings/KeyframeEffectConstructor.h>
#include <LibWeb/Bindings/KeyframeEffectPrototype.h>
#include <LibWeb/Bindings/ScrollTimelineConstructor.h>
#include <LibWeb/Bindings/ScrollTimelinePrototype.h>
#include <LibWeb/Bindings/ClipboardConstructor.h>
#include <LibWeb/Bindings/ClipboardPrototype.h>
#include <LibWeb/Bindings/ClipboardEventConstructor.h>
#include <LibWeb/Bindings/ClipboardEventPrototype.h>
#include <LibWeb/Bindings/ClipboardItemConstructor.h>
#include <LibWeb/Bindings/ClipboardItemPrototype.h>
#include <LibWeb/Bindings/SecurityPolicyViolationEventConstructor.h>
#include <LibWeb/Bindings/SecurityPolicyViolationEventPrototype.h>
#include <LibWeb/Bindings/CompressionStreamConstructor.h>
#include <LibWeb/Bindings/CompressionStreamPrototype.h>
#include <LibWeb/Bindings/DecompressionStreamConstructor.h>
#include <LibWeb/Bindings/DecompressionStreamPrototype.h>
#include <LibWeb/Bindings/CookieChangeEventConstructor.h>
#include <LibWeb/Bindings/CookieChangeEventPrototype.h>
#include <LibWeb/Bindings/CookieStoreConstructor.h>
#include <LibWeb/Bindings/CookieStorePrototype.h>
#include <LibWeb/Bindings/CredentialConstructor.h>
#include <LibWeb/Bindings/CredentialPrototype.h>
#include <LibWeb/Bindings/CredentialsContainerConstructor.h>
#include <LibWeb/Bindings/CredentialsContainerPrototype.h>
#include <LibWeb/Bindings/FederatedCredentialConstructor.h>
#include <LibWeb/Bindings/FederatedCredentialPrototype.h>
#include <LibWeb/Bindings/PasswordCredentialConstructor.h>
#include <LibWeb/Bindings/PasswordCredentialPrototype.h>
#include <LibWeb/Bindings/CryptoConstructor.h>
#include <LibWeb/Bindings/CryptoPrototype.h>
#include <LibWeb/Bindings/CryptoKeyConstructor.h>
#include <LibWeb/Bindings/CryptoKeyPrototype.h>
#include <LibWeb/Bindings/SubtleCryptoConstructor.h>
#include <LibWeb/Bindings/SubtleCryptoPrototype.h>
#include <LibWeb/Bindings/AnimationEventConstructor.h>
#include <LibWeb/Bindings/AnimationEventPrototype.h>
#include <LibWeb/Bindings/CSSAnimationConstructor.h>
#include <LibWeb/Bindings/CSSAnimationPrototype.h>
#include <LibWeb/Bindings/CSSConditionRuleConstructor.h>
#include <LibWeb/Bindings/CSSConditionRulePrototype.h>
#include <LibWeb/Bindings/CSSCounterStyleRuleConstructor.h>
#include <LibWeb/Bindings/CSSCounterStyleRulePrototype.h>
#include <LibWeb/Bindings/CSSFontFaceDescriptorsConstructor.h>
#include <LibWeb/Bindings/CSSFontFaceDescriptorsPrototype.h>
#include <LibWeb/Bindings/CSSFontFaceRuleConstructor.h>
#include <LibWeb/Bindings/CSSFontFaceRulePrototype.h>
#include <LibWeb/Bindings/CSSFontFeatureValuesMapConstructor.h>
#include <LibWeb/Bindings/CSSFontFeatureValuesMapPrototype.h>
#include <LibWeb/Bindings/CSSFontFeatureValuesRuleConstructor.h>
#include <LibWeb/Bindings/CSSFontFeatureValuesRulePrototype.h>
#include <LibWeb/Bindings/CSSGroupingRuleConstructor.h>
#include <LibWeb/Bindings/CSSGroupingRulePrototype.h>
#include <LibWeb/Bindings/CSSImageValueConstructor.h>
#include <LibWeb/Bindings/CSSImageValuePrototype.h>
#include <LibWeb/Bindings/CSSImportRuleConstructor.h>
#include <LibWeb/Bindings/CSSImportRulePrototype.h>
#include <LibWeb/Bindings/CSSKeyframeRuleConstructor.h>
#include <LibWeb/Bindings/CSSKeyframeRulePrototype.h>
#include <LibWeb/Bindings/CSSKeyframesRuleConstructor.h>
#include <LibWeb/Bindings/CSSKeyframesRulePrototype.h>
#include <LibWeb/Bindings/CSSKeywordValueConstructor.h>
#include <LibWeb/Bindings/CSSKeywordValuePrototype.h>
#include <LibWeb/Bindings/CSSLayerBlockRuleConstructor.h>
#include <LibWeb/Bindings/CSSLayerBlockRulePrototype.h>
#include <LibWeb/Bindings/CSSLayerStatementRuleConstructor.h>
#include <LibWeb/Bindings/CSSLayerStatementRulePrototype.h>
#include <LibWeb/Bindings/CSSMarginRuleConstructor.h>
#include <LibWeb/Bindings/CSSMarginRulePrototype.h>
#include <LibWeb/Bindings/CSSMathClampConstructor.h>
#include <LibWeb/Bindings/CSSMathClampPrototype.h>
#include <LibWeb/Bindings/CSSMathInvertConstructor.h>
#include <LibWeb/Bindings/CSSMathInvertPrototype.h>
#include <LibWeb/Bindings/CSSMathMaxConstructor.h>
#include <LibWeb/Bindings/CSSMathMaxPrototype.h>
#include <LibWeb/Bindings/CSSMathMinConstructor.h>
#include <LibWeb/Bindings/CSSMathMinPrototype.h>
#include <LibWeb/Bindings/CSSMathNegateConstructor.h>
#include <LibWeb/Bindings/CSSMathNegatePrototype.h>
#include <LibWeb/Bindings/CSSMathProductConstructor.h>
#include <LibWeb/Bindings/CSSMathProductPrototype.h>
#include <LibWeb/Bindings/CSSMathSumConstructor.h>
#include <LibWeb/Bindings/CSSMathSumPrototype.h>
#include <LibWeb/Bindings/CSSMathValueConstructor.h>
#include <LibWeb/Bindings/CSSMathValuePrototype.h>
#include <LibWeb/Bindings/CSSMatrixComponentConstructor.h>
#include <LibWeb/Bindings/CSSMatrixComponentPrototype.h>
#include <LibWeb/Bindings/CSSMediaRuleConstructor.h>
#include <LibWeb/Bindings/CSSMediaRulePrototype.h>
#include <LibWeb/Bindings/CSSNamespaceRuleConstructor.h>
#include <LibWeb/Bindings/CSSNamespaceRulePrototype.h>
#include <LibWeb/Bindings/CSSNestedDeclarationsConstructor.h>
#include <LibWeb/Bindings/CSSNestedDeclarationsPrototype.h>
#include <LibWeb/Bindings/CSSNumericArrayConstructor.h>
#include <LibWeb/Bindings/CSSNumericArrayPrototype.h>
#include <LibWeb/Bindings/CSSNumericValueConstructor.h>
#include <LibWeb/Bindings/CSSNumericValuePrototype.h>
#include <LibWeb/Bindings/CSSPageRuleConstructor.h>
#include <LibWeb/Bindings/CSSPageRulePrototype.h>
#include <LibWeb/Bindings/CSSPageDescriptorsConstructor.h>
#include <LibWeb/Bindings/CSSPageDescriptorsPrototype.h>
#include <LibWeb/Bindings/CSSPerspectiveConstructor.h>
#include <LibWeb/Bindings/CSSPerspectivePrototype.h>
#include <LibWeb/Bindings/CSSPropertyRuleConstructor.h>
#include <LibWeb/Bindings/CSSPropertyRulePrototype.h>
#include <LibWeb/Bindings/CSSRotateConstructor.h>
#include <LibWeb/Bindings/CSSRotatePrototype.h>
#include <LibWeb/Bindings/CSSRuleConstructor.h>
#include <LibWeb/Bindings/CSSRulePrototype.h>
#include <LibWeb/Bindings/CSSRuleListConstructor.h>
#include <LibWeb/Bindings/CSSRuleListPrototype.h>
#include <LibWeb/Bindings/CSSScaleConstructor.h>
#include <LibWeb/Bindings/CSSScalePrototype.h>
#include <LibWeb/Bindings/CSSSkewConstructor.h>
#include <LibWeb/Bindings/CSSSkewPrototype.h>
#include <LibWeb/Bindings/CSSSkewXConstructor.h>
#include <LibWeb/Bindings/CSSSkewXPrototype.h>
#include <LibWeb/Bindings/CSSSkewYConstructor.h>
#include <LibWeb/Bindings/CSSSkewYPrototype.h>
#include <LibWeb/Bindings/CSSStyleDeclarationConstructor.h>
#include <LibWeb/Bindings/CSSStyleDeclarationPrototype.h>
#include <LibWeb/Bindings/CSSStylePropertiesConstructor.h>
#include <LibWeb/Bindings/CSSStylePropertiesPrototype.h>
#include <LibWeb/Bindings/CSSStyleRuleConstructor.h>
#include <LibWeb/Bindings/CSSStyleRulePrototype.h>
#include <LibWeb/Bindings/CSSStyleSheetConstructor.h>
#include <LibWeb/Bindings/CSSStyleSheetPrototype.h>
#include <LibWeb/Bindings/CSSStyleValueConstructor.h>
#include <LibWeb/Bindings/CSSStyleValuePrototype.h>
#include <LibWeb/Bindings/CSSSupportsRuleConstructor.h>
#include <LibWeb/Bindings/CSSSupportsRulePrototype.h>
#include <LibWeb/Bindings/CSSTransformComponentConstructor.h>
#include <LibWeb/Bindings/CSSTransformComponentPrototype.h>
#include <LibWeb/Bindings/CSSTransformValueConstructor.h>
#include <LibWeb/Bindings/CSSTransformValuePrototype.h>
#include <LibWeb/Bindings/CSSTransitionConstructor.h>
#include <LibWeb/Bindings/CSSTransitionPrototype.h>
#include <LibWeb/Bindings/CSSTranslateConstructor.h>
#include <LibWeb/Bindings/CSSTranslatePrototype.h>
#include <LibWeb/Bindings/CSSUnitValueConstructor.h>
#include <LibWeb/Bindings/CSSUnitValuePrototype.h>
#include <LibWeb/Bindings/CSSUnparsedValueConstructor.h>
#include <LibWeb/Bindings/CSSUnparsedValuePrototype.h>
#include <LibWeb/Bindings/CSSVariableReferenceValueConstructor.h>
#include <LibWeb/Bindings/CSSVariableReferenceValuePrototype.h>
#include <LibWeb/Bindings/FontFaceConstructor.h>
#include <LibWeb/Bindings/FontFacePrototype.h>
#include <LibWeb/Bindings/FontFaceSetConstructor.h>
#include <LibWeb/Bindings/FontFaceSetPrototype.h>
#include <LibWeb/Bindings/FontFaceSetLoadEventConstructor.h>
#include <LibWeb/Bindings/FontFaceSetLoadEventPrototype.h>
#include <LibWeb/Bindings/MediaListConstructor.h>
#include <LibWeb/Bindings/MediaListPrototype.h>
#include <LibWeb/Bindings/MediaQueryListConstructor.h>
#include <LibWeb/Bindings/MediaQueryListPrototype.h>
#include <LibWeb/Bindings/MediaQueryListEventConstructor.h>
#include <LibWeb/Bindings/MediaQueryListEventPrototype.h>
#include <LibWeb/Bindings/ScreenConstructor.h>
#include <LibWeb/Bindings/ScreenPrototype.h>
#include <LibWeb/Bindings/ScreenOrientationConstructor.h>
#include <LibWeb/Bindings/ScreenOrientationPrototype.h>
#include <LibWeb/Bindings/StylePropertyMapConstructor.h>
#include <LibWeb/Bindings/StylePropertyMapPrototype.h>
#include <LibWeb/Bindings/StylePropertyMapReadOnlyConstructor.h>
#include <LibWeb/Bindings/StylePropertyMapReadOnlyPrototype.h>
#include <LibWeb/Bindings/StyleSheetConstructor.h>
#include <LibWeb/Bindings/StyleSheetPrototype.h>
#include <LibWeb/Bindings/StyleSheetListConstructor.h>
#include <LibWeb/Bindings/StyleSheetListPrototype.h>
#include <LibWeb/Bindings/TransitionEventConstructor.h>
#include <LibWeb/Bindings/TransitionEventPrototype.h>
#include <LibWeb/Bindings/VisualViewportConstructor.h>
#include <LibWeb/Bindings/VisualViewportPrototype.h>
#include <LibWeb/Bindings/AbstractRangeConstructor.h>
#include <LibWeb/Bindings/AbstractRangePrototype.h>
#include <LibWeb/Bindings/AttrConstructor.h>
#include <LibWeb/Bindings/AttrPrototype.h>
#include <LibWeb/Bindings/AbortControllerConstructor.h>
#include <LibWeb/Bindings/AbortControllerPrototype.h>
#include <LibWeb/Bindings/AbortSignalConstructor.h>
#include <LibWeb/Bindings/AbortSignalPrototype.h>
#include <LibWeb/Bindings/CDATASectionConstructor.h>
#include <LibWeb/Bindings/CDATASectionPrototype.h>
#include <LibWeb/Bindings/CharacterDataConstructor.h>
#include <LibWeb/Bindings/CharacterDataPrototype.h>
#include <LibWeb/Bindings/CommentConstructor.h>
#include <LibWeb/Bindings/CommentPrototype.h>
#include <LibWeb/Bindings/CustomEventConstructor.h>
#include <LibWeb/Bindings/CustomEventPrototype.h>
#include <LibWeb/Bindings/DocumentConstructor.h>
#include <LibWeb/Bindings/DocumentPrototype.h>
#include <LibWeb/Bindings/DocumentFragmentConstructor.h>
#include <LibWeb/Bindings/DocumentFragmentPrototype.h>
#include <LibWeb/Bindings/DocumentTypeConstructor.h>
#include <LibWeb/Bindings/DocumentTypePrototype.h>
#include <LibWeb/Bindings/DOMImplementationConstructor.h>
#include <LibWeb/Bindings/DOMImplementationPrototype.h>
#include <LibWeb/Bindings/DOMTokenListConstructor.h>
#include <LibWeb/Bindings/DOMTokenListPrototype.h>
#include <LibWeb/Bindings/ElementConstructor.h>
#include <LibWeb/Bindings/ElementPrototype.h>
#include <LibWeb/Bindings/EventConstructor.h>
#include <LibWeb/Bindings/EventPrototype.h>
#include <LibWeb/Bindings/EventTargetConstructor.h>
#include <LibWeb/Bindings/EventTargetPrototype.h>
#include <LibWeb/Bindings/HTMLCollectionConstructor.h>
#include <LibWeb/Bindings/HTMLCollectionPrototype.h>
#include <LibWeb/Bindings/MutationObserverConstructor.h>
#include <LibWeb/Bindings/MutationObserverPrototype.h>
#include <LibWeb/Bindings/MutationRecordConstructor.h>
#include <LibWeb/Bindings/MutationRecordPrototype.h>
#include <LibWeb/Bindings/NamedNodeMapConstructor.h>
#include <LibWeb/Bindings/NamedNodeMapPrototype.h>
#include <LibWeb/Bindings/NodeConstructor.h>
#include <LibWeb/Bindings/NodePrototype.h>
#include <LibWeb/Bindings/NodeFilterConstructor.h>
#include <LibWeb/Bindings/NodeFilterPrototype.h>
#include <LibWeb/Bindings/NodeIteratorConstructor.h>
#include <LibWeb/Bindings/NodeIteratorPrototype.h>
#include <LibWeb/Bindings/NodeListConstructor.h>
#include <LibWeb/Bindings/NodeListPrototype.h>
#include <LibWeb/Bindings/ProcessingInstructionConstructor.h>
#include <LibWeb/Bindings/ProcessingInstructionPrototype.h>
#include <LibWeb/Bindings/RangeConstructor.h>
#include <LibWeb/Bindings/RangePrototype.h>
#include <LibWeb/Bindings/ShadowRootConstructor.h>
#include <LibWeb/Bindings/ShadowRootPrototype.h>
#include <LibWeb/Bindings/StaticRangeConstructor.h>
#include <LibWeb/Bindings/StaticRangePrototype.h>
#include <LibWeb/Bindings/TextConstructor.h>
#include <LibWeb/Bindings/TextPrototype.h>
#include <LibWeb/Bindings/TreeWalkerConstructor.h>
#include <LibWeb/Bindings/TreeWalkerPrototype.h>
#include <LibWeb/Bindings/XMLDocumentConstructor.h>
#include <LibWeb/Bindings/XMLDocumentPrototype.h>
#include <LibWeb/Bindings/DOMURLConstructor.h>
#include <LibWeb/Bindings/DOMURLPrototype.h>
#include <LibWeb/Bindings/OriginConstructor.h>
#include <LibWeb/Bindings/OriginPrototype.h>
#include <LibWeb/Bindings/URLSearchParamsConstructor.h>
#include <LibWeb/Bindings/URLSearchParamsPrototype.h>
#include <LibWeb/Bindings/TextDecoderConstructor.h>
#include <LibWeb/Bindings/TextDecoderPrototype.h>
#include <LibWeb/Bindings/TextEncoderConstructor.h>
#include <LibWeb/Bindings/TextEncoderPrototype.h>
#include <LibWeb/Bindings/TextEncoderStreamConstructor.h>
#include <LibWeb/Bindings/TextEncoderStreamPrototype.h>
#include <LibWeb/Bindings/MediaKeySystemAccessConstructor.h>
#include <LibWeb/Bindings/MediaKeySystemAccessPrototype.h>
#include <LibWeb/Bindings/FileSystemEntryConstructor.h>
#include <LibWeb/Bindings/FileSystemEntryPrototype.h>
#include <LibWeb/Bindings/PerformanceEventTimingConstructor.h>
#include <LibWeb/Bindings/PerformanceEventTimingPrototype.h>
#include <LibWeb/Bindings/HeadersConstructor.h>
#include <LibWeb/Bindings/HeadersPrototype.h>
#include <LibWeb/Bindings/RequestConstructor.h>
#include <LibWeb/Bindings/RequestPrototype.h>
#include <LibWeb/Bindings/ResponseConstructor.h>
#include <LibWeb/Bindings/ResponsePrototype.h>
#include <LibWeb/Bindings/BlobConstructor.h>
#include <LibWeb/Bindings/BlobPrototype.h>
#include <LibWeb/Bindings/FileConstructor.h>
#include <LibWeb/Bindings/FilePrototype.h>
#include <LibWeb/Bindings/FileListConstructor.h>
#include <LibWeb/Bindings/FileListPrototype.h>
#include <LibWeb/Bindings/FileReaderConstructor.h>
#include <LibWeb/Bindings/FileReaderPrototype.h>
#include <LibWeb/Bindings/FileReaderSyncConstructor.h>
#include <LibWeb/Bindings/FileReaderSyncPrototype.h>
#include <LibWeb/Bindings/GamepadConstructor.h>
#include <LibWeb/Bindings/GamepadPrototype.h>
#include <LibWeb/Bindings/GamepadButtonConstructor.h>
#include <LibWeb/Bindings/GamepadButtonPrototype.h>
#include <LibWeb/Bindings/GamepadEventConstructor.h>
#include <LibWeb/Bindings/GamepadEventPrototype.h>
#include <LibWeb/Bindings/GamepadHapticActuatorConstructor.h>
#include <LibWeb/Bindings/GamepadHapticActuatorPrototype.h>
#include <LibWeb/Bindings/GeolocationConstructor.h>
#include <LibWeb/Bindings/GeolocationPrototype.h>
#include <LibWeb/Bindings/GeolocationCoordinatesConstructor.h>
#include <LibWeb/Bindings/GeolocationCoordinatesPrototype.h>
#include <LibWeb/Bindings/GeolocationPositionConstructor.h>
#include <LibWeb/Bindings/GeolocationPositionPrototype.h>
#include <LibWeb/Bindings/GeolocationPositionErrorConstructor.h>
#include <LibWeb/Bindings/GeolocationPositionErrorPrototype.h>
#include <LibWeb/Bindings/DOMMatrixConstructor.h>
#include <LibWeb/Bindings/DOMMatrixPrototype.h>
#include <LibWeb/Bindings/DOMMatrixReadOnlyConstructor.h>
#include <LibWeb/Bindings/DOMMatrixReadOnlyPrototype.h>
#include <LibWeb/Bindings/DOMPointConstructor.h>
#include <LibWeb/Bindings/DOMPointPrototype.h>
#include <LibWeb/Bindings/DOMPointReadOnlyConstructor.h>
#include <LibWeb/Bindings/DOMPointReadOnlyPrototype.h>
#include <LibWeb/Bindings/DOMQuadConstructor.h>
#include <LibWeb/Bindings/DOMQuadPrototype.h>
#include <LibWeb/Bindings/DOMRectConstructor.h>
#include <LibWeb/Bindings/DOMRectPrototype.h>
#include <LibWeb/Bindings/DOMRectListConstructor.h>
#include <LibWeb/Bindings/DOMRectListPrototype.h>
#include <LibWeb/Bindings/DOMRectReadOnlyConstructor.h>
#include <LibWeb/Bindings/DOMRectReadOnlyPrototype.h>
#include <LibWeb/Bindings/AudioTrackConstructor.h>
#include <LibWeb/Bindings/AudioTrackPrototype.h>
#include <LibWeb/Bindings/AudioTrackListConstructor.h>
#include <LibWeb/Bindings/AudioTrackListPrototype.h>
#include <LibWeb/Bindings/BarPropConstructor.h>
#include <LibWeb/Bindings/BarPropPrototype.h>
#include <LibWeb/Bindings/BeforeUnloadEventConstructor.h>
#include <LibWeb/Bindings/BeforeUnloadEventPrototype.h>
#include <LibWeb/Bindings/BroadcastChannelConstructor.h>
#include <LibWeb/Bindings/BroadcastChannelPrototype.h>
#include <LibWeb/Bindings/CanvasGradientConstructor.h>
#include <LibWeb/Bindings/CanvasGradientPrototype.h>
#include <LibWeb/Bindings/CanvasPatternConstructor.h>
#include <LibWeb/Bindings/CanvasPatternPrototype.h>
#include <LibWeb/Bindings/CanvasRenderingContext2DConstructor.h>
#include <LibWeb/Bindings/CanvasRenderingContext2DPrototype.h>
#include <LibWeb/Bindings/CloseEventConstructor.h>
#include <LibWeb/Bindings/CloseEventPrototype.h>
#include <LibWeb/Bindings/CloseWatcherConstructor.h>
#include <LibWeb/Bindings/CloseWatcherPrototype.h>
#include <LibWeb/Bindings/CommandEventConstructor.h>
#include <LibWeb/Bindings/CommandEventPrototype.h>
#include <LibWeb/Bindings/CustomElementRegistryConstructor.h>
#include <LibWeb/Bindings/CustomElementRegistryPrototype.h>
#include <LibWeb/Bindings/CustomStateSetConstructor.h>
#include <LibWeb/Bindings/CustomStateSetPrototype.h>
#include <LibWeb/Bindings/DataTransferConstructor.h>
#include <LibWeb/Bindings/DataTransferPrototype.h>
#include <LibWeb/Bindings/DataTransferItemConstructor.h>
#include <LibWeb/Bindings/DataTransferItemPrototype.h>
#include <LibWeb/Bindings/DataTransferItemListConstructor.h>
#include <LibWeb/Bindings/DataTransferItemListPrototype.h>
#include <LibWeb/Bindings/DedicatedWorkerGlobalScopeConstructor.h>
#include <LibWeb/Bindings/DedicatedWorkerGlobalScopePrototype.h>
#include <LibWeb/Bindings/DOMParserConstructor.h>
#include <LibWeb/Bindings/DOMParserPrototype.h>
#include <LibWeb/Bindings/DOMStringListConstructor.h>
#include <LibWeb/Bindings/DOMStringListPrototype.h>
#include <LibWeb/Bindings/DOMStringMapConstructor.h>
#include <LibWeb/Bindings/DOMStringMapPrototype.h>
#include <LibWeb/Bindings/DragEventConstructor.h>
#include <LibWeb/Bindings/DragEventPrototype.h>
#include <LibWeb/Bindings/ElementInternalsConstructor.h>
#include <LibWeb/Bindings/ElementInternalsPrototype.h>
#include <LibWeb/Bindings/ErrorEventConstructor.h>
#include <LibWeb/Bindings/ErrorEventPrototype.h>
#include <LibWeb/Bindings/EventSourceConstructor.h>
#include <LibWeb/Bindings/EventSourcePrototype.h>
#include <LibWeb/Bindings/ExternalConstructor.h>
#include <LibWeb/Bindings/ExternalPrototype.h>
#include <LibWeb/Bindings/FormDataEventConstructor.h>
#include <LibWeb/Bindings/FormDataEventPrototype.h>
#include <LibWeb/Bindings/HashChangeEventConstructor.h>
#include <LibWeb/Bindings/HashChangeEventPrototype.h>
#include <LibWeb/Bindings/HistoryConstructor.h>
#include <LibWeb/Bindings/HistoryPrototype.h>
#include <LibWeb/Bindings/HTMLAllCollectionConstructor.h>
#include <LibWeb/Bindings/HTMLAllCollectionPrototype.h>
#include <LibWeb/Bindings/HTMLAnchorElementConstructor.h>
#include <LibWeb/Bindings/HTMLAnchorElementPrototype.h>
#include <LibWeb/Bindings/HTMLAreaElementConstructor.h>
#include <LibWeb/Bindings/HTMLAreaElementPrototype.h>
#include <LibWeb/Bindings/HTMLAudioElementConstructor.h>
#include <LibWeb/Bindings/HTMLAudioElementPrototype.h>
#include <LibWeb/Bindings/AudioConstructor.h>
#include <LibWeb/Bindings/HTMLBaseElementConstructor.h>
#include <LibWeb/Bindings/HTMLBaseElementPrototype.h>
#include <LibWeb/Bindings/HTMLBodyElementConstructor.h>
#include <LibWeb/Bindings/HTMLBodyElementPrototype.h>
#include <LibWeb/Bindings/HTMLBRElementConstructor.h>
#include <LibWeb/Bindings/HTMLBRElementPrototype.h>
#include <LibWeb/Bindings/HTMLButtonElementConstructor.h>
#include <LibWeb/Bindings/HTMLButtonElementPrototype.h>
#include <LibWeb/Bindings/HTMLCanvasElementConstructor.h>
#include <LibWeb/Bindings/HTMLCanvasElementPrototype.h>
#include <LibWeb/Bindings/HTMLDataElementConstructor.h>
#include <LibWeb/Bindings/HTMLDataElementPrototype.h>
#include <LibWeb/Bindings/HTMLDataListElementConstructor.h>
#include <LibWeb/Bindings/HTMLDataListElementPrototype.h>
#include <LibWeb/Bindings/HTMLDetailsElementConstructor.h>
#include <LibWeb/Bindings/HTMLDetailsElementPrototype.h>
#include <LibWeb/Bindings/HTMLDialogElementConstructor.h>
#include <LibWeb/Bindings/HTMLDialogElementPrototype.h>
#include <LibWeb/Bindings/HTMLDirectoryElementConstructor.h>
#include <LibWeb/Bindings/HTMLDirectoryElementPrototype.h>
#include <LibWeb/Bindings/HTMLDivElementConstructor.h>
#include <LibWeb/Bindings/HTMLDivElementPrototype.h>
#include <LibWeb/Bindings/HTMLDocumentConstructor.h>
#include <LibWeb/Bindings/HTMLDocumentPrototype.h>
#include <LibWeb/Bindings/HTMLDListElementConstructor.h>
#include <LibWeb/Bindings/HTMLDListElementPrototype.h>
#include <LibWeb/Bindings/HTMLElementConstructor.h>
#include <LibWeb/Bindings/HTMLElementPrototype.h>
#include <LibWeb/Bindings/HTMLEmbedElementConstructor.h>
#include <LibWeb/Bindings/HTMLEmbedElementPrototype.h>
#include <LibWeb/Bindings/HTMLFieldSetElementConstructor.h>
#include <LibWeb/Bindings/HTMLFieldSetElementPrototype.h>
#include <LibWeb/Bindings/HTMLFontElementConstructor.h>
#include <LibWeb/Bindings/HTMLFontElementPrototype.h>
#include <LibWeb/Bindings/HTMLFormControlsCollectionConstructor.h>
#include <LibWeb/Bindings/HTMLFormControlsCollectionPrototype.h>
#include <LibWeb/Bindings/HTMLFormElementConstructor.h>
#include <LibWeb/Bindings/HTMLFormElementPrototype.h>
#include <LibWeb/Bindings/HTMLFrameElementConstructor.h>
#include <LibWeb/Bindings/HTMLFrameElementPrototype.h>
#include <LibWeb/Bindings/HTMLFrameSetElementConstructor.h>
#include <LibWeb/Bindings/HTMLFrameSetElementPrototype.h>
#include <LibWeb/Bindings/HTMLHeadElementConstructor.h>
#include <LibWeb/Bindings/HTMLHeadElementPrototype.h>
#include <LibWeb/Bindings/HTMLHeadingElementConstructor.h>
#include <LibWeb/Bindings/HTMLHeadingElementPrototype.h>
#include <LibWeb/Bindings/HTMLHRElementConstructor.h>
#include <LibWeb/Bindings/HTMLHRElementPrototype.h>
#include <LibWeb/Bindings/HTMLHtmlElementConstructor.h>
#include <LibWeb/Bindings/HTMLHtmlElementPrototype.h>
#include <LibWeb/Bindings/HTMLIFrameElementConstructor.h>
#include <LibWeb/Bindings/HTMLIFrameElementPrototype.h>
#include <LibWeb/Bindings/HTMLImageElementConstructor.h>
#include <LibWeb/Bindings/HTMLImageElementPrototype.h>
#include <LibWeb/Bindings/ImageConstructor.h>
#include <LibWeb/Bindings/HTMLInputElementConstructor.h>
#include <LibWeb/Bindings/HTMLInputElementPrototype.h>
#include <LibWeb/Bindings/HTMLLabelElementConstructor.h>
#include <LibWeb/Bindings/HTMLLabelElementPrototype.h>
#include <LibWeb/Bindings/HTMLLegendElementConstructor.h>
#include <LibWeb/Bindings/HTMLLegendElementPrototype.h>
#include <LibWeb/Bindings/HTMLLIElementConstructor.h>
#include <LibWeb/Bindings/HTMLLIElementPrototype.h>
#include <LibWeb/Bindings/HTMLLinkElementConstructor.h>
#include <LibWeb/Bindings/HTMLLinkElementPrototype.h>
#include <LibWeb/Bindings/HTMLMapElementConstructor.h>
#include <LibWeb/Bindings/HTMLMapElementPrototype.h>
#include <LibWeb/Bindings/HTMLMarqueeElementConstructor.h>
#include <LibWeb/Bindings/HTMLMarqueeElementPrototype.h>
#include <LibWeb/Bindings/HTMLMediaElementConstructor.h>
#include <LibWeb/Bindings/HTMLMediaElementPrototype.h>
#include <LibWeb/Bindings/HTMLMenuElementConstructor.h>
#include <LibWeb/Bindings/HTMLMenuElementPrototype.h>
#include <LibWeb/Bindings/HTMLMetaElementConstructor.h>
#include <LibWeb/Bindings/HTMLMetaElementPrototype.h>
#include <LibWeb/Bindings/HTMLMeterElementConstructor.h>
#include <LibWeb/Bindings/HTMLMeterElementPrototype.h>
#include <LibWeb/Bindings/HTMLModElementConstructor.h>
#include <LibWeb/Bindings/HTMLModElementPrototype.h>
#include <LibWeb/Bindings/HTMLObjectElementConstructor.h>
#include <LibWeb/Bindings/HTMLObjectElementPrototype.h>
#include <LibWeb/Bindings/HTMLOListElementConstructor.h>
#include <LibWeb/Bindings/HTMLOListElementPrototype.h>
#include <LibWeb/Bindings/HTMLOptGroupElementConstructor.h>
#include <LibWeb/Bindings/HTMLOptGroupElementPrototype.h>
#include <LibWeb/Bindings/HTMLOptionElementConstructor.h>
#include <LibWeb/Bindings/HTMLOptionElementPrototype.h>
#include <LibWeb/Bindings/OptionConstructor.h>
#include <LibWeb/Bindings/HTMLOptionsCollectionConstructor.h>
#include <LibWeb/Bindings/HTMLOptionsCollectionPrototype.h>
#include <LibWeb/Bindings/HTMLOutputElementConstructor.h>
#include <LibWeb/Bindings/HTMLOutputElementPrototype.h>
#include <LibWeb/Bindings/HTMLParagraphElementConstructor.h>
#include <LibWeb/Bindings/HTMLParagraphElementPrototype.h>
#include <LibWeb/Bindings/HTMLParamElementConstructor.h>
#include <LibWeb/Bindings/HTMLParamElementPrototype.h>
#include <LibWeb/Bindings/HTMLPictureElementConstructor.h>
#include <LibWeb/Bindings/HTMLPictureElementPrototype.h>
#include <LibWeb/Bindings/HTMLPreElementConstructor.h>
#include <LibWeb/Bindings/HTMLPreElementPrototype.h>
#include <LibWeb/Bindings/HTMLProgressElementConstructor.h>
#include <LibWeb/Bindings/HTMLProgressElementPrototype.h>
#include <LibWeb/Bindings/HTMLQuoteElementConstructor.h>
#include <LibWeb/Bindings/HTMLQuoteElementPrototype.h>
#include <LibWeb/Bindings/HTMLScriptElementConstructor.h>
#include <LibWeb/Bindings/HTMLScriptElementPrototype.h>
#include <LibWeb/Bindings/HTMLSelectedContentElementConstructor.h>
#include <LibWeb/Bindings/HTMLSelectedContentElementPrototype.h>
#include <LibWeb/Bindings/HTMLSelectElementConstructor.h>
#include <LibWeb/Bindings/HTMLSelectElementPrototype.h>
#include <LibWeb/Bindings/HTMLSlotElementConstructor.h>
#include <LibWeb/Bindings/HTMLSlotElementPrototype.h>
#include <LibWeb/Bindings/HTMLSourceElementConstructor.h>
#include <LibWeb/Bindings/HTMLSourceElementPrototype.h>
#include <LibWeb/Bindings/HTMLSpanElementConstructor.h>
#include <LibWeb/Bindings/HTMLSpanElementPrototype.h>
#include <LibWeb/Bindings/HTMLStyleElementConstructor.h>
#include <LibWeb/Bindings/HTMLStyleElementPrototype.h>
#include <LibWeb/Bindings/HTMLTableCaptionElementConstructor.h>
#include <LibWeb/Bindings/HTMLTableCaptionElementPrototype.h>
#include <LibWeb/Bindings/HTMLTableCellElementConstructor.h>
#include <LibWeb/Bindings/HTMLTableCellElementPrototype.h>
#include <LibWeb/Bindings/HTMLTableColElementConstructor.h>
#include <LibWeb/Bindings/HTMLTableColElementPrototype.h>
#include <LibWeb/Bindings/HTMLTableElementConstructor.h>
#include <LibWeb/Bindings/HTMLTableElementPrototype.h>
#include <LibWeb/Bindings/HTMLTableRowElementConstructor.h>
#include <LibWeb/Bindings/HTMLTableRowElementPrototype.h>
#include <LibWeb/Bindings/HTMLTableSectionElementConstructor.h>
#include <LibWeb/Bindings/HTMLTableSectionElementPrototype.h>
#include <LibWeb/Bindings/HTMLTemplateElementConstructor.h>
#include <LibWeb/Bindings/HTMLTemplateElementPrototype.h>
#include <LibWeb/Bindings/HTMLTextAreaElementConstructor.h>
#include <LibWeb/Bindings/HTMLTextAreaElementPrototype.h>
#include <LibWeb/Bindings/HTMLTimeElementConstructor.h>
#include <LibWeb/Bindings/HTMLTimeElementPrototype.h>
#include <LibWeb/Bindings/HTMLTitleElementConstructor.h>
#include <LibWeb/Bindings/HTMLTitleElementPrototype.h>
#include <LibWeb/Bindings/HTMLTrackElementConstructor.h>
#include <LibWeb/Bindings/HTMLTrackElementPrototype.h>
#include <LibWeb/Bindings/HTMLUListElementConstructor.h>
#include <LibWeb/Bindings/HTMLUListElementPrototype.h>
#include <LibWeb/Bindings/HTMLUnknownElementConstructor.h>
#include <LibWeb/Bindings/HTMLUnknownElementPrototype.h>
#include <LibWeb/Bindings/HTMLVideoElementConstructor.h>
#include <LibWeb/Bindings/HTMLVideoElementPrototype.h>
#include <LibWeb/Bindings/ImageBitmapConstructor.h>
#include <LibWeb/Bindings/ImageBitmapPrototype.h>
#include <LibWeb/Bindings/ImageDataConstructor.h>
#include <LibWeb/Bindings/ImageDataPrototype.h>
#include <LibWeb/Bindings/LocationConstructor.h>
#include <LibWeb/Bindings/LocationPrototype.h>
#include <LibWeb/Bindings/MediaErrorConstructor.h>
#include <LibWeb/Bindings/MediaErrorPrototype.h>
#include <LibWeb/Bindings/MessageChannelConstructor.h>
#include <LibWeb/Bindings/MessageChannelPrototype.h>
#include <LibWeb/Bindings/MessageEventConstructor.h>
#include <LibWeb/Bindings/MessageEventPrototype.h>
#include <LibWeb/Bindings/MessagePortConstructor.h>
#include <LibWeb/Bindings/MessagePortPrototype.h>
#include <LibWeb/Bindings/MimeTypeConstructor.h>
#include <LibWeb/Bindings/MimeTypePrototype.h>
#include <LibWeb/Bindings/MimeTypeArrayConstructor.h>
#include <LibWeb/Bindings/MimeTypeArrayPrototype.h>
#include <LibWeb/Bindings/NavigateEventConstructor.h>
#include <LibWeb/Bindings/NavigateEventPrototype.h>
#include <LibWeb/Bindings/NavigationConstructor.h>
#include <LibWeb/Bindings/NavigationPrototype.h>
#include <LibWeb/Bindings/NavigationCurrentEntryChangeEventConstructor.h>
#include <LibWeb/Bindings/NavigationCurrentEntryChangeEventPrototype.h>
#include <LibWeb/Bindings/NavigationDestinationConstructor.h>
#include <LibWeb/Bindings/NavigationDestinationPrototype.h>
#include <LibWeb/Bindings/NavigationHistoryEntryConstructor.h>
#include <LibWeb/Bindings/NavigationHistoryEntryPrototype.h>
#include <LibWeb/Bindings/NavigationTransitionConstructor.h>
#include <LibWeb/Bindings/NavigationTransitionPrototype.h>
#include <LibWeb/Bindings/NavigatorConstructor.h>
#include <LibWeb/Bindings/NavigatorPrototype.h>
#include <LibWeb/Bindings/OffscreenCanvasConstructor.h>
#include <LibWeb/Bindings/OffscreenCanvasPrototype.h>
#include <LibWeb/Bindings/OffscreenCanvasRenderingContext2DConstructor.h>
#include <LibWeb/Bindings/OffscreenCanvasRenderingContext2DPrototype.h>
#include <LibWeb/Bindings/PageTransitionEventConstructor.h>
#include <LibWeb/Bindings/PageTransitionEventPrototype.h>
#include <LibWeb/Bindings/Path2DConstructor.h>
#include <LibWeb/Bindings/Path2DPrototype.h>
#include <LibWeb/Bindings/PluginConstructor.h>
#include <LibWeb/Bindings/PluginPrototype.h>
#include <LibWeb/Bindings/PluginArrayConstructor.h>
#include <LibWeb/Bindings/PluginArrayPrototype.h>
#include <LibWeb/Bindings/PopStateEventConstructor.h>
#include <LibWeb/Bindings/PopStateEventPrototype.h>
#include <LibWeb/Bindings/PromiseRejectionEventConstructor.h>
#include <LibWeb/Bindings/PromiseRejectionEventPrototype.h>
#include <LibWeb/Bindings/RadioNodeListConstructor.h>
#include <LibWeb/Bindings/RadioNodeListPrototype.h>
#include <LibWeb/Bindings/ShadowRealmGlobalScopeConstructor.h>
#include <LibWeb/Bindings/ShadowRealmGlobalScopePrototype.h>
#include <LibWeb/Bindings/SharedWorkerConstructor.h>
#include <LibWeb/Bindings/SharedWorkerPrototype.h>
#include <LibWeb/Bindings/SharedWorkerGlobalScopeConstructor.h>
#include <LibWeb/Bindings/SharedWorkerGlobalScopePrototype.h>
#include <LibWeb/Bindings/StorageConstructor.h>
#include <LibWeb/Bindings/StoragePrototype.h>
#include <LibWeb/Bindings/StorageEventConstructor.h>
#include <LibWeb/Bindings/StorageEventPrototype.h>
#include <LibWeb/Bindings/SubmitEventConstructor.h>
#include <LibWeb/Bindings/SubmitEventPrototype.h>
#include <LibWeb/Bindings/TextMetricsConstructor.h>
#include <LibWeb/Bindings/TextMetricsPrototype.h>
#include <LibWeb/Bindings/TextTrackConstructor.h>
#include <LibWeb/Bindings/TextTrackPrototype.h>
#include <LibWeb/Bindings/TextTrackCueConstructor.h>
#include <LibWeb/Bindings/TextTrackCuePrototype.h>
#include <LibWeb/Bindings/TextTrackCueListConstructor.h>
#include <LibWeb/Bindings/TextTrackCueListPrototype.h>
#include <LibWeb/Bindings/TextTrackListConstructor.h>
#include <LibWeb/Bindings/TextTrackListPrototype.h>
#include <LibWeb/Bindings/TimeRangesConstructor.h>
#include <LibWeb/Bindings/TimeRangesPrototype.h>
#include <LibWeb/Bindings/ToggleEventConstructor.h>
#include <LibWeb/Bindings/ToggleEventPrototype.h>
#include <LibWeb/Bindings/TrackEventConstructor.h>
#include <LibWeb/Bindings/TrackEventPrototype.h>
#include <LibWeb/Bindings/UserActivationConstructor.h>
#include <LibWeb/Bindings/UserActivationPrototype.h>
#include <LibWeb/Bindings/ValidityStateConstructor.h>
#include <LibWeb/Bindings/ValidityStatePrototype.h>
#include <LibWeb/Bindings/VideoTrackConstructor.h>
#include <LibWeb/Bindings/VideoTrackPrototype.h>
#include <LibWeb/Bindings/VideoTrackListConstructor.h>
#include <LibWeb/Bindings/VideoTrackListPrototype.h>
#include <LibWeb/Bindings/WindowConstructor.h>
#include <LibWeb/Bindings/WindowPrototype.h>
#include <LibWeb/Bindings/WorkerConstructor.h>
#include <LibWeb/Bindings/WorkerPrototype.h>
#include <LibWeb/Bindings/WorkerGlobalScopeConstructor.h>
#include <LibWeb/Bindings/WorkerGlobalScopePrototype.h>
#include <LibWeb/Bindings/WorkerLocationConstructor.h>
#include <LibWeb/Bindings/WorkerLocationPrototype.h>
#include <LibWeb/Bindings/WorkerNavigatorConstructor.h>
#include <LibWeb/Bindings/WorkerNavigatorPrototype.h>
#include <LibWeb/Bindings/WorkletGlobalScopeConstructor.h>
#include <LibWeb/Bindings/WorkletGlobalScopePrototype.h>
#include <LibWeb/Bindings/XMLSerializerConstructor.h>
#include <LibWeb/Bindings/XMLSerializerPrototype.h>
#include <LibWeb/Bindings/PerformanceConstructor.h>
#include <LibWeb/Bindings/PerformancePrototype.h>
#include <LibWeb/Bindings/IDBCursorConstructor.h>
#include <LibWeb/Bindings/IDBCursorPrototype.h>
#include <LibWeb/Bindings/IDBCursorWithValueConstructor.h>
#include <LibWeb/Bindings/IDBCursorWithValuePrototype.h>
#include <LibWeb/Bindings/IDBDatabaseConstructor.h>
#include <LibWeb/Bindings/IDBDatabasePrototype.h>
#include <LibWeb/Bindings/IDBFactoryConstructor.h>
#include <LibWeb/Bindings/IDBFactoryPrototype.h>
#include <LibWeb/Bindings/IDBIndexConstructor.h>
#include <LibWeb/Bindings/IDBIndexPrototype.h>
#include <LibWeb/Bindings/IDBKeyRangeConstructor.h>
#include <LibWeb/Bindings/IDBKeyRangePrototype.h>
#include <LibWeb/Bindings/IDBObjectStoreConstructor.h>
#include <LibWeb/Bindings/IDBObjectStorePrototype.h>
#include <LibWeb/Bindings/IDBOpenDBRequestConstructor.h>
#include <LibWeb/Bindings/IDBOpenDBRequestPrototype.h>
#include <LibWeb/Bindings/IDBRecordConstructor.h>
#include <LibWeb/Bindings/IDBRecordPrototype.h>
#include <LibWeb/Bindings/IDBRequestConstructor.h>
#include <LibWeb/Bindings/IDBRequestPrototype.h>
#include <LibWeb/Bindings/IDBTransactionConstructor.h>
#include <LibWeb/Bindings/IDBTransactionPrototype.h>
#include <LibWeb/Bindings/IDBVersionChangeEventConstructor.h>
#include <LibWeb/Bindings/IDBVersionChangeEventPrototype.h>
#include <LibWeb/Bindings/FakeXRDeviceConstructor.h>
#include <LibWeb/Bindings/FakeXRDevicePrototype.h>
#include <LibWeb/Bindings/InternalAnimationTimelineConstructor.h>
#include <LibWeb/Bindings/InternalAnimationTimelinePrototype.h>
#include <LibWeb/Bindings/InternalGamepadConstructor.h>
#include <LibWeb/Bindings/InternalGamepadPrototype.h>
#include <LibWeb/Bindings/InternalsConstructor.h>
#include <LibWeb/Bindings/InternalsPrototype.h>
#include <LibWeb/Bindings/WebUIConstructor.h>
#include <LibWeb/Bindings/WebUIPrototype.h>
#include <LibWeb/Bindings/XRTestConstructor.h>
#include <LibWeb/Bindings/XRTestPrototype.h>
#include <LibWeb/Bindings/IntersectionObserverConstructor.h>
#include <LibWeb/Bindings/IntersectionObserverPrototype.h>
#include <LibWeb/Bindings/IntersectionObserverEntryConstructor.h>
#include <LibWeb/Bindings/IntersectionObserverEntryPrototype.h>
#include <LibWeb/Bindings/MathMLElementConstructor.h>
#include <LibWeb/Bindings/MathMLElementPrototype.h>
#include <LibWeb/Bindings/MediaCapabilitiesConstructor.h>
#include <LibWeb/Bindings/MediaCapabilitiesPrototype.h>
#include <LibWeb/Bindings/BufferedChangeEventConstructor.h>
#include <LibWeb/Bindings/BufferedChangeEventPrototype.h>
#include <LibWeb/Bindings/ManagedMediaSourceConstructor.h>
#include <LibWeb/Bindings/ManagedMediaSourcePrototype.h>
#include <LibWeb/Bindings/ManagedSourceBufferConstructor.h>
#include <LibWeb/Bindings/ManagedSourceBufferPrototype.h>
#include <LibWeb/Bindings/MediaSourceConstructor.h>
#include <LibWeb/Bindings/MediaSourcePrototype.h>
#include <LibWeb/Bindings/MediaSourceHandleConstructor.h>
#include <LibWeb/Bindings/MediaSourceHandlePrototype.h>
#include <LibWeb/Bindings/SourceBufferConstructor.h>
#include <LibWeb/Bindings/SourceBufferPrototype.h>
#include <LibWeb/Bindings/SourceBufferListConstructor.h>
#include <LibWeb/Bindings/SourceBufferListPrototype.h>
#include <LibWeb/Bindings/PerformanceNavigationConstructor.h>
#include <LibWeb/Bindings/PerformanceNavigationPrototype.h>
#include <LibWeb/Bindings/PerformanceTimingConstructor.h>
#include <LibWeb/Bindings/PerformanceTimingPrototype.h>
#include <LibWeb/Bindings/NotificationConstructor.h>
#include <LibWeb/Bindings/NotificationPrototype.h>
#include <LibWeb/Bindings/PerformanceEntryConstructor.h>
#include <LibWeb/Bindings/PerformanceEntryPrototype.h>
#include <LibWeb/Bindings/PerformanceObserverConstructor.h>
#include <LibWeb/Bindings/PerformanceObserverPrototype.h>
#include <LibWeb/Bindings/PerformanceObserverEntryListConstructor.h>
#include <LibWeb/Bindings/PerformanceObserverEntryListPrototype.h>
#include <LibWeb/Bindings/IdleDeadlineConstructor.h>
#include <LibWeb/Bindings/IdleDeadlinePrototype.h>
#include <LibWeb/Bindings/ResizeObserverConstructor.h>
#include <LibWeb/Bindings/ResizeObserverPrototype.h>
#include <LibWeb/Bindings/ResizeObserverEntryConstructor.h>
#include <LibWeb/Bindings/ResizeObserverEntryPrototype.h>
#include <LibWeb/Bindings/ResizeObserverSizeConstructor.h>
#include <LibWeb/Bindings/ResizeObserverSizePrototype.h>
#include <LibWeb/Bindings/PerformanceResourceTimingConstructor.h>
#include <LibWeb/Bindings/PerformanceResourceTimingPrototype.h>
#include <LibWeb/Bindings/SerialConstructor.h>
#include <LibWeb/Bindings/SerialPrototype.h>
#include <LibWeb/Bindings/SerialPortConstructor.h>
#include <LibWeb/Bindings/SerialPortPrototype.h>
#include <LibWeb/Bindings/CacheStorageConstructor.h>
#include <LibWeb/Bindings/CacheStoragePrototype.h>
#include <LibWeb/Bindings/ServiceWorkerConstructor.h>
#include <LibWeb/Bindings/ServiceWorkerPrototype.h>
#include <LibWeb/Bindings/ServiceWorkerContainerConstructor.h>
#include <LibWeb/Bindings/ServiceWorkerContainerPrototype.h>
#include <LibWeb/Bindings/ServiceWorkerGlobalScopeConstructor.h>
#include <LibWeb/Bindings/ServiceWorkerGlobalScopePrototype.h>
#include <LibWeb/Bindings/ServiceWorkerRegistrationConstructor.h>
#include <LibWeb/Bindings/ServiceWorkerRegistrationPrototype.h>
#include <LibWeb/Bindings/SpeechGrammarConstructor.h>
#include <LibWeb/Bindings/SpeechGrammarPrototype.h>
#include <LibWeb/Bindings/SpeechGrammarListConstructor.h>
#include <LibWeb/Bindings/SpeechGrammarListPrototype.h>
#include <LibWeb/Bindings/SpeechRecognitionConstructor.h>
#include <LibWeb/Bindings/SpeechRecognitionPrototype.h>
#include <LibWeb/Bindings/SpeechRecognitionAlternativeConstructor.h>
#include <LibWeb/Bindings/SpeechRecognitionAlternativePrototype.h>
#include <LibWeb/Bindings/SpeechRecognitionEventConstructor.h>
#include <LibWeb/Bindings/SpeechRecognitionEventPrototype.h>
#include <LibWeb/Bindings/SpeechRecognitionPhraseConstructor.h>
#include <LibWeb/Bindings/SpeechRecognitionPhrasePrototype.h>
#include <LibWeb/Bindings/SpeechRecognitionResultConstructor.h>
#include <LibWeb/Bindings/SpeechRecognitionResultPrototype.h>
#include <LibWeb/Bindings/SpeechRecognitionResultListConstructor.h>
#include <LibWeb/Bindings/SpeechRecognitionResultListPrototype.h>
#include <LibWeb/Bindings/SpeechSynthesisConstructor.h>
#include <LibWeb/Bindings/SpeechSynthesisPrototype.h>
#include <LibWeb/Bindings/SpeechSynthesisUtteranceConstructor.h>
#include <LibWeb/Bindings/SpeechSynthesisUtterancePrototype.h>
#include <LibWeb/Bindings/SpeechSynthesisVoiceConstructor.h>
#include <LibWeb/Bindings/SpeechSynthesisVoicePrototype.h>
#include <LibWeb/Bindings/ByteLengthQueuingStrategyConstructor.h>
#include <LibWeb/Bindings/ByteLengthQueuingStrategyPrototype.h>
#include <LibWeb/Bindings/CountQueuingStrategyConstructor.h>
#include <LibWeb/Bindings/CountQueuingStrategyPrototype.h>
#include <LibWeb/Bindings/ReadableByteStreamControllerConstructor.h>
#include <LibWeb/Bindings/ReadableByteStreamControllerPrototype.h>
#include <LibWeb/Bindings/ReadableStreamConstructor.h>
#include <LibWeb/Bindings/ReadableStreamPrototype.h>
#include <LibWeb/Bindings/ReadableStreamBYOBReaderConstructor.h>
#include <LibWeb/Bindings/ReadableStreamBYOBReaderPrototype.h>
#include <LibWeb/Bindings/ReadableStreamBYOBRequestConstructor.h>
#include <LibWeb/Bindings/ReadableStreamBYOBRequestPrototype.h>
#include <LibWeb/Bindings/ReadableStreamDefaultControllerConstructor.h>
#include <LibWeb/Bindings/ReadableStreamDefaultControllerPrototype.h>
#include <LibWeb/Bindings/ReadableStreamDefaultReaderConstructor.h>
#include <LibWeb/Bindings/ReadableStreamDefaultReaderPrototype.h>
#include <LibWeb/Bindings/TransformStreamConstructor.h>
#include <LibWeb/Bindings/TransformStreamPrototype.h>
#include <LibWeb/Bindings/TransformStreamDefaultControllerConstructor.h>
#include <LibWeb/Bindings/TransformStreamDefaultControllerPrototype.h>
#include <LibWeb/Bindings/WritableStreamConstructor.h>
#include <LibWeb/Bindings/WritableStreamPrototype.h>
#include <LibWeb/Bindings/WritableStreamDefaultControllerConstructor.h>
#include <LibWeb/Bindings/WritableStreamDefaultControllerPrototype.h>
#include <LibWeb/Bindings/WritableStreamDefaultWriterConstructor.h>
#include <LibWeb/Bindings/WritableStreamDefaultWriterPrototype.h>
#include <LibWeb/Bindings/TrustedHTMLConstructor.h>
#include <LibWeb/Bindings/TrustedHTMLPrototype.h>
#include <LibWeb/Bindings/TrustedScriptConstructor.h>
#include <LibWeb/Bindings/TrustedScriptPrototype.h>
#include <LibWeb/Bindings/TrustedScriptURLConstructor.h>
#include <LibWeb/Bindings/TrustedScriptURLPrototype.h>
#include <LibWeb/Bindings/TrustedTypePolicyConstructor.h>
#include <LibWeb/Bindings/TrustedTypePolicyPrototype.h>
#include <LibWeb/Bindings/TrustedTypePolicyFactoryConstructor.h>
#include <LibWeb/Bindings/TrustedTypePolicyFactoryPrototype.h>
#include <LibWeb/Bindings/SVGAElementConstructor.h>
#include <LibWeb/Bindings/SVGAElementPrototype.h>
#include <LibWeb/Bindings/SVGAnimatedEnumerationConstructor.h>
#include <LibWeb/Bindings/SVGAnimatedEnumerationPrototype.h>
#include <LibWeb/Bindings/SVGAnimatedIntegerConstructor.h>
#include <LibWeb/Bindings/SVGAnimatedIntegerPrototype.h>
#include <LibWeb/Bindings/SVGAnimatedLengthConstructor.h>
#include <LibWeb/Bindings/SVGAnimatedLengthPrototype.h>
#include <LibWeb/Bindings/SVGAnimatedLengthListConstructor.h>
#include <LibWeb/Bindings/SVGAnimatedLengthListPrototype.h>
#include <LibWeb/Bindings/SVGAnimatedNumberConstructor.h>
#include <LibWeb/Bindings/SVGAnimatedNumberPrototype.h>
#include <LibWeb/Bindings/SVGAnimatedNumberListConstructor.h>
#include <LibWeb/Bindings/SVGAnimatedNumberListPrototype.h>
#include <LibWeb/Bindings/SVGAnimatedRectConstructor.h>
#include <LibWeb/Bindings/SVGAnimatedRectPrototype.h>
#include <LibWeb/Bindings/SVGAnimatedStringConstructor.h>
#include <LibWeb/Bindings/SVGAnimatedStringPrototype.h>
#include <LibWeb/Bindings/SVGAnimatedTransformListConstructor.h>
#include <LibWeb/Bindings/SVGAnimatedTransformListPrototype.h>
#include <LibWeb/Bindings/SVGAnimationElementConstructor.h>
#include <LibWeb/Bindings/SVGAnimationElementPrototype.h>
#include <LibWeb/Bindings/SVGClipPathElementConstructor.h>
#include <LibWeb/Bindings/SVGClipPathElementPrototype.h>
#include <LibWeb/Bindings/SVGComponentTransferFunctionElementConstructor.h>
#include <LibWeb/Bindings/SVGComponentTransferFunctionElementPrototype.h>
#include <LibWeb/Bindings/SVGDefsElementConstructor.h>
#include <LibWeb/Bindings/SVGDefsElementPrototype.h>
#include <LibWeb/Bindings/SVGDescElementConstructor.h>
#include <LibWeb/Bindings/SVGDescElementPrototype.h>
#include <LibWeb/Bindings/SVGElementConstructor.h>
#include <LibWeb/Bindings/SVGElementPrototype.h>
#include <LibWeb/Bindings/SVGGElementConstructor.h>
#include <LibWeb/Bindings/SVGGElementPrototype.h>
#include <LibWeb/Bindings/SVGGeometryElementConstructor.h>
#include <LibWeb/Bindings/SVGGeometryElementPrototype.h>
#include <LibWeb/Bindings/SVGGradientElementConstructor.h>
#include <LibWeb/Bindings/SVGGradientElementPrototype.h>
#include <LibWeb/Bindings/SVGGraphicsElementConstructor.h>
#include <LibWeb/Bindings/SVGGraphicsElementPrototype.h>
#include <LibWeb/Bindings/SVGImageElementConstructor.h>
#include <LibWeb/Bindings/SVGImageElementPrototype.h>
#include <LibWeb/Bindings/SVGCircleElementConstructor.h>
#include <LibWeb/Bindings/SVGCircleElementPrototype.h>
#include <LibWeb/Bindings/SVGEllipseElementConstructor.h>
#include <LibWeb/Bindings/SVGEllipseElementPrototype.h>
#include <LibWeb/Bindings/SVGFEBlendElementConstructor.h>
#include <LibWeb/Bindings/SVGFEBlendElementPrototype.h>
#include <LibWeb/Bindings/SVGFEColorMatrixElementConstructor.h>
#include <LibWeb/Bindings/SVGFEColorMatrixElementPrototype.h>
#include <LibWeb/Bindings/SVGFEComponentTransferElementConstructor.h>
#include <LibWeb/Bindings/SVGFEComponentTransferElementPrototype.h>
#include <LibWeb/Bindings/SVGFECompositeElementConstructor.h>
#include <LibWeb/Bindings/SVGFECompositeElementPrototype.h>
#include <LibWeb/Bindings/SVGFEDisplacementMapElementConstructor.h>
#include <LibWeb/Bindings/SVGFEDisplacementMapElementPrototype.h>
#include <LibWeb/Bindings/SVGFEDropShadowElementConstructor.h>
#include <LibWeb/Bindings/SVGFEDropShadowElementPrototype.h>
#include <LibWeb/Bindings/SVGFEFloodElementConstructor.h>
#include <LibWeb/Bindings/SVGFEFloodElementPrototype.h>
#include <LibWeb/Bindings/SVGFEFuncAElementConstructor.h>
#include <LibWeb/Bindings/SVGFEFuncAElementPrototype.h>
#include <LibWeb/Bindings/SVGFEFuncBElementConstructor.h>
#include <LibWeb/Bindings/SVGFEFuncBElementPrototype.h>
#include <LibWeb/Bindings/SVGFEFuncGElementConstructor.h>
#include <LibWeb/Bindings/SVGFEFuncGElementPrototype.h>
#include <LibWeb/Bindings/SVGFEFuncRElementConstructor.h>
#include <LibWeb/Bindings/SVGFEFuncRElementPrototype.h>
#include <LibWeb/Bindings/SVGFEGaussianBlurElementConstructor.h>
#include <LibWeb/Bindings/SVGFEGaussianBlurElementPrototype.h>
#include <LibWeb/Bindings/SVGFEImageElementConstructor.h>
#include <LibWeb/Bindings/SVGFEImageElementPrototype.h>
#include <LibWeb/Bindings/SVGFEMergeElementConstructor.h>
#include <LibWeb/Bindings/SVGFEMergeElementPrototype.h>
#include <LibWeb/Bindings/SVGFEMergeNodeElementConstructor.h>
#include <LibWeb/Bindings/SVGFEMergeNodeElementPrototype.h>
#include <LibWeb/Bindings/SVGFEMorphologyElementConstructor.h>
#include <LibWeb/Bindings/SVGFEMorphologyElementPrototype.h>
#include <LibWeb/Bindings/SVGFEOffsetElementConstructor.h>
#include <LibWeb/Bindings/SVGFEOffsetElementPrototype.h>
#include <LibWeb/Bindings/SVGFETurbulenceElementConstructor.h>
#include <LibWeb/Bindings/SVGFETurbulenceElementPrototype.h>
#include <LibWeb/Bindings/SVGFilterElementConstructor.h>
#include <LibWeb/Bindings/SVGFilterElementPrototype.h>
#include <LibWeb/Bindings/SVGForeignObjectElementConstructor.h>
#include <LibWeb/Bindings/SVGForeignObjectElementPrototype.h>
#include <LibWeb/Bindings/SVGLengthConstructor.h>
#include <LibWeb/Bindings/SVGLengthPrototype.h>
#include <LibWeb/Bindings/SVGLengthListConstructor.h>
#include <LibWeb/Bindings/SVGLengthListPrototype.h>
#include <LibWeb/Bindings/SVGLineElementConstructor.h>
#include <LibWeb/Bindings/SVGLineElementPrototype.h>
#include <LibWeb/Bindings/SVGLinearGradientElementConstructor.h>
#include <LibWeb/Bindings/SVGLinearGradientElementPrototype.h>
#include <LibWeb/Bindings/SVGMaskElementConstructor.h>
#include <LibWeb/Bindings/SVGMaskElementPrototype.h>
#include <LibWeb/Bindings/SVGMetadataElementConstructor.h>
#include <LibWeb/Bindings/SVGMetadataElementPrototype.h>
#include <LibWeb/Bindings/SVGNumberConstructor.h>
#include <LibWeb/Bindings/SVGNumberPrototype.h>
#include <LibWeb/Bindings/SVGNumberListConstructor.h>
#include <LibWeb/Bindings/SVGNumberListPrototype.h>
#include <LibWeb/Bindings/SVGPathElementConstructor.h>
#include <LibWeb/Bindings/SVGPathElementPrototype.h>
#include <LibWeb/Bindings/SVGPatternElementConstructor.h>
#include <LibWeb/Bindings/SVGPatternElementPrototype.h>
#include <LibWeb/Bindings/SVGPolygonElementConstructor.h>
#include <LibWeb/Bindings/SVGPolygonElementPrototype.h>
#include <LibWeb/Bindings/SVGPolylineElementConstructor.h>
#include <LibWeb/Bindings/SVGPolylineElementPrototype.h>
#include <LibWeb/Bindings/SVGRadialGradientElementConstructor.h>
#include <LibWeb/Bindings/SVGRadialGradientElementPrototype.h>
#include <LibWeb/Bindings/SVGRectElementConstructor.h>
#include <LibWeb/Bindings/SVGRectElementPrototype.h>
#include <LibWeb/Bindings/SVGScriptElementConstructor.h>
#include <LibWeb/Bindings/SVGScriptElementPrototype.h>
#include <LibWeb/Bindings/SVGSVGElementConstructor.h>
#include <LibWeb/Bindings/SVGSVGElementPrototype.h>
#include <LibWeb/Bindings/SVGStopElementConstructor.h>
#include <LibWeb/Bindings/SVGStopElementPrototype.h>
#include <LibWeb/Bindings/SVGStyleElementConstructor.h>
#include <LibWeb/Bindings/SVGStyleElementPrototype.h>
#include <LibWeb/Bindings/SVGSymbolElementConstructor.h>
#include <LibWeb/Bindings/SVGSymbolElementPrototype.h>
#include <LibWeb/Bindings/SVGTextContentElementConstructor.h>
#include <LibWeb/Bindings/SVGTextContentElementPrototype.h>
#include <LibWeb/Bindings/SVGTextElementConstructor.h>
#include <LibWeb/Bindings/SVGTextElementPrototype.h>
#include <LibWeb/Bindings/SVGTextPathElementConstructor.h>
#include <LibWeb/Bindings/SVGTextPathElementPrototype.h>
#include <LibWeb/Bindings/SVGTextPositioningElementConstructor.h>
#include <LibWeb/Bindings/SVGTextPositioningElementPrototype.h>
#include <LibWeb/Bindings/SVGTitleElementConstructor.h>
#include <LibWeb/Bindings/SVGTitleElementPrototype.h>
#include <LibWeb/Bindings/SVGTransformConstructor.h>
#include <LibWeb/Bindings/SVGTransformPrototype.h>
#include <LibWeb/Bindings/SVGTransformListConstructor.h>
#include <LibWeb/Bindings/SVGTransformListPrototype.h>
#include <LibWeb/Bindings/SVGTSpanElementConstructor.h>
#include <LibWeb/Bindings/SVGTSpanElementPrototype.h>
#include <LibWeb/Bindings/SVGUnitTypesConstructor.h>
#include <LibWeb/Bindings/SVGUnitTypesPrototype.h>
#include <LibWeb/Bindings/SVGUseElementConstructor.h>
#include <LibWeb/Bindings/SVGUseElementPrototype.h>
#include <LibWeb/Bindings/SVGViewElementConstructor.h>
#include <LibWeb/Bindings/SVGViewElementPrototype.h>
#include <LibWeb/Bindings/SelectionConstructor.h>
#include <LibWeb/Bindings/SelectionPrototype.h>
#include <LibWeb/Bindings/StorageManagerConstructor.h>
#include <LibWeb/Bindings/StorageManagerPrototype.h>
#include <LibWeb/Bindings/CompositionEventConstructor.h>
#include <LibWeb/Bindings/CompositionEventPrototype.h>
#include <LibWeb/Bindings/FocusEventConstructor.h>
#include <LibWeb/Bindings/FocusEventPrototype.h>
#include <LibWeb/Bindings/InputEventConstructor.h>
#include <LibWeb/Bindings/InputEventPrototype.h>
#include <LibWeb/Bindings/KeyboardEventConstructor.h>
#include <LibWeb/Bindings/KeyboardEventPrototype.h>
#include <LibWeb/Bindings/MouseEventConstructor.h>
#include <LibWeb/Bindings/MouseEventPrototype.h>
#include <LibWeb/Bindings/PointerEventConstructor.h>
#include <LibWeb/Bindings/PointerEventPrototype.h>
#include <LibWeb/Bindings/TextEventConstructor.h>
#include <LibWeb/Bindings/TextEventPrototype.h>
#include <LibWeb/Bindings/UIEventConstructor.h>
#include <LibWeb/Bindings/UIEventPrototype.h>
#include <LibWeb/Bindings/WheelEventConstructor.h>
#include <LibWeb/Bindings/WheelEventPrototype.h>
#include <LibWeb/Bindings/URLPatternConstructor.h>
#include <LibWeb/Bindings/URLPatternPrototype.h>
#include <LibWeb/Bindings/PerformanceMarkConstructor.h>
#include <LibWeb/Bindings/PerformanceMarkPrototype.h>
#include <LibWeb/Bindings/PerformanceMeasureConstructor.h>
#include <LibWeb/Bindings/PerformanceMeasurePrototype.h>
#include <LibWeb/Bindings/ViewTransitionConstructor.h>
#include <LibWeb/Bindings/ViewTransitionPrototype.h>
#include <LibWeb/Bindings/GlobalConstructor.h>
#include <LibWeb/Bindings/GlobalPrototype.h>
#include <LibWeb/Bindings/InstanceConstructor.h>
#include <LibWeb/Bindings/InstancePrototype.h>
#include <LibWeb/Bindings/MemoryConstructor.h>
#include <LibWeb/Bindings/MemoryPrototype.h>
#include <LibWeb/Bindings/ModuleConstructor.h>
#include <LibWeb/Bindings/ModulePrototype.h>
#include <LibWeb/Bindings/TableConstructor.h>
#include <LibWeb/Bindings/TablePrototype.h>
#include <LibWeb/Bindings/AnalyserNodeConstructor.h>
#include <LibWeb/Bindings/AnalyserNodePrototype.h>
#include <LibWeb/Bindings/AudioBufferConstructor.h>
#include <LibWeb/Bindings/AudioBufferPrototype.h>
#include <LibWeb/Bindings/AudioBufferSourceNodeConstructor.h>
#include <LibWeb/Bindings/AudioBufferSourceNodePrototype.h>
#include <LibWeb/Bindings/AudioContextConstructor.h>
#include <LibWeb/Bindings/AudioContextPrototype.h>
#include <LibWeb/Bindings/AudioDestinationNodeConstructor.h>
#include <LibWeb/Bindings/AudioDestinationNodePrototype.h>
#include <LibWeb/Bindings/AudioListenerConstructor.h>
#include <LibWeb/Bindings/AudioListenerPrototype.h>
#include <LibWeb/Bindings/AudioNodeConstructor.h>
#include <LibWeb/Bindings/AudioNodePrototype.h>
#include <LibWeb/Bindings/AudioParamConstructor.h>
#include <LibWeb/Bindings/AudioParamPrototype.h>
#include <LibWeb/Bindings/AudioScheduledSourceNodeConstructor.h>
#include <LibWeb/Bindings/AudioScheduledSourceNodePrototype.h>
#include <LibWeb/Bindings/BaseAudioContextConstructor.h>
#include <LibWeb/Bindings/BaseAudioContextPrototype.h>
#include <LibWeb/Bindings/BiquadFilterNodeConstructor.h>
#include <LibWeb/Bindings/BiquadFilterNodePrototype.h>
#include <LibWeb/Bindings/DynamicsCompressorNodeConstructor.h>
#include <LibWeb/Bindings/DynamicsCompressorNodePrototype.h>
#include <LibWeb/Bindings/ChannelMergerNodeConstructor.h>
#include <LibWeb/Bindings/ChannelMergerNodePrototype.h>
#include <LibWeb/Bindings/ChannelSplitterNodeConstructor.h>
#include <LibWeb/Bindings/ChannelSplitterNodePrototype.h>
#include <LibWeb/Bindings/ConstantSourceNodeConstructor.h>
#include <LibWeb/Bindings/ConstantSourceNodePrototype.h>
#include <LibWeb/Bindings/DelayNodeConstructor.h>
#include <LibWeb/Bindings/DelayNodePrototype.h>
#include <LibWeb/Bindings/GainNodeConstructor.h>
#include <LibWeb/Bindings/GainNodePrototype.h>
#include <LibWeb/Bindings/MediaElementAudioSourceNodeConstructor.h>
#include <LibWeb/Bindings/MediaElementAudioSourceNodePrototype.h>
#include <LibWeb/Bindings/OfflineAudioCompletionEventConstructor.h>
#include <LibWeb/Bindings/OfflineAudioCompletionEventPrototype.h>
#include <LibWeb/Bindings/OfflineAudioContextConstructor.h>
#include <LibWeb/Bindings/OfflineAudioContextPrototype.h>
#include <LibWeb/Bindings/OscillatorNodeConstructor.h>
#include <LibWeb/Bindings/OscillatorNodePrototype.h>
#include <LibWeb/Bindings/PannerNodeConstructor.h>
#include <LibWeb/Bindings/PannerNodePrototype.h>
#include <LibWeb/Bindings/PeriodicWaveConstructor.h>
#include <LibWeb/Bindings/PeriodicWavePrototype.h>
#include <LibWeb/Bindings/ScriptProcessorNodeConstructor.h>
#include <LibWeb/Bindings/ScriptProcessorNodePrototype.h>
#include <LibWeb/Bindings/StereoPannerNodeConstructor.h>
#include <LibWeb/Bindings/StereoPannerNodePrototype.h>
#include <LibWeb/Bindings/ANGLEInstancedArraysConstructor.h>
#include <LibWeb/Bindings/ANGLEInstancedArraysPrototype.h>
#include <LibWeb/Bindings/EXTBlendMinMaxConstructor.h>
#include <LibWeb/Bindings/EXTBlendMinMaxPrototype.h>
#include <LibWeb/Bindings/EXTColorBufferFloatConstructor.h>
#include <LibWeb/Bindings/EXTColorBufferFloatPrototype.h>
#include <LibWeb/Bindings/EXTRenderSnormConstructor.h>
#include <LibWeb/Bindings/EXTRenderSnormPrototype.h>
#include <LibWeb/Bindings/EXTTextureFilterAnisotropicConstructor.h>
#include <LibWeb/Bindings/EXTTextureFilterAnisotropicPrototype.h>
#include <LibWeb/Bindings/EXTTextureNorm16Constructor.h>
#include <LibWeb/Bindings/EXTTextureNorm16Prototype.h>
#include <LibWeb/Bindings/OESElementIndexUintConstructor.h>
#include <LibWeb/Bindings/OESElementIndexUintPrototype.h>
#include <LibWeb/Bindings/OESStandardDerivativesConstructor.h>
#include <LibWeb/Bindings/OESStandardDerivativesPrototype.h>
#include <LibWeb/Bindings/OESVertexArrayObjectConstructor.h>
#include <LibWeb/Bindings/OESVertexArrayObjectPrototype.h>
#include <LibWeb/Bindings/WebGLCompressedTextureS3tcConstructor.h>
#include <LibWeb/Bindings/WebGLCompressedTextureS3tcPrototype.h>
#include <LibWeb/Bindings/WebGLCompressedTextureS3tcSrgbConstructor.h>
#include <LibWeb/Bindings/WebGLCompressedTextureS3tcSrgbPrototype.h>
#include <LibWeb/Bindings/WebGLDrawBuffersConstructor.h>
#include <LibWeb/Bindings/WebGLDrawBuffersPrototype.h>
#include <LibWeb/Bindings/WebGLVertexArrayObjectOESConstructor.h>
#include <LibWeb/Bindings/WebGLVertexArrayObjectOESPrototype.h>
#include <LibWeb/Bindings/WebGL2RenderingContextConstructor.h>
#include <LibWeb/Bindings/WebGL2RenderingContextPrototype.h>
#include <LibWeb/Bindings/WebGLActiveInfoConstructor.h>
#include <LibWeb/Bindings/WebGLActiveInfoPrototype.h>
#include <LibWeb/Bindings/WebGLBufferConstructor.h>
#include <LibWeb/Bindings/WebGLBufferPrototype.h>
#include <LibWeb/Bindings/WebGLContextEventConstructor.h>
#include <LibWeb/Bindings/WebGLContextEventPrototype.h>
#include <LibWeb/Bindings/WebGLFramebufferConstructor.h>
#include <LibWeb/Bindings/WebGLFramebufferPrototype.h>
#include <LibWeb/Bindings/WebGLObjectConstructor.h>
#include <LibWeb/Bindings/WebGLObjectPrototype.h>
#include <LibWeb/Bindings/WebGLProgramConstructor.h>
#include <LibWeb/Bindings/WebGLProgramPrototype.h>
#include <LibWeb/Bindings/WebGLQueryConstructor.h>
#include <LibWeb/Bindings/WebGLQueryPrototype.h>
#include <LibWeb/Bindings/WebGLRenderbufferConstructor.h>
#include <LibWeb/Bindings/WebGLRenderbufferPrototype.h>
#include <LibWeb/Bindings/WebGLRenderingContextConstructor.h>
#include <LibWeb/Bindings/WebGLRenderingContextPrototype.h>
#include <LibWeb/Bindings/WebGLSamplerConstructor.h>
#include <LibWeb/Bindings/WebGLSamplerPrototype.h>
#include <LibWeb/Bindings/WebGLShaderConstructor.h>
#include <LibWeb/Bindings/WebGLShaderPrototype.h>
#include <LibWeb/Bindings/WebGLShaderPrecisionFormatConstructor.h>
#include <LibWeb/Bindings/WebGLShaderPrecisionFormatPrototype.h>
#include <LibWeb/Bindings/WebGLSyncConstructor.h>
#include <LibWeb/Bindings/WebGLSyncPrototype.h>
#include <LibWeb/Bindings/WebGLTextureConstructor.h>
#include <LibWeb/Bindings/WebGLTexturePrototype.h>
#include <LibWeb/Bindings/WebGLTransformFeedbackConstructor.h>
#include <LibWeb/Bindings/WebGLTransformFeedbackPrototype.h>
#include <LibWeb/Bindings/WebGLUniformLocationConstructor.h>
#include <LibWeb/Bindings/WebGLUniformLocationPrototype.h>
#include <LibWeb/Bindings/WebGLVertexArrayObjectConstructor.h>
#include <LibWeb/Bindings/WebGLVertexArrayObjectPrototype.h>
#include <LibWeb/Bindings/DOMExceptionConstructor.h>
#include <LibWeb/Bindings/DOMExceptionPrototype.h>
#include <LibWeb/Bindings/QuotaExceededErrorConstructor.h>
#include <LibWeb/Bindings/QuotaExceededErrorPrototype.h>
#include <LibWeb/Bindings/WebSocketConstructor.h>
#include <LibWeb/Bindings/WebSocketPrototype.h>
#include <LibWeb/Bindings/VTTCueConstructor.h>
#include <LibWeb/Bindings/VTTCuePrototype.h>
#include <LibWeb/Bindings/VTTRegionConstructor.h>
#include <LibWeb/Bindings/VTTRegionPrototype.h>
#include <LibWeb/Bindings/XRSessionConstructor.h>
#include <LibWeb/Bindings/XRSessionPrototype.h>
#include <LibWeb/Bindings/XRSessionEventConstructor.h>
#include <LibWeb/Bindings/XRSessionEventPrototype.h>
#include <LibWeb/Bindings/XRSystemConstructor.h>
#include <LibWeb/Bindings/XRSystemPrototype.h>
#include <LibWeb/Bindings/FormDataConstructor.h>
#include <LibWeb/Bindings/FormDataPrototype.h>
#include <LibWeb/Bindings/ProgressEventConstructor.h>
#include <LibWeb/Bindings/ProgressEventPrototype.h>
#include <LibWeb/Bindings/XMLHttpRequestConstructor.h>
#include <LibWeb/Bindings/XMLHttpRequestPrototype.h>
#include <LibWeb/Bindings/XMLHttpRequestEventTargetConstructor.h>
#include <LibWeb/Bindings/XMLHttpRequestEventTargetPrototype.h>
#include <LibWeb/Bindings/XMLHttpRequestUploadConstructor.h>
#include <LibWeb/Bindings/XMLHttpRequestUploadPrototype.h>
#include <LibWeb/Bindings/XPathResultConstructor.h>
#include <LibWeb/Bindings/XPathResultPrototype.h>
#include <LibWeb/Bindings/XPathExpressionConstructor.h>
#include <LibWeb/Bindings/XPathExpressionPrototype.h>
#include <LibWeb/Bindings/XPathEvaluatorConstructor.h>
#include <LibWeb/Bindings/XPathEvaluatorPrototype.h>

namespace Web::Bindings {

static constexpr bool is_secure_context_interface(InterfaceName name)
{
    switch (name) {
    case InterfaceName::Clipboard:
    case InterfaceName::ClipboardItem:
    case InterfaceName::CookieChangeEvent:
    case InterfaceName::CookieStore:
    case InterfaceName::Credential:
    case InterfaceName::CredentialsContainer:
    case InterfaceName::FederatedCredential:
    case InterfaceName::PasswordCredential:
    case InterfaceName::CryptoKey:
    case InterfaceName::SubtleCrypto:
    case InterfaceName::MediaKeySystemAccess:
    case InterfaceName::GeolocationCoordinates:
    case InterfaceName::GeolocationPosition:
    case InterfaceName::WorkletGlobalScope:
    case InterfaceName::Serial:
    case InterfaceName::SerialPort:
    case InterfaceName::CacheStorage:
    case InterfaceName::ServiceWorker:
    case InterfaceName::ServiceWorkerContainer:
    case InterfaceName::ServiceWorkerGlobalScope:
    case InterfaceName::ServiceWorkerRegistration:
    case InterfaceName::StorageManager:
    case InterfaceName::XRSession:
    case InterfaceName::XRSessionEvent:
    case InterfaceName::XRSystem:
        return true;
    default:
        return false;
    }
}

static constexpr bool is_experimental_interface(InterfaceName name)
{
    switch (name) {
    case InterfaceName::OffscreenCanvas:
    case InterfaceName::OffscreenCanvasRenderingContext2D:
    case InterfaceName::ManagedMediaSource:
    case InterfaceName::MediaSource:
    case InterfaceName::MediaSourceHandle:
    case InterfaceName::CacheStorage:
        return true;
    default:
        return false;
    }
}

static constexpr bool is_window_exposed(InterfaceName name)
{
    switch (name) {
    case InterfaceName::Animation:
    case InterfaceName::AnimationEffect:
    case InterfaceName::AnimationPlaybackEvent:
    case InterfaceName::AnimationTimeline:
    case InterfaceName::DocumentTimeline:
    case InterfaceName::KeyframeEffect:
    case InterfaceName::ScrollTimeline:
    case InterfaceName::Clipboard:
    case InterfaceName::ClipboardEvent:
    case InterfaceName::ClipboardItem:
    case InterfaceName::SecurityPolicyViolationEvent:
    case InterfaceName::CompressionStream:
    case InterfaceName::DecompressionStream:
    case InterfaceName::CookieChangeEvent:
    case InterfaceName::CookieStore:
    case InterfaceName::Credential:
    case InterfaceName::CredentialsContainer:
    case InterfaceName::FederatedCredential:
    case InterfaceName::PasswordCredential:
    case InterfaceName::Crypto:
    case InterfaceName::CryptoKey:
    case InterfaceName::SubtleCrypto:
    case InterfaceName::AnimationEvent:
    case InterfaceName::CSSAnimation:
    case InterfaceName::CSSConditionRule:
    case InterfaceName::CSSCounterStyleRule:
    case InterfaceName::CSSFontFaceDescriptors:
    case InterfaceName::CSSFontFaceRule:
    case InterfaceName::CSSFontFeatureValuesMap:
    case InterfaceName::CSSFontFeatureValuesRule:
    case InterfaceName::CSSGroupingRule:
    case InterfaceName::CSSImageValue:
    case InterfaceName::CSSImportRule:
    case InterfaceName::CSSKeyframeRule:
    case InterfaceName::CSSKeyframesRule:
    case InterfaceName::CSSKeywordValue:
    case InterfaceName::CSSLayerBlockRule:
    case InterfaceName::CSSLayerStatementRule:
    case InterfaceName::CSSMarginRule:
    case InterfaceName::CSSMathClamp:
    case InterfaceName::CSSMathInvert:
    case InterfaceName::CSSMathMax:
    case InterfaceName::CSSMathMin:
    case InterfaceName::CSSMathNegate:
    case InterfaceName::CSSMathProduct:
    case InterfaceName::CSSMathSum:
    case InterfaceName::CSSMathValue:
    case InterfaceName::CSSMatrixComponent:
    case InterfaceName::CSSMediaRule:
    case InterfaceName::CSSNamespaceRule:
    case InterfaceName::CSSNestedDeclarations:
    case InterfaceName::CSSNumericArray:
    case InterfaceName::CSSNumericValue:
    case InterfaceName::CSSPageRule:
    case InterfaceName::CSSPageDescriptors:
    case InterfaceName::CSSPerspective:
    case InterfaceName::CSSPropertyRule:
    case InterfaceName::CSSRotate:
    case InterfaceName::CSSRule:
    case InterfaceName::CSSRuleList:
    case InterfaceName::CSSScale:
    case InterfaceName::CSSSkew:
    case InterfaceName::CSSSkewX:
    case InterfaceName::CSSSkewY:
    case InterfaceName::CSSStyleDeclaration:
    case InterfaceName::CSSStyleProperties:
    case InterfaceName::CSSStyleRule:
    case InterfaceName::CSSStyleSheet:
    case InterfaceName::CSSStyleValue:
    case InterfaceName::CSSSupportsRule:
    case InterfaceName::CSSTransformComponent:
    case InterfaceName::CSSTransformValue:
    case InterfaceName::CSSTransition:
    case InterfaceName::CSSTranslate:
    case InterfaceName::CSSUnitValue:
    case InterfaceName::CSSUnparsedValue:
    case InterfaceName::CSSVariableReferenceValue:
    case InterfaceName::FontFace:
    case InterfaceName::FontFaceSet:
    case InterfaceName::FontFaceSetLoadEvent:
    case InterfaceName::MediaList:
    case InterfaceName::MediaQueryList:
    case InterfaceName::MediaQueryListEvent:
    case InterfaceName::Screen:
    case InterfaceName::ScreenOrientation:
    case InterfaceName::StylePropertyMap:
    case InterfaceName::StylePropertyMapReadOnly:
    case InterfaceName::StyleSheet:
    case InterfaceName::StyleSheetList:
    case InterfaceName::TransitionEvent:
    case InterfaceName::VisualViewport:
    case InterfaceName::AbstractRange:
    case InterfaceName::Attr:
    case InterfaceName::AbortController:
    case InterfaceName::AbortSignal:
    case InterfaceName::CDATASection:
    case InterfaceName::CharacterData:
    case InterfaceName::Comment:
    case InterfaceName::CustomEvent:
    case InterfaceName::Document:
    case InterfaceName::DocumentFragment:
    case InterfaceName::DocumentType:
    case InterfaceName::DOMImplementation:
    case InterfaceName::DOMTokenList:
    case InterfaceName::Element:
    case InterfaceName::Event:
    case InterfaceName::EventTarget:
    case InterfaceName::HTMLCollection:
    case InterfaceName::MutationObserver:
    case InterfaceName::MutationRecord:
    case InterfaceName::NamedNodeMap:
    case InterfaceName::Node:
    case InterfaceName::NodeFilter:
    case InterfaceName::NodeIterator:
    case InterfaceName::NodeList:
    case InterfaceName::ProcessingInstruction:
    case InterfaceName::Range:
    case InterfaceName::ShadowRoot:
    case InterfaceName::StaticRange:
    case InterfaceName::Text:
    case InterfaceName::TreeWalker:
    case InterfaceName::XMLDocument:
    case InterfaceName::URL:
    case InterfaceName::Origin:
    case InterfaceName::URLSearchParams:
    case InterfaceName::TextDecoder:
    case InterfaceName::TextEncoder:
    case InterfaceName::TextEncoderStream:
    case InterfaceName::MediaKeySystemAccess:
    case InterfaceName::FileSystemEntry:
    case InterfaceName::PerformanceEventTiming:
    case InterfaceName::Headers:
    case InterfaceName::Request:
    case InterfaceName::Response:
    case InterfaceName::Blob:
    case InterfaceName::File:
    case InterfaceName::FileList:
    case InterfaceName::FileReader:
    case InterfaceName::Gamepad:
    case InterfaceName::GamepadButton:
    case InterfaceName::GamepadEvent:
    case InterfaceName::GamepadHapticActuator:
    case InterfaceName::Geolocation:
    case InterfaceName::GeolocationCoordinates:
    case InterfaceName::GeolocationPosition:
    case InterfaceName::GeolocationPositionError:
    case InterfaceName::DOMMatrix:
    case InterfaceName::DOMMatrixReadOnly:
    case InterfaceName::DOMPoint:
    case InterfaceName::DOMPointReadOnly:
    case InterfaceName::DOMQuad:
    case InterfaceName::DOMRect:
    case InterfaceName::DOMRectList:
    case InterfaceName::DOMRectReadOnly:
    case InterfaceName::AudioTrack:
    case InterfaceName::AudioTrackList:
    case InterfaceName::BarProp:
    case InterfaceName::BeforeUnloadEvent:
    case InterfaceName::BroadcastChannel:
    case InterfaceName::CanvasGradient:
    case InterfaceName::CanvasPattern:
    case InterfaceName::CanvasRenderingContext2D:
    case InterfaceName::CloseEvent:
    case InterfaceName::CloseWatcher:
    case InterfaceName::CommandEvent:
    case InterfaceName::CustomElementRegistry:
    case InterfaceName::CustomStateSet:
    case InterfaceName::DataTransfer:
    case InterfaceName::DataTransferItem:
    case InterfaceName::DataTransferItemList:
    case InterfaceName::DOMParser:
    case InterfaceName::DOMStringList:
    case InterfaceName::DOMStringMap:
    case InterfaceName::DragEvent:
    case InterfaceName::ElementInternals:
    case InterfaceName::ErrorEvent:
    case InterfaceName::EventSource:
    case InterfaceName::External:
    case InterfaceName::FormDataEvent:
    case InterfaceName::HashChangeEvent:
    case InterfaceName::History:
    case InterfaceName::HTMLAllCollection:
    case InterfaceName::HTMLAnchorElement:
    case InterfaceName::HTMLAreaElement:
    case InterfaceName::HTMLAudioElement:
    case InterfaceName::HTMLBaseElement:
    case InterfaceName::HTMLBodyElement:
    case InterfaceName::HTMLBRElement:
    case InterfaceName::HTMLButtonElement:
    case InterfaceName::HTMLCanvasElement:
    case InterfaceName::HTMLDataElement:
    case InterfaceName::HTMLDataListElement:
    case InterfaceName::HTMLDetailsElement:
    case InterfaceName::HTMLDialogElement:
    case InterfaceName::HTMLDirectoryElement:
    case InterfaceName::HTMLDivElement:
    case InterfaceName::HTMLDocument:
    case InterfaceName::HTMLDListElement:
    case InterfaceName::HTMLElement:
    case InterfaceName::HTMLEmbedElement:
    case InterfaceName::HTMLFieldSetElement:
    case InterfaceName::HTMLFontElement:
    case InterfaceName::HTMLFormControlsCollection:
    case InterfaceName::HTMLFormElement:
    case InterfaceName::HTMLFrameElement:
    case InterfaceName::HTMLFrameSetElement:
    case InterfaceName::HTMLHeadElement:
    case InterfaceName::HTMLHeadingElement:
    case InterfaceName::HTMLHRElement:
    case InterfaceName::HTMLHtmlElement:
    case InterfaceName::HTMLIFrameElement:
    case InterfaceName::HTMLImageElement:
    case InterfaceName::HTMLInputElement:
    case InterfaceName::HTMLLabelElement:
    case InterfaceName::HTMLLegendElement:
    case InterfaceName::HTMLLIElement:
    case InterfaceName::HTMLLinkElement:
    case InterfaceName::HTMLMapElement:
    case InterfaceName::HTMLMarqueeElement:
    case InterfaceName::HTMLMediaElement:
    case InterfaceName::HTMLMenuElement:
    case InterfaceName::HTMLMetaElement:
    case InterfaceName::HTMLMeterElement:
    case InterfaceName::HTMLModElement:
    case InterfaceName::HTMLObjectElement:
    case InterfaceName::HTMLOListElement:
    case InterfaceName::HTMLOptGroupElement:
    case InterfaceName::HTMLOptionElement:
    case InterfaceName::HTMLOptionsCollection:
    case InterfaceName::HTMLOutputElement:
    case InterfaceName::HTMLParagraphElement:
    case InterfaceName::HTMLParamElement:
    case InterfaceName::HTMLPictureElement:
    case InterfaceName::HTMLPreElement:
    case InterfaceName::HTMLProgressElement:
    case InterfaceName::HTMLQuoteElement:
    case InterfaceName::HTMLScriptElement:
    case InterfaceName::HTMLSelectedContentElement:
    case InterfaceName::HTMLSelectElement:
    case InterfaceName::HTMLSlotElement:
    case InterfaceName::HTMLSourceElement:
    case InterfaceName::HTMLSpanElement:
    case InterfaceName::HTMLStyleElement:
    case InterfaceName::HTMLTableCaptionElement:
    case InterfaceName::HTMLTableCellElement:
    case InterfaceName::HTMLTableColElement:
    case InterfaceName::HTMLTableElement:
    case InterfaceName::HTMLTableRowElement:
    case InterfaceName::HTMLTableSectionElement:
    case InterfaceName::HTMLTemplateElement:
    case InterfaceName::HTMLTextAreaElement:
    case InterfaceName::HTMLTimeElement:
    case InterfaceName::HTMLTitleElement:
    case InterfaceName::HTMLTrackElement:
    case InterfaceName::HTMLUListElement:
    case InterfaceName::HTMLUnknownElement:
    case InterfaceName::HTMLVideoElement:
    case InterfaceName::ImageBitmap:
    case InterfaceName::ImageData:
    case InterfaceName::Location:
    case InterfaceName::MediaError:
    case InterfaceName::MessageChannel:
    case InterfaceName::MessageEvent:
    case InterfaceName::MessagePort:
    case InterfaceName::MimeType:
    case InterfaceName::MimeTypeArray:
    case InterfaceName::NavigateEvent:
    case InterfaceName::Navigation:
    case InterfaceName::NavigationCurrentEntryChangeEvent:
    case InterfaceName::NavigationDestination:
    case InterfaceName::NavigationHistoryEntry:
    case InterfaceName::NavigationTransition:
    case InterfaceName::Navigator:
    case InterfaceName::OffscreenCanvas:
    case InterfaceName::OffscreenCanvasRenderingContext2D:
    case InterfaceName::PageTransitionEvent:
    case InterfaceName::Path2D:
    case InterfaceName::Plugin:
    case InterfaceName::PluginArray:
    case InterfaceName::PopStateEvent:
    case InterfaceName::PromiseRejectionEvent:
    case InterfaceName::RadioNodeList:
    case InterfaceName::SharedWorker:
    case InterfaceName::Storage:
    case InterfaceName::StorageEvent:
    case InterfaceName::SubmitEvent:
    case InterfaceName::TextMetrics:
    case InterfaceName::TextTrack:
    case InterfaceName::TextTrackCue:
    case InterfaceName::TextTrackCueList:
    case InterfaceName::TextTrackList:
    case InterfaceName::TimeRanges:
    case InterfaceName::ToggleEvent:
    case InterfaceName::TrackEvent:
    case InterfaceName::UserActivation:
    case InterfaceName::ValidityState:
    case InterfaceName::VideoTrack:
    case InterfaceName::VideoTrackList:
    case InterfaceName::Window:
    case InterfaceName::Worker:
    case InterfaceName::XMLSerializer:
    case InterfaceName::Performance:
    case InterfaceName::IDBCursor:
    case InterfaceName::IDBCursorWithValue:
    case InterfaceName::IDBDatabase:
    case InterfaceName::IDBFactory:
    case InterfaceName::IDBIndex:
    case InterfaceName::IDBKeyRange:
    case InterfaceName::IDBObjectStore:
    case InterfaceName::IDBOpenDBRequest:
    case InterfaceName::IDBRecord:
    case InterfaceName::IDBRequest:
    case InterfaceName::IDBTransaction:
    case InterfaceName::IDBVersionChangeEvent:
    case InterfaceName::IntersectionObserver:
    case InterfaceName::IntersectionObserverEntry:
    case InterfaceName::MathMLElement:
    case InterfaceName::MediaCapabilities:
    case InterfaceName::BufferedChangeEvent:
    case InterfaceName::ManagedMediaSource:
    case InterfaceName::ManagedSourceBuffer:
    case InterfaceName::MediaSource:
    case InterfaceName::MediaSourceHandle:
    case InterfaceName::SourceBuffer:
    case InterfaceName::SourceBufferList:
    case InterfaceName::PerformanceNavigation:
    case InterfaceName::PerformanceTiming:
    case InterfaceName::Notification:
    case InterfaceName::PerformanceEntry:
    case InterfaceName::PerformanceObserver:
    case InterfaceName::PerformanceObserverEntryList:
    case InterfaceName::IdleDeadline:
    case InterfaceName::ResizeObserver:
    case InterfaceName::ResizeObserverEntry:
    case InterfaceName::ResizeObserverSize:
    case InterfaceName::PerformanceResourceTiming:
    case InterfaceName::Serial:
    case InterfaceName::SerialPort:
    case InterfaceName::CacheStorage:
    case InterfaceName::ServiceWorker:
    case InterfaceName::ServiceWorkerContainer:
    case InterfaceName::ServiceWorkerRegistration:
    case InterfaceName::SpeechGrammar:
    case InterfaceName::SpeechGrammarList:
    case InterfaceName::SpeechRecognition:
    case InterfaceName::SpeechRecognitionAlternative:
    case InterfaceName::SpeechRecognitionEvent:
    case InterfaceName::SpeechRecognitionPhrase:
    case InterfaceName::SpeechRecognitionResult:
    case InterfaceName::SpeechRecognitionResultList:
    case InterfaceName::SpeechSynthesis:
    case InterfaceName::SpeechSynthesisUtterance:
    case InterfaceName::SpeechSynthesisVoice:
    case InterfaceName::ByteLengthQueuingStrategy:
    case InterfaceName::CountQueuingStrategy:
    case InterfaceName::ReadableByteStreamController:
    case InterfaceName::ReadableStream:
    case InterfaceName::ReadableStreamBYOBReader:
    case InterfaceName::ReadableStreamBYOBRequest:
    case InterfaceName::ReadableStreamDefaultController:
    case InterfaceName::ReadableStreamDefaultReader:
    case InterfaceName::TransformStream:
    case InterfaceName::TransformStreamDefaultController:
    case InterfaceName::WritableStream:
    case InterfaceName::WritableStreamDefaultController:
    case InterfaceName::WritableStreamDefaultWriter:
    case InterfaceName::TrustedHTML:
    case InterfaceName::TrustedScript:
    case InterfaceName::TrustedScriptURL:
    case InterfaceName::TrustedTypePolicy:
    case InterfaceName::TrustedTypePolicyFactory:
    case InterfaceName::SVGAElement:
    case InterfaceName::SVGAnimatedEnumeration:
    case InterfaceName::SVGAnimatedInteger:
    case InterfaceName::SVGAnimatedLength:
    case InterfaceName::SVGAnimatedLengthList:
    case InterfaceName::SVGAnimatedNumber:
    case InterfaceName::SVGAnimatedNumberList:
    case InterfaceName::SVGAnimatedRect:
    case InterfaceName::SVGAnimatedString:
    case InterfaceName::SVGAnimatedTransformList:
    case InterfaceName::SVGAnimationElement:
    case InterfaceName::SVGClipPathElement:
    case InterfaceName::SVGComponentTransferFunctionElement:
    case InterfaceName::SVGDefsElement:
    case InterfaceName::SVGDescElement:
    case InterfaceName::SVGElement:
    case InterfaceName::SVGGElement:
    case InterfaceName::SVGGeometryElement:
    case InterfaceName::SVGGradientElement:
    case InterfaceName::SVGGraphicsElement:
    case InterfaceName::SVGImageElement:
    case InterfaceName::SVGCircleElement:
    case InterfaceName::SVGEllipseElement:
    case InterfaceName::SVGFEBlendElement:
    case InterfaceName::SVGFEColorMatrixElement:
    case InterfaceName::SVGFEComponentTransferElement:
    case InterfaceName::SVGFECompositeElement:
    case InterfaceName::SVGFEDisplacementMapElement:
    case InterfaceName::SVGFEDropShadowElement:
    case InterfaceName::SVGFEFloodElement:
    case InterfaceName::SVGFEFuncAElement:
    case InterfaceName::SVGFEFuncBElement:
    case InterfaceName::SVGFEFuncGElement:
    case InterfaceName::SVGFEFuncRElement:
    case InterfaceName::SVGFEGaussianBlurElement:
    case InterfaceName::SVGFEImageElement:
    case InterfaceName::SVGFEMergeElement:
    case InterfaceName::SVGFEMergeNodeElement:
    case InterfaceName::SVGFEMorphologyElement:
    case InterfaceName::SVGFEOffsetElement:
    case InterfaceName::SVGFETurbulenceElement:
    case InterfaceName::SVGFilterElement:
    case InterfaceName::SVGForeignObjectElement:
    case InterfaceName::SVGLength:
    case InterfaceName::SVGLengthList:
    case InterfaceName::SVGLineElement:
    case InterfaceName::SVGLinearGradientElement:
    case InterfaceName::SVGMaskElement:
    case InterfaceName::SVGMetadataElement:
    case InterfaceName::SVGNumber:
    case InterfaceName::SVGNumberList:
    case InterfaceName::SVGPathElement:
    case InterfaceName::SVGPatternElement:
    case InterfaceName::SVGPolygonElement:
    case InterfaceName::SVGPolylineElement:
    case InterfaceName::SVGRadialGradientElement:
    case InterfaceName::SVGRectElement:
    case InterfaceName::SVGScriptElement:
    case InterfaceName::SVGSVGElement:
    case InterfaceName::SVGStopElement:
    case InterfaceName::SVGStyleElement:
    case InterfaceName::SVGSymbolElement:
    case InterfaceName::SVGTextContentElement:
    case InterfaceName::SVGTextElement:
    case InterfaceName::SVGTextPathElement:
    case InterfaceName::SVGTextPositioningElement:
    case InterfaceName::SVGTitleElement:
    case InterfaceName::SVGTransform:
    case InterfaceName::SVGTransformList:
    case InterfaceName::SVGTSpanElement:
    case InterfaceName::SVGUnitTypes:
    case InterfaceName::SVGUseElement:
    case InterfaceName::SVGViewElement:
    case InterfaceName::Selection:
    case InterfaceName::StorageManager:
    case InterfaceName::CompositionEvent:
    case InterfaceName::FocusEvent:
    case InterfaceName::InputEvent:
    case InterfaceName::KeyboardEvent:
    case InterfaceName::MouseEvent:
    case InterfaceName::PointerEvent:
    case InterfaceName::TextEvent:
    case InterfaceName::UIEvent:
    case InterfaceName::WheelEvent:
    case InterfaceName::URLPattern:
    case InterfaceName::PerformanceMark:
    case InterfaceName::PerformanceMeasure:
    case InterfaceName::ViewTransition:
    case InterfaceName::Global:
    case InterfaceName::Instance:
    case InterfaceName::Memory:
    case InterfaceName::Module:
    case InterfaceName::Table:
    case InterfaceName::AnalyserNode:
    case InterfaceName::AudioBuffer:
    case InterfaceName::AudioBufferSourceNode:
    case InterfaceName::AudioContext:
    case InterfaceName::AudioDestinationNode:
    case InterfaceName::AudioListener:
    case InterfaceName::AudioNode:
    case InterfaceName::AudioParam:
    case InterfaceName::AudioScheduledSourceNode:
    case InterfaceName::BaseAudioContext:
    case InterfaceName::BiquadFilterNode:
    case InterfaceName::DynamicsCompressorNode:
    case InterfaceName::ChannelMergerNode:
    case InterfaceName::ChannelSplitterNode:
    case InterfaceName::ConstantSourceNode:
    case InterfaceName::DelayNode:
    case InterfaceName::GainNode:
    case InterfaceName::MediaElementAudioSourceNode:
    case InterfaceName::OfflineAudioCompletionEvent:
    case InterfaceName::OfflineAudioContext:
    case InterfaceName::OscillatorNode:
    case InterfaceName::PannerNode:
    case InterfaceName::PeriodicWave:
    case InterfaceName::ScriptProcessorNode:
    case InterfaceName::StereoPannerNode:
    case InterfaceName::ANGLEInstancedArrays:
    case InterfaceName::EXTBlendMinMax:
    case InterfaceName::EXTColorBufferFloat:
    case InterfaceName::EXTRenderSnorm:
    case InterfaceName::EXTTextureFilterAnisotropic:
    case InterfaceName::EXTTextureNorm16:
    case InterfaceName::OESElementIndexUint:
    case InterfaceName::OESStandardDerivatives:
    case InterfaceName::OESVertexArrayObject:
    case InterfaceName::WebGLCompressedTextureS3tc:
    case InterfaceName::WebGLCompressedTextureS3tcSrgb:
    case InterfaceName::WebGLDrawBuffers:
    case InterfaceName::WebGLVertexArrayObjectOES:
    case InterfaceName::WebGL2RenderingContext:
    case InterfaceName::WebGLActiveInfo:
    case InterfaceName::WebGLBuffer:
    case InterfaceName::WebGLContextEvent:
    case InterfaceName::WebGLFramebuffer:
    case InterfaceName::WebGLObject:
    case InterfaceName::WebGLProgram:
    case InterfaceName::WebGLQuery:
    case InterfaceName::WebGLRenderbuffer:
    case InterfaceName::WebGLRenderingContext:
    case InterfaceName::WebGLSampler:
    case InterfaceName::WebGLShader:
    case InterfaceName::WebGLShaderPrecisionFormat:
    case InterfaceName::WebGLSync:
    case InterfaceName::WebGLTexture:
    case InterfaceName::WebGLTransformFeedback:
    case InterfaceName::WebGLUniformLocation:
    case InterfaceName::WebGLVertexArrayObject:
    case InterfaceName::DOMException:
    case InterfaceName::QuotaExceededError:
    case InterfaceName::WebSocket:
    case InterfaceName::VTTCue:
    case InterfaceName::VTTRegion:
    case InterfaceName::XRSession:
    case InterfaceName::XRSessionEvent:
    case InterfaceName::XRSystem:
    case InterfaceName::FormData:
    case InterfaceName::ProgressEvent:
    case InterfaceName::XMLHttpRequest:
    case InterfaceName::XMLHttpRequestEventTarget:
    case InterfaceName::XMLHttpRequestUpload:
    case InterfaceName::XPathResult:
    case InterfaceName::XPathExpression:
    case InterfaceName::XPathEvaluator:
        return true;
    default:
        return false;
    }
}

static constexpr bool is_dedicated_worker_exposed(InterfaceName name)
{
    switch (name) {
    case InterfaceName::SecurityPolicyViolationEvent:
    case InterfaceName::CompressionStream:
    case InterfaceName::DecompressionStream:
    case InterfaceName::Crypto:
    case InterfaceName::CryptoKey:
    case InterfaceName::SubtleCrypto:
    case InterfaceName::CSSImageValue:
    case InterfaceName::CSSKeywordValue:
    case InterfaceName::CSSMathClamp:
    case InterfaceName::CSSMathInvert:
    case InterfaceName::CSSMathMax:
    case InterfaceName::CSSMathMin:
    case InterfaceName::CSSMathNegate:
    case InterfaceName::CSSMathProduct:
    case InterfaceName::CSSMathSum:
    case InterfaceName::CSSMathValue:
    case InterfaceName::CSSMatrixComponent:
    case InterfaceName::CSSNumericArray:
    case InterfaceName::CSSNumericValue:
    case InterfaceName::CSSPerspective:
    case InterfaceName::CSSRotate:
    case InterfaceName::CSSScale:
    case InterfaceName::CSSSkew:
    case InterfaceName::CSSSkewX:
    case InterfaceName::CSSSkewY:
    case InterfaceName::CSSStyleValue:
    case InterfaceName::CSSTransformComponent:
    case InterfaceName::CSSTransformValue:
    case InterfaceName::CSSTranslate:
    case InterfaceName::CSSUnitValue:
    case InterfaceName::CSSUnparsedValue:
    case InterfaceName::CSSVariableReferenceValue:
    case InterfaceName::FontFace:
    case InterfaceName::FontFaceSet:
    case InterfaceName::FontFaceSetLoadEvent:
    case InterfaceName::StylePropertyMapReadOnly:
    case InterfaceName::AbortController:
    case InterfaceName::AbortSignal:
    case InterfaceName::CustomEvent:
    case InterfaceName::Event:
    case InterfaceName::EventTarget:
    case InterfaceName::URL:
    case InterfaceName::Origin:
    case InterfaceName::URLSearchParams:
    case InterfaceName::TextDecoder:
    case InterfaceName::TextEncoder:
    case InterfaceName::TextEncoderStream:
    case InterfaceName::Headers:
    case InterfaceName::Request:
    case InterfaceName::Response:
    case InterfaceName::Blob:
    case InterfaceName::File:
    case InterfaceName::FileList:
    case InterfaceName::FileReader:
    case InterfaceName::FileReaderSync:
    case InterfaceName::DOMMatrix:
    case InterfaceName::DOMMatrixReadOnly:
    case InterfaceName::DOMPoint:
    case InterfaceName::DOMPointReadOnly:
    case InterfaceName::DOMQuad:
    case InterfaceName::DOMRect:
    case InterfaceName::DOMRectReadOnly:
    case InterfaceName::BroadcastChannel:
    case InterfaceName::CanvasGradient:
    case InterfaceName::CanvasPattern:
    case InterfaceName::CloseEvent:
    case InterfaceName::DedicatedWorkerGlobalScope:
    case InterfaceName::DOMStringList:
    case InterfaceName::ErrorEvent:
    case InterfaceName::EventSource:
    case InterfaceName::ImageBitmap:
    case InterfaceName::ImageData:
    case InterfaceName::MessageChannel:
    case InterfaceName::MessageEvent:
    case InterfaceName::MessagePort:
    case InterfaceName::OffscreenCanvas:
    case InterfaceName::OffscreenCanvasRenderingContext2D:
    case InterfaceName::Path2D:
    case InterfaceName::PromiseRejectionEvent:
    case InterfaceName::TextMetrics:
    case InterfaceName::Worker:
    case InterfaceName::WorkerGlobalScope:
    case InterfaceName::WorkerLocation:
    case InterfaceName::WorkerNavigator:
    case InterfaceName::Performance:
    case InterfaceName::IDBCursor:
    case InterfaceName::IDBCursorWithValue:
    case InterfaceName::IDBDatabase:
    case InterfaceName::IDBFactory:
    case InterfaceName::IDBIndex:
    case InterfaceName::IDBKeyRange:
    case InterfaceName::IDBObjectStore:
    case InterfaceName::IDBOpenDBRequest:
    case InterfaceName::IDBRecord:
    case InterfaceName::IDBRequest:
    case InterfaceName::IDBTransaction:
    case InterfaceName::IDBVersionChangeEvent:
    case InterfaceName::MediaCapabilities:
    case InterfaceName::BufferedChangeEvent:
    case InterfaceName::ManagedMediaSource:
    case InterfaceName::ManagedSourceBuffer:
    case InterfaceName::MediaSource:
    case InterfaceName::MediaSourceHandle:
    case InterfaceName::SourceBuffer:
    case InterfaceName::SourceBufferList:
    case InterfaceName::Notification:
    case InterfaceName::PerformanceEntry:
    case InterfaceName::PerformanceObserver:
    case InterfaceName::PerformanceObserverEntryList:
    case InterfaceName::PerformanceResourceTiming:
    case InterfaceName::Serial:
    case InterfaceName::SerialPort:
    case InterfaceName::CacheStorage:
    case InterfaceName::ServiceWorker:
    case InterfaceName::ServiceWorkerContainer:
    case InterfaceName::ServiceWorkerRegistration:
    case InterfaceName::ByteLengthQueuingStrategy:
    case InterfaceName::CountQueuingStrategy:
    case InterfaceName::ReadableByteStreamController:
    case InterfaceName::ReadableStream:
    case InterfaceName::ReadableStreamBYOBReader:
    case InterfaceName::ReadableStreamBYOBRequest:
    case InterfaceName::ReadableStreamDefaultController:
    case InterfaceName::ReadableStreamDefaultReader:
    case InterfaceName::TransformStream:
    case InterfaceName::TransformStreamDefaultController:
    case InterfaceName::WritableStream:
    case InterfaceName::WritableStreamDefaultController:
    case InterfaceName::WritableStreamDefaultWriter:
    case InterfaceName::TrustedHTML:
    case InterfaceName::TrustedScript:
    case InterfaceName::TrustedScriptURL:
    case InterfaceName::TrustedTypePolicy:
    case InterfaceName::TrustedTypePolicyFactory:
    case InterfaceName::StorageManager:
    case InterfaceName::URLPattern:
    case InterfaceName::PerformanceMark:
    case InterfaceName::PerformanceMeasure:
    case InterfaceName::Global:
    case InterfaceName::Instance:
    case InterfaceName::Memory:
    case InterfaceName::Module:
    case InterfaceName::Table:
    case InterfaceName::ANGLEInstancedArrays:
    case InterfaceName::EXTBlendMinMax:
    case InterfaceName::EXTColorBufferFloat:
    case InterfaceName::EXTRenderSnorm:
    case InterfaceName::EXTTextureFilterAnisotropic:
    case InterfaceName::EXTTextureNorm16:
    case InterfaceName::OESElementIndexUint:
    case InterfaceName::OESStandardDerivatives:
    case InterfaceName::OESVertexArrayObject:
    case InterfaceName::WebGLCompressedTextureS3tc:
    case InterfaceName::WebGLCompressedTextureS3tcSrgb:
    case InterfaceName::WebGLDrawBuffers:
    case InterfaceName::WebGLVertexArrayObjectOES:
    case InterfaceName::WebGL2RenderingContext:
    case InterfaceName::WebGLActiveInfo:
    case InterfaceName::WebGLBuffer:
    case InterfaceName::WebGLContextEvent:
    case InterfaceName::WebGLFramebuffer:
    case InterfaceName::WebGLObject:
    case InterfaceName::WebGLProgram:
    case InterfaceName::WebGLQuery:
    case InterfaceName::WebGLRenderbuffer:
    case InterfaceName::WebGLRenderingContext:
    case InterfaceName::WebGLSampler:
    case InterfaceName::WebGLShader:
    case InterfaceName::WebGLShaderPrecisionFormat:
    case InterfaceName::WebGLSync:
    case InterfaceName::WebGLTexture:
    case InterfaceName::WebGLTransformFeedback:
    case InterfaceName::WebGLUniformLocation:
    case InterfaceName::WebGLVertexArrayObject:
    case InterfaceName::DOMException:
    case InterfaceName::QuotaExceededError:
    case InterfaceName::WebSocket:
    case InterfaceName::FormData:
    case InterfaceName::ProgressEvent:
    case InterfaceName::XMLHttpRequest:
    case InterfaceName::XMLHttpRequestEventTarget:
    case InterfaceName::XMLHttpRequestUpload:
        return true;
    default:
        return false;
    }
}

static constexpr bool is_shared_worker_exposed(InterfaceName name)
{
    switch (name) {
    case InterfaceName::SecurityPolicyViolationEvent:
    case InterfaceName::CompressionStream:
    case InterfaceName::DecompressionStream:
    case InterfaceName::Crypto:
    case InterfaceName::CryptoKey:
    case InterfaceName::SubtleCrypto:
    case InterfaceName::CSSImageValue:
    case InterfaceName::CSSKeywordValue:
    case InterfaceName::CSSMathClamp:
    case InterfaceName::CSSMathInvert:
    case InterfaceName::CSSMathMax:
    case InterfaceName::CSSMathMin:
    case InterfaceName::CSSMathNegate:
    case InterfaceName::CSSMathProduct:
    case InterfaceName::CSSMathSum:
    case InterfaceName::CSSMathValue:
    case InterfaceName::CSSMatrixComponent:
    case InterfaceName::CSSNumericArray:
    case InterfaceName::CSSNumericValue:
    case InterfaceName::CSSPerspective:
    case InterfaceName::CSSRotate:
    case InterfaceName::CSSScale:
    case InterfaceName::CSSSkew:
    case InterfaceName::CSSSkewX:
    case InterfaceName::CSSSkewY:
    case InterfaceName::CSSStyleValue:
    case InterfaceName::CSSTransformComponent:
    case InterfaceName::CSSTransformValue:
    case InterfaceName::CSSTranslate:
    case InterfaceName::CSSUnitValue:
    case InterfaceName::CSSUnparsedValue:
    case InterfaceName::CSSVariableReferenceValue:
    case InterfaceName::FontFace:
    case InterfaceName::FontFaceSet:
    case InterfaceName::FontFaceSetLoadEvent:
    case InterfaceName::StylePropertyMapReadOnly:
    case InterfaceName::AbortController:
    case InterfaceName::AbortSignal:
    case InterfaceName::CustomEvent:
    case InterfaceName::Event:
    case InterfaceName::EventTarget:
    case InterfaceName::URL:
    case InterfaceName::Origin:
    case InterfaceName::URLSearchParams:
    case InterfaceName::TextDecoder:
    case InterfaceName::TextEncoder:
    case InterfaceName::TextEncoderStream:
    case InterfaceName::Headers:
    case InterfaceName::Request:
    case InterfaceName::Response:
    case InterfaceName::Blob:
    case InterfaceName::File:
    case InterfaceName::FileList:
    case InterfaceName::FileReader:
    case InterfaceName::FileReaderSync:
    case InterfaceName::DOMMatrix:
    case InterfaceName::DOMMatrixReadOnly:
    case InterfaceName::DOMPoint:
    case InterfaceName::DOMPointReadOnly:
    case InterfaceName::DOMQuad:
    case InterfaceName::DOMRect:
    case InterfaceName::DOMRectReadOnly:
    case InterfaceName::BroadcastChannel:
    case InterfaceName::CanvasGradient:
    case InterfaceName::CanvasPattern:
    case InterfaceName::CloseEvent:
    case InterfaceName::DOMStringList:
    case InterfaceName::ErrorEvent:
    case InterfaceName::EventSource:
    case InterfaceName::ImageBitmap:
    case InterfaceName::ImageData:
    case InterfaceName::MessageChannel:
    case InterfaceName::MessageEvent:
    case InterfaceName::MessagePort:
    case InterfaceName::OffscreenCanvas:
    case InterfaceName::OffscreenCanvasRenderingContext2D:
    case InterfaceName::Path2D:
    case InterfaceName::PromiseRejectionEvent:
    case InterfaceName::SharedWorkerGlobalScope:
    case InterfaceName::TextMetrics:
    case InterfaceName::Worker:
    case InterfaceName::WorkerGlobalScope:
    case InterfaceName::WorkerLocation:
    case InterfaceName::WorkerNavigator:
    case InterfaceName::Performance:
    case InterfaceName::IDBCursor:
    case InterfaceName::IDBCursorWithValue:
    case InterfaceName::IDBDatabase:
    case InterfaceName::IDBFactory:
    case InterfaceName::IDBIndex:
    case InterfaceName::IDBKeyRange:
    case InterfaceName::IDBObjectStore:
    case InterfaceName::IDBOpenDBRequest:
    case InterfaceName::IDBRecord:
    case InterfaceName::IDBRequest:
    case InterfaceName::IDBTransaction:
    case InterfaceName::IDBVersionChangeEvent:
    case InterfaceName::MediaCapabilities:
    case InterfaceName::Notification:
    case InterfaceName::PerformanceEntry:
    case InterfaceName::PerformanceObserver:
    case InterfaceName::PerformanceObserverEntryList:
    case InterfaceName::PerformanceResourceTiming:
    case InterfaceName::CacheStorage:
    case InterfaceName::ServiceWorker:
    case InterfaceName::ServiceWorkerContainer:
    case InterfaceName::ServiceWorkerRegistration:
    case InterfaceName::ByteLengthQueuingStrategy:
    case InterfaceName::CountQueuingStrategy:
    case InterfaceName::ReadableByteStreamController:
    case InterfaceName::ReadableStream:
    case InterfaceName::ReadableStreamBYOBReader:
    case InterfaceName::ReadableStreamBYOBRequest:
    case InterfaceName::ReadableStreamDefaultController:
    case InterfaceName::ReadableStreamDefaultReader:
    case InterfaceName::TransformStream:
    case InterfaceName::TransformStreamDefaultController:
    case InterfaceName::WritableStream:
    case InterfaceName::WritableStreamDefaultController:
    case InterfaceName::WritableStreamDefaultWriter:
    case InterfaceName::TrustedHTML:
    case InterfaceName::TrustedScript:
    case InterfaceName::TrustedScriptURL:
    case InterfaceName::TrustedTypePolicy:
    case InterfaceName::TrustedTypePolicyFactory:
    case InterfaceName::StorageManager:
    case InterfaceName::URLPattern:
    case InterfaceName::PerformanceMark:
    case InterfaceName::PerformanceMeasure:
    case InterfaceName::Global:
    case InterfaceName::Instance:
    case InterfaceName::Memory:
    case InterfaceName::Module:
    case InterfaceName::Table:
    case InterfaceName::ANGLEInstancedArrays:
    case InterfaceName::EXTBlendMinMax:
    case InterfaceName::EXTColorBufferFloat:
    case InterfaceName::EXTRenderSnorm:
    case InterfaceName::EXTTextureFilterAnisotropic:
    case InterfaceName::EXTTextureNorm16:
    case InterfaceName::OESElementIndexUint:
    case InterfaceName::OESStandardDerivatives:
    case InterfaceName::OESVertexArrayObject:
    case InterfaceName::WebGLCompressedTextureS3tc:
    case InterfaceName::WebGLCompressedTextureS3tcSrgb:
    case InterfaceName::WebGLDrawBuffers:
    case InterfaceName::WebGLVertexArrayObjectOES:
    case InterfaceName::WebGL2RenderingContext:
    case InterfaceName::WebGLActiveInfo:
    case InterfaceName::WebGLBuffer:
    case InterfaceName::WebGLContextEvent:
    case InterfaceName::WebGLFramebuffer:
    case InterfaceName::WebGLObject:
    case InterfaceName::WebGLProgram:
    case InterfaceName::WebGLQuery:
    case InterfaceName::WebGLRenderbuffer:
    case InterfaceName::WebGLRenderingContext:
    case InterfaceName::WebGLSampler:
    case InterfaceName::WebGLShader:
    case InterfaceName::WebGLShaderPrecisionFormat:
    case InterfaceName::WebGLSync:
    case InterfaceName::WebGLTexture:
    case InterfaceName::WebGLTransformFeedback:
    case InterfaceName::WebGLUniformLocation:
    case InterfaceName::WebGLVertexArrayObject:
    case InterfaceName::DOMException:
    case InterfaceName::QuotaExceededError:
    case InterfaceName::WebSocket:
    case InterfaceName::FormData:
    case InterfaceName::ProgressEvent:
    case InterfaceName::XMLHttpRequest:
    case InterfaceName::XMLHttpRequestEventTarget:
    case InterfaceName::XMLHttpRequestUpload:
        return true;
    default:
        return false;
    }
}

static constexpr bool is_shadow_realm_exposed(InterfaceName name)
{
    switch (name) {
    case InterfaceName::CompressionStream:
    case InterfaceName::DecompressionStream:
    case InterfaceName::AbortController:
    case InterfaceName::AbortSignal:
    case InterfaceName::CustomEvent:
    case InterfaceName::Event:
    case InterfaceName::EventTarget:
    case InterfaceName::URL:
    case InterfaceName::Origin:
    case InterfaceName::URLSearchParams:
    case InterfaceName::TextDecoder:
    case InterfaceName::TextEncoder:
    case InterfaceName::TextEncoderStream:
    case InterfaceName::CloseEvent:
    case InterfaceName::PromiseRejectionEvent:
    case InterfaceName::ShadowRealmGlobalScope:
    case InterfaceName::ByteLengthQueuingStrategy:
    case InterfaceName::CountQueuingStrategy:
    case InterfaceName::ReadableByteStreamController:
    case InterfaceName::ReadableStream:
    case InterfaceName::ReadableStreamBYOBReader:
    case InterfaceName::ReadableStreamBYOBRequest:
    case InterfaceName::ReadableStreamDefaultController:
    case InterfaceName::ReadableStreamDefaultReader:
    case InterfaceName::TransformStream:
    case InterfaceName::TransformStreamDefaultController:
    case InterfaceName::WritableStream:
    case InterfaceName::WritableStreamDefaultController:
    case InterfaceName::WritableStreamDefaultWriter:
    case InterfaceName::Global:
    case InterfaceName::Instance:
    case InterfaceName::Memory:
    case InterfaceName::Module:
    case InterfaceName::Table:
    case InterfaceName::DOMException:
    case InterfaceName::QuotaExceededError:
        return true;
    default:
        return false;
    }
}

// An interface, callback interface, namespace, or member construct is exposed in a given realm realm if the following steps return true:
// FIXME: Make this compatible with non-interface types.
bool is_exposed(InterfaceName name, JS::Realm& realm)
{
    auto const& global_object = realm.global_object();

    // 1. If construct’s exposure set is not *, and realm.[[GlobalObject]] does not implement an interface that is in construct’s exposure set, then return false.
    if (is<HTML::Window>(global_object)) {
       if (!is_window_exposed(name))
           return false;
    } else if (is<HTML::DedicatedWorkerGlobalScope>(global_object)) {
       if (!is_dedicated_worker_exposed(name))
           return false;
    } else if (is<HTML::SharedWorkerGlobalScope>(global_object)) {
        if (!is_shared_worker_exposed(name))
            return false;
    } else if (is<HTML::ShadowRealmGlobalScope>(global_object)) {
        if (!is_shadow_realm_exposed(name))
            return false;
    } else {
        TODO(); // FIXME: ServiceWorkerGlobalScope and WorkletGlobalScope.
    }

    // 2. If realm’s settings object is not a secure context, and construct is conditionally exposed on
    //    [SecureContext], then return false.
    if (is_secure_context_interface(name) && HTML::is_non_secure_context(principal_host_defined_environment_settings_object(realm)))
        return false;

    // AD-HOC: Do not expose experimental interfaces unless instructed to do so.
    if (!HTML::UniversalGlobalScopeMixin::expose_experimental_interfaces() && is_experimental_interface(name))
        return false;

    // FIXME: 3. If realm’s settings object’s cross-origin isolated capability is false, and construct is
    //           conditionally exposed on [CrossOriginIsolated], then return false.

    // 4. Return true.
    return true;
}


template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AnimationPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AnimationPrototype>(realm);
    m_prototypes.set("Animation"_fly_string, prototype);

    auto constructor = realm.create<AnimationConstructor>(realm);
    m_constructors.set("Animation"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AnimationEffectPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AnimationEffectPrototype>(realm);
    m_prototypes.set("AnimationEffect"_fly_string, prototype);

    auto constructor = realm.create<AnimationEffectConstructor>(realm);
    m_constructors.set("AnimationEffect"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AnimationPlaybackEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AnimationPlaybackEventPrototype>(realm);
    m_prototypes.set("AnimationPlaybackEvent"_fly_string, prototype);

    auto constructor = realm.create<AnimationPlaybackEventConstructor>(realm);
    m_constructors.set("AnimationPlaybackEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AnimationTimelinePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AnimationTimelinePrototype>(realm);
    m_prototypes.set("AnimationTimeline"_fly_string, prototype);

    auto constructor = realm.create<AnimationTimelineConstructor>(realm);
    m_constructors.set("AnimationTimeline"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DocumentTimelinePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DocumentTimelinePrototype>(realm);
    m_prototypes.set("DocumentTimeline"_fly_string, prototype);

    auto constructor = realm.create<DocumentTimelineConstructor>(realm);
    m_constructors.set("DocumentTimeline"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<KeyframeEffectPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<KeyframeEffectPrototype>(realm);
    m_prototypes.set("KeyframeEffect"_fly_string, prototype);

    auto constructor = realm.create<KeyframeEffectConstructor>(realm);
    m_constructors.set("KeyframeEffect"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ScrollTimelinePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ScrollTimelinePrototype>(realm);
    m_prototypes.set("ScrollTimeline"_fly_string, prototype);

    auto constructor = realm.create<ScrollTimelineConstructor>(realm);
    m_constructors.set("ScrollTimeline"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ClipboardPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ClipboardPrototype>(realm);
    m_prototypes.set("Clipboard"_fly_string, prototype);

    auto constructor = realm.create<ClipboardConstructor>(realm);
    m_constructors.set("Clipboard"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ClipboardEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ClipboardEventPrototype>(realm);
    m_prototypes.set("ClipboardEvent"_fly_string, prototype);

    auto constructor = realm.create<ClipboardEventConstructor>(realm);
    m_constructors.set("ClipboardEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ClipboardItemPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ClipboardItemPrototype>(realm);
    m_prototypes.set("ClipboardItem"_fly_string, prototype);

    auto constructor = realm.create<ClipboardItemConstructor>(realm);
    m_constructors.set("ClipboardItem"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SecurityPolicyViolationEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SecurityPolicyViolationEventPrototype>(realm);
    m_prototypes.set("SecurityPolicyViolationEvent"_fly_string, prototype);

    auto constructor = realm.create<SecurityPolicyViolationEventConstructor>(realm);
    m_constructors.set("SecurityPolicyViolationEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CompressionStreamPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CompressionStreamPrototype>(realm);
    m_prototypes.set("CompressionStream"_fly_string, prototype);

    auto constructor = realm.create<CompressionStreamConstructor>(realm);
    m_constructors.set("CompressionStream"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DecompressionStreamPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DecompressionStreamPrototype>(realm);
    m_prototypes.set("DecompressionStream"_fly_string, prototype);

    auto constructor = realm.create<DecompressionStreamConstructor>(realm);
    m_constructors.set("DecompressionStream"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CookieChangeEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CookieChangeEventPrototype>(realm);
    m_prototypes.set("CookieChangeEvent"_fly_string, prototype);

    auto constructor = realm.create<CookieChangeEventConstructor>(realm);
    m_constructors.set("CookieChangeEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CookieStorePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CookieStorePrototype>(realm);
    m_prototypes.set("CookieStore"_fly_string, prototype);

    auto constructor = realm.create<CookieStoreConstructor>(realm);
    m_constructors.set("CookieStore"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CredentialPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CredentialPrototype>(realm);
    m_prototypes.set("Credential"_fly_string, prototype);

    auto constructor = realm.create<CredentialConstructor>(realm);
    m_constructors.set("Credential"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CredentialsContainerPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CredentialsContainerPrototype>(realm);
    m_prototypes.set("CredentialsContainer"_fly_string, prototype);

    auto constructor = realm.create<CredentialsContainerConstructor>(realm);
    m_constructors.set("CredentialsContainer"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<FederatedCredentialPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<FederatedCredentialPrototype>(realm);
    m_prototypes.set("FederatedCredential"_fly_string, prototype);

    auto constructor = realm.create<FederatedCredentialConstructor>(realm);
    m_constructors.set("FederatedCredential"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PasswordCredentialPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PasswordCredentialPrototype>(realm);
    m_prototypes.set("PasswordCredential"_fly_string, prototype);

    auto constructor = realm.create<PasswordCredentialConstructor>(realm);
    m_constructors.set("PasswordCredential"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CryptoPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CryptoPrototype>(realm);
    m_prototypes.set("Crypto"_fly_string, prototype);

    auto constructor = realm.create<CryptoConstructor>(realm);
    m_constructors.set("Crypto"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CryptoKeyPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CryptoKeyPrototype>(realm);
    m_prototypes.set("CryptoKey"_fly_string, prototype);

    auto constructor = realm.create<CryptoKeyConstructor>(realm);
    m_constructors.set("CryptoKey"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SubtleCryptoPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SubtleCryptoPrototype>(realm);
    m_prototypes.set("SubtleCrypto"_fly_string, prototype);

    auto constructor = realm.create<SubtleCryptoConstructor>(realm);
    m_constructors.set("SubtleCrypto"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AnimationEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AnimationEventPrototype>(realm);
    m_prototypes.set("AnimationEvent"_fly_string, prototype);

    auto constructor = realm.create<AnimationEventConstructor>(realm);
    m_constructors.set("AnimationEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSAnimationPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSAnimationPrototype>(realm);
    m_prototypes.set("CSSAnimation"_fly_string, prototype);

    auto constructor = realm.create<CSSAnimationConstructor>(realm);
    m_constructors.set("CSSAnimation"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSConditionRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSConditionRulePrototype>(realm);
    m_prototypes.set("CSSConditionRule"_fly_string, prototype);

    auto constructor = realm.create<CSSConditionRuleConstructor>(realm);
    m_constructors.set("CSSConditionRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSCounterStyleRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSCounterStyleRulePrototype>(realm);
    m_prototypes.set("CSSCounterStyleRule"_fly_string, prototype);

    auto constructor = realm.create<CSSCounterStyleRuleConstructor>(realm);
    m_constructors.set("CSSCounterStyleRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSFontFaceDescriptorsPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSFontFaceDescriptorsPrototype>(realm);
    m_prototypes.set("CSSFontFaceDescriptors"_fly_string, prototype);

    auto constructor = realm.create<CSSFontFaceDescriptorsConstructor>(realm);
    m_constructors.set("CSSFontFaceDescriptors"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSFontFaceRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSFontFaceRulePrototype>(realm);
    m_prototypes.set("CSSFontFaceRule"_fly_string, prototype);

    auto constructor = realm.create<CSSFontFaceRuleConstructor>(realm);
    m_constructors.set("CSSFontFaceRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSFontFeatureValuesMapPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSFontFeatureValuesMapPrototype>(realm);
    m_prototypes.set("CSSFontFeatureValuesMap"_fly_string, prototype);

    auto constructor = realm.create<CSSFontFeatureValuesMapConstructor>(realm);
    m_constructors.set("CSSFontFeatureValuesMap"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSFontFeatureValuesRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSFontFeatureValuesRulePrototype>(realm);
    m_prototypes.set("CSSFontFeatureValuesRule"_fly_string, prototype);

    auto constructor = realm.create<CSSFontFeatureValuesRuleConstructor>(realm);
    m_constructors.set("CSSFontFeatureValuesRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSGroupingRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSGroupingRulePrototype>(realm);
    m_prototypes.set("CSSGroupingRule"_fly_string, prototype);

    auto constructor = realm.create<CSSGroupingRuleConstructor>(realm);
    m_constructors.set("CSSGroupingRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSImageValuePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSImageValuePrototype>(realm);
    m_prototypes.set("CSSImageValue"_fly_string, prototype);

    auto constructor = realm.create<CSSImageValueConstructor>(realm);
    m_constructors.set("CSSImageValue"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSImportRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSImportRulePrototype>(realm);
    m_prototypes.set("CSSImportRule"_fly_string, prototype);

    auto constructor = realm.create<CSSImportRuleConstructor>(realm);
    m_constructors.set("CSSImportRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSKeyframeRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSKeyframeRulePrototype>(realm);
    m_prototypes.set("CSSKeyframeRule"_fly_string, prototype);

    auto constructor = realm.create<CSSKeyframeRuleConstructor>(realm);
    m_constructors.set("CSSKeyframeRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSKeyframesRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSKeyframesRulePrototype>(realm);
    m_prototypes.set("CSSKeyframesRule"_fly_string, prototype);

    auto constructor = realm.create<CSSKeyframesRuleConstructor>(realm);
    m_constructors.set("CSSKeyframesRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSKeywordValuePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSKeywordValuePrototype>(realm);
    m_prototypes.set("CSSKeywordValue"_fly_string, prototype);

    auto constructor = realm.create<CSSKeywordValueConstructor>(realm);
    m_constructors.set("CSSKeywordValue"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSLayerBlockRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSLayerBlockRulePrototype>(realm);
    m_prototypes.set("CSSLayerBlockRule"_fly_string, prototype);

    auto constructor = realm.create<CSSLayerBlockRuleConstructor>(realm);
    m_constructors.set("CSSLayerBlockRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSLayerStatementRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSLayerStatementRulePrototype>(realm);
    m_prototypes.set("CSSLayerStatementRule"_fly_string, prototype);

    auto constructor = realm.create<CSSLayerStatementRuleConstructor>(realm);
    m_constructors.set("CSSLayerStatementRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSMarginRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSMarginRulePrototype>(realm);
    m_prototypes.set("CSSMarginRule"_fly_string, prototype);

    auto constructor = realm.create<CSSMarginRuleConstructor>(realm);
    m_constructors.set("CSSMarginRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSMathClampPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSMathClampPrototype>(realm);
    m_prototypes.set("CSSMathClamp"_fly_string, prototype);

    auto constructor = realm.create<CSSMathClampConstructor>(realm);
    m_constructors.set("CSSMathClamp"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSMathInvertPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSMathInvertPrototype>(realm);
    m_prototypes.set("CSSMathInvert"_fly_string, prototype);

    auto constructor = realm.create<CSSMathInvertConstructor>(realm);
    m_constructors.set("CSSMathInvert"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSMathMaxPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSMathMaxPrototype>(realm);
    m_prototypes.set("CSSMathMax"_fly_string, prototype);

    auto constructor = realm.create<CSSMathMaxConstructor>(realm);
    m_constructors.set("CSSMathMax"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSMathMinPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSMathMinPrototype>(realm);
    m_prototypes.set("CSSMathMin"_fly_string, prototype);

    auto constructor = realm.create<CSSMathMinConstructor>(realm);
    m_constructors.set("CSSMathMin"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSMathNegatePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSMathNegatePrototype>(realm);
    m_prototypes.set("CSSMathNegate"_fly_string, prototype);

    auto constructor = realm.create<CSSMathNegateConstructor>(realm);
    m_constructors.set("CSSMathNegate"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSMathProductPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSMathProductPrototype>(realm);
    m_prototypes.set("CSSMathProduct"_fly_string, prototype);

    auto constructor = realm.create<CSSMathProductConstructor>(realm);
    m_constructors.set("CSSMathProduct"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSMathSumPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSMathSumPrototype>(realm);
    m_prototypes.set("CSSMathSum"_fly_string, prototype);

    auto constructor = realm.create<CSSMathSumConstructor>(realm);
    m_constructors.set("CSSMathSum"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSMathValuePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSMathValuePrototype>(realm);
    m_prototypes.set("CSSMathValue"_fly_string, prototype);

    auto constructor = realm.create<CSSMathValueConstructor>(realm);
    m_constructors.set("CSSMathValue"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSMatrixComponentPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSMatrixComponentPrototype>(realm);
    m_prototypes.set("CSSMatrixComponent"_fly_string, prototype);

    auto constructor = realm.create<CSSMatrixComponentConstructor>(realm);
    m_constructors.set("CSSMatrixComponent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSMediaRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSMediaRulePrototype>(realm);
    m_prototypes.set("CSSMediaRule"_fly_string, prototype);

    auto constructor = realm.create<CSSMediaRuleConstructor>(realm);
    m_constructors.set("CSSMediaRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSNamespaceRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSNamespaceRulePrototype>(realm);
    m_prototypes.set("CSSNamespaceRule"_fly_string, prototype);

    auto constructor = realm.create<CSSNamespaceRuleConstructor>(realm);
    m_constructors.set("CSSNamespaceRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSNestedDeclarationsPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSNestedDeclarationsPrototype>(realm);
    m_prototypes.set("CSSNestedDeclarations"_fly_string, prototype);

    auto constructor = realm.create<CSSNestedDeclarationsConstructor>(realm);
    m_constructors.set("CSSNestedDeclarations"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSNumericArrayPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSNumericArrayPrototype>(realm);
    m_prototypes.set("CSSNumericArray"_fly_string, prototype);

    auto constructor = realm.create<CSSNumericArrayConstructor>(realm);
    m_constructors.set("CSSNumericArray"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSNumericValuePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSNumericValuePrototype>(realm);
    m_prototypes.set("CSSNumericValue"_fly_string, prototype);

    auto constructor = realm.create<CSSNumericValueConstructor>(realm);
    m_constructors.set("CSSNumericValue"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSPageRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSPageRulePrototype>(realm);
    m_prototypes.set("CSSPageRule"_fly_string, prototype);

    auto constructor = realm.create<CSSPageRuleConstructor>(realm);
    m_constructors.set("CSSPageRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSPageDescriptorsPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSPageDescriptorsPrototype>(realm);
    m_prototypes.set("CSSPageDescriptors"_fly_string, prototype);

    auto constructor = realm.create<CSSPageDescriptorsConstructor>(realm);
    m_constructors.set("CSSPageDescriptors"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSPerspectivePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSPerspectivePrototype>(realm);
    m_prototypes.set("CSSPerspective"_fly_string, prototype);

    auto constructor = realm.create<CSSPerspectiveConstructor>(realm);
    m_constructors.set("CSSPerspective"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSPropertyRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSPropertyRulePrototype>(realm);
    m_prototypes.set("CSSPropertyRule"_fly_string, prototype);

    auto constructor = realm.create<CSSPropertyRuleConstructor>(realm);
    m_constructors.set("CSSPropertyRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSRotatePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSRotatePrototype>(realm);
    m_prototypes.set("CSSRotate"_fly_string, prototype);

    auto constructor = realm.create<CSSRotateConstructor>(realm);
    m_constructors.set("CSSRotate"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSRulePrototype>(realm);
    m_prototypes.set("CSSRule"_fly_string, prototype);

    auto constructor = realm.create<CSSRuleConstructor>(realm);
    m_constructors.set("CSSRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSRuleListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSRuleListPrototype>(realm);
    m_prototypes.set("CSSRuleList"_fly_string, prototype);

    auto constructor = realm.create<CSSRuleListConstructor>(realm);
    m_constructors.set("CSSRuleList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSScalePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSScalePrototype>(realm);
    m_prototypes.set("CSSScale"_fly_string, prototype);

    auto constructor = realm.create<CSSScaleConstructor>(realm);
    m_constructors.set("CSSScale"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSSkewPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSSkewPrototype>(realm);
    m_prototypes.set("CSSSkew"_fly_string, prototype);

    auto constructor = realm.create<CSSSkewConstructor>(realm);
    m_constructors.set("CSSSkew"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSSkewXPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSSkewXPrototype>(realm);
    m_prototypes.set("CSSSkewX"_fly_string, prototype);

    auto constructor = realm.create<CSSSkewXConstructor>(realm);
    m_constructors.set("CSSSkewX"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSSkewYPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSSkewYPrototype>(realm);
    m_prototypes.set("CSSSkewY"_fly_string, prototype);

    auto constructor = realm.create<CSSSkewYConstructor>(realm);
    m_constructors.set("CSSSkewY"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSStyleDeclarationPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSStyleDeclarationPrototype>(realm);
    m_prototypes.set("CSSStyleDeclaration"_fly_string, prototype);

    auto constructor = realm.create<CSSStyleDeclarationConstructor>(realm);
    m_constructors.set("CSSStyleDeclaration"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSStylePropertiesPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSStylePropertiesPrototype>(realm);
    m_prototypes.set("CSSStyleProperties"_fly_string, prototype);

    auto constructor = realm.create<CSSStylePropertiesConstructor>(realm);
    m_constructors.set("CSSStyleProperties"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSStyleRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSStyleRulePrototype>(realm);
    m_prototypes.set("CSSStyleRule"_fly_string, prototype);

    auto constructor = realm.create<CSSStyleRuleConstructor>(realm);
    m_constructors.set("CSSStyleRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSStyleSheetPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSStyleSheetPrototype>(realm);
    m_prototypes.set("CSSStyleSheet"_fly_string, prototype);

    auto constructor = realm.create<CSSStyleSheetConstructor>(realm);
    m_constructors.set("CSSStyleSheet"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSStyleValuePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSStyleValuePrototype>(realm);
    m_prototypes.set("CSSStyleValue"_fly_string, prototype);

    auto constructor = realm.create<CSSStyleValueConstructor>(realm);
    m_constructors.set("CSSStyleValue"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSSupportsRulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSSupportsRulePrototype>(realm);
    m_prototypes.set("CSSSupportsRule"_fly_string, prototype);

    auto constructor = realm.create<CSSSupportsRuleConstructor>(realm);
    m_constructors.set("CSSSupportsRule"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSTransformComponentPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSTransformComponentPrototype>(realm);
    m_prototypes.set("CSSTransformComponent"_fly_string, prototype);

    auto constructor = realm.create<CSSTransformComponentConstructor>(realm);
    m_constructors.set("CSSTransformComponent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSTransformValuePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSTransformValuePrototype>(realm);
    m_prototypes.set("CSSTransformValue"_fly_string, prototype);

    auto constructor = realm.create<CSSTransformValueConstructor>(realm);
    m_constructors.set("CSSTransformValue"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSTransitionPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSTransitionPrototype>(realm);
    m_prototypes.set("CSSTransition"_fly_string, prototype);

    auto constructor = realm.create<CSSTransitionConstructor>(realm);
    m_constructors.set("CSSTransition"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSTranslatePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSTranslatePrototype>(realm);
    m_prototypes.set("CSSTranslate"_fly_string, prototype);

    auto constructor = realm.create<CSSTranslateConstructor>(realm);
    m_constructors.set("CSSTranslate"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSUnitValuePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSUnitValuePrototype>(realm);
    m_prototypes.set("CSSUnitValue"_fly_string, prototype);

    auto constructor = realm.create<CSSUnitValueConstructor>(realm);
    m_constructors.set("CSSUnitValue"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSUnparsedValuePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSUnparsedValuePrototype>(realm);
    m_prototypes.set("CSSUnparsedValue"_fly_string, prototype);

    auto constructor = realm.create<CSSUnparsedValueConstructor>(realm);
    m_constructors.set("CSSUnparsedValue"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CSSVariableReferenceValuePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CSSVariableReferenceValuePrototype>(realm);
    m_prototypes.set("CSSVariableReferenceValue"_fly_string, prototype);

    auto constructor = realm.create<CSSVariableReferenceValueConstructor>(realm);
    m_constructors.set("CSSVariableReferenceValue"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<FontFacePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<FontFacePrototype>(realm);
    m_prototypes.set("FontFace"_fly_string, prototype);

    auto constructor = realm.create<FontFaceConstructor>(realm);
    m_constructors.set("FontFace"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<FontFaceSetPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<FontFaceSetPrototype>(realm);
    m_prototypes.set("FontFaceSet"_fly_string, prototype);

    auto constructor = realm.create<FontFaceSetConstructor>(realm);
    m_constructors.set("FontFaceSet"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<FontFaceSetLoadEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<FontFaceSetLoadEventPrototype>(realm);
    m_prototypes.set("FontFaceSetLoadEvent"_fly_string, prototype);

    auto constructor = realm.create<FontFaceSetLoadEventConstructor>(realm);
    m_constructors.set("FontFaceSetLoadEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MediaListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MediaListPrototype>(realm);
    m_prototypes.set("MediaList"_fly_string, prototype);

    auto constructor = realm.create<MediaListConstructor>(realm);
    m_constructors.set("MediaList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MediaQueryListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MediaQueryListPrototype>(realm);
    m_prototypes.set("MediaQueryList"_fly_string, prototype);

    auto constructor = realm.create<MediaQueryListConstructor>(realm);
    m_constructors.set("MediaQueryList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MediaQueryListEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MediaQueryListEventPrototype>(realm);
    m_prototypes.set("MediaQueryListEvent"_fly_string, prototype);

    auto constructor = realm.create<MediaQueryListEventConstructor>(realm);
    m_constructors.set("MediaQueryListEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ScreenPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ScreenPrototype>(realm);
    m_prototypes.set("Screen"_fly_string, prototype);

    auto constructor = realm.create<ScreenConstructor>(realm);
    m_constructors.set("Screen"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ScreenOrientationPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ScreenOrientationPrototype>(realm);
    m_prototypes.set("ScreenOrientation"_fly_string, prototype);

    auto constructor = realm.create<ScreenOrientationConstructor>(realm);
    m_constructors.set("ScreenOrientation"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<StylePropertyMapPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<StylePropertyMapPrototype>(realm);
    m_prototypes.set("StylePropertyMap"_fly_string, prototype);

    auto constructor = realm.create<StylePropertyMapConstructor>(realm);
    m_constructors.set("StylePropertyMap"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<StylePropertyMapReadOnlyPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<StylePropertyMapReadOnlyPrototype>(realm);
    m_prototypes.set("StylePropertyMapReadOnly"_fly_string, prototype);

    auto constructor = realm.create<StylePropertyMapReadOnlyConstructor>(realm);
    m_constructors.set("StylePropertyMapReadOnly"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<StyleSheetPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<StyleSheetPrototype>(realm);
    m_prototypes.set("StyleSheet"_fly_string, prototype);

    auto constructor = realm.create<StyleSheetConstructor>(realm);
    m_constructors.set("StyleSheet"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<StyleSheetListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<StyleSheetListPrototype>(realm);
    m_prototypes.set("StyleSheetList"_fly_string, prototype);

    auto constructor = realm.create<StyleSheetListConstructor>(realm);
    m_constructors.set("StyleSheetList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TransitionEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TransitionEventPrototype>(realm);
    m_prototypes.set("TransitionEvent"_fly_string, prototype);

    auto constructor = realm.create<TransitionEventConstructor>(realm);
    m_constructors.set("TransitionEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<VisualViewportPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<VisualViewportPrototype>(realm);
    m_prototypes.set("VisualViewport"_fly_string, prototype);

    auto constructor = realm.create<VisualViewportConstructor>(realm);
    m_constructors.set("VisualViewport"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AbstractRangePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AbstractRangePrototype>(realm);
    m_prototypes.set("AbstractRange"_fly_string, prototype);

    auto constructor = realm.create<AbstractRangeConstructor>(realm);
    m_constructors.set("AbstractRange"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AttrPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AttrPrototype>(realm);
    m_prototypes.set("Attr"_fly_string, prototype);

    auto constructor = realm.create<AttrConstructor>(realm);
    m_constructors.set("Attr"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AbortControllerPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AbortControllerPrototype>(realm);
    m_prototypes.set("AbortController"_fly_string, prototype);

    auto constructor = realm.create<AbortControllerConstructor>(realm);
    m_constructors.set("AbortController"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AbortSignalPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AbortSignalPrototype>(realm);
    m_prototypes.set("AbortSignal"_fly_string, prototype);

    auto constructor = realm.create<AbortSignalConstructor>(realm);
    m_constructors.set("AbortSignal"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CDATASectionPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CDATASectionPrototype>(realm);
    m_prototypes.set("CDATASection"_fly_string, prototype);

    auto constructor = realm.create<CDATASectionConstructor>(realm);
    m_constructors.set("CDATASection"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CharacterDataPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CharacterDataPrototype>(realm);
    m_prototypes.set("CharacterData"_fly_string, prototype);

    auto constructor = realm.create<CharacterDataConstructor>(realm);
    m_constructors.set("CharacterData"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CommentPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CommentPrototype>(realm);
    m_prototypes.set("Comment"_fly_string, prototype);

    auto constructor = realm.create<CommentConstructor>(realm);
    m_constructors.set("Comment"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CustomEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CustomEventPrototype>(realm);
    m_prototypes.set("CustomEvent"_fly_string, prototype);

    auto constructor = realm.create<CustomEventConstructor>(realm);
    m_constructors.set("CustomEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DocumentPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DocumentPrototype>(realm);
    m_prototypes.set("Document"_fly_string, prototype);

    auto constructor = realm.create<DocumentConstructor>(realm);
    m_constructors.set("Document"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DocumentFragmentPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DocumentFragmentPrototype>(realm);
    m_prototypes.set("DocumentFragment"_fly_string, prototype);

    auto constructor = realm.create<DocumentFragmentConstructor>(realm);
    m_constructors.set("DocumentFragment"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DocumentTypePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DocumentTypePrototype>(realm);
    m_prototypes.set("DocumentType"_fly_string, prototype);

    auto constructor = realm.create<DocumentTypeConstructor>(realm);
    m_constructors.set("DocumentType"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DOMImplementationPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DOMImplementationPrototype>(realm);
    m_prototypes.set("DOMImplementation"_fly_string, prototype);

    auto constructor = realm.create<DOMImplementationConstructor>(realm);
    m_constructors.set("DOMImplementation"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DOMTokenListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DOMTokenListPrototype>(realm);
    m_prototypes.set("DOMTokenList"_fly_string, prototype);

    auto constructor = realm.create<DOMTokenListConstructor>(realm);
    m_constructors.set("DOMTokenList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ElementPrototype>(realm);
    m_prototypes.set("Element"_fly_string, prototype);

    auto constructor = realm.create<ElementConstructor>(realm);
    m_constructors.set("Element"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<EventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<EventPrototype>(realm);
    m_prototypes.set("Event"_fly_string, prototype);

    auto constructor = realm.create<EventConstructor>(realm);
    m_constructors.set("Event"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<EventTargetPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<EventTargetPrototype>(realm);
    m_prototypes.set("EventTarget"_fly_string, prototype);

    auto constructor = realm.create<EventTargetConstructor>(realm);
    m_constructors.set("EventTarget"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLCollectionPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLCollectionPrototype>(realm);
    m_prototypes.set("HTMLCollection"_fly_string, prototype);

    auto constructor = realm.create<HTMLCollectionConstructor>(realm);
    m_constructors.set("HTMLCollection"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MutationObserverPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MutationObserverPrototype>(realm);
    m_prototypes.set("MutationObserver"_fly_string, prototype);

    auto constructor = realm.create<MutationObserverConstructor>(realm);
    m_constructors.set("MutationObserver"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MutationRecordPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MutationRecordPrototype>(realm);
    m_prototypes.set("MutationRecord"_fly_string, prototype);

    auto constructor = realm.create<MutationRecordConstructor>(realm);
    m_constructors.set("MutationRecord"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<NamedNodeMapPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<NamedNodeMapPrototype>(realm);
    m_prototypes.set("NamedNodeMap"_fly_string, prototype);

    auto constructor = realm.create<NamedNodeMapConstructor>(realm);
    m_constructors.set("NamedNodeMap"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<NodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<NodePrototype>(realm);
    m_prototypes.set("Node"_fly_string, prototype);

    auto constructor = realm.create<NodeConstructor>(realm);
    m_constructors.set("Node"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<NodeFilterPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<NodeFilterPrototype>(realm);
    m_prototypes.set("NodeFilter"_fly_string, prototype);

    auto constructor = realm.create<NodeFilterConstructor>(realm);
    m_constructors.set("NodeFilter"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<NodeIteratorPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<NodeIteratorPrototype>(realm);
    m_prototypes.set("NodeIterator"_fly_string, prototype);

    auto constructor = realm.create<NodeIteratorConstructor>(realm);
    m_constructors.set("NodeIterator"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<NodeListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<NodeListPrototype>(realm);
    m_prototypes.set("NodeList"_fly_string, prototype);

    auto constructor = realm.create<NodeListConstructor>(realm);
    m_constructors.set("NodeList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ProcessingInstructionPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ProcessingInstructionPrototype>(realm);
    m_prototypes.set("ProcessingInstruction"_fly_string, prototype);

    auto constructor = realm.create<ProcessingInstructionConstructor>(realm);
    m_constructors.set("ProcessingInstruction"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<RangePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<RangePrototype>(realm);
    m_prototypes.set("Range"_fly_string, prototype);

    auto constructor = realm.create<RangeConstructor>(realm);
    m_constructors.set("Range"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ShadowRootPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ShadowRootPrototype>(realm);
    m_prototypes.set("ShadowRoot"_fly_string, prototype);

    auto constructor = realm.create<ShadowRootConstructor>(realm);
    m_constructors.set("ShadowRoot"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<StaticRangePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<StaticRangePrototype>(realm);
    m_prototypes.set("StaticRange"_fly_string, prototype);

    auto constructor = realm.create<StaticRangeConstructor>(realm);
    m_constructors.set("StaticRange"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TextPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TextPrototype>(realm);
    m_prototypes.set("Text"_fly_string, prototype);

    auto constructor = realm.create<TextConstructor>(realm);
    m_constructors.set("Text"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TreeWalkerPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TreeWalkerPrototype>(realm);
    m_prototypes.set("TreeWalker"_fly_string, prototype);

    auto constructor = realm.create<TreeWalkerConstructor>(realm);
    m_constructors.set("TreeWalker"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<XMLDocumentPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<XMLDocumentPrototype>(realm);
    m_prototypes.set("XMLDocument"_fly_string, prototype);

    auto constructor = realm.create<XMLDocumentConstructor>(realm);
    m_constructors.set("XMLDocument"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DOMURLPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DOMURLPrototype>(realm);
    m_prototypes.set("URL"_fly_string, prototype);

    auto constructor = realm.create<DOMURLConstructor>(realm);
    m_constructors.set("URL"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<OriginPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<OriginPrototype>(realm);
    m_prototypes.set("Origin"_fly_string, prototype);

    auto constructor = realm.create<OriginConstructor>(realm);
    m_constructors.set("Origin"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<URLSearchParamsPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<URLSearchParamsPrototype>(realm);
    m_prototypes.set("URLSearchParams"_fly_string, prototype);

    auto constructor = realm.create<URLSearchParamsConstructor>(realm);
    m_constructors.set("URLSearchParams"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TextDecoderPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TextDecoderPrototype>(realm);
    m_prototypes.set("TextDecoder"_fly_string, prototype);

    auto constructor = realm.create<TextDecoderConstructor>(realm);
    m_constructors.set("TextDecoder"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TextEncoderPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TextEncoderPrototype>(realm);
    m_prototypes.set("TextEncoder"_fly_string, prototype);

    auto constructor = realm.create<TextEncoderConstructor>(realm);
    m_constructors.set("TextEncoder"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TextEncoderStreamPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TextEncoderStreamPrototype>(realm);
    m_prototypes.set("TextEncoderStream"_fly_string, prototype);

    auto constructor = realm.create<TextEncoderStreamConstructor>(realm);
    m_constructors.set("TextEncoderStream"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MediaKeySystemAccessPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MediaKeySystemAccessPrototype>(realm);
    m_prototypes.set("MediaKeySystemAccess"_fly_string, prototype);

    auto constructor = realm.create<MediaKeySystemAccessConstructor>(realm);
    m_constructors.set("MediaKeySystemAccess"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<FileSystemEntryPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<FileSystemEntryPrototype>(realm);
    m_prototypes.set("FileSystemEntry"_fly_string, prototype);

    auto constructor = realm.create<FileSystemEntryConstructor>(realm);
    m_constructors.set("FileSystemEntry"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PerformanceEventTimingPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PerformanceEventTimingPrototype>(realm);
    m_prototypes.set("PerformanceEventTiming"_fly_string, prototype);

    auto constructor = realm.create<PerformanceEventTimingConstructor>(realm);
    m_constructors.set("PerformanceEventTiming"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HeadersPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HeadersPrototype>(realm);
    m_prototypes.set("Headers"_fly_string, prototype);

    auto constructor = realm.create<HeadersConstructor>(realm);
    m_constructors.set("Headers"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<RequestPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<RequestPrototype>(realm);
    m_prototypes.set("Request"_fly_string, prototype);

    auto constructor = realm.create<RequestConstructor>(realm);
    m_constructors.set("Request"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ResponsePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ResponsePrototype>(realm);
    m_prototypes.set("Response"_fly_string, prototype);

    auto constructor = realm.create<ResponseConstructor>(realm);
    m_constructors.set("Response"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<BlobPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<BlobPrototype>(realm);
    m_prototypes.set("Blob"_fly_string, prototype);

    auto constructor = realm.create<BlobConstructor>(realm);
    m_constructors.set("Blob"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<FilePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<FilePrototype>(realm);
    m_prototypes.set("File"_fly_string, prototype);

    auto constructor = realm.create<FileConstructor>(realm);
    m_constructors.set("File"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<FileListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<FileListPrototype>(realm);
    m_prototypes.set("FileList"_fly_string, prototype);

    auto constructor = realm.create<FileListConstructor>(realm);
    m_constructors.set("FileList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<FileReaderPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<FileReaderPrototype>(realm);
    m_prototypes.set("FileReader"_fly_string, prototype);

    auto constructor = realm.create<FileReaderConstructor>(realm);
    m_constructors.set("FileReader"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<FileReaderSyncPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<FileReaderSyncPrototype>(realm);
    m_prototypes.set("FileReaderSync"_fly_string, prototype);

    auto constructor = realm.create<FileReaderSyncConstructor>(realm);
    m_constructors.set("FileReaderSync"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<GamepadPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<GamepadPrototype>(realm);
    m_prototypes.set("Gamepad"_fly_string, prototype);

    auto constructor = realm.create<GamepadConstructor>(realm);
    m_constructors.set("Gamepad"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<GamepadButtonPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<GamepadButtonPrototype>(realm);
    m_prototypes.set("GamepadButton"_fly_string, prototype);

    auto constructor = realm.create<GamepadButtonConstructor>(realm);
    m_constructors.set("GamepadButton"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<GamepadEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<GamepadEventPrototype>(realm);
    m_prototypes.set("GamepadEvent"_fly_string, prototype);

    auto constructor = realm.create<GamepadEventConstructor>(realm);
    m_constructors.set("GamepadEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<GamepadHapticActuatorPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<GamepadHapticActuatorPrototype>(realm);
    m_prototypes.set("GamepadHapticActuator"_fly_string, prototype);

    auto constructor = realm.create<GamepadHapticActuatorConstructor>(realm);
    m_constructors.set("GamepadHapticActuator"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<GeolocationPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<GeolocationPrototype>(realm);
    m_prototypes.set("Geolocation"_fly_string, prototype);

    auto constructor = realm.create<GeolocationConstructor>(realm);
    m_constructors.set("Geolocation"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<GeolocationCoordinatesPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<GeolocationCoordinatesPrototype>(realm);
    m_prototypes.set("GeolocationCoordinates"_fly_string, prototype);

    auto constructor = realm.create<GeolocationCoordinatesConstructor>(realm);
    m_constructors.set("GeolocationCoordinates"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<GeolocationPositionPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<GeolocationPositionPrototype>(realm);
    m_prototypes.set("GeolocationPosition"_fly_string, prototype);

    auto constructor = realm.create<GeolocationPositionConstructor>(realm);
    m_constructors.set("GeolocationPosition"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<GeolocationPositionErrorPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<GeolocationPositionErrorPrototype>(realm);
    m_prototypes.set("GeolocationPositionError"_fly_string, prototype);

    auto constructor = realm.create<GeolocationPositionErrorConstructor>(realm);
    m_constructors.set("GeolocationPositionError"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DOMMatrixPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DOMMatrixPrototype>(realm);
    m_prototypes.set("DOMMatrix"_fly_string, prototype);

    auto constructor = realm.create<DOMMatrixConstructor>(realm);
    m_constructors.set("DOMMatrix"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DOMMatrixReadOnlyPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DOMMatrixReadOnlyPrototype>(realm);
    m_prototypes.set("DOMMatrixReadOnly"_fly_string, prototype);

    auto constructor = realm.create<DOMMatrixReadOnlyConstructor>(realm);
    m_constructors.set("DOMMatrixReadOnly"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DOMPointPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DOMPointPrototype>(realm);
    m_prototypes.set("DOMPoint"_fly_string, prototype);

    auto constructor = realm.create<DOMPointConstructor>(realm);
    m_constructors.set("DOMPoint"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DOMPointReadOnlyPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DOMPointReadOnlyPrototype>(realm);
    m_prototypes.set("DOMPointReadOnly"_fly_string, prototype);

    auto constructor = realm.create<DOMPointReadOnlyConstructor>(realm);
    m_constructors.set("DOMPointReadOnly"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DOMQuadPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DOMQuadPrototype>(realm);
    m_prototypes.set("DOMQuad"_fly_string, prototype);

    auto constructor = realm.create<DOMQuadConstructor>(realm);
    m_constructors.set("DOMQuad"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DOMRectPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DOMRectPrototype>(realm);
    m_prototypes.set("DOMRect"_fly_string, prototype);

    auto constructor = realm.create<DOMRectConstructor>(realm);
    m_constructors.set("DOMRect"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DOMRectListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DOMRectListPrototype>(realm);
    m_prototypes.set("DOMRectList"_fly_string, prototype);

    auto constructor = realm.create<DOMRectListConstructor>(realm);
    m_constructors.set("DOMRectList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DOMRectReadOnlyPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DOMRectReadOnlyPrototype>(realm);
    m_prototypes.set("DOMRectReadOnly"_fly_string, prototype);

    auto constructor = realm.create<DOMRectReadOnlyConstructor>(realm);
    m_constructors.set("DOMRectReadOnly"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AudioTrackPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AudioTrackPrototype>(realm);
    m_prototypes.set("AudioTrack"_fly_string, prototype);

    auto constructor = realm.create<AudioTrackConstructor>(realm);
    m_constructors.set("AudioTrack"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AudioTrackListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AudioTrackListPrototype>(realm);
    m_prototypes.set("AudioTrackList"_fly_string, prototype);

    auto constructor = realm.create<AudioTrackListConstructor>(realm);
    m_constructors.set("AudioTrackList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<BarPropPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<BarPropPrototype>(realm);
    m_prototypes.set("BarProp"_fly_string, prototype);

    auto constructor = realm.create<BarPropConstructor>(realm);
    m_constructors.set("BarProp"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<BeforeUnloadEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<BeforeUnloadEventPrototype>(realm);
    m_prototypes.set("BeforeUnloadEvent"_fly_string, prototype);

    auto constructor = realm.create<BeforeUnloadEventConstructor>(realm);
    m_constructors.set("BeforeUnloadEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<BroadcastChannelPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<BroadcastChannelPrototype>(realm);
    m_prototypes.set("BroadcastChannel"_fly_string, prototype);

    auto constructor = realm.create<BroadcastChannelConstructor>(realm);
    m_constructors.set("BroadcastChannel"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CanvasGradientPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CanvasGradientPrototype>(realm);
    m_prototypes.set("CanvasGradient"_fly_string, prototype);

    auto constructor = realm.create<CanvasGradientConstructor>(realm);
    m_constructors.set("CanvasGradient"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CanvasPatternPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CanvasPatternPrototype>(realm);
    m_prototypes.set("CanvasPattern"_fly_string, prototype);

    auto constructor = realm.create<CanvasPatternConstructor>(realm);
    m_constructors.set("CanvasPattern"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CanvasRenderingContext2DPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CanvasRenderingContext2DPrototype>(realm);
    m_prototypes.set("CanvasRenderingContext2D"_fly_string, prototype);

    auto constructor = realm.create<CanvasRenderingContext2DConstructor>(realm);
    m_constructors.set("CanvasRenderingContext2D"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CloseEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CloseEventPrototype>(realm);
    m_prototypes.set("CloseEvent"_fly_string, prototype);

    auto constructor = realm.create<CloseEventConstructor>(realm);
    m_constructors.set("CloseEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CloseWatcherPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CloseWatcherPrototype>(realm);
    m_prototypes.set("CloseWatcher"_fly_string, prototype);

    auto constructor = realm.create<CloseWatcherConstructor>(realm);
    m_constructors.set("CloseWatcher"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CommandEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CommandEventPrototype>(realm);
    m_prototypes.set("CommandEvent"_fly_string, prototype);

    auto constructor = realm.create<CommandEventConstructor>(realm);
    m_constructors.set("CommandEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CustomElementRegistryPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CustomElementRegistryPrototype>(realm);
    m_prototypes.set("CustomElementRegistry"_fly_string, prototype);

    auto constructor = realm.create<CustomElementRegistryConstructor>(realm);
    m_constructors.set("CustomElementRegistry"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CustomStateSetPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CustomStateSetPrototype>(realm);
    m_prototypes.set("CustomStateSet"_fly_string, prototype);

    auto constructor = realm.create<CustomStateSetConstructor>(realm);
    m_constructors.set("CustomStateSet"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DataTransferPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DataTransferPrototype>(realm);
    m_prototypes.set("DataTransfer"_fly_string, prototype);

    auto constructor = realm.create<DataTransferConstructor>(realm);
    m_constructors.set("DataTransfer"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DataTransferItemPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DataTransferItemPrototype>(realm);
    m_prototypes.set("DataTransferItem"_fly_string, prototype);

    auto constructor = realm.create<DataTransferItemConstructor>(realm);
    m_constructors.set("DataTransferItem"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DataTransferItemListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DataTransferItemListPrototype>(realm);
    m_prototypes.set("DataTransferItemList"_fly_string, prototype);

    auto constructor = realm.create<DataTransferItemListConstructor>(realm);
    m_constructors.set("DataTransferItemList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DedicatedWorkerGlobalScopePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DedicatedWorkerGlobalScopePrototype>(realm);
    m_prototypes.set("DedicatedWorkerGlobalScope"_fly_string, prototype);

    auto constructor = realm.create<DedicatedWorkerGlobalScopeConstructor>(realm);
    m_constructors.set("DedicatedWorkerGlobalScope"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DOMParserPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DOMParserPrototype>(realm);
    m_prototypes.set("DOMParser"_fly_string, prototype);

    auto constructor = realm.create<DOMParserConstructor>(realm);
    m_constructors.set("DOMParser"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DOMStringListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DOMStringListPrototype>(realm);
    m_prototypes.set("DOMStringList"_fly_string, prototype);

    auto constructor = realm.create<DOMStringListConstructor>(realm);
    m_constructors.set("DOMStringList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DOMStringMapPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DOMStringMapPrototype>(realm);
    m_prototypes.set("DOMStringMap"_fly_string, prototype);

    auto constructor = realm.create<DOMStringMapConstructor>(realm);
    m_constructors.set("DOMStringMap"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DragEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DragEventPrototype>(realm);
    m_prototypes.set("DragEvent"_fly_string, prototype);

    auto constructor = realm.create<DragEventConstructor>(realm);
    m_constructors.set("DragEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ElementInternalsPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ElementInternalsPrototype>(realm);
    m_prototypes.set("ElementInternals"_fly_string, prototype);

    auto constructor = realm.create<ElementInternalsConstructor>(realm);
    m_constructors.set("ElementInternals"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ErrorEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ErrorEventPrototype>(realm);
    m_prototypes.set("ErrorEvent"_fly_string, prototype);

    auto constructor = realm.create<ErrorEventConstructor>(realm);
    m_constructors.set("ErrorEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<EventSourcePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<EventSourcePrototype>(realm);
    m_prototypes.set("EventSource"_fly_string, prototype);

    auto constructor = realm.create<EventSourceConstructor>(realm);
    m_constructors.set("EventSource"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ExternalPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ExternalPrototype>(realm);
    m_prototypes.set("External"_fly_string, prototype);

    auto constructor = realm.create<ExternalConstructor>(realm);
    m_constructors.set("External"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<FormDataEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<FormDataEventPrototype>(realm);
    m_prototypes.set("FormDataEvent"_fly_string, prototype);

    auto constructor = realm.create<FormDataEventConstructor>(realm);
    m_constructors.set("FormDataEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HashChangeEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HashChangeEventPrototype>(realm);
    m_prototypes.set("HashChangeEvent"_fly_string, prototype);

    auto constructor = realm.create<HashChangeEventConstructor>(realm);
    m_constructors.set("HashChangeEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HistoryPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HistoryPrototype>(realm);
    m_prototypes.set("History"_fly_string, prototype);

    auto constructor = realm.create<HistoryConstructor>(realm);
    m_constructors.set("History"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLAllCollectionPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLAllCollectionPrototype>(realm);
    m_prototypes.set("HTMLAllCollection"_fly_string, prototype);

    auto constructor = realm.create<HTMLAllCollectionConstructor>(realm);
    m_constructors.set("HTMLAllCollection"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLAnchorElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLAnchorElementPrototype>(realm);
    m_prototypes.set("HTMLAnchorElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLAnchorElementConstructor>(realm);
    m_constructors.set("HTMLAnchorElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLAreaElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLAreaElementPrototype>(realm);
    m_prototypes.set("HTMLAreaElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLAreaElementConstructor>(realm);
    m_constructors.set("HTMLAreaElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLAudioElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLAudioElementPrototype>(realm);
    m_prototypes.set("HTMLAudioElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLAudioElementConstructor>(realm);
    m_constructors.set("HTMLAudioElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

    auto legacy_constructor = realm.create<AudioConstructor>(realm);
    m_constructors.set("Audio"_fly_string, legacy_constructor);
}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLBaseElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLBaseElementPrototype>(realm);
    m_prototypes.set("HTMLBaseElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLBaseElementConstructor>(realm);
    m_constructors.set("HTMLBaseElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLBodyElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLBodyElementPrototype>(realm);
    m_prototypes.set("HTMLBodyElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLBodyElementConstructor>(realm);
    m_constructors.set("HTMLBodyElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLBRElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLBRElementPrototype>(realm);
    m_prototypes.set("HTMLBRElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLBRElementConstructor>(realm);
    m_constructors.set("HTMLBRElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLButtonElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLButtonElementPrototype>(realm);
    m_prototypes.set("HTMLButtonElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLButtonElementConstructor>(realm);
    m_constructors.set("HTMLButtonElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLCanvasElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLCanvasElementPrototype>(realm);
    m_prototypes.set("HTMLCanvasElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLCanvasElementConstructor>(realm);
    m_constructors.set("HTMLCanvasElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLDataElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLDataElementPrototype>(realm);
    m_prototypes.set("HTMLDataElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLDataElementConstructor>(realm);
    m_constructors.set("HTMLDataElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLDataListElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLDataListElementPrototype>(realm);
    m_prototypes.set("HTMLDataListElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLDataListElementConstructor>(realm);
    m_constructors.set("HTMLDataListElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLDetailsElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLDetailsElementPrototype>(realm);
    m_prototypes.set("HTMLDetailsElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLDetailsElementConstructor>(realm);
    m_constructors.set("HTMLDetailsElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLDialogElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLDialogElementPrototype>(realm);
    m_prototypes.set("HTMLDialogElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLDialogElementConstructor>(realm);
    m_constructors.set("HTMLDialogElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLDirectoryElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLDirectoryElementPrototype>(realm);
    m_prototypes.set("HTMLDirectoryElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLDirectoryElementConstructor>(realm);
    m_constructors.set("HTMLDirectoryElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLDivElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLDivElementPrototype>(realm);
    m_prototypes.set("HTMLDivElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLDivElementConstructor>(realm);
    m_constructors.set("HTMLDivElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLDocumentPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLDocumentPrototype>(realm);
    m_prototypes.set("HTMLDocument"_fly_string, prototype);

    auto constructor = realm.create<HTMLDocumentConstructor>(realm);
    m_constructors.set("HTMLDocument"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLDListElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLDListElementPrototype>(realm);
    m_prototypes.set("HTMLDListElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLDListElementConstructor>(realm);
    m_constructors.set("HTMLDListElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLElementPrototype>(realm);
    m_prototypes.set("HTMLElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLElementConstructor>(realm);
    m_constructors.set("HTMLElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLEmbedElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLEmbedElementPrototype>(realm);
    m_prototypes.set("HTMLEmbedElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLEmbedElementConstructor>(realm);
    m_constructors.set("HTMLEmbedElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLFieldSetElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLFieldSetElementPrototype>(realm);
    m_prototypes.set("HTMLFieldSetElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLFieldSetElementConstructor>(realm);
    m_constructors.set("HTMLFieldSetElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLFontElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLFontElementPrototype>(realm);
    m_prototypes.set("HTMLFontElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLFontElementConstructor>(realm);
    m_constructors.set("HTMLFontElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLFormControlsCollectionPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLFormControlsCollectionPrototype>(realm);
    m_prototypes.set("HTMLFormControlsCollection"_fly_string, prototype);

    auto constructor = realm.create<HTMLFormControlsCollectionConstructor>(realm);
    m_constructors.set("HTMLFormControlsCollection"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLFormElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLFormElementPrototype>(realm);
    m_prototypes.set("HTMLFormElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLFormElementConstructor>(realm);
    m_constructors.set("HTMLFormElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLFrameElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLFrameElementPrototype>(realm);
    m_prototypes.set("HTMLFrameElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLFrameElementConstructor>(realm);
    m_constructors.set("HTMLFrameElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLFrameSetElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLFrameSetElementPrototype>(realm);
    m_prototypes.set("HTMLFrameSetElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLFrameSetElementConstructor>(realm);
    m_constructors.set("HTMLFrameSetElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLHeadElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLHeadElementPrototype>(realm);
    m_prototypes.set("HTMLHeadElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLHeadElementConstructor>(realm);
    m_constructors.set("HTMLHeadElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLHeadingElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLHeadingElementPrototype>(realm);
    m_prototypes.set("HTMLHeadingElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLHeadingElementConstructor>(realm);
    m_constructors.set("HTMLHeadingElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLHRElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLHRElementPrototype>(realm);
    m_prototypes.set("HTMLHRElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLHRElementConstructor>(realm);
    m_constructors.set("HTMLHRElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLHtmlElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLHtmlElementPrototype>(realm);
    m_prototypes.set("HTMLHtmlElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLHtmlElementConstructor>(realm);
    m_constructors.set("HTMLHtmlElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLIFrameElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLIFrameElementPrototype>(realm);
    m_prototypes.set("HTMLIFrameElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLIFrameElementConstructor>(realm);
    m_constructors.set("HTMLIFrameElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLImageElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLImageElementPrototype>(realm);
    m_prototypes.set("HTMLImageElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLImageElementConstructor>(realm);
    m_constructors.set("HTMLImageElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

    auto legacy_constructor = realm.create<ImageConstructor>(realm);
    m_constructors.set("Image"_fly_string, legacy_constructor);
}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLInputElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLInputElementPrototype>(realm);
    m_prototypes.set("HTMLInputElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLInputElementConstructor>(realm);
    m_constructors.set("HTMLInputElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLLabelElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLLabelElementPrototype>(realm);
    m_prototypes.set("HTMLLabelElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLLabelElementConstructor>(realm);
    m_constructors.set("HTMLLabelElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLLegendElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLLegendElementPrototype>(realm);
    m_prototypes.set("HTMLLegendElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLLegendElementConstructor>(realm);
    m_constructors.set("HTMLLegendElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLLIElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLLIElementPrototype>(realm);
    m_prototypes.set("HTMLLIElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLLIElementConstructor>(realm);
    m_constructors.set("HTMLLIElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLLinkElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLLinkElementPrototype>(realm);
    m_prototypes.set("HTMLLinkElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLLinkElementConstructor>(realm);
    m_constructors.set("HTMLLinkElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLMapElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLMapElementPrototype>(realm);
    m_prototypes.set("HTMLMapElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLMapElementConstructor>(realm);
    m_constructors.set("HTMLMapElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLMarqueeElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLMarqueeElementPrototype>(realm);
    m_prototypes.set("HTMLMarqueeElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLMarqueeElementConstructor>(realm);
    m_constructors.set("HTMLMarqueeElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLMediaElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLMediaElementPrototype>(realm);
    m_prototypes.set("HTMLMediaElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLMediaElementConstructor>(realm);
    m_constructors.set("HTMLMediaElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLMenuElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLMenuElementPrototype>(realm);
    m_prototypes.set("HTMLMenuElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLMenuElementConstructor>(realm);
    m_constructors.set("HTMLMenuElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLMetaElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLMetaElementPrototype>(realm);
    m_prototypes.set("HTMLMetaElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLMetaElementConstructor>(realm);
    m_constructors.set("HTMLMetaElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLMeterElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLMeterElementPrototype>(realm);
    m_prototypes.set("HTMLMeterElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLMeterElementConstructor>(realm);
    m_constructors.set("HTMLMeterElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLModElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLModElementPrototype>(realm);
    m_prototypes.set("HTMLModElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLModElementConstructor>(realm);
    m_constructors.set("HTMLModElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLObjectElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLObjectElementPrototype>(realm);
    m_prototypes.set("HTMLObjectElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLObjectElementConstructor>(realm);
    m_constructors.set("HTMLObjectElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLOListElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLOListElementPrototype>(realm);
    m_prototypes.set("HTMLOListElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLOListElementConstructor>(realm);
    m_constructors.set("HTMLOListElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLOptGroupElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLOptGroupElementPrototype>(realm);
    m_prototypes.set("HTMLOptGroupElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLOptGroupElementConstructor>(realm);
    m_constructors.set("HTMLOptGroupElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLOptionElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLOptionElementPrototype>(realm);
    m_prototypes.set("HTMLOptionElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLOptionElementConstructor>(realm);
    m_constructors.set("HTMLOptionElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

    auto legacy_constructor = realm.create<OptionConstructor>(realm);
    m_constructors.set("Option"_fly_string, legacy_constructor);
}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLOptionsCollectionPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLOptionsCollectionPrototype>(realm);
    m_prototypes.set("HTMLOptionsCollection"_fly_string, prototype);

    auto constructor = realm.create<HTMLOptionsCollectionConstructor>(realm);
    m_constructors.set("HTMLOptionsCollection"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLOutputElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLOutputElementPrototype>(realm);
    m_prototypes.set("HTMLOutputElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLOutputElementConstructor>(realm);
    m_constructors.set("HTMLOutputElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLParagraphElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLParagraphElementPrototype>(realm);
    m_prototypes.set("HTMLParagraphElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLParagraphElementConstructor>(realm);
    m_constructors.set("HTMLParagraphElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLParamElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLParamElementPrototype>(realm);
    m_prototypes.set("HTMLParamElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLParamElementConstructor>(realm);
    m_constructors.set("HTMLParamElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLPictureElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLPictureElementPrototype>(realm);
    m_prototypes.set("HTMLPictureElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLPictureElementConstructor>(realm);
    m_constructors.set("HTMLPictureElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLPreElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLPreElementPrototype>(realm);
    m_prototypes.set("HTMLPreElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLPreElementConstructor>(realm);
    m_constructors.set("HTMLPreElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLProgressElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLProgressElementPrototype>(realm);
    m_prototypes.set("HTMLProgressElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLProgressElementConstructor>(realm);
    m_constructors.set("HTMLProgressElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLQuoteElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLQuoteElementPrototype>(realm);
    m_prototypes.set("HTMLQuoteElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLQuoteElementConstructor>(realm);
    m_constructors.set("HTMLQuoteElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLScriptElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLScriptElementPrototype>(realm);
    m_prototypes.set("HTMLScriptElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLScriptElementConstructor>(realm);
    m_constructors.set("HTMLScriptElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLSelectedContentElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLSelectedContentElementPrototype>(realm);
    m_prototypes.set("HTMLSelectedContentElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLSelectedContentElementConstructor>(realm);
    m_constructors.set("HTMLSelectedContentElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLSelectElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLSelectElementPrototype>(realm);
    m_prototypes.set("HTMLSelectElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLSelectElementConstructor>(realm);
    m_constructors.set("HTMLSelectElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLSlotElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLSlotElementPrototype>(realm);
    m_prototypes.set("HTMLSlotElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLSlotElementConstructor>(realm);
    m_constructors.set("HTMLSlotElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLSourceElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLSourceElementPrototype>(realm);
    m_prototypes.set("HTMLSourceElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLSourceElementConstructor>(realm);
    m_constructors.set("HTMLSourceElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLSpanElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLSpanElementPrototype>(realm);
    m_prototypes.set("HTMLSpanElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLSpanElementConstructor>(realm);
    m_constructors.set("HTMLSpanElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLStyleElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLStyleElementPrototype>(realm);
    m_prototypes.set("HTMLStyleElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLStyleElementConstructor>(realm);
    m_constructors.set("HTMLStyleElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLTableCaptionElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLTableCaptionElementPrototype>(realm);
    m_prototypes.set("HTMLTableCaptionElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLTableCaptionElementConstructor>(realm);
    m_constructors.set("HTMLTableCaptionElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLTableCellElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLTableCellElementPrototype>(realm);
    m_prototypes.set("HTMLTableCellElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLTableCellElementConstructor>(realm);
    m_constructors.set("HTMLTableCellElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLTableColElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLTableColElementPrototype>(realm);
    m_prototypes.set("HTMLTableColElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLTableColElementConstructor>(realm);
    m_constructors.set("HTMLTableColElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLTableElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLTableElementPrototype>(realm);
    m_prototypes.set("HTMLTableElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLTableElementConstructor>(realm);
    m_constructors.set("HTMLTableElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLTableRowElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLTableRowElementPrototype>(realm);
    m_prototypes.set("HTMLTableRowElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLTableRowElementConstructor>(realm);
    m_constructors.set("HTMLTableRowElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLTableSectionElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLTableSectionElementPrototype>(realm);
    m_prototypes.set("HTMLTableSectionElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLTableSectionElementConstructor>(realm);
    m_constructors.set("HTMLTableSectionElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLTemplateElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLTemplateElementPrototype>(realm);
    m_prototypes.set("HTMLTemplateElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLTemplateElementConstructor>(realm);
    m_constructors.set("HTMLTemplateElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLTextAreaElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLTextAreaElementPrototype>(realm);
    m_prototypes.set("HTMLTextAreaElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLTextAreaElementConstructor>(realm);
    m_constructors.set("HTMLTextAreaElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLTimeElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLTimeElementPrototype>(realm);
    m_prototypes.set("HTMLTimeElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLTimeElementConstructor>(realm);
    m_constructors.set("HTMLTimeElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLTitleElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLTitleElementPrototype>(realm);
    m_prototypes.set("HTMLTitleElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLTitleElementConstructor>(realm);
    m_constructors.set("HTMLTitleElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLTrackElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLTrackElementPrototype>(realm);
    m_prototypes.set("HTMLTrackElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLTrackElementConstructor>(realm);
    m_constructors.set("HTMLTrackElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLUListElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLUListElementPrototype>(realm);
    m_prototypes.set("HTMLUListElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLUListElementConstructor>(realm);
    m_constructors.set("HTMLUListElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLUnknownElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLUnknownElementPrototype>(realm);
    m_prototypes.set("HTMLUnknownElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLUnknownElementConstructor>(realm);
    m_constructors.set("HTMLUnknownElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<HTMLVideoElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<HTMLVideoElementPrototype>(realm);
    m_prototypes.set("HTMLVideoElement"_fly_string, prototype);

    auto constructor = realm.create<HTMLVideoElementConstructor>(realm);
    m_constructors.set("HTMLVideoElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ImageBitmapPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ImageBitmapPrototype>(realm);
    m_prototypes.set("ImageBitmap"_fly_string, prototype);

    auto constructor = realm.create<ImageBitmapConstructor>(realm);
    m_constructors.set("ImageBitmap"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ImageDataPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ImageDataPrototype>(realm);
    m_prototypes.set("ImageData"_fly_string, prototype);

    auto constructor = realm.create<ImageDataConstructor>(realm);
    m_constructors.set("ImageData"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<LocationPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<LocationPrototype>(realm);
    m_prototypes.set("Location"_fly_string, prototype);

    auto constructor = realm.create<LocationConstructor>(realm);
    m_constructors.set("Location"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MediaErrorPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MediaErrorPrototype>(realm);
    m_prototypes.set("MediaError"_fly_string, prototype);

    auto constructor = realm.create<MediaErrorConstructor>(realm);
    m_constructors.set("MediaError"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MessageChannelPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MessageChannelPrototype>(realm);
    m_prototypes.set("MessageChannel"_fly_string, prototype);

    auto constructor = realm.create<MessageChannelConstructor>(realm);
    m_constructors.set("MessageChannel"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MessageEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MessageEventPrototype>(realm);
    m_prototypes.set("MessageEvent"_fly_string, prototype);

    auto constructor = realm.create<MessageEventConstructor>(realm);
    m_constructors.set("MessageEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MessagePortPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MessagePortPrototype>(realm);
    m_prototypes.set("MessagePort"_fly_string, prototype);

    auto constructor = realm.create<MessagePortConstructor>(realm);
    m_constructors.set("MessagePort"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MimeTypePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MimeTypePrototype>(realm);
    m_prototypes.set("MimeType"_fly_string, prototype);

    auto constructor = realm.create<MimeTypeConstructor>(realm);
    m_constructors.set("MimeType"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MimeTypeArrayPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MimeTypeArrayPrototype>(realm);
    m_prototypes.set("MimeTypeArray"_fly_string, prototype);

    auto constructor = realm.create<MimeTypeArrayConstructor>(realm);
    m_constructors.set("MimeTypeArray"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<NavigateEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<NavigateEventPrototype>(realm);
    m_prototypes.set("NavigateEvent"_fly_string, prototype);

    auto constructor = realm.create<NavigateEventConstructor>(realm);
    m_constructors.set("NavigateEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<NavigationPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<NavigationPrototype>(realm);
    m_prototypes.set("Navigation"_fly_string, prototype);

    auto constructor = realm.create<NavigationConstructor>(realm);
    m_constructors.set("Navigation"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<NavigationCurrentEntryChangeEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<NavigationCurrentEntryChangeEventPrototype>(realm);
    m_prototypes.set("NavigationCurrentEntryChangeEvent"_fly_string, prototype);

    auto constructor = realm.create<NavigationCurrentEntryChangeEventConstructor>(realm);
    m_constructors.set("NavigationCurrentEntryChangeEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<NavigationDestinationPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<NavigationDestinationPrototype>(realm);
    m_prototypes.set("NavigationDestination"_fly_string, prototype);

    auto constructor = realm.create<NavigationDestinationConstructor>(realm);
    m_constructors.set("NavigationDestination"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<NavigationHistoryEntryPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<NavigationHistoryEntryPrototype>(realm);
    m_prototypes.set("NavigationHistoryEntry"_fly_string, prototype);

    auto constructor = realm.create<NavigationHistoryEntryConstructor>(realm);
    m_constructors.set("NavigationHistoryEntry"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<NavigationTransitionPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<NavigationTransitionPrototype>(realm);
    m_prototypes.set("NavigationTransition"_fly_string, prototype);

    auto constructor = realm.create<NavigationTransitionConstructor>(realm);
    m_constructors.set("NavigationTransition"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<NavigatorPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<NavigatorPrototype>(realm);
    m_prototypes.set("Navigator"_fly_string, prototype);

    auto constructor = realm.create<NavigatorConstructor>(realm);
    m_constructors.set("Navigator"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<OffscreenCanvasPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<OffscreenCanvasPrototype>(realm);
    m_prototypes.set("OffscreenCanvas"_fly_string, prototype);

    auto constructor = realm.create<OffscreenCanvasConstructor>(realm);
    m_constructors.set("OffscreenCanvas"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<OffscreenCanvasRenderingContext2DPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<OffscreenCanvasRenderingContext2DPrototype>(realm);
    m_prototypes.set("OffscreenCanvasRenderingContext2D"_fly_string, prototype);

    auto constructor = realm.create<OffscreenCanvasRenderingContext2DConstructor>(realm);
    m_constructors.set("OffscreenCanvasRenderingContext2D"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PageTransitionEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PageTransitionEventPrototype>(realm);
    m_prototypes.set("PageTransitionEvent"_fly_string, prototype);

    auto constructor = realm.create<PageTransitionEventConstructor>(realm);
    m_constructors.set("PageTransitionEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<Path2DPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<Path2DPrototype>(realm);
    m_prototypes.set("Path2D"_fly_string, prototype);

    auto constructor = realm.create<Path2DConstructor>(realm);
    m_constructors.set("Path2D"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PluginPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PluginPrototype>(realm);
    m_prototypes.set("Plugin"_fly_string, prototype);

    auto constructor = realm.create<PluginConstructor>(realm);
    m_constructors.set("Plugin"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PluginArrayPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PluginArrayPrototype>(realm);
    m_prototypes.set("PluginArray"_fly_string, prototype);

    auto constructor = realm.create<PluginArrayConstructor>(realm);
    m_constructors.set("PluginArray"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PopStateEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PopStateEventPrototype>(realm);
    m_prototypes.set("PopStateEvent"_fly_string, prototype);

    auto constructor = realm.create<PopStateEventConstructor>(realm);
    m_constructors.set("PopStateEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PromiseRejectionEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PromiseRejectionEventPrototype>(realm);
    m_prototypes.set("PromiseRejectionEvent"_fly_string, prototype);

    auto constructor = realm.create<PromiseRejectionEventConstructor>(realm);
    m_constructors.set("PromiseRejectionEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<RadioNodeListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<RadioNodeListPrototype>(realm);
    m_prototypes.set("RadioNodeList"_fly_string, prototype);

    auto constructor = realm.create<RadioNodeListConstructor>(realm);
    m_constructors.set("RadioNodeList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ShadowRealmGlobalScopePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ShadowRealmGlobalScopePrototype>(realm);
    m_prototypes.set("ShadowRealmGlobalScope"_fly_string, prototype);

    auto constructor = realm.create<ShadowRealmGlobalScopeConstructor>(realm);
    m_constructors.set("ShadowRealmGlobalScope"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SharedWorkerPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SharedWorkerPrototype>(realm);
    m_prototypes.set("SharedWorker"_fly_string, prototype);

    auto constructor = realm.create<SharedWorkerConstructor>(realm);
    m_constructors.set("SharedWorker"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SharedWorkerGlobalScopePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SharedWorkerGlobalScopePrototype>(realm);
    m_prototypes.set("SharedWorkerGlobalScope"_fly_string, prototype);

    auto constructor = realm.create<SharedWorkerGlobalScopeConstructor>(realm);
    m_constructors.set("SharedWorkerGlobalScope"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<StoragePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<StoragePrototype>(realm);
    m_prototypes.set("Storage"_fly_string, prototype);

    auto constructor = realm.create<StorageConstructor>(realm);
    m_constructors.set("Storage"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<StorageEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<StorageEventPrototype>(realm);
    m_prototypes.set("StorageEvent"_fly_string, prototype);

    auto constructor = realm.create<StorageEventConstructor>(realm);
    m_constructors.set("StorageEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SubmitEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SubmitEventPrototype>(realm);
    m_prototypes.set("SubmitEvent"_fly_string, prototype);

    auto constructor = realm.create<SubmitEventConstructor>(realm);
    m_constructors.set("SubmitEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TextMetricsPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TextMetricsPrototype>(realm);
    m_prototypes.set("TextMetrics"_fly_string, prototype);

    auto constructor = realm.create<TextMetricsConstructor>(realm);
    m_constructors.set("TextMetrics"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TextTrackPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TextTrackPrototype>(realm);
    m_prototypes.set("TextTrack"_fly_string, prototype);

    auto constructor = realm.create<TextTrackConstructor>(realm);
    m_constructors.set("TextTrack"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TextTrackCuePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TextTrackCuePrototype>(realm);
    m_prototypes.set("TextTrackCue"_fly_string, prototype);

    auto constructor = realm.create<TextTrackCueConstructor>(realm);
    m_constructors.set("TextTrackCue"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TextTrackCueListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TextTrackCueListPrototype>(realm);
    m_prototypes.set("TextTrackCueList"_fly_string, prototype);

    auto constructor = realm.create<TextTrackCueListConstructor>(realm);
    m_constructors.set("TextTrackCueList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TextTrackListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TextTrackListPrototype>(realm);
    m_prototypes.set("TextTrackList"_fly_string, prototype);

    auto constructor = realm.create<TextTrackListConstructor>(realm);
    m_constructors.set("TextTrackList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TimeRangesPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TimeRangesPrototype>(realm);
    m_prototypes.set("TimeRanges"_fly_string, prototype);

    auto constructor = realm.create<TimeRangesConstructor>(realm);
    m_constructors.set("TimeRanges"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ToggleEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ToggleEventPrototype>(realm);
    m_prototypes.set("ToggleEvent"_fly_string, prototype);

    auto constructor = realm.create<ToggleEventConstructor>(realm);
    m_constructors.set("ToggleEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TrackEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TrackEventPrototype>(realm);
    m_prototypes.set("TrackEvent"_fly_string, prototype);

    auto constructor = realm.create<TrackEventConstructor>(realm);
    m_constructors.set("TrackEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<UserActivationPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<UserActivationPrototype>(realm);
    m_prototypes.set("UserActivation"_fly_string, prototype);

    auto constructor = realm.create<UserActivationConstructor>(realm);
    m_constructors.set("UserActivation"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ValidityStatePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ValidityStatePrototype>(realm);
    m_prototypes.set("ValidityState"_fly_string, prototype);

    auto constructor = realm.create<ValidityStateConstructor>(realm);
    m_constructors.set("ValidityState"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<VideoTrackPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<VideoTrackPrototype>(realm);
    m_prototypes.set("VideoTrack"_fly_string, prototype);

    auto constructor = realm.create<VideoTrackConstructor>(realm);
    m_constructors.set("VideoTrack"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<VideoTrackListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<VideoTrackListPrototype>(realm);
    m_prototypes.set("VideoTrackList"_fly_string, prototype);

    auto constructor = realm.create<VideoTrackListConstructor>(realm);
    m_constructors.set("VideoTrackList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WindowPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto named_properties_object = realm.create<WindowProperties>(realm);
    m_prototypes.set("WindowProperties"_fly_string, named_properties_object);


    auto prototype = realm.create<WindowPrototype>(realm);
    m_prototypes.set("Window"_fly_string, prototype);

    auto constructor = realm.create<WindowConstructor>(realm);
    m_constructors.set("Window"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WorkerPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WorkerPrototype>(realm);
    m_prototypes.set("Worker"_fly_string, prototype);

    auto constructor = realm.create<WorkerConstructor>(realm);
    m_constructors.set("Worker"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WorkerGlobalScopePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WorkerGlobalScopePrototype>(realm);
    m_prototypes.set("WorkerGlobalScope"_fly_string, prototype);

    auto constructor = realm.create<WorkerGlobalScopeConstructor>(realm);
    m_constructors.set("WorkerGlobalScope"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WorkerLocationPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WorkerLocationPrototype>(realm);
    m_prototypes.set("WorkerLocation"_fly_string, prototype);

    auto constructor = realm.create<WorkerLocationConstructor>(realm);
    m_constructors.set("WorkerLocation"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WorkerNavigatorPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WorkerNavigatorPrototype>(realm);
    m_prototypes.set("WorkerNavigator"_fly_string, prototype);

    auto constructor = realm.create<WorkerNavigatorConstructor>(realm);
    m_constructors.set("WorkerNavigator"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WorkletGlobalScopePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WorkletGlobalScopePrototype>(realm);
    m_prototypes.set("WorkletGlobalScope"_fly_string, prototype);

    auto constructor = realm.create<WorkletGlobalScopeConstructor>(realm);
    m_constructors.set("WorkletGlobalScope"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<XMLSerializerPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<XMLSerializerPrototype>(realm);
    m_prototypes.set("XMLSerializer"_fly_string, prototype);

    auto constructor = realm.create<XMLSerializerConstructor>(realm);
    m_constructors.set("XMLSerializer"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PerformancePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PerformancePrototype>(realm);
    m_prototypes.set("Performance"_fly_string, prototype);

    auto constructor = realm.create<PerformanceConstructor>(realm);
    m_constructors.set("Performance"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<IDBCursorPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<IDBCursorPrototype>(realm);
    m_prototypes.set("IDBCursor"_fly_string, prototype);

    auto constructor = realm.create<IDBCursorConstructor>(realm);
    m_constructors.set("IDBCursor"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<IDBCursorWithValuePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<IDBCursorWithValuePrototype>(realm);
    m_prototypes.set("IDBCursorWithValue"_fly_string, prototype);

    auto constructor = realm.create<IDBCursorWithValueConstructor>(realm);
    m_constructors.set("IDBCursorWithValue"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<IDBDatabasePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<IDBDatabasePrototype>(realm);
    m_prototypes.set("IDBDatabase"_fly_string, prototype);

    auto constructor = realm.create<IDBDatabaseConstructor>(realm);
    m_constructors.set("IDBDatabase"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<IDBFactoryPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<IDBFactoryPrototype>(realm);
    m_prototypes.set("IDBFactory"_fly_string, prototype);

    auto constructor = realm.create<IDBFactoryConstructor>(realm);
    m_constructors.set("IDBFactory"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<IDBIndexPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<IDBIndexPrototype>(realm);
    m_prototypes.set("IDBIndex"_fly_string, prototype);

    auto constructor = realm.create<IDBIndexConstructor>(realm);
    m_constructors.set("IDBIndex"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<IDBKeyRangePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<IDBKeyRangePrototype>(realm);
    m_prototypes.set("IDBKeyRange"_fly_string, prototype);

    auto constructor = realm.create<IDBKeyRangeConstructor>(realm);
    m_constructors.set("IDBKeyRange"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<IDBObjectStorePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<IDBObjectStorePrototype>(realm);
    m_prototypes.set("IDBObjectStore"_fly_string, prototype);

    auto constructor = realm.create<IDBObjectStoreConstructor>(realm);
    m_constructors.set("IDBObjectStore"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<IDBOpenDBRequestPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<IDBOpenDBRequestPrototype>(realm);
    m_prototypes.set("IDBOpenDBRequest"_fly_string, prototype);

    auto constructor = realm.create<IDBOpenDBRequestConstructor>(realm);
    m_constructors.set("IDBOpenDBRequest"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<IDBRecordPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<IDBRecordPrototype>(realm);
    m_prototypes.set("IDBRecord"_fly_string, prototype);

    auto constructor = realm.create<IDBRecordConstructor>(realm);
    m_constructors.set("IDBRecord"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<IDBRequestPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<IDBRequestPrototype>(realm);
    m_prototypes.set("IDBRequest"_fly_string, prototype);

    auto constructor = realm.create<IDBRequestConstructor>(realm);
    m_constructors.set("IDBRequest"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<IDBTransactionPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<IDBTransactionPrototype>(realm);
    m_prototypes.set("IDBTransaction"_fly_string, prototype);

    auto constructor = realm.create<IDBTransactionConstructor>(realm);
    m_constructors.set("IDBTransaction"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<IDBVersionChangeEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<IDBVersionChangeEventPrototype>(realm);
    m_prototypes.set("IDBVersionChangeEvent"_fly_string, prototype);

    auto constructor = realm.create<IDBVersionChangeEventConstructor>(realm);
    m_constructors.set("IDBVersionChangeEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<FakeXRDevicePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<FakeXRDevicePrototype>(realm);
    m_prototypes.set("FakeXRDevice"_fly_string, prototype);

    auto constructor = realm.create<FakeXRDeviceConstructor>(realm);
    m_constructors.set("FakeXRDevice"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<InternalAnimationTimelinePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<InternalAnimationTimelinePrototype>(realm);
    m_prototypes.set("InternalAnimationTimeline"_fly_string, prototype);

    auto constructor = realm.create<InternalAnimationTimelineConstructor>(realm);
    m_constructors.set("InternalAnimationTimeline"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<InternalGamepadPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<InternalGamepadPrototype>(realm);
    m_prototypes.set("InternalGamepad"_fly_string, prototype);

    auto constructor = realm.create<InternalGamepadConstructor>(realm);
    m_constructors.set("InternalGamepad"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<InternalsPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<InternalsPrototype>(realm);
    m_prototypes.set("Internals"_fly_string, prototype);

    auto constructor = realm.create<InternalsConstructor>(realm);
    m_constructors.set("Internals"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebUIPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebUIPrototype>(realm);
    m_prototypes.set("WebUI"_fly_string, prototype);

    auto constructor = realm.create<WebUIConstructor>(realm);
    m_constructors.set("WebUI"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<XRTestPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<XRTestPrototype>(realm);
    m_prototypes.set("XRTest"_fly_string, prototype);

    auto constructor = realm.create<XRTestConstructor>(realm);
    m_constructors.set("XRTest"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<IntersectionObserverPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<IntersectionObserverPrototype>(realm);
    m_prototypes.set("IntersectionObserver"_fly_string, prototype);

    auto constructor = realm.create<IntersectionObserverConstructor>(realm);
    m_constructors.set("IntersectionObserver"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<IntersectionObserverEntryPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<IntersectionObserverEntryPrototype>(realm);
    m_prototypes.set("IntersectionObserverEntry"_fly_string, prototype);

    auto constructor = realm.create<IntersectionObserverEntryConstructor>(realm);
    m_constructors.set("IntersectionObserverEntry"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MathMLElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MathMLElementPrototype>(realm);
    m_prototypes.set("MathMLElement"_fly_string, prototype);

    auto constructor = realm.create<MathMLElementConstructor>(realm);
    m_constructors.set("MathMLElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MediaCapabilitiesPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MediaCapabilitiesPrototype>(realm);
    m_prototypes.set("MediaCapabilities"_fly_string, prototype);

    auto constructor = realm.create<MediaCapabilitiesConstructor>(realm);
    m_constructors.set("MediaCapabilities"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<BufferedChangeEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<BufferedChangeEventPrototype>(realm);
    m_prototypes.set("BufferedChangeEvent"_fly_string, prototype);

    auto constructor = realm.create<BufferedChangeEventConstructor>(realm);
    m_constructors.set("BufferedChangeEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ManagedMediaSourcePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ManagedMediaSourcePrototype>(realm);
    m_prototypes.set("ManagedMediaSource"_fly_string, prototype);

    auto constructor = realm.create<ManagedMediaSourceConstructor>(realm);
    m_constructors.set("ManagedMediaSource"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ManagedSourceBufferPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ManagedSourceBufferPrototype>(realm);
    m_prototypes.set("ManagedSourceBuffer"_fly_string, prototype);

    auto constructor = realm.create<ManagedSourceBufferConstructor>(realm);
    m_constructors.set("ManagedSourceBuffer"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MediaSourcePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MediaSourcePrototype>(realm);
    m_prototypes.set("MediaSource"_fly_string, prototype);

    auto constructor = realm.create<MediaSourceConstructor>(realm);
    m_constructors.set("MediaSource"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MediaSourceHandlePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MediaSourceHandlePrototype>(realm);
    m_prototypes.set("MediaSourceHandle"_fly_string, prototype);

    auto constructor = realm.create<MediaSourceHandleConstructor>(realm);
    m_constructors.set("MediaSourceHandle"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SourceBufferPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SourceBufferPrototype>(realm);
    m_prototypes.set("SourceBuffer"_fly_string, prototype);

    auto constructor = realm.create<SourceBufferConstructor>(realm);
    m_constructors.set("SourceBuffer"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SourceBufferListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SourceBufferListPrototype>(realm);
    m_prototypes.set("SourceBufferList"_fly_string, prototype);

    auto constructor = realm.create<SourceBufferListConstructor>(realm);
    m_constructors.set("SourceBufferList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PerformanceNavigationPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PerformanceNavigationPrototype>(realm);
    m_prototypes.set("PerformanceNavigation"_fly_string, prototype);

    auto constructor = realm.create<PerformanceNavigationConstructor>(realm);
    m_constructors.set("PerformanceNavigation"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PerformanceTimingPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PerformanceTimingPrototype>(realm);
    m_prototypes.set("PerformanceTiming"_fly_string, prototype);

    auto constructor = realm.create<PerformanceTimingConstructor>(realm);
    m_constructors.set("PerformanceTiming"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<NotificationPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<NotificationPrototype>(realm);
    m_prototypes.set("Notification"_fly_string, prototype);

    auto constructor = realm.create<NotificationConstructor>(realm);
    m_constructors.set("Notification"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PerformanceEntryPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PerformanceEntryPrototype>(realm);
    m_prototypes.set("PerformanceEntry"_fly_string, prototype);

    auto constructor = realm.create<PerformanceEntryConstructor>(realm);
    m_constructors.set("PerformanceEntry"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PerformanceObserverPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PerformanceObserverPrototype>(realm);
    m_prototypes.set("PerformanceObserver"_fly_string, prototype);

    auto constructor = realm.create<PerformanceObserverConstructor>(realm);
    m_constructors.set("PerformanceObserver"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PerformanceObserverEntryListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PerformanceObserverEntryListPrototype>(realm);
    m_prototypes.set("PerformanceObserverEntryList"_fly_string, prototype);

    auto constructor = realm.create<PerformanceObserverEntryListConstructor>(realm);
    m_constructors.set("PerformanceObserverEntryList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<IdleDeadlinePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<IdleDeadlinePrototype>(realm);
    m_prototypes.set("IdleDeadline"_fly_string, prototype);

    auto constructor = realm.create<IdleDeadlineConstructor>(realm);
    m_constructors.set("IdleDeadline"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ResizeObserverPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ResizeObserverPrototype>(realm);
    m_prototypes.set("ResizeObserver"_fly_string, prototype);

    auto constructor = realm.create<ResizeObserverConstructor>(realm);
    m_constructors.set("ResizeObserver"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ResizeObserverEntryPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ResizeObserverEntryPrototype>(realm);
    m_prototypes.set("ResizeObserverEntry"_fly_string, prototype);

    auto constructor = realm.create<ResizeObserverEntryConstructor>(realm);
    m_constructors.set("ResizeObserverEntry"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ResizeObserverSizePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ResizeObserverSizePrototype>(realm);
    m_prototypes.set("ResizeObserverSize"_fly_string, prototype);

    auto constructor = realm.create<ResizeObserverSizeConstructor>(realm);
    m_constructors.set("ResizeObserverSize"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PerformanceResourceTimingPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PerformanceResourceTimingPrototype>(realm);
    m_prototypes.set("PerformanceResourceTiming"_fly_string, prototype);

    auto constructor = realm.create<PerformanceResourceTimingConstructor>(realm);
    m_constructors.set("PerformanceResourceTiming"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SerialPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SerialPrototype>(realm);
    m_prototypes.set("Serial"_fly_string, prototype);

    auto constructor = realm.create<SerialConstructor>(realm);
    m_constructors.set("Serial"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SerialPortPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SerialPortPrototype>(realm);
    m_prototypes.set("SerialPort"_fly_string, prototype);

    auto constructor = realm.create<SerialPortConstructor>(realm);
    m_constructors.set("SerialPort"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CacheStoragePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CacheStoragePrototype>(realm);
    m_prototypes.set("CacheStorage"_fly_string, prototype);

    auto constructor = realm.create<CacheStorageConstructor>(realm);
    m_constructors.set("CacheStorage"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ServiceWorkerPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ServiceWorkerPrototype>(realm);
    m_prototypes.set("ServiceWorker"_fly_string, prototype);

    auto constructor = realm.create<ServiceWorkerConstructor>(realm);
    m_constructors.set("ServiceWorker"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ServiceWorkerContainerPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ServiceWorkerContainerPrototype>(realm);
    m_prototypes.set("ServiceWorkerContainer"_fly_string, prototype);

    auto constructor = realm.create<ServiceWorkerContainerConstructor>(realm);
    m_constructors.set("ServiceWorkerContainer"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ServiceWorkerGlobalScopePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ServiceWorkerGlobalScopePrototype>(realm);
    m_prototypes.set("ServiceWorkerGlobalScope"_fly_string, prototype);

    auto constructor = realm.create<ServiceWorkerGlobalScopeConstructor>(realm);
    m_constructors.set("ServiceWorkerGlobalScope"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ServiceWorkerRegistrationPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ServiceWorkerRegistrationPrototype>(realm);
    m_prototypes.set("ServiceWorkerRegistration"_fly_string, prototype);

    auto constructor = realm.create<ServiceWorkerRegistrationConstructor>(realm);
    m_constructors.set("ServiceWorkerRegistration"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SpeechGrammarPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SpeechGrammarPrototype>(realm);
    m_prototypes.set("SpeechGrammar"_fly_string, prototype);

    auto constructor = realm.create<SpeechGrammarConstructor>(realm);
    m_constructors.set("SpeechGrammar"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SpeechGrammarListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SpeechGrammarListPrototype>(realm);
    m_prototypes.set("SpeechGrammarList"_fly_string, prototype);

    auto constructor = realm.create<SpeechGrammarListConstructor>(realm);
    m_constructors.set("SpeechGrammarList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SpeechRecognitionPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SpeechRecognitionPrototype>(realm);
    m_prototypes.set("SpeechRecognition"_fly_string, prototype);

    auto constructor = realm.create<SpeechRecognitionConstructor>(realm);
    m_constructors.set("SpeechRecognition"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SpeechRecognitionAlternativePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SpeechRecognitionAlternativePrototype>(realm);
    m_prototypes.set("SpeechRecognitionAlternative"_fly_string, prototype);

    auto constructor = realm.create<SpeechRecognitionAlternativeConstructor>(realm);
    m_constructors.set("SpeechRecognitionAlternative"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SpeechRecognitionEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SpeechRecognitionEventPrototype>(realm);
    m_prototypes.set("SpeechRecognitionEvent"_fly_string, prototype);

    auto constructor = realm.create<SpeechRecognitionEventConstructor>(realm);
    m_constructors.set("SpeechRecognitionEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SpeechRecognitionPhrasePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SpeechRecognitionPhrasePrototype>(realm);
    m_prototypes.set("SpeechRecognitionPhrase"_fly_string, prototype);

    auto constructor = realm.create<SpeechRecognitionPhraseConstructor>(realm);
    m_constructors.set("SpeechRecognitionPhrase"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SpeechRecognitionResultPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SpeechRecognitionResultPrototype>(realm);
    m_prototypes.set("SpeechRecognitionResult"_fly_string, prototype);

    auto constructor = realm.create<SpeechRecognitionResultConstructor>(realm);
    m_constructors.set("SpeechRecognitionResult"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SpeechRecognitionResultListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SpeechRecognitionResultListPrototype>(realm);
    m_prototypes.set("SpeechRecognitionResultList"_fly_string, prototype);

    auto constructor = realm.create<SpeechRecognitionResultListConstructor>(realm);
    m_constructors.set("SpeechRecognitionResultList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SpeechSynthesisPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SpeechSynthesisPrototype>(realm);
    m_prototypes.set("SpeechSynthesis"_fly_string, prototype);

    auto constructor = realm.create<SpeechSynthesisConstructor>(realm);
    m_constructors.set("SpeechSynthesis"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SpeechSynthesisUtterancePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SpeechSynthesisUtterancePrototype>(realm);
    m_prototypes.set("SpeechSynthesisUtterance"_fly_string, prototype);

    auto constructor = realm.create<SpeechSynthesisUtteranceConstructor>(realm);
    m_constructors.set("SpeechSynthesisUtterance"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SpeechSynthesisVoicePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SpeechSynthesisVoicePrototype>(realm);
    m_prototypes.set("SpeechSynthesisVoice"_fly_string, prototype);

    auto constructor = realm.create<SpeechSynthesisVoiceConstructor>(realm);
    m_constructors.set("SpeechSynthesisVoice"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ByteLengthQueuingStrategyPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ByteLengthQueuingStrategyPrototype>(realm);
    m_prototypes.set("ByteLengthQueuingStrategy"_fly_string, prototype);

    auto constructor = realm.create<ByteLengthQueuingStrategyConstructor>(realm);
    m_constructors.set("ByteLengthQueuingStrategy"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CountQueuingStrategyPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CountQueuingStrategyPrototype>(realm);
    m_prototypes.set("CountQueuingStrategy"_fly_string, prototype);

    auto constructor = realm.create<CountQueuingStrategyConstructor>(realm);
    m_constructors.set("CountQueuingStrategy"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ReadableByteStreamControllerPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ReadableByteStreamControllerPrototype>(realm);
    m_prototypes.set("ReadableByteStreamController"_fly_string, prototype);

    auto constructor = realm.create<ReadableByteStreamControllerConstructor>(realm);
    m_constructors.set("ReadableByteStreamController"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ReadableStreamPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ReadableStreamPrototype>(realm);
    m_prototypes.set("ReadableStream"_fly_string, prototype);

    auto constructor = realm.create<ReadableStreamConstructor>(realm);
    m_constructors.set("ReadableStream"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ReadableStreamBYOBReaderPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ReadableStreamBYOBReaderPrototype>(realm);
    m_prototypes.set("ReadableStreamBYOBReader"_fly_string, prototype);

    auto constructor = realm.create<ReadableStreamBYOBReaderConstructor>(realm);
    m_constructors.set("ReadableStreamBYOBReader"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ReadableStreamBYOBRequestPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ReadableStreamBYOBRequestPrototype>(realm);
    m_prototypes.set("ReadableStreamBYOBRequest"_fly_string, prototype);

    auto constructor = realm.create<ReadableStreamBYOBRequestConstructor>(realm);
    m_constructors.set("ReadableStreamBYOBRequest"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ReadableStreamDefaultControllerPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ReadableStreamDefaultControllerPrototype>(realm);
    m_prototypes.set("ReadableStreamDefaultController"_fly_string, prototype);

    auto constructor = realm.create<ReadableStreamDefaultControllerConstructor>(realm);
    m_constructors.set("ReadableStreamDefaultController"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ReadableStreamDefaultReaderPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ReadableStreamDefaultReaderPrototype>(realm);
    m_prototypes.set("ReadableStreamDefaultReader"_fly_string, prototype);

    auto constructor = realm.create<ReadableStreamDefaultReaderConstructor>(realm);
    m_constructors.set("ReadableStreamDefaultReader"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TransformStreamPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TransformStreamPrototype>(realm);
    m_prototypes.set("TransformStream"_fly_string, prototype);

    auto constructor = realm.create<TransformStreamConstructor>(realm);
    m_constructors.set("TransformStream"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TransformStreamDefaultControllerPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TransformStreamDefaultControllerPrototype>(realm);
    m_prototypes.set("TransformStreamDefaultController"_fly_string, prototype);

    auto constructor = realm.create<TransformStreamDefaultControllerConstructor>(realm);
    m_constructors.set("TransformStreamDefaultController"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WritableStreamPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WritableStreamPrototype>(realm);
    m_prototypes.set("WritableStream"_fly_string, prototype);

    auto constructor = realm.create<WritableStreamConstructor>(realm);
    m_constructors.set("WritableStream"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WritableStreamDefaultControllerPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WritableStreamDefaultControllerPrototype>(realm);
    m_prototypes.set("WritableStreamDefaultController"_fly_string, prototype);

    auto constructor = realm.create<WritableStreamDefaultControllerConstructor>(realm);
    m_constructors.set("WritableStreamDefaultController"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WritableStreamDefaultWriterPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WritableStreamDefaultWriterPrototype>(realm);
    m_prototypes.set("WritableStreamDefaultWriter"_fly_string, prototype);

    auto constructor = realm.create<WritableStreamDefaultWriterConstructor>(realm);
    m_constructors.set("WritableStreamDefaultWriter"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TrustedHTMLPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TrustedHTMLPrototype>(realm);
    m_prototypes.set("TrustedHTML"_fly_string, prototype);

    auto constructor = realm.create<TrustedHTMLConstructor>(realm);
    m_constructors.set("TrustedHTML"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TrustedScriptPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TrustedScriptPrototype>(realm);
    m_prototypes.set("TrustedScript"_fly_string, prototype);

    auto constructor = realm.create<TrustedScriptConstructor>(realm);
    m_constructors.set("TrustedScript"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TrustedScriptURLPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TrustedScriptURLPrototype>(realm);
    m_prototypes.set("TrustedScriptURL"_fly_string, prototype);

    auto constructor = realm.create<TrustedScriptURLConstructor>(realm);
    m_constructors.set("TrustedScriptURL"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TrustedTypePolicyPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TrustedTypePolicyPrototype>(realm);
    m_prototypes.set("TrustedTypePolicy"_fly_string, prototype);

    auto constructor = realm.create<TrustedTypePolicyConstructor>(realm);
    m_constructors.set("TrustedTypePolicy"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TrustedTypePolicyFactoryPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TrustedTypePolicyFactoryPrototype>(realm);
    m_prototypes.set("TrustedTypePolicyFactory"_fly_string, prototype);

    auto constructor = realm.create<TrustedTypePolicyFactoryConstructor>(realm);
    m_constructors.set("TrustedTypePolicyFactory"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGAElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGAElementPrototype>(realm);
    m_prototypes.set("SVGAElement"_fly_string, prototype);

    auto constructor = realm.create<SVGAElementConstructor>(realm);
    m_constructors.set("SVGAElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGAnimatedEnumerationPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGAnimatedEnumerationPrototype>(realm);
    m_prototypes.set("SVGAnimatedEnumeration"_fly_string, prototype);

    auto constructor = realm.create<SVGAnimatedEnumerationConstructor>(realm);
    m_constructors.set("SVGAnimatedEnumeration"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGAnimatedIntegerPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGAnimatedIntegerPrototype>(realm);
    m_prototypes.set("SVGAnimatedInteger"_fly_string, prototype);

    auto constructor = realm.create<SVGAnimatedIntegerConstructor>(realm);
    m_constructors.set("SVGAnimatedInteger"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGAnimatedLengthPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGAnimatedLengthPrototype>(realm);
    m_prototypes.set("SVGAnimatedLength"_fly_string, prototype);

    auto constructor = realm.create<SVGAnimatedLengthConstructor>(realm);
    m_constructors.set("SVGAnimatedLength"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGAnimatedLengthListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGAnimatedLengthListPrototype>(realm);
    m_prototypes.set("SVGAnimatedLengthList"_fly_string, prototype);

    auto constructor = realm.create<SVGAnimatedLengthListConstructor>(realm);
    m_constructors.set("SVGAnimatedLengthList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGAnimatedNumberPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGAnimatedNumberPrototype>(realm);
    m_prototypes.set("SVGAnimatedNumber"_fly_string, prototype);

    auto constructor = realm.create<SVGAnimatedNumberConstructor>(realm);
    m_constructors.set("SVGAnimatedNumber"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGAnimatedNumberListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGAnimatedNumberListPrototype>(realm);
    m_prototypes.set("SVGAnimatedNumberList"_fly_string, prototype);

    auto constructor = realm.create<SVGAnimatedNumberListConstructor>(realm);
    m_constructors.set("SVGAnimatedNumberList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGAnimatedRectPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGAnimatedRectPrototype>(realm);
    m_prototypes.set("SVGAnimatedRect"_fly_string, prototype);

    auto constructor = realm.create<SVGAnimatedRectConstructor>(realm);
    m_constructors.set("SVGAnimatedRect"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGAnimatedStringPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGAnimatedStringPrototype>(realm);
    m_prototypes.set("SVGAnimatedString"_fly_string, prototype);

    auto constructor = realm.create<SVGAnimatedStringConstructor>(realm);
    m_constructors.set("SVGAnimatedString"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGAnimatedTransformListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGAnimatedTransformListPrototype>(realm);
    m_prototypes.set("SVGAnimatedTransformList"_fly_string, prototype);

    auto constructor = realm.create<SVGAnimatedTransformListConstructor>(realm);
    m_constructors.set("SVGAnimatedTransformList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGAnimationElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGAnimationElementPrototype>(realm);
    m_prototypes.set("SVGAnimationElement"_fly_string, prototype);

    auto constructor = realm.create<SVGAnimationElementConstructor>(realm);
    m_constructors.set("SVGAnimationElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGClipPathElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGClipPathElementPrototype>(realm);
    m_prototypes.set("SVGClipPathElement"_fly_string, prototype);

    auto constructor = realm.create<SVGClipPathElementConstructor>(realm);
    m_constructors.set("SVGClipPathElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGComponentTransferFunctionElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGComponentTransferFunctionElementPrototype>(realm);
    m_prototypes.set("SVGComponentTransferFunctionElement"_fly_string, prototype);

    auto constructor = realm.create<SVGComponentTransferFunctionElementConstructor>(realm);
    m_constructors.set("SVGComponentTransferFunctionElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGDefsElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGDefsElementPrototype>(realm);
    m_prototypes.set("SVGDefsElement"_fly_string, prototype);

    auto constructor = realm.create<SVGDefsElementConstructor>(realm);
    m_constructors.set("SVGDefsElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGDescElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGDescElementPrototype>(realm);
    m_prototypes.set("SVGDescElement"_fly_string, prototype);

    auto constructor = realm.create<SVGDescElementConstructor>(realm);
    m_constructors.set("SVGDescElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGElementPrototype>(realm);
    m_prototypes.set("SVGElement"_fly_string, prototype);

    auto constructor = realm.create<SVGElementConstructor>(realm);
    m_constructors.set("SVGElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGGElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGGElementPrototype>(realm);
    m_prototypes.set("SVGGElement"_fly_string, prototype);

    auto constructor = realm.create<SVGGElementConstructor>(realm);
    m_constructors.set("SVGGElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGGeometryElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGGeometryElementPrototype>(realm);
    m_prototypes.set("SVGGeometryElement"_fly_string, prototype);

    auto constructor = realm.create<SVGGeometryElementConstructor>(realm);
    m_constructors.set("SVGGeometryElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGGradientElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGGradientElementPrototype>(realm);
    m_prototypes.set("SVGGradientElement"_fly_string, prototype);

    auto constructor = realm.create<SVGGradientElementConstructor>(realm);
    m_constructors.set("SVGGradientElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGGraphicsElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGGraphicsElementPrototype>(realm);
    m_prototypes.set("SVGGraphicsElement"_fly_string, prototype);

    auto constructor = realm.create<SVGGraphicsElementConstructor>(realm);
    m_constructors.set("SVGGraphicsElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGImageElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGImageElementPrototype>(realm);
    m_prototypes.set("SVGImageElement"_fly_string, prototype);

    auto constructor = realm.create<SVGImageElementConstructor>(realm);
    m_constructors.set("SVGImageElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGCircleElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGCircleElementPrototype>(realm);
    m_prototypes.set("SVGCircleElement"_fly_string, prototype);

    auto constructor = realm.create<SVGCircleElementConstructor>(realm);
    m_constructors.set("SVGCircleElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGEllipseElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGEllipseElementPrototype>(realm);
    m_prototypes.set("SVGEllipseElement"_fly_string, prototype);

    auto constructor = realm.create<SVGEllipseElementConstructor>(realm);
    m_constructors.set("SVGEllipseElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFEBlendElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFEBlendElementPrototype>(realm);
    m_prototypes.set("SVGFEBlendElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFEBlendElementConstructor>(realm);
    m_constructors.set("SVGFEBlendElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFEColorMatrixElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFEColorMatrixElementPrototype>(realm);
    m_prototypes.set("SVGFEColorMatrixElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFEColorMatrixElementConstructor>(realm);
    m_constructors.set("SVGFEColorMatrixElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFEComponentTransferElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFEComponentTransferElementPrototype>(realm);
    m_prototypes.set("SVGFEComponentTransferElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFEComponentTransferElementConstructor>(realm);
    m_constructors.set("SVGFEComponentTransferElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFECompositeElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFECompositeElementPrototype>(realm);
    m_prototypes.set("SVGFECompositeElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFECompositeElementConstructor>(realm);
    m_constructors.set("SVGFECompositeElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFEDisplacementMapElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFEDisplacementMapElementPrototype>(realm);
    m_prototypes.set("SVGFEDisplacementMapElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFEDisplacementMapElementConstructor>(realm);
    m_constructors.set("SVGFEDisplacementMapElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFEDropShadowElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFEDropShadowElementPrototype>(realm);
    m_prototypes.set("SVGFEDropShadowElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFEDropShadowElementConstructor>(realm);
    m_constructors.set("SVGFEDropShadowElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFEFloodElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFEFloodElementPrototype>(realm);
    m_prototypes.set("SVGFEFloodElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFEFloodElementConstructor>(realm);
    m_constructors.set("SVGFEFloodElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFEFuncAElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFEFuncAElementPrototype>(realm);
    m_prototypes.set("SVGFEFuncAElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFEFuncAElementConstructor>(realm);
    m_constructors.set("SVGFEFuncAElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFEFuncBElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFEFuncBElementPrototype>(realm);
    m_prototypes.set("SVGFEFuncBElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFEFuncBElementConstructor>(realm);
    m_constructors.set("SVGFEFuncBElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFEFuncGElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFEFuncGElementPrototype>(realm);
    m_prototypes.set("SVGFEFuncGElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFEFuncGElementConstructor>(realm);
    m_constructors.set("SVGFEFuncGElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFEFuncRElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFEFuncRElementPrototype>(realm);
    m_prototypes.set("SVGFEFuncRElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFEFuncRElementConstructor>(realm);
    m_constructors.set("SVGFEFuncRElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFEGaussianBlurElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFEGaussianBlurElementPrototype>(realm);
    m_prototypes.set("SVGFEGaussianBlurElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFEGaussianBlurElementConstructor>(realm);
    m_constructors.set("SVGFEGaussianBlurElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFEImageElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFEImageElementPrototype>(realm);
    m_prototypes.set("SVGFEImageElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFEImageElementConstructor>(realm);
    m_constructors.set("SVGFEImageElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFEMergeElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFEMergeElementPrototype>(realm);
    m_prototypes.set("SVGFEMergeElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFEMergeElementConstructor>(realm);
    m_constructors.set("SVGFEMergeElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFEMergeNodeElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFEMergeNodeElementPrototype>(realm);
    m_prototypes.set("SVGFEMergeNodeElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFEMergeNodeElementConstructor>(realm);
    m_constructors.set("SVGFEMergeNodeElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFEMorphologyElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFEMorphologyElementPrototype>(realm);
    m_prototypes.set("SVGFEMorphologyElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFEMorphologyElementConstructor>(realm);
    m_constructors.set("SVGFEMorphologyElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFEOffsetElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFEOffsetElementPrototype>(realm);
    m_prototypes.set("SVGFEOffsetElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFEOffsetElementConstructor>(realm);
    m_constructors.set("SVGFEOffsetElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFETurbulenceElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFETurbulenceElementPrototype>(realm);
    m_prototypes.set("SVGFETurbulenceElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFETurbulenceElementConstructor>(realm);
    m_constructors.set("SVGFETurbulenceElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGFilterElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGFilterElementPrototype>(realm);
    m_prototypes.set("SVGFilterElement"_fly_string, prototype);

    auto constructor = realm.create<SVGFilterElementConstructor>(realm);
    m_constructors.set("SVGFilterElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGForeignObjectElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGForeignObjectElementPrototype>(realm);
    m_prototypes.set("SVGForeignObjectElement"_fly_string, prototype);

    auto constructor = realm.create<SVGForeignObjectElementConstructor>(realm);
    m_constructors.set("SVGForeignObjectElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGLengthPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGLengthPrototype>(realm);
    m_prototypes.set("SVGLength"_fly_string, prototype);

    auto constructor = realm.create<SVGLengthConstructor>(realm);
    m_constructors.set("SVGLength"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGLengthListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGLengthListPrototype>(realm);
    m_prototypes.set("SVGLengthList"_fly_string, prototype);

    auto constructor = realm.create<SVGLengthListConstructor>(realm);
    m_constructors.set("SVGLengthList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGLineElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGLineElementPrototype>(realm);
    m_prototypes.set("SVGLineElement"_fly_string, prototype);

    auto constructor = realm.create<SVGLineElementConstructor>(realm);
    m_constructors.set("SVGLineElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGLinearGradientElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGLinearGradientElementPrototype>(realm);
    m_prototypes.set("SVGLinearGradientElement"_fly_string, prototype);

    auto constructor = realm.create<SVGLinearGradientElementConstructor>(realm);
    m_constructors.set("SVGLinearGradientElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGMaskElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGMaskElementPrototype>(realm);
    m_prototypes.set("SVGMaskElement"_fly_string, prototype);

    auto constructor = realm.create<SVGMaskElementConstructor>(realm);
    m_constructors.set("SVGMaskElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGMetadataElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGMetadataElementPrototype>(realm);
    m_prototypes.set("SVGMetadataElement"_fly_string, prototype);

    auto constructor = realm.create<SVGMetadataElementConstructor>(realm);
    m_constructors.set("SVGMetadataElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGNumberPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGNumberPrototype>(realm);
    m_prototypes.set("SVGNumber"_fly_string, prototype);

    auto constructor = realm.create<SVGNumberConstructor>(realm);
    m_constructors.set("SVGNumber"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGNumberListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGNumberListPrototype>(realm);
    m_prototypes.set("SVGNumberList"_fly_string, prototype);

    auto constructor = realm.create<SVGNumberListConstructor>(realm);
    m_constructors.set("SVGNumberList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGPathElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGPathElementPrototype>(realm);
    m_prototypes.set("SVGPathElement"_fly_string, prototype);

    auto constructor = realm.create<SVGPathElementConstructor>(realm);
    m_constructors.set("SVGPathElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGPatternElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGPatternElementPrototype>(realm);
    m_prototypes.set("SVGPatternElement"_fly_string, prototype);

    auto constructor = realm.create<SVGPatternElementConstructor>(realm);
    m_constructors.set("SVGPatternElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGPolygonElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGPolygonElementPrototype>(realm);
    m_prototypes.set("SVGPolygonElement"_fly_string, prototype);

    auto constructor = realm.create<SVGPolygonElementConstructor>(realm);
    m_constructors.set("SVGPolygonElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGPolylineElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGPolylineElementPrototype>(realm);
    m_prototypes.set("SVGPolylineElement"_fly_string, prototype);

    auto constructor = realm.create<SVGPolylineElementConstructor>(realm);
    m_constructors.set("SVGPolylineElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGRadialGradientElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGRadialGradientElementPrototype>(realm);
    m_prototypes.set("SVGRadialGradientElement"_fly_string, prototype);

    auto constructor = realm.create<SVGRadialGradientElementConstructor>(realm);
    m_constructors.set("SVGRadialGradientElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGRectElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGRectElementPrototype>(realm);
    m_prototypes.set("SVGRectElement"_fly_string, prototype);

    auto constructor = realm.create<SVGRectElementConstructor>(realm);
    m_constructors.set("SVGRectElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGScriptElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGScriptElementPrototype>(realm);
    m_prototypes.set("SVGScriptElement"_fly_string, prototype);

    auto constructor = realm.create<SVGScriptElementConstructor>(realm);
    m_constructors.set("SVGScriptElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGSVGElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGSVGElementPrototype>(realm);
    m_prototypes.set("SVGSVGElement"_fly_string, prototype);

    auto constructor = realm.create<SVGSVGElementConstructor>(realm);
    m_constructors.set("SVGSVGElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGStopElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGStopElementPrototype>(realm);
    m_prototypes.set("SVGStopElement"_fly_string, prototype);

    auto constructor = realm.create<SVGStopElementConstructor>(realm);
    m_constructors.set("SVGStopElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGStyleElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGStyleElementPrototype>(realm);
    m_prototypes.set("SVGStyleElement"_fly_string, prototype);

    auto constructor = realm.create<SVGStyleElementConstructor>(realm);
    m_constructors.set("SVGStyleElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGSymbolElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGSymbolElementPrototype>(realm);
    m_prototypes.set("SVGSymbolElement"_fly_string, prototype);

    auto constructor = realm.create<SVGSymbolElementConstructor>(realm);
    m_constructors.set("SVGSymbolElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGTextContentElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGTextContentElementPrototype>(realm);
    m_prototypes.set("SVGTextContentElement"_fly_string, prototype);

    auto constructor = realm.create<SVGTextContentElementConstructor>(realm);
    m_constructors.set("SVGTextContentElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGTextElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGTextElementPrototype>(realm);
    m_prototypes.set("SVGTextElement"_fly_string, prototype);

    auto constructor = realm.create<SVGTextElementConstructor>(realm);
    m_constructors.set("SVGTextElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGTextPathElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGTextPathElementPrototype>(realm);
    m_prototypes.set("SVGTextPathElement"_fly_string, prototype);

    auto constructor = realm.create<SVGTextPathElementConstructor>(realm);
    m_constructors.set("SVGTextPathElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGTextPositioningElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGTextPositioningElementPrototype>(realm);
    m_prototypes.set("SVGTextPositioningElement"_fly_string, prototype);

    auto constructor = realm.create<SVGTextPositioningElementConstructor>(realm);
    m_constructors.set("SVGTextPositioningElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGTitleElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGTitleElementPrototype>(realm);
    m_prototypes.set("SVGTitleElement"_fly_string, prototype);

    auto constructor = realm.create<SVGTitleElementConstructor>(realm);
    m_constructors.set("SVGTitleElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGTransformPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGTransformPrototype>(realm);
    m_prototypes.set("SVGTransform"_fly_string, prototype);

    auto constructor = realm.create<SVGTransformConstructor>(realm);
    m_constructors.set("SVGTransform"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGTransformListPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGTransformListPrototype>(realm);
    m_prototypes.set("SVGTransformList"_fly_string, prototype);

    auto constructor = realm.create<SVGTransformListConstructor>(realm);
    m_constructors.set("SVGTransformList"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGTSpanElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGTSpanElementPrototype>(realm);
    m_prototypes.set("SVGTSpanElement"_fly_string, prototype);

    auto constructor = realm.create<SVGTSpanElementConstructor>(realm);
    m_constructors.set("SVGTSpanElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGUnitTypesPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGUnitTypesPrototype>(realm);
    m_prototypes.set("SVGUnitTypes"_fly_string, prototype);

    auto constructor = realm.create<SVGUnitTypesConstructor>(realm);
    m_constructors.set("SVGUnitTypes"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGUseElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGUseElementPrototype>(realm);
    m_prototypes.set("SVGUseElement"_fly_string, prototype);

    auto constructor = realm.create<SVGUseElementConstructor>(realm);
    m_constructors.set("SVGUseElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SVGViewElementPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SVGViewElementPrototype>(realm);
    m_prototypes.set("SVGViewElement"_fly_string, prototype);

    auto constructor = realm.create<SVGViewElementConstructor>(realm);
    m_constructors.set("SVGViewElement"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<SelectionPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<SelectionPrototype>(realm);
    m_prototypes.set("Selection"_fly_string, prototype);

    auto constructor = realm.create<SelectionConstructor>(realm);
    m_constructors.set("Selection"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<StorageManagerPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<StorageManagerPrototype>(realm);
    m_prototypes.set("StorageManager"_fly_string, prototype);

    auto constructor = realm.create<StorageManagerConstructor>(realm);
    m_constructors.set("StorageManager"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<CompositionEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<CompositionEventPrototype>(realm);
    m_prototypes.set("CompositionEvent"_fly_string, prototype);

    auto constructor = realm.create<CompositionEventConstructor>(realm);
    m_constructors.set("CompositionEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<FocusEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<FocusEventPrototype>(realm);
    m_prototypes.set("FocusEvent"_fly_string, prototype);

    auto constructor = realm.create<FocusEventConstructor>(realm);
    m_constructors.set("FocusEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<InputEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<InputEventPrototype>(realm);
    m_prototypes.set("InputEvent"_fly_string, prototype);

    auto constructor = realm.create<InputEventConstructor>(realm);
    m_constructors.set("InputEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<KeyboardEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<KeyboardEventPrototype>(realm);
    m_prototypes.set("KeyboardEvent"_fly_string, prototype);

    auto constructor = realm.create<KeyboardEventConstructor>(realm);
    m_constructors.set("KeyboardEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MouseEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MouseEventPrototype>(realm);
    m_prototypes.set("MouseEvent"_fly_string, prototype);

    auto constructor = realm.create<MouseEventConstructor>(realm);
    m_constructors.set("MouseEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PointerEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PointerEventPrototype>(realm);
    m_prototypes.set("PointerEvent"_fly_string, prototype);

    auto constructor = realm.create<PointerEventConstructor>(realm);
    m_constructors.set("PointerEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TextEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TextEventPrototype>(realm);
    m_prototypes.set("TextEvent"_fly_string, prototype);

    auto constructor = realm.create<TextEventConstructor>(realm);
    m_constructors.set("TextEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<UIEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<UIEventPrototype>(realm);
    m_prototypes.set("UIEvent"_fly_string, prototype);

    auto constructor = realm.create<UIEventConstructor>(realm);
    m_constructors.set("UIEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WheelEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WheelEventPrototype>(realm);
    m_prototypes.set("WheelEvent"_fly_string, prototype);

    auto constructor = realm.create<WheelEventConstructor>(realm);
    m_constructors.set("WheelEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<URLPatternPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<URLPatternPrototype>(realm);
    m_prototypes.set("URLPattern"_fly_string, prototype);

    auto constructor = realm.create<URLPatternConstructor>(realm);
    m_constructors.set("URLPattern"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PerformanceMarkPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PerformanceMarkPrototype>(realm);
    m_prototypes.set("PerformanceMark"_fly_string, prototype);

    auto constructor = realm.create<PerformanceMarkConstructor>(realm);
    m_constructors.set("PerformanceMark"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PerformanceMeasurePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PerformanceMeasurePrototype>(realm);
    m_prototypes.set("PerformanceMeasure"_fly_string, prototype);

    auto constructor = realm.create<PerformanceMeasureConstructor>(realm);
    m_constructors.set("PerformanceMeasure"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ViewTransitionPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ViewTransitionPrototype>(realm);
    m_prototypes.set("ViewTransition"_fly_string, prototype);

    auto constructor = realm.create<ViewTransitionConstructor>(realm);
    m_constructors.set("ViewTransition"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<GlobalPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<GlobalPrototype>(realm);
    m_prototypes.set("WebAssembly.Global"_fly_string, prototype);

    auto constructor = realm.create<GlobalConstructor>(realm);
    m_constructors.set("WebAssembly.Global"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<InstancePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<InstancePrototype>(realm);
    m_prototypes.set("WebAssembly.Instance"_fly_string, prototype);

    auto constructor = realm.create<InstanceConstructor>(realm);
    m_constructors.set("WebAssembly.Instance"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MemoryPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MemoryPrototype>(realm);
    m_prototypes.set("WebAssembly.Memory"_fly_string, prototype);

    auto constructor = realm.create<MemoryConstructor>(realm);
    m_constructors.set("WebAssembly.Memory"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ModulePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ModulePrototype>(realm);
    m_prototypes.set("WebAssembly.Module"_fly_string, prototype);

    auto constructor = realm.create<ModuleConstructor>(realm);
    m_constructors.set("WebAssembly.Module"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<TablePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<TablePrototype>(realm);
    m_prototypes.set("WebAssembly.Table"_fly_string, prototype);

    auto constructor = realm.create<TableConstructor>(realm);
    m_constructors.set("WebAssembly.Table"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AnalyserNodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AnalyserNodePrototype>(realm);
    m_prototypes.set("AnalyserNode"_fly_string, prototype);

    auto constructor = realm.create<AnalyserNodeConstructor>(realm);
    m_constructors.set("AnalyserNode"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AudioBufferPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AudioBufferPrototype>(realm);
    m_prototypes.set("AudioBuffer"_fly_string, prototype);

    auto constructor = realm.create<AudioBufferConstructor>(realm);
    m_constructors.set("AudioBuffer"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AudioBufferSourceNodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AudioBufferSourceNodePrototype>(realm);
    m_prototypes.set("AudioBufferSourceNode"_fly_string, prototype);

    auto constructor = realm.create<AudioBufferSourceNodeConstructor>(realm);
    m_constructors.set("AudioBufferSourceNode"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AudioContextPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AudioContextPrototype>(realm);
    m_prototypes.set("AudioContext"_fly_string, prototype);

    auto constructor = realm.create<AudioContextConstructor>(realm);
    m_constructors.set("AudioContext"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AudioDestinationNodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AudioDestinationNodePrototype>(realm);
    m_prototypes.set("AudioDestinationNode"_fly_string, prototype);

    auto constructor = realm.create<AudioDestinationNodeConstructor>(realm);
    m_constructors.set("AudioDestinationNode"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AudioListenerPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AudioListenerPrototype>(realm);
    m_prototypes.set("AudioListener"_fly_string, prototype);

    auto constructor = realm.create<AudioListenerConstructor>(realm);
    m_constructors.set("AudioListener"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AudioNodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AudioNodePrototype>(realm);
    m_prototypes.set("AudioNode"_fly_string, prototype);

    auto constructor = realm.create<AudioNodeConstructor>(realm);
    m_constructors.set("AudioNode"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AudioParamPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AudioParamPrototype>(realm);
    m_prototypes.set("AudioParam"_fly_string, prototype);

    auto constructor = realm.create<AudioParamConstructor>(realm);
    m_constructors.set("AudioParam"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<AudioScheduledSourceNodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<AudioScheduledSourceNodePrototype>(realm);
    m_prototypes.set("AudioScheduledSourceNode"_fly_string, prototype);

    auto constructor = realm.create<AudioScheduledSourceNodeConstructor>(realm);
    m_constructors.set("AudioScheduledSourceNode"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<BaseAudioContextPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<BaseAudioContextPrototype>(realm);
    m_prototypes.set("BaseAudioContext"_fly_string, prototype);

    auto constructor = realm.create<BaseAudioContextConstructor>(realm);
    m_constructors.set("BaseAudioContext"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<BiquadFilterNodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<BiquadFilterNodePrototype>(realm);
    m_prototypes.set("BiquadFilterNode"_fly_string, prototype);

    auto constructor = realm.create<BiquadFilterNodeConstructor>(realm);
    m_constructors.set("BiquadFilterNode"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DynamicsCompressorNodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DynamicsCompressorNodePrototype>(realm);
    m_prototypes.set("DynamicsCompressorNode"_fly_string, prototype);

    auto constructor = realm.create<DynamicsCompressorNodeConstructor>(realm);
    m_constructors.set("DynamicsCompressorNode"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ChannelMergerNodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ChannelMergerNodePrototype>(realm);
    m_prototypes.set("ChannelMergerNode"_fly_string, prototype);

    auto constructor = realm.create<ChannelMergerNodeConstructor>(realm);
    m_constructors.set("ChannelMergerNode"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ChannelSplitterNodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ChannelSplitterNodePrototype>(realm);
    m_prototypes.set("ChannelSplitterNode"_fly_string, prototype);

    auto constructor = realm.create<ChannelSplitterNodeConstructor>(realm);
    m_constructors.set("ChannelSplitterNode"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ConstantSourceNodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ConstantSourceNodePrototype>(realm);
    m_prototypes.set("ConstantSourceNode"_fly_string, prototype);

    auto constructor = realm.create<ConstantSourceNodeConstructor>(realm);
    m_constructors.set("ConstantSourceNode"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DelayNodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DelayNodePrototype>(realm);
    m_prototypes.set("DelayNode"_fly_string, prototype);

    auto constructor = realm.create<DelayNodeConstructor>(realm);
    m_constructors.set("DelayNode"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<GainNodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<GainNodePrototype>(realm);
    m_prototypes.set("GainNode"_fly_string, prototype);

    auto constructor = realm.create<GainNodeConstructor>(realm);
    m_constructors.set("GainNode"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<MediaElementAudioSourceNodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<MediaElementAudioSourceNodePrototype>(realm);
    m_prototypes.set("MediaElementAudioSourceNode"_fly_string, prototype);

    auto constructor = realm.create<MediaElementAudioSourceNodeConstructor>(realm);
    m_constructors.set("MediaElementAudioSourceNode"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<OfflineAudioCompletionEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<OfflineAudioCompletionEventPrototype>(realm);
    m_prototypes.set("OfflineAudioCompletionEvent"_fly_string, prototype);

    auto constructor = realm.create<OfflineAudioCompletionEventConstructor>(realm);
    m_constructors.set("OfflineAudioCompletionEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<OfflineAudioContextPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<OfflineAudioContextPrototype>(realm);
    m_prototypes.set("OfflineAudioContext"_fly_string, prototype);

    auto constructor = realm.create<OfflineAudioContextConstructor>(realm);
    m_constructors.set("OfflineAudioContext"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<OscillatorNodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<OscillatorNodePrototype>(realm);
    m_prototypes.set("OscillatorNode"_fly_string, prototype);

    auto constructor = realm.create<OscillatorNodeConstructor>(realm);
    m_constructors.set("OscillatorNode"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PannerNodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PannerNodePrototype>(realm);
    m_prototypes.set("PannerNode"_fly_string, prototype);

    auto constructor = realm.create<PannerNodeConstructor>(realm);
    m_constructors.set("PannerNode"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<PeriodicWavePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<PeriodicWavePrototype>(realm);
    m_prototypes.set("PeriodicWave"_fly_string, prototype);

    auto constructor = realm.create<PeriodicWaveConstructor>(realm);
    m_constructors.set("PeriodicWave"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ScriptProcessorNodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ScriptProcessorNodePrototype>(realm);
    m_prototypes.set("ScriptProcessorNode"_fly_string, prototype);

    auto constructor = realm.create<ScriptProcessorNodeConstructor>(realm);
    m_constructors.set("ScriptProcessorNode"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<StereoPannerNodePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<StereoPannerNodePrototype>(realm);
    m_prototypes.set("StereoPannerNode"_fly_string, prototype);

    auto constructor = realm.create<StereoPannerNodeConstructor>(realm);
    m_constructors.set("StereoPannerNode"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ANGLEInstancedArraysPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ANGLEInstancedArraysPrototype>(realm);
    m_prototypes.set("ANGLEInstancedArrays"_fly_string, prototype);

    auto constructor = realm.create<ANGLEInstancedArraysConstructor>(realm);
    m_constructors.set("ANGLEInstancedArrays"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<EXTBlendMinMaxPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<EXTBlendMinMaxPrototype>(realm);
    m_prototypes.set("EXTBlendMinMax"_fly_string, prototype);

    auto constructor = realm.create<EXTBlendMinMaxConstructor>(realm);
    m_constructors.set("EXTBlendMinMax"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<EXTColorBufferFloatPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<EXTColorBufferFloatPrototype>(realm);
    m_prototypes.set("EXTColorBufferFloat"_fly_string, prototype);

    auto constructor = realm.create<EXTColorBufferFloatConstructor>(realm);
    m_constructors.set("EXTColorBufferFloat"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<EXTRenderSnormPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<EXTRenderSnormPrototype>(realm);
    m_prototypes.set("EXTRenderSnorm"_fly_string, prototype);

    auto constructor = realm.create<EXTRenderSnormConstructor>(realm);
    m_constructors.set("EXTRenderSnorm"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<EXTTextureFilterAnisotropicPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<EXTTextureFilterAnisotropicPrototype>(realm);
    m_prototypes.set("EXTTextureFilterAnisotropic"_fly_string, prototype);

    auto constructor = realm.create<EXTTextureFilterAnisotropicConstructor>(realm);
    m_constructors.set("EXTTextureFilterAnisotropic"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<EXTTextureNorm16Prototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<EXTTextureNorm16Prototype>(realm);
    m_prototypes.set("EXTTextureNorm16"_fly_string, prototype);

    auto constructor = realm.create<EXTTextureNorm16Constructor>(realm);
    m_constructors.set("EXTTextureNorm16"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<OESElementIndexUintPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<OESElementIndexUintPrototype>(realm);
    m_prototypes.set("OESElementIndexUint"_fly_string, prototype);

    auto constructor = realm.create<OESElementIndexUintConstructor>(realm);
    m_constructors.set("OESElementIndexUint"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<OESStandardDerivativesPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<OESStandardDerivativesPrototype>(realm);
    m_prototypes.set("OESStandardDerivatives"_fly_string, prototype);

    auto constructor = realm.create<OESStandardDerivativesConstructor>(realm);
    m_constructors.set("OESStandardDerivatives"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<OESVertexArrayObjectPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<OESVertexArrayObjectPrototype>(realm);
    m_prototypes.set("OESVertexArrayObject"_fly_string, prototype);

    auto constructor = realm.create<OESVertexArrayObjectConstructor>(realm);
    m_constructors.set("OESVertexArrayObject"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLCompressedTextureS3tcPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLCompressedTextureS3tcPrototype>(realm);
    m_prototypes.set("WebGLCompressedTextureS3tc"_fly_string, prototype);

    auto constructor = realm.create<WebGLCompressedTextureS3tcConstructor>(realm);
    m_constructors.set("WebGLCompressedTextureS3tc"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLCompressedTextureS3tcSrgbPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLCompressedTextureS3tcSrgbPrototype>(realm);
    m_prototypes.set("WebGLCompressedTextureS3tcSrgb"_fly_string, prototype);

    auto constructor = realm.create<WebGLCompressedTextureS3tcSrgbConstructor>(realm);
    m_constructors.set("WebGLCompressedTextureS3tcSrgb"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLDrawBuffersPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLDrawBuffersPrototype>(realm);
    m_prototypes.set("WebGLDrawBuffers"_fly_string, prototype);

    auto constructor = realm.create<WebGLDrawBuffersConstructor>(realm);
    m_constructors.set("WebGLDrawBuffers"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLVertexArrayObjectOESPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLVertexArrayObjectOESPrototype>(realm);
    m_prototypes.set("WebGLVertexArrayObjectOES"_fly_string, prototype);

    auto constructor = realm.create<WebGLVertexArrayObjectOESConstructor>(realm);
    m_constructors.set("WebGLVertexArrayObjectOES"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGL2RenderingContextPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGL2RenderingContextPrototype>(realm);
    m_prototypes.set("WebGL2RenderingContext"_fly_string, prototype);

    auto constructor = realm.create<WebGL2RenderingContextConstructor>(realm);
    m_constructors.set("WebGL2RenderingContext"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLActiveInfoPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLActiveInfoPrototype>(realm);
    m_prototypes.set("WebGLActiveInfo"_fly_string, prototype);

    auto constructor = realm.create<WebGLActiveInfoConstructor>(realm);
    m_constructors.set("WebGLActiveInfo"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLBufferPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLBufferPrototype>(realm);
    m_prototypes.set("WebGLBuffer"_fly_string, prototype);

    auto constructor = realm.create<WebGLBufferConstructor>(realm);
    m_constructors.set("WebGLBuffer"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLContextEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLContextEventPrototype>(realm);
    m_prototypes.set("WebGLContextEvent"_fly_string, prototype);

    auto constructor = realm.create<WebGLContextEventConstructor>(realm);
    m_constructors.set("WebGLContextEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLFramebufferPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLFramebufferPrototype>(realm);
    m_prototypes.set("WebGLFramebuffer"_fly_string, prototype);

    auto constructor = realm.create<WebGLFramebufferConstructor>(realm);
    m_constructors.set("WebGLFramebuffer"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLObjectPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLObjectPrototype>(realm);
    m_prototypes.set("WebGLObject"_fly_string, prototype);

    auto constructor = realm.create<WebGLObjectConstructor>(realm);
    m_constructors.set("WebGLObject"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLProgramPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLProgramPrototype>(realm);
    m_prototypes.set("WebGLProgram"_fly_string, prototype);

    auto constructor = realm.create<WebGLProgramConstructor>(realm);
    m_constructors.set("WebGLProgram"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLQueryPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLQueryPrototype>(realm);
    m_prototypes.set("WebGLQuery"_fly_string, prototype);

    auto constructor = realm.create<WebGLQueryConstructor>(realm);
    m_constructors.set("WebGLQuery"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLRenderbufferPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLRenderbufferPrototype>(realm);
    m_prototypes.set("WebGLRenderbuffer"_fly_string, prototype);

    auto constructor = realm.create<WebGLRenderbufferConstructor>(realm);
    m_constructors.set("WebGLRenderbuffer"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLRenderingContextPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLRenderingContextPrototype>(realm);
    m_prototypes.set("WebGLRenderingContext"_fly_string, prototype);

    auto constructor = realm.create<WebGLRenderingContextConstructor>(realm);
    m_constructors.set("WebGLRenderingContext"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLSamplerPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLSamplerPrototype>(realm);
    m_prototypes.set("WebGLSampler"_fly_string, prototype);

    auto constructor = realm.create<WebGLSamplerConstructor>(realm);
    m_constructors.set("WebGLSampler"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLShaderPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLShaderPrototype>(realm);
    m_prototypes.set("WebGLShader"_fly_string, prototype);

    auto constructor = realm.create<WebGLShaderConstructor>(realm);
    m_constructors.set("WebGLShader"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLShaderPrecisionFormatPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLShaderPrecisionFormatPrototype>(realm);
    m_prototypes.set("WebGLShaderPrecisionFormat"_fly_string, prototype);

    auto constructor = realm.create<WebGLShaderPrecisionFormatConstructor>(realm);
    m_constructors.set("WebGLShaderPrecisionFormat"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLSyncPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLSyncPrototype>(realm);
    m_prototypes.set("WebGLSync"_fly_string, prototype);

    auto constructor = realm.create<WebGLSyncConstructor>(realm);
    m_constructors.set("WebGLSync"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLTexturePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLTexturePrototype>(realm);
    m_prototypes.set("WebGLTexture"_fly_string, prototype);

    auto constructor = realm.create<WebGLTextureConstructor>(realm);
    m_constructors.set("WebGLTexture"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLTransformFeedbackPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLTransformFeedbackPrototype>(realm);
    m_prototypes.set("WebGLTransformFeedback"_fly_string, prototype);

    auto constructor = realm.create<WebGLTransformFeedbackConstructor>(realm);
    m_constructors.set("WebGLTransformFeedback"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLUniformLocationPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLUniformLocationPrototype>(realm);
    m_prototypes.set("WebGLUniformLocation"_fly_string, prototype);

    auto constructor = realm.create<WebGLUniformLocationConstructor>(realm);
    m_constructors.set("WebGLUniformLocation"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebGLVertexArrayObjectPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebGLVertexArrayObjectPrototype>(realm);
    m_prototypes.set("WebGLVertexArrayObject"_fly_string, prototype);

    auto constructor = realm.create<WebGLVertexArrayObjectConstructor>(realm);
    m_constructors.set("WebGLVertexArrayObject"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<DOMExceptionPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<DOMExceptionPrototype>(realm);
    m_prototypes.set("DOMException"_fly_string, prototype);

    auto constructor = realm.create<DOMExceptionConstructor>(realm);
    m_constructors.set("DOMException"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<QuotaExceededErrorPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<QuotaExceededErrorPrototype>(realm);
    m_prototypes.set("QuotaExceededError"_fly_string, prototype);

    auto constructor = realm.create<QuotaExceededErrorConstructor>(realm);
    m_constructors.set("QuotaExceededError"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<WebSocketPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<WebSocketPrototype>(realm);
    m_prototypes.set("WebSocket"_fly_string, prototype);

    auto constructor = realm.create<WebSocketConstructor>(realm);
    m_constructors.set("WebSocket"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<VTTCuePrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<VTTCuePrototype>(realm);
    m_prototypes.set("VTTCue"_fly_string, prototype);

    auto constructor = realm.create<VTTCueConstructor>(realm);
    m_constructors.set("VTTCue"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<VTTRegionPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<VTTRegionPrototype>(realm);
    m_prototypes.set("VTTRegion"_fly_string, prototype);

    auto constructor = realm.create<VTTRegionConstructor>(realm);
    m_constructors.set("VTTRegion"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<XRSessionPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<XRSessionPrototype>(realm);
    m_prototypes.set("XRSession"_fly_string, prototype);

    auto constructor = realm.create<XRSessionConstructor>(realm);
    m_constructors.set("XRSession"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<XRSessionEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<XRSessionEventPrototype>(realm);
    m_prototypes.set("XRSessionEvent"_fly_string, prototype);

    auto constructor = realm.create<XRSessionEventConstructor>(realm);
    m_constructors.set("XRSessionEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<XRSystemPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<XRSystemPrototype>(realm);
    m_prototypes.set("XRSystem"_fly_string, prototype);

    auto constructor = realm.create<XRSystemConstructor>(realm);
    m_constructors.set("XRSystem"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<FormDataPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<FormDataPrototype>(realm);
    m_prototypes.set("FormData"_fly_string, prototype);

    auto constructor = realm.create<FormDataConstructor>(realm);
    m_constructors.set("FormData"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<ProgressEventPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<ProgressEventPrototype>(realm);
    m_prototypes.set("ProgressEvent"_fly_string, prototype);

    auto constructor = realm.create<ProgressEventConstructor>(realm);
    m_constructors.set("ProgressEvent"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<XMLHttpRequestPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<XMLHttpRequestPrototype>(realm);
    m_prototypes.set("XMLHttpRequest"_fly_string, prototype);

    auto constructor = realm.create<XMLHttpRequestConstructor>(realm);
    m_constructors.set("XMLHttpRequest"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<XMLHttpRequestEventTargetPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<XMLHttpRequestEventTargetPrototype>(realm);
    m_prototypes.set("XMLHttpRequestEventTarget"_fly_string, prototype);

    auto constructor = realm.create<XMLHttpRequestEventTargetConstructor>(realm);
    m_constructors.set("XMLHttpRequestEventTarget"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<XMLHttpRequestUploadPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<XMLHttpRequestUploadPrototype>(realm);
    m_prototypes.set("XMLHttpRequestUpload"_fly_string, prototype);

    auto constructor = realm.create<XMLHttpRequestUploadConstructor>(realm);
    m_constructors.set("XMLHttpRequestUpload"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<XPathResultPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<XPathResultPrototype>(realm);
    m_prototypes.set("XPathResult"_fly_string, prototype);

    auto constructor = realm.create<XPathResultConstructor>(realm);
    m_constructors.set("XPathResult"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<XPathExpressionPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<XPathExpressionPrototype>(realm);
    m_prototypes.set("XPathExpression"_fly_string, prototype);

    auto constructor = realm.create<XPathExpressionConstructor>(realm);
    m_constructors.set("XPathExpression"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

template<>
WEB_API void Intrinsics::create_web_prototype_and_constructor<XPathEvaluatorPrototype>(JS::Realm& realm)
{
    auto& vm = realm.vm();


    auto prototype = realm.create<XPathEvaluatorPrototype>(realm);
    m_prototypes.set("XPathEvaluator"_fly_string, prototype);

    auto constructor = realm.create<XPathEvaluatorConstructor>(realm);
    m_constructors.set("XPathEvaluator"_fly_string, constructor);

    prototype->define_direct_property(vm.names.constructor, constructor.ptr(), JS::Attribute::Writable | JS::Attribute::Configurable);

}

}
