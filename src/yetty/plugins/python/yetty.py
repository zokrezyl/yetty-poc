"""
yetty - Python plugin decorator for yetty terminal.

Usage:
    import yetty

    @yetty.layer
    class MyLayer:
        def init(self, ctx):
            '''Called once when layer is created. ctx contains WebGPU handles.'''
            pass
            
        def render(self, frame, width, height):
            '''Called every frame. Return the texture to display.'''
            pass
            
        def destroy(self):
            '''Called when layer is destroyed.'''
            pass
"""

import yetty_wgpu

# Registry of layer classes and instances
_layer_classes = {}  # name -> class
_layer_instances = {}  # id -> instance
_current_layer_id = None


class LayerContext:
    """Context passed to layer init() method."""
    
    def __init__(self, device_handle, queue_handle, width, height):
        self.device_handle = device_handle
        self.queue_handle = queue_handle
        self.width = width
        self.height = height
        self._texture = None
        self._texture_view = None
    
    @property
    def device(self):
        """Get the WGPUDevice handle."""
        return self.device_handle
    
    @property
    def queue(self):
        """Get the WGPUQueue handle."""
        return self.queue_handle
    
    def get_render_texture(self):
        """Get or create the render texture for this layer."""
        if self._texture is None:
            # Create texture via yetty_wgpu
            if not yetty_wgpu.create_render_texture(self.width, self.height):
                raise RuntimeError(f"Failed to create render texture {self.width}x{self.height}")
            self._texture = yetty_wgpu.get_render_texture_handle()
        return self._texture
    
    def get_render_texture_view(self):
        """Get the render texture view."""
        if self._texture is None:
            self.get_render_texture()
        return yetty_wgpu.get_render_texture_view_handle()


class RenderPass:
    """
    Wrapper around yetty's WGPURenderPassEncoder.
    
    This wraps the raw handle in a wgpu-py compatible GPURenderPassEncoder
    so pygfx can draw directly to yetty's render pass.
    """
    
    def __init__(self, handle, width, height):
        self._handle = handle
        self.width = width
        self.height = height
        self._wgpu_pass = None
    
    def get_wgpu_render_pass(self):
        """
        Get a wgpu-py GPURenderPassEncoder wrapping yetty's render pass.
        
        This allows pygfx to draw directly to yetty's render pass.
        """
        if self._wgpu_pass is None:
            self._wgpu_pass = _create_wgpu_render_pass(self._handle)
        return self._wgpu_pass
    
    @property
    def handle(self):
        """Raw WGPURenderPassEncoder handle."""
        return self._handle


def _create_wgpu_render_pass(handle):
    """
    Create a wgpu-py GPURenderPassEncoder from a raw handle.
    """
    import sys
    from wgpu.backends.wgpu_native import ffi
    from wgpu.backends.wgpu_native._api import GPURenderPassEncoder
    
    # Cast the integer handle to a cffi pointer
    raw_pass = ffi.cast('WGPURenderPassEncoder', handle)
    
    # Create a subclass that won't release the handle
    class YettyRenderPassEncoder(GPURenderPassEncoder):
        _release_function = None
        
        def __del__(self):
            pass
    
    encoder = YettyRenderPassEncoder.__new__(YettyRenderPassEncoder)
    encoder._internal = raw_pass
    encoder._device = None
    encoder._label = ""  # Required by wgpu-py
    
    return encoder


def layer(cls):
    """
    Decorator to register a class as a yetty layer.
    
    The class should implement:
        - init(self, ctx): Called once with LayerContext
        - render(self, render_pass, frame, width, height): Called every frame
          render_pass is a RenderPass object with yetty's WGPURenderPassEncoder
        - destroy(self): Called when layer is destroyed (optional)
    
    Example:
        @yetty.layer
        class MyPlot:
            def init(self, ctx):
                self.figure = create_figure(ctx)
            
            def render(self, render_pass, frame, width, height):
                # Get wgpu-py render pass for pygfx
                wgpu_pass = render_pass.get_wgpu_render_pass()
                # Draw directly to yetty's render pass
                self.figure.draw(wgpu_pass)
            
            def destroy(self):
                self.figure.close()
    """
    # Validate the class has required methods
    if not hasattr(cls, 'init'):
        raise TypeError(f"@yetty.layer class {cls.__name__} must have init(self, ctx) method")
    if not hasattr(cls, 'render'):
        raise TypeError(f"@yetty.layer class {cls.__name__} must have render(self, render_pass, frame, width, height) method")
    
    # Register the class
    name = cls.__name__
    _layer_classes[name] = cls
    print(f"[yetty] Registered layer class: {name}")
    
    # Return the class unchanged
    return cls


