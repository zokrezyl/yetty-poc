
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSRulePrototype : public JS::Object {
    JS_OBJECT(CSSRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(css_text_getter);

    JS_DECLARE_NATIVE_FUNCTION(css_text_setter);

    JS_DECLARE_NATIVE_FUNCTION(parent_rule_getter);

    JS_DECLARE_NATIVE_FUNCTION(parent_style_sheet_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);


};


} // namespace Web::Bindings
    