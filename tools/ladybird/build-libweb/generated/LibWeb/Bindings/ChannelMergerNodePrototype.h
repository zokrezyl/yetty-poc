
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ChannelMergerNodePrototype : public JS::Object {
    JS_OBJECT(ChannelMergerNodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ChannelMergerNodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ChannelMergerNodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ChannelMergerNodePrototype() override;
private:


};


} // namespace Web::Bindings
    