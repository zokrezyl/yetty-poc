
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSFontFeatureValuesMapPrototype : public JS::Object {
    JS_OBJECT(CSSFontFeatureValuesMapPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSFontFeatureValuesMapPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSFontFeatureValuesMapPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSFontFeatureValuesMapPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(set);
        
    JS_DECLARE_NATIVE_FUNCTION(get_size);
    JS_DECLARE_NATIVE_FUNCTION(entries);
    JS_DECLARE_NATIVE_FUNCTION(keys);
    JS_DECLARE_NATIVE_FUNCTION(values);
    JS_DECLARE_NATIVE_FUNCTION(for_each);
    JS_DECLARE_NATIVE_FUNCTION(get);
    JS_DECLARE_NATIVE_FUNCTION(has);
    JS_DECLARE_NATIVE_FUNCTION(delete_);
    JS_DECLARE_NATIVE_FUNCTION(clear);


};


} // namespace Web::Bindings
    