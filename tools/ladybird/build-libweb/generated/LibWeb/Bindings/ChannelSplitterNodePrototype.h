
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ChannelSplitterNodePrototype : public JS::Object {
    JS_OBJECT(ChannelSplitterNodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ChannelSplitterNodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ChannelSplitterNodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ChannelSplitterNodePrototype() override;
private:


};


} // namespace Web::Bindings
    