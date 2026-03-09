
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class BufferedChangeEventPrototype : public JS::Object {
    JS_OBJECT(BufferedChangeEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(BufferedChangeEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit BufferedChangeEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~BufferedChangeEventPrototype() override;
private:


};


} // namespace Web::Bindings
    