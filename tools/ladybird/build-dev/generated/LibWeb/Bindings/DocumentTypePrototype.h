
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DocumentTypePrototype : public JS::Object {
    JS_OBJECT(DocumentTypePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DocumentTypePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DocumentTypePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DocumentTypePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(before);
        
    JS_DECLARE_NATIVE_FUNCTION(after);
        
    JS_DECLARE_NATIVE_FUNCTION(replace_with);
        
    JS_DECLARE_NATIVE_FUNCTION(remove);
        
    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(public_id_getter);

    JS_DECLARE_NATIVE_FUNCTION(system_id_getter);


};


} // namespace Web::Bindings
    