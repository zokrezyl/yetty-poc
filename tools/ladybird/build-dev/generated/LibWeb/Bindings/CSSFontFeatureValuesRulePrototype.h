
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSFontFeatureValuesRulePrototype : public JS::Object {
    JS_OBJECT(CSSFontFeatureValuesRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSFontFeatureValuesRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSFontFeatureValuesRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSFontFeatureValuesRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(font_family_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_family_setter);

    JS_DECLARE_NATIVE_FUNCTION(annotation_getter);

    JS_DECLARE_NATIVE_FUNCTION(ornaments_getter);

    JS_DECLARE_NATIVE_FUNCTION(stylistic_getter);

    JS_DECLARE_NATIVE_FUNCTION(swash_getter);

    JS_DECLARE_NATIVE_FUNCTION(character_variant_getter);

    JS_DECLARE_NATIVE_FUNCTION(styleset_getter);

    JS_DECLARE_NATIVE_FUNCTION(historical_forms_getter);


};


} // namespace Web::Bindings
    