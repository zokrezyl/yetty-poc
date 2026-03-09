
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class FontFaceSetPrototype : public JS::Object {
    JS_OBJECT(FontFaceSetPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(FontFaceSetPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit FontFaceSetPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~FontFaceSetPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(add);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_);
        
    JS_DECLARE_NATIVE_FUNCTION(clear);
        
    JS_DECLARE_NATIVE_FUNCTION(load);
        
    JS_DECLARE_NATIVE_FUNCTION(get_size);
    JS_DECLARE_NATIVE_FUNCTION(entries);
    JS_DECLARE_NATIVE_FUNCTION(values);
    JS_DECLARE_NATIVE_FUNCTION(for_each);
    JS_DECLARE_NATIVE_FUNCTION(has);

    JS_DECLARE_NATIVE_FUNCTION(onloading_getter);

    JS_DECLARE_NATIVE_FUNCTION(onloading_setter);

    JS_DECLARE_NATIVE_FUNCTION(onloadingdone_getter);

    JS_DECLARE_NATIVE_FUNCTION(onloadingdone_setter);

    JS_DECLARE_NATIVE_FUNCTION(onloadingerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onloadingerror_setter);

    JS_DECLARE_NATIVE_FUNCTION(ready_getter);

    JS_DECLARE_NATIVE_FUNCTION(status_getter);


};


enum class FontFaceSetLoadStatus : u8 {

    Loading,

    Loaded,

};

inline String idl_enum_to_string(FontFaceSetLoadStatus value)
{
    switch (value) {

    case FontFaceSetLoadStatus::Loading:
        return "loading"_string;

    case FontFaceSetLoadStatus::Loaded:
        return "loaded"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    