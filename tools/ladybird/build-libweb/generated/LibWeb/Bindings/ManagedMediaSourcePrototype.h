
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ManagedMediaSourcePrototype : public JS::Object {
    JS_OBJECT(ManagedMediaSourcePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ManagedMediaSourcePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ManagedMediaSourcePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ManagedMediaSourcePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(onstartstreaming_getter);

    JS_DECLARE_NATIVE_FUNCTION(onstartstreaming_setter);

    JS_DECLARE_NATIVE_FUNCTION(onendstreaming_getter);

    JS_DECLARE_NATIVE_FUNCTION(onendstreaming_setter);


};


} // namespace Web::Bindings
    