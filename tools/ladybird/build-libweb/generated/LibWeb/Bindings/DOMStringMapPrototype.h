
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DOMStringMapPrototype : public JS::Object {
    JS_OBJECT(DOMStringMapPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DOMStringMapPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DOMStringMapPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMStringMapPrototype() override;
private:


};


} // namespace Web::Bindings
    