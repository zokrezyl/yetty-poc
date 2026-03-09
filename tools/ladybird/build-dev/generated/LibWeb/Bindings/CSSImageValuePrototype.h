
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSImageValuePrototype : public JS::Object {
    JS_OBJECT(CSSImageValuePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSImageValuePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSImageValuePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSImageValuePrototype() override;
private:


};


} // namespace Web::Bindings
    