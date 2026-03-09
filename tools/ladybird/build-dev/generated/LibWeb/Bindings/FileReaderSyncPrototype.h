
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class FileReaderSyncPrototype : public JS::Object {
    JS_OBJECT(FileReaderSyncPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(FileReaderSyncPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit FileReaderSyncPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~FileReaderSyncPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(read_as_array_buffer);
        
    JS_DECLARE_NATIVE_FUNCTION(read_as_text);
        
    JS_DECLARE_NATIVE_FUNCTION(read_as_binary_string);
        
    JS_DECLARE_NATIVE_FUNCTION(read_as_data_url);
        

};


} // namespace Web::Bindings
    