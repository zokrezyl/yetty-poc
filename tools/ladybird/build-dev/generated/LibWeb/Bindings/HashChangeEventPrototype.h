
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HashChangeEventPrototype : public JS::Object {
    JS_OBJECT(HashChangeEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HashChangeEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HashChangeEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HashChangeEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(old_url_getter);

    JS_DECLARE_NATIVE_FUNCTION(new_url_getter);


};


} // namespace Web::Bindings
    