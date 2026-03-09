
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DataTransferPrototype : public JS::Object {
    JS_OBJECT(DataTransferPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DataTransferPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DataTransferPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DataTransferPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_data);
        
    JS_DECLARE_NATIVE_FUNCTION(set_data);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_data);
        
    JS_DECLARE_NATIVE_FUNCTION(drop_effect_getter);

    JS_DECLARE_NATIVE_FUNCTION(drop_effect_setter);

    JS_DECLARE_NATIVE_FUNCTION(effect_allowed_getter);

    JS_DECLARE_NATIVE_FUNCTION(effect_allowed_setter);

    JS_DECLARE_NATIVE_FUNCTION(items_getter);

    JS_DECLARE_NATIVE_FUNCTION(types_getter);

    JS_DECLARE_NATIVE_FUNCTION(files_getter);


};


} // namespace Web::Bindings
    