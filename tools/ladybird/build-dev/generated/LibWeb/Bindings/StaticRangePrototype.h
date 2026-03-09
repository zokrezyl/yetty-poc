
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class StaticRangePrototype : public JS::Object {
    JS_OBJECT(StaticRangePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(StaticRangePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit StaticRangePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~StaticRangePrototype() override;
private:


};


} // namespace Web::Bindings
    