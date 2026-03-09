
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class StylePropertyMapReadOnlyPrototype : public JS::Object {
    JS_OBJECT(StylePropertyMapReadOnlyPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(StylePropertyMapReadOnlyPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit StylePropertyMapReadOnlyPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~StylePropertyMapReadOnlyPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get);
        
    JS_DECLARE_NATIVE_FUNCTION(get_all);
        
    JS_DECLARE_NATIVE_FUNCTION(has);
        
    JS_DECLARE_NATIVE_FUNCTION(size_getter);


};


} // namespace Web::Bindings
    