
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PeriodicWavePrototype : public JS::Object {
    JS_OBJECT(PeriodicWavePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PeriodicWavePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PeriodicWavePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PeriodicWavePrototype() override;
private:


};


} // namespace Web::Bindings
    