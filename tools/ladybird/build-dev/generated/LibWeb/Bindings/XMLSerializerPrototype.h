
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class XMLSerializerPrototype : public JS::Object {
    JS_OBJECT(XMLSerializerPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(XMLSerializerPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit XMLSerializerPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~XMLSerializerPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(serialize_to_string);
        

};


} // namespace Web::Bindings
    