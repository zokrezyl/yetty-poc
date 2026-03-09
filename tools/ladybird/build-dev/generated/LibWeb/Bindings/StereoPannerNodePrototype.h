
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class StereoPannerNodePrototype : public JS::Object {
    JS_OBJECT(StereoPannerNodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(StereoPannerNodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit StereoPannerNodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~StereoPannerNodePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(pan_getter);


};


} // namespace Web::Bindings
    