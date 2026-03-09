
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSKeyframesRulePrototype : public JS::Object {
    JS_OBJECT(CSSKeyframesRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSKeyframesRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSKeyframesRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSKeyframesRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(css_rules_getter);

    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    