def _create_layer_instance(layer_id, class_name=None):
    """
    Create a new layer instance. Called by C++ plugin.
    
    Args:
        layer_id: Unique ID for this layer instance
        class_name: Name of the registered class (optional, uses first if None)
    
    Returns:
        True if successful
    """
    global _current_layer_id
    
    if class_name:
        if class_name not in _layer_classes:
            raise ValueError(f"Layer class '{class_name}' not registered. Use @yetty.layer decorator.")
        cls = _layer_classes[class_name]
    else:
        # Use the most recently registered class
        if not _layer_classes:
            raise ValueError("No layer classes registered. Use @yetty.layer decorator on a class.")
        cls = list(_layer_classes.values())[-1]
    
    # Create instance
    instance = cls()
    _layer_instances[layer_id] = instance
    _current_layer_id = layer_id
    
    print(f"[yetty] Created layer instance {layer_id} of class {cls.__name__}")
    return True


def _init_layer(layer_id, device_handle, queue_handle, width, height):
    """
    Initialize a layer instance. Called by C++ plugin.
    """
    global _current_layer_id
    
    if layer_id not in _layer_instances:
        raise ValueError(f"Layer {layer_id} not created")
    
    instance = _layer_instances[layer_id]
    _current_layer_id = layer_id
    
    # Set WebGPU handles
    yetty_wgpu.set_handles(device=device_handle, queue=queue_handle)
    
    # Create context
    ctx = LayerContext(device_handle, queue_handle, width, height)
    
    # Call user's init
    print(f"[yetty] Initializing layer {layer_id}")
    instance.init(ctx)
    
    # Store context on instance for later use
    instance._yetty_ctx = ctx
    
    return True


def _render_layer(layer_id, frame, width, height, render_pass_handle):
    """
    Render a layer. Called by C++ plugin every frame.
    
    Args:
        layer_id: Layer instance ID
        frame: Frame number
        width: Render width
        height: Render height
        render_pass_handle: Raw WGPURenderPassEncoder handle from yetty
    """
    global _current_layer_id
    
    if layer_id not in _layer_instances:
        return False
    
    instance = _layer_instances[layer_id]
    _current_layer_id = layer_id
    
    # Create a RenderPass wrapper for the user
    render_pass = RenderPass(render_pass_handle, width, height)
    
    # Call user's render with the render pass
    instance.render(render_pass, frame, width, height)
    return True


def _destroy_layer(layer_id):
    """
    Destroy a layer instance. Called by C++ plugin.
    """
    global _current_layer_id
    
    if layer_id not in _layer_instances:
        return
    
    instance = _layer_instances[layer_id]
    
    # Call user's destroy if it exists
    if hasattr(instance, 'destroy'):
        print(f"[yetty] Destroying layer {layer_id}")
        instance.destroy()
    
    # Clean up
    del _layer_instances[layer_id]
    
    if _current_layer_id == layer_id:
        _current_layer_id = None


def get_current_layer():
    """Get the currently active layer instance."""
    if _current_layer_id is None:
        return None
    return _layer_instances.get(_current_layer_id)


def get_layer(layer_id):
    """Get a layer instance by ID."""
    return _layer_instances.get(layer_id)


# Expose for C++ to call
__all__ = [
    'layer',
    'LayerContext',
    'RenderPass',
    'get_current_layer',
    'get_layer',
    '_create_layer_instance',
    '_init_layer', 
    '_render_layer',
    '_destroy_layer',
]
