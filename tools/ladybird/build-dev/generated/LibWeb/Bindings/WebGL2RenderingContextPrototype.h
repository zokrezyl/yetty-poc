
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGL2RenderingContextPrototype : public JS::Object {
    JS_OBJECT(WebGL2RenderingContextPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGL2RenderingContextPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGL2RenderingContextPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGL2RenderingContextPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(copy_buffer_sub_data);
        
    JS_DECLARE_NATIVE_FUNCTION(get_buffer_sub_data);
        
    JS_DECLARE_NATIVE_FUNCTION(blit_framebuffer);
        
    JS_DECLARE_NATIVE_FUNCTION(framebuffer_texture_layer);
        
    JS_DECLARE_NATIVE_FUNCTION(invalidate_framebuffer);
        
    JS_DECLARE_NATIVE_FUNCTION(invalidate_sub_framebuffer);
        
    JS_DECLARE_NATIVE_FUNCTION(read_buffer);
        
    JS_DECLARE_NATIVE_FUNCTION(get_internalformat_parameter);
        
    JS_DECLARE_NATIVE_FUNCTION(renderbuffer_storage_multisample);
        
    JS_DECLARE_NATIVE_FUNCTION(tex_storage2d);
        
    JS_DECLARE_NATIVE_FUNCTION(tex_storage3d);
        
    JS_DECLARE_NATIVE_FUNCTION(tex_image3d);
        
    JS_DECLARE_NATIVE_FUNCTION(tex_image3d0);

    JS_DECLARE_NATIVE_FUNCTION(tex_image3d1);

    JS_DECLARE_NATIVE_FUNCTION(tex_sub_image3d);
        
    JS_DECLARE_NATIVE_FUNCTION(compressed_tex_image3d);
        
    JS_DECLARE_NATIVE_FUNCTION(compressed_tex_sub_image3d);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform1ui);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform2ui);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform3ui);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform4ui);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform1uiv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform2uiv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform3uiv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform4uiv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform_matrix3x2fv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform_matrix4x2fv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform_matrix2x3fv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform_matrix4x3fv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform_matrix2x4fv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform_matrix3x4fv);
        
    JS_DECLARE_NATIVE_FUNCTION(vertex_attrib_i4i);
        
    JS_DECLARE_NATIVE_FUNCTION(vertex_attrib_i4iv);
        
    JS_DECLARE_NATIVE_FUNCTION(vertex_attrib_i4ui);
        
    JS_DECLARE_NATIVE_FUNCTION(vertex_attrib_i4uiv);
        
    JS_DECLARE_NATIVE_FUNCTION(vertex_attrib_i_pointer);
        
    JS_DECLARE_NATIVE_FUNCTION(vertex_attrib_divisor);
        
    JS_DECLARE_NATIVE_FUNCTION(draw_arrays_instanced);
        
    JS_DECLARE_NATIVE_FUNCTION(draw_elements_instanced);
        
    JS_DECLARE_NATIVE_FUNCTION(draw_range_elements);
        
    JS_DECLARE_NATIVE_FUNCTION(draw_buffers);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_bufferfv);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_bufferiv);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_bufferuiv);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_bufferfi);
        
    JS_DECLARE_NATIVE_FUNCTION(create_query);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_query);
        
    JS_DECLARE_NATIVE_FUNCTION(begin_query);
        
    JS_DECLARE_NATIVE_FUNCTION(end_query);
        
    JS_DECLARE_NATIVE_FUNCTION(get_query);
        
    JS_DECLARE_NATIVE_FUNCTION(get_query_parameter);
        
    JS_DECLARE_NATIVE_FUNCTION(create_sampler);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_sampler);
        
    JS_DECLARE_NATIVE_FUNCTION(bind_sampler);
        
    JS_DECLARE_NATIVE_FUNCTION(sampler_parameteri);
        
    JS_DECLARE_NATIVE_FUNCTION(sampler_parameterf);
        
    JS_DECLARE_NATIVE_FUNCTION(fence_sync);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_sync);
        
    JS_DECLARE_NATIVE_FUNCTION(client_wait_sync);
        
    JS_DECLARE_NATIVE_FUNCTION(wait_sync);
        
    JS_DECLARE_NATIVE_FUNCTION(get_sync_parameter);
        
    JS_DECLARE_NATIVE_FUNCTION(create_transform_feedback);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_transform_feedback);
        
    JS_DECLARE_NATIVE_FUNCTION(bind_transform_feedback);
        
    JS_DECLARE_NATIVE_FUNCTION(begin_transform_feedback);
        
    JS_DECLARE_NATIVE_FUNCTION(end_transform_feedback);
        
    JS_DECLARE_NATIVE_FUNCTION(transform_feedback_varyings);
        
    JS_DECLARE_NATIVE_FUNCTION(pause_transform_feedback);
        
    JS_DECLARE_NATIVE_FUNCTION(resume_transform_feedback);
        
    JS_DECLARE_NATIVE_FUNCTION(bind_buffer_base);
        
    JS_DECLARE_NATIVE_FUNCTION(bind_buffer_range);
        
    JS_DECLARE_NATIVE_FUNCTION(get_uniform_indices);
        
    JS_DECLARE_NATIVE_FUNCTION(get_active_uniforms);
        
    JS_DECLARE_NATIVE_FUNCTION(get_uniform_block_index);
        
    JS_DECLARE_NATIVE_FUNCTION(get_active_uniform_block_parameter);
        
    JS_DECLARE_NATIVE_FUNCTION(get_active_uniform_block_name);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform_block_binding);
        
    JS_DECLARE_NATIVE_FUNCTION(create_vertex_array);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_vertex_array);
        
    JS_DECLARE_NATIVE_FUNCTION(is_vertex_array);
        
    JS_DECLARE_NATIVE_FUNCTION(bind_vertex_array);
        
    JS_DECLARE_NATIVE_FUNCTION(buffer_data);
        
    JS_DECLARE_NATIVE_FUNCTION(buffer_data0);

    JS_DECLARE_NATIVE_FUNCTION(buffer_data1);

    JS_DECLARE_NATIVE_FUNCTION(buffer_data2);

    JS_DECLARE_NATIVE_FUNCTION(buffer_sub_data);
        
    JS_DECLARE_NATIVE_FUNCTION(buffer_sub_data0);

    JS_DECLARE_NATIVE_FUNCTION(buffer_sub_data1);

    JS_DECLARE_NATIVE_FUNCTION(tex_image2d);
        
    JS_DECLARE_NATIVE_FUNCTION(tex_image2d0);

    JS_DECLARE_NATIVE_FUNCTION(tex_image2d1);

    JS_DECLARE_NATIVE_FUNCTION(tex_image2d2);

    JS_DECLARE_NATIVE_FUNCTION(tex_image2d3);

    JS_DECLARE_NATIVE_FUNCTION(tex_sub_image2d);
        
    JS_DECLARE_NATIVE_FUNCTION(tex_sub_image2d0);

    JS_DECLARE_NATIVE_FUNCTION(tex_sub_image2d1);

    JS_DECLARE_NATIVE_FUNCTION(tex_sub_image2d2);

    JS_DECLARE_NATIVE_FUNCTION(tex_sub_image2d3);

    JS_DECLARE_NATIVE_FUNCTION(compressed_tex_image2d);
        
    JS_DECLARE_NATIVE_FUNCTION(compressed_tex_sub_image2d);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform1fv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform2fv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform3fv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform4fv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform1iv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform2iv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform3iv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform4iv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform_matrix2fv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform_matrix3fv);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform_matrix4fv);
        
    JS_DECLARE_NATIVE_FUNCTION(read_pixels);
        
    JS_DECLARE_NATIVE_FUNCTION(read_pixels0);

    JS_DECLARE_NATIVE_FUNCTION(read_pixels1);

    JS_DECLARE_NATIVE_FUNCTION(get_context_attributes);
        
    JS_DECLARE_NATIVE_FUNCTION(is_context_lost);
        
    JS_DECLARE_NATIVE_FUNCTION(get_supported_extensions);
        
    JS_DECLARE_NATIVE_FUNCTION(get_extension);
        
    JS_DECLARE_NATIVE_FUNCTION(active_texture);
        
    JS_DECLARE_NATIVE_FUNCTION(attach_shader);
        
    JS_DECLARE_NATIVE_FUNCTION(bind_attrib_location);
        
    JS_DECLARE_NATIVE_FUNCTION(bind_buffer);
        
    JS_DECLARE_NATIVE_FUNCTION(bind_framebuffer);
        
    JS_DECLARE_NATIVE_FUNCTION(bind_renderbuffer);
        
    JS_DECLARE_NATIVE_FUNCTION(bind_texture);
        
    JS_DECLARE_NATIVE_FUNCTION(blend_color);
        
    JS_DECLARE_NATIVE_FUNCTION(blend_equation);
        
    JS_DECLARE_NATIVE_FUNCTION(blend_equation_separate);
        
    JS_DECLARE_NATIVE_FUNCTION(blend_func);
        
    JS_DECLARE_NATIVE_FUNCTION(blend_func_separate);
        
    JS_DECLARE_NATIVE_FUNCTION(check_framebuffer_status);
        
    JS_DECLARE_NATIVE_FUNCTION(clear);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_color);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_depth);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_stencil);
        
    JS_DECLARE_NATIVE_FUNCTION(color_mask);
        
    JS_DECLARE_NATIVE_FUNCTION(compile_shader);
        
    JS_DECLARE_NATIVE_FUNCTION(copy_tex_image2d);
        
    JS_DECLARE_NATIVE_FUNCTION(copy_tex_sub_image2d);
        
    JS_DECLARE_NATIVE_FUNCTION(create_buffer);
        
    JS_DECLARE_NATIVE_FUNCTION(create_framebuffer);
        
    JS_DECLARE_NATIVE_FUNCTION(create_program);
        
    JS_DECLARE_NATIVE_FUNCTION(create_renderbuffer);
        
    JS_DECLARE_NATIVE_FUNCTION(create_shader);
        
    JS_DECLARE_NATIVE_FUNCTION(create_texture);
        
    JS_DECLARE_NATIVE_FUNCTION(cull_face);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_buffer);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_framebuffer);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_program);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_renderbuffer);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_shader);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_texture);
        
    JS_DECLARE_NATIVE_FUNCTION(depth_func);
        
    JS_DECLARE_NATIVE_FUNCTION(depth_mask);
        
    JS_DECLARE_NATIVE_FUNCTION(depth_range);
        
    JS_DECLARE_NATIVE_FUNCTION(detach_shader);
        
    JS_DECLARE_NATIVE_FUNCTION(disable);
        
    JS_DECLARE_NATIVE_FUNCTION(disable_vertex_attrib_array);
        
    JS_DECLARE_NATIVE_FUNCTION(draw_arrays);
        
    JS_DECLARE_NATIVE_FUNCTION(draw_elements);
        
    JS_DECLARE_NATIVE_FUNCTION(enable);
        
    JS_DECLARE_NATIVE_FUNCTION(enable_vertex_attrib_array);
        
    JS_DECLARE_NATIVE_FUNCTION(finish);
        
    JS_DECLARE_NATIVE_FUNCTION(flush);
        
    JS_DECLARE_NATIVE_FUNCTION(framebuffer_renderbuffer);
        
    JS_DECLARE_NATIVE_FUNCTION(framebuffer_texture2d);
        
    JS_DECLARE_NATIVE_FUNCTION(front_face);
        
    JS_DECLARE_NATIVE_FUNCTION(generate_mipmap);
        
    JS_DECLARE_NATIVE_FUNCTION(get_active_attrib);
        
    JS_DECLARE_NATIVE_FUNCTION(get_active_uniform);
        
    JS_DECLARE_NATIVE_FUNCTION(get_attached_shaders);
        
    JS_DECLARE_NATIVE_FUNCTION(get_attrib_location);
        
    JS_DECLARE_NATIVE_FUNCTION(get_buffer_parameter);
        
    JS_DECLARE_NATIVE_FUNCTION(get_parameter);
        
    JS_DECLARE_NATIVE_FUNCTION(get_error);
        
    JS_DECLARE_NATIVE_FUNCTION(get_program_parameter);
        
    JS_DECLARE_NATIVE_FUNCTION(get_program_info_log);
        
    JS_DECLARE_NATIVE_FUNCTION(get_renderbuffer_parameter);
        
    JS_DECLARE_NATIVE_FUNCTION(get_shader_parameter);
        
    JS_DECLARE_NATIVE_FUNCTION(get_shader_precision_format);
        
    JS_DECLARE_NATIVE_FUNCTION(get_shader_info_log);
        
    JS_DECLARE_NATIVE_FUNCTION(get_shader_source);
        
    JS_DECLARE_NATIVE_FUNCTION(get_tex_parameter);
        
    JS_DECLARE_NATIVE_FUNCTION(get_uniform);
        
    JS_DECLARE_NATIVE_FUNCTION(get_uniform_location);
        
    JS_DECLARE_NATIVE_FUNCTION(get_vertex_attrib);
        
    JS_DECLARE_NATIVE_FUNCTION(get_vertex_attrib_offset);
        
    JS_DECLARE_NATIVE_FUNCTION(hint);
        
    JS_DECLARE_NATIVE_FUNCTION(is_buffer);
        
    JS_DECLARE_NATIVE_FUNCTION(is_enabled);
        
    JS_DECLARE_NATIVE_FUNCTION(is_framebuffer);
        
    JS_DECLARE_NATIVE_FUNCTION(is_program);
        
    JS_DECLARE_NATIVE_FUNCTION(is_renderbuffer);
        
    JS_DECLARE_NATIVE_FUNCTION(is_shader);
        
    JS_DECLARE_NATIVE_FUNCTION(is_texture);
        
    JS_DECLARE_NATIVE_FUNCTION(line_width);
        
    JS_DECLARE_NATIVE_FUNCTION(link_program);
        
    JS_DECLARE_NATIVE_FUNCTION(pixel_storei);
        
    JS_DECLARE_NATIVE_FUNCTION(polygon_offset);
        
    JS_DECLARE_NATIVE_FUNCTION(renderbuffer_storage);
        
    JS_DECLARE_NATIVE_FUNCTION(sample_coverage);
        
    JS_DECLARE_NATIVE_FUNCTION(scissor);
        
    JS_DECLARE_NATIVE_FUNCTION(shader_source);
        
    JS_DECLARE_NATIVE_FUNCTION(stencil_func);
        
    JS_DECLARE_NATIVE_FUNCTION(stencil_func_separate);
        
    JS_DECLARE_NATIVE_FUNCTION(stencil_mask);
        
    JS_DECLARE_NATIVE_FUNCTION(stencil_mask_separate);
        
    JS_DECLARE_NATIVE_FUNCTION(stencil_op);
        
    JS_DECLARE_NATIVE_FUNCTION(stencil_op_separate);
        
    JS_DECLARE_NATIVE_FUNCTION(tex_parameterf);
        
    JS_DECLARE_NATIVE_FUNCTION(tex_parameteri);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform1f);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform2f);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform3f);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform4f);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform1i);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform2i);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform3i);
        
    JS_DECLARE_NATIVE_FUNCTION(uniform4i);
        
    JS_DECLARE_NATIVE_FUNCTION(use_program);
        
    JS_DECLARE_NATIVE_FUNCTION(validate_program);
        
    JS_DECLARE_NATIVE_FUNCTION(vertex_attrib1f);
        
    JS_DECLARE_NATIVE_FUNCTION(vertex_attrib2f);
        
    JS_DECLARE_NATIVE_FUNCTION(vertex_attrib3f);
        
    JS_DECLARE_NATIVE_FUNCTION(vertex_attrib4f);
        
    JS_DECLARE_NATIVE_FUNCTION(vertex_attrib1fv);
        
    JS_DECLARE_NATIVE_FUNCTION(vertex_attrib2fv);
        
    JS_DECLARE_NATIVE_FUNCTION(vertex_attrib3fv);
        
    JS_DECLARE_NATIVE_FUNCTION(vertex_attrib4fv);
        
    JS_DECLARE_NATIVE_FUNCTION(vertex_attrib_pointer);
        
    JS_DECLARE_NATIVE_FUNCTION(viewport);
        
    JS_DECLARE_NATIVE_FUNCTION(canvas_getter);

    JS_DECLARE_NATIVE_FUNCTION(drawing_buffer_width_getter);

    JS_DECLARE_NATIVE_FUNCTION(drawing_buffer_height_getter);


};


} // namespace Web::Bindings
    