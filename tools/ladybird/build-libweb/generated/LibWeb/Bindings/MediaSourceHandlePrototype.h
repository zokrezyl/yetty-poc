
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MediaSourceHandlePrototype : public JS::Object {
    JS_OBJECT(MediaSourceHandlePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MediaSourceHandlePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MediaSourceHandlePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MediaSourceHandlePrototype() override;
private:


};


} // namespace Web::Bindings
    