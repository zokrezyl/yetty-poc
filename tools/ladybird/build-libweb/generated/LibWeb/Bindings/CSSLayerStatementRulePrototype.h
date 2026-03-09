
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSLayerStatementRulePrototype : public JS::Object {
    JS_OBJECT(CSSLayerStatementRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSLayerStatementRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSLayerStatementRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSLayerStatementRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(name_list_getter);


};


} // namespace Web::Bindings
    