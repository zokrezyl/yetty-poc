
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CryptoPrototype : public JS::Object {
    JS_OBJECT(CryptoPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CryptoPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CryptoPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CryptoPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_random_values);
        
    JS_DECLARE_NATIVE_FUNCTION(random_uuid);
        
    JS_DECLARE_NATIVE_FUNCTION(subtle_getter);


};


} // namespace Web::Bindings
    