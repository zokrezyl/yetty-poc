
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class EXTTextureNorm16Prototype : public JS::Object {
    JS_OBJECT(EXTTextureNorm16Prototype, JS::Object);
    GC_DECLARE_ALLOCATOR(EXTTextureNorm16Prototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit EXTTextureNorm16Prototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~EXTTextureNorm16Prototype() override;
private:


};


} // namespace Web::Bindings
    