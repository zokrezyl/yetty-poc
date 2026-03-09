
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class XPathEvaluatorPrototype : public JS::Object {
    JS_OBJECT(XPathEvaluatorPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(XPathEvaluatorPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit XPathEvaluatorPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~XPathEvaluatorPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(create_expression);
        
    JS_DECLARE_NATIVE_FUNCTION(create_ns_resolver);
        
    JS_DECLARE_NATIVE_FUNCTION(evaluate);
        

};


} // namespace Web::Bindings
    