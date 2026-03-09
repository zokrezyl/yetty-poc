
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class EXTTextureFilterAnisotropicPrototype : public JS::Object {
    JS_OBJECT(EXTTextureFilterAnisotropicPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(EXTTextureFilterAnisotropicPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit EXTTextureFilterAnisotropicPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~EXTTextureFilterAnisotropicPrototype() override;
private:


};


} // namespace Web::Bindings
    