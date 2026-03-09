
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class NodeFilterPrototype : public JS::Object {
    JS_OBJECT(NodeFilterPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(NodeFilterPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit NodeFilterPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~NodeFilterPrototype() override;
private:


};


} // namespace Web::Bindings
    