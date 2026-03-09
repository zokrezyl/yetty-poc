
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSRuleListPrototype : public JS::Object {
    JS_OBJECT(CSSRuleListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSRuleListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSRuleListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSRuleListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(item);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    