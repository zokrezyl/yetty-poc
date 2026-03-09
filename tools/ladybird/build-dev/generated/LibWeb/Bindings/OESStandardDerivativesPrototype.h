
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class OESStandardDerivativesPrototype : public JS::Object {
    JS_OBJECT(OESStandardDerivativesPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(OESStandardDerivativesPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit OESStandardDerivativesPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~OESStandardDerivativesPrototype() override;
private:


};


} // namespace Web::Bindings
    