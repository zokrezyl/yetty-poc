
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DataTransferItemPrototype : public JS::Object {
    JS_OBJECT(DataTransferItemPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DataTransferItemPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DataTransferItemPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DataTransferItemPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_as_string);
        
    JS_DECLARE_NATIVE_FUNCTION(get_as_file);
        
    JS_DECLARE_NATIVE_FUNCTION(webkit_get_as_entry);
        
    JS_DECLARE_NATIVE_FUNCTION(kind_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);


};


} // namespace Web::Bindings
    