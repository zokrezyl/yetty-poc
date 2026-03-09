
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class StoragePrototype : public JS::Object {
    JS_OBJECT(StoragePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(StoragePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit StoragePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~StoragePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(key);
        
    JS_DECLARE_NATIVE_FUNCTION(get_item);
        
    JS_DECLARE_NATIVE_FUNCTION(set_item);
        
    JS_DECLARE_NATIVE_FUNCTION(remove_item);
        
    JS_DECLARE_NATIVE_FUNCTION(clear);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    