
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WorkletGlobalScopePrototype : public JS::Object {
    JS_OBJECT(WorkletGlobalScopePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WorkletGlobalScopePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WorkletGlobalScopePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WorkletGlobalScopePrototype() override;
private:


};


} // namespace Web::Bindings
    