
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSStyleDeclarationPrototype : public JS::Object {
    JS_OBJECT(CSSStyleDeclarationPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSStyleDeclarationPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSStyleDeclarationPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSStyleDeclarationPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(item);
        
    JS_DECLARE_NATIVE_FUNCTION(get_property_value);
        
    JS_DECLARE_NATIVE_FUNCTION(get_property_priority);
        
    JS_DECLARE_NATIVE_FUNCTION(set_property);
        
    JS_DECLARE_NATIVE_FUNCTION(remove_property);
        
    JS_DECLARE_NATIVE_FUNCTION(css_text_getter);

    JS_DECLARE_NATIVE_FUNCTION(css_text_setter);

    JS_DECLARE_NATIVE_FUNCTION(length_getter);

    JS_DECLARE_NATIVE_FUNCTION(parent_rule_getter);


};


} // namespace Web::Bindings
    