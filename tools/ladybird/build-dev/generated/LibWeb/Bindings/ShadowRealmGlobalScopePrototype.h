
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ShadowRealmGlobalScopePrototype : public JS::Object {
    JS_OBJECT(ShadowRealmGlobalScopePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ShadowRealmGlobalScopePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ShadowRealmGlobalScopePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ShadowRealmGlobalScopePrototype() override;
private:

};

} // namespace Web::Bindings
    