
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class IDBRequestPrototype : public JS::Object {
    JS_OBJECT(IDBRequestPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(IDBRequestPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit IDBRequestPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~IDBRequestPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(result_getter);

    JS_DECLARE_NATIVE_FUNCTION(error_getter);

    JS_DECLARE_NATIVE_FUNCTION(source_getter);

    JS_DECLARE_NATIVE_FUNCTION(transaction_getter);

    JS_DECLARE_NATIVE_FUNCTION(ready_state_getter);

    JS_DECLARE_NATIVE_FUNCTION(onsuccess_getter);

    JS_DECLARE_NATIVE_FUNCTION(onsuccess_setter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_setter);


};


enum class IDBRequestReadyState : u8 {

    Pending,

    Done,

};

inline String idl_enum_to_string(IDBRequestReadyState value)
{
    switch (value) {

    case IDBRequestReadyState::Pending:
        return "pending"_string;

    case IDBRequestReadyState::Done:
        return "done"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    