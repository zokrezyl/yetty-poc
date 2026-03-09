
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TablePrototype : public JS::Object {
    JS_OBJECT(TablePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TablePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TablePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TablePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(grow);
        
    JS_DECLARE_NATIVE_FUNCTION(get);
        
    JS_DECLARE_NATIVE_FUNCTION(set);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


enum class TableKind : u8 {

    Externref,

    Anyfunc,

};

inline String idl_enum_to_string(TableKind value)
{
    switch (value) {

    case TableKind::Externref:
        return "externref"_string;

    case TableKind::Anyfunc:
        return "anyfunc"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    