
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CDATASectionPrototype : public JS::Object {
    JS_OBJECT(CDATASectionPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CDATASectionPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CDATASectionPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CDATASectionPrototype() override;
private:


};


} // namespace Web::Bindings
    