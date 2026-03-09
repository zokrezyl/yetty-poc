
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class IDBObjectStorePrototype : public JS::Object {
    JS_OBJECT(IDBObjectStorePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(IDBObjectStorePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit IDBObjectStorePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~IDBObjectStorePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(put);
        
    JS_DECLARE_NATIVE_FUNCTION(add);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_);
        
    JS_DECLARE_NATIVE_FUNCTION(clear);
        
    JS_DECLARE_NATIVE_FUNCTION(get);
        
    JS_DECLARE_NATIVE_FUNCTION(get_key);
        
    JS_DECLARE_NATIVE_FUNCTION(get_all);
        
    JS_DECLARE_NATIVE_FUNCTION(get_all_keys);
        
    JS_DECLARE_NATIVE_FUNCTION(get_all_records);
        
    JS_DECLARE_NATIVE_FUNCTION(count);
        
    JS_DECLARE_NATIVE_FUNCTION(open_cursor);
        
    JS_DECLARE_NATIVE_FUNCTION(open_key_cursor);
        
    JS_DECLARE_NATIVE_FUNCTION(index);
        
    JS_DECLARE_NATIVE_FUNCTION(create_index);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_index);
        
    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(key_path_getter);

    JS_DECLARE_NATIVE_FUNCTION(index_names_getter);

    JS_DECLARE_NATIVE_FUNCTION(transaction_getter);

    JS_DECLARE_NATIVE_FUNCTION(auto_increment_getter);


};


} // namespace Web::Bindings
    