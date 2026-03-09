
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AttrPrototype : public JS::Object {
    JS_OBJECT(AttrPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AttrPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AttrPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AttrPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(namespace_uri_getter);

    JS_DECLARE_NATIVE_FUNCTION(prefix_getter);

    JS_DECLARE_NATIVE_FUNCTION(local_name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_setter);

    JS_DECLARE_NATIVE_FUNCTION(owner_element_getter);

    JS_DECLARE_NATIVE_FUNCTION(specified_getter);


};


} // namespace Web::Bindings
    