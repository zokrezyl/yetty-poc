
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DynamicsCompressorNodePrototype : public JS::Object {
    JS_OBJECT(DynamicsCompressorNodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DynamicsCompressorNodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DynamicsCompressorNodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DynamicsCompressorNodePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(threshold_getter);

    JS_DECLARE_NATIVE_FUNCTION(knee_getter);

    JS_DECLARE_NATIVE_FUNCTION(ratio_getter);

    JS_DECLARE_NATIVE_FUNCTION(reduction_getter);

    JS_DECLARE_NATIVE_FUNCTION(attack_getter);

    JS_DECLARE_NATIVE_FUNCTION(release_getter);


};


} // namespace Web::Bindings
    