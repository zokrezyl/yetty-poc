
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSGroupingRulePrototype : public JS::Object {
    JS_OBJECT(CSSGroupingRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSGroupingRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSGroupingRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSGroupingRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(insert_rule);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_rule);
        
    JS_DECLARE_NATIVE_FUNCTION(css_rules_getter);


};


} // namespace Web::Bindings
    