
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLPictureElementPrototype : public JS::Object {
    JS_OBJECT(HTMLPictureElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLPictureElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLPictureElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLPictureElementPrototype() override;
private:


};


} // namespace Web::Bindings
    