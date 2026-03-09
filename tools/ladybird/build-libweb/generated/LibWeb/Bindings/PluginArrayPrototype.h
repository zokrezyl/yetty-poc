
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PluginArrayPrototype : public JS::Object {
    JS_OBJECT(PluginArrayPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PluginArrayPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PluginArrayPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PluginArrayPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(refresh);
        
    JS_DECLARE_NATIVE_FUNCTION(item);
        
    JS_DECLARE_NATIVE_FUNCTION(named_item);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    