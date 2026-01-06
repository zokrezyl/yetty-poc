"""
Yetty Python plugin initialization callbacks.

This module provides lifecycle hooks for the Python plugin system.
User scripts should not modify this file.
"""

import os

# Set WGPU_LIB_PATH BEFORE any wgpu/pygfx imports
# This ensures wgpu-py uses the same wgpu-native library as yetty
_yetty_wgpu_lib = os.environ.get('YETTY_WGPU_LIB_PATH')
if _yetty_wgpu_lib:
    os.environ['WGPU_LIB_PATH'] = _yetty_wgpu_lib

# Force offscreen mode for rendercanvas
os.environ['RENDERCANVAS_FORCE_OFFSCREEN'] = '1'

import yetty_wgpu
import yetty

# Global state
_plugin_initialized = False
_current_layer_id = 0


def init_plugin():
    """
    Called once when the Python plugin is loaded.
    
    This is the place for global initialization that should happen
    once per plugin, not per layer.
    """
    global _plugin_initialized
    
    if _plugin_initialized:
        return
    
    print("[yetty] Python plugin initializing...")
    
    # Any global setup goes here
    
    _plugin_initialized = True
    print("[yetty] Python plugin initialized")


def init_layer(ctx, width, height):
    """
    Called when a new Python layer is created.
    
    This is called AFTER the user script runs and registers a @yetty.layer class.
    
    Args:
        ctx: Dictionary with WebGPU context info:
            - 'device': WGPUDevice handle (as int)
            - 'queue': WGPUQueue handle (as int)
        width: Layer width in pixels
        height: Layer height in pixels
    """
    global _current_layer_id
    
    print(f"[yetty] init_layer called: {width}x{height}")
    
    # Generate layer ID
    _current_layer_id += 1
    layer_id = _current_layer_id
    
    # Create layer instance from registered class
    yetty._create_layer_instance(layer_id)
    
    # Initialize the layer
    yetty._init_layer(
        layer_id,
        ctx['device'],
        ctx['queue'],
        width,
        height
    )
    
    print(f"[yetty] Layer {layer_id} initialized: {width}x{height}")
    return layer_id


def render_layer(layer_id, frame, width, height, render_pass_handle):
    """
    Called every frame to render a layer.
    
    Args:
        layer_id: The layer ID returned from init_layer
        frame: Frame counter
        width: Current width
        height: Current height
        render_pass_handle: Raw WGPURenderPassEncoder handle from yetty
    """
    return yetty._render_layer(layer_id, frame, width, height, render_pass_handle)


def dispose_layer(layer_id):
    """
    Called when a Python layer is destroyed.
    
    Args:
        layer_id: The layer ID to destroy
    """
    print(f"[yetty] Disposing layer {layer_id}...")
    
    yetty._destroy_layer(layer_id)
    
    # Cleanup yetty_wgpu resources if no more layers
    if not yetty._layer_instances:
        yetty_wgpu.cleanup()
    
    print(f"[yetty] Layer {layer_id} disposed")


def dispose_plugin():
    """
    Called when the Python plugin is unloaded (future feature).
    
    This is a placeholder for future implementation.
    """
    global _plugin_initialized
    
    print("[yetty] Python plugin disposing...")
    
    # Destroy all remaining layers
    for layer_id in list(yetty._layer_instances.keys()):
        dispose_layer(layer_id)
    
    _plugin_initialized = False
    print("[yetty] Python plugin disposed")
