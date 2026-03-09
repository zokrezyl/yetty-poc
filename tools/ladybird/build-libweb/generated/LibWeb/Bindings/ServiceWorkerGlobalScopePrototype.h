
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ServiceWorkerGlobalScopePrototype : public JS::Object {
    JS_OBJECT(ServiceWorkerGlobalScopePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ServiceWorkerGlobalScopePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ServiceWorkerGlobalScopePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ServiceWorkerGlobalScopePrototype() override;
private:

};

} // namespace Web::Bindings
    