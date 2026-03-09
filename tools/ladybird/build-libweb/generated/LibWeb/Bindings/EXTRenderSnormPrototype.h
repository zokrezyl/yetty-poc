
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class EXTRenderSnormPrototype : public JS::Object {
    JS_OBJECT(EXTRenderSnormPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(EXTRenderSnormPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit EXTRenderSnormPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~EXTRenderSnormPrototype() override;
private:


};


} // namespace Web::Bindings
    