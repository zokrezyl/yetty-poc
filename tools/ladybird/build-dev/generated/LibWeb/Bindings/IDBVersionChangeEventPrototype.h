
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class IDBVersionChangeEventPrototype : public JS::Object {
    JS_OBJECT(IDBVersionChangeEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(IDBVersionChangeEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit IDBVersionChangeEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~IDBVersionChangeEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(old_version_getter);

    JS_DECLARE_NATIVE_FUNCTION(new_version_getter);


};


} // namespace Web::Bindings
    