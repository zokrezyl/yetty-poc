
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGDescElementPrototype : public JS::Object {
    JS_OBJECT(SVGDescElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGDescElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGDescElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGDescElementPrototype() override;
private:


};


} // namespace Web::Bindings
    