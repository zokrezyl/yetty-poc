
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AudioDestinationNodePrototype : public JS::Object {
    JS_OBJECT(AudioDestinationNodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AudioDestinationNodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AudioDestinationNodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AudioDestinationNodePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(max_channel_count_getter);


};


} // namespace Web::Bindings
    