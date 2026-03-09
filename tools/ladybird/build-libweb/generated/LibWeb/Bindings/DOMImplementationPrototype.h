
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DOMImplementationPrototype : public JS::Object {
    JS_OBJECT(DOMImplementationPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DOMImplementationPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DOMImplementationPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMImplementationPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(create_document_type);
        
    JS_DECLARE_NATIVE_FUNCTION(create_document);
        
    JS_DECLARE_NATIVE_FUNCTION(create_html_document);
        
    JS_DECLARE_NATIVE_FUNCTION(has_feature);
        

};


} // namespace Web::Bindings
    