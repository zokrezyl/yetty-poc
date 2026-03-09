
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGAnimationElementPrototype : public JS::Object {
    JS_OBJECT(SVGAnimationElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGAnimationElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGAnimationElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGAnimationElementPrototype() override;
private:


};


} // namespace Web::Bindings
    