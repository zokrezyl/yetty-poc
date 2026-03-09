
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MimeTypeArrayPrototype : public JS::Object {
    JS_OBJECT(MimeTypeArrayPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MimeTypeArrayPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MimeTypeArrayPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MimeTypeArrayPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(item);
        
    JS_DECLARE_NATIVE_FUNCTION(named_item);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    