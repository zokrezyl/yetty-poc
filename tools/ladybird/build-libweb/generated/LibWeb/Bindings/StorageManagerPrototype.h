
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class StorageManagerPrototype : public JS::Object {
    JS_OBJECT(StorageManagerPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(StorageManagerPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit StorageManagerPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~StorageManagerPrototype() override;
private:


};


} // namespace Web::Bindings
    