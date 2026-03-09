
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class StyleSheetListPrototype : public JS::Object {
    JS_OBJECT(StyleSheetListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(StyleSheetListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit StyleSheetListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~StyleSheetListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(item);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    