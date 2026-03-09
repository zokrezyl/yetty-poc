
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DOMRectListPrototype : public JS::Object {
    JS_OBJECT(DOMRectListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DOMRectListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DOMRectListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMRectListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(item);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    