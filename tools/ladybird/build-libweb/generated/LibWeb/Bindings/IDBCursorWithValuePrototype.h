
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class IDBCursorWithValuePrototype : public JS::Object {
    JS_OBJECT(IDBCursorWithValuePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(IDBCursorWithValuePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit IDBCursorWithValuePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~IDBCursorWithValuePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(value_getter);


};


} // namespace Web::Bindings
    