
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class XMLHttpRequestUploadPrototype : public JS::Object {
    JS_OBJECT(XMLHttpRequestUploadPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(XMLHttpRequestUploadPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit XMLHttpRequestUploadPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~XMLHttpRequestUploadPrototype() override;
private:


};


} // namespace Web::Bindings
    