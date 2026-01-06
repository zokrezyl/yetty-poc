"""pygfx demo - orange box rendering to yetty's render pass."""
import sys
import yetty
import pygfx

@yetty.layer
class OrangeBoxDemo:
    def init(self, ctx):
        from yetty_pygfx import PygfxRenderer
        self.renderer = PygfxRenderer(ctx)
        
        self.scene = pygfx.Scene()
        self.camera = pygfx.OrthographicCamera(4, 4)
        self.camera.local.position = (0, 0, 5)
        
        geometry = pygfx.box_geometry(3, 3, 0.1)
        material = pygfx.MeshBasicMaterial(color=(1.0, 0.5, 0.0))
        self.box = pygfx.Mesh(geometry, material)
        self.scene.add(self.box)
        
        sys.stderr.write("[Demo] Orange box scene created\n")
        sys.stderr.flush()
    
    def render(self, render_pass, frame, width, height):
        self.renderer.render(render_pass, self.scene, self.camera)
