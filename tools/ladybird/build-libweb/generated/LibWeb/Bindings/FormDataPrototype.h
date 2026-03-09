
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class FormDataPrototype : public JS::Object {
    JS_OBJECT(FormDataPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(FormDataPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit FormDataPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~FormDataPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(append);
        
    JS_DECLARE_NATIVE_FUNCTION(append0);

    JS_DECLARE_NATIVE_FUNCTION(append1);

    JS_DECLARE_NATIVE_FUNCTION(delete_);
        
    JS_DECLARE_NATIVE_FUNCTION(get);
        
    JS_DECLARE_NATIVE_FUNCTION(get_all);
        
    JS_DECLARE_NATIVE_FUNCTION(has);
        
    JS_DECLARE_NATIVE_FUNCTION(set);
        
    JS_DECLARE_NATIVE_FUNCTION(set0);

    JS_DECLARE_NATIVE_FUNCTION(set1);

    JS_DECLARE_NATIVE_FUNCTION(entries);
    JS_DECLARE_NATIVE_FUNCTION(for_each);
    JS_DECLARE_NATIVE_FUNCTION(keys);
    JS_DECLARE_NATIVE_FUNCTION(values);
        

};


} // namespace Web::Bindings
    