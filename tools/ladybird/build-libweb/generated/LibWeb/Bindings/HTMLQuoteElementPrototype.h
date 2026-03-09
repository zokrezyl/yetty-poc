
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLQuoteElementPrototype : public JS::Object {
    JS_OBJECT(HTMLQuoteElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLQuoteElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLQuoteElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLQuoteElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(cite_getter);

    JS_DECLARE_NATIVE_FUNCTION(cite_setter);


};


} // namespace Web::Bindings
    