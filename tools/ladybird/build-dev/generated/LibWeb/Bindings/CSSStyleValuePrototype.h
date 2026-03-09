
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSStyleValuePrototype : public JS::Object {
    JS_OBJECT(CSSStyleValuePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSStyleValuePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSStyleValuePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSStyleValuePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(to_string);
        

};


} // namespace Web::Bindings
    