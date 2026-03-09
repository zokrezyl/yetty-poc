
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGClipPathElementPrototype : public JS::Object {
    JS_OBJECT(SVGClipPathElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGClipPathElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGClipPathElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGClipPathElementPrototype() override;
private:


};


} // namespace Web::Bindings
    