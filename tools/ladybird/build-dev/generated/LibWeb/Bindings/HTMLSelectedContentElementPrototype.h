
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLSelectedContentElementPrototype : public JS::Object {
    JS_OBJECT(HTMLSelectedContentElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLSelectedContentElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLSelectedContentElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLSelectedContentElementPrototype() override;
private:


};


} // namespace Web::Bindings
    