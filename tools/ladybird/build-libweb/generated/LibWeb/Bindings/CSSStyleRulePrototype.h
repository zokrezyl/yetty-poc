
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSStyleRulePrototype : public JS::Object {
    JS_OBJECT(CSSStyleRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSStyleRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSStyleRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSStyleRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(selector_text_getter);

    JS_DECLARE_NATIVE_FUNCTION(selector_text_setter);

    JS_DECLARE_NATIVE_FUNCTION(style_getter);

    JS_DECLARE_NATIVE_FUNCTION(style_setter);

    JS_DECLARE_NATIVE_FUNCTION(style_map_getter);


};


} // namespace Web::Bindings
    