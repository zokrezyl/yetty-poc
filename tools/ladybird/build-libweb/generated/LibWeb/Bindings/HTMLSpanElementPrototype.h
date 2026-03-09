
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLSpanElementPrototype : public JS::Object {
    JS_OBJECT(HTMLSpanElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLSpanElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLSpanElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLSpanElementPrototype() override;
private:


};


} // namespace Web::Bindings
    