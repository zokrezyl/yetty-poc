
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class RadioNodeListPrototype : public JS::Object {
    JS_OBJECT(RadioNodeListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(RadioNodeListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit RadioNodeListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~RadioNodeListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(value_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_setter);


};


} // namespace Web::Bindings
    