
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSKeywordValuePrototype : public JS::Object {
    JS_OBJECT(CSSKeywordValuePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSKeywordValuePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSKeywordValuePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSKeywordValuePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(value_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_setter);


};


} // namespace Web::Bindings
    