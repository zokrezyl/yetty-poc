
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PerformanceResourceTimingPrototype : public JS::Object {
    JS_OBJECT(PerformanceResourceTimingPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PerformanceResourceTimingPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PerformanceResourceTimingPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PerformanceResourceTimingPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(to_json);
        
    JS_DECLARE_NATIVE_FUNCTION(initiator_type_getter);

    JS_DECLARE_NATIVE_FUNCTION(delivery_type_getter);

    JS_DECLARE_NATIVE_FUNCTION(next_hop_protocol_getter);

    JS_DECLARE_NATIVE_FUNCTION(worker_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(redirect_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(redirect_end_getter);

    JS_DECLARE_NATIVE_FUNCTION(fetch_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(domain_lookup_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(domain_lookup_end_getter);

    JS_DECLARE_NATIVE_FUNCTION(connect_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(connect_end_getter);

    JS_DECLARE_NATIVE_FUNCTION(secure_connection_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(request_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(final_response_headers_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(first_interim_response_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(response_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(response_end_getter);

    JS_DECLARE_NATIVE_FUNCTION(transfer_size_getter);

    JS_DECLARE_NATIVE_FUNCTION(encoded_body_size_getter);

    JS_DECLARE_NATIVE_FUNCTION(decoded_body_size_getter);

    JS_DECLARE_NATIVE_FUNCTION(response_status_getter);

    JS_DECLARE_NATIVE_FUNCTION(render_blocking_status_getter);

    JS_DECLARE_NATIVE_FUNCTION(content_type_getter);


};


enum class RenderBlockingStatusType : u8 {

    Blocking,

    NonBlocking,

};

inline String idl_enum_to_string(RenderBlockingStatusType value)
{
    switch (value) {

    case RenderBlockingStatusType::Blocking:
        return "blocking"_string;

    case RenderBlockingStatusType::NonBlocking:
        return "non-blocking"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    