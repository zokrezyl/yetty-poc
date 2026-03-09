
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DedicatedWorkerGlobalScopePrototype : public JS::Object {
    JS_OBJECT(DedicatedWorkerGlobalScopePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DedicatedWorkerGlobalScopePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DedicatedWorkerGlobalScopePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DedicatedWorkerGlobalScopePrototype() override;
private:

};

} // namespace Web::Bindings
    