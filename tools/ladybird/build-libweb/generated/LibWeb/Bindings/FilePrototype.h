
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class FilePrototype : public JS::Object {
    JS_OBJECT(FilePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(FilePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit FilePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~FilePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(last_modified_getter);


};


} // namespace Web::Bindings
    