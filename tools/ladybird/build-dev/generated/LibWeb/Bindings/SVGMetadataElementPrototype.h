
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGMetadataElementPrototype : public JS::Object {
    JS_OBJECT(SVGMetadataElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGMetadataElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGMetadataElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGMetadataElementPrototype() override;
private:


};


} // namespace Web::Bindings
    