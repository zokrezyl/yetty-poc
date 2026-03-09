
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class IDBFactoryPrototype : public JS::Object {
    JS_OBJECT(IDBFactoryPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(IDBFactoryPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit IDBFactoryPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~IDBFactoryPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(open);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_database);
        
    JS_DECLARE_NATIVE_FUNCTION(databases);
        
    JS_DECLARE_NATIVE_FUNCTION(cmp);
        

};


} // namespace Web::Bindings
    