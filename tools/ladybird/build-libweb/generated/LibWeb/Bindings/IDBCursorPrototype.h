
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class IDBCursorPrototype : public JS::Object {
    JS_OBJECT(IDBCursorPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(IDBCursorPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit IDBCursorPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~IDBCursorPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(advance);
        
    JS_DECLARE_NATIVE_FUNCTION(continue_);
        
    JS_DECLARE_NATIVE_FUNCTION(continue_primary_key);
        
    JS_DECLARE_NATIVE_FUNCTION(update);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_);
        
    JS_DECLARE_NATIVE_FUNCTION(source_getter);

    JS_DECLARE_NATIVE_FUNCTION(direction_getter);

    JS_DECLARE_NATIVE_FUNCTION(key_getter);

    JS_DECLARE_NATIVE_FUNCTION(primary_key_getter);

    JS_DECLARE_NATIVE_FUNCTION(request_getter);


};


enum class IDBCursorDirection : u8 {

    Next,

    Nextunique,

    Prev,

    Prevunique,

};

inline String idl_enum_to_string(IDBCursorDirection value)
{
    switch (value) {

    case IDBCursorDirection::Next:
        return "next"_string;

    case IDBCursorDirection::Nextunique:
        return "nextunique"_string;

    case IDBCursorDirection::Prev:
        return "prev"_string;

    case IDBCursorDirection::Prevunique:
        return "prevunique"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    