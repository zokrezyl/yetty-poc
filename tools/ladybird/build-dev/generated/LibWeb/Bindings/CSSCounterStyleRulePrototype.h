
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSCounterStyleRulePrototype : public JS::Object {
    JS_OBJECT(CSSCounterStyleRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSCounterStyleRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSCounterStyleRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSCounterStyleRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(system_getter);

    JS_DECLARE_NATIVE_FUNCTION(system_setter);

    JS_DECLARE_NATIVE_FUNCTION(symbols_getter);

    JS_DECLARE_NATIVE_FUNCTION(symbols_setter);

    JS_DECLARE_NATIVE_FUNCTION(additive_symbols_getter);

    JS_DECLARE_NATIVE_FUNCTION(additive_symbols_setter);

    JS_DECLARE_NATIVE_FUNCTION(negative_getter);

    JS_DECLARE_NATIVE_FUNCTION(negative_setter);

    JS_DECLARE_NATIVE_FUNCTION(prefix_getter);

    JS_DECLARE_NATIVE_FUNCTION(prefix_setter);

    JS_DECLARE_NATIVE_FUNCTION(suffix_getter);

    JS_DECLARE_NATIVE_FUNCTION(suffix_setter);

    JS_DECLARE_NATIVE_FUNCTION(range_getter);

    JS_DECLARE_NATIVE_FUNCTION(range_setter);

    JS_DECLARE_NATIVE_FUNCTION(pad_getter);

    JS_DECLARE_NATIVE_FUNCTION(pad_setter);

    JS_DECLARE_NATIVE_FUNCTION(speak_as_getter);

    JS_DECLARE_NATIVE_FUNCTION(speak_as_setter);

    JS_DECLARE_NATIVE_FUNCTION(fallback_getter);

    JS_DECLARE_NATIVE_FUNCTION(fallback_setter);


};


} // namespace Web::Bindings
    