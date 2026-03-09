
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MediaListPrototype : public JS::Object {
    JS_OBJECT(MediaListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MediaListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MediaListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MediaListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(item);
        
    JS_DECLARE_NATIVE_FUNCTION(append_medium);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_medium);
        
    JS_DECLARE_NATIVE_FUNCTION(to_string);
        
    JS_DECLARE_NATIVE_FUNCTION(media_text_getter);

    JS_DECLARE_NATIVE_FUNCTION(media_text_setter);

    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    