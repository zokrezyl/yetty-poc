
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class NodeListPrototype : public JS::Object {
    JS_OBJECT(NodeListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(NodeListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit NodeListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~NodeListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(item);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    