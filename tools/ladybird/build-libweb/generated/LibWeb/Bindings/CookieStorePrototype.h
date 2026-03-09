
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CookieStorePrototype : public JS::Object {
    JS_OBJECT(CookieStorePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CookieStorePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CookieStorePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CookieStorePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get);
        
    JS_DECLARE_NATIVE_FUNCTION(get0);

    JS_DECLARE_NATIVE_FUNCTION(get1);

    JS_DECLARE_NATIVE_FUNCTION(get_all);
        
    JS_DECLARE_NATIVE_FUNCTION(get_all0);

    JS_DECLARE_NATIVE_FUNCTION(get_all1);

    JS_DECLARE_NATIVE_FUNCTION(set);
        
    JS_DECLARE_NATIVE_FUNCTION(set0);

    JS_DECLARE_NATIVE_FUNCTION(set1);

    JS_DECLARE_NATIVE_FUNCTION(delete_);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_0);

    JS_DECLARE_NATIVE_FUNCTION(delete_1);

    JS_DECLARE_NATIVE_FUNCTION(onchange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onchange_setter);


};


enum class CookieSameSite : u8 {

    Strict,

    Lax,

    None,

};

inline String idl_enum_to_string(CookieSameSite value)
{
    switch (value) {

    case CookieSameSite::Strict:
        return "strict"_string;

    case CookieSameSite::Lax:
        return "lax"_string;

    case CookieSameSite::None:
        return "none"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    