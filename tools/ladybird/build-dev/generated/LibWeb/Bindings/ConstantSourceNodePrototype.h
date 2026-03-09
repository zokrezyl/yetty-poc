
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ConstantSourceNodePrototype : public JS::Object {
    JS_OBJECT(ConstantSourceNodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ConstantSourceNodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ConstantSourceNodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ConstantSourceNodePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(offset_getter);


};


} // namespace Web::Bindings
    