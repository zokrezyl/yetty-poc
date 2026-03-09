
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class FileListPrototype : public JS::Object {
    JS_OBJECT(FileListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(FileListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit FileListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~FileListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(item);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    