
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class FileSystemEntryPrototype : public JS::Object {
    JS_OBJECT(FileSystemEntryPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(FileSystemEntryPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit FileSystemEntryPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~FileSystemEntryPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(is_file_getter);

    JS_DECLARE_NATIVE_FUNCTION(is_directory_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_getter);


};


} // namespace Web::Bindings
    