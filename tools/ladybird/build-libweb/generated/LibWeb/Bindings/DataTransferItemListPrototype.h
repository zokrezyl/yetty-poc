
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DataTransferItemListPrototype : public JS::Object {
    JS_OBJECT(DataTransferItemListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DataTransferItemListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DataTransferItemListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DataTransferItemListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(add);
        
    JS_DECLARE_NATIVE_FUNCTION(add0);

    JS_DECLARE_NATIVE_FUNCTION(add1);

    JS_DECLARE_NATIVE_FUNCTION(remove);
        
    JS_DECLARE_NATIVE_FUNCTION(clear);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    