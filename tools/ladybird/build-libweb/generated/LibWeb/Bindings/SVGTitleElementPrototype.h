
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGTitleElementPrototype : public JS::Object {
    JS_OBJECT(SVGTitleElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGTitleElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGTitleElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGTitleElementPrototype() override;
private:


};


} // namespace Web::Bindings
    