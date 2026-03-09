
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLTableRowElementPrototype : public JS::Object {
    JS_OBJECT(HTMLTableRowElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLTableRowElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLTableRowElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLTableRowElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(insert_cell);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_cell);
        
    JS_DECLARE_NATIVE_FUNCTION(row_index_getter);

    JS_DECLARE_NATIVE_FUNCTION(section_row_index_getter);

    JS_DECLARE_NATIVE_FUNCTION(cells_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_setter);

    JS_DECLARE_NATIVE_FUNCTION(ch_getter);

    JS_DECLARE_NATIVE_FUNCTION(ch_setter);

    JS_DECLARE_NATIVE_FUNCTION(ch_off_getter);

    JS_DECLARE_NATIVE_FUNCTION(ch_off_setter);

    JS_DECLARE_NATIVE_FUNCTION(v_align_getter);

    JS_DECLARE_NATIVE_FUNCTION(v_align_setter);

    JS_DECLARE_NATIVE_FUNCTION(bg_color_getter);

    JS_DECLARE_NATIVE_FUNCTION(bg_color_setter);


};


} // namespace Web::Bindings
    