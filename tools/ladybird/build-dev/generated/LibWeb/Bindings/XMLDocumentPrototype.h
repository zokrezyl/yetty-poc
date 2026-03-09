
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class XMLDocumentPrototype : public JS::Object {
    JS_OBJECT(XMLDocumentPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(XMLDocumentPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit XMLDocumentPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~XMLDocumentPrototype() override;
private:


};


} // namespace Web::Bindings
    