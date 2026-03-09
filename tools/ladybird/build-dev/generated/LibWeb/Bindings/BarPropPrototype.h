
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class BarPropPrototype : public JS::Object {
    JS_OBJECT(BarPropPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(BarPropPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit BarPropPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~BarPropPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(visible_getter);


};


} // namespace Web::Bindings
    