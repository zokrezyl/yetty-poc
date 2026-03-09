
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class OESElementIndexUintPrototype : public JS::Object {
    JS_OBJECT(OESElementIndexUintPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(OESElementIndexUintPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit OESElementIndexUintPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~OESElementIndexUintPrototype() override;
private:


};


} // namespace Web::Bindings
    