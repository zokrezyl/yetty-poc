
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class EXTBlendMinMaxPrototype : public JS::Object {
    JS_OBJECT(EXTBlendMinMaxPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(EXTBlendMinMaxPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit EXTBlendMinMaxPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~EXTBlendMinMaxPrototype() override;
private:


};


} // namespace Web::Bindings
    