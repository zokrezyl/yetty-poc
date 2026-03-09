
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLUnknownElementPrototype : public JS::Object {
    JS_OBJECT(HTMLUnknownElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLUnknownElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLUnknownElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLUnknownElementPrototype() override;
private:


};


} // namespace Web::Bindings
    