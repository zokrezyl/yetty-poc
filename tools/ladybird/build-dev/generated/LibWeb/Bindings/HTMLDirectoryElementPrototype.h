
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLDirectoryElementPrototype : public JS::Object {
    JS_OBJECT(HTMLDirectoryElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLDirectoryElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLDirectoryElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLDirectoryElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(compact_getter);

    JS_DECLARE_NATIVE_FUNCTION(compact_setter);


};


} // namespace Web::Bindings
    