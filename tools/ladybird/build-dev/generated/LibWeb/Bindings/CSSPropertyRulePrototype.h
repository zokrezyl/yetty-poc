
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSPropertyRulePrototype : public JS::Object {
    JS_OBJECT(CSSPropertyRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSPropertyRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSPropertyRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSPropertyRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(syntax_getter);

    JS_DECLARE_NATIVE_FUNCTION(inherits_getter);

    JS_DECLARE_NATIVE_FUNCTION(initial_value_getter);


};


} // namespace Web::Bindings
    