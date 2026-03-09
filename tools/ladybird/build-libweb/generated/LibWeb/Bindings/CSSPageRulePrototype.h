
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSPageRulePrototype : public JS::Object {
    JS_OBJECT(CSSPageRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSPageRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSPageRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSPageRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(selector_text_getter);

    JS_DECLARE_NATIVE_FUNCTION(selector_text_setter);

    JS_DECLARE_NATIVE_FUNCTION(style_getter);

    JS_DECLARE_NATIVE_FUNCTION(style_setter);


};


} // namespace Web::Bindings
    