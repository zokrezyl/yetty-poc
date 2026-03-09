
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSConditionRulePrototype : public JS::Object {
    JS_OBJECT(CSSConditionRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSConditionRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSConditionRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSConditionRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(condition_text_getter);


};


} // namespace Web::Bindings
    