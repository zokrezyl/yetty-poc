
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSSupportsRulePrototype : public JS::Object {
    JS_OBJECT(CSSSupportsRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSSupportsRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSSupportsRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSSupportsRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(matches_getter);


};


} // namespace Web::Bindings
    