
#include <LibJS/Runtime/Object.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/Bindings/WindowExposedInterfaces.h>
#include <LibWeb/HTML/Scripting/Environments.h>
#include <LibWeb/HTML/UniversalGlobalScope.h>
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
#include <LibWeb/Bindings/SharedWorkerConstructor.h>
#include <LibWeb/Bindings/SharedWorkerPrototype.h>
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

void add_window_exposed_interfaces(JS::Object& global)
{
    static constexpr u8 attr = JS::Attribute::Writable | JS::Attribute::Configurable;

    [[maybe_unused]] bool is_secure_context = HTML::is_secure_context(HTML::relevant_principal_settings_object(global));
    [[maybe_unused]] bool expose_experimental_interfaces = HTML::UniversalGlobalScopeMixin::expose_experimental_interfaces();

    global.define_intrinsic_accessor("Animation"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AnimationPrototype>(realm, "Animation"_fly_string); });
    global.define_intrinsic_accessor("AnimationEffect"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AnimationEffectPrototype>(realm, "AnimationEffect"_fly_string); });
    global.define_intrinsic_accessor("AnimationPlaybackEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AnimationPlaybackEventPrototype>(realm, "AnimationPlaybackEvent"_fly_string); });
    global.define_intrinsic_accessor("AnimationTimeline"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AnimationTimelinePrototype>(realm, "AnimationTimeline"_fly_string); });
    global.define_intrinsic_accessor("DocumentTimeline"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DocumentTimelinePrototype>(realm, "DocumentTimeline"_fly_string); });
    global.define_intrinsic_accessor("KeyframeEffect"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<KeyframeEffectPrototype>(realm, "KeyframeEffect"_fly_string); });
    global.define_intrinsic_accessor("ScrollTimeline"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ScrollTimelinePrototype>(realm, "ScrollTimeline"_fly_string); });
    if (is_secure_context) {
    global.define_intrinsic_accessor("Clipboard"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ClipboardPrototype>(realm, "Clipboard"_fly_string); });
    }
    global.define_intrinsic_accessor("ClipboardEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ClipboardEventPrototype>(realm, "ClipboardEvent"_fly_string); });
    if (is_secure_context) {
    global.define_intrinsic_accessor("ClipboardItem"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ClipboardItemPrototype>(realm, "ClipboardItem"_fly_string); });
    }
    global.define_intrinsic_accessor("SecurityPolicyViolationEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SecurityPolicyViolationEventPrototype>(realm, "SecurityPolicyViolationEvent"_fly_string); });
    global.define_intrinsic_accessor("CompressionStream"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CompressionStreamPrototype>(realm, "CompressionStream"_fly_string); });
    global.define_intrinsic_accessor("DecompressionStream"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DecompressionStreamPrototype>(realm, "DecompressionStream"_fly_string); });
    if (is_secure_context) {
    global.define_intrinsic_accessor("CookieChangeEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CookieChangeEventPrototype>(realm, "CookieChangeEvent"_fly_string); });
    }
    if (is_secure_context) {
    global.define_intrinsic_accessor("CookieStore"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CookieStorePrototype>(realm, "CookieStore"_fly_string); });
    }
    if (is_secure_context) {
    global.define_intrinsic_accessor("Credential"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CredentialPrototype>(realm, "Credential"_fly_string); });
    }
    if (is_secure_context) {
    global.define_intrinsic_accessor("CredentialsContainer"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CredentialsContainerPrototype>(realm, "CredentialsContainer"_fly_string); });
    }
    if (is_secure_context) {
    global.define_intrinsic_accessor("FederatedCredential"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FederatedCredentialPrototype>(realm, "FederatedCredential"_fly_string); });
    }
    if (is_secure_context) {
    global.define_intrinsic_accessor("PasswordCredential"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PasswordCredentialPrototype>(realm, "PasswordCredential"_fly_string); });
    }
    global.define_intrinsic_accessor("Crypto"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CryptoPrototype>(realm, "Crypto"_fly_string); });
    if (is_secure_context) {
    global.define_intrinsic_accessor("CryptoKey"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CryptoKeyPrototype>(realm, "CryptoKey"_fly_string); });
    }
    if (is_secure_context) {
    global.define_intrinsic_accessor("SubtleCrypto"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SubtleCryptoPrototype>(realm, "SubtleCrypto"_fly_string); });
    }
    global.define_intrinsic_accessor("AnimationEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AnimationEventPrototype>(realm, "AnimationEvent"_fly_string); });
    global.define_intrinsic_accessor("CSSAnimation"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSAnimationPrototype>(realm, "CSSAnimation"_fly_string); });
    global.define_intrinsic_accessor("CSSConditionRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSConditionRulePrototype>(realm, "CSSConditionRule"_fly_string); });
    global.define_intrinsic_accessor("CSSCounterStyleRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSCounterStyleRulePrototype>(realm, "CSSCounterStyleRule"_fly_string); });
    global.define_intrinsic_accessor("CSSFontFaceDescriptors"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSFontFaceDescriptorsPrototype>(realm, "CSSFontFaceDescriptors"_fly_string); });
    global.define_intrinsic_accessor("CSSFontFaceRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSFontFaceRulePrototype>(realm, "CSSFontFaceRule"_fly_string); });
    global.define_intrinsic_accessor("CSSFontFeatureValuesMap"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSFontFeatureValuesMapPrototype>(realm, "CSSFontFeatureValuesMap"_fly_string); });
    global.define_intrinsic_accessor("CSSFontFeatureValuesRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSFontFeatureValuesRulePrototype>(realm, "CSSFontFeatureValuesRule"_fly_string); });
    global.define_intrinsic_accessor("CSSGroupingRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSGroupingRulePrototype>(realm, "CSSGroupingRule"_fly_string); });
    global.define_intrinsic_accessor("CSSImageValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSImageValuePrototype>(realm, "CSSImageValue"_fly_string); });
    global.define_intrinsic_accessor("CSSImportRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSImportRulePrototype>(realm, "CSSImportRule"_fly_string); });
    global.define_intrinsic_accessor("CSSKeyframeRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSKeyframeRulePrototype>(realm, "CSSKeyframeRule"_fly_string); });
    global.define_intrinsic_accessor("CSSKeyframesRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSKeyframesRulePrototype>(realm, "CSSKeyframesRule"_fly_string); });
    global.define_intrinsic_accessor("CSSKeywordValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSKeywordValuePrototype>(realm, "CSSKeywordValue"_fly_string); });
    global.define_intrinsic_accessor("CSSLayerBlockRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSLayerBlockRulePrototype>(realm, "CSSLayerBlockRule"_fly_string); });
    global.define_intrinsic_accessor("CSSLayerStatementRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSLayerStatementRulePrototype>(realm, "CSSLayerStatementRule"_fly_string); });
    global.define_intrinsic_accessor("CSSMarginRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMarginRulePrototype>(realm, "CSSMarginRule"_fly_string); });
    global.define_intrinsic_accessor("CSSMathClamp"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMathClampPrototype>(realm, "CSSMathClamp"_fly_string); });
    global.define_intrinsic_accessor("CSSMathInvert"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMathInvertPrototype>(realm, "CSSMathInvert"_fly_string); });
    global.define_intrinsic_accessor("CSSMathMax"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMathMaxPrototype>(realm, "CSSMathMax"_fly_string); });
    global.define_intrinsic_accessor("CSSMathMin"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMathMinPrototype>(realm, "CSSMathMin"_fly_string); });
    global.define_intrinsic_accessor("CSSMathNegate"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMathNegatePrototype>(realm, "CSSMathNegate"_fly_string); });
    global.define_intrinsic_accessor("CSSMathProduct"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMathProductPrototype>(realm, "CSSMathProduct"_fly_string); });
    global.define_intrinsic_accessor("CSSMathSum"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMathSumPrototype>(realm, "CSSMathSum"_fly_string); });
    global.define_intrinsic_accessor("CSSMathValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMathValuePrototype>(realm, "CSSMathValue"_fly_string); });
    global.define_intrinsic_accessor("CSSMatrixComponent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMatrixComponentPrototype>(realm, "CSSMatrixComponent"_fly_string); });
    global.define_intrinsic_accessor("CSSMediaRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSMediaRulePrototype>(realm, "CSSMediaRule"_fly_string); });
    global.define_intrinsic_accessor("CSSNamespaceRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSNamespaceRulePrototype>(realm, "CSSNamespaceRule"_fly_string); });
    global.define_intrinsic_accessor("CSSNestedDeclarations"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSNestedDeclarationsPrototype>(realm, "CSSNestedDeclarations"_fly_string); });
    global.define_intrinsic_accessor("CSSNumericArray"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSNumericArrayPrototype>(realm, "CSSNumericArray"_fly_string); });
    global.define_intrinsic_accessor("CSSNumericValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSNumericValuePrototype>(realm, "CSSNumericValue"_fly_string); });
    global.define_intrinsic_accessor("CSSPageRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSPageRulePrototype>(realm, "CSSPageRule"_fly_string); });
    global.define_intrinsic_accessor("CSSPageDescriptors"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSPageDescriptorsPrototype>(realm, "CSSPageDescriptors"_fly_string); });
    global.define_intrinsic_accessor("CSSPerspective"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSPerspectivePrototype>(realm, "CSSPerspective"_fly_string); });
    global.define_intrinsic_accessor("CSSPropertyRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSPropertyRulePrototype>(realm, "CSSPropertyRule"_fly_string); });
    global.define_intrinsic_accessor("CSSRotate"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSRotatePrototype>(realm, "CSSRotate"_fly_string); });
    global.define_intrinsic_accessor("CSSRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSRulePrototype>(realm, "CSSRule"_fly_string); });
    global.define_intrinsic_accessor("CSSRuleList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSRuleListPrototype>(realm, "CSSRuleList"_fly_string); });
    global.define_intrinsic_accessor("CSSScale"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSScalePrototype>(realm, "CSSScale"_fly_string); });
    global.define_intrinsic_accessor("CSSSkew"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSSkewPrototype>(realm, "CSSSkew"_fly_string); });
    global.define_intrinsic_accessor("CSSSkewX"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSSkewXPrototype>(realm, "CSSSkewX"_fly_string); });
    global.define_intrinsic_accessor("CSSSkewY"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSSkewYPrototype>(realm, "CSSSkewY"_fly_string); });
    global.define_intrinsic_accessor("CSSStyleDeclaration"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSStyleDeclarationPrototype>(realm, "CSSStyleDeclaration"_fly_string); });
    global.define_intrinsic_accessor("CSSStyleProperties"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSStylePropertiesPrototype>(realm, "CSSStyleProperties"_fly_string); });
    global.define_intrinsic_accessor("CSSStyleRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSStyleRulePrototype>(realm, "CSSStyleRule"_fly_string); });
    global.define_intrinsic_accessor("CSSStyleSheet"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSStyleSheetPrototype>(realm, "CSSStyleSheet"_fly_string); });
    global.define_intrinsic_accessor("CSSStyleValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSStyleValuePrototype>(realm, "CSSStyleValue"_fly_string); });
    global.define_intrinsic_accessor("CSSSupportsRule"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSSupportsRulePrototype>(realm, "CSSSupportsRule"_fly_string); });
    global.define_intrinsic_accessor("CSSTransformComponent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSTransformComponentPrototype>(realm, "CSSTransformComponent"_fly_string); });
    global.define_intrinsic_accessor("CSSTransformValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSTransformValuePrototype>(realm, "CSSTransformValue"_fly_string); });
    global.define_intrinsic_accessor("CSSTransition"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSTransitionPrototype>(realm, "CSSTransition"_fly_string); });
    global.define_intrinsic_accessor("CSSTranslate"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSTranslatePrototype>(realm, "CSSTranslate"_fly_string); });
    global.define_intrinsic_accessor("CSSUnitValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSUnitValuePrototype>(realm, "CSSUnitValue"_fly_string); });
    global.define_intrinsic_accessor("CSSUnparsedValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSUnparsedValuePrototype>(realm, "CSSUnparsedValue"_fly_string); });
    global.define_intrinsic_accessor("CSSVariableReferenceValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CSSVariableReferenceValuePrototype>(realm, "CSSVariableReferenceValue"_fly_string); });
    global.define_intrinsic_accessor("FontFace"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FontFacePrototype>(realm, "FontFace"_fly_string); });
    global.define_intrinsic_accessor("FontFaceSet"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FontFaceSetPrototype>(realm, "FontFaceSet"_fly_string); });
    global.define_intrinsic_accessor("FontFaceSetLoadEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FontFaceSetLoadEventPrototype>(realm, "FontFaceSetLoadEvent"_fly_string); });
    global.define_intrinsic_accessor("MediaList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MediaListPrototype>(realm, "MediaList"_fly_string); });
    global.define_intrinsic_accessor("MediaQueryList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MediaQueryListPrototype>(realm, "MediaQueryList"_fly_string); });
    global.define_intrinsic_accessor("MediaQueryListEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MediaQueryListEventPrototype>(realm, "MediaQueryListEvent"_fly_string); });
    global.define_intrinsic_accessor("Screen"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ScreenPrototype>(realm, "Screen"_fly_string); });
    global.define_intrinsic_accessor("ScreenOrientation"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ScreenOrientationPrototype>(realm, "ScreenOrientation"_fly_string); });
    global.define_intrinsic_accessor("StylePropertyMap"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<StylePropertyMapPrototype>(realm, "StylePropertyMap"_fly_string); });
    global.define_intrinsic_accessor("StylePropertyMapReadOnly"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<StylePropertyMapReadOnlyPrototype>(realm, "StylePropertyMapReadOnly"_fly_string); });
    global.define_intrinsic_accessor("StyleSheet"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<StyleSheetPrototype>(realm, "StyleSheet"_fly_string); });
    global.define_intrinsic_accessor("StyleSheetList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<StyleSheetListPrototype>(realm, "StyleSheetList"_fly_string); });
    global.define_intrinsic_accessor("TransitionEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TransitionEventPrototype>(realm, "TransitionEvent"_fly_string); });
    global.define_intrinsic_accessor("VisualViewport"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<VisualViewportPrototype>(realm, "VisualViewport"_fly_string); });
    global.define_intrinsic_accessor("AbstractRange"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AbstractRangePrototype>(realm, "AbstractRange"_fly_string); });
    global.define_intrinsic_accessor("Attr"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AttrPrototype>(realm, "Attr"_fly_string); });
    global.define_intrinsic_accessor("AbortController"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AbortControllerPrototype>(realm, "AbortController"_fly_string); });
    global.define_intrinsic_accessor("AbortSignal"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AbortSignalPrototype>(realm, "AbortSignal"_fly_string); });
    global.define_intrinsic_accessor("CDATASection"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CDATASectionPrototype>(realm, "CDATASection"_fly_string); });
    global.define_intrinsic_accessor("CharacterData"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CharacterDataPrototype>(realm, "CharacterData"_fly_string); });
    global.define_intrinsic_accessor("Comment"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CommentPrototype>(realm, "Comment"_fly_string); });
    global.define_intrinsic_accessor("CustomEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CustomEventPrototype>(realm, "CustomEvent"_fly_string); });
    global.define_intrinsic_accessor("Document"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DocumentPrototype>(realm, "Document"_fly_string); });
    global.define_intrinsic_accessor("DocumentFragment"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DocumentFragmentPrototype>(realm, "DocumentFragment"_fly_string); });
    global.define_intrinsic_accessor("DocumentType"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DocumentTypePrototype>(realm, "DocumentType"_fly_string); });
    global.define_intrinsic_accessor("DOMImplementation"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMImplementationPrototype>(realm, "DOMImplementation"_fly_string); });
    global.define_intrinsic_accessor("DOMTokenList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMTokenListPrototype>(realm, "DOMTokenList"_fly_string); });
    global.define_intrinsic_accessor("Element"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ElementPrototype>(realm, "Element"_fly_string); });
    global.define_intrinsic_accessor("Event"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<EventPrototype>(realm, "Event"_fly_string); });
    global.define_intrinsic_accessor("EventTarget"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<EventTargetPrototype>(realm, "EventTarget"_fly_string); });
    global.define_intrinsic_accessor("HTMLCollection"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLCollectionPrototype>(realm, "HTMLCollection"_fly_string); });
    global.define_intrinsic_accessor("MutationObserver"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MutationObserverPrototype>(realm, "MutationObserver"_fly_string); });
    global.define_intrinsic_accessor("MutationRecord"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MutationRecordPrototype>(realm, "MutationRecord"_fly_string); });
    global.define_intrinsic_accessor("NamedNodeMap"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<NamedNodeMapPrototype>(realm, "NamedNodeMap"_fly_string); });
    global.define_intrinsic_accessor("Node"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<NodePrototype>(realm, "Node"_fly_string); });
    global.define_intrinsic_accessor("NodeFilter"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<NodeFilterPrototype>(realm, "NodeFilter"_fly_string); });
    global.define_intrinsic_accessor("NodeIterator"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<NodeIteratorPrototype>(realm, "NodeIterator"_fly_string); });
    global.define_intrinsic_accessor("NodeList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<NodeListPrototype>(realm, "NodeList"_fly_string); });
    global.define_intrinsic_accessor("ProcessingInstruction"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ProcessingInstructionPrototype>(realm, "ProcessingInstruction"_fly_string); });
    global.define_intrinsic_accessor("Range"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<RangePrototype>(realm, "Range"_fly_string); });
    global.define_intrinsic_accessor("ShadowRoot"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ShadowRootPrototype>(realm, "ShadowRoot"_fly_string); });
    global.define_intrinsic_accessor("StaticRange"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<StaticRangePrototype>(realm, "StaticRange"_fly_string); });
    global.define_intrinsic_accessor("Text"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TextPrototype>(realm, "Text"_fly_string); });
    global.define_intrinsic_accessor("TreeWalker"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TreeWalkerPrototype>(realm, "TreeWalker"_fly_string); });
    global.define_intrinsic_accessor("XMLDocument"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<XMLDocumentPrototype>(realm, "XMLDocument"_fly_string); });
    global.define_intrinsic_accessor("URL"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMURLPrototype>(realm, "URL"_fly_string); });
    global.define_intrinsic_accessor("webkitURL"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMURLPrototype>(realm, "URL"_fly_string); });
    global.define_intrinsic_accessor("Origin"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<OriginPrototype>(realm, "Origin"_fly_string); });
    global.define_intrinsic_accessor("URLSearchParams"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<URLSearchParamsPrototype>(realm, "URLSearchParams"_fly_string); });
    global.define_intrinsic_accessor("TextDecoder"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TextDecoderPrototype>(realm, "TextDecoder"_fly_string); });
    global.define_intrinsic_accessor("TextEncoder"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TextEncoderPrototype>(realm, "TextEncoder"_fly_string); });
    global.define_intrinsic_accessor("TextEncoderStream"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TextEncoderStreamPrototype>(realm, "TextEncoderStream"_fly_string); });
    if (is_secure_context) {
    global.define_intrinsic_accessor("MediaKeySystemAccess"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MediaKeySystemAccessPrototype>(realm, "MediaKeySystemAccess"_fly_string); });
    }
    global.define_intrinsic_accessor("FileSystemEntry"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FileSystemEntryPrototype>(realm, "FileSystemEntry"_fly_string); });
    global.define_intrinsic_accessor("PerformanceEventTiming"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PerformanceEventTimingPrototype>(realm, "PerformanceEventTiming"_fly_string); });
    global.define_intrinsic_accessor("Headers"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HeadersPrototype>(realm, "Headers"_fly_string); });
    global.define_intrinsic_accessor("Request"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<RequestPrototype>(realm, "Request"_fly_string); });
    global.define_intrinsic_accessor("Response"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ResponsePrototype>(realm, "Response"_fly_string); });
    global.define_intrinsic_accessor("Blob"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<BlobPrototype>(realm, "Blob"_fly_string); });
    global.define_intrinsic_accessor("File"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FilePrototype>(realm, "File"_fly_string); });
    global.define_intrinsic_accessor("FileList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FileListPrototype>(realm, "FileList"_fly_string); });
    global.define_intrinsic_accessor("FileReader"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FileReaderPrototype>(realm, "FileReader"_fly_string); });
    global.define_intrinsic_accessor("Gamepad"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<GamepadPrototype>(realm, "Gamepad"_fly_string); });
    global.define_intrinsic_accessor("GamepadButton"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<GamepadButtonPrototype>(realm, "GamepadButton"_fly_string); });
    global.define_intrinsic_accessor("GamepadEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<GamepadEventPrototype>(realm, "GamepadEvent"_fly_string); });
    global.define_intrinsic_accessor("GamepadHapticActuator"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<GamepadHapticActuatorPrototype>(realm, "GamepadHapticActuator"_fly_string); });
    global.define_intrinsic_accessor("Geolocation"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<GeolocationPrototype>(realm, "Geolocation"_fly_string); });
    if (is_secure_context) {
    global.define_intrinsic_accessor("GeolocationCoordinates"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<GeolocationCoordinatesPrototype>(realm, "GeolocationCoordinates"_fly_string); });
    }
    if (is_secure_context) {
    global.define_intrinsic_accessor("GeolocationPosition"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<GeolocationPositionPrototype>(realm, "GeolocationPosition"_fly_string); });
    }
    global.define_intrinsic_accessor("GeolocationPositionError"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<GeolocationPositionErrorPrototype>(realm, "GeolocationPositionError"_fly_string); });
    global.define_intrinsic_accessor("DOMMatrix"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMMatrixPrototype>(realm, "DOMMatrix"_fly_string); });
    global.define_intrinsic_accessor("SVGMatrix"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMMatrixPrototype>(realm, "DOMMatrix"_fly_string); });
    global.define_intrinsic_accessor("WebKitCSSMatrix"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMMatrixPrototype>(realm, "DOMMatrix"_fly_string); });
    global.define_intrinsic_accessor("DOMMatrixReadOnly"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMMatrixReadOnlyPrototype>(realm, "DOMMatrixReadOnly"_fly_string); });
    global.define_intrinsic_accessor("DOMPoint"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMPointPrototype>(realm, "DOMPoint"_fly_string); });
    global.define_intrinsic_accessor("SVGPoint"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMPointPrototype>(realm, "DOMPoint"_fly_string); });
    global.define_intrinsic_accessor("DOMPointReadOnly"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMPointReadOnlyPrototype>(realm, "DOMPointReadOnly"_fly_string); });
    global.define_intrinsic_accessor("DOMQuad"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMQuadPrototype>(realm, "DOMQuad"_fly_string); });
    global.define_intrinsic_accessor("DOMRect"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMRectPrototype>(realm, "DOMRect"_fly_string); });
    global.define_intrinsic_accessor("SVGRect"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMRectPrototype>(realm, "DOMRect"_fly_string); });
    global.define_intrinsic_accessor("DOMRectList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMRectListPrototype>(realm, "DOMRectList"_fly_string); });
    global.define_intrinsic_accessor("DOMRectReadOnly"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMRectReadOnlyPrototype>(realm, "DOMRectReadOnly"_fly_string); });
    global.define_intrinsic_accessor("AudioTrack"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AudioTrackPrototype>(realm, "AudioTrack"_fly_string); });
    global.define_intrinsic_accessor("AudioTrackList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AudioTrackListPrototype>(realm, "AudioTrackList"_fly_string); });
    global.define_intrinsic_accessor("BarProp"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<BarPropPrototype>(realm, "BarProp"_fly_string); });
    global.define_intrinsic_accessor("BeforeUnloadEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<BeforeUnloadEventPrototype>(realm, "BeforeUnloadEvent"_fly_string); });
    global.define_intrinsic_accessor("BroadcastChannel"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<BroadcastChannelPrototype>(realm, "BroadcastChannel"_fly_string); });
    global.define_intrinsic_accessor("CanvasGradient"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CanvasGradientPrototype>(realm, "CanvasGradient"_fly_string); });
    global.define_intrinsic_accessor("CanvasPattern"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CanvasPatternPrototype>(realm, "CanvasPattern"_fly_string); });
    global.define_intrinsic_accessor("CanvasRenderingContext2D"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CanvasRenderingContext2DPrototype>(realm, "CanvasRenderingContext2D"_fly_string); });
    global.define_intrinsic_accessor("CloseEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CloseEventPrototype>(realm, "CloseEvent"_fly_string); });
    global.define_intrinsic_accessor("CloseWatcher"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CloseWatcherPrototype>(realm, "CloseWatcher"_fly_string); });
    global.define_intrinsic_accessor("CommandEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CommandEventPrototype>(realm, "CommandEvent"_fly_string); });
    global.define_intrinsic_accessor("CustomElementRegistry"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CustomElementRegistryPrototype>(realm, "CustomElementRegistry"_fly_string); });
    global.define_intrinsic_accessor("CustomStateSet"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CustomStateSetPrototype>(realm, "CustomStateSet"_fly_string); });
    global.define_intrinsic_accessor("DataTransfer"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DataTransferPrototype>(realm, "DataTransfer"_fly_string); });
    global.define_intrinsic_accessor("DataTransferItem"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DataTransferItemPrototype>(realm, "DataTransferItem"_fly_string); });
    global.define_intrinsic_accessor("DataTransferItemList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DataTransferItemListPrototype>(realm, "DataTransferItemList"_fly_string); });
    global.define_intrinsic_accessor("DOMParser"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMParserPrototype>(realm, "DOMParser"_fly_string); });
    global.define_intrinsic_accessor("DOMStringList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMStringListPrototype>(realm, "DOMStringList"_fly_string); });
    global.define_intrinsic_accessor("DOMStringMap"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMStringMapPrototype>(realm, "DOMStringMap"_fly_string); });
    global.define_intrinsic_accessor("DragEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DragEventPrototype>(realm, "DragEvent"_fly_string); });
    global.define_intrinsic_accessor("ElementInternals"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ElementInternalsPrototype>(realm, "ElementInternals"_fly_string); });
    global.define_intrinsic_accessor("ErrorEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ErrorEventPrototype>(realm, "ErrorEvent"_fly_string); });
    global.define_intrinsic_accessor("EventSource"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<EventSourcePrototype>(realm, "EventSource"_fly_string); });
    global.define_intrinsic_accessor("External"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ExternalPrototype>(realm, "External"_fly_string); });
    global.define_intrinsic_accessor("FormDataEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FormDataEventPrototype>(realm, "FormDataEvent"_fly_string); });
    global.define_intrinsic_accessor("HashChangeEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HashChangeEventPrototype>(realm, "HashChangeEvent"_fly_string); });
    global.define_intrinsic_accessor("History"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HistoryPrototype>(realm, "History"_fly_string); });
    global.define_intrinsic_accessor("HTMLAllCollection"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLAllCollectionPrototype>(realm, "HTMLAllCollection"_fly_string); });
    global.define_intrinsic_accessor("HTMLAnchorElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLAnchorElementPrototype>(realm, "HTMLAnchorElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLAreaElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLAreaElementPrototype>(realm, "HTMLAreaElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLAudioElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLAudioElementPrototype>(realm, "HTMLAudioElement"_fly_string); });
    global.define_intrinsic_accessor("Audio"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLAudioElementPrototype>(realm, "Audio"_fly_string); });
    global.define_intrinsic_accessor("HTMLBaseElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLBaseElementPrototype>(realm, "HTMLBaseElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLBodyElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLBodyElementPrototype>(realm, "HTMLBodyElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLBRElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLBRElementPrototype>(realm, "HTMLBRElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLButtonElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLButtonElementPrototype>(realm, "HTMLButtonElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLCanvasElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLCanvasElementPrototype>(realm, "HTMLCanvasElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLDataElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLDataElementPrototype>(realm, "HTMLDataElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLDataListElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLDataListElementPrototype>(realm, "HTMLDataListElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLDetailsElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLDetailsElementPrototype>(realm, "HTMLDetailsElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLDialogElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLDialogElementPrototype>(realm, "HTMLDialogElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLDirectoryElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLDirectoryElementPrototype>(realm, "HTMLDirectoryElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLDivElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLDivElementPrototype>(realm, "HTMLDivElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLDocument"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLDocumentPrototype>(realm, "HTMLDocument"_fly_string); });
    global.define_intrinsic_accessor("HTMLDListElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLDListElementPrototype>(realm, "HTMLDListElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLElementPrototype>(realm, "HTMLElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLEmbedElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLEmbedElementPrototype>(realm, "HTMLEmbedElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLFieldSetElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLFieldSetElementPrototype>(realm, "HTMLFieldSetElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLFontElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLFontElementPrototype>(realm, "HTMLFontElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLFormControlsCollection"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLFormControlsCollectionPrototype>(realm, "HTMLFormControlsCollection"_fly_string); });
    global.define_intrinsic_accessor("HTMLFormElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLFormElementPrototype>(realm, "HTMLFormElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLFrameElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLFrameElementPrototype>(realm, "HTMLFrameElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLFrameSetElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLFrameSetElementPrototype>(realm, "HTMLFrameSetElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLHeadElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLHeadElementPrototype>(realm, "HTMLHeadElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLHeadingElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLHeadingElementPrototype>(realm, "HTMLHeadingElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLHRElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLHRElementPrototype>(realm, "HTMLHRElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLHtmlElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLHtmlElementPrototype>(realm, "HTMLHtmlElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLIFrameElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLIFrameElementPrototype>(realm, "HTMLIFrameElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLImageElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLImageElementPrototype>(realm, "HTMLImageElement"_fly_string); });
    global.define_intrinsic_accessor("Image"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLImageElementPrototype>(realm, "Image"_fly_string); });
    global.define_intrinsic_accessor("HTMLInputElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLInputElementPrototype>(realm, "HTMLInputElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLLabelElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLLabelElementPrototype>(realm, "HTMLLabelElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLLegendElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLLegendElementPrototype>(realm, "HTMLLegendElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLLIElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLLIElementPrototype>(realm, "HTMLLIElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLLinkElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLLinkElementPrototype>(realm, "HTMLLinkElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLMapElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLMapElementPrototype>(realm, "HTMLMapElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLMarqueeElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLMarqueeElementPrototype>(realm, "HTMLMarqueeElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLMediaElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLMediaElementPrototype>(realm, "HTMLMediaElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLMenuElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLMenuElementPrototype>(realm, "HTMLMenuElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLMetaElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLMetaElementPrototype>(realm, "HTMLMetaElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLMeterElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLMeterElementPrototype>(realm, "HTMLMeterElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLModElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLModElementPrototype>(realm, "HTMLModElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLObjectElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLObjectElementPrototype>(realm, "HTMLObjectElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLOListElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLOListElementPrototype>(realm, "HTMLOListElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLOptGroupElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLOptGroupElementPrototype>(realm, "HTMLOptGroupElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLOptionElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLOptionElementPrototype>(realm, "HTMLOptionElement"_fly_string); });
    global.define_intrinsic_accessor("Option"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLOptionElementPrototype>(realm, "Option"_fly_string); });
    global.define_intrinsic_accessor("HTMLOptionsCollection"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLOptionsCollectionPrototype>(realm, "HTMLOptionsCollection"_fly_string); });
    global.define_intrinsic_accessor("HTMLOutputElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLOutputElementPrototype>(realm, "HTMLOutputElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLParagraphElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLParagraphElementPrototype>(realm, "HTMLParagraphElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLParamElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLParamElementPrototype>(realm, "HTMLParamElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLPictureElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLPictureElementPrototype>(realm, "HTMLPictureElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLPreElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLPreElementPrototype>(realm, "HTMLPreElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLProgressElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLProgressElementPrototype>(realm, "HTMLProgressElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLQuoteElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLQuoteElementPrototype>(realm, "HTMLQuoteElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLScriptElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLScriptElementPrototype>(realm, "HTMLScriptElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLSelectedContentElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLSelectedContentElementPrototype>(realm, "HTMLSelectedContentElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLSelectElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLSelectElementPrototype>(realm, "HTMLSelectElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLSlotElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLSlotElementPrototype>(realm, "HTMLSlotElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLSourceElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLSourceElementPrototype>(realm, "HTMLSourceElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLSpanElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLSpanElementPrototype>(realm, "HTMLSpanElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLStyleElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLStyleElementPrototype>(realm, "HTMLStyleElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLTableCaptionElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLTableCaptionElementPrototype>(realm, "HTMLTableCaptionElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLTableCellElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLTableCellElementPrototype>(realm, "HTMLTableCellElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLTableColElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLTableColElementPrototype>(realm, "HTMLTableColElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLTableElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLTableElementPrototype>(realm, "HTMLTableElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLTableRowElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLTableRowElementPrototype>(realm, "HTMLTableRowElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLTableSectionElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLTableSectionElementPrototype>(realm, "HTMLTableSectionElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLTemplateElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLTemplateElementPrototype>(realm, "HTMLTemplateElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLTextAreaElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLTextAreaElementPrototype>(realm, "HTMLTextAreaElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLTimeElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLTimeElementPrototype>(realm, "HTMLTimeElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLTitleElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLTitleElementPrototype>(realm, "HTMLTitleElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLTrackElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLTrackElementPrototype>(realm, "HTMLTrackElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLUListElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLUListElementPrototype>(realm, "HTMLUListElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLUnknownElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLUnknownElementPrototype>(realm, "HTMLUnknownElement"_fly_string); });
    global.define_intrinsic_accessor("HTMLVideoElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<HTMLVideoElementPrototype>(realm, "HTMLVideoElement"_fly_string); });
    global.define_intrinsic_accessor("ImageBitmap"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ImageBitmapPrototype>(realm, "ImageBitmap"_fly_string); });
    global.define_intrinsic_accessor("ImageData"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ImageDataPrototype>(realm, "ImageData"_fly_string); });
    global.define_intrinsic_accessor("Location"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<LocationPrototype>(realm, "Location"_fly_string); });
    global.define_intrinsic_accessor("MediaError"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MediaErrorPrototype>(realm, "MediaError"_fly_string); });
    global.define_intrinsic_accessor("MessageChannel"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MessageChannelPrototype>(realm, "MessageChannel"_fly_string); });
    global.define_intrinsic_accessor("MessageEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MessageEventPrototype>(realm, "MessageEvent"_fly_string); });
    global.define_intrinsic_accessor("MessagePort"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MessagePortPrototype>(realm, "MessagePort"_fly_string); });
    global.define_intrinsic_accessor("MimeType"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MimeTypePrototype>(realm, "MimeType"_fly_string); });
    global.define_intrinsic_accessor("MimeTypeArray"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MimeTypeArrayPrototype>(realm, "MimeTypeArray"_fly_string); });
    global.define_intrinsic_accessor("NavigateEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<NavigateEventPrototype>(realm, "NavigateEvent"_fly_string); });
    global.define_intrinsic_accessor("Navigation"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<NavigationPrototype>(realm, "Navigation"_fly_string); });
    global.define_intrinsic_accessor("NavigationCurrentEntryChangeEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<NavigationCurrentEntryChangeEventPrototype>(realm, "NavigationCurrentEntryChangeEvent"_fly_string); });
    global.define_intrinsic_accessor("NavigationDestination"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<NavigationDestinationPrototype>(realm, "NavigationDestination"_fly_string); });
    global.define_intrinsic_accessor("NavigationHistoryEntry"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<NavigationHistoryEntryPrototype>(realm, "NavigationHistoryEntry"_fly_string); });
    global.define_intrinsic_accessor("NavigationTransition"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<NavigationTransitionPrototype>(realm, "NavigationTransition"_fly_string); });
    global.define_intrinsic_accessor("Navigator"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<NavigatorPrototype>(realm, "Navigator"_fly_string); });
    if (expose_experimental_interfaces) {
    global.define_intrinsic_accessor("OffscreenCanvas"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<OffscreenCanvasPrototype>(realm, "OffscreenCanvas"_fly_string); });
    }
    if (expose_experimental_interfaces) {
    global.define_intrinsic_accessor("OffscreenCanvasRenderingContext2D"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<OffscreenCanvasRenderingContext2DPrototype>(realm, "OffscreenCanvasRenderingContext2D"_fly_string); });
    }
    global.define_intrinsic_accessor("PageTransitionEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PageTransitionEventPrototype>(realm, "PageTransitionEvent"_fly_string); });
    global.define_intrinsic_accessor("Path2D"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<Path2DPrototype>(realm, "Path2D"_fly_string); });
    global.define_intrinsic_accessor("Plugin"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PluginPrototype>(realm, "Plugin"_fly_string); });
    global.define_intrinsic_accessor("PluginArray"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PluginArrayPrototype>(realm, "PluginArray"_fly_string); });
    global.define_intrinsic_accessor("PopStateEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PopStateEventPrototype>(realm, "PopStateEvent"_fly_string); });
    global.define_intrinsic_accessor("PromiseRejectionEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PromiseRejectionEventPrototype>(realm, "PromiseRejectionEvent"_fly_string); });
    global.define_intrinsic_accessor("RadioNodeList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<RadioNodeListPrototype>(realm, "RadioNodeList"_fly_string); });
    global.define_intrinsic_accessor("SharedWorker"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SharedWorkerPrototype>(realm, "SharedWorker"_fly_string); });
    global.define_intrinsic_accessor("Storage"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<StoragePrototype>(realm, "Storage"_fly_string); });
    global.define_intrinsic_accessor("StorageEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<StorageEventPrototype>(realm, "StorageEvent"_fly_string); });
    global.define_intrinsic_accessor("SubmitEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SubmitEventPrototype>(realm, "SubmitEvent"_fly_string); });
    global.define_intrinsic_accessor("TextMetrics"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TextMetricsPrototype>(realm, "TextMetrics"_fly_string); });
    global.define_intrinsic_accessor("TextTrack"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TextTrackPrototype>(realm, "TextTrack"_fly_string); });
    global.define_intrinsic_accessor("TextTrackCue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TextTrackCuePrototype>(realm, "TextTrackCue"_fly_string); });
    global.define_intrinsic_accessor("TextTrackCueList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TextTrackCueListPrototype>(realm, "TextTrackCueList"_fly_string); });
    global.define_intrinsic_accessor("TextTrackList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TextTrackListPrototype>(realm, "TextTrackList"_fly_string); });
    global.define_intrinsic_accessor("TimeRanges"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TimeRangesPrototype>(realm, "TimeRanges"_fly_string); });
    global.define_intrinsic_accessor("ToggleEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ToggleEventPrototype>(realm, "ToggleEvent"_fly_string); });
    global.define_intrinsic_accessor("TrackEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TrackEventPrototype>(realm, "TrackEvent"_fly_string); });
    global.define_intrinsic_accessor("UserActivation"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<UserActivationPrototype>(realm, "UserActivation"_fly_string); });
    global.define_intrinsic_accessor("ValidityState"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ValidityStatePrototype>(realm, "ValidityState"_fly_string); });
    global.define_intrinsic_accessor("VideoTrack"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<VideoTrackPrototype>(realm, "VideoTrack"_fly_string); });
    global.define_intrinsic_accessor("VideoTrackList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<VideoTrackListPrototype>(realm, "VideoTrackList"_fly_string); });
    global.define_intrinsic_accessor("Window"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WindowPrototype>(realm, "Window"_fly_string); });
    global.define_intrinsic_accessor("Worker"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WorkerPrototype>(realm, "Worker"_fly_string); });
    global.define_intrinsic_accessor("XMLSerializer"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<XMLSerializerPrototype>(realm, "XMLSerializer"_fly_string); });
    global.define_intrinsic_accessor("Performance"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PerformancePrototype>(realm, "Performance"_fly_string); });
    global.define_intrinsic_accessor("IDBCursor"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBCursorPrototype>(realm, "IDBCursor"_fly_string); });
    global.define_intrinsic_accessor("IDBCursorWithValue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBCursorWithValuePrototype>(realm, "IDBCursorWithValue"_fly_string); });
    global.define_intrinsic_accessor("IDBDatabase"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBDatabasePrototype>(realm, "IDBDatabase"_fly_string); });
    global.define_intrinsic_accessor("IDBFactory"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBFactoryPrototype>(realm, "IDBFactory"_fly_string); });
    global.define_intrinsic_accessor("IDBIndex"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBIndexPrototype>(realm, "IDBIndex"_fly_string); });
    global.define_intrinsic_accessor("IDBKeyRange"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBKeyRangePrototype>(realm, "IDBKeyRange"_fly_string); });
    global.define_intrinsic_accessor("IDBObjectStore"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBObjectStorePrototype>(realm, "IDBObjectStore"_fly_string); });
    global.define_intrinsic_accessor("IDBOpenDBRequest"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBOpenDBRequestPrototype>(realm, "IDBOpenDBRequest"_fly_string); });
    global.define_intrinsic_accessor("IDBRecord"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBRecordPrototype>(realm, "IDBRecord"_fly_string); });
    global.define_intrinsic_accessor("IDBRequest"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBRequestPrototype>(realm, "IDBRequest"_fly_string); });
    global.define_intrinsic_accessor("IDBTransaction"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBTransactionPrototype>(realm, "IDBTransaction"_fly_string); });
    global.define_intrinsic_accessor("IDBVersionChangeEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IDBVersionChangeEventPrototype>(realm, "IDBVersionChangeEvent"_fly_string); });
    global.define_intrinsic_accessor("IntersectionObserver"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IntersectionObserverPrototype>(realm, "IntersectionObserver"_fly_string); });
    global.define_intrinsic_accessor("IntersectionObserverEntry"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IntersectionObserverEntryPrototype>(realm, "IntersectionObserverEntry"_fly_string); });
    global.define_intrinsic_accessor("MathMLElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MathMLElementPrototype>(realm, "MathMLElement"_fly_string); });
    global.define_intrinsic_accessor("MediaCapabilities"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MediaCapabilitiesPrototype>(realm, "MediaCapabilities"_fly_string); });
    global.define_intrinsic_accessor("BufferedChangeEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<BufferedChangeEventPrototype>(realm, "BufferedChangeEvent"_fly_string); });
    if (expose_experimental_interfaces) {
    global.define_intrinsic_accessor("ManagedMediaSource"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ManagedMediaSourcePrototype>(realm, "ManagedMediaSource"_fly_string); });
    }
    global.define_intrinsic_accessor("ManagedSourceBuffer"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ManagedSourceBufferPrototype>(realm, "ManagedSourceBuffer"_fly_string); });
    if (expose_experimental_interfaces) {
    global.define_intrinsic_accessor("MediaSource"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MediaSourcePrototype>(realm, "MediaSource"_fly_string); });
    }
    if (expose_experimental_interfaces) {
    global.define_intrinsic_accessor("MediaSourceHandle"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MediaSourceHandlePrototype>(realm, "MediaSourceHandle"_fly_string); });
    }
    global.define_intrinsic_accessor("SourceBuffer"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SourceBufferPrototype>(realm, "SourceBuffer"_fly_string); });
    global.define_intrinsic_accessor("SourceBufferList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SourceBufferListPrototype>(realm, "SourceBufferList"_fly_string); });
    global.define_intrinsic_accessor("PerformanceNavigation"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PerformanceNavigationPrototype>(realm, "PerformanceNavigation"_fly_string); });
    global.define_intrinsic_accessor("PerformanceTiming"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PerformanceTimingPrototype>(realm, "PerformanceTiming"_fly_string); });
    global.define_intrinsic_accessor("Notification"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<NotificationPrototype>(realm, "Notification"_fly_string); });
    global.define_intrinsic_accessor("PerformanceEntry"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PerformanceEntryPrototype>(realm, "PerformanceEntry"_fly_string); });
    global.define_intrinsic_accessor("PerformanceObserver"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PerformanceObserverPrototype>(realm, "PerformanceObserver"_fly_string); });
    global.define_intrinsic_accessor("PerformanceObserverEntryList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PerformanceObserverEntryListPrototype>(realm, "PerformanceObserverEntryList"_fly_string); });
    global.define_intrinsic_accessor("IdleDeadline"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<IdleDeadlinePrototype>(realm, "IdleDeadline"_fly_string); });
    global.define_intrinsic_accessor("ResizeObserver"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ResizeObserverPrototype>(realm, "ResizeObserver"_fly_string); });
    global.define_intrinsic_accessor("ResizeObserverEntry"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ResizeObserverEntryPrototype>(realm, "ResizeObserverEntry"_fly_string); });
    global.define_intrinsic_accessor("ResizeObserverSize"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ResizeObserverSizePrototype>(realm, "ResizeObserverSize"_fly_string); });
    global.define_intrinsic_accessor("PerformanceResourceTiming"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PerformanceResourceTimingPrototype>(realm, "PerformanceResourceTiming"_fly_string); });
    if (is_secure_context) {
    global.define_intrinsic_accessor("Serial"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SerialPrototype>(realm, "Serial"_fly_string); });
    }
    if (is_secure_context) {
    global.define_intrinsic_accessor("SerialPort"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SerialPortPrototype>(realm, "SerialPort"_fly_string); });
    }
    if (is_secure_context) {
    if (expose_experimental_interfaces) {
    global.define_intrinsic_accessor("CacheStorage"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CacheStoragePrototype>(realm, "CacheStorage"_fly_string); });
    }
    }
    if (is_secure_context) {
    global.define_intrinsic_accessor("ServiceWorker"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ServiceWorkerPrototype>(realm, "ServiceWorker"_fly_string); });
    }
    if (is_secure_context) {
    global.define_intrinsic_accessor("ServiceWorkerContainer"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ServiceWorkerContainerPrototype>(realm, "ServiceWorkerContainer"_fly_string); });
    }
    if (is_secure_context) {
    global.define_intrinsic_accessor("ServiceWorkerRegistration"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ServiceWorkerRegistrationPrototype>(realm, "ServiceWorkerRegistration"_fly_string); });
    }
    global.define_intrinsic_accessor("SpeechGrammar"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SpeechGrammarPrototype>(realm, "SpeechGrammar"_fly_string); });
    global.define_intrinsic_accessor("SpeechGrammarList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SpeechGrammarListPrototype>(realm, "SpeechGrammarList"_fly_string); });
    global.define_intrinsic_accessor("SpeechRecognition"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SpeechRecognitionPrototype>(realm, "SpeechRecognition"_fly_string); });
    global.define_intrinsic_accessor("SpeechRecognitionAlternative"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SpeechRecognitionAlternativePrototype>(realm, "SpeechRecognitionAlternative"_fly_string); });
    global.define_intrinsic_accessor("SpeechRecognitionEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SpeechRecognitionEventPrototype>(realm, "SpeechRecognitionEvent"_fly_string); });
    global.define_intrinsic_accessor("SpeechRecognitionPhrase"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SpeechRecognitionPhrasePrototype>(realm, "SpeechRecognitionPhrase"_fly_string); });
    global.define_intrinsic_accessor("SpeechRecognitionResult"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SpeechRecognitionResultPrototype>(realm, "SpeechRecognitionResult"_fly_string); });
    global.define_intrinsic_accessor("SpeechRecognitionResultList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SpeechRecognitionResultListPrototype>(realm, "SpeechRecognitionResultList"_fly_string); });
    global.define_intrinsic_accessor("SpeechSynthesis"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SpeechSynthesisPrototype>(realm, "SpeechSynthesis"_fly_string); });
    global.define_intrinsic_accessor("SpeechSynthesisUtterance"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SpeechSynthesisUtterancePrototype>(realm, "SpeechSynthesisUtterance"_fly_string); });
    global.define_intrinsic_accessor("SpeechSynthesisVoice"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SpeechSynthesisVoicePrototype>(realm, "SpeechSynthesisVoice"_fly_string); });
    global.define_intrinsic_accessor("ByteLengthQueuingStrategy"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ByteLengthQueuingStrategyPrototype>(realm, "ByteLengthQueuingStrategy"_fly_string); });
    global.define_intrinsic_accessor("CountQueuingStrategy"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CountQueuingStrategyPrototype>(realm, "CountQueuingStrategy"_fly_string); });
    global.define_intrinsic_accessor("ReadableByteStreamController"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ReadableByteStreamControllerPrototype>(realm, "ReadableByteStreamController"_fly_string); });
    global.define_intrinsic_accessor("ReadableStream"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ReadableStreamPrototype>(realm, "ReadableStream"_fly_string); });
    global.define_intrinsic_accessor("ReadableStreamBYOBReader"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ReadableStreamBYOBReaderPrototype>(realm, "ReadableStreamBYOBReader"_fly_string); });
    global.define_intrinsic_accessor("ReadableStreamBYOBRequest"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ReadableStreamBYOBRequestPrototype>(realm, "ReadableStreamBYOBRequest"_fly_string); });
    global.define_intrinsic_accessor("ReadableStreamDefaultController"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ReadableStreamDefaultControllerPrototype>(realm, "ReadableStreamDefaultController"_fly_string); });
    global.define_intrinsic_accessor("ReadableStreamDefaultReader"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ReadableStreamDefaultReaderPrototype>(realm, "ReadableStreamDefaultReader"_fly_string); });
    global.define_intrinsic_accessor("TransformStream"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TransformStreamPrototype>(realm, "TransformStream"_fly_string); });
    global.define_intrinsic_accessor("TransformStreamDefaultController"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TransformStreamDefaultControllerPrototype>(realm, "TransformStreamDefaultController"_fly_string); });
    global.define_intrinsic_accessor("WritableStream"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WritableStreamPrototype>(realm, "WritableStream"_fly_string); });
    global.define_intrinsic_accessor("WritableStreamDefaultController"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WritableStreamDefaultControllerPrototype>(realm, "WritableStreamDefaultController"_fly_string); });
    global.define_intrinsic_accessor("WritableStreamDefaultWriter"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WritableStreamDefaultWriterPrototype>(realm, "WritableStreamDefaultWriter"_fly_string); });
    global.define_intrinsic_accessor("TrustedHTML"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TrustedHTMLPrototype>(realm, "TrustedHTML"_fly_string); });
    global.define_intrinsic_accessor("TrustedScript"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TrustedScriptPrototype>(realm, "TrustedScript"_fly_string); });
    global.define_intrinsic_accessor("TrustedScriptURL"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TrustedScriptURLPrototype>(realm, "TrustedScriptURL"_fly_string); });
    global.define_intrinsic_accessor("TrustedTypePolicy"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TrustedTypePolicyPrototype>(realm, "TrustedTypePolicy"_fly_string); });
    global.define_intrinsic_accessor("TrustedTypePolicyFactory"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TrustedTypePolicyFactoryPrototype>(realm, "TrustedTypePolicyFactory"_fly_string); });
    global.define_intrinsic_accessor("SVGAElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGAElementPrototype>(realm, "SVGAElement"_fly_string); });
    global.define_intrinsic_accessor("SVGAnimatedEnumeration"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGAnimatedEnumerationPrototype>(realm, "SVGAnimatedEnumeration"_fly_string); });
    global.define_intrinsic_accessor("SVGAnimatedInteger"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGAnimatedIntegerPrototype>(realm, "SVGAnimatedInteger"_fly_string); });
    global.define_intrinsic_accessor("SVGAnimatedLength"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGAnimatedLengthPrototype>(realm, "SVGAnimatedLength"_fly_string); });
    global.define_intrinsic_accessor("SVGAnimatedLengthList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGAnimatedLengthListPrototype>(realm, "SVGAnimatedLengthList"_fly_string); });
    global.define_intrinsic_accessor("SVGAnimatedNumber"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGAnimatedNumberPrototype>(realm, "SVGAnimatedNumber"_fly_string); });
    global.define_intrinsic_accessor("SVGAnimatedNumberList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGAnimatedNumberListPrototype>(realm, "SVGAnimatedNumberList"_fly_string); });
    global.define_intrinsic_accessor("SVGAnimatedRect"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGAnimatedRectPrototype>(realm, "SVGAnimatedRect"_fly_string); });
    global.define_intrinsic_accessor("SVGAnimatedString"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGAnimatedStringPrototype>(realm, "SVGAnimatedString"_fly_string); });
    global.define_intrinsic_accessor("SVGAnimatedTransformList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGAnimatedTransformListPrototype>(realm, "SVGAnimatedTransformList"_fly_string); });
    global.define_intrinsic_accessor("SVGAnimationElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGAnimationElementPrototype>(realm, "SVGAnimationElement"_fly_string); });
    global.define_intrinsic_accessor("SVGClipPathElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGClipPathElementPrototype>(realm, "SVGClipPathElement"_fly_string); });
    global.define_intrinsic_accessor("SVGComponentTransferFunctionElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGComponentTransferFunctionElementPrototype>(realm, "SVGComponentTransferFunctionElement"_fly_string); });
    global.define_intrinsic_accessor("SVGDefsElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGDefsElementPrototype>(realm, "SVGDefsElement"_fly_string); });
    global.define_intrinsic_accessor("SVGDescElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGDescElementPrototype>(realm, "SVGDescElement"_fly_string); });
    global.define_intrinsic_accessor("SVGElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGElementPrototype>(realm, "SVGElement"_fly_string); });
    global.define_intrinsic_accessor("SVGGElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGGElementPrototype>(realm, "SVGGElement"_fly_string); });
    global.define_intrinsic_accessor("SVGGeometryElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGGeometryElementPrototype>(realm, "SVGGeometryElement"_fly_string); });
    global.define_intrinsic_accessor("SVGGradientElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGGradientElementPrototype>(realm, "SVGGradientElement"_fly_string); });
    global.define_intrinsic_accessor("SVGGraphicsElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGGraphicsElementPrototype>(realm, "SVGGraphicsElement"_fly_string); });
    global.define_intrinsic_accessor("SVGImageElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGImageElementPrototype>(realm, "SVGImageElement"_fly_string); });
    global.define_intrinsic_accessor("SVGCircleElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGCircleElementPrototype>(realm, "SVGCircleElement"_fly_string); });
    global.define_intrinsic_accessor("SVGEllipseElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGEllipseElementPrototype>(realm, "SVGEllipseElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFEBlendElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFEBlendElementPrototype>(realm, "SVGFEBlendElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFEColorMatrixElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFEColorMatrixElementPrototype>(realm, "SVGFEColorMatrixElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFEComponentTransferElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFEComponentTransferElementPrototype>(realm, "SVGFEComponentTransferElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFECompositeElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFECompositeElementPrototype>(realm, "SVGFECompositeElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFEDisplacementMapElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFEDisplacementMapElementPrototype>(realm, "SVGFEDisplacementMapElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFEDropShadowElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFEDropShadowElementPrototype>(realm, "SVGFEDropShadowElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFEFloodElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFEFloodElementPrototype>(realm, "SVGFEFloodElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFEFuncAElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFEFuncAElementPrototype>(realm, "SVGFEFuncAElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFEFuncBElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFEFuncBElementPrototype>(realm, "SVGFEFuncBElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFEFuncGElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFEFuncGElementPrototype>(realm, "SVGFEFuncGElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFEFuncRElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFEFuncRElementPrototype>(realm, "SVGFEFuncRElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFEGaussianBlurElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFEGaussianBlurElementPrototype>(realm, "SVGFEGaussianBlurElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFEImageElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFEImageElementPrototype>(realm, "SVGFEImageElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFEMergeElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFEMergeElementPrototype>(realm, "SVGFEMergeElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFEMergeNodeElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFEMergeNodeElementPrototype>(realm, "SVGFEMergeNodeElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFEMorphologyElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFEMorphologyElementPrototype>(realm, "SVGFEMorphologyElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFEOffsetElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFEOffsetElementPrototype>(realm, "SVGFEOffsetElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFETurbulenceElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFETurbulenceElementPrototype>(realm, "SVGFETurbulenceElement"_fly_string); });
    global.define_intrinsic_accessor("SVGFilterElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGFilterElementPrototype>(realm, "SVGFilterElement"_fly_string); });
    global.define_intrinsic_accessor("SVGForeignObjectElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGForeignObjectElementPrototype>(realm, "SVGForeignObjectElement"_fly_string); });
    global.define_intrinsic_accessor("SVGLength"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGLengthPrototype>(realm, "SVGLength"_fly_string); });
    global.define_intrinsic_accessor("SVGLengthList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGLengthListPrototype>(realm, "SVGLengthList"_fly_string); });
    global.define_intrinsic_accessor("SVGLineElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGLineElementPrototype>(realm, "SVGLineElement"_fly_string); });
    global.define_intrinsic_accessor("SVGLinearGradientElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGLinearGradientElementPrototype>(realm, "SVGLinearGradientElement"_fly_string); });
    global.define_intrinsic_accessor("SVGMaskElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGMaskElementPrototype>(realm, "SVGMaskElement"_fly_string); });
    global.define_intrinsic_accessor("SVGMetadataElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGMetadataElementPrototype>(realm, "SVGMetadataElement"_fly_string); });
    global.define_intrinsic_accessor("SVGNumber"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGNumberPrototype>(realm, "SVGNumber"_fly_string); });
    global.define_intrinsic_accessor("SVGNumberList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGNumberListPrototype>(realm, "SVGNumberList"_fly_string); });
    global.define_intrinsic_accessor("SVGPathElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGPathElementPrototype>(realm, "SVGPathElement"_fly_string); });
    global.define_intrinsic_accessor("SVGPatternElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGPatternElementPrototype>(realm, "SVGPatternElement"_fly_string); });
    global.define_intrinsic_accessor("SVGPolygonElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGPolygonElementPrototype>(realm, "SVGPolygonElement"_fly_string); });
    global.define_intrinsic_accessor("SVGPolylineElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGPolylineElementPrototype>(realm, "SVGPolylineElement"_fly_string); });
    global.define_intrinsic_accessor("SVGRadialGradientElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGRadialGradientElementPrototype>(realm, "SVGRadialGradientElement"_fly_string); });
    global.define_intrinsic_accessor("SVGRectElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGRectElementPrototype>(realm, "SVGRectElement"_fly_string); });
    global.define_intrinsic_accessor("SVGScriptElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGScriptElementPrototype>(realm, "SVGScriptElement"_fly_string); });
    global.define_intrinsic_accessor("SVGSVGElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGSVGElementPrototype>(realm, "SVGSVGElement"_fly_string); });
    global.define_intrinsic_accessor("SVGStopElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGStopElementPrototype>(realm, "SVGStopElement"_fly_string); });
    global.define_intrinsic_accessor("SVGStyleElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGStyleElementPrototype>(realm, "SVGStyleElement"_fly_string); });
    global.define_intrinsic_accessor("SVGSymbolElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGSymbolElementPrototype>(realm, "SVGSymbolElement"_fly_string); });
    global.define_intrinsic_accessor("SVGTextContentElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGTextContentElementPrototype>(realm, "SVGTextContentElement"_fly_string); });
    global.define_intrinsic_accessor("SVGTextElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGTextElementPrototype>(realm, "SVGTextElement"_fly_string); });
    global.define_intrinsic_accessor("SVGTextPathElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGTextPathElementPrototype>(realm, "SVGTextPathElement"_fly_string); });
    global.define_intrinsic_accessor("SVGTextPositioningElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGTextPositioningElementPrototype>(realm, "SVGTextPositioningElement"_fly_string); });
    global.define_intrinsic_accessor("SVGTitleElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGTitleElementPrototype>(realm, "SVGTitleElement"_fly_string); });
    global.define_intrinsic_accessor("SVGTransform"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGTransformPrototype>(realm, "SVGTransform"_fly_string); });
    global.define_intrinsic_accessor("SVGTransformList"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGTransformListPrototype>(realm, "SVGTransformList"_fly_string); });
    global.define_intrinsic_accessor("SVGTSpanElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGTSpanElementPrototype>(realm, "SVGTSpanElement"_fly_string); });
    global.define_intrinsic_accessor("SVGUnitTypes"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGUnitTypesPrototype>(realm, "SVGUnitTypes"_fly_string); });
    global.define_intrinsic_accessor("SVGUseElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGUseElementPrototype>(realm, "SVGUseElement"_fly_string); });
    global.define_intrinsic_accessor("SVGViewElement"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SVGViewElementPrototype>(realm, "SVGViewElement"_fly_string); });
    global.define_intrinsic_accessor("Selection"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<SelectionPrototype>(realm, "Selection"_fly_string); });
    if (is_secure_context) {
    global.define_intrinsic_accessor("StorageManager"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<StorageManagerPrototype>(realm, "StorageManager"_fly_string); });
    }
    global.define_intrinsic_accessor("CompositionEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<CompositionEventPrototype>(realm, "CompositionEvent"_fly_string); });
    global.define_intrinsic_accessor("FocusEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FocusEventPrototype>(realm, "FocusEvent"_fly_string); });
    global.define_intrinsic_accessor("InputEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<InputEventPrototype>(realm, "InputEvent"_fly_string); });
    global.define_intrinsic_accessor("KeyboardEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<KeyboardEventPrototype>(realm, "KeyboardEvent"_fly_string); });
    global.define_intrinsic_accessor("MouseEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MouseEventPrototype>(realm, "MouseEvent"_fly_string); });
    global.define_intrinsic_accessor("PointerEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PointerEventPrototype>(realm, "PointerEvent"_fly_string); });
    global.define_intrinsic_accessor("TextEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<TextEventPrototype>(realm, "TextEvent"_fly_string); });
    global.define_intrinsic_accessor("UIEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<UIEventPrototype>(realm, "UIEvent"_fly_string); });
    global.define_intrinsic_accessor("WheelEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WheelEventPrototype>(realm, "WheelEvent"_fly_string); });
    global.define_intrinsic_accessor("URLPattern"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<URLPatternPrototype>(realm, "URLPattern"_fly_string); });
    global.define_intrinsic_accessor("PerformanceMark"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PerformanceMarkPrototype>(realm, "PerformanceMark"_fly_string); });
    global.define_intrinsic_accessor("PerformanceMeasure"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PerformanceMeasurePrototype>(realm, "PerformanceMeasure"_fly_string); });
    global.define_intrinsic_accessor("ViewTransition"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ViewTransitionPrototype>(realm, "ViewTransition"_fly_string); });
    global.define_intrinsic_accessor("AnalyserNode"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AnalyserNodePrototype>(realm, "AnalyserNode"_fly_string); });
    global.define_intrinsic_accessor("AudioBuffer"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AudioBufferPrototype>(realm, "AudioBuffer"_fly_string); });
    global.define_intrinsic_accessor("AudioBufferSourceNode"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AudioBufferSourceNodePrototype>(realm, "AudioBufferSourceNode"_fly_string); });
    global.define_intrinsic_accessor("AudioContext"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AudioContextPrototype>(realm, "AudioContext"_fly_string); });
    global.define_intrinsic_accessor("AudioDestinationNode"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AudioDestinationNodePrototype>(realm, "AudioDestinationNode"_fly_string); });
    global.define_intrinsic_accessor("AudioListener"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AudioListenerPrototype>(realm, "AudioListener"_fly_string); });
    global.define_intrinsic_accessor("AudioNode"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AudioNodePrototype>(realm, "AudioNode"_fly_string); });
    global.define_intrinsic_accessor("AudioParam"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AudioParamPrototype>(realm, "AudioParam"_fly_string); });
    global.define_intrinsic_accessor("AudioScheduledSourceNode"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<AudioScheduledSourceNodePrototype>(realm, "AudioScheduledSourceNode"_fly_string); });
    global.define_intrinsic_accessor("BaseAudioContext"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<BaseAudioContextPrototype>(realm, "BaseAudioContext"_fly_string); });
    global.define_intrinsic_accessor("BiquadFilterNode"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<BiquadFilterNodePrototype>(realm, "BiquadFilterNode"_fly_string); });
    global.define_intrinsic_accessor("DynamicsCompressorNode"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DynamicsCompressorNodePrototype>(realm, "DynamicsCompressorNode"_fly_string); });
    global.define_intrinsic_accessor("ChannelMergerNode"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ChannelMergerNodePrototype>(realm, "ChannelMergerNode"_fly_string); });
    global.define_intrinsic_accessor("ChannelSplitterNode"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ChannelSplitterNodePrototype>(realm, "ChannelSplitterNode"_fly_string); });
    global.define_intrinsic_accessor("ConstantSourceNode"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ConstantSourceNodePrototype>(realm, "ConstantSourceNode"_fly_string); });
    global.define_intrinsic_accessor("DelayNode"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DelayNodePrototype>(realm, "DelayNode"_fly_string); });
    global.define_intrinsic_accessor("GainNode"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<GainNodePrototype>(realm, "GainNode"_fly_string); });
    global.define_intrinsic_accessor("MediaElementAudioSourceNode"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<MediaElementAudioSourceNodePrototype>(realm, "MediaElementAudioSourceNode"_fly_string); });
    global.define_intrinsic_accessor("OfflineAudioCompletionEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<OfflineAudioCompletionEventPrototype>(realm, "OfflineAudioCompletionEvent"_fly_string); });
    global.define_intrinsic_accessor("OfflineAudioContext"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<OfflineAudioContextPrototype>(realm, "OfflineAudioContext"_fly_string); });
    global.define_intrinsic_accessor("OscillatorNode"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<OscillatorNodePrototype>(realm, "OscillatorNode"_fly_string); });
    global.define_intrinsic_accessor("PannerNode"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PannerNodePrototype>(realm, "PannerNode"_fly_string); });
    global.define_intrinsic_accessor("PeriodicWave"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<PeriodicWavePrototype>(realm, "PeriodicWave"_fly_string); });
    global.define_intrinsic_accessor("ScriptProcessorNode"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ScriptProcessorNodePrototype>(realm, "ScriptProcessorNode"_fly_string); });
    global.define_intrinsic_accessor("StereoPannerNode"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<StereoPannerNodePrototype>(realm, "StereoPannerNode"_fly_string); });
    global.define_intrinsic_accessor("WebGL2RenderingContext"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGL2RenderingContextPrototype>(realm, "WebGL2RenderingContext"_fly_string); });
    global.define_intrinsic_accessor("WebGLActiveInfo"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLActiveInfoPrototype>(realm, "WebGLActiveInfo"_fly_string); });
    global.define_intrinsic_accessor("WebGLBuffer"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLBufferPrototype>(realm, "WebGLBuffer"_fly_string); });
    global.define_intrinsic_accessor("WebGLContextEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLContextEventPrototype>(realm, "WebGLContextEvent"_fly_string); });
    global.define_intrinsic_accessor("WebGLFramebuffer"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLFramebufferPrototype>(realm, "WebGLFramebuffer"_fly_string); });
    global.define_intrinsic_accessor("WebGLObject"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLObjectPrototype>(realm, "WebGLObject"_fly_string); });
    global.define_intrinsic_accessor("WebGLProgram"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLProgramPrototype>(realm, "WebGLProgram"_fly_string); });
    global.define_intrinsic_accessor("WebGLQuery"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLQueryPrototype>(realm, "WebGLQuery"_fly_string); });
    global.define_intrinsic_accessor("WebGLRenderbuffer"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLRenderbufferPrototype>(realm, "WebGLRenderbuffer"_fly_string); });
    global.define_intrinsic_accessor("WebGLRenderingContext"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLRenderingContextPrototype>(realm, "WebGLRenderingContext"_fly_string); });
    global.define_intrinsic_accessor("WebGLSampler"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLSamplerPrototype>(realm, "WebGLSampler"_fly_string); });
    global.define_intrinsic_accessor("WebGLShader"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLShaderPrototype>(realm, "WebGLShader"_fly_string); });
    global.define_intrinsic_accessor("WebGLShaderPrecisionFormat"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLShaderPrecisionFormatPrototype>(realm, "WebGLShaderPrecisionFormat"_fly_string); });
    global.define_intrinsic_accessor("WebGLSync"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLSyncPrototype>(realm, "WebGLSync"_fly_string); });
    global.define_intrinsic_accessor("WebGLTexture"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLTexturePrototype>(realm, "WebGLTexture"_fly_string); });
    global.define_intrinsic_accessor("WebGLTransformFeedback"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLTransformFeedbackPrototype>(realm, "WebGLTransformFeedback"_fly_string); });
    global.define_intrinsic_accessor("WebGLUniformLocation"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLUniformLocationPrototype>(realm, "WebGLUniformLocation"_fly_string); });
    global.define_intrinsic_accessor("WebGLVertexArrayObject"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebGLVertexArrayObjectPrototype>(realm, "WebGLVertexArrayObject"_fly_string); });
    global.define_intrinsic_accessor("DOMException"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<DOMExceptionPrototype>(realm, "DOMException"_fly_string); });
    global.define_intrinsic_accessor("QuotaExceededError"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<QuotaExceededErrorPrototype>(realm, "QuotaExceededError"_fly_string); });
    global.define_intrinsic_accessor("WebSocket"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<WebSocketPrototype>(realm, "WebSocket"_fly_string); });
    global.define_intrinsic_accessor("VTTCue"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<VTTCuePrototype>(realm, "VTTCue"_fly_string); });
    global.define_intrinsic_accessor("VTTRegion"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<VTTRegionPrototype>(realm, "VTTRegion"_fly_string); });
    if (is_secure_context) {
    global.define_intrinsic_accessor("XRSession"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<XRSessionPrototype>(realm, "XRSession"_fly_string); });
    }
    if (is_secure_context) {
    global.define_intrinsic_accessor("XRSessionEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<XRSessionEventPrototype>(realm, "XRSessionEvent"_fly_string); });
    }
    if (is_secure_context) {
    global.define_intrinsic_accessor("XRSystem"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<XRSystemPrototype>(realm, "XRSystem"_fly_string); });
    }
    global.define_intrinsic_accessor("FormData"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<FormDataPrototype>(realm, "FormData"_fly_string); });
    global.define_intrinsic_accessor("ProgressEvent"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<ProgressEventPrototype>(realm, "ProgressEvent"_fly_string); });
    global.define_intrinsic_accessor("XMLHttpRequest"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<XMLHttpRequestPrototype>(realm, "XMLHttpRequest"_fly_string); });
    global.define_intrinsic_accessor("XMLHttpRequestEventTarget"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<XMLHttpRequestEventTargetPrototype>(realm, "XMLHttpRequestEventTarget"_fly_string); });
    global.define_intrinsic_accessor("XMLHttpRequestUpload"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<XMLHttpRequestUploadPrototype>(realm, "XMLHttpRequestUpload"_fly_string); });
    global.define_intrinsic_accessor("XPathResult"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<XPathResultPrototype>(realm, "XPathResult"_fly_string); });
    global.define_intrinsic_accessor("XPathExpression"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<XPathExpressionPrototype>(realm, "XPathExpression"_fly_string); });
    global.define_intrinsic_accessor("XPathEvaluator"_utf16_fly_string, attr, [](auto& realm) -> JS::Value { return &ensure_web_constructor<XPathEvaluatorPrototype>(realm, "XPathEvaluator"_fly_string); });
}

}
