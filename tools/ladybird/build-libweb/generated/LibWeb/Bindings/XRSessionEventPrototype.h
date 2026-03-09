
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class XRSessionEventPrototype : public JS::Object {
    JS_OBJECT(XRSessionEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(XRSessionEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit XRSessionEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~XRSessionEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(session_getter);


};


} // namespace Web::Bindings
    