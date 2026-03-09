
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AnalyserNodePrototype : public JS::Object {
    JS_OBJECT(AnalyserNodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AnalyserNodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AnalyserNodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AnalyserNodePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_float_frequency_data);
        
    JS_DECLARE_NATIVE_FUNCTION(get_byte_frequency_data);
        
    JS_DECLARE_NATIVE_FUNCTION(get_float_time_domain_data);
        
    JS_DECLARE_NATIVE_FUNCTION(get_byte_time_domain_data);
        
    JS_DECLARE_NATIVE_FUNCTION(fft_size_getter);

    JS_DECLARE_NATIVE_FUNCTION(fft_size_setter);

    JS_DECLARE_NATIVE_FUNCTION(frequency_bin_count_getter);

    JS_DECLARE_NATIVE_FUNCTION(min_decibels_getter);

    JS_DECLARE_NATIVE_FUNCTION(min_decibels_setter);

    JS_DECLARE_NATIVE_FUNCTION(max_decibels_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_decibels_setter);

    JS_DECLARE_NATIVE_FUNCTION(smoothing_time_constant_getter);

    JS_DECLARE_NATIVE_FUNCTION(smoothing_time_constant_setter);


};


} // namespace Web::Bindings
    