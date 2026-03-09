
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLDocumentPrototype : public JS::Object {
    JS_OBJECT(HTMLDocumentPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLDocumentPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLDocumentPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLDocumentPrototype() override;
private:


};


} // namespace Web::Bindings
    