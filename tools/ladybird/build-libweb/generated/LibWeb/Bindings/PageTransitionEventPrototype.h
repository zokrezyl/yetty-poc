
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PageTransitionEventPrototype : public JS::Object {
    JS_OBJECT(PageTransitionEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PageTransitionEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PageTransitionEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PageTransitionEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(persisted_getter);


};


} // namespace Web::Bindings
    