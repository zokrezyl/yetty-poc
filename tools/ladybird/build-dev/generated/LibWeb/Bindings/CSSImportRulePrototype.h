
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSImportRulePrototype : public JS::Object {
    JS_OBJECT(CSSImportRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSImportRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSImportRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSImportRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(href_getter);

    JS_DECLARE_NATIVE_FUNCTION(media_getter);

    JS_DECLARE_NATIVE_FUNCTION(media_setter);

    JS_DECLARE_NATIVE_FUNCTION(style_sheet_getter);

    JS_DECLARE_NATIVE_FUNCTION(layer_name_getter);

    JS_DECLARE_NATIVE_FUNCTION(supports_text_getter);


};


} // namespace Web::Bindings
    