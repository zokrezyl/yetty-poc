
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class IDBDatabasePrototype : public JS::Object {
    JS_OBJECT(IDBDatabasePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(IDBDatabasePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit IDBDatabasePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~IDBDatabasePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(transaction);
        
    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(create_object_store);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_object_store);
        
    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(version_getter);

    JS_DECLARE_NATIVE_FUNCTION(object_store_names_getter);

    JS_DECLARE_NATIVE_FUNCTION(onabort_getter);

    JS_DECLARE_NATIVE_FUNCTION(onabort_setter);

    JS_DECLARE_NATIVE_FUNCTION(onclose_getter);

    JS_DECLARE_NATIVE_FUNCTION(onclose_setter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_setter);

    JS_DECLARE_NATIVE_FUNCTION(onversionchange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onversionchange_setter);


};


enum class IDBTransactionDurability : u8 {

    Default,

    Strict,

    Relaxed,

};

inline String idl_enum_to_string(IDBTransactionDurability value)
{
    switch (value) {

    case IDBTransactionDurability::Default:
        return "default"_string;

    case IDBTransactionDurability::Strict:
        return "strict"_string;

    case IDBTransactionDurability::Relaxed:
        return "relaxed"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    