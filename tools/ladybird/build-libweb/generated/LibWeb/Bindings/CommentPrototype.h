
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CommentPrototype : public JS::Object {
    JS_OBJECT(CommentPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CommentPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CommentPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CommentPrototype() override;
private:


};


} // namespace Web::Bindings
    