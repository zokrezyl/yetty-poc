
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSStyleSheetPrototype : public JS::Object {
    JS_OBJECT(CSSStyleSheetPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSStyleSheetPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSStyleSheetPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSStyleSheetPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(insert_rule);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_rule);
        
    JS_DECLARE_NATIVE_FUNCTION(replace);
        
    JS_DECLARE_NATIVE_FUNCTION(replace_sync);
        
    JS_DECLARE_NATIVE_FUNCTION(add_rule);
        
    JS_DECLARE_NATIVE_FUNCTION(remove_rule);
        
    JS_DECLARE_NATIVE_FUNCTION(owner_rule_getter);

    JS_DECLARE_NATIVE_FUNCTION(css_rules_getter);

    JS_DECLARE_NATIVE_FUNCTION(rules_getter);


};


} // namespace Web::Bindings
    