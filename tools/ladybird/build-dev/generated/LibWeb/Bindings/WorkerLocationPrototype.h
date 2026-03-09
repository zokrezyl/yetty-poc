
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WorkerLocationPrototype : public JS::Object {
    JS_OBJECT(WorkerLocationPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WorkerLocationPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WorkerLocationPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WorkerLocationPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(to_string);
        
    JS_DECLARE_NATIVE_FUNCTION(href_getter);

    JS_DECLARE_NATIVE_FUNCTION(origin_getter);

    JS_DECLARE_NATIVE_FUNCTION(protocol_getter);

    JS_DECLARE_NATIVE_FUNCTION(host_getter);

    JS_DECLARE_NATIVE_FUNCTION(hostname_getter);

    JS_DECLARE_NATIVE_FUNCTION(port_getter);

    JS_DECLARE_NATIVE_FUNCTION(pathname_getter);

    JS_DECLARE_NATIVE_FUNCTION(search_getter);

    JS_DECLARE_NATIVE_FUNCTION(hash_getter);


};


} // namespace Web::Bindings
    