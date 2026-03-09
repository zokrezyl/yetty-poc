
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DocumentTimelinePrototype : public JS::Object {
    JS_OBJECT(DocumentTimelinePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DocumentTimelinePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DocumentTimelinePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DocumentTimelinePrototype() override;
private:


};


} // namespace Web::Bindings
    