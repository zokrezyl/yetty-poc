
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSKeyframeRulePrototype : public JS::Object {
    JS_OBJECT(CSSKeyframeRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSKeyframeRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSKeyframeRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSKeyframeRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(key_text_getter);

    JS_DECLARE_NATIVE_FUNCTION(key_text_setter);

    JS_DECLARE_NATIVE_FUNCTION(style_getter);

    JS_DECLARE_NATIVE_FUNCTION(style_setter);


};


} // namespace Web::Bindings
    