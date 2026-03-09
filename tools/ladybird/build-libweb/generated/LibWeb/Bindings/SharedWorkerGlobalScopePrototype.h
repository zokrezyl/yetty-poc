
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SharedWorkerGlobalScopePrototype : public JS::Object {
    JS_OBJECT(SharedWorkerGlobalScopePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SharedWorkerGlobalScopePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SharedWorkerGlobalScopePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SharedWorkerGlobalScopePrototype() override;
private:

};

} // namespace Web::Bindings
    