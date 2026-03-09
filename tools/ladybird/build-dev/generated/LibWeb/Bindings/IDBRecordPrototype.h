
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class IDBRecordPrototype : public JS::Object {
    JS_OBJECT(IDBRecordPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(IDBRecordPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit IDBRecordPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~IDBRecordPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(key_getter);

    JS_DECLARE_NATIVE_FUNCTION(primary_key_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_getter);


};


} // namespace Web::Bindings
    