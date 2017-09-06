import os

import bpy
import bmesh

from .fractal_cpp.fractal import x


def _create_fractal(self, _context):
    if self.grammar_path == "":
        return

    verts, edges = x(self.grammar_path, self.iteration)

    profile_mesh = bpy.data.meshes.new("FractalMesh")
    profile_mesh.from_pydata(verts, edges, [])
    profile_mesh.update()
    profile_object = bpy.data.objects.new("Fractal", profile_mesh)
    profile_object.data = profile_mesh

    scene = bpy.context.scene
    scene.objects.link(profile_object)
    profile_object.select = True

    # bpy.context.scene.objects.active = profile_object
    # bpy.ops.object.editmode_toogle()
    # bpy.ops.mesh.select_all(action='SELECT')
    # bpy.ops.mesh.remove_doubles()
    # bpy.ops.object.editmode_toogle()

    bm = bmesh.new()
    bm.from_mesh(profile_mesh)
    bmesh.ops.remove_doubles(bm, verts=bm.verts, dist=0.001)
    bm.to_mesh(profile_mesh)
    profile_mesh.update()

    bpy.context.window_manager.progress_end()


class Fractal_add_object(bpy.types.Operator):
    """Create a new Fractal"""
    bl_idname = "mesh.add_fractal"
    bl_label = "Add Fracal"
    bl_options = {'REGISTER', 'UNDO'}

    iteration = bpy.props.IntProperty(
        name="Iteration Count",
        default=2,
        min=1,
        soft_min=1,
        soft_max=7,
        subtype='UNSIGNED',
        description="Number of iterations of the fractal")

    standard_path = os.path.join(os.path.dirname(os.path.realpath(__file__)),
                                 "examples", "standard", "sierpinski.txt")

    def reset_iteration(self, _context):
        self.iteration = 2

    grammar_path = bpy.props.StringProperty(
        name="Grammar path",
        default=standard_path,
        description="The grammar for the fractal you want to draw",
        subtype='FILE_PATH',
        update=reset_iteration
    )

    def execute(self, context):

        _create_fractal(self, context)

        return {'FINISHED'}
