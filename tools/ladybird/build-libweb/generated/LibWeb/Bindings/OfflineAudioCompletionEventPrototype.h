
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class OfflineAudioCompletionEventPrototype : public JS::Object {
    JS_OBJECT(OfflineAudioCompletionEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(OfflineAudioCompletionEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit OfflineAudioCompletionEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~OfflineAudioCompletionEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(rendered_buffer_getter);


};


} // namespace Web::Bindings
    