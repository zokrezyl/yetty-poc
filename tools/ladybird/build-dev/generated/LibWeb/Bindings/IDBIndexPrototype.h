
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class IDBIndexPrototype : public JS::Object {
    JS_OBJECT(IDBIndexPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(IDBIndexPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit IDBIndexPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~IDBIndexPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get);
        
    JS_DECLARE_NATIVE_FUNCTION(get_key);
        
    JS_DECLARE_NATIVE_FUNCTION(get_all);
        
    JS_DECLARE_NATIVE_FUNCTION(get_all_keys);
        
    JS_DECLARE_NATIVE_FUNCTION(get_all_records);
        
    JS_DECLARE_NATIVE_FUNCTION(count);
        
    JS_DECLARE_NATIVE_FUNCTION(open_cursor);
        
    JS_DECLARE_NATIVE_FUNCTION(open_key_cursor);
        
    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(object_store_getter);

    JS_DECLARE_NATIVE_FUNCTION(key_path_getter);

    JS_DECLARE_NATIVE_FUNCTION(multi_entry_getter);

    JS_DECLARE_NATIVE_FUNCTION(unique_getter);


};


} // namespace Web::Bindings
    