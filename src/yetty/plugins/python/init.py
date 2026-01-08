"""
Yetty Python plugin initialization callbacks.

This module provides lifecycle hooks for the Python plugin system.
User scripts should not modify this file.
"""

import yetty_wgpu

# Global state
_plugin_initialized = False


def init_plugin():
    """
    Called once when the Python plugin is loaded.
    
    This is the place for global initialization that should happen
    once per plugin, not per widget.
    """
    global _plugin_initialized
    
    if _plugin_initialized:
        return
    
    print("[yetty] Python plugin initializing...")
    
    # Any global setup goes here
    
    _plugin_initialized = True
    print("[yetty] Python plugin initialized")


def init_widget(ctx, width, height):
    """
    Called when a new Python widget is created.
    
    This is called BEFORE the user script runs, so WebGPU resources
    are available to the user script.
    
    Args:
        ctx: Dictionary with WebGPU context info:
            - 'device': WGPUDevice handle (as int)
            - 'queue': WGPUQueue handle (as int)
            - 'width': Render target width
            - 'height': Render target height
        width: Widget width in pixels
        height: Widget height in pixels
    """
    print(f"[yetty] Initializing widget: {width}x{height}")
    
    # Set WebGPU handles for yetty_wgpu module
    yetty_wgpu.set_handles(
        device=ctx['device'],
        queue=ctx['queue']
    )
    
    # Create render texture for this widget
    if not yetty_wgpu.create_render_texture(width, height):
        raise RuntimeError(f"Failed to create render texture {width}x{height}")
    
    print(f"[yetty] Widget initialized: {width}x{height}")


def dispose_widget():
    """
    Called when a Python widget is destroyed.
    
    Clean up any widget-specific resources here.
    """
    print("[yetty] Disposing widget...")
    
    # Cleanup yetty_wgpu resources
    yetty_wgpu.cleanup()
    
    print("[yetty] Widget disposed")


def dispose_plugin():
    """
    Called when the Python plugin is unloaded (future feature).
    
    This is a placeholder for future implementation.
    """
    global _plugin_initialized
    
    print("[yetty] Python plugin disposing...")
    
    # Global cleanup goes here
    
    _plugin_initialized = False
    print("[yetty] Python plugin disposed")
