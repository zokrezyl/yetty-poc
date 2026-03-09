
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class XPathExpressionPrototype : public JS::Object {
    JS_OBJECT(XPathExpressionPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(XPathExpressionPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit XPathExpressionPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~XPathExpressionPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(evaluate);
        

};


} // namespace Web::Bindings
    