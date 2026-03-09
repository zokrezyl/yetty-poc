
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CacheStoragePrototype : public JS::Object {
    JS_OBJECT(CacheStoragePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CacheStoragePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CacheStoragePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CacheStoragePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(has);
        
    JS_DECLARE_NATIVE_FUNCTION(open);
        

};


} // namespace Web::Bindings
    