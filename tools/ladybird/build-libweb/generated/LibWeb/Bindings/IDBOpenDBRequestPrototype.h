
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class IDBOpenDBRequestPrototype : public JS::Object {
    JS_OBJECT(IDBOpenDBRequestPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(IDBOpenDBRequestPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit IDBOpenDBRequestPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~IDBOpenDBRequestPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(onblocked_getter);

    JS_DECLARE_NATIVE_FUNCTION(onblocked_setter);

    JS_DECLARE_NATIVE_FUNCTION(onupgradeneeded_getter);

    JS_DECLARE_NATIVE_FUNCTION(onupgradeneeded_setter);


};


} // namespace Web::Bindings
    