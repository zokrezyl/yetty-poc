
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class IDBTransactionPrototype : public JS::Object {
    JS_OBJECT(IDBTransactionPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(IDBTransactionPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit IDBTransactionPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~IDBTransactionPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(object_store);
        
    JS_DECLARE_NATIVE_FUNCTION(commit);
        
    JS_DECLARE_NATIVE_FUNCTION(abort);
        
    JS_DECLARE_NATIVE_FUNCTION(object_store_names_getter);

    JS_DECLARE_NATIVE_FUNCTION(mode_getter);

    JS_DECLARE_NATIVE_FUNCTION(durability_getter);

    JS_DECLARE_NATIVE_FUNCTION(db_getter);

    JS_DECLARE_NATIVE_FUNCTION(error_getter);

    JS_DECLARE_NATIVE_FUNCTION(onabort_getter);

    JS_DECLARE_NATIVE_FUNCTION(onabort_setter);

    JS_DECLARE_NATIVE_FUNCTION(oncomplete_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncomplete_setter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_setter);


};


enum class IDBTransactionMode : u8 {

    Readonly,

    Readwrite,

    Versionchange,

};

inline String idl_enum_to_string(IDBTransactionMode value)
{
    switch (value) {

    case IDBTransactionMode::Readonly:
        return "readonly"_string;

    case IDBTransactionMode::Readwrite:
        return "readwrite"_string;

    case IDBTransactionMode::Versionchange:
        return "versionchange"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    