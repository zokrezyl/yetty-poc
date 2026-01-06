"""
yetty_pygfx - Direct rendering to yetty's render pass
"""
import os
import sys

os.environ['RENDERCANVAS_FORCE_OFFSCREEN'] = '1'

import wgpu
from wgpu.backends.wgpu_native import ffi, lib


class PygfxRenderer:
    def __init__(self, ctx):
        self.ctx = ctx
        self._setup_pygfx()
    
    def _setup_pygfx(self):
        from wgpu.backends.wgpu_native._api import GPUDevice, GPUQueue
        from pygfx.renderers.wgpu.engine.shared import Shared
        
        device_ptr = ffi.cast('WGPUDevice', self.ctx.device_handle)
        queue_ptr = ffi.cast('WGPUQueue', self.ctx.queue_handle)
        
        class YettyQueue(GPUQueue):
            _release_function = None
        class YettyDevice(GPUDevice):
            _release_function = None
        
        queue = YettyQueue.__new__(YettyQueue)
        queue._internal = queue_ptr
        queue._device = None
        
        device = YettyDevice.__new__(YettyDevice)
        device._internal = device_ptr
        device._adapter = None
        device._features = frozenset(['float32-filterable'])
        device._limits = {}
        device._queue = queue
        device._poller = None
        queue._device = device
        
        self._device = device
        
        if Shared._instance:
            Shared._instance._device = device
            Shared._instance._queue = queue
        
        from pygfx.renderers.wgpu.engine.shared import get_shared
        self._shared = get_shared()
    
    def render(self, render_pass, scene, camera):
        from pygfx.renderers.wgpu.engine.renderer import FlatScene
        from pygfx.renderers.wgpu.engine.renderstate import get_renderstate
        from wgpu.backends.wgpu_native._api import GPURenderPassEncoder
        
        width = render_pass.width
        height = render_pass.height
        
        camera.set_view_size(width, height)
        
        # Wrap yetty's render pass
        class YettyRenderPass(GPURenderPassEncoder):
            _release_function = None
            def __del__(self): pass
        
        wgpu_pass = YettyRenderPass.__new__(YettyRenderPass)
        wgpu_pass._internal = ffi.cast('WGPURenderPassEncoder', render_pass.handle)
        wgpu_pass._device = self._device
        wgpu_pass._label = ""
        
        # Update uniforms
        stdinfo = self._shared.uniform_buffer.data
        stdinfo["cam_transform"] = camera.world.inverse_matrix.T
        stdinfo["cam_transform_inv"] = camera.world.matrix.T
        stdinfo["projection_transform"] = camera.projection_matrix.T
        stdinfo["projection_transform_inv"] = camera.projection_matrix_inverse.T
        stdinfo["physical_size"] = (width, height)
        stdinfo["logical_size"] = (width, height)
        stdinfo["ndc_offset"] = (1.0, 1.0, 0.0, 0.0)
        self._shared.uniform_buffer.update_full()
        
        # Force upload buffers NOW
        for resource in [self._shared.uniform_buffer]:
            if hasattr(resource, '_gfx_pending_uploads') and resource._gfx_pending_uploads:
                from pygfx.renderers.wgpu.engine.update import update_resource
                update_resource(resource, self._device)
        
        self._shared.pre_render_hook()
        
        # Collect scene
        view_matrix = camera.world.inverse_matrix
        flat = FlatScene(scene, view_matrix, 0)
        flat.sort()
        
        # Custom blender for bgra8unorm_srgb
        blender = _Blender(self._device)
        renderstate = get_renderstate(flat.lights, blender)
        
        flat.collect_pipelines_container_groups(renderstate)
        flat.call_bake_functions(camera, (width, height))
        
        # Draw using lib directly
        lib.wgpuRenderPassEncoderSetViewport(wgpu_pass._internal, 0, 0, float(width), float(height), 0.0, 1.0)
        lib.wgpuRenderPassEncoderSetScissorRect(wgpu_pass._internal, 0, 0, width, height)
        
        for pass_type, containers in flat.iter_render_pipelines_per_pass_type():
            for container in containers:
                if container.broken:
                    continue
                
                # Force update object's buffers
                for resource in container.wobject._wgpu_resources:
                    if hasattr(resource, '_gfx_pending_uploads') and resource._gfx_pending_uploads:
                        from pygfx.renderers.wgpu.engine.update import update_resource
                        update_resource(resource, self._device)
                
                pipeline = container.wgpu_pipeline
                indices = container.render_info["indices"]
                bind_groups = container.wgpu_bind_groups
                
                lib.wgpuRenderPassEncoderSetPipeline(wgpu_pass._internal, pipeline._internal)
                
                for bg_id, bg in enumerate(bind_groups):
                    lib.wgpuRenderPassEncoderSetBindGroup(wgpu_pass._internal, bg_id, bg._internal, 0, ffi.NULL)
                
                rs_bg_id = len(bind_groups)
                lib.wgpuRenderPassEncoderSetBindGroup(wgpu_pass._internal, rs_bg_id, renderstate.wgpu_bind_group._internal, 0, ffi.NULL)
                
                lib.wgpuRenderPassEncoderDraw(wgpu_pass._internal, indices[0], indices[1], indices[2], indices[3])


class _Blender:
    def __init__(self, device):
        self.device = device
        self.size = (0, 0)
        self._texture_info = {"color": {"name": "color", "format": wgpu.TextureFormat.bgra8unorm_srgb, "usage": wgpu.TextureUsage.RENDER_ATTACHMENT, "is_used": True, "clear": False}}
        self._hash = hash(("yetty", "bgra8unorm_srgb"))
    
    @property
    def hash(self):
        return self._hash
    
    def get_color_descriptors(self, material_pick_write, alpha_config):
        bf, bo = wgpu.BlendFactor, wgpu.BlendOperation
        return [{"format": wgpu.TextureFormat.bgra8unorm_srgb, "blend": {"color": {"src_factor": bf.src_alpha, "dst_factor": bf.one_minus_src_alpha, "operation": bo.add}, "alpha": {"src_factor": bf.one, "dst_factor": bf.one_minus_src_alpha, "operation": bo.add}}, "write_mask": wgpu.ColorWrite.ALL}]
    
    def get_depth_descriptor(self, depth_test, depth_compare, depth_write):
        return None
    
    def get_shader_kwargs(self, material_pick_write, alpha_config):
        return {"alpha_method": alpha_config.get("method", "opaque"), "fragment_output_code": "struct FragmentOutput { @location(0) color: vec4<f32>, }; fn apply_virtual_fields_of_fragment_output(outp: ptr<function,FragmentOutput>, stub: bool) { let color = (*outp).color; (*outp).color = vec4f(color.rgb, 1.0); }", "write_pick": False}
    
    def get_color_attachments(self, pass_type):
        return []
    
    def get_depth_attachment(self, pass_type=None):
        return None
    
    def ensure_target_size(self, size):
        self.size = size

__all__ = ['PygfxRenderer']
