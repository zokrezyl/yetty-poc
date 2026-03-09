
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class OESVertexArrayObjectPrototype : public JS::Object {
    JS_OBJECT(OESVertexArrayObjectPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(OESVertexArrayObjectPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit OESVertexArrayObjectPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~OESVertexArrayObjectPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(create_vertex_array_oes);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_vertex_array_oes);
        
    JS_DECLARE_NATIVE_FUNCTION(is_vertex_array_oes);
        
    JS_DECLARE_NATIVE_FUNCTION(bind_vertex_array_oes);
        

};


} // namespace Web::Bindings
    