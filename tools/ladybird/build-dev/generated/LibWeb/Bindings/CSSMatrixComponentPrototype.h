
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSMatrixComponentPrototype : public JS::Object {
    JS_OBJECT(CSSMatrixComponentPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSMatrixComponentPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSMatrixComponentPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSMatrixComponentPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(matrix_getter);

    JS_DECLARE_NATIVE_FUNCTION(matrix_setter);


};


} // namespace Web::Bindings
    