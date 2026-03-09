
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSNamespaceRulePrototype : public JS::Object {
    JS_OBJECT(CSSNamespaceRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSNamespaceRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSNamespaceRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSNamespaceRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(namespace_uri_getter);

    JS_DECLARE_NATIVE_FUNCTION(prefix_getter);


};


} // namespace Web::Bindings
    