
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PluginPrototype : public JS::Object {
    JS_OBJECT(PluginPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PluginPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PluginPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PluginPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(item);
        
    JS_DECLARE_NATIVE_FUNCTION(named_item);
        
    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(description_getter);

    JS_DECLARE_NATIVE_FUNCTION(filename_getter);

    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    