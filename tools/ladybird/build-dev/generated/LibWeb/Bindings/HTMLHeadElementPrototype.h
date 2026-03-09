
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLHeadElementPrototype : public JS::Object {
    JS_OBJECT(HTMLHeadElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLHeadElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLHeadElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLHeadElementPrototype() override;
private:


};


} // namespace Web::Bindings
    