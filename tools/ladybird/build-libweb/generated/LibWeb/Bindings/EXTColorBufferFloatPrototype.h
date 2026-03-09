
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class EXTColorBufferFloatPrototype : public JS::Object {
    JS_OBJECT(EXTColorBufferFloatPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(EXTColorBufferFloatPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit EXTColorBufferFloatPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~EXTColorBufferFloatPrototype() override;
private:


};


} // namespace Web::Bindings
    