
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class GainNodePrototype : public JS::Object {
    JS_OBJECT(GainNodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(GainNodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit GainNodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~GainNodePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(gain_getter);


};


} // namespace Web::Bindings
    