
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MediaQueryListEventPrototype : public JS::Object {
    JS_OBJECT(MediaQueryListEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MediaQueryListEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MediaQueryListEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MediaQueryListEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(media_getter);

    JS_DECLARE_NATIVE_FUNCTION(matches_getter);


};


} // namespace Web::Bindings
    