
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class StorageEventPrototype : public JS::Object {
    JS_OBJECT(StorageEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(StorageEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit StorageEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~StorageEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(init_storage_event);
        
    JS_DECLARE_NATIVE_FUNCTION(key_getter);

    JS_DECLARE_NATIVE_FUNCTION(old_value_getter);

    JS_DECLARE_NATIVE_FUNCTION(new_value_getter);

    JS_DECLARE_NATIVE_FUNCTION(url_getter);

    JS_DECLARE_NATIVE_FUNCTION(storage_area_getter);


};


} // namespace Web::Bindings
    