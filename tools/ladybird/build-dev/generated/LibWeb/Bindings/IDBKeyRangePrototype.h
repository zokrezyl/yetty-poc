
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class IDBKeyRangePrototype : public JS::Object {
    JS_OBJECT(IDBKeyRangePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(IDBKeyRangePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit IDBKeyRangePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~IDBKeyRangePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(includes);
        
    JS_DECLARE_NATIVE_FUNCTION(lower_getter);

    JS_DECLARE_NATIVE_FUNCTION(upper_getter);

    JS_DECLARE_NATIVE_FUNCTION(lower_open_getter);

    JS_DECLARE_NATIVE_FUNCTION(upper_open_getter);


};


} // namespace Web::Bindings
    