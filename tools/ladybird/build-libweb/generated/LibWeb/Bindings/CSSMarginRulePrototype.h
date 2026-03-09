
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSMarginRulePrototype : public JS::Object {
    JS_OBJECT(CSSMarginRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSMarginRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSMarginRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSMarginRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(style_getter);

    JS_DECLARE_NATIVE_FUNCTION(style_setter);


};


} // namespace Web::Bindings
    