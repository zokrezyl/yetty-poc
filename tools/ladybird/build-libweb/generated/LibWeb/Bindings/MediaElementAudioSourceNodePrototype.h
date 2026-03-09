
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MediaElementAudioSourceNodePrototype : public JS::Object {
    JS_OBJECT(MediaElementAudioSourceNodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MediaElementAudioSourceNodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MediaElementAudioSourceNodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MediaElementAudioSourceNodePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(media_element_getter);


};


} // namespace Web::Bindings
    