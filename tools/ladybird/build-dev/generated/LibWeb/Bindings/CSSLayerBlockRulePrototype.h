
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSLayerBlockRulePrototype : public JS::Object {
    JS_OBJECT(CSSLayerBlockRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSLayerBlockRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSLayerBlockRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSLayerBlockRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(name_getter);


};


} // namespace Web::Bindings
    